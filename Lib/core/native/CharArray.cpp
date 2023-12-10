//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#include <core/private/Unsafe.h>
#include <core/ArgumentException.h>
#include <core/util/Preconditions.h>
#include <core/IndexException.h>
#include "CharArray.h"

namespace core {
    namespace native {

        CORE_ALIAS(U, native::Unsafe);
        using util::Preconditions;

        namespace {
            template<class T, class K>
            void permute(T &t, K &k) {
                T t2 = t;
                t = k;
                k = t2;
            }
        }

        CharArray::CharArray(gint length) : CharArray(length, false) {}

        CharArray::CharArray(gint length, gchar initialValue) : Array<Character>(length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.CharArray"));
            value = (STORAGE) U::allocateMemory(length * (glong) U::ARRAY_CHAR_INDEX_SCALE);
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = initialValue;
        }

        CharArray::CharArray(const CharArray &array) : Array<Character>(0) {
            gint length = array.length();
            if (len < 0)
                ArgumentException("Negative array length").throws(__trace("core.CharArray"));
            value = (STORAGE) U::allocateMemory(length * (glong) U::ARRAY_CHAR_INDEX_SCALE);
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = array.value[i];
        }

        CharArray::CharArray(CharArray &&array) CORE_NOTHROW: Array<Character>(0) {
            permute(value, array.value);
            permute(len, array.len);
        }

        CharArray &CharArray::operator=(const CharArray &array) {
            if (this != &array) {
                gint length = array.len;
                if (len != length) {
                    value = (STORAGE) U::reallocateMemory((glong) value,
                                                                 length * U::ARRAY_CHAR_INDEX_SCALE);
                    len = length;
                }
                for (gint i = 0; i < length; ++i)
                    value[i] = array.value[i];
            }
            return *this;
        }

        CharArray &CharArray::operator=(CharArray &&array) CORE_NOTHROW {
            if (this != &array) {
                permute(value, array.value);
                permute(len, array.len);
            }
            return *this;
        }

        gchar &CharArray::get(gint index) {
            try {
                Preconditions::checkIndex(index, Array<Character>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.CharArray"));
            }
        }

        const gchar CharArray::get(gint index) const {
            try {
                Preconditions::checkIndex(index, Array<Character>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.CharArray"));
            }
        }

        Object &CharArray::clone() const {
            return U::createInstance<CharArray>(*this);
        }

        CharArray::~CharArray() {
            U::freeMemory((glong) value);
            value = null;
        }
    } // core
} // native