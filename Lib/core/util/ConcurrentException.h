//
// Created by T.N.Brunshweeck on 30/11/2023.
//

#ifndef CORE23_CONCURRENTEXCEPTION_H
#define CORE23_CONCURRENTEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {
    namespace util {

        class ConcurrentException: public RuntimeException {
        public:
            ConcurrentException() CORE_NOTHROW = default;

            Object &clone() const override;

            CORE_NORETURN void raise() && override;
        };

    } // core
} // util

#endif //CORE23_CONCURRENTEXCEPTION_H
