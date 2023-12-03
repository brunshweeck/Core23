//
// Created by Brunshweeck on 12/09/2023.
//

#include "Object.h"
#include <core/String.h>
#include "UnsupportedMethodException.h"
#include "CloneNotSupportedException.h"
#include <core/Integer.h>
#include <core/Long.h>
#include <typeinfo>

#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#endif

namespace {
    using namespace core;

    String classname0(const Object &o) {
        const type_info &t = typeid(o);
#if __has_include(<cxxabi.h>)
        // GCC
        const char* rawName = t.name();
        char name0[256] = {};
        size_t len = 256;
        gint status = 0;
        const char* name1 = __cxxabiv1::__cxa_demangle(rawName, name0, &len, &status);
        String name = status != 0 ? "?" : name1;
#elif defined(CORE_COMPILER_MSVC)
        // MSVC
        const char *rawName = t.raw_name();
        String name = rawName;
        if (name.startsWith("class") || name.startsWith("union"))
            name = name.subString(5);
        elif (name.startsWith("struct"))
            name = name.subString(6);
        else
            name = "$";
        name = name.replace("@", "");
#endif
        return name;
    }
}

namespace core {
    gbool Object::equals(const Object &o) const {
        if (this == &o)
            return true;
        gbyte *bytes1 = (gbyte *) this;
        gbyte *bytes2 = (gbyte *) &o;
        for (int i = 0; i < sizeof(Object); ++i) {
            if (bytes1[i] != bytes2[i])
                return false;
        }
        return false;
    }

    Object &Object::clone() const {
        CloneNotSupportedException().throws(__trace("core.Object"));
    }

    String Object::toString() const {
        glong h = hash();
        return classname() + "@" + Long::toUnsignedString(
                (h == 0 ? (glong ) typeid(*this).hash_code() : h) ^ (glong) this, 16);
    }


    gint Object::hash() const {
        return Long::hash((glong) typeid(*this).hash_code() ^ (glong ) this);
    }

    String Object::classname() const {
        return classname0(*this);
    }

    gbool Object::equals(const Object &a, const Object &b) { return a.equals(b); }

} // core