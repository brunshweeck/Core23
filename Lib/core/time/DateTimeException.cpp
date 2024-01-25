//
// Created by T.N.Brunshweeck on 20/12/2023.
//

#include "DateTimeException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace time {

        DateTimeException::DateTimeException(String message) :
                RuntimeException(Unsafe::moveInstance(message)) {}

        DateTimeException::DateTimeException(String message, const Throwable &cause) :
                RuntimeException(Unsafe::moveInstance(message), cause) {}

        Object &DateTimeException::clone() const {
            return Unsafe::allocateInstance<DateTimeException>(*this);
        }

        void DateTimeException::raise() &&{
            throw DateTimeException(*this);
        }

    } // time
} // core
