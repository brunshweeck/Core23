//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "CastException.h"

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    CastException::CastException(String message) CORE_NOTHROW:
    RuntimeException(U::moveInstance(message)) {}

    Object &CastException::clone() const {
        return U::createInstance<CastException>(*this);
    }

    void CastException::raise() &&{ throw *this;}
} // core