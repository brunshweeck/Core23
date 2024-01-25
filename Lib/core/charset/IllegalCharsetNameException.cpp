//
// Created by T.N.Brunshweeck on 06/01/2024.
//

#include "IllegalCharsetNameException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        IllegalCharsetNameException::IllegalCharsetNameException(String charsetName) :
                IllegalArgumentException(charsetName), charset(Unsafe::moveInstance(charsetName)) {}

        String IllegalCharsetNameException::charsetName() const {
            return charset;
        }

        Object &IllegalCharsetNameException::clone() const {
            return Unsafe::allocateInstance<IllegalCharsetNameException>(*this);
        }

        void IllegalCharsetNameException::raise() &&{
            throw IllegalCharsetNameException(*this);
        }
    } // charset
} // core
