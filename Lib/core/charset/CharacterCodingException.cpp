//
// Created by T.N.Brunshweeck on 09/01/2024.
//

#include "CharacterCodingException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        Object &CharacterCodingException::clone() const {
            return Unsafe::allocateInstance<CharacterCodingException>(*this);
        }

        void CharacterCodingException::raise() &&{
            throw CharacterCodingException(*this);
        }
    } // charset
} // core
