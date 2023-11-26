//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_VOID_H
#define CORE23_VOID_H

#include "Object.h"

namespace core {

    class Void: private Object {
    private:
        CORE_FAST Void() = default;

        CORE_DISABLE_COPY_MOVE(Void);
    };

} // core

#endif //CORE23_VOID_H
