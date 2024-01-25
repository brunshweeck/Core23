//
// Created by T.N.Brunshweeck on 05/01/2024.
//

#ifndef CORE23_BUFFERUNDERFLOWEXCEPTION_H
#define CORE23_BUFFERUNDERFLOWEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {
    namespace io {

        /**
         * Unchecked exception thrown when a relative <i>get</i> operation reaches
         * the source buffer's limit.
         */
        class BufferUnderflowException: public RuntimeException {
        public:

            /**
             * Constructs an instance of this class.
             */
            BufferUnderflowException() = default;

            Object &clone() const override;

        private:
            void raise() && override;
        };

    } // io
} // core

#endif //CORE23_BUFFERUNDERFLOWEXCEPTION_H
