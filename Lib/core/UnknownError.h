//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_UNKNOWNERROR_H
#define CORE23_UNKNOWNERROR_H

#include "Error.h"

namespace core {

    class UnknownError: public Error {
    public:
        UnknownError() CORE_NOTHROW = default;

        explicit UnknownError(String message) CORE_NOTHROW;

        explicit UnknownError(String message, const Throwable &cause) CORE_NOTHROW;

        Object &clone() const override;

    private:
        void raise() && override;
    };

} // core

#endif //CORE23_UNKNOWNERROR_H
