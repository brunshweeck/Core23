//
// Created by T.N.Brunshweeck on 20/12/2023.
//

#include "UnsupportedTemporalException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace time {
        CORE_ALIAS(U, native::Unsafe);

        UnsupportedTemporalException::UnsupportedTemporalException(String message) :
                DateTimeException(U::moveInstance(message)) {}

        UnsupportedTemporalException::UnsupportedTemporalException(String message, const Throwable &cause) :
                DateTimeException(U::moveInstance(message), cause) {}

        Object &UnsupportedTemporalException::clone() const {
            return U::createInstance<UnsupportedTemporalException>(*this);
        }

        void UnsupportedTemporalException::raise() &&{
            throw *this;
        }
    } // time
} // core