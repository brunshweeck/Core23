//
// Created by Brunshweeck on 12/09/2023.
//

#include "UnsupportedMethodException.h"
#include <core/private/Unsafe.h>

namespace core {
    CORE_ALIAS(U, native::Unsafe);

    UnsupportedMethodException::UnsupportedMethodException(String message) CORE_NOTHROW:
            RuntimeException(U::moveInstance(message)) {}

    UnsupportedMethodException::UnsupportedMethodException(String message, const Throwable &cause) CORE_NOTHROW:
            RuntimeException(U::moveInstance(message), cause) {}


    void UnsupportedMethodException::raise() &&{ throw *this; }

    Object &UnsupportedMethodException::clone() const {
        return U::createInstance<UnsupportedMethodException>(*this);
    }
} // core