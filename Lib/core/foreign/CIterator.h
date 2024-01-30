//
// Created by T.N.Brunshweeck on 29/01/2024.
//

#ifndef CORE23_CITERATOR_H
#define CORE23_CITERATOR_H

#include <core/function/BiPredicate.h>
#include <core/function/BiFunction.h>
#include <core/AssertionError.h>

namespace core {
    namespace foreign {

        using namespace function;

        /**
         * Iterator used for structure 'for-loop'
         */
        template<class E>
        class CIterator CORE_FINAL : public Object {
        private:
            Object &This; // non null
            Object &hasMore; // null if is end mark (returned by end() method
            Object &nextValue; // null if is end mark (returned by end() method
            LongArray fields; // pointers and/or indexes (empty if iterator not require fields)


        public:
            // called by begin() method
            CORE_EXPLICIT CIterator(Object &obj, Object &hasMore, Object &nextValue, LongArray fields) :
                    This(obj), hasMore(hasMore), nextValue(nextValue), fields(Unsafe::moveInstance(fields)) {
                CORE_ASSERT_IF(obj != null, "CIterator require non null object", "core.foreign.CIterator");
                CORE_ASSERT_IF((Class<BiPredicate<Object &, Object>>::hasInstance(hasMore)), "Illegal predicate",
                               "core.foreign.CIterator");
                CORE_ASSERT_IF((Class<BiFunction<Object &, Object &, E &>>::hasInstance(nextValue)),
                               "Illegal function", "core.foreign.CIterator");
            }

            // called by end() method
            CORE_EXPLICIT CIterator(Object &obj) : This(obj), hasMore(null), nextValue(null), fields() {
                CORE_ASSERT_IF(obj != null, "CIterator require non null object", "core.foreign.CIterator");
            }

            //
            gbool isEnd() const {
                if (null == nextValue || null == hasMore) {
                    return true;
                }
                if (!Class<BiPredicate<Object &, Object>>::hasInstance(hasMore)) {
                    return false;
                }
                BiPredicate<Object &, Object> const &hasNextPredicate = (const BiPredicate<Object &, Object> &) hasMore;
                if (fields.isEmpty())
                    return !hasNextPredicate.test(This, null);
                else
                    return !hasNextPredicate.test(This, fields);
            }

            CIterator &operator++() {
                return *this;
            }

            E &operator*() {
                CORE_ASSERT((Class<BiFunction<Object &, Object &, E &>>::hasInstance(nextValue)),
                            "core.foreign.CIterator");
                BiFunction<Object &, Object &, E &> const &getValue = (const BiFunction<Object &, Object &, E &> &) nextValue;
                return getValue.apply(This, fields);
            }

            gbool equals(const Object &o) const override {
                if (this == &o) {
                    return true;
                }
                if (!Class<CIterator<E>>::hasInstance(o)) {
                    return false;
                }
                CIterator<E> const &it = (const CIterator<E> &) o;
                if (&it.This != &This)
                    return false;
                if (isEnd())
                    return it.isEnd();
                return fields.equals(it.fields);
            }

            ~CIterator() override {
                if (null != hasMore)
                    Unsafe::destroyInstance(hasMore);
                if (null != nextValue)
                    Unsafe::destroyInstance(nextValue);
            }
        };

    } // foreign
} // core

#endif //CORE23_CITERATOR_H
