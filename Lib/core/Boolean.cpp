//
// Created by Brunshweeck on 12/09/2023.
//

#include "Boolean.h"
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    gbool Boolean::parseBoolean(const String &str) {
        return str.equalsIgnoreCase(u"true"_S);
    }

    Boolean Boolean::valueOf(const String &str) {
        return valueOf(parseBoolean(str));
    }

    String Boolean::toString(gbool b) {
        return b ? u"true"_S : u"false"_S;
    }

    String Boolean::toString() const {
        return toString(value);
    }

    gint Boolean::hash() const {
        return hash(value);
    }

    gbool Boolean::equals(const Object &object) const {
        if (Class<Boolean>::hasInstance(object))
            return value == ((Boolean &) object).booleanValue();
        return false;
    }

    gint Boolean::compareTo(const Boolean &other) const {
        return compare(value, other.value);
    }

    Object &Boolean::clone() const {
        return Unsafe::allocateInstance<Boolean>(value);
    }

    Boolean Boolean::valueOf(gbool b) { return b; }
} // core
