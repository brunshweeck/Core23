//
// Created by Brunshweeck on 12/09/2023.
//

#include "IndexException.h"

#include <utility>
#include <core/private/Unsafe.h>

namespace core {
    IndexException::IndexException(String message) CORE_NOTHROW
            : ArgumentException(std::move(message)) {}

    IndexException::IndexException(gint index) CORE_NOTHROW
            : ArgumentException("Index out of range " + String::valueOf(index)) {}

    IndexException::IndexException(glong index) CORE_NOTHROW
            : ArgumentException("Index out of range " + String::valueOf(index)) {}

    void IndexException::raise() &&{
        throw *this;
    }

    Object &IndexException::clone() const {
        return native::Unsafe::U.createInstance<IndexException>(*this);
    }
} // core