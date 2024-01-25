//
// Created by T.N.Brunshweeck on 06/01/2024.
//

#ifndef CORE23_ILLEGALCHARSETNAMEEXCEPTION_H
#define CORE23_ILLEGALCHARSETNAMEEXCEPTION_H

#include <core/IllegalArgumentException.h>

namespace core {
    namespace charset {

        class IllegalCharsetNameException: public IllegalArgumentException {
        private:
            String charset;

        public:

            /**
             * Constructs an instance of this class.
             *
             * @param  charsetName
             *         The illegal charset name
             */
            CORE_EXPLICIT IllegalCharsetNameException(String charsetName);

            /**
             * Retrieves the illegal charset name.
             *
             * @return  The illegal charset name
             */
             virtual String charsetName() const;

            Object &clone() const override;

        private:
            void raise() && override;

        };

    } // charset
} // core

#endif //CORE23_ILLEGALCHARSETNAMEEXCEPTION_H
