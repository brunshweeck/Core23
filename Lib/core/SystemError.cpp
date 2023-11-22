//
// Created by Brunshweeck on 13/09/2023.
//

#include "SystemError.h"

#include <utility>

namespace core {
    SystemError::SystemError(String message) CORE_NOTHROW: Error(std::move(message)) {}

    SystemError::SystemError(String message, const Throwable &cause) CORE_NOTHROW: Error(std::move(message), cause) {}

    void SystemError::raise() &&{
        throw *this;
    }
} // core