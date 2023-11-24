//
// Created by Brunshweeck on 12/09/2023.
//

#include "MemoryError.h"
#include <core/private/Unsafe.h>

namespace core {
    MemoryError::MemoryError(String message) CORE_NOTHROW:
            SystemError(native::Unsafe::moveInstance(message)) {}

    void MemoryError::raise() &&{
        throw *this;
    }

    Object &MemoryError::clone() const {
        return native::Unsafe::U.createInstance<MemoryError>(*this);
    }
} // core