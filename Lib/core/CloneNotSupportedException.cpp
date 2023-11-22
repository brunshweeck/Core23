//
// Created by Brunshweeck on 12/09/2023.
//

#include "CloneNotSupportedException.h"

#include <utility>

namespace core {
    CloneNotSupportedException::CloneNotSupportedException(String message) CORE_NOTHROW:
        UnsupportedMethodException(std::move(message)) {}


    void CloneNotSupportedException::raise() &&{
        throw *this;
    }
} // core