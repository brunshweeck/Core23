//
// Created by T.N.Brunshweeck on 30/11/2023.
//

#ifndef CORE23_CONCURRENTEXCEPTION_H
#define CORE23_CONCURRENTEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {
    namespace util {

        /**
         * This exception may be thrown by methods that have detected concurrent
         * modification of an object when such modification is not permissible.
         * <p>
         * For example, it is not generally permissible for one thread to modify a Collection
         * while another thread is iterating over it.  In general, the results of the
         * iteration are undefined under these circumstances.  Some Iterator
         * implementations (including those of all the general purpose collection implementations
         * provided by the JRE) may choose to throw this exception if this behavior is
         * detected.  Iterators that do this are known as <i>fail-fast</i> iterators,
         * as they fail quickly and cleanly, rather that risking arbitrary,
         * non-deterministic behavior at an undetermined time in the future.
         * <p>
         * Note that this exception does not always indicate that an object has
         * been concurrently modified by a <i>different</i> thread.  If a single
         * thread issues a sequence of method invocations that violates the
         * contract of an object, the object may throw this exception.  For
         * example, if a thread modifies a collection directly while it is
         * iterating over the collection with a fail-fast iterator, the iterator
         * will throw this exception.
         *
         * <p>Note that fail-fast behavior cannot be guaranteed as it is, generally
         * speaking, impossible to make any hard guarantees in the presence of
         * unsynchronized concurrent modification.  Fail-fast operations
         * throw <b> ConcurrentModificationException</b> on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness: <i><b> ConcurrentModificationException</b>
         * should be used only to detect bugs.</i>
         *
         * @author  Brunshweeck Tazeussong
         * @see     Collection
         * @see     Iterator
         * @see     ListIterator
         * @see     Vector
         * @see     LinkedList
         * @see     HashSet
         * @see     Hashtable
         * @see     TreeMap
         */


        class ConcurrentException: public RuntimeException {
        public:

            /**
             * Construct new ConcurrentException without detail message
             */
            ConcurrentException() CORE_NOTHROW = default;

            /**
             * Construct new ConcurrentException with detail message
             */
            CORE_EXPLICIT ConcurrentException(String message) CORE_NOTHROW;

            /**
             * Construct new ConcurrentException without detail message
             * and cause
             */
            CORE_EXPLICIT ConcurrentException(String message, const Throwable &cause) CORE_NOTHROW;

            CORE_EXPLICIT ConcurrentException(const Throwable &cause);

            Object &clone() const override;

        private:
            CORE_NORETURN void raise() && override;
        };

    } // core
} // util

#endif //CORE23_CONCURRENTEXCEPTION_H
