//
// Created by T.N.Brunshweeck on 07/01/2024.
//

#ifndef CORE23_READONLYBUFFEREXCEPTION_H
#define CORE23_READONLYBUFFEREXCEPTION_H

#include <core/UnsupportedOperationException.h>

namespace core {
    namespace io {


        /**
         * Unchecked exception thrown when a content-mutation method such as
         * <code>put</code> or <code>compact</code> is invoked upon a read-only buffer.
         */
        class ReadOnlyBufferException : public UnsupportedOperationException {
        public:
            /**
             * Constructs an instance of this class.
             */
            ReadOnlyBufferException() = default;

            Object &clone() const override;

        private:
            void raise() && override;
        };

    } // io
} // core

#endif //CORE23_READONLYBUFFEREXCEPTION_H
