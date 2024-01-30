//
// Created by T.N.Brunshweeck on 01/01/2024.
//

#ifndef CORE23_NTFS_H
#define CORE23_NTFS_H

#include <core/io/FileSystem.h>
#include <core/io/File.h>
#include <core/StringBuffer.h>
#include <core/String.h>
#include <core/native/CharArray.h>
#include <core/native/StringArray.h>
#include <core/util/HashMap.h>
#include <core/util/ArrayList.h>

namespace core {
    namespace io {

        /**
         * NTFS class represent the local filesystem of Windows.
         * is a class
         */
        class NTFS CORE_FINAL : public FileSystem {

            gchar slash;
            gchar altSlash;
            gchar semicolon;
            String userDir;
            String tempDir;

        public:

            NTFS();

            static gbool isSlash(gchar c);

            static gbool isLetter(gchar c);

            String slashify(const String &p) const;

            /* -- Normalization and construction -- */

            gchar separator() const override;

            gchar pathSeparator() const override;

            /* Check that the given pathname is normal.  If not, invoke the real
               normalizer on the part of the pathname that requires normalization.
               This way we iterate through the whole pathname string only once. */

            String normalize(const String &path) const override;

        private:
            /* Normalize the given pathname, whose length is len, starting at the given
               offset; everything before this offset is already normal. */
            String normalize(const String &path, gint len, gint off) const;

            /* A normal Win32 pathname contains no duplicate slashes, except possibly
               for a UNC prefix, and does not end with a slash.  It may be the empty
               string.  Normalized Win32 pathnames have the convenient property that
               the length of the prefix almost uniquely identifies the type of the path
               and whether it is absolute or relative:
        
                   0  relative to both drive and directory
                   1  drive-relative (begins with '\' )
                   2  absolute UNC (if first char is '\'),
                        else directory-relative (has form "z:foo")
                   3  absolute local pathname (begins with "z:\")
             */
            gint normalizePrefix(const String &path, gint len, StringBuffer &sb) const;

        public:
            gint prefixLength(const String &path) const override;


            String resolve(const String &parent, const String &child) const override;


            String defaultParent() const override;


            String fromURIPath(const String &path) const override;

            /* -- Path operations -- */


            gbool isAbsolute(const File &f) const override;

            gbool isInvalid(const File &f) const override;

            String resolve(const File &f) const override;

            String userPath() const override;

        private:
            String drive(const String &path) const;

            static StringArray driveDirCache;

            static gint driveIndex(gchar d);

            static String driveDirectory0(gint drive);

            static String driveDirectory(gchar drive);

            // Caches for canonicalization results to improve startup performance.
            // The first cache handles repeated canonicalizations of the same path
            // name. The prefix cache handles repeated canonicalizations within the
            // same directory, and must not create results differing from the true
            // canonicalization algorithm in canonicalize_md.c. For this reason the
            // prefix cache is conservative and is not used for complex path names.
            static util::HashMap<String, String> cache;

        public:

            String canonicalize(const String &path) const override;

        private:
            String canonicalize0(const String &path) const;

            String fullPath(const String &str) const;

            String resolveReparsePoint(const String &str) const;

        public:
            /* -- Attribute accessors -- */

            /**
             * Windows File Attributes
             */
            static CORE_FAST gint FA_EXISTS = 0x00000001; // see FileSystem.h
            static CORE_FAST gint FA_REGULAR = 0x00000002; // see FileSystem.h
            static CORE_FAST gint FA_DIRECTORY = 0x00000004; // see FileSystem.h

            /**
             * The hidden file
             */
            static CORE_FAST gint FA_HIDDEN = 0x00000008; // see FileSystem.h
            static CORE_FAST gint FA_REPARSE_POINT = 0x00000010;
            static CORE_FAST gint FA_SYSTEM = 0x00000020;

            /**
             * The file will be archived
             */
            static CORE_FAST gint FA_ARCHIVE = 0x00000040;
            static CORE_FAST gint FA_DEVICE = 0x00000080;
            static CORE_FAST gint FA_READONLY = 0x00000100;
            static CORE_FAST gint FA_TEMPORARY = 0x00000200;
            static CORE_FAST gint FA_SPARSE_FILE = 0x00000400;
            static CORE_FAST gint FA_COMPRESSED = 0x00000800;
            static CORE_FAST gint FA_OFFLINE = 0x00001000;
            static CORE_FAST gint FA_NO_CONTENT_COMPRESSED = 0x00002000;

            /**
             * All data in file are encrypted
             */
            static CORE_FAST gint FA_ENCRYPTED = 0x00004000;

            /**
             * File is configured with integrity
             */
            static CORE_FAST gint FA_INTEGRITY_STREAM = 0x00008000;
            static CORE_FAST gint FA_VIRTUAL = 0x00010000;
            static CORE_FAST gint FA_NO_SCRUB_DATA = 0x00020000;
            static CORE_FAST gint FA_EA = 0x00040000;
            static CORE_FAST gint FA_PINNED = 0x00080000;
            static CORE_FAST gint FA_UNPINNED = 0x00100000;
            static CORE_FAST gint FA_RECALL_ON_OPEN = 0x00200000;
            static CORE_FAST gint FA_RECALL_ON_DATA_ACCESS = 0x00400000;

            gint attributes(const File &f) const override;

            gbool checkAccess(const File &f, gint access) const override;

            glong lastModifiedTime(const File &f) const override;

            glong lastAccessTime(const File &f) const override;

            glong creationTime(const File &f) const override;

            glong size(const File &f) const override;

            gbool setPermission(const File &f, gint access, gbool enable, gbool owneronly) const override;

            /* -- File operations -- */

            gbool createFileExclusively(const String &path) const override;

            StringArray childList(const File &f) const override;

            gbool createDirectory(const File &f) const override;

            gbool setLastModifiedTime(const File &f, glong time) const override;

            gbool setLastAccessTime(const File &f, glong time) const override;

            gbool setCreationTime(const File &f, glong time) const override;

            gbool setReadOnly(const File &f) const override;

            gbool deleteFile(const File &f) const override;

            gbool renameFile(const File &f1, const File &f2) const override;

            String ownerName(const File &f) const override;

            gbool setOwnerName(const File &f, const String &newOwner) const override;

            /* -- Filesystem interface -- */

            FileArray rootList() const override;

            /* -- Disk usage -- */

            glong diskSpace(const File &f, gint t) const override;

            /* -- Basic infrastructure -- */

            // Obtain maximum file component length from GetVolumeInformation which
            // expects the path to be null or a root component ending in a backslash
            gint nameMax(const String &path) const override;

            gint compare(const File &f1, const File &f2) const override;

            gint hash(const File &f) const override;

            String tempPath() const override;

            gbool createLink(const File &f1, const File &f2, gint linkType) override;

            gbool recycleFile(const File &f) override;

            String toString() const override;

            gbool checkAttributes(const File &f, gint attributes) const override;

            gbool copyFile(const File &f1, const File &f2, gint copyOptions) const override;

            gbool moveFile(const File &f1, const File &f2, gint copyOptions) const override;

            gbool equals(const Object &o) const override;

            Object &clone() const override;

            gbool setCurrentDirectory(const File &f) override;

        protected:
            void deleteOnExit(const File &f) override;

            static util::ArrayList<File> ExitHook;

        public:
            ~NTFS() override;

            CORE_FRATERNITY(File);
        };

    } // io
} // core

#endif //CORE23_NTFS_H
