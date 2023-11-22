//
// Created by Brunshweeck on 12/09/2023.
//

#include "NumberFormatException.h"

#include <utility>

namespace core {
    NumberFormatException::NumberFormatException(String message) CORE_NOTHROW: ArgumentException(std::move(message)) {}

    void NumberFormatException::raise() &&{
        throw *this;
    }
} // core