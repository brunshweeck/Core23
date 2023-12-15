//
// Created by Brunshweeck on 12/09/2023.
//

#include <typeinfo>
#include "Object.h"
#include <core/String.h>
#include "UnsupportedMethodException.h"
#include "CloneNotSupportedException.h"
#include <core/Long.h>
#include <core/private/Unsafe.h>

#if __has_include(<cxxabi.h>)

#include <cxxabi.h>
#include <core/private/Unsafe.h>

#endif

static glong objectCounter = 0;

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
        const char *rawName = t.name();
        String name = rawName;
        static String CLASS_PREFIX = "class ";
        static String CLASS_TEMPLATE_PREFIX = "<class ";
        static String CLASS_FUNCTION_PREFIX = "(class ";
        static String CLASS_NEXT_PREFIX = ",class ";

        static String UNION_PREFIX = "union ";
        static String UNION_TEMPLATE_PREFIX = "<union ";
        static String UNION_FUNCTION_PREFIX = "(union ";
        static String UNION_NEXT_PREFIX = ",union ";

        static String ENUM_PREFIX = "enum ";
        static String ENUM_TEMPLATE_PREFIX = "<enum ";
        static String ENUM_FUNCTION_PREFIX = "(enum ";
        static String ENUM_NEXT_PREFIX = ",enum ";

        static String STRUCT_PREFIX = "struct ";
        static String STRUCT_TEMPLATE_PREFIX = "<struct ";
        static String STRUCT_FUNCTION_PREFIX = "(struct ";
        static String STRUCT_NEXT_PREFIX = ",struct ";

        // remove all prefix
        if (name.startsWith(CLASS_PREFIX)) {
            name = name.subString(6);
        } else if (name.startsWith(UNION_PREFIX)) {
            name = name.subString(6);
        } else if (name.startsWith(ENUM_PREFIX)) {
            name = name.subString(5);
        } else if (name.startsWith(STRUCT_PREFIX)) {
            name = name.subString(7);
        }

        // is all prefix after beginning of template specification ('<')
        if (name.indexOf(CLASS_TEMPLATE_PREFIX) >= 0) {
            name = name.replace(CLASS_TEMPLATE_PREFIX, "<");
        }
        if (name.indexOf(UNION_TEMPLATE_PREFIX) >= 0) {
            name = name.replace(UNION_TEMPLATE_PREFIX, "<");
        }
        if (name.indexOf(ENUM_TEMPLATE_PREFIX) >= 0) {
            name = name.replace(ENUM_TEMPLATE_PREFIX, "<");
        }
        if (name.indexOf(STRUCT_TEMPLATE_PREFIX) >= 0) {
            name = name.replace(STRUCT_TEMPLATE_PREFIX, "<");
        }

        // is all prefix after beginning of function specification ('(')
        if (name.indexOf(CLASS_FUNCTION_PREFIX) >= 0) {
            name = name.replace(CLASS_FUNCTION_PREFIX, "(");
        }
        if (name.indexOf(UNION_FUNCTION_PREFIX) >= 0) {
            name = name.replace(UNION_FUNCTION_PREFIX, "(");
        }
        if (name.indexOf(ENUM_FUNCTION_PREFIX) >= 0) {
            name = name.replace(ENUM_FUNCTION_PREFIX, "(");
        }
        if (name.indexOf(STRUCT_FUNCTION_PREFIX) >= 0) {
            name = name.replace(STRUCT_FUNCTION_PREFIX, "(");
        }

        // is all prefix after beginning of coma specification (',')
        if (name.indexOf(CLASS_NEXT_PREFIX) >= 0) {
            name = name.replace(CLASS_NEXT_PREFIX, "(");
        }
        if (name.indexOf(UNION_NEXT_PREFIX) >= 0) {
            name = name.replace(UNION_NEXT_PREFIX, ",");
        }
        if (name.indexOf(ENUM_NEXT_PREFIX) >= 0) {
            name = name.replace(ENUM_NEXT_PREFIX, ",");
        }
        if (name.indexOf(STRUCT_NEXT_PREFIX) >= 0) {
            name = name.replace(STRUCT_NEXT_PREFIX, ",");
        }
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


    gint Object::hash() const { return identityHash(*this); }

    String Object::classname() const { return classname0(*this); }

    gbool Object::equals(const Object &a, const Object &b) { return a.equals(b); }

    gint Object::identityHash(const Object &x) CORE_NOTHROW {
        if (&x == &null)
            return 0;
        glong hashForClass = (glong) typeid(x).hash_code();
        glong address = (glong) &x;
        return Long::hash(hashForClass) ^ Long::hash(address);
    }


} // core