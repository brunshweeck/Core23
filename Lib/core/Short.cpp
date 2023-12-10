//
// Created by Brunshweeck on 12/09/2023.
//

#include "Short.h"
#include "Integer.h"
#include "Long.h"
#include "NumberFormatException.h"
#include "CastException.h"
#include <core/private/Unsafe.h>

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    gint Short::hash() const {
        return hash(value);
    }

    gbool Short::equals(const Object &object) const {
        if (Class<Short>::hasInstance(object))
            return value == ((Short &) object).value;
        return false;
    }

    gint Short::compareTo(const Short& other) const {
        return compare(value, other.value);
    }

    gshort Short::parseShort(const String &s, gint base) {
        gint i;
        try {
            i = Integer::parseInt(s, base);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Short"));
        }
        if(MIN_VALUE > i || MAX_VALUE < i)
            NumberFormatException("Value out of range, for input \"" + s + "\"").throws(__trace("core.Short"));
        return (gshort) i;
    }

    gshort Short::parseShort(const String &s) {
        gshort i;
        try {
            i = parseShort(s, 10);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Short"));
        }
        return i;
    }

    Short Short::valueOf(const String &s, gint base) {
        gshort i;
        try {
            i = parseShort(s, base);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Short"));
        }
        return valueOf(i);
    }

    Short Short::valueOf(const String& s) {
        try {
            return valueOf(s, 10);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Short"));
        }
    }

    Short Short::decode(const String& s) {
        gint i;
        try {
            i = Integer::decode(s);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Short"));
        }
        if(MIN_VALUE > i || MAX_VALUE < i)
            NumberFormatException("Value out of range, for input \"" + s + "\"").throws(__trace("core.Short"));
        return (gshort) i;
    }

    String Short::toString() const {
        return toString(value);
    }

    String Short::toString(gshort b) {
        return Integer::toString((gint)b);
    }

    Object &Short::clone() const {
        return U::createInstance<Short>(*this);
    }

    Short Short::valueOf(gshort i) {
        return i;
    }
} // core