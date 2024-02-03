//
// Created by T.N.Brunshweeck on 02/02/2024.
//

#include "ArrayStoreException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace native {
        ArrayStoreException::ArrayStoreException(String message) noexcept:
        RuntimeException(Unsafe::moveInstance(message)) {}

        Object &ArrayStoreException::clone() const {
            return Unsafe::allocateInstance<ArrayStoreException>(*this);
        }

        void ArrayStoreException::raise() &&{
            throw *this;
        }
    } // native
} // core
