//
// Created by Brunshweeck on 12/09/2023.
//

#include "MemoryError.h"

#include <utility>

namespace core {
    MemoryError::MemoryError(String message) CORE_NOTHROW: SystemError(std::move(message)) {}

    void MemoryError::raise() &&{
        throw *this;
    }
} // core