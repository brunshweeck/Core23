//
// Created by T.N.Brunshweeck on 10/01/2024.
//

#ifndef CORE23_UNSUPPORTEDCHARSETEXCEPTION_H
#define CORE23_UNSUPPORTEDCHARSETEXCEPTION_H

#include <core/IllegalArgumentException.h>


namespace core {
    namespace charset {


        /**
         * Unchecked exception thrown when no support is available
         * for a requested charset.
         */
        class UnsupportedCharsetException : public IllegalArgumentException {
        private:
            /**
             * The name of the unsupported charset.
             */
            String name;
        public:

            /**
             * Constructs an instance of this class.
             *
             * @param  charsetName
             *         The name of the unsupported charset
             */
            CORE_EXPLICIT UnsupportedCharsetException(String charsetName);

            /**
             * Retrieves the name of the unsupported charset.
             *
             * @return  The name of the unsupported charset
             */
            String charsetName() const;

            Object &clone() const override;

        private:
            void raise() && override;
        };

    } // charset
} // core

#endif //CORE23_UNSUPPORTEDCHARSETEXCEPTION_H
