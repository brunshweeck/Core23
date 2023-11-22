//
// Created by Brunshweeck on 12/09/2023.
//

#include "Null.h"
#include <core/String.h>

namespace core {

    gbool Null::equals(const Object &o) const {
        return &o == &null && this == &null;
    }

    Object &Null::clone() const {
        return null;
    }

    String Null::toString() const {
        if(this != &null)
            return Object::toString();
        return "null"_S;
    }

    Null null = {};

} // core