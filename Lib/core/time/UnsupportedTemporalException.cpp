//
// Created by T.N.Brunshweeck on 20/12/2023.
//

#include "UnsupportedTemporalException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace time {

        UnsupportedTemporalException::UnsupportedTemporalException(String message) :
                DateTimeException(Unsafe::moveInstance(message)) {}

        UnsupportedTemporalException::UnsupportedTemporalException(String message, const Throwable &cause) :
                DateTimeException(Unsafe::moveInstance(message), cause) {}

        Object &UnsupportedTemporalException::clone() const {
            return Unsafe::allocateInstance<UnsupportedTemporalException>(*this);
        }

        void UnsupportedTemporalException::raise() &&{
            throw UnsupportedTemporalException(*this);
        }
    } // time
} // core
