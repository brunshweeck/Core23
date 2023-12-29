//
// Created by T.N.Brunshweeck on 20/12/2023.
//

#ifndef CORE23_UNSUPPORTEDTEMPORALEXCEPTION_H
#define CORE23_UNSUPPORTEDTEMPORALEXCEPTION_H

#include <core/time/DateTimeException.h>

namespace core {
    namespace time {

        /**
         * UnsupportedTemporalException indicates that a temporal Field or Unit is
         * not supported for a Temporal class.
         *
         * @implSpec
         * This class is intended for use in a single thread.
         *
         */
        class UnsupportedTemporalException : public DateTimeException {
        public:

            /**
             * Constructs a new UnsupportedTemporalTypeException with the specified message.
             *
             * @param message  the message to use for this exception, may be null
             */
            UnsupportedTemporalException(String message);

            /**
             * Constructs a new UnsupportedTemporalTypeException with the specified message and cause.
             *
             * @param message  the message to use for this exception, may be null
             * @param cause  the cause of the exception, may be null
             */
            UnsupportedTemporalException(String message, const Throwable &cause);

            /**
             * Return shadow copy of this object
             */
            Object &clone() const override;

            /**
             * Throw this exception.
             */
            void raise() && override;
        };

    } // time
} // core

#endif //CORE23_UNSUPPORTEDTEMPORALEXCEPTION_H
