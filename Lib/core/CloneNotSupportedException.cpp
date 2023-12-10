//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "CloneNotSupportedException.h"

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    CloneNotSupportedException::CloneNotSupportedException(String message) CORE_NOTHROW:
            UnsupportedMethodException(U::moveInstance(message)) {}


    void CloneNotSupportedException::raise() &&{ throw *this; }

    Object &CloneNotSupportedException::clone() const {
        return U::createInstance<CloneNotSupportedException>(*this);
    }

} // core