//
// Created by Brunshweeck on 12/09/2023.
//

#include "ArithmeticException.h"
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    ArithmeticException::ArithmeticException(String message) CORE_NOTHROW:
            RuntimeException(Unsafe::moveInstance(message)) {}

    void ArithmeticException::raise() &&{
        throw ArithmeticException(*this);
    }

    Object &ArithmeticException::clone() const {
        return Unsafe::allocateInstance<ArithmeticException>(*this);
    }
} // core
