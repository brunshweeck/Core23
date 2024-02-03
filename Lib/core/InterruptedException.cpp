//
// Created by T.N.Brunshweeck on 02/02/2024.
//

#include "InterruptedException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace concurrent {
        InterruptedException::InterruptedException(String message) noexcept:
            Exception(Unsafe::moveInstance(message)) {}

        Object &InterruptedException::clone() const {
            return Unsafe::allocateInstance<InterruptedException>(*this);
        }

        void InterruptedException::raise() &&{
            throw InterruptedException(*this);
        }
    } // concurrent
} // core
