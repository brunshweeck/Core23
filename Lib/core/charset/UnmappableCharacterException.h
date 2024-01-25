//
// Created by T.N.Brunshweeck on 09/01/2024.
//

#ifndef CORE23_UNMAPPABLECHARACTEREXCEPTION_H
#define CORE23_UNMAPPABLECHARACTEREXCEPTION_H

#include <core/charset/CharacterCodingException.h>

namespace core {
    namespace charset {

        class UnmappableCharacterException : public CharacterCodingException {
        private:

            /**
             * The length of the input.
             */
            gint length;
        public:

            /**
             * Constructs an <b> MalformedInputException</b> with the given
             * length.
             * @param inputLength the length of the input
             */
            CORE_EXPLICIT UnmappableCharacterException(gint inputLength);

            /**
             * Returns the length of the input.
             * @return the length of the input
             */
            gint inputLength() const;

            /**
             * Returns the message.
             * @return the message
             */
            String message() const override;

            Object &clone() const override;

        private:
            void raise() && override;
        };

    } // charset
} // core

#endif //CORE23_UNMAPPABLECHARACTEREXCEPTION_H
