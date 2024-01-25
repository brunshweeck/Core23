//
// Created by Brunshweeck on 12/09/2023.
//

#include "UnsupportedOperationException.h"
#include <core/private/Unsafe.h>

namespace core {
    using namespace native;

    UnsupportedOperationException::UnsupportedOperationException(String message) CORE_NOTHROW:
            RuntimeException(Unsafe::moveInstance(message)) {}

    UnsupportedOperationException::UnsupportedOperationException(String message, const Throwable &cause) CORE_NOTHROW:
            RuntimeException(Unsafe::moveInstance(message), cause) {}


    void UnsupportedOperationException::raise() &&{
        throw UnsupportedOperationException(*this);
    }

    Object &UnsupportedOperationException::clone() const {
        return Unsafe::allocateInstance<UnsupportedOperationException>(*this);
    }
} // core
