//
// Created by Brunshweeck on 12/09/2023.
//

#include "NoSuchItemException.h"
#include <core/private/Unsafe.h>

namespace core {

    using native::Unsafe;

    NoSuchItemException::NoSuchItemException(String message, const Throwable &cause) CORE_NOTHROW:
            RuntimeException(Unsafe::moveInstance(message), cause) {}

    NoSuchItemException::NoSuchItemException(String message) CORE_NOTHROW:
            RuntimeException(Unsafe::moveInstance(message)) {}

    void NoSuchItemException::raise() &&{
        throw *this;
    }

    Object &NoSuchItemException::clone() const {
        return native::Unsafe::U.createInstance<NoSuchItemException>(*this);
    }
} // core