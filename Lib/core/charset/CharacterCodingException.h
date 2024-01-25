//
// Created by T.N.Brunshweeck on 09/01/2024.
//

#ifndef CORE23_CHARACTERCODINGEXCEPTION_H
#define CORE23_CHARACTERCODINGEXCEPTION_H

#include <core/io/IOException.h>

namespace core {
    namespace charset {


        /**
         * Checked exception thrown when a character encoding
         * or decoding error occurs.
         */
        class CharacterCodingException : public Exception {
        public:
            /**
             * Constructs an instance of this class.
             */
            CharacterCodingException() = default;

            Object &clone() const override;

        private:
            void raise() && override;
        };

    } // charset
} // core

#endif //CORE23_CHARACTERCODINGEXCEPTION_H
