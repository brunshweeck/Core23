//
// Created by Brunshweeck on 12/09/2023.
//

#include "NoSuchElementException.h"
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    NoSuchElementException::NoSuchElementException(String message, const Throwable &cause) CORE_NOTHROW:
            RuntimeException(Unsafe::moveInstance(message), cause) {}

    NoSuchElementException::NoSuchElementException(String message) CORE_NOTHROW:
            RuntimeException(Unsafe::moveInstance(message)) {}

    void NoSuchElementException::raise() &&{
        throw NoSuchElementException(*this);
    }

    Object &NoSuchElementException::clone() const {
        return Unsafe::allocateInstance<NoSuchElementException>(*this);
    }

} // core
