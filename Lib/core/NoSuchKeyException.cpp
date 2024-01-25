//
// Created by Brunshweeck on 12/09/2023.
//

#include "NoSuchKeyException.h"
#include <core/private/Unsafe.h>

namespace core {
    using namespace native;

    NoSuchKeyException::NoSuchKeyException(String message) CORE_NOTHROW:
            IllegalArgumentException(Unsafe::moveInstance(message)) {}

    NoSuchKeyException::NoSuchKeyException(const Object &key) CORE_NOTHROW:
            IllegalArgumentException(String::valueOf(key)) {}

    NoSuchKeyException::NoSuchKeyException(String message, const Throwable &cause) CORE_NOTHROW:
            IllegalArgumentException(Unsafe::moveInstance(message), cause) {}

    NoSuchKeyException::NoSuchKeyException(const Object &key, const Throwable &cause) CORE_NOTHROW
            : IllegalArgumentException(String::valueOf(key), cause) {}

    void NoSuchKeyException::raise() &&{
        throw NoSuchKeyException(*this);
    }

    Object &NoSuchKeyException::clone() const {
        return Unsafe::allocateInstance<NoSuchKeyException>(*this);
    }

} // core
