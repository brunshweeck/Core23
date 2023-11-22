//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_BICONSUMER_H
#define CORE23_BICONSUMER_H

#include "Functional.h"

namespace core {
    namespace Function {

        template<class T, class U>
        class BiConsumer: public BinaryFunction<T, U, Void> {

        };

    } // core
} // Functor

#endif //CORE23_BICONSUMER_H
