//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_PRIORITYQUEUE_H
#define CORE23_PRIORITYQUEUE_H

#include "Queue.h"

namespace core {
    namespace util {

        /**
         * An unbounded priority <b style="color:green;">queue</b> based on a priority heap.
         * The elements of the priority queue are ordered according to their
         * <b style="color:green;">natural ordering</b>, or by a <b style="color:orange;">Comparator</b>
         * provided at queue construction time, depending on which constructor is
         * used.  A priority queue does not permit <b>null</b> elements.
         * A priority queue relying on natural ordering also does not permit
         * insertion of non-comparable objects (doing so may result in
         * <b>CastException</b>).
         *
         * <p>The <em>head</em> of this queue is the <em>least</em> element
         * with respect to the specified ordering.  If multiple elements are
         * tied for least value, the head is one of those elements -- ties are
         * broken arbitrarily.  The queue retrieval operations <b>poll</b>,
         * <b>remove</b>, <b>peek</b>, and <b>element</b> access the
         * element at the head of the queue.
         *
         * <p>A priority queue is unbounded, but has an internal
         * <i>capacity</i> governing the size of an array used to store the
         * elements on the queue.  It is always at least as large as the queue
         * size.  As elements are added to a priority queue, its capacity
         * grows automatically.  The details of the growth policy are not
         * specified.
         *
         * <p>This class and its iterator implement all of the
         * <em>optional</em> methods of the <b style="color:orange;">Collection</b> and <b style="color:orange;">
         * Iterator</b> interfaces.  The Iterator provided in method <b style="color:orange;">
         * PriorityQueue.iterator()</b> is <em>not</em> guaranteed to traverse the elements of
         * the priority queue in any particular order.
         *
         * <p><strong>Note that this implementation is not synchronized.</strong>
         * Multiple threads should not access a <b>PriorityQueue</b>
         * instance concurrently if any of the threads modifies the queue.
         * Instead, use the thread-safe <b style="color:orange;">
         * core.util.PrioritySynchronizedQueue</b> class.
         *
         * <p>Implementation note: this implementation provides
         * O(log(n)) time for the enqueuing and dequeuing methods
         * (<b>push</b>, <b>pop</b>, <b>remove()</b> and <b>add</b>);
         * linear time for the <b>remove(Object)</b> and <b>contains(Object)</b>
         * methods; and constant time for the retrieval methods
         * (<b>get</b>, and <b>size</b>).
         *
         * @author Brunshweeck Tazeussong
         * @param E the type of elements held in this queue
         */
        template<class E>
        class PriorityQueue : public Queue<E> {

        };

    }
} // core

#endif //CORE23_PRIORITYQUEUE_H
