//
// Created by Brunshweeck on 12/09/2023.
//

#include "MemoryError.h"
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    MemoryError::MemoryError(String message) CORE_NOTHROW:
            SystemError(Unsafe::moveInstance(message)) {}

    void MemoryError::raise() &&{
        throw MemoryError(*this);
    }

    Object &MemoryError::clone() const {
        return Unsafe::allocateInstance<MemoryError>(*this);
    }
} // core
