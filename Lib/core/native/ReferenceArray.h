//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_REFERENCEARRAY_H
#define CORE23_REFERENCEARRAY_H

#include <core/native/Array.h>

namespace core {

    namespace util {
        template<class> class ArrayList;
        template<class> class Vector;
        template<class> class PriorityQueue;
        template<class,class> class HashMap;
        template<class,class> class HashTable;
    }

    namespace native {
        class Unsafe;
    }

    namespace native {

        /**
         * The ReferenceArray class wrap the static array of values from native type
         * (generic) object in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this native type
         * (such as ReferenceBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class ReferenceArray CORE_FINAL : public Array<Object> {
        private:

            CORE_ALIAS(VRef, typename Class<Object>::Ptr);

            CORE_ALIAS(STORAGE, typename Class<VRef>::Ptr);

            /**
             * The array used to store references
             */
            STORAGE value = {};

            /**
             * The number of reference supported by this array
             */
            gint len = {};

            /**
             *  (transient) to simplify nested class access
             */
            template<class T> friend class util::ArrayList;
            template<class T> friend class util::Vector;
            template<class T> friend class util::PriorityQueue;
            template<class K, class V> friend class util::HashMap;
            template<class K, class V> friend class util::HashTable;
            friend Unsafe;

        public:

            /**
             * Construct new empty Reference Array
             */
            CORE_FAST ReferenceArray() = default;

            /**
             * Construct new ReferenceArray with the given number of place
             * initialized with null reference.
             *
             * @param length The number of place on this array
             * @throws ArgumentException If the given length is negative
             */
            CORE_EXPLICIT ReferenceArray(gint length);

            /**
             * Construct new ReferenceArray with the given number of place
             * initialized with given reference.
             *
             * @param length The number of place on this array
             * @param initialValue The reference used to initialize array
             * @throws ArgumentException If the given length is negative
             */
            CORE_EXPLICIT ReferenceArray(gint length, Object& initialValue);

            /**
             * Initialize newly created ReferenceArray with items of another.
             *
             * @param a The array that items are used to initialize this array
             */
            ReferenceArray(const ReferenceArray &a);

            /**
             * Initialize newly created ReferenceArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            ReferenceArray(ReferenceArray &&a) CORE_NOTHROW;

            ReferenceArray &operator=(const ReferenceArray &a);

            ReferenceArray &operator=(ReferenceArray &&a) CORE_NOTHROW;

            /**
             * Return Shadow copy of this array
             */
            Object &clone() const override;

            /**
             * Return String representation of this array
             *
             */
            String toString() const override;

            /**
             * Return number of element of this array
             */
            gint length() const override;

            /**
             * Return this value at specified index
             *
             * @param index The position of value returned
             * @throws IndexException If the given index out of this array bounds.
             */
            Object &get(gint index) override;

            const Object &get(gint index) const override;

            void set(gint index, Object &newValue);

            ~ReferenceArray() override;
        };

    } // core
} // native

#endif //CORE23_REFERENCEARRAY_H
