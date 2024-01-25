//
// Created by Brunshweeck on 12/09/2023.
//

#include "NumberFormatException.h"
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    NumberFormatException::NumberFormatException(String message) CORE_NOTHROW:
            IllegalArgumentException(Unsafe::moveInstance(message)) {}

    void NumberFormatException::raise() &&{
        throw NumberFormatException(*this);
    }

    Object &NumberFormatException::clone() const {
        return Unsafe::allocateInstance<NumberFormatException>(*this);
    }
} // core
