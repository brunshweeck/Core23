//
// Created by Brunshweeck on 12/09/2023.
//

#include "Byte.h"
#include <core/NumberFormatException.h>
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    gbyte Byte::parseByte(const String &s, gint base) {
        gint i = {};
        try {
            i = Integer::parseInt(s, base);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Byte"));
        }
        if (i < MIN_VALUE || i > MAX_VALUE)
            NumberFormatException(R"(Value out of range for input ")" + s + "\"").throws(__trace("core.Byte"));
        return (gbyte) i;
    }

    gbyte Byte::parseByte(const String &s) {
        try {
            return parseByte(s, 10);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Byte"));
        }
    }

    Byte Byte::valueOf(const String &s, gint base) {
        try {
            return valueOf(parseByte(s, base));
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Byte"));
        }
    }

    Byte Byte::valueOf(const String &s) {
        try {
            return valueOf(s, 10);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Byte"));
        }
    }

    Byte Byte::decode(const String &s) {
        gint i = {};
        try {
            i = Integer::decode(s);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Byte"));
        }
        if (i < MIN_VALUE || i > MAX_VALUE)
            NumberFormatException(R"(Value out of range for input ")" + s + "\"").throws(__trace("core.Byte"));
        return (gbyte) i;
    }

    String Byte::toString() const {
        return toString(value);
    }

    String Byte::toString(gbyte b) {
        return Integer::toString(b);
    }

    gint Byte::hash() const {
        return hash(value);
    }

    gbool Byte::equals(const Object &object) const {
        if (this == &object)
            return true;
        if (Class<Byte>::hasInstance(object))
            return value == ((Byte &) object).value;
        return false;
    }

    gint Byte::compareTo(const Byte &other) const {
        return compare(value, other.value);
    }

    Object &Byte::clone() const {
        return Unsafe::allocateInstance<Byte>(value);
    }

    Byte Byte::valueOf(gbyte b) { return b; }
} // core
