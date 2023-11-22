//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "ArithmeticException.h"

namespace core {
    using native::Unsafe;

    ArithmeticException::ArithmeticException(String message) CORE_NOTHROW:
    RuntimeException(Unsafe::moveInstance(message)) {}

    void ArithmeticException::raise() &&{
        throw *this;
    }

    Object &ArithmeticException::clone() const {
        Unsafe &U = Unsafe::U;
        return U.createInstance<ArithmeticException>(*this);
    }
} // core