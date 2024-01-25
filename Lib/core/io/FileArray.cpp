//
// Created by T.N.Brunshweeck on 29/12/2023.
//

#include "FileArray.h"
#include <core/private/Unsafe.h>
#include <core/io/File.h>
#include <core/util/Preconditions.h>
#include <core/IndexException.h>
#include <core/StringBuffer.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        FileArray::FileArray(gint length) : value(null) {
            if (length < 0)
                IllegalArgumentException("Negative root size").throws(__trace("core.io.FileArray"));
            value = (FileRefs) Unsafe::allocateMemory((len = length) * (glong) sizeof(File));
            for (gint i = 0; i < length; ++i) {
                Unsafe::initializeInstance<File>((glong) (value + i), "");
            }
        }

        FileArray::FileArray(const FileArray &files) : value(null) {
            if (files.len > 0) {
                value = (FileRefs) Unsafe::allocateMemory((len = files.len) * (glong) sizeof(File));
                for (gint i = 0; i < len; ++i) {
                    Unsafe::initializeInstance<File>((glong) (value + i), files.value[i]);
                }
            }
        }

        FileArray::FileArray(FileArray &&files) CORE_NOTHROW {
            Unsafe::swapValues(len, files.len);
            Unsafe::swapValues(value, files.value);
        }

        FileArray &FileArray::operator=(const FileArray &files) {
            if (this != &files) {
                if (len < files.len) {
                    FileRefs refs = (FileRefs) Unsafe::allocateMemory(files.len * (glong) sizeof(File));
                    // initialize copy
                    for (gint i = 0; i < files.len; ++i) {
                        Unsafe::initializeInstance<File>((glong) (refs + i), files.value[i]);
                    }
                    // destroy old root
                    for (gint i = 0; i < len; ++i) {
                        value[i].~File();
                    }
                    Unsafe::swapValues(refs, value);
                    len = files.len;
                    Unsafe::freeMemory((glong) refs);
                } else {
                    // no reallocate memory
                    gint const remaining = len - files.len;
                    len = files.len;
                    for (gint i = 0; i < files.len; ++i) {
                        // destroy old value
                        value[i].~File();
                        // set copy
                        Unsafe::initializeInstance<File>((glong) (value + i), files.value[i]);
                    }
                    // destroy remaining values
                    for (gint i = 0; i < remaining; ++i) {
                        value[i + files.len].~File();
                    }
                }
            }
            return *this;
        }

        FileArray &FileArray::operator=(FileArray &&files) CORE_NOTHROW {
            if (this != &files) {
                Unsafe::swapValues(len, files.len);
                Unsafe::swapValues(value, files.value);
            }
            return *this;
        }

        gint FileArray::length() const {
            return len;
        }

        File &FileArray::get(gint index) {
            try {
                Preconditions::checkIndex(index, len);
                return value[index];
            } catch (const IndexException &iex) { iex.throws(__trace("core.io.FileArray")); }
        }

        const File &FileArray::get(gint index) const {
            try {
                Preconditions::checkIndex(index, len);
                return value[index];
            } catch (const IndexException &iex) { iex.throws(__trace("core.io.FileArray")); }
        }

        Object &FileArray::clone() const {
            return Unsafe::allocateInstance<FileArray>(*this);
        }

        String FileArray::toString() const {
            StringBuffer sb = StringBuffer(32);
            sb.append(u'[');
            for (gint i = 0; i < len - 1; ++i) {
                sb.append(value[i]).append(", ");
            }
            if (len != 0)
                sb.append(value[len - 1]);
            sb.append(u']');
            return sb.toString();
        }

        gint FileArray::hash() const {
            gint h = 0;
            for (gint i = 0; i < len; ++i) {
                h += (value[i].hash() * 31) ^ (len - i - 1);
            }
            return h;
        }

        FileArray::~FileArray() {
            gint const n = len;
            len = 0;
            for (gint i = 0; i < n; ++i) {
                value[i].~File();
            }
            Unsafe::freeMemory((glong) value);
            value = null;
        }

        void FileArray::set(gint index, const File &file) {
            try {
                Preconditions::checkIndex(index, len);
                value[index] = file;
            } catch (const IndexException &iex) {
                iex.throws(__trace("core.io.File"));
            }
        }

        gbool FileArray::equals(const Object &o) const {
            if (this == &o)
                return true;
            if (Class<PrimitiveArray<File>>::hasInstance(o)) {
                PrimitiveArray<File> const &aa = CORE_DYN_CAST(PrimitiveArray<File> const&, o);
                if (len != aa.length()) {
                    return false;
                }
                CORE_TRY_ONLY
                ({
                     for (int i = 0; i < len; ++i) {
                         if (get(i) != aa[i])
                             return false;
                     }
                     return true;
                 })
            } else if (Class<PrimitiveArray<File &>>::hasInstance(o)) {
                PrimitiveArray<File> const &aa = CORE_DYN_CAST(PrimitiveArray<File> const&, o);
                if (len != aa.length()) {
                    return false;
                }
                CORE_TRY_ONLY
                ({
                     for (int i = 0; i < len; ++i) {
                         if (get(i) != aa[i])
                             return false;
                     }
                     return true;
                 })
            }
            return false;
        }
    } // io
} // core
