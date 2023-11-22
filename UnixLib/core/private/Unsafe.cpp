//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#include <core/private/Unsafe.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace core {
    namespace native {

        glong Unsafe::allocateMemoryImpl(glong sizeInBytes) {
            //
        }

        glong Unsafe::reallocateMemoryImpl(glong address, glong sizeInBytes) {}

        void Unsafe::freeMemoryImpl(glong address) {}

        void Unsafe::setMemoryImpl(glong address, glong sizeInBytes, gbyte value) {
            memset((void *) address, sizeInBytes, value);
        }

        void Unsafe::copyMemoryImpl(glong srcAddress, Object &dest, glong destAddress, glong sizeInBytes) {
            memcpy((void *) srcAddress, (void *) destAddress, sizeInBytes);
        }

        void Unsafe::copySwapMemoryImpl(glong srcAddress, glong destAddress, glong sizeInBytes, glong elemSize) {
            memmove((void *) srcAddress, (void *) destAddress, sizeInBytes);
        }

    }
}