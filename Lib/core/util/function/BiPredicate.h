//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_BIPREDICATE_H
#define CORE23_BIPREDICATE_H

#include "Functional.h"

namespace core {
    namespace Function {

        template<class T, class U>
        class BiPredicate: public BinaryFunction<T, U, Boolean> {

        };

    } // core
} // function

#endif //CORE23_BIPREDICATE_H
