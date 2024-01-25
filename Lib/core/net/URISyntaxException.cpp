//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "URISyntaxException.h"
#include <core/IllegalArgumentException.h>
#include <core/private/Unsafe.h>
#include <core/StringBuffer.h>

namespace core {
    namespace net {

        using namespace native;

        URISyntaxException::URISyntaxException(String input, String reason, gint index) :
                Exception(Unsafe::moveInstance(reason)), errorInput(Unsafe::moveInstance(input)),
                errorIndex(index) {
            if (index < -1)
                IllegalArgumentException().throws(__trace("core.net.URISyntaxException"));
        }

        URISyntaxException::URISyntaxException(String input, String reason):
                URISyntaxException(Unsafe::moveInstance(input), Unsafe::moveInstance(reason), -1){}

        String URISyntaxException::input() const {
            return errorInput;
        }

        String URISyntaxException::reason() const {
            return Exception::message();
        }

        gint URISyntaxException::index() const {
            return errorIndex;
        }

        String URISyntaxException::message() const {
            StringBuffer sb = {};
            sb.append(reason());
            if (errorIndex > -1) {
                sb.append(" at index "_S);
                sb.append(errorIndex);
            }
            sb.append(": "_S);
            sb.append(errorInput);
            return sb.toString();
        }

        Object &URISyntaxException::clone() const {
            return Unsafe::allocateInstance<URISyntaxException>(*this);
        }

        void URISyntaxException::raise() &&{
            throw URISyntaxException(*this);
        }
    } // core
} // net
