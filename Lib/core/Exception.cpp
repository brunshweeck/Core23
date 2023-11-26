//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "Exception.h"

namespace core {
    using native::Unsafe;

    Exception::Exception(String message) CORE_NOTHROW:
            Throwable(Unsafe::moveInstance(message)) {}

    Exception::Exception(String message, const Throwable &cause) CORE_NOTHROW:
            Throwable(Unsafe::moveInstance(message), cause) {}

    void Exception::raise() &&{
        throw *this;
    }

    Object &Exception::clone() const {
        return Unsafe::U.createInstance<Exception>(*this);
    }
} // core