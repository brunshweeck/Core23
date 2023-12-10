//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "ArgumentException.h"

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    ArgumentException::ArgumentException(String message) CORE_NOTHROW
            : RuntimeException(U::moveInstance(message)) {}

    ArgumentException::ArgumentException(String message, const Throwable &cause) CORE_NOTHROW
            : RuntimeException(U::moveInstance(message), cause) {}

    void ArgumentException::raise() &&{ throw *this; }


    Object &ArgumentException::clone() const {
        return U::createInstance<ArgumentException>(*this);
    }
} // core