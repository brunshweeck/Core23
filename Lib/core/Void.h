//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_VOID_H
#define CORE23_VOID_H

#include "Object.h"

namespace core {

    /**
     * The <b> Void</b> class is an uninstantiable placeholder class to hold a
     * reference to the <b> Class</b> object representing the keyword
     * void.
     *
     */
    class Void CORE_FINAL : public Object {
    private:
        /*
         * The Void class cannot be instantiated.
         */
        CORE_FAST Void() = default;
    };

} // core

#endif //CORE23_VOID_H
