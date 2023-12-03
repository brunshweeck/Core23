//
// Created by T.N.Brunshweeck on 30/11/2023.
//

#include "ConcurrentException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace util {

        CORE_ALIAS(U, native::Unsafe);

        Object &ConcurrentException::clone() const {
            return U::U.createInstance<ConcurrentException>(*this);
        }

        void ConcurrentException::raise() &&{
            throw *this;
        }
    } // core
} // util