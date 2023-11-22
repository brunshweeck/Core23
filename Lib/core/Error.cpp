//
// Created by Brunshweeck on 12/09/2023.
//

#include "Error.h"

#include <utility>

namespace core {
    Error::Error(String message) CORE_NOTHROW: Throwable(std::move(message)) {}

    Error::Error(String message, const Throwable &cause) CORE_NOTHROW: Throwable(std::move(message), cause) {}

    void Error::raise() &&{
        throw *this;
    }
} // core