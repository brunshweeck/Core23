//
// Created by Brunshweeck on 12/09/2023.
//

#include "Exception.h"

#include <utility>

namespace core {
    Exception::Exception(String message) CORE_NOTHROW: Throwable(std::move(message)) {}

    Exception::Exception(String message, const Throwable &cause) CORE_NOTHROW: Throwable(std::move(message), cause) {}

    void Exception::raise() &&{
        throw *this;
    }
} // core