//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "CastException.h"

namespace core {

    using native::Unsafe;

    CastException::CastException(String message) CORE_NOTHROW:
    RuntimeException(Unsafe::moveInstance(message)) {}

    Object &CastException::clone() const {
        return Unsafe::U.createInstance<CastException>(*this);
    }

    void CastException::raise() &&{
        throw *this;
    }
} // core