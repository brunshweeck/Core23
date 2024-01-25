//
// Created by Brunshweeck on 12/09/2023.
//

#include "IndexException.h"
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    IndexException::IndexException(String message) CORE_NOTHROW
            : IllegalArgumentException(Unsafe::moveInstance(message)) {}

    IndexException::IndexException(gint index) CORE_NOTHROW
            : IllegalArgumentException("Index out of range " + String::valueOf(index)) {}

    IndexException::IndexException(glong index) CORE_NOTHROW
            : IllegalArgumentException("Index out of range " + String::valueOf(index)) {}

    void IndexException::raise() &&{
        throw IndexException(*this);
    }

    Object &IndexException::clone() const {
        return Unsafe::allocateInstance<IndexException>(*this);
    }

} // core
