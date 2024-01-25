//
// Created by T.N.Brunshweeck on 24/01/2024.
//

#ifndef CORE23_MISSINGRESOURCEEXCEPTION_H
#define CORE23_MISSINGRESOURCEEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {
    namespace util {

        /**
         * Signals that a resource is missing.
         *
         * @see Exception
         * @author  Brunshweeck Tazeussong
         */
        class MissingResourceException : public RuntimeException {
        public:
            /**
             * Constructs a MissingResourceException with the specified information.
             * A detail message is a String that describes this particular exception.
             * @param s the detail message
             * @param className the name of the resource class
             * @param key the key for the missing resource.
             */
            CORE_EXPLICIT MissingResourceException(String s, String className, String key);

            /**
             * Constructs a {@code MissingResourceException} with
             * {@code message}, {@code className}, {@code key},
             * and {@code cause}. This constructor is package private for
             * use by {@code ResourceBundle.getBundle}.
             *
             * @param message
             *        the detail message
             * @param className
             *        the name of the resource class
             * @param key
             *        the key for the missing resource.
             * @param cause
             *        the cause (which is saved for later retrieval by the
             *        {@link Throwable#getCause()} method). (A null value is
             *        permitted, and indicates that the cause is nonexistent
             *        or unknown.)
             */
            CORE_EXPLICIT MissingResourceException(String message, String className, String key,
                                                   const Throwable &cause);

            /**
             * Gets parameter passed by constructor.
             *
             * @return the name of the resource class
             */
            String className() const;

            /**
             * Gets parameter passed by constructor.
             *
             * @return the key for the missing resource
             */
            String key() const;

            Object &clone() const override;

        private:
            void raise() && override;

            /**
             * The class name of the resource bundle requested by the user.
             */
            String classN;

            /**
             * The name of the specific resource requested by the user.
             */
            String k;
        };

    } // util
} // core

#endif //CORE23_MISSINGRESOURCEEXCEPTION_H
