//
// Created by T.N.Brunshweeck on 09/01/2024.
//

#include "CoderMalfunctionError.h"
#include <core/Exception.h>
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        CoderMalfunctionError::CoderMalfunctionError(const Exception &cause) :
            Error(cause) {}

        Object &CoderMalfunctionError::clone() const {
            return Unsafe::allocateInstance<CoderMalfunctionError>(*this);
        }

        void CoderMalfunctionError::raise() &&{
            throw CoderMalfunctionError(*this);
        }

    } // charset
} // core
