//
// Created by T.N.Brunshweeck on 09/01/2024.
//

#include "UnmappableCharacterException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        using namespace native;

        UnmappableCharacterException::UnmappableCharacterException(gint length): length(length) {}

        gint UnmappableCharacterException::inputLength() const {
            return length;
        }

        String UnmappableCharacterException::message() const {
            return u"Input length = "_S + String::valueOf(length);
        }

        Object &UnmappableCharacterException::clone() const {
            return Unsafe::allocateInstance<UnmappableCharacterException>(*this);
        }

        void UnmappableCharacterException::raise() &&{
            throw UnmappableCharacterException(*this);
        }
    } // charset
} // core
