//
// Created by T.N.Brunshweeck on 24/01/2024.
//

#include "MissingResourceException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace util {
        MissingResourceException::MissingResourceException(String s, String className, String key) :
                RuntimeException(Unsafe::moveInstance(s)), classN(Unsafe::moveInstance(className)),
                k(Unsafe::moveInstance(key)) {}

        MissingResourceException::MissingResourceException(String s, String className, String key,
                                                           const Throwable &cause) :
                RuntimeException(Unsafe::moveInstance(s), cause), classN(Unsafe::moveInstance(className)),
                k(Unsafe::moveInstance(key)) {}

        String MissingResourceException::className() const {
            return classN;
        }

        String MissingResourceException::key() const {
            return k;
        }

        Object &MissingResourceException::clone() const {
            return Unsafe::allocateInstance<MissingResourceException>(*this);
        }

        void MissingResourceException::raise() &&{
            throw MissingResourceException(*this);
        }
    } // util
} // core
