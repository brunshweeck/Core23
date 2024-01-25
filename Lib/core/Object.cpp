//
// Created by Brunshweeck on 12/09/2023.
//

#include "Object.h"
#include <core/CloneNotSupportedException.h>
#include <core/StringBuffer.h>
#include <core/private/Unsafe.h>
#include <typeinfo>

#if __has_include(<cxxabi.h>)

#include <cxxabi.h>

#endif

namespace {
    using namespace core;
    using namespace std;

    gbool isPredecessor(gchar c) {
        return c == ',' || c == '<' || c == '(' || c == ' ';
    }
}

namespace core {

    using namespace native;

    gbool Object::equals(const Object &o) const { return this == &o; }

    Object &Object::clone() const {
        if(this == &null)
            return null;
        CloneNotSupportedException().throws(__trace("core.Object"));
    }

    String Object::toString() const {
        if (this == &null)
            return u"null"_S;
        glong const h = hash();
        return classname() + "@" + Integer::toUnsignedString(identityHash(*this) & 0xFFFF);
    }


    gint Object::hash() const { return identityHash(*this); }

    String Object::classname() const { return classname0(*this); }

    gbool Object::equals(const Object &a, const Object &b) { return a.equals(b); }

    gint Object::identityHash(const Object &x) CORE_NOTHROW {
        return identityHash0(x);
    }

    gint Object::identityHash0(const Object &obj) {
        return String(
#ifdef CORE_COMPILER_MSVC
                typeid(obj).raw_name()
#else
                typeid(obj).name()
#endif
        ).hash();
    }

    String Object::classname0(const Object &obj) {
        const type_info &t = typeid(obj);
#if __has_include(<cxxabi.h>)
        // GCC
        const char *rawName = t.name();
        char name0[256] = {};
        size_t len = 256;
        gint status = 0;
        const char *name1 = __cxxabiv1::__cxa_demangle(rawName, name0, &len, &status);
        String name = status != 0 ? "?" : name1;
        return name;
#elif defined(CORE_COMPILER_MSVC)
        // MSVC
        const char *rawName = t.name();
        StringBuffer name = StringBuffer(rawName);
        static String const CLASS_PREFIX = "class ";
        static String const UNION_PREFIX = "union ";
        static String const ENUM_PREFIX = "enum ";
        static String const STRUCT_PREFIX = "struct ";

        {
            // removing class specifier
            gint i = name.indexOf(CLASS_PREFIX);
            gint j = 0;
            while (i >= 0) {
                if (i > 0 && isPredecessor(name.charAt(i - 1))) {
                    name.remove(i, i + CLASS_PREFIX.length());
                    j = i + 1;
                    i = name.indexOf(CLASS_PREFIX, j);
                } else if (i == 0) {
                    name.remove(0, CLASS_PREFIX.length());
                    j = 0;
                    i = name.indexOf(CLASS_PREFIX, j);
                } else if (i == j) {
                    i += 1;
                } else {
                    break;
                }
            }
        }
        {
            // removing struct specifier
            gint i = name.indexOf(STRUCT_PREFIX);
            gint j = 0;
            while (i >= 0) {
                if (i > 0 && isPredecessor(name.charAt(i - 1))) {
                    name.remove(i, i + STRUCT_PREFIX.length());
                    j = i + 1;
                    i = name.indexOf(STRUCT_PREFIX, j);
                } else if (i == 0) {
                    name.remove(0, STRUCT_PREFIX.length());
                    j = 0;
                    i = name.indexOf(STRUCT_PREFIX, j);
                } else if (i == j) {
                    i += 1;
                } else {
                    break;
                }
            }
        }
        {
            // removing enum specifier
            gint i = name.indexOf(ENUM_PREFIX);
            gint j = 0;
            while (i >= 0) {
                if (i > 0 && isPredecessor(name.charAt(i - 1))) {
                    name.remove(i, i + ENUM_PREFIX.length());
                    j = i + 1;
                    i = name.indexOf(ENUM_PREFIX, j);
                } else if (i == 0) {
                    name.remove(0, ENUM_PREFIX.length());
                    j = 0;
                    i = name.indexOf(ENUM_PREFIX, j);
                } else if (i == j) {
                    i += 1;
                } else {
                    break;
                }
            }
        }
        {
            // removing union specifier
            gint i = name.indexOf(UNION_PREFIX);
            gint j = 0;
            while (i >= 0) {
                if (i > 0 && isPredecessor(name.charAt(i - 1))) {
                    name.remove(i, i + UNION_PREFIX.length());
                    j = i + 1;
                    i = name.indexOf(UNION_PREFIX, j);
                } else if (i == 0) {
                    name.remove(0, UNION_PREFIX.length());
                    j = 0;
                    i = name.indexOf(UNION_PREFIX, j);
                } else if (i == j) {
                    i += 1;
                } else {
                    break;
                }
            }
        }
        // removing const specifier
        return name.toString().strip();
#endif
    }


} // core
