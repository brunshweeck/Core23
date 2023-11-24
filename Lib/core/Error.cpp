//
// Created by Brunshweeck on 12/09/2023.
//

#include "Error.h"

#include <core/private/Unsafe.h>

namespace core {

    using native::Unsafe;


    Error::Error(String message) CORE_NOTHROW:
            Throwable(Unsafe::moveInstance(message)) {}

    Error::Error(String message, const Throwable &cause) CORE_NOTHROW:
            Throwable(Unsafe::moveInstance(message), cause) {}

    void Error::raise() &&{
        throw *this;
    }

    Object &Error::clone() const {
        return Unsafe::U.createInstance<Error>(*this);
    }

} // core