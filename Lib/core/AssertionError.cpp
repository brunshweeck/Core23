//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "AssertionError.h"

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    AssertionError::AssertionError(String message) CORE_NOTHROW:
            Error(U::moveInstance(message)) {}

    AssertionError::AssertionError(const Object &expression) CORE_NOTHROW:
            Error(Class<Throwable>::hasInstance(expression) ? "" : String::valueOf(expression)) {
        if (Class<Throwable>::hasInstance(expression)) setCause((const Throwable &) expression);
    }

    AssertionError::AssertionError(gbool expression) CORE_NOTHROW:
            Error(String::valueOf(expression)) {}

    AssertionError::AssertionError(gchar expression) CORE_NOTHROW:
            Error(String::valueOf(expression)) {}

    AssertionError::AssertionError(gint expression) CORE_NOTHROW:
            Error(String::valueOf(expression)) {}

    AssertionError::AssertionError(glong expression) CORE_NOTHROW:
            Error(String::valueOf(expression)) {}

    AssertionError::AssertionError(gfloat expression) CORE_NOTHROW:
            Error(String::valueOf(expression)) {}

    AssertionError::AssertionError(gdouble expression) CORE_NOTHROW:
            Error(String::valueOf(expression)) {}

    AssertionError::AssertionError(String message, const Throwable &cause) CORE_NOTHROW:
            Error(U::moveInstance(message), cause) {}

    void AssertionError::raise() &&{throw *this;}

    Object &AssertionError::clone() const {return U::createInstance<AssertionError>(*this);}
} // core