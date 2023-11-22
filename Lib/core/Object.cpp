//
// Created by Brunshweeck on 12/09/2023.
//

#include "Object.h"
#include <core/String.h>
#include <core/Integer.h>

namespace core {
    gbool Object::equals(const Object &o) const {
        if(this == &o)
            return true;
        gbyte *bytes1 = (gbyte *) this;
        gbyte *bytes2 = (gbyte *) &o;
        for (int i = 0; i < sizeof(Object); ++i) {
            if(bytes1[i] != bytes2[i])
                return false;
        }
        return false;
    }

    Object &Object::clone() const {
        // try {
        //      Object& o Unsafe::allocateInstance<Object>();
        //      gbyte *bytes1 = (gbyte *) this;
        //      gbyte *bytes2 = (gbyte *) &o;
        //      for (int i = 0; i < sizeof(Object); ++i) {
        //          bytes2[i] = bytes1[i]
        //      }
        //      return o;
        // } catch(const Throwable& thr) {
        //     thr.throws(__trace("core::Object"));
        // }
    }

    String Object::toString() const {
        return classname() + Integer::toUnsignedString((gint)(((glong) this) & Integer::MAX_VALUE), 16);
    }


    gint Object::hash() const {
        return 0;
    }

    String Object::classname() const {
        return "core.Object"_S;
    }

} // core