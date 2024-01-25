//
// Created by Brunshweeck on 12/09/2023.
//

#include "ClassCastException.h"
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    ClassCastException::ClassCastException(String message) CORE_NOTHROW:
            RuntimeException(Unsafe::moveInstance(message)) {}

    Object &ClassCastException::clone() const {
        return Unsafe::allocateInstance<ClassCastException>(*this);
    }

    void ClassCastException::raise() &&{
        throw ClassCastException(*this);
    }

} // core
