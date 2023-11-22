//
// Created by Brunshweeck on 12/09/2023.
//

#include "IndexException.h"

#include <utility>

namespace core {
    IndexException::IndexException(String message) CORE_NOTHROW
            : ArgumentException(std::move(message)) {}

    IndexException::IndexException(gint index) CORE_NOTHROW
            : ArgumentException("Index out of range "_S.concat(String::valueOf(index))) {}

    IndexException::IndexException(glong index) CORE_NOTHROW
            : ArgumentException("Index out of range "_S.concat(String::valueOf(index))) {}

    void IndexException::raise() &&{
        throw *this;
    }
} // core