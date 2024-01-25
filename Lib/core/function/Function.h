//
// Created by T.N.Brunshweeck on 27/11/2023.
//

#ifndef CORE23_FUNCTION_H
#define CORE23_FUNCTION_H

#include "Functional.h"

namespace core {
    namespace function {

        template<class T, class R>
        class Function : public Functional {
        private:
            CORE_ALIAS(Param, Params < T >);
            CORE_ALIAS(Ret, Return < R >);

        public:
            virtual Ret apply(Param /*p*/) const { CORE_UNREACHABLE(); }
        };

    } // core
} // util

#endif //CORE23_FUNCTION_H
