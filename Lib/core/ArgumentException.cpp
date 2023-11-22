//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "ArgumentException.h"

namespace core {
    using native::Unsafe;

    ArgumentException::ArgumentException(String message) CORE_NOTHROW
            : RuntimeException(Unsafe::moveInstance(message)) {}

    ArgumentException::ArgumentException(String message, const Throwable &cause) CORE_NOTHROW
            : RuntimeException(Unsafe::moveInstance(message), cause) {}

    void ArgumentException::raise() &&{
        throw *this;
    }


    Object &ArgumentException::clone() const {
        Unsafe &U = Unsafe::U;
        return U.createInstance<ArgumentException>(*this);
    }
} // core