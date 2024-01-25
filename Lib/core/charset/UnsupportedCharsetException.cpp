//
// Created by T.N.Brunshweeck on 10/01/2024.
//

#include "UnsupportedCharsetException.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        using namespace native;

        UnsupportedCharsetException::UnsupportedCharsetException(String name) :
        name(Unsafe::moveInstance(name)) {}

        String UnsupportedCharsetException::charsetName() const {
            return name;
        }

        Object &UnsupportedCharsetException::clone() const {
            return Unsafe::allocateInstance<UnsupportedCharsetException>(*this);
        }

        void UnsupportedCharsetException::raise() &&{
            throw UnsupportedCharsetException(*this);
        }
    } // charset
} // core
