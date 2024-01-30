//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_FILESYSTEM_H
#define CORE23_FILESYSTEM_H

#include <core/Object.h>

namespace core {
    namespace io {

        interface FileSystem : public Object {

            /* -- Normalization and construction -- */

            /**
             * Return the local filesystem's name-separator character.
             */
            virtual gchar separator() const = 0;

            /**
             * Return the local filesystem's path-separator character.
             */
            virtual gchar pathSeparator() const = 0;

            /**
             * Convert the given pathname string to normal form.  If the string is
             * already in normal form then it is simply returned.
             */
            virtual String normalize(const String &path) const = 0;

            /**
             * Compute the length of this pathname string's prefix.  The pathname
             * string must be in normal form.
             */
            virtual gint prefixLength(const String &path) const = 0;

            /**
             * Resolve the child pathname string against the parent.
             * Both strings must be in normal form, and the result
             * will be in normal form.
             */
            virtual String resolve(const String &parent, const String &child) const = 0;

            /**
             * Return the parent pathname string to be used when the parent-directory
             * argument in one of the two-argument File constructors is the empty
             * pathname.
             */
            virtual String defaultParent() const = 0;

            /**
             * Post-process the given URI path string if necessary.  This is used on
             * win32, e.g., to transform "/c:/foo" into "c:/foo".  The path string
             * still has slash separators; code in the File class will translate them
             * after this method returns.
             */
            virtual String fromURIPath(const String &path) const = 0;


            /* -- Path operations -- */

            /**
             * Tell whether or not the given abstract pathname is absolute.
             */
            virtual gbool isAbsolute(const File &f) const = 0;

            /**
             * Tell whether the given abstract pathname is invalid.
             */
            virtual gbool isInvalid(const File &f) const = 0;

            /**
             * Resolve the given abstract pathname into absolute form.  Invoked by the
             * getAbsolutePath and getCanonicalPath methods in the File class.
             */
            virtual String resolve(const File &f) const = 0;

            virtual String canonicalize(const String &path) const = 0;

            virtual String userPath() const = 0;


            /* -- Attribute accessors -- */

            /* Constants for simple boolean attributes */
            static CORE_FAST gint FA_EXISTS = 0x00000001;
            static CORE_FAST gint FA_REGULAR = 0x00000002;
            static CORE_FAST gint FA_DIRECTORY = 0x00000004;
            static CORE_FAST gint FA_HIDDEN = 0x00000008;
            static CORE_FAST gint FA_SYMLINK = 0x00800000;

            /**
             * Return the simple gbool attributes for the file or directory denoted
             * by the given abstract pathname, or zero if it does not exist or some
             * other I/O error occurs.
             */
            virtual gint attributes(const File &f) const = 0;

            /**
             * Checks if all the given gbool attributes are true for the file or
             * directory denoted by the given abstract pathname. False if it does not
             * exist or some other I/O error occurs.
             */
            virtual gbool checkAttributes(const File &f, gint attributes) const;

            static CORE_FAST gint ACCESS_READ = 0x04;
            static CORE_FAST gint ACCESS_WRITE = 0x02;
            static CORE_FAST gint ACCESS_EXECUTE = 0x01;

            /**
             * Check whether the file or directory denoted by the given abstract
             * pathname may be accessed by this process.  The second argument specifies
             * which access, ACCESS_READ, ACCESS_WRITE or ACCESS_EXECUTE, to check.
             * Return false if access is denied or an I/O error occurs
             */
            virtual gbool checkAccess(const File &f, gint access) const = 0;

            /**
             * Set on or off the access permission (to owner only or to all) to the file
             * or directory denoted by the given abstract pathname, based on the parameters
             * enable, access and owerOnly.
             */
            virtual gbool setPermission(const File &f, gint access, gbool enable, gbool ownerOnly) const = 0;

            /**
             * Return the time at which the file or directory denoted by the given
             * abstract pathname was last modified, or zero if it does not exist or
             * some other I/O error occurs.
             */
            virtual glong lastModifiedTime(const File &f) const = 0;

            /**
             * Return the time at which the file or directory denoted by the given
             * abstract pathname was last accessed, or zero if it does not exist or
             * some other I/O error occurs.
             */
            virtual glong lastAccessTime(const File &f) const = 0;

            /**
             * Return the time at which the file or directory denoted by the given
             * abstract pathname was created, or zero if it does not exist or
             * some other I/O error occurs.
             */
            virtual glong creationTime(const File &f) const = 0;

            /**
             * Return the length in bytes of the file denoted by the given abstract
             * pathname, or zero if it does not exist, is a directory, or some other
             * I/O error occurs.
             */
            virtual glong size(const File &f) const = 0;


            /* -- File operations -- */

            /**
             * Create a new empty file with the given pathname.  Return
             * <b> true</b>  if the file was created and <b> false</b>  if a
             * file or directory with the given pathname already exists.  Throw an
             * IOException if an I/O error occurs.
             */
            virtual gbool createFileExclusively(const String &pathname) const = 0;

            /**
             * Delete the file or directory denoted by the given abstract pathname,
             * returning <b> true</b>  if and only if the operation succeeds.
             */
            virtual gbool deleteFile(const File &f) const = 0;

            /**
             * List the elements of the directory denoted by the given abstract
             * pathname.  Return an array of strings naming the elements of the
             * directory if successful; otherwise, return <b> null</b> .
             */
            virtual StringArray childList(const File &f) const = 0;

            /**
             * Create a new directory denoted by the given abstract pathname,
             * returning <b> true</b>  if and only if the operation succeeds.
             */
            virtual gbool createDirectory(const File &f) const = 0;

            /**
             * Rename the file or directory denoted by the first abstract pathname to
             * the second abstract pathname, returning <b> true</b>  if and only if
             * the operation succeeds.
             */
            virtual gbool renameFile(const File &f1, const File &f2) const = 0;

            /**
             * Copy the file or directory denoted by the first abstract pathname to
             * the second abstract pathname, returning <b> true</b>  if and only if
             * the operation succeeds.
             */
            virtual gbool copyFile(const File &f1, const File &f2, gint copyOptions) const = 0;

            /**
             * Move the file or directory denoted by the first abstract pathname to
             * the second abstract pathname, returning <b> true</b>  if and only if
             * the operation succeeds.
             */
            virtual gbool moveFile(const File &f1, const File &f2, gint copyOptions) const = 0;

            /**
             * Set the last-modified time of the file or directory denoted by the
             * given abstract pathname, returning <b> true</b>  if and only if the
             * operation succeeds.
             */
            virtual gbool setLastModifiedTime(const File &f, glong time) const = 0;

            /**
             * Set the last-modified time of the file or directory denoted by the
             * given abstract pathname, returning <b> true</b>  if and only if the
             * operation succeeds.
             */
            virtual gbool setLastAccessTime(const File &f, glong time) const = 0;

            /**
             * Set the last-modified time of the file or directory denoted by the
             * given abstract pathname, returning <b> true</b>  if and only if the
             * operation succeeds.
             */
            virtual gbool setCreationTime(const File &f, glong time) const = 0;

            /**
             * Mark the file or directory denoted by the given abstract pathname as
             * read-only, returning <b> true</b>  if and only if the operation
             * succeeds.
             */
            virtual gbool setReadOnly(const File &f) const = 0;

            /**
             * Return the name of current owner of specified file in
             * format: domainName\\accountName
             */
            virtual String ownerName(const File &f) const = 0;

            /**
             * Set the name of current owner of specified file in
             * format: domainName\\accountName or accountName
             */
            virtual gbool setOwnerName(const File &f, const String &newOwner) const = 0;


            /* -- Filesystem interface -- */

            /**
             * List the available filesystem roots.
             */
            virtual FileArray rootList() const = 0;

            /* -- Disk usage -- */
            static CORE_FAST gint SPACE_TOTAL = 0;
            static CORE_FAST gint SPACE_FREE = 1;
            static CORE_FAST gint SPACE_USABLE = 2;

            virtual glong diskSpace(const File &f, gint t) const = 0;

            /* -- Basic infrastructure -- */

            /**
             * Retrieve the maximum length of a component of a file path.
             *
             * @return The maximum length of a file path component.
             */
            virtual gint nameMax(const String &path) const = 0;

            /**
             * Compare two abstract pathnames lexicographically.
             */
            virtual gint compare(const File &f1, const File &f2) const = 0;

            /**
             * Compute the hash code of an abstract pathname.
             */
            virtual gint hash(const File &f) const = 0;

            /**
             * Return the system path contains temporary files.
             */
            virtual String tempPath() const = 0;

            static CORE_FAST gint SYMBOLIC_LINK = 0;
            static CORE_FAST gint HARD_LINK = 1;
            static CORE_FAST gint SHORTCUT_LINK = 2;

            /**
             * Create link f1 with target f2
             */
            virtual gbool createLink(const File& f1, const File& f2, gint linkType) = 0;

            /**
             * Move file to trash (recycle bin)
             */
            virtual gbool recycleFile(const File& f) = 0;

            virtual gbool setCurrentDirectory(const File& f) = 0;

        protected:

            static gint prefixLength0(const File &f);

            virtual void deleteOnExit(const File &f) = 0;

            static FileSystem& defaultFileSystem();

            CORE_FRATERNITY(File);
        };

    }
} // core

#endif //CORE23_FILESYSTEM_H
