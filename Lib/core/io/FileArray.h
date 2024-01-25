//
// Created by T.N.Brunshweeck on 29/12/2023.
//

#ifndef CORE23_FILEARRAY_H
#define CORE23_FILEARRAY_H

#include <core/native/PrimitiveArray.h>
#include <core/io/File.h>

namespace core {
    namespace io {

        using namespace native;

        class FileArray CORE_FINAL : public PrimitiveArray<File> {
        private:
            /**
             * Number of file in this root
             */
            gint len = {};

            CORE_ALIAS(FileRefs, typename Class<File>::Ptr);

            FileRefs value = {};

        public:
            /**
             * Create new Empty root
             */
            CORE_FAST FileArray() = default;

            /**
             * Create new File root with the given number of place
             */
            CORE_EXPLICIT FileArray(gint length);

            FileArray(const FileArray &files);

            FileArray(FileArray &&files) CORE_NOTHROW;

            FileArray &operator=(const FileArray &files);

            FileArray &operator=(FileArray &&files) CORE_NOTHROW;

            gint length() const override;

            File &get(gint index) override;

            const File &get(gint index) const override;

            Object &clone() const override;

            String toString() const override;

            gint hash() const override;

            void set(gint index, const File &file) override;

            ~FileArray() override;

            gbool equals(const Object &o) const override;
        };

    } // io
} // core

#endif //CORE23_FILEARRAY_H
