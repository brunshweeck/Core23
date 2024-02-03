//
// Created by Brunshweeck on 12/09/2023.
//

#include "Exception.h"
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    Exception::Exception(String message) CORE_NOTHROW:
            Throwable(Unsafe::moveInstance(message)) {}

    Exception::Exception(String message, const Throwable &cause) CORE_NOTHROW:
            Throwable(Unsafe::moveInstance(message), cause) {}

    void Exception::raise() &&{
        throw Exception(*this);
    }

    Object &Exception::clone() const {
        return Unsafe::allocateInstance<Exception>(*this);
    }

    Exception::Exception(const Throwable &cause) : Throwable(cause.toString(), cause) {}

} // core
