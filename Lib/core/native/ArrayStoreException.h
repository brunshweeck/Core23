//
// Created by T.N.Brunshweeck on 02/02/2024.
//

#ifndef CORE23_ARRAYSTOREEXCEPTION_H
#define CORE23_ARRAYSTOREEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {
    namespace native {
        
        /**
         * Thrown to indicate that an attempt has been made to store the
         * wrong type of object into an array of objects. For example, the
         * following code generates an <b> ArrayStoreException</b> :
         * <blockquote><pre> @code
         *     ObjectArray& a = Unsafe::allocateInstance<StringArray>(3);
         *     a.set(Integer::valueOf(0));
         * @endcode </pre></blockquote>
         */
        class ArrayStoreException: public RuntimeException {
        public:

            /**
             * Constructs an <b> ArrayStoreException</b>  with no detail message.
             */
            ArrayStoreException() CORE_NOTHROW = default;

            /**
             * Constructs an <b> ArrayStoreException</b>  with the specified
             * detail message.
             *
             * @param   s   the detail message.
             */
            CORE_EXPLICIT ArrayStoreException(String s) CORE_NOTHROW;

            Object &clone() const override;

        private:
            void raise() && override;
        };

    } // native
} // core

#endif //CORE23_ARRAYSTOREEXCEPTION_H
