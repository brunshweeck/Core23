//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "Exception.h"

namespace core {
    CORE_ALIAS(U, native::Unsafe);

    Exception::Exception(String message) CORE_NOTHROW: Throwable(U::moveInstance(message)) {}

    Exception::Exception(String message, const Throwable &cause) CORE_NOTHROW: Throwable(U::moveInstance(message),
                                                                                         cause) {}

    void Exception::raise() &&{ throw *this; }

    Object &Exception::clone() const { return U::createInstance<Exception>(*this); }

} // core