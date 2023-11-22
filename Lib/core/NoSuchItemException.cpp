//
// Created by Brunshweeck on 12/09/2023.
//

#include "NoSuchItemException.h"

#include <utility>

namespace core {
    NoSuchItemException::NoSuchItemException(String message, const Throwable &cause) CORE_NOTHROW:
        RuntimeException(std::move(message), cause) {}

    NoSuchItemException::NoSuchItemException(String message) CORE_NOTHROW: RuntimeException(std::move(message)) {}

    void NoSuchItemException::raise() &&{
        throw *this;
    }
} // core