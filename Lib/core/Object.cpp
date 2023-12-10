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
#include <core/private/Unsafe.h>

#endif

namespace {
    using namespace core;
    using std::type_info;
    CORE_ALIAS(U, native::Unsafe);

    String classname0(const Object &o) {
        const type_info &t = typeid(o);
#if __has_include(<cxxabi.h>)
        // GCC
        const char *rawName = t.name();
        char name0[256] = {};
        size_t len = 256;
        gint status = 0;
        const char *name1 = __cxxabiv1::__cxa_demangle(rawName, name0, &len, &status);
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

    CORE_ALIAS(U, native::Unsafe);

    gbool Object::equals(const Object &o) const { return this == &o; }

    Object &Object::clone() const { CloneNotSupportedException().throws(__trace("core.Object")); }

    String Object::toString() const {
        if (this == &null) return "null";
        glong h = hash();
        return classname() + "@" + Long::toUnsignedString(
                (h == 0 ? (glong) typeid(*this).hash_code() : h) ^ (glong) this, 16);
    }


    gint Object::hash() const { return Long::hash((glong) typeid(*this).hash_code() ^ (glong) this); }

    String Object::classname() const { return classname0(*this); }

    gbool Object::equals(const Object &a, const Object &b) { return a.equals(b); }

} // core