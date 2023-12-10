//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "NumberFormatException.h"

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    NumberFormatException::NumberFormatException(String message) CORE_NOTHROW:
            ArgumentException(U::moveInstance(message)) {}

    void NumberFormatException::raise() &&{ throw *this; }

    Object &NumberFormatException::clone() const {
        return U::createInstance<NumberFormatException>(*this);
    }
} // core