//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_VECTOR_H
#define CORE23_VECTOR_H

#include "List.h"

namespace core {
    namespace util {
        
        /**
         * The <b>Vector</b> class implements a growable array of
         * objects. Like an array, it contains components that can be
         * accessed using an integer index. However, the size of a
         * <b>Vector</b> can grow or shrink as needed to accommodate
         * adding and removing items after the <b>Vector</b> has been created.
         *
         * <p>Each vector tries to optimize storage management by maintaining a
         * <b>capacity</b> and a <b>capacityIncrement</b>. The
         * <b>capacity</b> is always at least as large as the vector
         * size; it is usually larger because as components are added to the
         * vector, the vector's storage increases in chunks the size of
         * <b>capacityIncrement</b>. An application can increase the
         * capacity of a vector before inserting a large number of
         * components; this reduces the amount of incremental reallocation.
         *
         * <p id="fail-fast">
         * The iterators returned by this class's <b style="color:orange;">iterator</b>
         * methods are <em>fail-fast</em>:
         * if the vector is structurally modified at any time after the iterator is
         * created, in any way except through the iterator is own
         * <b style="color:orange;">remove</b> or
         * <b style="color:orange;">add</b> methods, the iterator will throw a
         * <b style="color:orange;">ConcurrentException</b>.  Thus, in the face of
         * concurrent modification, the iterator fails quickly and cleanly, rather
         * than risking arbitrary, non-deterministic behavior at an undetermined
         * time in the future.  The <b style="color:orange;">Enumerations</b> returned by
         * the <b style="color:orange;">elements</b> method are <em>not</em> fail-fast; if the
         * Vector is structurally modified at any time after the enumeration is
         * created then the results of enumerating are undefined.
         *
         * <p>Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw <b>ConcurrentException</b> on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness:  <i>the fail-fast behavior of iterators
         * should be used only to detect bugs.</i>
         *
         * @param E Type of component elements
         *
         * @author  Brunshweeck Tazeussong
         *
         * @see Collection
         * @see LinkedList
         */
        template<class E>
        class Vector : public List<E> {

        };

    }
} // core

#endif //CORE23_VECTOR_H
