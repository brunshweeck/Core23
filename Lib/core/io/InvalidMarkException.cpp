//
// Created by T.N.Brunshweeck on 05/01/2024.
//

#include "InvalidMarkException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace io {

        Object &InvalidMarkException::clone() const {
            return Unsafe::allocateInstance<InvalidMarkException>(*this);
        }

        void InvalidMarkException::raise() &&{
            throw InvalidMarkException(*this);
        }
    } // io
} // core
