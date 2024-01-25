//
// Created by T.N.Brunshweeck on 05/01/2024.
//

#include "BufferOverflowException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace io {

        using namespace native;

        Object &BufferOverflowException::clone() const {
            return Unsafe::allocateInstance<BufferOverflowException>(*this);
        }

        void BufferOverflowException::raise() &&{
            throw BufferOverflowException(*this);
        }
    } // io
} // core
