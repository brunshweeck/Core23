//
// Created by T.N.Brunshweeck on 05/01/2024.
//

#ifndef CORE23_INVALIDMARKEXCEPTION_H
#define CORE23_INVALIDMARKEXCEPTION_H

#include <core/IllegalStateException.h>

namespace core {
    namespace io {

        /**
         * Unchecked exception thrown when an attempt is made to reset a buffer
         * when its mark is not defined.
         *
         */
        class InvalidMarkException : public IllegalStateException {
        public:

            /**
             * Constructs an instance of this class.
             */
            InvalidMarkException() = default;

            Object &clone() const override;

        private:
            void raise() && override;
        };

    } // io
} // core

#endif //CORE23_INVALIDMARKEXCEPTION_H
