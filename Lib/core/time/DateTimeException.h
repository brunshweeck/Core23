//
// Created by T.N.Brunshweeck on 20/12/2023.
//

#ifndef CORE23_DATETIMEEXCEPTION_H
#define CORE23_DATETIMEEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {
    namespace time {

        /**
         * Exception used to indicate a problem while calculating a date-time.
         * <p>
         * This exception is used to indicate problems with creating, querying
         * and manipulating date-time objects.
         *
         * @implSpec
         * This class is intended for use in a single thread.
         *
         */
        class DateTimeException : public RuntimeException {
        public:

            /**
             * Constructs a new date-time exception with the specified message.
             *
             * @param message  the message to use for this exception
             */
            CORE_EXPLICIT DateTimeException(String message);

            /**
             * Constructs a new date-time exception with the specified message and cause.
             *
             * @param message  the message to use for this exception
             * @param cause  the cause of the exception
             */
            CORE_EXPLICIT DateTimeException(String message, const Throwable &cause);

            /**
             * Return shadow copy of this object
             */
            Object &clone() const override;

        private:
            /**
             * Throw this exception.
             */
            void raise() && override;
        };

    } // time
} // core

#endif //CORE23_DATETIMEEXCEPTION_H
