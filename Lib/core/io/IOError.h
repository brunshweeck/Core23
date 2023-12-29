//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_IOERROR_H
#define CORE23_IOERROR_H

#include <core/Error.h>

namespace core {
    namespace io {

        /**
         * Thrown when a serious I/O error has occurred.
         *
         * @author  Brunshweeck Tazeussong
         */
        class IOError : public Error {
        public:
            /**
             * Constructs a new instance of IOError with the specified cause. The
             * IOError is created with the detail message of
             * {@code cause.toString()} (which typically
             * contains the class and detail message of cause).
             *
             * @param  cause
             *         The cause of this error
             */
            CORE_EXPLICIT IOError(const Throwable &cause);

            Object &clone() const override;

        private:
            void raise() && override;
        };

    }
} // core

#endif //CORE23_IOERROR_H
