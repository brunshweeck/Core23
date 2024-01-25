//
// Created by T.N.Brunshweeck on 05/01/2024.
//

#include "BufferUnderflowException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace io {

        Object &BufferUnderflowException::clone() const {
            return Unsafe::allocateInstance<BufferUnderflowException>(*this);
        }

        void BufferUnderflowException::raise() &&{
            throw BufferUnderflowException(*this);
        }
    } // io
} // core
