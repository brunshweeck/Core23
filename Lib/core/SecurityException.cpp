//
// Created by Brunshweeck on 12/09/2023.
//

#include "SecurityException.h"
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    SecurityException::SecurityException(String message) CORE_NOTHROW:
            RuntimeException(Unsafe::moveInstance(message)) {}

    SecurityException::SecurityException(String message, const Throwable &cause) :
            RuntimeException(Unsafe::moveInstance(message)) {}


    void SecurityException::raise() &&{
        throw SecurityException(*this);
    }

    Object &SecurityException::clone() const {
        return Unsafe::allocateInstance<SecurityException>(*this);
    }
} // core
