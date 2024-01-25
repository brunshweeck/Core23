//
// Created by T.N.Brunshweeck on 24/01/2024.
//

#include "LocaleSyntaxException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace util {
        LocaleSyntaxException::LocaleSyntaxException(String msg) :
                LocaleSyntaxException(Unsafe::moveInstance(msg), 0) {}

        LocaleSyntaxException::LocaleSyntaxException(String msg, int errorIndex) :
                Exception(Unsafe::moveInstance(msg)), index(errorIndex) {}

        gint LocaleSyntaxException::errorIndex() const {
            return index;
        }

        Object &LocaleSyntaxException::clone() const {
            return Unsafe::allocateInstance<LocaleSyntaxException>(*this);
        }

        void LocaleSyntaxException::raise() &&{
            throw LocaleSyntaxException(*this);
        }

    } // util
} // core
