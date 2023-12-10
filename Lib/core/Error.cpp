//
// Created by Brunshweeck on 12/09/2023.
//

#include "Error.h"

#include <core/private/Unsafe.h>

namespace core {

    CORE_ALIAS(U, native::Unsafe);


    Error::Error(String message) CORE_NOTHROW: Throwable(U::moveInstance(message)) {}

    Error::Error(String message, const Throwable &cause) CORE_NOTHROW: Throwable(U::moveInstance(message), cause) {}

    void Error::raise() &&{ throw *this; }

    Object &Error::clone() const { return U::createInstance<Error>(*this); }

} // core