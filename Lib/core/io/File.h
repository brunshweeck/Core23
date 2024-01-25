//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_FILE_H
#define CORE23_FILE_H

#include <core/String.h>
#include <core/net/URI.h>
#include <core/function/Predicate.h>

namespace core {
    namespace io {

        /**
         * An abstract representation of file and directory pathnames.
         *
         * <p> User interfaces and operating systems use system-dependent <em>pathname
         * strings</em> to name files and directories.  This class presents an
         * abstract, system-independent view of hierarchical pathnames.  An
         * <em>abstract pathname</em> has two components:
         *
         * <ol>
         * <li> An optional system-dependent <em>prefix</em> string,
         *      such as a disk-drive specifier, <b> "/"</b>&nbsp;for the UNIX root
         *      directory, or <b> "\\\\"</b>&nbsp;for a Microsoft Windows UNC pathname, and
         * <li> A sequence of zero or more string <em>names</em>.
         * </ol>
         *
         * The first name in an abstract pathname may be a directory name or, in the
         * case of Microsoft Windows UNC pathnames, a hostname.  Each subsequent name
         * in an abstract pathname denotes a directory; the last name may denote
         * either a directory or a file.  The <em>empty</em> abstract pathname has no
         * prefix and an empty name sequence.
         *
         * <p> The conversion of a pathname string to or from an abstract pathname is
         * inherently system-dependent.  When an abstract pathname is converted into a
         * pathname string, each name is separated from the next by a single copy of
         * the default <em>separator character</em>.  The default name-separator
         * character is made available in the public static fields <b style="color: orange;"> SEPARATOR</b> of this class.
         * When a pathname string is converted into an abstract pathname, the names
         * within it may be separated by the default name-separator character or by any
         * other name-separator character that is supported by the underlying system.
         *
         * <p> A pathname, whether abstract or in string form, may be either
         * <em>absolute</em> or <em>relative</em>.  An absolute pathname is complete in
         * that no other information is required in order to locate the file that it
         * denotes.  A relative pathname, in contrast, must be interpreted in terms of
         * information taken from some other pathname.  By default the classes in the
         * <b> java.io</b> package always resolve relative pathnames against the
         * current user directory.  This directory is named by the system property
         * <b> user.dir</b>, and is typically the directory in which the Java
         * virtual machine was invoked.
         *
         * <p> The <em>parent</em> of an abstract pathname may be obtained by invoking
         * the <b style="color: orange;"> parent</b> method of this class and consists of the pathname's
         * prefix and each name in the pathname's name sequence except for the last.
         * Each directory's absolute pathname is an ancestor of any <b> File</b>
         * object with an absolute abstract pathname which begins with the directory's
         * absolute pathname.  For example, the directory denoted by the abstract
         * pathname <b> "/usr"</b> is an ancestor of the directory denoted by the
         * pathname <b> "/usr/local/bin"</b>.
         *
         * <p> The prefix concept is used to handle root directories on UNIX platforms,
         * and drive specifiers, root directories and UNC pathnames on Microsoft Windows platforms,
         * as follows:
         *
         * <ul>
         *
         * <li> For UNIX platforms, the prefix of an absolute pathname is always
         * <b> "/"</b>.  Relative pathnames have no prefix.  The abstract pathname
         * denoting the root directory has the prefix <b> "/"</b> and an empty
         * name sequence.
         *
         * <li> For Microsoft Windows platforms, the prefix of a pathname that contains a drive
         * specifier consists of the drive letter followed by <b> ":"</b> and
         * possibly followed by <b> "\\"</b> if the pathname is absolute.  The
         * prefix of a UNC pathname is <b> "\\\\"</b>; the hostname and the share
         * name are the first two names in the name sequence.  A relative pathname that
         * does not specify a drive has no prefix.
         *
         * </ul>
         *
         * <p> Instances of this class may or may not denote an actual file-system
         * object such as a file or a directory.  If it does denote such an object
         * then that object resides in a <i>partition</i>.  A partition is an
         * operating system-specific portion of storage for a file system.  A single
         * storage device (e.g. a physical disk-drive, flash memory, CD-ROM) may
         * contain multiple partitions.  The object, if any, will reside on the
         * partition <a id="partName">named</a> by some ancestor of the absolute
         * form of this pathname.
         *
         * <p> A file system may implement restrictions to certain operations on the
         * actual file-system object, such as reading, writing, and executing.  These
         * restrictions are collectively known as <i>access permissions</i>.  The file
         * system may have multiple sets of access permissions on a single object.
         * For example, one set may apply to the object's <i>owner</i>, and another
         * may apply to all other users.  The access permissions on an object may
         * cause some methods in this class to fail.
         *
         * <p> Instances of the <b> File</b> class are immutable; that is, once
         * created, the abstract pathname represented by a <b> File</b> object
         * will never change.
         */
        class File CORE_FINAL : public Object, public Comparable<File> {
        private:
            /**
             * The FileSystem object representing the platform's local file system.
             */
            static FileSystem &fs;

            /**
             * This abstract pathname's normalized pathname string. A normalized
             * pathname string uses the default name-separator character and does not
             * contain any duplicate or redundant separators.
             *
             * @serial
             */
            String pathname = {};

            /**
             * Enum type that indicates the status of a file path.
             */
            enum PathStatus : gbyte {
                INVALID = 1, CHECKED = 2
            };

            /**
             * The flag indicating whether the file path is invalid.
             */
            PathStatus status = (PathStatus) (CHECKED | INVALID);

            /**
             * Check if the file has an invalid path. Currently, the inspection of
             * a file path is very limited, and it only covers Nul character check
             * unless further checking is explicitly enabled by a system property.
             * Returning true means the path is definitely invalid/garbage, but
             * returning false does not guarantee that the path is valid.
             *
             * @return true if the file path is invalid.
             */
            gbool isInvalid() const;

            /**
             * The length of this abstract pathname's prefix, or zero if it has no
             * prefix. (Used by FileSystem classes)
             */
            gint prefixLength = {};

            CORE_FRATERNITY(FileSystem);

            CORE_ALIAS(FileFilter, function::Predicate<File>);
            CORE_ALIAS(FileNameFilter, function::Predicate<String>);

        public:

            /**
             * The system-dependent default name-separator character.  This field is
             * initialized to contain the first character of the value of the system
             * property <b> file.separator</b>.  On UNIX systems the value of this
             * field is <b> "/"</b>; on Microsoft Windows systems it is <b> "\\"</b>.
             */
            static const String SEPARATOR;

            /**
             * The system-dependent path-separator character.  This field is
             * initialized to contain the first character of the value of the system
             * property <b> path.separator</b>.  This character is used to
             * separate filenames in a sequence of files given as a <em>path list</em>.
             * On UNIX systems, this character is <b> ":"</b>; on Microsoft Windows systems it
             * is <b> ";"</b>.
             */
            static const String PATH_SEPARATOR;

        private:

            /**
             * Internal constructor for already-normalized pathname strings.
             */
            CORE_EXPLICIT File(const String &pathname, gint prefixLength);

            /**
             * Internal constructor for already-normalized pathname strings.
             * The parameter order is used to disambiguate this method from the
             * public(File, String) constructor.
             */
            CORE_EXPLICIT File(const String &child, const File &parent);

        public:
            /**
             * Creates a new <b> File</b> INSTANCE by converting the given
             * pathname string into an abstract pathname.  If the given string is
             * the empty string, then the result is the empty abstract pathname.
             *
             * @param   pathname  A pathname string
             */
            CORE_EXPLICIT File(const String &pathname);

            /* Note: The two-argument File constructors do not interpret an empty
               parent abstract pathname as the current user directory.  An empty parent
               instead causes the child to be resolved against the system-dependent
               directory defined by the FileSystem.getDefaultParent method.  On Unix
               this default is "/", while on Microsoft Windows it is "\\".  This is required for
               compatibility with the original behavior of this class. */

            /**
             * Creates a new <b> File</b> INSTANCE from a parent pathname string
             * and a child pathname string.
             *
             * <p> The <b> parent</b> pathname string is taken to denote
             * a directory, and the <b> child</b> pathname string is taken to
             * denote either a directory or a file.  If the <b> child</b> pathname
             * string is absolute then it is converted into a relative pathname in a
             * system-dependent way.  If <b> parent</b> is the empty string then
             * the new <b> File</b> INSTANCE is created by converting
             * <b> child</b> into an abstract pathname and resolving the result
             * against a system-dependent default directory.  Otherwise each pathname
             * string is converted into an abstract pathname and the child abstract
             * pathname is resolved against the parent.
             *
             * @param   parent  The parent pathname string
             * @param   child   The child pathname string
             */
            CORE_EXPLICIT File(const String &parent, const String &child);

            /**
             * Creates a new <b> File</b> INSTANCE from a parent abstract
             * pathname and a child pathname string.
             *
             * <p> Otherwise the <b> parent</b> abstract pathname is taken to
             * denote a directory, and the <b> child</b> pathname string is taken
             * to denote either a directory or a file.  If the <b> child</b>
             * pathname string is absolute then it is converted into a relative
             * pathname in a system-dependent way.  If <b> parent</b> is the empty
             * abstract pathname then the new <b> File</b> INSTANCE is created by
             * converting <b> child</b> into an abstract pathname and resolving
             * the result against a system-dependent default directory.  Otherwise each
             * pathname string is converted into an abstract pathname and the child
             * abstract pathname is resolved against the parent.
             *
             * @param   parent  The parent abstract pathname
             * @param   child   The child pathname string
             */
            CORE_EXPLICIT File(const File &parent, const String &child);

            /**
             * Creates a new <b> File</b> INSTANCE by converting the given
             * <b> file:</b> URI into an abstract pathname.
             *
             * <p> The exact form of a <b> file:</b> URI is system-dependent, hence
             * the transformation performed by this constructor is also
             * system-dependent.
             *
             * <p> For a given abstract pathname <i>f</i> it is guaranteed that
             *
             * <blockquote><code>
             * File(</code><i>&nbsp;f</i><code>.<b style="color: orange;">
             * toURI</b>()).equals(</code><i>&nbsp;f</i><code>.<b style="color: orange;"> absoluteFile</b>())
             * </code></blockquote>
             *
             * so long as the original abstract pathname, the URI, and the new abstract
             * pathname are all created in (possibly different invocations of) the same
             * Java virtual machine.  This relationship typically does not hold,
             * however, when a <b> file:</b> URI that is created in a virtual machine
             * on one operating system is converted into an abstract pathname in a
             * virtual machine on a different operating system.
             *
             * @param  uri
             *         An absolute, hierarchical URI with a scheme equal to
             *         <b> "file"</b>, a non-empty path component, and undefined
             *         authority, query, and fragment components
             *
             * @throws  IllegalArgumentException
             *          If the preconditions on the parameter do not hold
             *
             * @see toURI()
             */
            CORE_EXPLICIT File(const net::URI &uri);

            /**
             * Returns the name of the file or directory denoted by this abstract
             * pathname.  This is just the last name in the pathname's name
             * sequence.  If the pathname's name sequence is empty, then the empty
             * string is returned.
             *
             * @return  The name of the file or directory denoted by this abstract
             *          pathname, or the empty string if this pathname's name sequence
             *          is empty
             */
            String name() const;

            /**
             * Returns the pathname string of this abstract pathname's parent, or
             * <b> ""</b> if this pathname does not name a parent directory.
             *
             * <p> The <em>parent</em> of an abstract pathname consists of the
             * pathname's prefix, if any, and each name in the pathname's name
             * sequence except for the last.  If the name sequence is empty then
             * the pathname does not name a parent directory.
             *
             * @return  The pathname string of the parent directory named by this
             *          abstract pathname, or <b> ""</b> if this pathname
             *          does not name a parent
             */
            String parent() const;

            /**
             * Returns the abstract pathname of this abstract pathname's parent,
             * or <b> File("")</b> if this pathname does not name a parent
             * directory.
             *
             * <p> The <em>parent</em> of an abstract pathname consists of the
             * pathname's prefix, if any, and each name in the pathname's name
             * sequence except for the last.  If the name sequence is empty then
             * the pathname does not name a parent directory.
             *
             * @return  The abstract pathname of the parent directory named by this
             *          abstract pathname, or <b> File(".")</b> if this pathname
             *          does not name a parent
             */
            File parentFile() const;

            /**
             * Converts this abstract pathname into a pathname string.  The resulting
             * string uses the <b style="color: orange;"> default name-separator character</b> to
             * separate the names in the name sequence.
             *
             * @return  The string form of this abstract pathname
             */
            String path() const;

            /**
             * Tests whether this abstract pathname is absolute.  The definition of
             * absolute pathname is system dependent.  On UNIX systems, a pathname is
             * absolute if its prefix is <b> "/"</b>.  On Microsoft Windows systems, a
             * pathname is absolute if its prefix is a drive specifier followed by
             * <b> "\\"</b>, or if its prefix is <b> "\\\\"</b>.
             *
             * @return  <b> true</b> if this abstract pathname is absolute,
             *          <b> false</b> otherwise
             */
            gbool isAbsolute() const;

            /**
             * Returns the absolute pathname string of this abstract pathname.
             *
             * <p> If this abstract pathname is already absolute, then the pathname
             * string is simply returned as if by the <b style="color: orange;"> path</b>
             * method.  If this abstract pathname is the empty abstract pathname then
             * the pathname string of the current user directory, which is named by the
             * system property <b> user.dir</b>, is returned.  Otherwise this
             * pathname is resolved in a system-dependent way.  On UNIX systems, a
             * relative pathname is made absolute by resolving it against the current
             * user directory.  On Microsoft Windows systems, a relative pathname is made absolute
             * by resolving it against the current directory of the drive named by the
             * pathname, if any; if not, it is resolved against the current user
             * directory.
             *
             * @return  The absolute pathname string denoting the same file or
             *          directory as this abstract pathname
             */
            String absolutePath() const;

            /**
             * Returns the absolute form of this abstract pathname.  Equivalent to
             * <code>new&nbsp;File(this.<b style="color: orange;"> absolutePath</b>)</code>.
             *
             * @return  The absolute abstract pathname denoting the same file or
             *          directory as this abstract pathname
             *
             * @throws  SecurityException
             *          If a required system property value cannot be accessed.
             */
            File absoluteFile() const;

            /**
             * Returns the canonical pathname string of this abstract pathname.
             *
             * <p> A canonical pathname is both absolute and unique.  The precise
             * definition of canonical form is system-dependent.  This method first
             * converts this pathname to absolute form if necessary, as if by invoking the
             * <b style="color: orange;"> absolutePath</b> method, and then maps it to its unique form in a
             * system-dependent way.  This typically involves removing redundant names
             * such as <b> "."</b> and <b> ".."</b> from the pathname, resolving
             * symbolic links (on UNIX platforms), and converting drive letters to a
             * standard case (on Microsoft Windows platforms).
             *
             * <p> Every pathname that denotes an existing file or directory has a
             * unique canonical form.  Every pathname that denotes a nonexistent file
             * or directory also has a unique canonical form.  The canonical form of
             * the pathname of a nonexistent file or directory may be different from
             * the canonical form of the same pathname after the file or directory is
             * created.  Similarly, the canonical form of the pathname of an existing
             * file or directory may be different from the canonical form of the same
             * pathname after the file or directory is deleted.
             *
             * @return  The canonical pathname string denoting the same file or
             *          directory as this abstract pathname
             *
             * @throws  IOException
             *          If an I/O error occurs, which is possible because the
             *          construction of the canonical pathname may require
             *          filesystem queries
             */
            String canonicalPath() const;

            /**
             * Returns the canonical form of this abstract pathname.  Equivalent to
             * <code>new&nbsp;File(this.<b style="color: orange;"> canonicalPath</b>)</code>.
             *
             * @return  The canonical pathname string denoting the same file or
             *          directory as this abstract pathname
             *
             * @throws  IOException
             *          If an I/O error occurs, which is possible because the
             *          construction of the canonical pathname may require
             *          filesystem queries
             */
            File canonicalFile() const;

        private:
            static String slashify(const String &path, gbool isDirectory);

        public:

            /**
             * Constructs a <b> file:</b> URI that represents this abstract pathname.
             *
             * <p> The exact form of the URI is system-dependent.  If it can be
             * determined that the file denoted by this abstract pathname is a
             * directory, then the resulting URI will end with a slash.
             *
             * <p> For a given abstract pathname <i>f</i>, it is guaranteed that
             *
             * <blockquote><code>
             * <b style="color: orange;"> File</b>(</code><i>&nbsp;f</i><code>.toURI()).equals(
             * </code><i>&nbsp;f</i><code>.<b style="color: orange;"> absoluteFile</b>())
             * </code></blockquote>
             *
             * so long as the original abstract pathname, the URI, and the new abstract
             * pathname are all created in (possibly different invocations of) the same
             * Java virtual machine.  Due to the system-dependent nature of abstract
             * pathnames, however, this relationship typically does not hold when a
             * <b> file:</b> URI that is created in a virtual machine on one operating
             * system is converted into an abstract pathname in a virtual machine on a
             * different operating system.
             *
             * @return  An absolute, hierarchical URI with a scheme equal to
             *          <b> "file"</b>, a path representing this abstract pathname,
             *          and undefined authority, query, and fragment components
             * @throws SecurityException If a required system property value cannot
             * be accessed.
             */
            net::URI toUri() const;

            /**
             * Tests whether the application can read the file denoted by this
             * abstract pathname. On some platforms it may be possible to start the
             * programs with special privileges that allow it to read files that are
             * marked as unreadable. Consequently this method may return <b> true</b>
             * even though the file does not have read permissions.
             *
             * @return  <b> true</b> if and only if the file specified by this
             *          abstract pathname exists <em>and</em> can be read by the
             *          application; <b> false</b> otherwise
             */
            gbool isReadable() const;

            /**
             * Tests whether the application can modify the file denoted by this
             * abstract pathname. On some platforms it may be possible to start the
             * programs with special privileges that allow it to modify files that
             * are marked read-only. Consequently this method may return <b> true</b>
             * even though the file is marked read-only.
             *
             * @return  <b> true</b> if and only if the file system actually
             *          contains a file denoted by this abstract pathname <em>and</em>
             *          the application is allowed to write to the file;
             *          <b> false</b> otherwise.
             */
            gbool isWritable() const;

            /**
             * Tests whether the application can execute the file denoted by this
             * abstract pathname. On some platforms it may be possible to start the
             * programs with special privileges that allow it to execute files that
             * are not marked executable. Consequently this method may return <b> true</b>
             * even though the file does not have execute permissions.
             *
             * @return  <b> true</b> if and only if the abstract pathname exists
             *          <em>and</em> the application is allowed to execute the file
             */
            gbool isExecutable() const;

            /**
             * Tests whether the file or directory denoted by this abstract pathname
             * exists.
             *
             * @return  <b> true</b> if and only if the file or directory denoted
             *          by this abstract pathname exists; <b> false</b> otherwise
             */
            gbool exists() const;

            /**
             * Tests whether the file denoted by this abstract pathname is a
             * directory.
             *
             * @return <b> true</b> if and only if the file denoted by this
             *          abstract pathname exists <em>and</em> is a directory;
             *          <b> false</b> otherwise
             */
            gbool isDirectory() const;

            /**
             * Tests whether the file denoted by this abstract pathname is a normal
             * file.  A file is <em>normal</em> if it is not a directory and, in
             * addition, satisfies other system-dependent criteria.  Any non-directory
             * file created is guaranteed to be a normal file.
             *
             * @return  <b> true</b> if and only if the file denoted by this
             *          abstract pathname exists <em>and</em> is a normal file;
             *          <b> false</b> otherwise
             */
            gbool isFile() const;

            /**
             * Tests whether the file denoted by this abstract pathname is a symbolic
             * link.
             *
             * @return  <b> true</b> if and only if the file denoted by this
             *          abstract pathname exists <em>and</em> is a symbolic file;
             *          <b> false</b> otherwise
             */
            gbool isSymbolicLink() const;

            /**
             * Tests whether the file named by this abstract pathname is a hidden
             * file.  The exact definition of <em>hidden</em> is system-dependent.  On
             * UNIX systems, a file is considered to be hidden if its name begins with
             * a period character (<b> '.'</b>).  On Microsoft Windows systems, a file is
             * considered to be hidden if it has been marked as such in the filesystem.
             *
             * @return  <b> true</b> if and only if the file denoted by this
             *          abstract pathname is hidden according to the conventions of the
             *          underlying platform
             */
            gbool isHidden() const;

            /**
             * Returns the time that the file denoted by this abstract pathname was
             * last modified.
             *
             * @apiNote
             * While the unit of time of the return value is milliseconds, the
             * granularity of the value depends on the underlying file system and may
             * be larger.  For example, some file systems use time stamps in units of
             * seconds.
             *
             * @return  A <b> long</b> value representing the time the file was
             *          last modified, measured in milliseconds since the epoch
             *          (00:00:00 GMT, January 1, 1970), or <b> 0L</b> if the
             *          file does not exist or if an I/O error occurs.  The value may
             *          be negative indicating the number of milliseconds before the
             *          epoch
             */
            glong lastModifiedTime() const;

            /**
             * Returns the time that the file denoted by this abstract pathname was
             * last accessed.
             *
             * @apiNote
             * While the unit of time of the return value is milliseconds, the
             * granularity of the value depends on the underlying file system and may
             * be larger.  For example, some file systems use time stamps in units of
             * seconds.
             *
             * @return  A <b> long</b> value representing the time the file was
             *          last accessed, measured in milliseconds since the epoch
             *          (00:00:00 GMT, January 1, 1970), or <b> 0L</b> if the
             *          file does not exist or if an I/O error occurs.  The value may
             *          be negative indicating the number of milliseconds before the
             *          epoch
             */
            glong lastAccessTime() const;

            /**
             * Returns the time that the file denoted by this abstract pathname was
             * created.
             *
             * @apiNote
             * While the unit of time of the return value is milliseconds, the
             * granularity of the value depends on the underlying file system and may
             * be larger.  For example, some file systems use time stamps in units of
             * seconds.
             *
             * @return  A <b> long</b> value representing the time the file was
             *          created, measured in milliseconds since the epoch
             *          (00:00:00 GMT, January 1, 1970), or <b> 0L</b> if the
             *          file does not exist or if an I/O error occurs.  The value may
             *          be negative indicating the number of milliseconds before the
             *          epoch
             */
            glong creationTime() const;

            /**
             * Returns the length of the file denoted by this abstract pathname.
             * The return value is unspecified if this pathname denotes a directory.
             *
             * @return  The length, in bytes, of the file denoted by this abstract
             *          pathname, or <b> 0L</b> if the file does not exist.  Some
             *          operating systems may return <b> 0L</b> for pathnames
             *          denoting system-dependent entities such as devices or pipes.
             */
            glong size() const;

            /**
             * Atomically creates a new, empty file named by this abstract pathname if
             * and only if a file with this name does not yet exist.  The check for the
             * existence of the file and the creation of the file if it does not exist
             * are a single operation that is atomic with respect to all other
             * filesystem activities that might affect the file.
             * <P>
             * Note: this method should <i>not</i> be used for file-locking, as
             * the resulting protocol cannot be made to work reliably. The
             * <b style="color: orange;"> FileLock</b> facility should be used instead.
             *
             * @return  <b> true</b> if the named file does not exist and was
             *          successfully created; <b> false</b> if the named file
             *          already exists
             *
             * @throws  IOException
             *          If an I/O error occurred
             */
            gbool createFile() const;

            /**
             * Automatically create a new shortcut named by this abstract
             * pathname and that have target denoted by specified pathname
             *
             * @param target The abstract pathname of link target
             */
            gbool createLink(const File& target) const;

            /**
             * Automatically create a new hard link named by this abstract
             * pathname and that have target denoted by specified pathname
             *
             * @param target The abstract pathname of link target
             */
            gbool createHardLink(const File& target) const;

            /**
             * Automatically create a new symbolic link named by this abstract
             * pathname and that have target denoted by specified abstract
             * pathname
             *
             * @param target The abstract pathname of link target
             *
             * @return true If was successfully created
             */
            gbool createSymbolicLink(const File& target) const;

            /**
             * Deletes the file or directory denoted by this abstract pathname.  If
             * this pathname denotes a directory, then the directory must be empty in
             * order to be deleted.
             *
             * @return  <b> true</b> if and only if the file or directory is
             *          successfully deleted; <b> false</b> otherwise
             */
            gbool deleteFile() const;

            /**
             * Move the file or directory denoted by this abstract pathname to trash.
             *
             * @return  <b> true</b> if and only if the file or directory is
             *          successfully deleted; <b> false</b> otherwise
             */
            gbool recycleFile() const;

            /**
             * Requests that the file or directory denoted by this abstract
             * pathname be deleted when the terminates.
             * Files (or directories) are deleted in the reverse order that
             * they are registered. Invoking this method to delete a file or
             * directory that is already registered for deletion has no effect.
             *
             * <p> Once deletion has been requested, it is not possible to cancel the
             * request.  This method should therefore be used with care.
             *
             * <P>
             * Note: this method should <i>not</i> be used for file-locking, as
             * the resulting protocol cannot be made to work reliably.
             */
            void deleteOnExit() const;

            /**
             * Returns an root of strings naming the files and directories in the
             * directory denoted by this abstract pathname.
             *
             * <p> If this abstract pathname does not denote a directory, then this
             * method returns <b> empty root</b>.  Otherwise an root of strings is
             * returned, one for each file or directory in the directory.  Names
             * denoting the directory itself and the directory's parent directory are
             * not included in the result.  Each string is a file name rather than a
             * complete path.
             *
             * <p> There is no guarantee that the name strings in the resulting root
             * will appear in any specific order; they are not, in particular,
             * guaranteed to appear in alphabetical order.
             *
             * @return  An root of strings naming the files and directories in the
             *          directory denoted by this abstract pathname.  The root will be
             *          empty if the directory is empty.  Returns <b> empty root </b> if
             *          this abstract pathname does not denote a directory, or if an
             *          I/O error occurs.
             */
            StringArray childList() const;

            /**
             * Returns an root of strings naming the files and directories in the
             * directory denoted by this abstract pathname that satisfy the specified
             * filter.  The behavior of this method is the same as that of the
             * <b style="color: orange;"> childList()</b> method, except that the strings in the returned root
             * must satisfy the filter.
             *
             * @param  filter
             *         A filename filter
             *
             * @return  An root of strings naming the files and directories in the
             *          directory denoted by this abstract pathname that were accepted
             *          by the given <b> filter</b>.  The root will be empty if the
             *          directory is empty or if no names were accepted by the filter.
             *          Returns <b> empty root</b> if this abstract pathname does not denote
             *          a directory, or if an I/O error occurs.
             */
            StringArray childList(const FileNameFilter &filter) const;

            /**
             * Returns an root of abstract pathnames denoting the files in the
             * directory denoted by this abstract pathname.
             *
             * <p> If this abstract pathname does not denote a directory, then this
             * method returns <b> empty root</b>.  Otherwise an root of <b> File</b> objects
             * is returned, one for each file or directory in the directory.  Pathnames
             * denoting the directory itself and the directory's parent directory are
             * not included in the result.  Each resulting abstract pathname is
             * constructed from this abstract pathname using the <b style="color: orange;"> File(File,&nbsp;String)</b>
             * constructor.  Therefore if this
             * pathname is absolute then each resulting pathname is absolute; if this
             * pathname is relative then each resulting pathname will be relative to
             * the same directory.
             *
             * <p> There is no guarantee that the name strings in the resulting root
             * will appear in any specific order; they are not, in particular,
             * guaranteed to appear in alphabetical order.
             *
             * @return  An root of abstract pathnames denoting the files and
             *          directories in the directory denoted by this abstract pathname.
             *          The root will be empty if the directory is empty.  Returns
             *          <b> empty root</b> if this abstract pathname does not denote a
             *          directory, or if an I/O error occurs.
             */
            FileArray childFiles() const;

            /**
             * Returns an root of abstract pathnames denoting the files and
             * directories in the directory denoted by this abstract pathname that
             * satisfy the specified filter.  The behavior of this method is the same
             * as that of the <b style="color: orange;"> childFiles()</b> method, except that the pathnames in
             * the returned root must satisfy the filter.
             *
             * @param  filter
             *         A filename filter
             *
             * @return  An root of abstract pathnames denoting the files and
             *          directories in the directory denoted by this abstract pathname.
             *          The root will be empty if the directory is empty.  Returns
             *          <b> empty root</b> if this abstract pathname does not denote a
             *          directory, or if an I/O error occurs.
             */
            FileArray childFiles(const FileFilter &filter) const;

            /**
             * Creates the directory named by this abstract pathname.
             *
             * @return  <b> true</b> if and only if the directory was
             *          created; <b> false</b> otherwise
             */
            gbool createDirectory() const;

            /**
             * Creates the directory named by this abstract pathname, including any
             * necessary but nonexistent parent directories.  Note that if this
             * operation fails it may have succeeded in creating some of the necessary
             * parent directories.
             *
             * @return  <b> true</b> if and only if the directory was created,
             *          along with all necessary parent directories; <b> false</b>
             *          otherwise
             */
            gbool createDirectories() const;

            /**
             * Renames the file denoted by this abstract pathname.
             *
             * <p> Many aspects of the behavior of this method are inherently
             * platform-dependent: The rename operation might not be able to move a
             * file from one filesystem to another, it might not be atomic, and it
             * might not succeed if a file with the destination abstract pathname
             * already exists.  The return value should always be checked to make sure
             * that the rename operation was successful.  As instances of <b> File</b>
             * are immutable, this File object is not changed to name the destination
             * file or directory.
             *
             * @param  dest  The new abstract pathname for the named file
             *
             * @return  <b> true</b> if and only if the renaming succeeded;
             *          <b> false</b> otherwise
             */
            gbool renameTo(const File &dest) const;

            /**
             * An object that configures how to copy or move a file.
             *
             * <p> Objects of this type may be used with the
             * <b style="color: orange;"> File.copy(File, CopyOption...)</b>, and <b style="color: orange;"> File.move(File, CopyOption...)</b>
             * methods to configure how a file is copied or moved.
             */
            enum CopyOption : gshort {
                /**
                 * No option
                 */
                DEFAULT_OPTION = 0x00000000,

                /**
                 * Replace contains of destination file if it exists.
                 *
                 * Operation fail if source or destination file is directory
                 */
                REPLACE_EXISTING = 0x00000001,

                /**
                 * The copy operation is doe with encrypted I/O.
                 * is recommended for large files
                 */
                COPY_NO_BUFFERING = 0x00000002,

                /**
                 * This option is used to copy symbolic link, not target (iff The source is symbolic link)
                 * otherwise the copy is normal.
                 * <ul>
                 * <li> If CopyOption.COPY_SYMLINK is specified.
                 * <ol>
                 *    <li> If source file is symbolic link,
                 *          The destination will be the symbolic link, not the source target
                 *    <li> If source file isn't symbolic link, The destination is doing normally
                 *    <li> If destination file exists and is symbolic link, it target will be replaced
                 *    <li> If CopyOption.REPLACE_EXISTING isn't specified and destination file exists and is symbolic link,
                 *          The operation fail.
                 *  </ol>
                 * <li> If CopyOption.COPY_SYMLINK isn't specified
                 * <ol>
                 *    <li> If CopyOption.REPLACE_EXISTING isn't specified and destination file exists and is symbolic link,
                 *          The operation fail if and only if the source is symbolic link
                 *    <li> If CopyOption.REPLACE_EXISTING is specified, the operation is doing normally.
                 * </ol>
                 * </ul>
                 */
                COPY_SYMLINK = 0x00000004,

                /**
                 * The copy progress can be restarted after previous
                 * failing.
                 */
                COPY_RESTARTABLE = 0x00000008,

                /**
                 * The trying of encrypted file is success
                 * even if the destination copy can't be encrypted
                 */
                COPY_DECRYPTED_DESTINATION = 0x00000020,

                /**
                 * The move option is simulated by copying and deleting.
                 * This recommended to use this option if destination
                 * file is in other filesystem.
                 */
                MOVE_BY_COPY = 0x00000040,

                /**
                 * For future usage
                 */
                MOVE_CREATE_HARDLINK = 0x00000080,

                /**
                 * Move after system rebooting
                 */
                MOVE_UNTIL_REBOOT = 0x00000100,

                /**
                 * Move if and only if after operation the source if will be
                 * always trackable.
                 */
                MOVE_TRACKABLE = 0x00000200,
            };

            /**
             * copy the file denoted by this abstract pathname to given path.
             *
             * @param  dest  The new abstract pathname for the named file
             * @param copyOptions The options of copy
             *
             * @throws IOException If I/O exception occurs
             *
             * @return  <b> true</b> if and only if the renaming succeeded;
             *          <b> false</b> otherwise
             */
            gbool copyTo(const File &dest, gint copyOptions...) const;

            /**
             * move the file denoted by this abstract pathname to given path.
             *
             * @param  dest  The new abstract pathname for the named file
             * @param copyOptions The options of copy
             *
             * @throws IOException If I/O exception occurs
             *
             * @return  <b> true</b> if and only if the renaming succeeded;
             *          <b> false</b> otherwise
             */
            gbool moveTo(const File &dest, gint copyOptions...) const;

            /**
             * Sets the last-modified time of the file or directory named by this
             * abstract pathname.
             *
             * <p> All platforms support file-modification times to the nearest second,
             * but some provide more precision.  The argument will be truncated to fit
             * the supported precision.  If the operation succeeds and no intervening
             * operations on the file take place, then the next invocation of the
             * <b style="color: orange;"> lastModifiedTime</b> method will return the (possibly
             * truncated) <b> time</b> argument that was passed to this method.
             *
             * @param  time  The new last-modified time, measured in milliseconds since
             *               the epoch (00:00:00 GMT, January 1, 1970)
             *
             * @return <b> true</b> if and only if the operation succeeded;
             *          <b> false</b> otherwise
             *
             * @throws  IllegalArgumentException  If the argument is negative
             */
            gbool setLastModifiedTime(glong time) const;

            /**
             * Sets the last-access time of the file or directory named by this
             * abstract pathname.
             *
             * <p> All platforms support file-modification times to the nearest second,
             * but some provide more precision.  The argument will be truncated to fit
             * the supported precision.  If the operation succeeds and no intervening
             * operations on the file take place, then the next invocation of the
             * <b style="color: orange;"> lastAccessTime</b> method will return the (possibly
             * truncated) <b> time</b> argument that was passed to this method.
             *
             * @param  time  The new last-modified time, measured in milliseconds since
             *               the epoch (00:00:00 GMT, January 1, 1970)
             *
             * @return <b> true</b> if and only if the operation succeeded;
             *          <b> false</b> otherwise
             *
             * @throws  IllegalArgumentException  If the argument is negative
             */
            gbool setLastAccessTime(glong time) const;

            /**
             * Sets the creation time of the file or directory named by this
             * abstract pathname.
             *
             * <p> All platforms support file-modification times to the nearest second,
             * but some provide more precision.  The argument will be truncated to fit
             * the supported precision.  If the operation succeeds and no intervening
             * operations on the file take place, then the next invocation of the
             * <b style="color: orange;"> creationTime</b> method will return the (possibly
             * truncated) <b> time</b> argument that was passed to this method.
             *
             * @param  time  The new last-modified time, measured in milliseconds since
             *               the epoch (00:00:00 GMT, January 1, 1970)
             *
             * @return <b> true</b> if and only if the operation succeeded;
             *          <b> false</b> otherwise
             *
             * @throws  IllegalArgumentException  If the argument is negative
             */
            gbool setCreationTime(glong time) const;

            /**
             * Marks the file or directory named by this abstract pathname so that
             * only read operations are allowed. After invoking this method the file
             * or directory will not change until it is either deleted or marked
             * to allow write access. On some platforms it may be possible to start the
             * Java virtual machine with special privileges that allow it to modify
             * files that are marked read-only. Whether or not a read-only file or
             * directory may be deleted depends upon the underlying system.
             *
             * @return <b> true</b> if and only if the operation succeeded;
             *          <b> false</b> otherwise
             */
            gbool setReadOnly() const;

            /**
             * Sets the owner's or everybody's write permission for this abstract
             * pathname. On some platforms it may be possible to start the Java virtual
             * machine with special privileges that allow it to modify files that
             * disallow write operations.
             *
             * <p> The <b style="color: orange;"> java.nio.file.Files</b> class defines methods that operate on
             * file attributes including file permissions. This may be used when finer
             * manipulation of file permissions is required.
             *
             * @param   writable
             *          If <b> true</b>, sets the access permission to allow write
             *          operations; if <b> false</b> to disallow write operations
             *
             * @param   ownerOnly
             *          If <b> true</b>, the write permission applies only to the
             *          owner's write permission; otherwise, it applies to everybody.  If
             *          the underlying file system can not distinguish the owner's write
             *          permission from that of others, then the permission will apply to
             *          everybody, regardless of this value.
             *
             * @return  <b> true</b> if and only if the operation succeeded. The
             *          operation will fail if the user does not have permission to change
             *          the access permissions of this abstract pathname.
             */
            gbool setWritable(gbool writable, gbool ownerOnly) const;

            /**
             * A convenience method to set the owner's write permission for this abstract
             * pathname. On some platforms it may be possible to start the Java virtual
             * machine with special privileges that allow it to modify files that
             * disallow write operations.
             *
             * <p> An invocation of this method of the form <b> file.setWritable(arg)</b>
             * behaves in exactly the same way as the invocation
             *
             * <pre><b> 
             *     file.setWritable(arg, true)
             * </b></pre>
             *
             * @param   writable
             *          If <b> true</b>, sets the access permission to allow write
             *          operations; if <b> false</b> to disallow write operations
             *
             * @return  <b> true</b> if and only if the operation succeeded.  The
             *          operation will fail if the user does not have permission to
             *          change the access permissions of this abstract pathname.
             */
            gbool setWritable(gbool writable) const;

            /**
             * Sets the owner's or everybody's read permission for this abstract
             * pathname. On some platforms it may be possible to start the Java virtual
             * machine with special privileges that allow it to read files that are
             * marked as unreadable.
             *
             * <p> The <b style="color: orange;"> java.nio.file.Files</b> class defines methods that operate on
             * file attributes including file permissions. This may be used when finer
             * manipulation of file permissions is required.
             *
             * @param   readable
             *          If <b> true</b>, sets the access permission to allow read
             *          operations; if <b> false</b> to disallow read operations
             *
             * @param   ownerOnly
             *          If <b> true</b>, the read permission applies only to the
             *          owner's read permission; otherwise, it applies to everybody.  If
             *          the underlying file system can not distinguish the owner's read
             *          permission from that of others, then the permission will apply to
             *          everybody, regardless of this value.
             *
             * @return  <b> true</b> if and only if the operation succeeded.  The
             *          operation will fail if the user does not have permission to
             *          change the access permissions of this abstract pathname.  If
             *          <b> readable</b> is <b> false</b> and the underlying
             *          file system does not implement a read permission, then the
             *          operation will fail.
             */
            gbool setReadable(gbool readable, gbool ownerOnly) const;

            /**
             * A convenience method to set the owner's read permission for this abstract
             * pathname. On some platforms it may be possible to start the Java virtual
             * machine with special privileges that allow it to read files that are
             * marked as unreadable.
             *
             * <p>An invocation of this method of the form <b> file.setReadable(arg)</b>
             * behaves in exactly the same way as the invocation
             *
             * <pre><b> 
             *     file.setReadable(arg, true)
             * </b></pre>
             *
             * @param  readable
             *          If <b> true</b>, sets the access permission to allow read
             *          operations; if <b> false</b> to disallow read operations
             *
             * @return  <b> true</b> if and only if the operation succeeded.  The
             *          operation will fail if the user does not have permission to
             *          change the access permissions of this abstract pathname.  If
             *          <b> readable</b> is <b> false</b> and the underlying
             *          file system does not implement a read permission, then the
             *          operation will fail.
             */
            gbool setReadable(gbool readable) const;

            /**
             * Sets the owner's or everybody's execute permission for this abstract
             * pathname. On some platforms it may be possible to start the Java virtual
             * machine with special privileges that allow it to execute files that are
             * not marked executable.
             *
             * <p> The <b style="color: orange;"> java.nio.file.Files</b> class defines methods that operate on
             * file attributes including file permissions. This may be used when finer
             * manipulation of file permissions is required.
             *
             * @param   executable
             *          If <b> true</b>, sets the access permission to allow execute
             *          operations; if <b> false</b> to disallow execute operations
             *
             * @param   ownerOnly
             *          If <b> true</b>, the execute permission applies only to the
             *          owner's execute permission; otherwise, it applies to everybody.
             *          If the underlying file system can not distinguish the owner's
             *          execute permission from that of others, then the permission will
             *          apply to everybody, regardless of this value.
             *
             * @return  <b> true</b> if and only if the operation succeeded.  The
             *          operation will fail if the user does not have permission to
             *          change the access permissions of this abstract pathname.  If
             *          <b> executable</b> is <b> false</b> and the underlying
             *          file system does not implement an execute permission, then the
             *          operation will fail.
             */
            gbool setExecutable(gbool executable, gbool ownerOnly) const;

            /**
             * A convenience method to set the owner's execute permission for this
             * abstract pathname. On some platforms it may be possible to start the Java
             * virtual machine with special privileges that allow it to execute files
             * that are not marked executable.
             *
             * <p>An invocation of this method of the form <b> file.setExcutable(arg)</b>
             * behaves in exactly the same way as the invocation
             *
             * <pre><b> 
             *     file.setExecutable(arg, true)
             * </b></pre>
             *
             * @param   executable
             *          If <b> true</b>, sets the access permission to allow execute
             *          operations; if <b> false</b> to disallow execute operations
             *
             * @return   <b> true</b> if and only if the operation succeeded.  The
             *           operation will fail if the user does not have permission to
             *           change the access permissions of this abstract pathname.  If
             *           <b> executable</b> is <b> false</b> and the underlying
             *           file system does not implement an execute permission, then the
             *           operation will fail.
             */
            gbool setExecutable(gbool executable) const;

            /**
             * List the available filesystem roots.
             *
             * <p> A particular Java platform may support zero or more
             * hierarchically-organized file systems.  Each file system has a
             * <b> root</b> directory from which all other files in that file system
             * can be reached.  Windows platforms, for example, have a root directory
             * for each active drive; UNIX platforms have a single root directory,
             * namely <b> "/"</b>.  The set of available filesystem roots is affected
             * by various system-level operations such as the insertion or ejection of
             * removable media and the disconnecting or unmounting of physical or
             * virtual disk drives.
             *
             * <p> This method returns an root of <b> File</b> objects that denote the
             * root directories of the available filesystem roots.  It is guaranteed
             * that the canonical pathname of any file physically present on the local
             * machine will begin with one of the roots returned by this method.
             *
             * <p> The canonical pathname of a file that resides on some other machine
             * and is accessed via a remote-filesystem protocol such as SMB or NFS may
             * or may not begin with one of the roots returned by this method.  If the
             * pathname of a remote file is syntactically indistinguishable from the
             * pathname of a local file then it will begin with one of the roots
             * returned by this method.  Thus, for example, <b> File</b> objects
             * denoting the root directories of the mapped network drives of a Windows
             * platform will be returned by this method, while <b> File</b> objects
             * containing UNC pathnames will not be returned by this method.
             *
             * @return  An root of <b> File</b> objects denoting the available
             *          filesystem roots, or <b> empty root</b> if the set of roots could not
             *          be determined.  The root will be empty if there are no
             *          filesystem roots.
             */
            static FileArray rootFiles();

            /**
             * Returns the size of the partition <a href="">named</a> by this
             * abstract pathname. If the total number of bytes in the partition is
             * greater than <b style="color: orange;"> Long.MAX_VALUE</b>, then <b> Long.MAX_VALUE</b> will be
             * returned.
             *
             * @return  The size, in bytes, of the partition or <b> 0L</b> if this
             *          abstract pathname does not name a partition or if the size
             *          cannot be obtained
             */
            glong totalDiskSpace() const;

            /**
             * Returns the available diskSpace of the partition <a href="">named</a> by this
             * abstract pathname. If the total number of bytes in the partition is
             * greater than <b style="color: orange;"> Long.MAX_VALUE</b>, then <b> Long.MAX_VALUE</b> will be
             * returned.
             *
             * @return  The size, in bytes, of the partition or <b> 0L</b> if this
             *          abstract pathname does not name a partition or if the size
             *          cannot be obtained
             */
            glong availableDiskSpace() const;

            /**
             * Returns the usable diskSpace of the partition <a href="">named</a> by this
             * abstract pathname. If the total number of bytes in the partition is
             * greater than <b style="color: orange;"> Long.MAX_VALUE</b>, then <b> Long.MAX_VALUE</b> will be
             * returned.
             *
             * @return  The size, in bytes, of the partition or <b> 0L</b> if this
             *          abstract pathname does not name a partition or if the size
             *          cannot be obtained
             */
            glong usableDiskSpace() const;

            /**
             * <p> Creates a new empty file in the specified directory, using the
             * given prefix and suffix strings to generate its name.  If this method
             * returns successfully then it is guaranteed that:
             *
             * <ol>
             * <li> The file denoted by the returned abstract pathname did not exist
             *      before this method was invoked, and
             * <li> Neither this method nor any of its variants will return the same
             *      abstract pathname again in the current invocation of the virtual
             *      machine.
             * </ol>
             *
             * This method provides only part of a temporary-file facility.  To arrange
             * for a file created by this method to be deleted automatically, use the
             * <b style="color: orange;"> deleteOnExit</b> method.
             *
             * <p> The <b> prefix</b> argument must be at least three characters
             * long.  It is recommended that the prefix be a short, meaningful string
             * such as <b> "hjb"</b> or <b> "mail"</b>.  The
             * <b> suffix</b> argument may be <b> ""</b>, in which case the
             * suffix <b> ".tmp"</b> will be used.
             *
             * <p> To create the new file, the prefix and the suffix may first be
             * adjusted to fit the limitations of the underlying platform.  If the
             * prefix is too long then it will be truncated, but its first three
             * characters will always be preserved.  If the suffix is too long then it
             * too will be truncated, but if it begins with a period character
             * (<b> '.'</b>) then the period and the first three characters
             * following it will always be preserved.  Once these adjustments have been
             * made the name of the new file will be generated by concatenating the
             * prefix, five or more internally-generated characters, and the suffix.
             *
             * <p> If the <b> directory</b> argument is <b> ""</b> then the
             * system-dependent default temporary-file directory will be used.  The
             * default temporary-file directory is specified by the system property
             * <b> java.io.tmpdir</b>.  On UNIX systems the default value of this
             * property is typically <b> "/tmp"</b> or <b> "/var/tmp"</b>; on
             * Microsoft Windows systems it is typically <b> "C:\\WINNT\\TEMP"</b>.  A different
             * value may be given to this system property when the Java virtual machine
             * is invoked, but programmatic changes to this property are not guaranteed
             * to have any effect upon the temporary directory used by this method.
             *
             * <p> If the <b> directory</b> argument is not <b> ""</b> and its
             * abstract pathname is valid and denotes an existing, writable directory,
             * then the file will be created in that directory. Otherwise the file will
             * not be created and an <b> IOException</b> will be thrown.  Under no
             * circumstances will a directory be created at the location specified by
             * the <b> directory</b> argument.
             *
             * @param  prefix     The prefix string to be used in generating the file's
             *                    name; must be at least three characters long
             *
             * @param  suffix     The suffix string to be used in generating the file's
             *                    name; may be <b> ""</b>, in which case the
             *                    suffix <b> ".tmp"</b> will be used
             *
             * @param  directory  The directory in which the file is to be created, or
             *                    <b> ""</b> if the default temporary-file
             *                    directory is to be used
             *
             * @return  An abstract pathname denoting a newly-created empty file
             *
             * @throws  IllegalArgumentException
             *          If the <b> prefix</b> argument contains fewer than three
             *          characters
             *
             * @throws  IOException
             *          If a file could not be created
             */
            static File createTempFile(const String &prefix, const String &suffix, const File &directory);

            /**
             * Creates an empty file in the default temporary-file directory, using
             * the given prefix and suffix to generate its name. Invoking this method
             * is equivalent to invoking <b style="color: orange;"> createTempFile(prefix,&nbsp;suffix,&nbsp;File::TEMP_DIR)</b>.
             *
             * @param  prefix     The prefix string to be used in generating the file's
             *                    name; must be at least three characters long
             *
             * @param  suffix     The suffix string to be used in generating the file's
             *                    name; may be <b> ""</b>, in which case the
             *                    suffix <b> ".tmp"</b> will be used
             *
             * @return  An abstract pathname denoting a newly-created empty file
             *
             * @throws  IllegalArgumentException
             *          If the <b> prefix</b> argument contains fewer than three
             *          characters
             *
             * @throws  IOException  If a file could not be created
             */
            static File createTempFile(const String &prefix, const String &suffix);

            /**
             * Compares two abstract pathnames lexicographically.  The ordering
             * defined by this method depends upon the underlying system.  On UNIX
             * systems, alphabetic case is significant in comparing pathnames; on
             * Microsoft Windows systems it is not.
             *
             * @param   pathname  The abstract pathname to be compared to this abstract
             *                    pathname
             *
             * @return  Zero if the argument is equal to this abstract pathname, a
             *          value less than zero if this abstract pathname is
             *          lexicographically less than the argument, or a value greater
             *          than zero if this abstract pathname is lexicographically
             *          greater than the argument
             */
            gint compareTo(const File &pathname) const override;

            /**
             * Tests this abstract pathname for equality with the given object.
             * Returns <b> true</b> if and only if the argument is not
             * <b> null</b> and is an abstract pathname that is the same as this
             * abstract pathname.  Whether or not two abstract
             * pathnames are equal depends upon the underlying operating system.
             * On UNIX systems, alphabetic case is significant in comparing pathnames;
             * on Microsoft Windows systems it is not.
             *
             * @apiNote This method only tests whether the abstract pathnames are equal;
             *          it does not access the file system and the file is not required
             *          to exist.
             *
             * @param   obj   The object to be compared with this abstract pathname
             *
             * @return  <b> true</b> if and only if the objects are the same;
             *          <b> false</b> otherwise
             */
            gbool equals(const Object &obj) const override;

            /**
             * Computes a hash code for this abstract pathname.  Because equality of
             * abstract pathnames is inherently system-dependent, so is the computation
             * of their hash codes.  On UNIX systems, the hash code of an abstract
             * pathname is equal to the exclusive <em>or</em> of the hash code
             * of its pathname string and the decimal value
             * <b> 1234321</b>.  On Microsoft Windows systems, the hash
             * code is equal to the exclusive <em>or</em> of the hash code of
             * its pathname string converted to lower case and the decimal
             * value <b> 1234321</b>.  Locale is not taken into account on
             * lowercasing the pathname string.
             *
             * @return  A hash code for this abstract pathname
             */
            gint hash() const override;

            /**
             * Returns the pathname string of this abstract pathname.  This is just the
             * string returned by the <b style="color: orange;"> path</b> method.
             *
             * @return  The string form of this abstract pathname
             */
            String toString() const override;

            Object &clone() const override;

            /**
             * Return abstract pathname representing the current directory.
             * it is equivalent to call File("."_S)
             */
            static File currentDirectory();

            /**
             * Set current directory with specified abstract pathname
             */
            static gbool setCurrentDirectory(const File& f);

            /**
             * Return abstract pathname representing the path of
             * current user directory. it is equivalent to File(""_S)
             */
            static File home();

            /**
             * Return abstract pathname representing the path of
             * current user for temporary files
             */
            static File temp();
        };

    }
} // core

#endif //CORE23_FILE_H
