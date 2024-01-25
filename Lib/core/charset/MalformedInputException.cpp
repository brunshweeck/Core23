//
// Created by T.N.Brunshweeck on 09/01/2024.
//

#include "MalformedInputException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        MalformedInputException::MalformedInputException(gint length) : length(length) {}

        gint MalformedInputException::inputLength() const {
            return length;
        }

        String MalformedInputException::message() const {
            return "Input length = " + String::valueOf(length);
        }

        Object &MalformedInputException::clone() const {
            return Unsafe::allocateInstance<MalformedInputException>(*this);
        }

        void MalformedInputException::raise() &&{
            throw MalformedInputException(*this);
        }
    } // charset
} // core
