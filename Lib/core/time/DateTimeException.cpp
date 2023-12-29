//
// Created by T.N.Brunshweeck on 20/12/2023.
//

#include "DateTimeException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace time {
        CORE_ALIAS(U, native::Unsafe);

        DateTimeException::DateTimeException(String message) :
                RuntimeException(U::moveInstance(message)) {}

        DateTimeException::DateTimeException(String message, const Throwable &cause) :
                RuntimeException(U::moveInstance(message), cause) {}

        Object &DateTimeException::clone() const {
            return U::createInstance<DateTimeException>(*this);
        }

        void DateTimeException::raise() &&{ throw *this; }

    } // time
} // core