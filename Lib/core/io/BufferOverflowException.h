//
// Created by T.N.Brunshweeck on 05/01/2024.
//

#ifndef CORE23_BUFFEROVERFLOWEXCEPTION_H
#define CORE23_BUFFEROVERFLOWEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {
    namespace io {

        /**
         * Unchecked exception thrown when a relative <i>put</i> operation reaches
         * the target buffer's limit.
         *
         */
        class BufferOverflowException: public RuntimeException {
        public:

            /**
             * Constructs an instance of this class.
             */
            BufferOverflowException() = default;

            Object &clone() const override;

        private:
            void raise() && override;
        };

    } // io
} // core

#endif //CORE23_BUFFEROVERFLOWEXCEPTION_H
