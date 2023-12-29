//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "IOError.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace io {
        IOError::IOError(const Throwable &cause) : Error(cause.toString(), cause) {}

        Object &IOError::clone() const {
            return native::Unsafe::createInstance<IOError>(*this);
        }

        void IOError::raise() &&{
            throw *this;
        }
    }
} // core