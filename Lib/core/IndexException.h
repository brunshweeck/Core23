//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_INDEXEXCEPTION_H
#define CORE23_INDEXEXCEPTION_H

#include <core/ArgumentException.h>

namespace core {

    /**
     * Thrown to indicate that an index of some sort (such as to an array, to a
     * string, or to a list) is out of range.
     * <p>
     * Applications can subclass this class to indicate similar exceptions.
     */
    class IndexException : public ArgumentException {
    public:

        /**
         * Constructs an IndexException with no detail message.
         */
        IndexException() CORE_NOTHROW {}

        /**
         * Constructs an IndexException with the specified detail
         * message.
         *
         * @param message
         *          The detail message
         */
        CORE_EXPLICIT IndexException(String message) CORE_NOTHROW;

        /**
         * Constructs a new IndexException class with an argument indicating the illegal index.
         *
         * <p>
         * The index is included in this exception's detail message.  The
         * exact presentation format of the detail message is unspecified.
         *
         * @param index
         *          The illegal index.
         */
        CORE_EXPLICIT IndexException(gint index) CORE_NOTHROW;

        /**
         * Constructs a new IndexException class with an argument indicating the illegal index.
         *
         * <p>
         * The index is included in this exception's detail message.  The
         * exact presentation format of the detail message is unspecified.
         *
         * @param index
         *          The illegal index.
         */
        CORE_EXPLICIT IndexException(glong index) CORE_NOTHROW;

        /**
         * Return sharable copy of this exception
         */
        Object &clone() const override;

        /**
         * Thrown this exception
         */
        void raise() && override;
    };

} // core

#endif //CORE23_INDEXEXCEPTION_H
