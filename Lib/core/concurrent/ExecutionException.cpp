//
// Created by T.N.Brunshweeck on 02/02/2024.
//

#include "ExecutionException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace concurrent {
        ExecutionException::ExecutionException(String message) noexcept:
            Exception(Unsafe::moveInstance(message)) {}

        ExecutionException::ExecutionException(String message, const Throwable &cause) noexcept:
            Exception(Unsafe::moveInstance(message), cause) {}

        Object &ExecutionException::clone() const {
            return Unsafe::allocateInstance<ExecutionException>(*this);
        }

        void ExecutionException::raise() &&{
            throw ExecutionException(*this);
        }

        ExecutionException::ExecutionException(const Throwable &cause) : Exception(cause) {}
    } // concurrent
} // core
