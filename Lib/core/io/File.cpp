//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "File.h"
#include <core/io/IOException.h>
#include <core/io/FileSystem.h>
#include <core/native/StringArray.h>
#include <core/io/FileArray.h>
#include <core/util/ArrayList.h>
#include <core/private/Unsafe.h>
#include <core/time/DateTimeException.h>
#include <core/Long.h>

namespace core {
    namespace io {

        using namespace net;
        using namespace util;
        using namespace native;
        using namespace time;

        FileSystem &File::fs = FileSystem::defaultFileSystem();
        const String File::SEPARATOR = String::valueOf(fs.separator());
        const String File::PATH_SEPARATOR = String::valueOf(fs.pathSeparator());

        gbool File::isInvalid() const {
            if (status != CHECKED && status != INVALID) {
                PathStatus &st = (PathStatus &) status;
                st = fs.isInvalid(*this) ? INVALID : CHECKED;
            }
            return status == INVALID;
        }

        File::File(const String &pathname, gint prefixLength) : pathname(pathname), prefixLength(prefixLength) {}

        File::File(const String &child, const File &parent) : prefixLength(parent.prefixLength) {
            pathname = fs.resolve(parent.pathname, child);
        }

        File::File(const String &pathname) {
            File::pathname = fs.normalize(pathname);
            File::prefixLength = fs.prefixLength(File::pathname);
        }

        File::File(const String &parent, const String &child) {
            File::pathname = fs.resolve(parent.isEmpty() ?
                                        fs.defaultParent() : fs.normalize(parent), fs.normalize(child));
            File::prefixLength = fs.prefixLength(File::pathname);
        }

        File::File(const File &parent, const String &child) {
            File::pathname = fs.resolve(parent.pathname.isEmpty() ?
                                        fs.defaultParent() : fs.normalize(parent.pathname), fs.normalize(child));
            File::prefixLength = fs.prefixLength(File::pathname);
        }

        File::File(const net::URI &uri) {
            CORE_IGNORE(uri);
        }

        String File::name() const {
            if (File::pathname.isEmpty()) {
                String const user = fs.userPath();
                if (user.isEmpty())
                    return {};
                return home().name();
            }
            gint const i = File::pathname.lastIndexOf(SEPARATOR.charAt(0));
            return File::pathname.subString(i < prefixLength ? prefixLength : i + 1);
        }

        String File::parent() const {
            if (File::pathname.isEmpty()) {
                String const user = fs.userPath();
                if (user.isEmpty())
                    return "..";
                return home().parent();
            }
            gint const i = File::pathname.lastIndexOf(SEPARATOR.charAt(0));
            if (i < prefixLength) {
                if (prefixLength > 0 && File::pathname.length() >= prefixLength) {
                    return File::pathname.subString(0, prefixLength);
                }
                return currentDirectory().path();
            }
            return File::pathname.subString(0, i);
        }

        File File::parentFile() const {
            return File(parent(), prefixLength);
        }

        String File::path() const {
            return File::pathname;
        }

        gbool File::isAbsolute() const {
            return fs.isAbsolute(*this);
        }

        String File::absolutePath() const {
            return fs.resolve(*this);
        }

        File File::absoluteFile() const {
            const String absolutePath = fs.resolve(*this);
            return File(absolutePath, fs.prefixLength(absolutePath));
        }

        String File::canonicalPath() const {
            if (isInvalid())
                IOException("Invalid file path").throws(__trace("core.io.File"));
            if(pathname.isEmpty())
                return home().path();
            const String absolutePath = fs.resolve(*this);
            return fs.canonicalize(absolutePath);
        }

        File File::canonicalFile() const {
            if (isInvalid())
                IOException("Invalid file path").throws(__trace("core.io.File"));
            const String canonicalPath = File::canonicalPath();
            return File(canonicalPath, fs.prefixLength(canonicalPath));
        }

        String File::slashify(const String &path, gbool isDirectory) {
            CORE_IGNORE(path);
            CORE_IGNORE(isDirectory);
            CORE_UNREACHABLE();
        }

        net::URI File::toUri() const {
            CORE_UNREACHABLE();
        }

        gbool File::isReadable() const {
            if (isInvalid()) {
                return false;
            }
            if(pathname.isEmpty())
                return home().isReadable();
            return fs.checkAccess(*this, FileSystem::ACCESS_READ);
        }

        gbool File::isWritable() const {
            if (isInvalid()) {
                return false;
            }
            if(pathname.isEmpty())
                return home().isWritable();
            return fs.checkAccess(*this, FileSystem::ACCESS_WRITE);
        }

        gbool File::isExecutable() const {
            if (isInvalid()) {
                return false;
            }
            if(pathname.isEmpty())
                return home().isExecutable();
            return fs.checkAccess(*this, FileSystem::ACCESS_EXECUTE);
        }

        gbool File::exists() const {
            if (isInvalid()) {
                return false;
            }
            if(pathname.isEmpty())
                return home().exists();
            return fs.checkAttributes(*this, FileSystem::FA_EXISTS);
        }

        gbool File::isDirectory() const {
            if (isInvalid()) {
                return false;
            }
            if(pathname.isEmpty())
                return home().isDirectory();
            return fs.checkAttributes(*this, FileSystem::FA_DIRECTORY);
        }

        gbool File::isFile() const {
            if (isInvalid()) {
                return false;
            }
            if(pathname.isEmpty())
                return home().isFile();
            return fs.checkAttributes(*this, FileSystem::FA_REGULAR);
        }

        gbool File::isHidden() const {
            if (isInvalid()) {
                return false;
            }
            if(pathname.isEmpty())
                return home().isHidden();
            return fs.checkAttributes(*this, FileSystem::FA_HIDDEN);
        }

        glong File::lastModifiedTime() const {
            if (isInvalid()) {
                return 0L;
            }
            if(pathname.isEmpty())
                return home().lastModifiedTime();
            return fs.lastModifiedTime(*this);
        }

        glong File::lastAccessTime() const {
            if (isInvalid()) {
                return 0L;
            }
            if(pathname.isEmpty())
                return home().lastAccessTime();
            return fs.lastAccessTime(*this);
        }

        glong File::creationTime() const {
            if (isInvalid()) {
                return 0L;
            }
            if(pathname.isEmpty())
                return home().creationTime();
            return fs.creationTime(*this);
        }

        glong File::size() const {
            if (isInvalid()) {
                return 0L;
            }
            if(pathname.isEmpty())
                return home().size();
            return fs.size(*this);
        }

        gbool File::createFile() const {
            if (isInvalid()) {
                return false;
            }
            if(pathname.isEmpty())
                return home().createFile();
            return fs.createFileExclusively(File::pathname);
        }

        gbool File::deleteFile() const {
            if (isInvalid()) {
                return false;
            }
            if(pathname.isEmpty())
                return home().deleteFile();
            return fs.deleteFile(*this);
        }

        void File::deleteOnExit() const {
            if (!isInvalid()) {
                fs.deleteOnExit(absoluteFile());
            }
        }

        StringArray File::childList() const {
            if (!isInvalid())
                IOException("Invalid file path ").throws(__trace("core.io.File"));
            if(pathname.isEmpty())
                return home().childList();
            StringArray childNames = fs.childList(*this);
            if (childNames.isEmpty())
                return {};
            for (String &childName: childNames) {
                childName = fs.normalize(childName);
            }
            return childNames;
        }

        StringArray File::childList(const FileNameFilter &filter) const {
            if (!isInvalid())
                IOException("Invalid file path ").throws(__trace("core.io.File"));
            StringArray childNames = fs.childList(*this);
            if (childNames.isEmpty())
                return {};
            ArrayList<String> selected{childNames.length()};
            for (String &childName: childNames) {
                childName = fs.normalize(childName);
                if (filter.test(childName)) {
                    selected.add(childName);
                }
            }
            if (childNames.length() > selected.size()) {
                childNames = StringArray(selected.size());
                gint i = 0;
                for (String &childName: childNames) {
                    String &s = selected[i++];
                    childName = s;
                    Unsafe::destroyInstance(s);
                }
            }
            return childNames;
        }

        FileArray File::childFiles() const {
            if (!isInvalid())
                IOException("Invalid file path ").throws(__trace("core.io.File"));
            StringArray const childNames = fs.childList(*this);
            if (childNames.isEmpty())
                return {};
            FileArray childFiles{childNames.length()};
            gint i = 0;
            for (String const &childName: childNames) {
                childFiles[i++] = File(childName);
            }
            return childFiles;
        }

        FileArray File::childFiles(const FileFilter &filter) const {
            if (!isInvalid())
                IOException("Invalid file path ").throws(__trace("core.io.File"));
            StringArray const childNames = fs.childList(*this);
            if (childNames.isEmpty())
                return {};
            ArrayList<File> selected{childNames.length()};
            for (String const &childName: childNames) {
                File const childFile = File(*this, childName);
                if (filter.test(childFile)) {
                    selected.add(childFile);
                }
            }
            FileArray childFiles{selected.size()};
            gint i = 0;
            for (File &childFile: childFiles) {
                File &f = selected[i++];
                childFile = f;
                Unsafe::destroyInstance(f);
            }
            return childFiles;
        }

        gbool File::createDirectory() const {
            if (isInvalid()) {
                return false;
            }
            if(pathname.isEmpty())
                return home().createDirectory();
            return fs.createDirectory(*this);
        }

        gbool File::createDirectories() const {
            if(pathname.isEmpty())
                return home().createDirectories();
            if (isInvalid() || exists()) {
                return false;
            }
            try {
                File const canonical = canonicalFile();
                File const parent = canonical.parentFile();
                return parent.exists() ? canonical.createDirectory() :
                       parent.createDirectories() ? canonical.createDirectory() : false;
            } catch (const Exception &) {
                return false;
            }
        }

        gbool File::renameTo(const File &dest) const {
            if (isInvalid() || dest.isInvalid())
                return false;
            if(pathname.isEmpty())
                return home().renameTo(dest);
            if (dest.exists())
                return false;
            return fs.renameFile(*this, dest);
        }

        gbool File::copyTo(const File &dest, gint copyOptions...) const {
            if (isInvalid() || dest.isInvalid())
                return false;
            if(pathname.isEmpty())
                return home().copyTo(dest, copyOptions);
            // ---------------------------------------------
            glong const address = (glong) &copyOptions;
            gint i = 4;
            gint j = 4;
            while (true) {
                if (Unsafe::getByte(address + j) == 0) {
                    if (j - i > 4)
                        break;
                    gint const k = Unsafe::getInt(address + i);
                    if (k > 0 && k <= MOVE_TRACKABLE) {
                        copyOptions |= k;
                        i = j;
                    } else
                        break;
                }
                j += 1;
            }
            // ---------------------------------------------
            return fs.copyFile(*this, dest, copyOptions);
        }

        gbool File::moveTo(const File &dest, gint copyOptions...) const {
            if (isInvalid() || dest.isInvalid())
                return false;
            if(pathname.isEmpty())
                return home().moveTo(dest, copyOptions);
            gint moveOptions = 0;
            // convert copy options to move options
            if ((copyOptions & REPLACE_EXISTING) != 0) {
                moveOptions |= REPLACE_EXISTING;
            }
            if ((copyOptions & MOVE_TRACKABLE) != 0) {
                moveOptions |= MOVE_TRACKABLE;
            }
            if ((copyOptions & MOVE_UNTIL_REBOOT) != 0) {
                moveOptions |= MOVE_UNTIL_REBOOT;
            }
            if ((copyOptions & MOVE_CREATE_HARDLINK) != 0) {
                moveOptions |= MOVE_CREATE_HARDLINK;
            }
            if ((copyOptions & MOVE_BY_COPY) != 0) {
                moveOptions |= MOVE_BY_COPY;
            }
            return fs.moveFile(*this, dest, moveOptions);
        }

        gbool File::setLastModifiedTime(glong time) const {
            if (isInvalid())
                return false;
            if(pathname.isEmpty())
                return home().setLastModifiedTime(time);
            try {
                return fs.setLastModifiedTime(*this, time);
            } catch (const DateTimeException &dtex) {
                return false;
            }
        }

        gbool File::setLastAccessTime(glong time) const {
            if (isInvalid())
                return false;
            if(pathname.isEmpty())
                return home().setLastAccessTime(time);
            try {
                return fs.setLastAccessTime(*this, time);
            } catch (const DateTimeException &dtex) {
                return false;
            }
        }

        gbool File::setCreationTime(glong time) const {
            if (isInvalid())
                return false;
            if(pathname.isEmpty())
                return home().setCreationTime(time);
            try {
                return fs.setCreationTime(*this, time);
            } catch (DateTimeException &dtex) {
                return false;
            }
        }

        gbool File::setReadOnly() const {
            if (isInvalid())
                return false;
            if(pathname.isEmpty())
                return home().setReadOnly();
            return fs.setReadOnly(*this);
        }

        gbool File::setWritable(gbool writable, gbool ownerOnly) const {
            if (isInvalid())
                return false;
            if(pathname.isEmpty())
                return home().setWritable(writable, ownerOnly);
            return fs.setPermission(*this, FileSystem::ACCESS_WRITE, writable, ownerOnly);
        }

        gbool File::setWritable(gbool writable) const {
            if (isInvalid())
                return false;
            if(pathname.isEmpty())
                return home().setWritable(writable);
            return fs.setPermission(*this, FileSystem::ACCESS_WRITE, writable, true);
        }

        gbool File::setReadable(gbool readable, gbool ownerOnly) const {
            if (isInvalid())
                return false;
            if(pathname.isEmpty())
                return home().setReadable(readable, ownerOnly);
            return fs.setPermission(*this, FileSystem::ACCESS_READ, readable, ownerOnly);
        }

        gbool File::setReadable(gbool readable) const {
            if (isInvalid())
                return false;
            if(pathname.isEmpty())
                return home().setReadable(readable);
            return fs.setPermission(*this, FileSystem::ACCESS_READ, readable, true);
        }

        gbool File::setExecutable(gbool executable, gbool ownerOnly) const {
            if (isInvalid())
                return false;
            if(pathname.isEmpty())
                return home().setExecutable(executable, ownerOnly);
            return fs.setPermission(*this, FileSystem::ACCESS_EXECUTE, executable, ownerOnly);
        }

        gbool File::setExecutable(gbool executable) const {
            if (isInvalid())
                return false;
            if(pathname.isEmpty())
                return home().setExecutable(executable);
            return fs.setPermission(*this, FileSystem::ACCESS_EXECUTE, executable, true);
        }

        FileArray File::rootFiles() {
            return fs.rootList();
        }

        glong File::totalDiskSpace() const {
            if (isInvalid())
                return 0L;
            glong const diskSpace = fs.diskSpace(*this, FileSystem::SPACE_TOTAL);
            return diskSpace < 0 ? Long::MAX_VALUE : diskSpace;
        }

        glong File::availableDiskSpace() const {
            if (isInvalid())
                return 0L;
            glong const diskSpace = fs.diskSpace(*this, FileSystem::SPACE_FREE);
            return diskSpace < 0 ? Long::MAX_VALUE : diskSpace;
        }

        glong File::usableDiskSpace() const {
            if (isInvalid())
                return 0L;
            glong const diskSpace = fs.diskSpace(*this, FileSystem::SPACE_USABLE);
            return diskSpace < 0 ? Long::MAX_VALUE : diskSpace;
        }

        File File::createTempFile(const String &prefix, const String &suffix, const File &directory) {
            CORE_UNREACHABLE();
        }

        File File::createTempFile(const String &prefix, const String &suffix) {
            CORE_UNREACHABLE();
        }

        gint File::compareTo(const File &pathname) const {
            return fs.compare(*this, pathname);
        }

        gbool File::equals(const Object &obj) const {
            if (this == &obj)
                return true;
            if (!Class<File>::hasInstance(obj)) {
                return false;
            }
            return fs.compare(*this, (const File &) obj) == 0;
        }

        gint File::hash() const {
            return fs.hash(*this);
        }

        String File::toString() const {
            return pathname;
        }

        Object &File::clone() const {
            return Unsafe::allocateInstance<File>(*this);
        }

        gbool File::createLink(const File &target) const {
            if (isInvalid() || target.isInvalid())
                return false;
            return fs.createLink(absoluteFile(), target.absoluteFile(), FileSystem::SHORTCUT_LINK);
        }

        gbool File::createHardLink(const File &target) const {
            if (isInvalid() || target.isInvalid())
                return false;
            return fs.createLink(absoluteFile(), target.absoluteFile(), FileSystem::HARD_LINK);
        }

        gbool File::createSymbolicLink(const File &target) const {
            if (isInvalid() || target.isInvalid())
                return false;
            return fs.createLink(absoluteFile(), target.absoluteFile(), FileSystem::SYMBOLIC_LINK);
        }

        gbool File::isSymbolicLink() const {
            if (isInvalid())
                return false;
            return fs.checkAttributes(*this, FileSystem::FA_SYMLINK);
        }

        gbool File::recycleFile() const {
            if (isInvalid())
                return false;
            return fs.recycleFile(*this);
        }

        File File::currentDirectory() {
            return File("."_S).absoluteFile();
        }

        gbool File::setCurrentDirectory(const File &f) {
            if(f.pathname.isEmpty())
                return setCurrentDirectory(home());
            return fs.setCurrentDirectory(f);
        }

        File File::home() {
            return File(fs.userPath());
        }

        File File::temp() {
            return File(fs.tempPath());
        }

        String File::owner() const {
            if (isInvalid()) {
                return ""_S;
            }
            return fs.ownerName(*this);
        }

        gbool File::setOwner(const String &newOwner) const {
            if (isInvalid()) {
                return false;
            }
            if (newOwner.isEmpty()) {
                return false;
            }
            return fs.setOwnerName(*this, newOwner);
        }

    }
} // core
