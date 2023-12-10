//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_VOID_H
#define CORE23_VOID_H

#include "Object.h"

namespace core {

    class Void CORE_FINAL : private Object {
    private:
        CORE_FAST Void() = default;

    public:
        virtual ~Void() = default;
    };

} // core

#endif //CORE23_VOID_H
