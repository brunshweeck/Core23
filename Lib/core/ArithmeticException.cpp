//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "ArithmeticException.h"

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    ArithmeticException::ArithmeticException(String message) CORE_NOTHROW: RuntimeException(U::moveInstance(message)) {}

    void ArithmeticException::raise() &&{ throw *this; }

    Object &ArithmeticException::clone() const {
        return U::createInstance<ArithmeticException>(*this);
    }
} // core