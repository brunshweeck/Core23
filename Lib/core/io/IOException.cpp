//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "IOException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace io {
        using namespace native;

        IOException::IOException(String message) : Exception(Unsafe::moveInstance(message)) {}

        IOException::IOException(String message, const Throwable &cause) :
                Exception(Unsafe::moveInstance(message), cause) {}

        IOException::IOException(const Throwable &cause) : Exception(cause.toString(), cause) {}

        Object &IOException::clone() const {
            return Unsafe::createInstance<IOException>(*this);
        }

        void IOException::raise() &&{
            throw *this;
        }

    }
} // core