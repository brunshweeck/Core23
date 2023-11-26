//
// Created by Brunshweeck on 12/09/2023.
//

#include "UnsupportedMethodException.h"
#include <core/private/Unsafe.h>

namespace core {
    using native::Unsafe;

    UnsupportedMethodException::UnsupportedMethodException(String message) CORE_NOTHROW:
            RuntimeException(Unsafe::moveInstance(message)) {}

    UnsupportedMethodException::UnsupportedMethodException(String message, const Throwable &cause) CORE_NOTHROW
            : RuntimeException(Unsafe::moveInstance(message), cause) {}


    void UnsupportedMethodException::raise() &&{
        throw *this;
    }

    Object &UnsupportedMethodException::clone() const {
        return native::Unsafe::U.createInstance<UnsupportedMethodException>(*this);
    }
} // core