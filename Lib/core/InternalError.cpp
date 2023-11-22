//
// Created by Brunshweeck on 12/09/2023.
//

#include "InternalError.h"

#include <utility>

namespace core {
    InternalError::InternalError(String message) CORE_NOTHROW: SystemError(std::move(message)) {}

    InternalError::InternalError(String message, const Throwable &cause) CORE_NOTHROW: SystemError(std::move(message), cause) {}

    void InternalError::raise() &&{
        throw *this;
    }
} // core