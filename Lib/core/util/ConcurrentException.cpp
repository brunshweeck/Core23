//
// Created by T.N.Brunshweeck on 30/11/2023.
//

#include "ConcurrentException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace util {

        Object &ConcurrentException::clone() const {
            return Unsafe::allocateInstance<ConcurrentException>(*this);
        }

        void ConcurrentException::raise() &&{
            throw ConcurrentException(*this);
        }

        ConcurrentException::ConcurrentException(String message) CORE_NOTHROW:
                RuntimeException(Unsafe::moveInstance(message)) {}

        ConcurrentException::ConcurrentException(String message, const Throwable &cause) CORE_NOTHROW:
                RuntimeException(Unsafe::moveInstance(message), cause) {}
    } // core
} // util
