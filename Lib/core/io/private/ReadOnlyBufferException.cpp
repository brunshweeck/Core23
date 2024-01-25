//
// Created by T.N.Brunshweeck on 07/01/2024.
//

#include "ReadOnlyBufferException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace io {

        Object &ReadOnlyBufferException::clone() const {
            return Unsafe::allocateInstance<ReadOnlyBufferException>(*this);
        }

        void ReadOnlyBufferException::raise() &&{
            throw ReadOnlyBufferException(*this);
        }
    } // io
} // core
