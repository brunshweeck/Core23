//
// Created by Brunshweeck on 12/09/2023.
//

#include "IndexException.h"
#include <core/private/Unsafe.h>

namespace core {
    CORE_ALIAS(U, native::Unsafe);

    IndexException::IndexException(String message) CORE_NOTHROW
            : ArgumentException(U::moveInstance(message)) {}

    IndexException::IndexException(gint index) CORE_NOTHROW
            : ArgumentException("Index out of range " + String::valueOf(index)) {}

    IndexException::IndexException(glong index) CORE_NOTHROW
            : ArgumentException("Index out of range " + String::valueOf(index)) {}

    void IndexException::raise() &&{ throw *this; }

    Object &IndexException::clone() const { return U::createInstance<IndexException>(*this); }

} // core
