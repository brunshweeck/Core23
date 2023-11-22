//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_PREDICATE_H
#define CORE23_PREDICATE_H

#include "Functional.h"

namespace core {
    namespace Function {
        template<class T>
        class Predicate : public UnaryFunction<T, Boolean> {

        };
    }

} // core

#endif //CORE23_PREDICATE_H
