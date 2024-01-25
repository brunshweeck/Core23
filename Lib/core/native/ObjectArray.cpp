//
// Created by T.N.Brunshweeck on 19/01/2024.
//

#include "ObjectArray.h"
#include <core/private/Unsafe.h>
#include <core/util/Preconditions.h>
#include <core/IndexException.h>

namespace core {
    namespace native {

        using namespace util;

        ObjectArray::ObjectArray() : len(0), value(null) {}

        ObjectArray::ObjectArray(gint length) : ObjectArray(length, null) {}

        ObjectArray::ObjectArray(gint length, Object &initialValue) : ObjectArray() {
            if (length < 0)
                IllegalArgumentException("Negative length").throws(__trace("core.native.ObjectArray"));
            if (length > 0) {
                try {
                    value = (VALUE) Unsafe::allocateMemory((glong) (length * sizeof(REF)));
                    for (int i = 0; i < length; i += 1) {
                        value[i] = &initialValue;
                    }
                    len = length;
                } catch (const MemoryError &error) {
                    error.throws();
                }
            }
        }

        ObjectArray::ObjectArray(gint length, const Object &initialValue) : ObjectArray() {
            if (length < 0)
                IllegalArgumentException("Negative length").throws(__trace("core.native.ObjectArray"));
            if (length > 0) {
                try {
                    value = (VALUE) Unsafe::allocateMemory((glong) (length * sizeof(REF)));
                    try {
                        Object &copy = Unsafe::copyInstance(initialValue, true);
                        for (int i = 0; i < length; i += 1) {
                            value[i] = &copy;
                        }
                        len = length;
                    } catch (const CloneNotSupportedException &ex) {
                        // value is not cloneable/copyable
                        Object &copy = (Object &) initialValue;
                        for (int i = 0; i < length; i += 1) {
                            value[i] = &copy;
                        }
                        len = length;
                    } catch (const MemoryError &error) {
                        // memory is not sufficient to clone/copy value
                        Unsafe::freeMemory((glong) value);
                        error.throws();
                    } catch (const Throwable &th) {
                        // error/exception during copy
                        Unsafe::freeMemory((glong) value);
                        th.throws(__trace("core.native.ObjectArray"));
                    }
                } catch (const MemoryError &error) {
                    error.throws();
                }
            }
        }

        ObjectArray::ObjectArray(const ObjectArray &array) : ObjectArray(array.len) {
            if (!array.isEmpty()) {
                gint const n = array.len;
                for (int i = 0; i < n; i += 1) {
                    value[i] = array.value[i];
                }
            }
        }

        ObjectArray::ObjectArray(ObjectArray &&array) CORE_NOTHROW: ObjectArray() {
            Unsafe::swapValues(value, array.value);
            Unsafe::swapValues(len, array.len);
        }

        ObjectArray &ObjectArray::operator=(const ObjectArray &array) {
            if (this != &array) {
                try {
                    ObjectArray copy{array};
                    Unsafe::swapValues(value, copy.value);
                    Unsafe::swapValues(len, copy.len);
                } catch (const MemoryError &error) {
                    error.throws();
                }
            }
            return *this;
        }

        ObjectArray &ObjectArray::operator=(ObjectArray &&array) CORE_NOTHROW {
            if (this != &array) {
                Unsafe::swapValues(value, array.value);
                Unsafe::swapValues(len, array.len);
            }
            return *this;
        }

        gint ObjectArray::length() const {
            return Math::max(len, 0);
        }

        gbool ObjectArray::isEmpty() const {
            return len <= 0 || value == null;
        }

        Object &ObjectArray::get(gint index) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                if (validateObject(*value[i])) {
                    return *value[i];
                } else {
                    ClassCastException("Could not cast to element of class " + classname()
                                       + ", the reference of type " + value[i]->classname())
                                       .throws(__trace("core.native.ObjectArray"));
                }
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.ObjectArray"));
            }
            return null;
        }

        Object const &ObjectArray::get(gint index) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                if (validateObject(*value[i])) {
                    return *value[i];
                } else {
                    ClassCastException("Could not cast to element of class " + classname()
                                       + ", the reference of type " + value[i]->classname())
                            .throws(__trace("core.native.ObjectArray"));
                }
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.ObjectArray"));
            }
            return null;
        }

        void ObjectArray::set(gint index, Object &newValue) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                if (validateObject(newValue)) {
                    value[i] = &newValue;
                } else {
                    ClassCastException("Could not cast to element of class " + classname()
                                       + ", the reference of type " + newValue.classname())
                            .throws(__trace("core.native.ObjectArray"));
                }
            } catch (const IndexException &ex) {
                ex.throws(__trace("core.native.ObjectArray"));
            }
        }

        void ObjectArray::set(gint index, const Object &newValue) {
            gint i;
            try {
                gint const n = length();
                i = Preconditions::checkIndex(index, n);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.ObjectArray"));
                return;
            }
            if (!validateObject(newValue)) {
                ClassCastException("Could not cast to element of class " + classname()
                                   + ", the reference of type " + newValue.classname())
                        .throws(__trace("core.native.ObjectArray"));
            }
            try {
                Object &copy = Unsafe::copyInstance(newValue, true);
                value[i] = &copy;
            } catch (const CloneNotSupportedException &ex) {
                // value is not cloneable/copyable
                Object &copy = (Object &) newValue;
                value[i] = &copy;
            } catch (const MemoryError &error) {
                // memory is not sufficient to clone/copy value
                error.throws();
            } catch (const Throwable &th) {
                // error/exception during copy
                th.throws(__trace("core.native.ObjectArray"));
            }
        }

        ObjectArray ObjectArray::of() {
            return {};
        }

        ObjectArray ObjectArray::of(const Object &v0) {
            try {
                ObjectArray array{1};
                array.set(0, v0);
                return Unsafe::moveInstance(array);
            } catch (const Throwable &th) {
                th.throws();
            }
            return {};
        }

        ObjectArray ObjectArray::of(const Object &v0, const Object &v1) {
            try {
                ObjectArray array{2};
                array.set(0, v0);
                array.set(1, v1);
                return Unsafe::moveInstance(array);
            } catch (const Throwable &th) {
                th.throws();
            }
            return {};
        }

        ObjectArray ObjectArray::of(const Object &v0, const Object &v1, const Object &v2) {
            try {
                ObjectArray array{3};
                array.set(0, v0);
                array.set(1, v1);
                array.set(2, v2);
                return Unsafe::moveInstance(array);
            } catch (const Throwable &th) {
                th.throws();
            }
            return {};
        }

        ObjectArray ObjectArray::of(const Object &v0, const Object &v1, const Object &v2, const Object &v3) {
            try {
                ObjectArray array{4};
                array.set(0, v0);
                array.set(1, v1);
                array.set(2, v2);
                array.set(3, v3);
                return Unsafe::moveInstance(array);
            } catch (const Throwable &th) {
                th.throws();
            }
            return {};
        }

        ObjectArray
        ObjectArray::of(const Object &v0, const Object &v1, const Object &v2, const Object &v3, const Object &v4) {
            try {
                ObjectArray array{5};
                array.set(0, v0);
                array.set(1, v1);
                array.set(2, v2);
                array.set(3, v3);
                array.set(4, v4);
                return Unsafe::moveInstance(array);
            } catch (const Throwable &th) {
                th.throws();
            }
            return {};
        }

        ObjectArray
        ObjectArray::of(const Object &v0, const Object &v1, const Object &v2, const Object &v3, const Object &v4,
                        const Object &v5) {
            try {
                ObjectArray array{6};
                array.set(0, v0);
                array.set(1, v1);
                array.set(2, v2);
                array.set(3, v3);
                array.set(4, v4);
                array.set(5, v5);
                return Unsafe::moveInstance(array);
            } catch (const Throwable &th) {
                th.throws();
            }
            return {};
        }

        ObjectArray
        ObjectArray::of(const Object &v0, const Object &v1, const Object &v2, const Object &v3, const Object &v4,
                        const Object &v5, const Object &v6) {
            try {
                ObjectArray array{7};
                array.set(0, v0);
                array.set(1, v1);
                array.set(2, v2);
                array.set(3, v3);
                array.set(4, v4);
                array.set(5, v5);
                array.set(6, v6);
                return Unsafe::moveInstance(array);
            } catch (const Throwable &th) {
                th.throws();
            }
            return {};
        }

        ObjectArray
        ObjectArray::of(const Object &v0, const Object &v1, const Object &v2, const Object &v3, const Object &v4,
                        const Object &v5, const Object &v6, const Object &v7) {
            try {
                ObjectArray array{8};
                array.set(0, v0);
                array.set(1, v1);
                array.set(2, v2);
                array.set(3, v3);
                array.set(4, v4);
                array.set(5, v5);
                array.set(6, v6);
                array.set(7, v7);
                return Unsafe::moveInstance(array);
            } catch (const Throwable &th) {
                th.throws();
            }
            return {};
        }

        ObjectArray
        ObjectArray::of(const Object &v0, const Object &v1, const Object &v2, const Object &v3, const Object &v4,
                        const Object &v5, const Object &v6, const Object &v7, const Object &v8) {
            try {
                ObjectArray array{9};
                array.set(0, v0);
                array.set(1, v1);
                array.set(2, v2);
                array.set(3, v3);
                array.set(4, v4);
                array.set(5, v5);
                array.set(6, v6);
                array.set(7, v7);
                array.set(8, v8);
                return Unsafe::moveInstance(array);
            } catch (const Throwable &th) {
                th.throws();
            }
            return {};
        }

        ObjectArray
        ObjectArray::of(const Object &v0, const Object &v1, const Object &v2, const Object &v3, const Object &v4,
                        const Object &v5, const Object &v6, const Object &v7, const Object &v8, const Object &v9) {
            try {
                ObjectArray array{10};
                array.set(0, v0);
                array.set(1, v1);
                array.set(2, v2);
                array.set(3, v3);
                array.set(4, v4);
                array.set(5, v5);
                array.set(6, v6);
                array.set(7, v7);
                array.set(8, v8);
                array.set(9, v9);
                return Unsafe::moveInstance(array);
            } catch (const Throwable &th) {
                th.throws();
            }
            return {};
        }

        gbool ObjectArray::equals(const Object &o) const {
            if (this == &o)
                return true;
            if (!Class<ObjectArray>::hasInstance(o))
                return false;
            ObjectArray const &array = CORE_DYN_CAST(const ObjectArray&, o);
            gint const n = length();
            if (n != array.length())
                return false;
            try {
                for (int i = 0; i < n; i += 1) {
                    if (get(i) != array.get(i))
                        return false;
                }
            } catch (const Exception &ex) {
                return false;
            }
            return true;
        }

        Object &ObjectArray::clone() const {
            return Object::clone();
        }

        String ObjectArray::toString() const {
            return Object::toString();
        }

        Object &ObjectArray::operator[](gint index) {
            return get(index);
        }

        Object const &ObjectArray::operator[](gint index) const {
            return get(index);
        }

        ObjectArray::CIterator::CIterator(ObjectArray &array, gint cursor) :
                array(array), cursor(cursor), limit(array.length()) {}

        gbool ObjectArray::CIterator::equals(const Object &o) const {
            if (this == &o)
                return true;
            if (!Class<CIterator>::hasInstance(o))
                return false;
            CIterator const &it = (const CIterator &) o;
            return &array == &it.array && cursor == it.cursor;
        }

        gbool ObjectArray::CIterator::hasNext() const {
            return cursor < limit;
        }

        Object &ObjectArray::CIterator::next() {
            if (!hasNext())
                NoSuchElementException().throws(__trace("core.native.ObjectArray.CIterator"));
            cursor += 1;
            return array[cursor - 1];
        }
    } // native
} // core
