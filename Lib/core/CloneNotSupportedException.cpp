//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "CloneNotSupportedException.h"

namespace core {

    using native::Unsafe;

    CloneNotSupportedException::CloneNotSupportedException(String message) CORE_NOTHROW:
        UnsupportedMethodException(Unsafe::moveInstance(message)) {}


    void CloneNotSupportedException::raise() &&{
        throw *this;
    }

    Object &CloneNotSupportedException::clone() const {
        return Unsafe::U.createInstance<CloneNotSupportedException>(*this);
    }

} // core