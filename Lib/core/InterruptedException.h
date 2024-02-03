//
// Created by T.N.Brunshweeck on 02/02/2024.
//

#ifndef CORE23_INTERRUPTEDEXCEPTION_H
#define CORE23_INTERRUPTEDEXCEPTION_H

#include <core/Exception.h>

namespace core {
    namespace concurrent {

        /**
         * Thrown when a thread is waiting, sleeping, or otherwise occupied,
         * and the thread is interrupted, either before or during the activity.
         * Occasionally a method may wish to test whether the current
         * thread has been interrupted, and if so, to immediately throw
         * this exception.  The following code can be used to achieve
         * this effect:
         * @code
         * if (Thread.interrupted())  // Clears interrupted status!
         *     throw new InterruptedException();
         *
         * @endcode
         *
         * @author  Brunshweeck Tazeussong
         */
        class InterruptedException: public Exception{
        public:

            /**
             * Constructs an <b> InterruptedException</b> with no detail  message.
             */
            InterruptedException() CORE_NOTHROW = default;

            /**
             * Constructs an <b> InterruptedException</b> with the
             * specified detail message.
             *
             * @param   s   the detail message.
             */
            CORE_EXPLICIT InterruptedException(String message) CORE_NOTHROW;

            Object &clone() const override;

        private:
            void raise() && override;
        };

    } // concurrent
} // core

#endif //CORE23_INTERRUPTEDEXCEPTION_H
