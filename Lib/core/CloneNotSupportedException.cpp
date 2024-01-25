//
// Created by Brunshweeck on 12/09/2023.
//

#include "CloneNotSupportedException.h"
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    CloneNotSupportedException::CloneNotSupportedException(String message) CORE_NOTHROW:
            UnsupportedOperationException(Unsafe::moveInstance(message)) {}


    void CloneNotSupportedException::raise() &&{
        throw CloneNotSupportedException(*this);
    }

    Object &CloneNotSupportedException::clone() const {
        return Unsafe::allocateInstance<CloneNotSupportedException>(*this);
    }

} // core
