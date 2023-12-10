//
// Created by Brunshweeck on 12/09/2023.
//

#include "NoSuchItemException.h"
#include <core/private/Unsafe.h>

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    NoSuchItemException::NoSuchItemException(String message, const Throwable &cause) CORE_NOTHROW:
            RuntimeException(U::moveInstance(message), cause) {}

    NoSuchItemException::NoSuchItemException(String message) CORE_NOTHROW:
            RuntimeException(U::moveInstance(message)) {}

    void NoSuchItemException::raise() &&{ throw *this; }

    Object &NoSuchItemException::clone() const {
        return U::createInstance<NoSuchItemException>(*this);
    }

} // core