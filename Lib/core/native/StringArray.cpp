//
// Created by T.N.Brunshweeck on 14/12/2023.
//



#include <core/private/Unsafe.h>
#include <core/ArgumentException.h>
#include <core/IndexException.h>
#include <core/util/Preconditions.h>
#include "StringArray.h"
#include <new>

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

            CORE_FAST glong L(gint size) { return 1LL * size * (glong) sizeof(String); }
        }

        StringArray::StringArray(gint length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.StringArray"));
            if(length == 0) {
                value = null;
                return;
            }
            value = (STORAGE ) U::allocateMemory(L(length));
            len = length;
            for (int i = 0; i < length; ++i) {
                new (value + i) String();
            }
        }

        StringArray::StringArray(gint length, const String& initialValue)  {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.StringArray"));
            if(length == 0){
                value = null;
                return;
            }
            value = (STORAGE) U::allocateMemory(L(length));
            len = length;
            for (int i = 0; i < length; ++i) {
                new (value) String(initialValue);
            }
        }

        StringArray::StringArray(const StringArray &array)  {
            gint const length = array.length();
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.StringArray"));
            if(array.len == 0){
                value = null;
                return;
            }
            value = (STORAGE) U::allocateMemory(L(length));
            len = length;
            for (gint i = 0; i < length; ++i)
                new (value + i) String(array.value[i]);
        }

        StringArray::StringArray(StringArray &&array) CORE_NOTHROW {
            permute(value, array.value);
            permute(len, array.len);
            permute(isLocal, array.isLocal);
        }

        StringArray &StringArray::operator=(const StringArray &array) {
            if (this != &array) {
                gint const length = array.len;
                if (array.isLocal) {
                    if (!isLocal) {
                        U::freeMemory((glong) value);
                    }
                    value = array.value;
                    len = array.len;
                    isLocal = true;
                } else {
                    if(length == 0){
                        value = null;
                        return *this;
                    }
                    if (len != length) {
                        STORAGE newValue = (STORAGE) U::allocateMemory(L(length));
                        if (!isLocal) {
                            U::freeMemory((glong) value);
                            value = newValue;
                        }
                        len = length;
                    }
                    for (int i = 0; i < length; ++i) {
                        new (value + i) String(array.value[i]);
                    }
                }
            }
            return *this;
        }

        StringArray &StringArray::operator=(StringArray &&array) CORE_NOTHROW {
            if (this != &array) {
                permute(value, array.value);
                permute(len, array.len);
                permute(isLocal, array.isLocal);
            }
            return *this;
        }

        String &StringArray::get(gint index) {
            try {
                Preconditions::checkIndex(index, len);
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.native.StringArray"));
            }
        }

        const String &StringArray::get(gint index) const {
            try {
                Preconditions::checkIndex(index, len);
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.native.StringArray"));
            }
        }

        Object &StringArray::clone() const {
            return U::createInstance<StringArray>(*this);
        }

        StringArray::~StringArray() {
            if (!isLocal) {
                for (int i = 0; i < len; ++i)
                    value[i].~String();
                U::freeMemory((glong) value);
            }
            len = 0;
            value = null;
        }

        StringArray StringArray::fromAddress(glong addr, gint length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.StringArray"));
            if (addr == 0)
                ArgumentException("Null address").throws(__trace("core.native.StringArray"));
            StringArray ba = {};
            ba.value = (STORAGE) addr;
            ba.len = length;
            ba.isLocal = true;
            return ba;
        }

        gint StringArray::length() const {
            return len;
        }
    } // core
} // native


