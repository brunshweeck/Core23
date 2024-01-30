//
// Created by T.N.Brunshweeck on 01/01/2024.
//

#include "NTFS.h"
#include <core/private/Unsafe.h>
#include <core/InternalError.h>
#include <core/util/StringTokenizer.h>
#include <core/util/LinkedList.h>
#include <core/io/FileArray.h>
#include <core/io/private/NativeBuffer.h>
#include <core/io/IOException.h>
#include <core/time/LocalDateTime.h>
#include <core/time/ZoneOffset.h>
#include "../native/WindowsException.h"
#include <core/io/File.h>
#include <Windows.h>
#include <UserEnv.h>
#include <AccCtrl.h>
#include <AclAPI.h>
//#include <ShellAPI.h>
//#include <ObjBase.h>
#include <ShlObj.h>
//#include <ShObjIdl.h>
#include <initguid.h>
#include <io.h>
#include <lm.h>

#undef max
#undef min

enum Constant : gshort {
    EXTENDED_MAX_PATH = 32767
};

namespace core {
    namespace io {

        using namespace native;
        using namespace util;
        using namespace time;

        util::ArrayList<File> NTFS::ExitHook{512};

        namespace {
            enum class PathType : gbyte {
                PT_ABSOLUTE,                   //  C:\foo
                PT_UNC,                        //  \\server\share\foo
                PT_RELATIVE,                   //  foo
                PT_DIRECTORY_RELATIVE,         //  \foo
                PT_DRIVE_RELATIVE,             //  C:foo
                PT_INVALID
            };

            CORE_FAST gbool isSlash(gchar ch) {
                return (ch == '\\') || (ch == '/');
            }

            CORE_FAST gbool isLetter(gchar ch) {
                return ('A' <= ch && ch <= 'Z') || ('a' <= ch && ch <= 'z');
            }

            int nextNonSlash(const String &path, int off, int end) {
                while (off < end && isSlash(path.charAt(off))) {
                    off++;
                }
                return off;
            }

            gbool isInvalidPathChar(gchar ch) {
                return ch < '\u0020' ||
                       ch == '<' || ch == '>' ||
                       ch == ':' || ch == '\"' || ch == '|' || ch == '*' || ch == '?';
            }

            int nextSlash(const String &path, int off, int end) {
                gchar c = 0;
                while (off < end && !isSlash(c = path.charAt(off))) {
                    if (isInvalidPathChar(c)) {
                        return -1;
                    }
                    off++;
                }
                return off;
            }

            class ParseResult CORE_FINAL : public Object {
            public:
                PathType type = {};
                String root;
                String path;

                CORE_FAST ParseResult() : type(PathType::PT_INVALID) {}

                CORE_EXPLICIT ParseResult(PathType type, const String &root, const String &path) :
                        type(type), root(root), path(path) {}

                Object &clone() const override {
                    return Unsafe::allocateInstance<ParseResult>(*this);
                }

                gbool equals(const Object &obj) const override {
                    if (this == &obj) {
                        return true;
                    }
                    if (!Class<ParseResult>::hasInstance(obj))
                        return false;
                    ParseResult const &result = (const ParseResult &) obj;
                    if (result.type == PathType::PT_INVALID)
                        return type == PathType::PT_INVALID;
                    return result.type == type && root == result.root && path == result.path;
                }

                String toString() const override {
                    if (type == PathType::PT_INVALID)
                        return "ParseResult[Error]";
                    return "ParseResult[root=" + root + ", path=" + path + "]";
                }
            };

            String normalizePath(StringBuffer &sb, const String &path, gint off) {
                int const len = path.length();
                off = nextNonSlash(path, off, len);
                int start = off;
                gchar lastC = 0;
                while (off < len) {
                    gchar const c = path.charAt(off);
                    if (isSlash(c)) {
                        if (lastC == ' ') {
                            // Trailing char is diskSpace character
                            return {};
                        }
                        sb.append(path, start, off);
                        off = nextNonSlash(path, off, len);
                        if (off != len)   //no slash at the end of normalized path
                            sb.append('\\');
                        start = off;
                    } else {
                        if (isInvalidPathChar(c)) {
                            // Invalid char in pathname
                            return {};
                        }
                        lastC = c;
                        off++;
                    }
                }
                if (start != off) {
                    if (lastC == ' ') {
                        // Trailing char is diskSpace character
                        return {};
                    }
                    sb.append(path, start, off);
                }
                return sb.toString();
            }

            ParseResult parsePath(const String &path, gbool requireNormalization) {
                String root = {};
                PathType type = PathType::PT_INVALID;
                String const &input = path;

                int const len = input.length();
                int off = 0;
                if (len > 1) {
                    gchar const c0 = input.charAt(0);
                    gchar const c1 = input.charAt(1);
                    gchar const c = 0;
                    int next = 2;
                    if (isSlash(c0) && isSlash(c1)) {
                        // UNC: We keep the first two slashes, collapse all the
                        // following, then take the hostname and share name out,
                        // meanwhile collapsing all the redundant slashes.
                        type = PathType::PT_UNC;
                        off = nextNonSlash(input, next, len);
                        next = nextSlash(input, off, len);
                        if (off == next || next == -1) {
                            // UNC path is missing hostname
                            return ParseResult();
                        }
                        String const host = input.subString(off, next);  //host
                        off = nextNonSlash(input, next, len);
                        next = nextSlash(input, off, len);
                        if (off == next || next == -1) {
                            // UNC path is missing share-name
                            return ParseResult();
                        }
                        root = "\\\\" + host + "\\" + input.subString(off, next) + "\\";
                        off = next;
                    } else {
                        if (isLetter(c0) && c1 == ':') {
                            gchar c2 = 0;
                            if (len > 2 && isSlash(c2 = input.charAt(2))) {
                                // avoid concatenation when root is "D:\"
                                if (c2 == '\\') {
                                    root = input.subString(0, 3);
                                } else {
                                    root = input.subString(0, 2) + "\\";
                                }
                                off = 3;
                                type = PathType::PT_ABSOLUTE;
                            } else {
                                root = input.subString(0, 2);
                                off = 2;
                                type = PathType::PT_DRIVE_RELATIVE;
                            }
                        }
                    }
                }
                if (off == 0) {
                    if (len > 0 && isSlash(input.charAt(0))) {
                        type = PathType::PT_DIRECTORY_RELATIVE;
                        root = "\\";
                    } else {
                        type = PathType::PT_RELATIVE;
                    }
                }

                if (requireNormalization) {
                    StringBuffer sb = StringBuffer(input.length());
                    sb.append(root);
                    const String normalized = normalizePath(sb, input, off);
                    if (normalized.isEmpty() && !root.isEmpty()) {
                        return ParseResult();
                    } else
                        return ParseResult(type, root, normalized);
                }
                return ParseResult(type, root, input);
            }

#if !defined(REPARSE_DATA_BUFFER_HEADER_SIZE)
            typedef struct _REPARSE_DATA_BUFFER {
                ULONG ReparseTag;
                USHORT ReparseDataLength;
                USHORT Reserved;
                union {
                    struct {
                        USHORT SubstituteNameOffset;
                        USHORT SubstituteNameLength;
                        USHORT PrintNameOffset;
                        USHORT PrintNameLength;
                        ULONG Flags;
                        WCHAR PathBuffer[1];
                    } SymbolicLinkReparseBuffer;
                    struct {
                        USHORT SubstituteNameOffset;
                        USHORT SubstituteNameLength;
                        USHORT PrintNameOffset;
                        USHORT PrintNameLength;
                        WCHAR PathBuffer[1];
                    } MountPointReparseBuffer;
                    struct {
                        UCHAR DataBuffer[1];
                    } GenericReparseBuffer;
                };
            } REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;
#  define REPARSE_DATA_BUFFER_HEADER_SIZE  CORE_OFFSETOF(REPARSE_DATA_BUFFER, GenericReparseBuffer)
#endif // !defined(REPARSE_DATA_BUFFER_HEADER_SIZE)
#ifndef MAXIMUM_REPARSE_DATA_BUFFER_SIZE
#define MAXIMUM_REPARSE_DATA_BUFFER_SIZE 16384
#endif
#ifndef IO_REPARSE_TAG_SYMLINK
#  define IO_REPARSE_TAG_SYMLINK (0xA000000CL)
#endif
#ifndef FSCTL_GET_REPARSE_POINT
#define FSCTL_GET_REPARSE_POINT  CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 42, METHOD_BUFFERED, FILE_ANY_ACCESS)
#endif

            core::io::NTFS FILESYSTEM = {};
        }


        NTFS::NTFS() : slash('\\'), altSlash('/'), semicolon(';') {}

        gbool NTFS::isSlash(gchar c) {
            return (c == '\\') || (c == '/');
        }

        gbool NTFS::isLetter(gchar c) {
            return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'));
        }

        String NTFS::slashify(const String &p) const {
            if (!p.isEmpty() && p.charAt(0) != slash)
                return String::valueOf(slash) + p;
            else return p;
        }

        gchar NTFS::separator() const {
            return slash;
        }

        gchar NTFS::pathSeparator() const {
            return semicolon;
        }

        String NTFS::normalize(const String &path) const {
            gint const n = path.length();
            gchar const slash0 = this->slash;
            gchar const slash1 = this->altSlash;
            gchar prev = 0;
            for (gint i = 0; i < n; i++) {
                gchar const c = path.charAt(i);
                if (c == slash1)
                    return normalize(path, n, (prev == slash0) ? i - 1 : i);
                if ((c == slash0) && (prev == slash0) && (i > 1))
                    return normalize(path, n, i - 1);
                if ((c == ':') && (i > 1))
                    return normalize(path, n, 0);
                prev = c;
            }
            if (prev == slash0)
                return normalize(path, n, n - 1);
            return path;
        }

        String NTFS::normalize(const String &path, gint len, gint off) const {
            if (len == 0)
                return path;
            if (off < 3) off = 0;   /* Avoid fencepost cases with UNC pathnames */
            gint src = 0;
            gchar const slash0 = this->slash;
            StringBuffer sb{len};

            if (off == 0) {
                /* Complete normalization, including prefix */
                src = normalizePrefix(path, len, sb);
            } else {
                /* Partial normalization */
                src = off;
                sb.append(path, 0, off);
            }

            /* Remove redundant slashes from the remainder of the path, forcing all
               slashes into the preferred slash0 */
            while (src < len) {
                gchar const c = path.charAt(src++);
                if (isSlash(c)) {
                    while ((src < len) && isSlash(path.charAt(src))) src++;
                    if (src == len) {
                        /* Check for trailing separator */
                        gint const sn = sb.length();
                        if ((sn == 2) && (sb.charAt(1) == ':')) {
                            /* "z:\\" */
                            sb.append(slash0);
                            break;
                        }
                        if (sn == 0) {
                            /* "\\" */
                            sb.append(slash0);
                            break;
                        }
                        if ((sn == 1) && (isSlash(sb.charAt(0)))) {
                            /* "\\\\" is not collapsed to "\\" because "\\\\" marks
                               the beginning of a UNC pathname.  Even though it is
                               not, by itself, a valid UNC pathname, we leave it as
                               is in order to be consistent with the win32 APIs,
                               which treat this case as an invalid UNC pathname
                               rather than as an alias for the root directory of
                               the current drive. */
                            sb.append(slash0);
                            break;
                        }
                        /* Path does not denote a root directory, so do not append
                           trailing slash0 */
                        break;
                    } else {
                        sb.append(slash0);
                    }
                } else {
                    sb.append(c);
                }
            }

            return sb.toString();
        }

        gint NTFS::normalizePrefix(const String &path, gint len, StringBuffer &sb) const {
            gint src = 0;
            while ((src < len) && isSlash(path.charAt(src))) src++;
            gchar c = 0;
            if ((len - src >= 2)
                && isLetter(c = path.charAt(src))
                && path.charAt(src + 1) == ':') {
                /* Remove leading slashes if followed by drive specifier.
                   This hack is necessary to support file URLs containing drive
                   specifiers (e.g., "file://c:/path").  As a side effect,
                   "/c:/path" can be used as an alternative to "c:/path". */
                sb.append(c);
                sb.append(':');
                src += 2;
            } else {
                src = 0;
                if ((len >= 2)
                    && isSlash(path.charAt(0))
                    && isSlash(path.charAt(1))) {
                    /* UNC pathname: Retain the first slash; leave src pointed at
                       the second slash so that further slashes will be collapsed
                       into the second slash.  The result will be a pathname
                       beginning with "\\\\" followed (most likely) by a host
                       name. */
                    src = 1;
                    sb.append(slash);
                }
            }
            return src;
        }

        gint NTFS::prefixLength(const String &path) const {
            gchar const slash0 = this->slash;
            gint const n = path.length();
            if (n == 0)
                return 0;
            gchar const c0 = path.charAt(0);
            gchar const c1 = (n > 1) ? path.charAt(1) : 0;
            if (c0 == slash0) {
                if (c1 == slash0)
                    return 2;  /* Absolute UNC pathname "\\\\foo" */
                return 1;                   /* Drive-relative "\\foo" */
            }
            if (isLetter(c0) && (c1 == ':')) {
                if ((n > 2) && (path.charAt(2) == slash0))
                    return 3;               /* Absolute local pathname "z:\\foo" */
                return 2;                   /* Directory-relative "z:foo" */
            }
            return 0;                       /* Completely relative */
        }

        String NTFS::resolve(const String &parent, const String &child) const {
            gint const pn = parent.length();
            if (pn == 0)
                return child;
            gint const cn = child.length();
            if (cn == 0)
                return parent;

            String const &c = child;
            gint childStart = 0;
            gint parentEnd = pn;

            gbool const isDirectoryRelative =
                    pn == 2 && isLetter(parent.charAt(0)) && parent.charAt(1) == ':';

            if ((cn > 1) && (c.charAt(0) == slash)) {
                if (c.charAt(1) == slash) {
                    /* Drop prefix when child is a UNC pathname */
                    childStart = 2;
                } else if (!isDirectoryRelative) {
                    /* Drop prefix when child is drive-relative */
                    childStart = 1;

                }
                if (cn == childStart) { // Child is a double slash
                    if (parent.charAt(pn - 1) == slash)
                        return parent.subString(0, pn - 1);
                    return parent;
                }
            }

            if (parent.charAt(pn - 1) == slash)
                parentEnd--;

            gint const strlen = parentEnd + cn - childStart;
            CharArray theChars = {};
            if (child.charAt(childStart) == slash || isDirectoryRelative) {
                theChars = CharArray(strlen);
                parent.chars(0, parentEnd, theChars, 0);
                child.chars(childStart, cn, theChars, parentEnd);
            } else {
                theChars = CharArray(strlen + 1);
                parent.chars(0, parentEnd, theChars, 0);
                theChars[parentEnd] = slash;
                child.chars(childStart, cn, theChars, parentEnd + 1);
            }
            return String(theChars);
        }

        String NTFS::defaultParent() const {
            return String::valueOf(slash);
        }

        String NTFS::fromURIPath(const String &path) const {
            String p = path;
            if ((p.length() > 2) && (p.charAt(2) == ':')) {
                // "/c:/foo" --> "c:/foo"
                p = p.subString(1);
                // "c:/foo/" --> "c:/foo", but "c:/" --> "c:/"
                if ((p.length() > 3) && p.endsWith("/"))
                    p = p.subString(0, p.length() - 1);
            } else if ((p.length() > 1) && p.endsWith("/")) {
                // "/foo/" --> "/foo"
                p = p.subString(0, p.length() - 1);
            }
            return p;
        }

        gbool NTFS::isAbsolute(const File &f) const {
            gint const pl = prefixLength0(f);
            return (((pl == 2) && (f.path().charAt(0) == slash))
                    || (pl == 3));
        }

        gbool NTFS::isInvalid(const File &f) const {
            if (f.path().indexOf('\u0000') >= 0)
                return true;

            // Invalid if there is a ":" at a position greater than 1, or if there
            // is a ":" at position 1 and the first character is not a letter
            String const pathname = f.path();
            gint const lastColon = pathname.lastIndexOf(u':');

            // Valid if there is no ":" present or if the last ":" present is
            // at index 1 and the first character is the latter
            if (lastColon < 0 ||
                (lastColon == 1 && isLetter(pathname.charAt(0))))
                return false;

            // Invalid if path creation fails
            ParseResult const result = parsePath(pathname, pathname.indexOf('/') >= 0);
            return result.type == PathType::PT_INVALID;
        }

        String NTFS::resolve(const File &f) const {
            String path = f.path();
            gint const pl = prefixLength0(f);
            if ((pl == 2) && (path.charAt(0) == slash))
                return path;                        /* UNC */
            if (pl == 3)
                return path;                        /* Absolute local */
            if (pl == 0) {
                if (path.length() > 0) {
                    try {
                        String const currentDir = fullPath("."_S);
                        gint const length = currentDir.length();
                        gchar const last = currentDir.charAt(length - 1);
                        gchar const prelast = length > 1 ? currentDir.charAt(length - 2) : 0;
                        if (prelast == slash && last == '.') {
                            // endswith "\."
                            if (path.length() == 1 && path.charAt(0) == '.')
                                return currentDir.subString(0, length - 1);
                            return currentDir.subString(0, length - 1) + path;
                        }
                        if (last == slash) {
                            currentDir + path;
                        } else if (path.length() == 1 && path.charAt(0) == '.') {
                            return Unsafe::moveInstance(currentDir);
                        } else {
                            return currentDir + slashify(path);
                        }
                    } catch (const Exception &ex) {
                        CORE_IGNORE(ex);
                    }
                }
                return userPath() + slashify(path);
            } /* Completely relative */
            if (pl == 1) {                          /* Drive-relative */
                String const up = userPath();
                String const ud = drive(up);
                if (!ud.isEmpty())
                    return ud + path;
                return up + path;                   /* User dir is a UNC path */
            }
            if (pl == 2) {                          /* Directory-relative */
                String const up = userPath();
                String const ud = drive(up);
                if ((ud != null) && path.startsWith(ud))
                    return up + slashify(path.subString(2));
                gchar const drive = path.charAt(0);
                String const dir = driveDirectory(drive);
                if (!dir.isEmpty()) {
                    /* When resolving a directory-relative path that refers to a
                       drive other than the current drive, insist that the caller
                       have read permission on the result */
                    String p = String::valueOf(drive) + (":" + dir + slashify(path.subString(2)));
                    return p;
                }
                return String::valueOf(drive) + ":" + slashify(path.subString(2)); /* fake it */
            }
            throw InternalError("Unresolvable path: " + path);
        }

        String NTFS::userPath() const {
            if (!userDir.isEmpty())
                return userDir;
            DWORD size = 0;
            // using environment variables
            NativeBuffer buffer = NativeBuffer(0);
/*

            HANDLE handle = GetCurrentProcess();
            HANDLE token = null;
            if (OpenProcessToken(handle, TOKEN_QUERY, &token) != FALSE) {
                if (GetUserProfileDirectoryW(token, null, &size) != FALSE) {
                    if (size > MAX_PATH) {
                        buffer = NativeBuffer((gint) size * 2);
                        if (GetUserProfileDirectoryW(token, (LPWSTR) buffer.address(), &size) != FALSE) {
                            (String &) userDir = String((LPWSTR) buffer.address(), 0, (gint) size);
                            CloseHandle(token);
                            return userDir;
                        }
                    }
                }
                CloseHandle(token);
            }
*/

            buffer = NativeBuffer(MAX_PATH * 2);
            if ((size = GetEnvironmentVariableW(L"USERPROFILE", (LPWSTR) buffer.address(), MAX_PATH)) == 0) {
                // user root without '\' in trailing (C:)
                size = GetEnvironmentVariableW(L"HOMEDRIVE", (LPWSTR) buffer.address(), MAX_PATH);
                // user path without a root path starting with '\' (\Users\<username>)
                size += GetEnvironmentVariableW(L"HOMEPATH", (LPWSTR) buffer.address() + size, MAX_PATH - size);
                if (size == 0) {
                    size = GetEnvironmentVariableW(L"HOME", (LPWSTR) buffer.address(), MAX_PATH);
                }
                if (size == 0) {
                    (String &) userDir = "~\\";
                    throw InternalError("Couldn't read property user.dir");
                }
            }
            (String &) userDir = String((LPWSTR) buffer.address(), 0, (gint) size);
            return userDir;
        }

        String NTFS::drive(const String &path) const {
            gint const pl = prefixLength(path);
            return (pl == 3) ? path.subString(0, 2) : String();
        }

        gint NTFS::driveIndex(gchar d) {
            if ((d >= 'a') && (d <= 'z')) return d - 'a';
            if ((d >= 'A') && (d <= 'Z')) return d - 'A';
            return -1;
        }

        String NTFS::driveDirectory0(gint drive) {
            DWORD const drives = GetLogicalDrives();
            if ((drives & (1 << drive)) != 0) {
                return String::valueOf(drive + 'A') + ":\\";
            }
            return {};
        }

        StringArray NTFS::driveDirCache = StringArray(26);

        String NTFS::driveDirectory(gchar drive) {
            gint const i = driveIndex(drive);
            if (i < 0)
                return {};
            String s = driveDirCache[i];
            if (!s.isEmpty())
                return s;
            s = driveDirectory0(i + 1);
            driveDirCache[i] = s;
            return s;
        }

        String NTFS::canonicalize(const String &path) const {
            // If a path is a drive letter only then skip canonicalization
            gint const len = path.length();
            if ((len == 2) &&
                (isLetter(path.charAt(0))) &&
                (path.charAt(1) == ':')) {
                gchar const c = path.charAt(0);
                if ((c >= 'A') && (c <= 'Z'))
                    return path;
                return String::valueOf((gchar) (c - 32)) + ":";
            } else if ((len == 3) &&
                       (isLetter(path.charAt(0))) &&
                       (path.charAt(1) == ':') &&
                       (path.charAt(2) == '\\')) {
                gchar const c = path.charAt(0);
                if ((c >= 'A') && (c <= 'Z'))
                    return path;
                return String::valueOf((gchar) (c - 32)) + ":\\";
            }
            return canonicalize0(path);
        }

        String NTFS::canonicalize0(const String &path) const {
            File const f = File(path);
            String const input = f.absolutePath();
            StringBuffer sb = StringBuffer(Math::max(path.length(), 32));
            StringTokenizer st = StringTokenizer(input, "\\");
            gint lastSeparator = -1;
            gint count = 0;
            // remove all. and.
            while (st.hasMore()) {
                String const token = st.nextToken();
                gint const n = token.length();
                if ((n == 0) || (n == 1) && (token.charAt(0) == '.')) {
                    // ignore current directory C:\dir1\.\dir2 -> C:\dir1\dir2 | C:\dir1\. -> C:\dir1
                    continue;
                } else if ((n == 2) && (token.charAt(0) == '.') && (token.charAt(1) == '.')) {
                    // back to parent directory C:\dir1\..\dir2 -> C:\dir2 | C:\dir1\.. -> C:\
                    //
                    if (count > 1) {
                        // C:\dir1\..\dir2 -> C:\dir2
                        lastSeparator = sb.lastIndexOf(File::SEPARATOR, lastSeparator - 1);
                        if (lastSeparator == -1) {
                            lastSeparator = sb.lastIndexOf(":", lastSeparator - 1) + 1;
                        }
                        sb.remove(lastSeparator + 1, sb.length());
                        count -= 1;
                    } else if (count == 1) {
                        // C:\.. -> C:\
                        // ignore directory (parent of root is root)
                        continue;
                    } else {
                        // error (absolutePath() function failed)
                        InternalError("Couldn't resolve path " + path).throws(__trace("core.io.NTFS"));
                    }
                } else {
                    sb.append(token);
                    if (st.hasMore()) {
                        sb.append(slash);
                        lastSeparator = sb.length() - 1;
                    }
                    count += 1;
                }
            }
            if (count == 1) {
                // The root path only is true path
                return sb.toString();
            }
            // clear all buffers and tokenizer
            st = StringTokenizer(sb.toString(), "\\");
            // restart each
            String lastPath = {};
            gint attr = 0;
            while (st.hasMore()) {
                String const child = st.nextToken();
                if (child.isEmpty())
                    continue;
                if (child.length() == 2) {
                    gchar const c1 = child.charAt(0);
                    gchar const c2 = child.charAt(1);
                    if (c2 == ':' && ('a' <= c1 && c1 <= 'z' || 'A' <= c1 && c1 <= 'Z')) {
                        lastPath = child;
                        continue;
                    }
                }
                lastPath = lastPath + slashify(child);
                File const file = File(lastPath);
                attr = attributes(file);
                if ((attr & (FA_EXISTS | FA_REPARSE_POINT)) == 0) {
                    // file doesn't exist and is not reparse-point
                    // return resolved part + remaining part
                    lastPath = file.path();
                    while (st.hasMore())
                        lastPath += slashify(st.nextToken());
                    return lastPath;
                } else if ((attr & FA_REPARSE_POINT) == 0) {
                    // file exists and is not reparse-point
                    lastPath = file.path();
                } else {
                    // file exists and is reparse-point
                    String &target = cache.getOrDefault(file.path(), "\0"_S);
                    if (target.length() == 1 && target.charAt(0) == 0) {
                        // key isn't found
                        target = resolveReparsePoint(file.path());
                    }
                    if (target.length() == 1 && target.charAt(0) == 0) {
                        // error: target not found
                        // return resolved part + remaining part
                        Unsafe::destroyInstance(target);
                        lastPath = file.path();
                        while (st.hasMore())
                            lastPath += slashify(st.nextToken());
                        return lastPath;
                    } else {
                        cache.put(file.path(), target);
                        lastPath = file.path();
                    }
                }
            }
            // start canonicalization
            return sb.toString();
        }

        namespace {
            CORE_FAST gint WA2FA[] = {
                    FILE_ATTRIBUTE_NORMAL, NTFS::FA_REGULAR,
                    FILE_ATTRIBUTE_DIRECTORY, NTFS::FA_DIRECTORY,
                    FILE_ATTRIBUTE_HIDDEN, NTFS::FA_HIDDEN,
                    FILE_ATTRIBUTE_REPARSE_POINT, NTFS::FA_REPARSE_POINT,
                    FILE_ATTRIBUTE_SYSTEM, NTFS::FA_SYSTEM,
                    FILE_ATTRIBUTE_ARCHIVE, NTFS::FA_ARCHIVE,
                    FILE_ATTRIBUTE_DEVICE, NTFS::FA_DEVICE,
                    FILE_ATTRIBUTE_READONLY, NTFS::FA_READONLY,
                    FILE_ATTRIBUTE_TEMPORARY, NTFS::FA_TEMPORARY,
                    FILE_ATTRIBUTE_SPARSE_FILE, NTFS::FA_SPARSE_FILE,
                    FILE_ATTRIBUTE_COMPRESSED, NTFS::FA_COMPRESSED,
                    FILE_ATTRIBUTE_OFFLINE, NTFS::FA_OFFLINE,
                    FILE_ATTRIBUTE_NOT_CONTENT_INDEXED, NTFS::FA_NO_CONTENT_COMPRESSED,
                    FILE_ATTRIBUTE_ENCRYPTED, NTFS::FA_ENCRYPTED,
                    FILE_ATTRIBUTE_INTEGRITY_STREAM, NTFS::FA_INTEGRITY_STREAM,
                    FILE_ATTRIBUTE_VIRTUAL, NTFS::FA_VIRTUAL,
                    FILE_ATTRIBUTE_EA, NTFS::FA_EA,
                    FILE_ATTRIBUTE_PINNED, NTFS::FA_PINNED,
                    FILE_ATTRIBUTE_UNPINNED, NTFS::FA_UNPINNED,
                    FILE_ATTRIBUTE_RECALL_ON_OPEN, NTFS::FA_RECALL_ON_OPEN,
                    FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS, NTFS::FA_RECALL_ON_DATA_ACCESS,
            };
        }

        gint NTFS::attributes(const File &f) const {
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            DWORD const dw = GetFileAttributesW((LPWSTR) &chars[0]);
            if (dw == INVALID_FILE_ATTRIBUTES) {
                return 0;
            }
            gint attr = 0;
            for (int i = 0; i < 42; i += 2) {
                if ((WA2FA[i] & dw) != 0) {
                    attr |= WA2FA[i + 1];
                }
            }
            if (attr != 0)
                attr |= FA_EXISTS;
            return attr;
        }

        String NTFS::fullPath(const String &path) const {
            CharArray chars = path.isEmpty() ? userPath().chars() : path.chars();
            NativeBuffer buffer = NativeBuffer(MAX_PATH * 2);
            DWORD size = GetFullPathNameW((LPCWSTR) &chars[0], MAX_PATH, (LPWSTR) buffer.address(), NULL);
            if (size > buffer.size()) {
                buffer = NativeBuffer((gint) size * 2);
                size = GetFullPathNameW((LPCWSTR) &chars[0], size, (LPWSTR) buffer.address(), NULL);
            }
            if (size == 0) {
                if (path.length() == 1 && path.charAt(0) == '.') {
                    buffer = NativeBuffer(EXTENDED_MAX_PATH * 2);
                    size = GetCurrentDirectoryW(EXTENDED_MAX_PATH, (LPWSTR) buffer.address());
                    if (size > 0) {
                        return String((LPWSTR) buffer.address(), 0, (gint) size);
                    }
                }
                IOException("Could not resolve path: " + path).throws(__trace("core.io.NTFS"));
            }
            String const res = String((LPWSTR) buffer.address(), 0, (gint) size);
            return res;
        }

        String NTFS::resolveReparsePoint(const String &path) const {
            if (path.isEmpty())
                return {};
            CharArray chars = path.chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            SECURITY_ATTRIBUTES securityAttributes = {0, NULL, FALSE};
            HANDLE handle = CreateFileW((LPWSTR) &chars[0],
                                        0,
                                        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                        &securityAttributes,
                                        OPEN_EXISTING,
                                        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
                                        NULL
            );
            if (handle != INVALID_HANDLE_VALUE) {
                NativeBuffer buffer = NativeBuffer(EXTENDED_MAX_PATH * 2);
                DWORD size = GetFinalPathNameByHandleW(handle, (LPWSTR) buffer.address(), EXTENDED_MAX_PATH,
                                                       FILE_NAME_NORMALIZED);
                if (size > 0) {
                    CloseHandle(handle);
                    return String((LPWSTR) buffer.address(), 0, (gint) size - 1);
                }
                buffer = NativeBuffer(MAXIMUM_REPARSE_DATA_BUFFER_SIZE);
                OVERLAPPED overlapped = {};
                if (DeviceIoControl(handle, FSCTL_GET_REPARSE_POINT, NULL, 0, (LPVOID) buffer.address(), buffer.size(),
                                    &size, &overlapped) != FALSE) {
                    CloseHandle(handle);
                    PREPARSE_DATA_BUFFER rdb = (PREPARSE_DATA_BUFFER) buffer.address();
                    gint length = 0;
                    gint offset = 0;
                    LPWSTR target = NULL;
                    if (rdb->ReparseTag == IO_REPARSE_TAG_MOUNT_POINT) {
                        offset = rdb->MountPointReparseBuffer.SubstituteNameOffset / 2;
                        length = rdb->MountPointReparseBuffer.SubstituteNameOffset / 2;
                        target = rdb->MountPointReparseBuffer.PathBuffer;
                    } else if (rdb->ReparseTag == IO_REPARSE_TAG_SYMLINK) {
                        offset = rdb->SymbolicLinkReparseBuffer.SubstituteNameOffset / 2;
                        length = rdb->SymbolicLinkReparseBuffer.SubstituteNameLength / 2;
                        target = rdb->SymbolicLinkReparseBuffer.PathBuffer;
                    }
                    String retVal{target, offset, length};
                    if (retVal.length() >= 4) {
                        // remove: \\?\, \??\, \\?\UNC\ specifiers,
                        gchar c1 = retVal.charAt(0);
                        gchar c2 = retVal.charAt(1);
                        gchar c3 = retVal.charAt(2);
                        gchar c4 = retVal.charAt(3);
                        if (isSlash(c1)) {
                            if (c2 == '?' && c3 == '?' && isSlash(c4) || isSlash(c2) && c3 == '?' && isSlash(c4)) {
                                retVal = retVal.subString(4);
                                if (retVal.length() >= 4) {
                                    c1 = retVal.charAt(0);
                                    c2 = retVal.charAt(1);
                                    c3 = retVal.charAt(2);
                                    c4 = retVal.charAt(3);
                                    if ((c1 == 'U' || c1 == 'u') && (c2 == 'N' || c2 == 'n') &&
                                        (c3 == 'C' || c3 == 'c') && isSlash(c4)) {
                                        retVal = retVal.subString(4);
                                    }
                                }
                            }
                        }
                    }
                    if (retVal.isEmpty())
                        return "\0"_S;
                    return retVal;
                } else {
                    CloseHandle(handle);
                }
            }
            NativeBuffer buffer = NativeBuffer(MAX_PATH);
            DWORD size = 0;
            DWORD r = GetVolumePathNamesForVolumeNameW(
                    (LPWSTR) &chars[0], (LPWCH) buffer.address(), buffer.size(), &size);

            if (r == 0) {
                if (GetLastError() == ERROR_MORE_DATA) {
                    buffer = NativeBuffer((gint) size);
                    size = 0;
                    chars = path.chars();
                    if (chars.length() == 0)
                        chars = userPath().chars();
                    r = GetVolumePathNamesForVolumeNameW((LPWSTR) &chars[0], (LPWCH) buffer.address(), buffer.size(),
                                                         &size);
                }
            }

            if (r == 0) {
                // error
                IOException("Could not resolve path: " + path).throws(__trace("core.io.NTFS"));
                return {};
            }
            return String((LPWSTR) buffer.address(), 0, (gint) size);
        }

        gbool NTFS::checkAccess(const File &f, gint access) const {
            if (access == -1)
                return false;
            DWORD dw = 0;
            // convert to Windows access permission
            if ((access & (ACCESS_READ | (ACCESS_READ << 3) | (ACCESS_READ << 6))) != 0)
                dw |= GENERIC_READ;
            if ((access & (ACCESS_WRITE | (ACCESS_WRITE << 3) | (ACCESS_WRITE << 6))) != 0)
                dw |= GENERIC_WRITE;
            if ((access & (ACCESS_EXECUTE | (ACCESS_EXECUTE << 3) | (ACCESS_EXECUTE << 6))) != 0)
                dw |= GENERIC_EXECUTE;
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();

            PSID owner;
            PSID group;
            PACL dacl;
            PSECURITY_DESCRIPTOR sd;
            DWORD const res = GetNamedSecurityInfoW(
                    (LPWSTR) &chars[0], SE_FILE_OBJECT,
                    OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION,
                    &owner, &group, &dacl, NULL, &sd);
            if (res == ERROR_SUCCESS) {
                GENERIC_MAPPING genericMapping = {};
                genericMapping.GenericAll = dw;
                genericMapping.GenericRead = dw & GENERIC_READ;
                genericMapping.GenericWrite = dw & GENERIC_WRITE;
                genericMapping.GenericExecute = dw & GENERIC_EXECUTE;
                BOOL accessStatus = FALSE;
                DWORD grantedAccess = 0;
                if (AccessCheck(sd, GetCurrentProcessToken(), dw, &genericMapping, NULL, 0, &grantedAccess,
                                &accessStatus) != FALSE) {
                    return accessStatus != 0;
                }
            }

            HANDLE handle = CreateFileW((LPCWSTR) &chars[0],
                                        dw,
                                        0,
                                        NULL,
                                        OPEN_EXISTING,
                                        FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY | FILE_FLAG_OVERLAPPED,
                                        NULL);
            if (handle == INVALID_HANDLE_VALUE) {
                // WindowsException(GetLastError()).throwsAsIOException(f.path(), __trace("core.io.NTFS"));
                dw = GetLastError();
                switch (dw) {
                    case ERROR_FILE_NOT_FOUND:
                    case ERROR_NOT_FOUND:
                        return false;
                    default:
                        dw = 0;
                }

                CORE_FAST gint ALL_ACCESS_READ = ACCESS_READ | (ACCESS_READ << 3) | (ACCESS_READ << 6);
                CORE_FAST gint ALL_ACCESS_WRITE = ACCESS_WRITE | (ACCESS_WRITE << 3) | (ACCESS_WRITE << 6);
                CORE_FAST gint ALL_ACCESS_EXECUTE = ACCESS_EXECUTE | (ACCESS_EXECUTE << 3) | (ACCESS_EXECUTE << 6);

                // convert to unix access permission
                if ((access & ALL_ACCESS_READ) != 0)
                    dw |= (access & ALL_ACCESS_READ);
                if ((access & ALL_ACCESS_WRITE) != 0)
                    dw |= access & ALL_ACCESS_WRITE;
                if ((access & ALL_ACCESS_EXECUTE) != 0)
                    dw |= (access & ALL_ACCESS_EXECUTE);
                return _waccess((LPCWSTR) &chars[0], (gint) dw) != -1;
            }
            CloseHandle(handle);
            return true;
        }

        gbool NTFS::setPermission(const File &f, gint access, gbool enable, gbool ownerOnly) const {
            if (access == -1)
                return false;
            CORE_IGNORE(ownerOnly);

            CORE_FAST gint ALL_ACCESS_READ = ACCESS_READ | (ACCESS_READ << 3) | (ACCESS_READ << 6);
            CORE_FAST gint ALL_ACCESS_WRITE = ACCESS_WRITE | (ACCESS_WRITE << 3) | (ACCESS_WRITE << 6);
            CORE_FAST gint ALL_ACCESS_EXECUTE = ACCESS_EXECUTE | (ACCESS_EXECUTE << 3) | (ACCESS_EXECUTE << 6);

            gint dw = 0;
            if ((access & ALL_ACCESS_READ) != 0)
                dw |= 0x0100; // S_IREAD

            if ((access & ALL_ACCESS_WRITE) != 0)
                dw |= 0x0080; // S_IWRITE

            if ((access & ALL_ACCESS_EXECUTE) != 0)
                dw |= 0x0040; // S_IEXEC

            if (!enable) {
                dw = (~dw) & 0x1c0; // S_IREAD | S_IWRITE | S_IEXEC
            }
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();

            return _wchmod((LPCWSTR) &chars[0], dw) == 0;
        }

        glong NTFS::lastModifiedTime(const File &f) const {
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            HANDLE handle = CreateFileW((LPCWSTR) &chars[0],
                                        GENERIC_READ,
                                        FILE_SHARE_READ,
                                        NULL,
                                        OPEN_EXISTING,
                                        FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY | FILE_FLAG_OVERLAPPED,
                                        NULL);
            if (handle == INVALID_HANDLE_VALUE) {
                return 0L;
            }
            FILETIME lmt = {};
            FILETIME lat = {};
            FILETIME ct = {};
            if (GetFileTime(handle, &ct, &lat, &lmt) == 0) {
                CloseHandle(handle);
                SYSTEMTIME slmt = {};
                FileTimeToSystemTime(&lmt, &slmt);
                LocalDateTime const dt = LocalDateTime(slmt.wYear, slmt.wMonth, slmt.wDay,
                                                       slmt.wHour, slmt.wMinute, slmt.wSecond,
                                                       slmt.wMilliseconds * 1000000);
                return (dt.toLocalDate().toEpochDay() * LocalTime::MILLIS_PER_DAY) +
                       (dt.toLocalTime().toNanoOfDay() / LocalTime::NANOS_PER_MILLI);
            }
            CloseHandle(handle);
            return 0LL;
        }

        glong NTFS::lastAccessTime(const File &f) const {
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            HANDLE handle = CreateFileW((LPCWSTR) &chars[0],
                                        GENERIC_READ,
                                        FILE_SHARE_READ,
                                        NULL,
                                        OPEN_EXISTING,
                                        FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY | FILE_FLAG_OVERLAPPED,
                                        NULL);
            if (handle == INVALID_HANDLE_VALUE) {
                return 0L;
            }
            FILETIME lmt = {};
            FILETIME lat = {};
            FILETIME ct = {};
            if (GetFileTime(handle, &ct, &lat, &lmt) == 0) {
                CloseHandle(handle);
                SYSTEMTIME slat = {};
                FileTimeToSystemTime(&lat, &slat);
                LocalDateTime const dt = LocalDateTime(slat.wYear, slat.wMonth, slat.wDay,
                                                       slat.wHour, slat.wMinute, slat.wSecond,
                                                       slat.wMilliseconds * 1000000);
                return (dt.toLocalDate().toEpochDay() * LocalTime::MILLIS_PER_DAY) +
                       (dt.toLocalTime().toNanoOfDay() / LocalTime::NANOS_PER_MILLI);
            }
            CloseHandle(handle);
            return 0LL;
        }

        glong NTFS::creationTime(const File &f) const {
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            HANDLE handle = CreateFileW((LPCWSTR) &chars[0],
                                        GENERIC_READ,
                                        FILE_SHARE_READ,
                                        NULL,
                                        OPEN_EXISTING,
                                        FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY | FILE_FLAG_OVERLAPPED,
                                        NULL);
            if (handle == INVALID_HANDLE_VALUE) {
                return 0L;
            }
            FILETIME lmt = {};
            FILETIME lat = {};
            FILETIME ct = {};
            if (GetFileTime(handle, &ct, &lat, &lmt) == 0) {
                CloseHandle(handle);
                SYSTEMTIME sct = {};
                FileTimeToSystemTime(&ct, &sct);
                LocalDateTime const dt = LocalDateTime(sct.wYear, sct.wMonth, sct.wDay,
                                                       sct.wHour, sct.wMinute, sct.wSecond,
                                                       sct.wMilliseconds * 1000000);
                return (dt.toLocalDate().toEpochDay() * LocalTime::MILLIS_PER_DAY) +
                       (dt.toLocalTime().toNanoOfDay() / LocalTime::NANOS_PER_MILLI);
            }
            CloseHandle(handle);
            return 0LL;
        }

        glong NTFS::size(const File &f) const {
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            HANDLE handle = CreateFileW((LPCWSTR) &chars[0],
                                        GENERIC_READ,
                                        FILE_SHARE_READ,
                                        NULL,
                                        OPEN_EXISTING,
                                        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                                        NULL);
            if (handle == INVALID_HANDLE_VALUE) {
                return 0L;
            }
            LARGE_INTEGER fSize = {};
            if (GetFileSizeEx(handle, &fSize) != FALSE) {
                CloseHandle(handle);
                return fSize.QuadPart;
            }
            CloseHandle(handle);
            return 0L;
        }

        gbool NTFS::createFileExclusively(const String &path) const {
            CharArray chars = path.chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            HANDLE handle = CreateFileW((LPCWSTR) &chars[0],
                                        GENERIC_READ | GENERIC_WRITE,
                                        0,
                                        NULL,
                                        CREATE_NEW,
                                        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                                        NULL);
            if (handle == INVALID_HANDLE_VALUE) {
                DWORD const error = GetLastError();
                switch (error) {
                    case ERROR_FILE_EXISTS:
                        break;
                    default: // ERROR_ACCESS_DENIED
                        WindowsException((gint) error).throwAsIOException(path, __trace("core.io.NTFS"));
                }
                return false;
            }
            CloseHandle(handle);
            return true;
        }

        StringArray NTFS::childList(const File &f) const {
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            WIN32_FIND_DATAW wfd = {};
            HANDLE handle = FindFirstFileW((LPCWSTR) &chars[0], &wfd);
            if (handle == INVALID_HANDLE_VALUE) {
                gint const error = (gint) GetLastError();
                switch (error) {
                    case ERROR_FILE_NOT_FOUND:
                    case ERROR_NOT_FOUND:
                        break;
                    default:
                        WindowsException(error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                }
                return {};
            }
            LinkedList<String> children = {};
            do {
                if (wfd.cFileName[0] != 0)
                    children.add(wfd.cFileName + 0);
                else
                    children.add(wfd.cAlternateFileName + 0);
            } while (FindNextFileW(handle, &wfd) != 0);
            StringArray files = StringArray(children.size());
            for (int i = 0; i < files.length(); ++i) {
                String &filename = children.get(i);
                files[i] = filename;
                Unsafe::destroyInstance(filename);
            }
            return files;
        }

        gbool NTFS::createDirectory(const File &f) const {
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            if (CreateDirectoryW((LPCWSTR) &chars[0], NULL) == 0) {
                gint const error = (gint) GetLastError();
                if (error != ERROR_ALREADY_EXISTS) {
                    WindowsException(error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                }
                return false;
            }
            return true;
        }

        gbool NTFS::setLastModifiedTime(const File &f, glong time) const {
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            try {
                LocalDateTime const dt = LocalDateTime::ofEpochSecond(time / 1000, (gint) (time - (time / 1000)),
                                                                      ZoneOffset::UTC);
                SYSTEMTIME const slmt = {
                        (WORD) dt.year(), (WORD) dt.month(), (WORD) dt.dayOfMonth(),
                        (WORD) dt.hour(), (WORD) dt.minute(), (WORD) dt.second(), (WORD) (dt.nano() / 1000000)
                };
                FILETIME lmt = {};
                if (SystemTimeToFileTime(&slmt, &lmt) == 0) {
                    return false;
                }
                HANDLE handle = CreateFileW((LPCWSTR) &chars[0],
                                            GENERIC_READ,
                                            FILE_SHARE_READ,
                                            NULL,
                                            OPEN_EXISTING,
                                            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                                            NULL);
                if (handle == INVALID_HANDLE_VALUE) {
                    gint const error = (gint) GetLastError();
                    if (error != ERROR_FILE_NOT_FOUND && error != ERROR_NOT_FOUND) {
                        WindowsException(error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                    }
                    return false;
                }
                if (SetFileTime(handle, NULL, NULL, &lmt) == 0) {
                    CloseHandle(handle);
                    gint const error = (gint) GetLastError();
                    WindowsException(error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                    return false;
                }
                CloseHandle(handle);
                return true;
            } catch (DateTimeException &dtex) {
                dtex.throws(__trace("core.io.NTFS"));
            }
        }

        gbool NTFS::setLastAccessTime(const File &f, glong time) const {
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            try {
                LocalDateTime const dt = LocalDateTime::ofEpochSecond(time / 1000, (gint) (time - (time / 1000)),
                                                                      ZoneOffset::UTC);
                SYSTEMTIME const slat = {
                        (WORD) dt.year(), (WORD) dt.month(), (WORD) dt.dayOfMonth(),
                        (WORD) dt.hour(), (WORD) dt.minute(), (WORD) dt.second(), (WORD) (dt.nano() / 1000000)
                };
                FILETIME lat = {};
                if (SystemTimeToFileTime(&slat, &lat) == 0) {
                    return false;
                }
                HANDLE handle = CreateFileW((LPCWSTR) &chars[0],
                                            GENERIC_READ,
                                            FILE_SHARE_READ,
                                            NULL,
                                            OPEN_EXISTING,
                                            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                                            NULL);
                if (handle == INVALID_HANDLE_VALUE) {
                    gint const error = (gint) GetLastError();
                    if (error != ERROR_FILE_NOT_FOUND && error != ERROR_NOT_FOUND) {
                        WindowsException(error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                    }
                    return false;
                }
                if (SetFileTime(handle, NULL, &lat, NULL) == 0) {
                    gint const error = (gint) GetLastError();
                    CloseHandle(handle);
                    WindowsException(error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                }
                return true;
            } catch (DateTimeException &dtex) {
                dtex.throws(__trace("core.io.NTFS"));
            }
        }

        gbool NTFS::setCreationTime(const File &f, glong time) const {
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            try {
                LocalDateTime const dt = LocalDateTime::ofEpochSecond(time / 1000, (gint) (time - (time / 1000)),
                                                                      ZoneOffset::UTC);
                SYSTEMTIME const sct = {
                        (WORD) dt.year(), (WORD) dt.month(), (WORD) dt.dayOfMonth(),
                        (WORD) dt.hour(), (WORD) dt.minute(), (WORD) dt.second(), (WORD) (dt.nano() / 1000000)
                };
                FILETIME ct = {};
                if (SystemTimeToFileTime(&sct, &ct) == 0) {
                    return false;
                }
                HANDLE handle = CreateFileW((LPCWSTR) &chars[0],
                                            GENERIC_READ,
                                            FILE_SHARE_READ,
                                            NULL,
                                            OPEN_EXISTING,
                                            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                                            NULL);
                if (handle == INVALID_HANDLE_VALUE) {
                    gint const error = (gint) GetLastError();
                    if (error != ERROR_FILE_NOT_FOUND) {
                        WindowsException(error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                    }
                    return false;
                }
                if (SetFileTime(handle, &ct, NULL, NULL) == 0) {
                    gint const error = (gint) GetLastError();
                    CloseHandle(handle);
                    WindowsException(error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                    return false;
                }
                return true;
            } catch (DateTimeException &dtex) {
                dtex.throws(__trace("core.io.NTFS"));
            }
        }

        gbool NTFS::setReadOnly(const File &f) const {
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            DWORD attr = GetFileAttributesW((LPCWSTR) &chars[0]);
            if (attr == INVALID_FILE_ATTRIBUTES) {
                gint const error = (gint) GetLastError();
                if (error != ERROR_FILE_NOT_FOUND && error != ERROR_NOT_FOUND) {
                    WindowsException(error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                }
                return false;
            }
            if ((attr & FILE_ATTRIBUTE_READONLY) != 0) {
                // is already read-only
                return true;
            }
            attr |= FILE_ATTRIBUTE_READONLY;
            if (SetFileAttributesW((LPCWSTR) &chars[0], attr) == 0) {
                gint const error = (gint) GetLastError();
                if (error != ERROR_FILE_NOT_FOUND) {
                    WindowsException(error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                }
                return false;
            }
            return true;
        }

        gbool NTFS::deleteFile(const File &f) const {
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            BOOL r = FALSE;
            if (f.isDirectory()) {
                r = RemoveDirectoryW((LPCWSTR) &chars[0]);
            } else {
                r = DeleteFileW((LPCWSTR) &chars[0]);
            }
            if (r == FALSE) {
                DWORD const error = (gint) GetLastError();
                if (error != ERROR_FILE_NOT_FOUND && error != ERROR_NOT_FOUND) {
                    WindowsException((gint) error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                }
                return false;
            }
            return true;
        }

        gbool NTFS::renameFile(const File &f1, const File &f2) const {
            CharArray src = f1.path().chars();
            if (src.length() == 0)
                src = userPath().chars();
            CharArray dest = f2.path().chars();
            if (dest.length() == 0 || src.length() == 0) {
                return false;
            }
            if (MoveFileExW((LPCWSTR) &src[0], (LPCWSTR) &dest[0], MOVEFILE_WRITE_THROUGH) == 0) {
                gint const error = (gint) GetLastError();
                if (error != ERROR_FILE_EXISTS && error != ERROR_NOT_FOUND) {
                    WindowsException(error).throwAsIOException(f1.path(), f2.path(), __trace("core.io.NTFS"));
                }
                return false;
            }
            return true;
        }

        FileArray NTFS::rootList() const {
            DWORD const dw = GetLogicalDrives();
            LinkedList<String> roots;
            for (int i = 0; i < 26; ++i) {
                if ((dw & (1 << i)) != 0) {
                    if (driveDirCache[i].isEmpty()) {
                        driveDirCache[i] = String::valueOf(i + 'A') + ":\\";
                    }
                    roots.add(driveDirCache[i]);
                }
            }
            FileArray rootNames = FileArray(roots.size());
            for (int i = 0; i < rootNames.length(); ++i) {
                String &root = roots.get(i);
                rootNames[i] = File(root);
                Unsafe::destroyInstance(root);
            }
            return rootNames;
        }

        glong NTFS::diskSpace(const File &f, gint t) const {
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            ULARGE_INTEGER total = {};
            ULARGE_INTEGER available = {};
            ULARGE_INTEGER free = {};
            if (GetDiskFreeSpaceExW((LPCWSTR) &chars[0], &available, &total, &free) == 0) {
                gint const error = (gint) GetLastError();
                if (error != ERROR_FILE_NOT_FOUND && error != ERROR_NOT_FOUND) {
                    WindowsException(error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                }
                return 0L;
            }
            switch (t) {
                case FileSystem::SPACE_FREE :
                    return free.QuadPart > Long::MAX_VALUE ? Long::MAX_VALUE : (glong) free.QuadPart;
                case FileSystem::SPACE_TOTAL:
                    return total.QuadPart > Long::MAX_VALUE ? Long::MAX_VALUE : (glong) total.QuadPart;
                case FileSystem::SPACE_USABLE:
                    return available.QuadPart > Long::MAX_VALUE ? Long::MAX_VALUE : (glong) available.QuadPart;
                default:
                    return 0L;
            }
        }

        gint NTFS::nameMax(const String &path) const {
            File f = File(path).absoluteFile();
            ParseResult const result = parsePath(f.path(), false);
            if (result.type == PathType::PT_INVALID) {
                return 0;
            }
            f = File(result.root);
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            /**
             * GetVolumeInformation
             */
            NativeBuffer const vnb = NativeBuffer(MAX_PATH * 2);
            DWORD vns = 0;
            DWORD mcl = 0;
            DWORD fsf = 0;
            NativeBuffer const fsnb = NativeBuffer(MAX_PATH * 2);
            if (GetVolumeInformationW((LPCWSTR) &chars[0], (LPWSTR) vnb.address(), MAX_PATH, &vns, &mcl, &fsf,
                                      (LPWSTR) fsnb.address(), MAX_PATH) == 0) {
                gint const error = (gint) GetLastError();
                if (error != ERROR_FILE_NOT_FOUND && error != ERROR_NOT_FOUND) {
                    WindowsException(error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                }
                return 0L;
            }
            return (gint) mcl;
        }

        gint NTFS::compare(const File &f1, const File &f2) const {
            return f1.path().compareToIgnoreCase(f2.path());
        }

        gint NTFS::hash(const File &f) const {
            /* Could make this more efficient: String.hashCodeIgnoreCase */
            return f.path().toLowerCase().hash() ^ 1234321;
        }

        gbool NTFS::checkAttributes(const File &f, gint attributes) const {
            if (attributes == 0)
                return false;
            gint const attr = this->attributes(f);
            if ((attributes & FA_SYMLINK) == FA_SYMLINK) {
                attributes &= ~FA_SYMLINK;
                if ((attributes & attr) != attributes)
                    return false;
                if ((attributes & FA_REPARSE_POINT) != FA_REPARSE_POINT)
                    return false;
                CharArray chars = f.path().chars();
                if (chars.length() == 0)
                    chars = userPath().chars();
                SECURITY_ATTRIBUTES sa = {0, NULL, FALSE};
                HANDLE handle = CreateFileW((LPCWSTR) &chars[0], GENERIC_READ, FILE_SHARE_READ, &sa, OPEN_EXISTING,
                                            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, NULL);
                if (handle == INVALID_HANDLE_VALUE) {
                    return false;
                }
                NativeBuffer const buffer = NativeBuffer(MAXIMUM_REPARSE_DATA_BUFFER_SIZE);
                DWORD size = 0;
                OVERLAPPED overlapped = {};
                BOOL const b = DeviceIoControl(handle, FSCTL_GET_REPARSE_POINT, NULL, 0, (LPVOID) buffer.address(),
                                               buffer.size(), &size, &overlapped);
                if (b == FALSE) {
                    CloseHandle(handle);
                    return false;
                }
                CloseHandle(handle);
                return (((PREPARSE_DATA_BUFFER) buffer.address())->ReparseTag & IO_REPARSE_TAG_SYMLINK) ==
                       IO_REPARSE_TAG_SYMLINK;
            } else
                return (attr & attributes) == attributes;
        }

        gbool NTFS::copyFile(const File &f1, const File &f2, gint copyOptions) const {
            String const src0 = f1.path();
            CharArray src = src0.isEmpty() ? userPath().chars() : src0.chars();
            String const src1 = f2.path();
            CharArray dest = src1.isEmpty() ? userPath().chars() : src1.chars();

            DWORD options = 0;
            if ((copyOptions & ~File::REPLACE_EXISTING) == ~File::REPLACE_EXISTING)
                options |= COPY_FILE_FAIL_IF_EXISTS;
            if ((copyOptions & File::COPY_NO_BUFFERING) != 0)
                options |= COPY_FILE_NO_BUFFERING;
            if ((copyOptions & File::COPY_SYMLINK) != 0)
                options |= COPY_FILE_COPY_SYMLINK;
            if ((copyOptions & File::COPY_RESTARTABLE) != 0)
                options |= COPY_FILE_RESTARTABLE;
            if ((copyOptions & File::COPY_DECRYPTED_DESTINATION) != 0)
                options |= COPY_FILE_ALLOW_DECRYPTED_DESTINATION;
            if (CopyFileExW((LPCWSTR) &src[0], (LPCWSTR) &dest[0], NULL, NULL, FALSE, options) == 0) {
                gint error = (gint) GetLastError();
                switch (error) {
                    case ERROR_FILE_EXISTS: // destination already exists
                    case ERROR_FILE_NOT_FOUND: // source/destination not found
                    case ERROR_NOT_FOUND: // source/destination not found
                        return false;
                }

                if (CopyFileW((LPCWSTR) &src[0], (LPCWSTR) &dest[0], (BOOL) (options & COPY_FILE_FAIL_IF_EXISTS)) ==
                    0) {
                    error = (gint) GetLastError();
                    switch (error) {
                        case ERROR_FILE_EXISTS: // destination already exists
                        case ERROR_FILE_NOT_FOUND: // source/destination not found
                        case ERROR_NOT_FOUND: // source/destination not found
                            break;
                        default:
                            WindowsException(error).throwAsIOException(src0, src1, __trace("core.io.NTFS"));
                    }
                    return false;
                }
            }
            return true;
        }

        gbool NTFS::moveFile(const File &f1, const File &f2, gint copyOptions) const {
            String const src0 = f1.path();
            CharArray src = src0.isEmpty() ? userPath().chars() : src0.chars();
            String const src1 = f2.path();
            CharArray dest = src1.isEmpty() ? userPath().chars() : src1.chars();

            DWORD options = 0;
            if ((copyOptions & File::REPLACE_EXISTING) != 0)
                options |= MOVEFILE_REPLACE_EXISTING;
            if ((copyOptions & File::MOVE_BY_COPY) != 0)
                options |= MOVEFILE_COPY_ALLOWED;
            if ((copyOptions & File::MOVE_CREATE_HARDLINK) != 0)
                options |= MOVEFILE_CREATE_HARDLINK;
            if ((copyOptions & File::MOVE_UNTIL_REBOOT) != 0)
                options |= MOVEFILE_DELAY_UNTIL_REBOOT;
            if ((copyOptions & ~File::MOVE_TRACKABLE) == ~File::MOVE_TRACKABLE)
                options |= MOVEFILE_FAIL_IF_NOT_TRACKABLE;
            if (MoveFileExW((LPCWSTR) &src[0], (LPCWSTR) &dest[0], options) == 0) {
                gint error = (gint) GetLastError();
                switch (error) {
                    case ERROR_FILE_EXISTS: // destination already exists
                    case ERROR_FILE_NOT_FOUND: // source/destination not found
                    case ERROR_NOT_FOUND: // source/destination not found
                        return false;
                }
                if (MoveFileW((LPCWSTR) &src[0], (LPCWSTR) &dest[0]) == 0) {
                    error = (gint) GetLastError();
                    switch (error) {
                        case ERROR_FILE_EXISTS: // destination already exists
                        case ERROR_FILE_NOT_FOUND: // source/destination not found
                        case ERROR_NOT_FOUND: // source/destination not found
                            break;
                        default:
                            WindowsException(error).throwAsIOException(src0, f2.path(), __trace("core.io.NTFS"));
                    }
                    return false;
                }
            }
            return true;
        }

        gbool NTFS::equals(const Object &o) const {
            return this == &o;
        }

        Object &NTFS::clone() const {
            return (Object &) *this;
        }

        HashMap<String, String> NTFS::cache = {};

        void NTFS::deleteOnExit(const File &f) {
            if (!ExitHook.contains(f))
                ExitHook.add(f);
        }

        NTFS::~NTFS() {
            // delete all files marked by File.deleteOnExit
            gint const size = ExitHook.size();
            for (int i = size - 1; i >= 0; --i) {
                File &f = ExitHook.get(i);
                f.deleteFile();
                ExitHook.removeAt(i);
                Unsafe::destroyInstance(f);
            }
        }

        String NTFS::tempPath() const {
            if (!tempDir.isEmpty())
                return tempDir;
            NativeBuffer buffer = NativeBuffer(MAX_PATH * 2);
            DWORD size = GetTempPathW(MAX_PATH, (LPWSTR) buffer.address());
            if (size > 0) {
                // extends name
                NativeBuffer const out = NativeBuffer(EXTENDED_MAX_PATH * 2);
                DWORD const newSize = GetLongPathNameW((LPCWSTR) buffer.address(), (LPWSTR) out.address(),
                                                       EXTENDED_MAX_PATH);
                if (newSize > 0) {
                    return (String &) tempDir = String((LPWSTR) out.address(), 0, (gint) newSize);
                }
                return (String &) tempDir = String((LPWSTR) buffer.address(), 0, (gint) size);
            } else {
                // reading of env variable
                buffer = NativeBuffer(EXTENDED_MAX_PATH * 2);
                size = GetEnvironmentVariableW(L"TEMP", (LPWSTR) buffer.address(), EXTENDED_MAX_PATH);
                if (size == 0)
                    size = GetEnvironmentVariableW(L"TMP", (LPWSTR) buffer.address(), EXTENDED_MAX_PATH);
                if (size > 0) {
                    // extends name
                    NativeBuffer const out = NativeBuffer(EXTENDED_MAX_PATH * 2);
                    DWORD const newSize = GetLongPathNameW((LPCWSTR) buffer.address(), (LPWSTR) out.address(),
                                                           EXTENDED_MAX_PATH);
                    if (newSize > 0) {
                        return (String &) tempDir = String((LPWSTR) out.address(), 0, (gint) newSize);
                    }
                    return (String &) tempDir = String((LPWSTR) buffer.address(), 0, (gint) size);
                }
                String const userD = userPath();
                if (!userD.isEmpty()) {
                    File const f = File(userD, R"(AppData\Local\Temp)"_S);
                    if (f.exists())
                        return (String &) tempDir = f.path();
                }
                // return %SYSTEM_DRIVE%\Temp
                size = GetEnvironmentVariableW(L"SystemDrive", (LPWSTR) buffer.address(), EXTENDED_MAX_PATH);
                String const systemDrive = size > 0 ? String((LPWSTR) buffer.address(), 0, (gint) size) : "C:"_S;
                return (String &) tempDir = systemDrive + R"(\Temp)"_S;
            }
        }

        namespace {
            class FileOperationProgress : public Object, public IFileOperationProgressSink {
            private:
                ULONG counter = 1;

            public:

                String target;
                HRESULT result = S_OK;

                ~FileOperationProgress() override = default;

                HRESULT StartOperations() override {
                    return S_OK;
                }

                HRESULT FinishOperations(HRESULT hrResult) override {
                    CORE_IGNORE(hrResult);
                    return S_OK;
                }

                HRESULT PreRenameItem(DWORD dwFlags, IShellItem *psiItem, LPCWSTR pszNewName) override {
                    CORE_IGNORE(dwFlags);
                    CORE_IGNORE(psiItem);
                    CORE_IGNORE(pszNewName);
                    return S_OK;
                }

                HRESULT PostRenameItem(DWORD dwFlags, IShellItem *psiItem, LPCWSTR pszNewName, HRESULT hrRename,
                                       IShellItem *psiNewlyCreated) override {
                    CORE_IGNORE(dwFlags);
                    CORE_IGNORE(psiItem);
                    CORE_IGNORE(pszNewName);
                    CORE_IGNORE(hrRename);
                    CORE_IGNORE(psiNewlyCreated);
                    return S_OK;
                }

                HRESULT PreMoveItem(DWORD dwFlags, IShellItem *psiItem, IShellItem *psiDestinationFolder,
                                    LPCWSTR pszNewName) override {
                    CORE_IGNORE(dwFlags);
                    CORE_IGNORE(psiItem);
                    CORE_IGNORE(pszNewName);
                    CORE_IGNORE(psiDestinationFolder);
                    return S_OK;
                }

                HRESULT
                PostMoveItem(DWORD dwFlags, IShellItem *psiItem, IShellItem *psiDestinationFolder, LPCWSTR pszNewName,
                             HRESULT hrMove, IShellItem *psiNewlyCreated) override {
                    CORE_IGNORE(dwFlags);
                    CORE_IGNORE(psiItem);
                    CORE_IGNORE(pszNewName);
                    CORE_IGNORE(psiDestinationFolder);
                    CORE_IGNORE(hrMove);
                    CORE_IGNORE(psiNewlyCreated);
                    return S_OK;
                }

                HRESULT PreCopyItem(DWORD dwFlags, IShellItem *psiItem, IShellItem *psiDestinationFolder,
                                    LPCWSTR pszNewName) override {
                    CORE_IGNORE(dwFlags);
                    CORE_IGNORE(psiItem);
                    CORE_IGNORE(pszNewName);
                    CORE_IGNORE(psiDestinationFolder);
                    return S_OK;
                }

                HRESULT
                PostCopyItem(DWORD dwFlags, IShellItem *psiItem, IShellItem *psiDestinationFolder, LPCWSTR pszNewName,
                             HRESULT hrCopy, IShellItem *psiNewlyCreated) override {
                    CORE_IGNORE(dwFlags);
                    CORE_IGNORE(psiItem);
                    CORE_IGNORE(pszNewName);
                    CORE_IGNORE(psiDestinationFolder);
                    CORE_IGNORE(hrCopy);
                    CORE_IGNORE(psiNewlyCreated);
                    return S_OK;
                }

                HRESULT PreDeleteItem(DWORD dwFlags, IShellItem *psiItem) override {
                    CORE_IGNORE(psiItem);
                    // stop the operation if the file will be deleted rather than trashed
                    return (dwFlags & TSF_DELETE_RECYCLE_IF_POSSIBLE) != 0 ? S_OK : E_FAIL;
                }

                HRESULT PostDeleteItem(DWORD dwFlags, IShellItem *psiItem, HRESULT hrDelete,
                                       IShellItem *psiNewlyCreated) override {
                    CORE_IGNORE(dwFlags);
                    CORE_IGNORE(psiItem);
                    result = hrDelete;
                    if (psiNewlyCreated != null) {
                        LPWSTR pszName = NULL;
                        psiNewlyCreated->GetDisplayName(SIGDN_FILESYSPATH, &pszName);
                        if (pszName != null) {
                            target = String(pszName);
                            CoTaskMemFree(pszName);
                        }
                    }
                    return S_OK;
                }

                HRESULT PreNewItem(DWORD dwFlags, IShellItem *psiDestinationFolder, LPCWSTR pszNewName) override {
                    CORE_IGNORE(dwFlags);
                    CORE_IGNORE(pszNewName);
                    CORE_IGNORE(psiDestinationFolder);
                    return S_OK;
                }

                HRESULT PostNewItem(DWORD dwFlags, IShellItem *psiDestinationFolder, LPCWSTR pszNewName,
                                    LPCWSTR pszTemplateName, DWORD dwFileAttributes, HRESULT hrNew,
                                    IShellItem *psiNewItem) override {
                    CORE_IGNORE(dwFlags);
                    CORE_IGNORE(pszNewName);
                    CORE_IGNORE(psiDestinationFolder);
                    CORE_IGNORE(pszTemplateName);
                    CORE_IGNORE(dwFileAttributes);
                    CORE_IGNORE(hrNew);
                    CORE_IGNORE(psiNewItem);
                    return S_OK;
                }

                HRESULT UpdateProgress(UINT iWorkTotal, UINT iWorkSoFar) override {
                    CORE_IGNORE(iWorkTotal);
                    CORE_IGNORE(iWorkSoFar);
                    return S_OK;
                }

                HRESULT ResetTimer() override {
                    return S_OK;
                }

                HRESULT PauseTimer() override {
                    return S_OK;
                }

                HRESULT ResumeTimer() override {
                    return S_OK;
                }

                HRESULT QueryInterface(const IID &riid, void **ppvObject) override {
                    if (ppvObject == null)
                        return E_POINTER;

                    *ppvObject = null;

                    if (riid == __uuidof(IUnknown)) {
                        *ppvObject = static_cast<IUnknown *>(this);
                    } else if (riid == __uuidof(IFileOperationProgressSink)) {
                        *ppvObject = static_cast<IFileOperationProgressSink *>(this);
                    }

                    if (*ppvObject != null) {
                        AddRef();
                        return S_OK;
                    }

                    return E_NOINTERFACE;
                }

                ULONG AddRef() override {
                    counter += 1;
                    return counter;
                }

                ULONG Release() override {
                    counter -= 1;
                    if (counter == 0) {
                        Unsafe::destroyInstance(*this);
                        return 0;
                    }
                    return counter;
                }

                Object &clone() const override {
                    return Unsafe::allocateInstance<FileOperationProgress>(*this);
                }
            };
        }

        gbool NTFS::createLink(const File &f1, const File &f2, gint linkType) {
            CharArray src = f1.path().chars();
            if (src.length() == 0)
                src = userPath().chars();
            CharArray dest = f2.path().chars();
            if (dest.length() == 0)
                dest = userPath().chars();
            DWORD retVal = FALSE;
            DWORD error = 0;
            switch (linkType) {
                case SYMBOLIC_LINK:
                    retVal = CreateSymbolicLinkW((LPWSTR) &src[0], (LPWSTR) &dest[0], 0);
                    if (retVal == FALSE) {
                        error = GetLastError();
                        if (error == ERROR_FILE_NOT_FOUND) {
                            return false;
                        }
                    }
                    if (retVal == FALSE && f2.isDirectory()) {
                        retVal = CreateSymbolicLinkW((LPWSTR) &src[0], (LPWSTR) &dest[0], SYMBOLIC_LINK_FLAG_DIRECTORY);
                    }
                    if (retVal == FALSE) {
                        error = GetLastError();
                    }
                    break;
                case HARD_LINK:
                    retVal = CreateHardLinkW((LPWSTR) &src[0], (LPWSTR) &dest[0], NULL);
                    if (retVal == FALSE) {
                        error = GetLastError();
                        if (error == ERROR_FILE_NOT_FOUND) {
                            return false;
                        }
                    }
                    break;
                case SHORTCUT_LINK: {
                    HRESULT hr = NULL;
                    IShellLinkW *shl = NULL;
                    hr = CoInitialize(NULL);
                    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
                    // has already been called.
                    hr = CoCreateInstance(
                            f2.isDirectory() ? CLSID_FolderShortcut : CLSID_ShellLink,
                            NULL,
                            CLSCTX_INPROC_SERVER,
                            IID_IShellLinkW,
                            (LPVOID *) &shl
                    );
                    if (SUCCEEDED(hr)) {
                        IPersistFile *pf = NULL;

                        // Set the path to the shortcut target and add the description.
                        shl->SetPath((LPCWSTR) &dest[0]);
                        shl->SetDescription(L"");

                        // Query IShellLink for the IPersistFile interface, used for saving the
                        // shortcut in persistent storage.
                        hr = shl->QueryInterface(IID_IPersistFile, (LPVOID *) &pf);

                        if (SUCCEEDED(hr)) {

                            hr = pf->Save((LPCOLESTR) &src[0], TRUE);
                            pf->Release();
                        }
                        shl->Release();
                    }
                    retVal = SUCCEEDED(hr);
                }
                    break;
                default:
                    retVal = FALSE;
                    break;
            }
            return retVal != FALSE;
        }

        gbool NTFS::recycleFile(const File &f) {
            if (!f.exists())
                return false;
            String const path = f.absolutePath();
            CharArray chars = path.chars();
            if (chars.length() == 0)
                chars = userPath().chars();

            IFileOperation *fo = NULL;
            IShellItem *shi = NULL;
            FileOperationProgress *fops = NULL;

            HRESULT hr = E_FAIL;
            hr = CoCreateInstance(CLSID_FileOperation, NULL, CLSCTX_ALL, IID_PPV_ARGS(&fo));
            if (fo != NULL) {
                fo->SetOperationFlags(
                        FOF_ALLOWUNDO | FOFX_RECYCLEONDELETE | FOF_NOCONFIRMATION | FOF_SILENT | FOF_NOERRORUI);
                hr = SHCreateItemFromParsingName((LPWSTR) &chars[0], NULL, IID_PPV_ARGS(&shi));
                if (shi == NULL)
                    return false;
                fops = &Unsafe::allocateInstance<FileOperationProgress>();
                hr = fo->DeleteItem(shi, (IFileOperationProgressSink *) fops);
                if (!SUCCEEDED(hr))
                    return false;
                hr = fo->PerformOperations();
                if (!SUCCEEDED(hr))
                    return false;
                if (!SUCCEEDED(fops->result)) {
                    return false;
                }
                String const newLocation = fops->target;
                return true;
            }
            SHFILEOPSTRUCTW sho = {};
            sho.wFunc = FO_DELETE;
            sho.pFrom = (LPWSTR) &chars[0];
            sho.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT | FOF_NOERRORUI;
            DWORD const retVal = SHFileOperationW(&sho);
            switch (retVal) {
                case 0:
                    return true;
                case ERROR_CANCELLED:
                    break;
                default:
                    WindowsException((gint) retVal).throwAsIOException(f.path(), __trace("core.io.NTFS"));
            }
            return false;
        }

        String NTFS::toString() const {
            return "NTFS"_S;
        }

        gbool NTFS::setCurrentDirectory(const File &f) {
            String const path = f.absolutePath();
            CharArray chars = path.chars();
            if (chars.length() == 0)
                chars = userPath().chars();
            return SetCurrentDirectoryW((LPWSTR) &chars[0]) != 0;
        }

        String NTFS::ownerName(const File &f) const {
            CharArray chars = f.path().chars();
            if (chars.length() == 0)
                chars = userPath().chars();

            HANDLE h = CreateFileW(
                    (LPCWSTR) &chars[0],
                    GENERIC_READ,
                    FILE_SHARE_READ,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL
            );
            if(h == INVALID_HANDLE_VALUE) {
                h = CreateFileW(
                        (LPCWSTR) &chars[0],
                        GENERIC_READ,
                        FILE_SHARE_READ,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_DIRECTORY,
                        NULL
                );
            }
            if (h == INVALID_HANDLE_VALUE) {
                DWORD const error = GetLastError();
                WindowsException((gint) error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                return ""_S;
            }
            PSID owner = NULL;
            PSID group = NULL;
            PSID other = NULL;
            PSECURITY_DESCRIPTOR descriptor;
            DWORD const retVal = GetSecurityInfo(
                    h,
                    SE_FILE_OBJECT,
                    OWNER_SECURITY_INFORMATION,
                    &owner,
                    &group,
                    NULL,
                    NULL,
                    &descriptor
            );
            if (retVal != ERROR_SUCCESS) {
                CloseHandle(h);
                DWORD const error = GetLastError();
                WindowsException((gint) error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                return ""_S;
            }
            DWORD size1 = 0;
            DWORD size2 = 0;
            SID_NAME_USE use = SidTypeUnknown;
            // get number of chars sufficient to store accountName and domainName
            BOOL ret = LookupAccountSidW(
                    NULL,
                    owner,
                    NULL,
                    &size1,
                    NULL,
                    &size2,
                    &use
            );
            NativeBuffer const acctName = NativeBuffer((gint) size1 * 2);
            NativeBuffer const domainName = NativeBuffer((gint) size2 * 2);
            ret = LookupAccountSidW(
                    NULL,
                    owner,
                    (LPWSTR) acctName.address(),
                    &size1,
                    (LPWSTR) domainName.address(),
                    &size2,
                    &use
            );
            if (ret == FALSE) {
                CloseHandle(h);
                DWORD const error = GetLastError();
                if (error == ERROR_NONE_MAPPED) {
                    WindowsException((gint) error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                }
                return ""_S;
            }
            CloseHandle(h);
            return String((LPCWSTR) domainName.address(), 0, (gint) size2) + LR"(\)"_S
                   + String((LPCWSTR) acctName.address(), 0, (gint) size1);
        }

        gbool NTFS::setOwnerName(const File &f, const String &newOwner) const {
            if (newOwner.isEmpty()) {
                return false;
            }
            CharArray chars = f.path().chars();
            if (chars.length() == 0) {
                chars = userPath().chars();
            }
            CharArray name = newOwner.chars();
            if (name.length() == 0) {
                name = CharArray::of(0);
            }
            HANDLE h = CreateFileW(
                    (LPCWSTR) &chars[0],
                    GENERIC_READ,
                    FILE_SHARE_READ,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY,
                    NULL
            );
            if (h == INVALID_HANDLE_VALUE) {
                DWORD const error = GetLastError();
                WindowsException((gint) error).throwAsIOException(f.path(), __trace("core.io.NTFS"));
                return false;
            }
            SID newOwnerSID = {};
            DWORD cbSID = {};
            // separate domainName to acctName
            StringTokenizer st = StringTokenizer(newOwner, LR"(\)"_S);
            String acctName;
            String domainName;
            if (st.hasMore()) {
                domainName = st.nextToken();
            }
            if (st.hasMore()) {
                acctName = st.nextToken();
            } else {
                acctName = (String &&) domainName;
            }
            CharArray acct = acctName.isEmpty() ? CharArray::of(0) : acctName.chars();
            CharArray domain = domainName.isEmpty() ? CharArray::of(0) : domainName.chars();

            BOOL ret = LookupAccountNameW(
                    NULL,
                    (LPCWSTR) &acct[0],
                    &newOwnerSID,
                    &cbSID,
                    NULL, 0,
                    NULL
            );

            if (ret == FALSE) {
                return false;
            }

            DWORD const retVal = SetSecurityInfo(
                    h,
                    SE_FILE_OBJECT,
                    OWNER_SECURITY_INFORMATION,
                    &newOwnerSID,
                    NULL,
                    NULL,
                    NULL
            );
            return retVal == ERROR_SUCCESS;
        }

        FileSystem &FileSystem::defaultFileSystem() {
            static core::io::NTFS *NTFS[2] = {NULL, NULL};
            if (NTFS[1] == null) {
                NTFS[1] = &Unsafe::allocateInstance<core::io::NTFS>();
                return *NTFS[1];
            }
            return FILESYSTEM;
        };


    } // io
} // core
