//
// Created by Brunshweeck on 12/09/2023.
//

#include "Throwable.h"

namespace core {
    Throwable::Throwable() CORE_NOTHROW:
            cse(null), stack(null), stackSize(0), temporal(false) {}
} // core