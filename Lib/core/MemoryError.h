//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_MEMORYERROR_H
#define CORE23_MEMORYERROR_H

#include <core/SystemError.h>

namespace core {

    /**
     * Thrown when the Java Virtual Machine cannot allocate an object
     * because it is out of memory, and no more memory could be made
     * available by the garbage collector.
     */
    class MemoryError: public SystemError {
    public:

        /**
         * Constructs an MemoryError with no detail message.
         */
        MemoryError() CORE_NOTHROW = default;

        /**
         * Constructs an MemoryError with the specified
         * detail message.
         *
         * @param message
         *          The detail message.
         */
        CORE_EXPLICIT MemoryError(String message) CORE_NOTHROW;

        /**
         * Return sharable copy of this error
         */
        Object &clone() const override;

        /**
         * Thrown this error.
         */
        void raise() && override;
    };

} // core

#endif //CORE23_MEMORYERROR_H
