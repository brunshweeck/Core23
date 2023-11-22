//
// Created by Brunshweeck on 12/09/2023.
//

#include "UnsupportedMethodException.h"

#include <utility>

namespace core {
    UnsupportedMethodException::UnsupportedMethodException(String message) CORE_NOTHROW:
            RuntimeException(std::move(message)) {}

    UnsupportedMethodException::UnsupportedMethodException(String message, const Throwable &cause) CORE_NOTHROW
            : RuntimeException(std::move(message), cause) {}


    void UnsupportedMethodException::raise() &&{
        throw *this;
    }
} // core