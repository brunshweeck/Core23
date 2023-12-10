//
// Created by Brunshweeck on 12/09/2023.
//

#include "MemoryError.h"
#include <core/private/Unsafe.h>

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    MemoryError::MemoryError(String message) CORE_NOTHROW:
            SystemError(U::moveInstance(message)) {}

    void MemoryError::raise() &&{ throw *this; }

    Object &MemoryError::clone() const { return U::createInstance<MemoryError>(*this); }
} // core