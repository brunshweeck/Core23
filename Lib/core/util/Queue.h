//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_QUEUE_H
#define CORE23_QUEUE_H

#include "Collection.h"
#include <core/NoSuchElementException.h>

namespace core {
    namespace util {

        /**
         * A collection designed for holding elements prior to processing.
         * Besides basic <b style="color:green;">Collection</b> operations, queues provide
         * additional insertion, extraction, and inspection operations.  The
         * latter form of the insert operation is designed specifically for
         * use with capacity-restricted <b>Queue</b> implementations; in most
         * implementations, insert operations cannot fail.
         *
         * <p>Queues typically, but do not necessarily, order elements in a
         * FIFO (first-in-first-out) manner.  Among the exceptions are
         * priority queues, which order elements according to a supplied
         * comparator, or the elements' natural ordering, and LIFO queues (or
         * stacks) which order the elements LIFO (last-in-first-out).
         * Whatever the ordering used, the <em>head</em> of the queue is that
         * element which would be removed by a call to <b style="color:green;">remove()</b> or
         * <b style="color:green;">pop()</b>.  In a FIFO queue, all new elements are inserted at
         * the <em>tail</em> of the queue. Other kinds of queues may use
         * different placement rules.  Every <b>Queue</b> implementation
         * must specify its ordering properties.
         *
         * <p>The <b style="color:green;">push</b> method inserts an element if possible,
         * otherwise returning <b>false</b>.  This differs from the <b style="color:green;">
         * Collection.add</b> method, which can fail to
         * add an element only by throwing an unchecked exception.  The
         * <b>push</b> method is designed for use when failure is a normal,
         * rather than exceptional occurrence, for example, in fixed-capacity
         * (or &quot;bounded&quot;) queues.
         *
         * <p>The <b style="color:green;">remove()</b> and <b style="color:green;">pop()</b> methods remove and
         * return the head of the queue.
         * Exactly which element is removed from the queue is a
         * function of the queue's ordering policy, which differs from
         * implementation to implementation. The <b>remove()</b> and
         * <b>pop()</b> methods differ only in their behavior when the
         * queue is empty: the <b>remove()</b> method throws an exception,
         * while the <b>pop()</b> method returns <b>null</b>.
         *
         * <p>The <b style="color:green;">get()</b> method return, but do
         * not remove, the head of the queue.
         *
         * <p><b>Queue</b> implementations generally do not define
         * element-based versions of methods <b>equals</b> and
         * but instead inherit the identity based versions
         * from class <b>Object</b>, because element-based equality is not
         * always well-defined for queues with the same elements but different
         * ordering properties.
         *
         * @author Brunshweeck Tazeussong
         * @param E the type of elements held in this queue
         */
        template<class E>
        class Queue : public Collection<E> {
        protected:

            CORE_ALIAS(Unsafe, native::Unsafe);
            CORE_ALIAS(ActionConsumer, , function::Consumer<E>);
            CORE_ALIAS(ElementFilter, , function::Predicate<E>);
            CORE_ALIAS(UnaryFunction, , function::Function<E, E>);

        public:

            /**
             * Inserts the specified element into this queue if it is possible to do so
             * immediately without violating capacity restrictions, returning
             * <b>true</b> upon success and throwing an <b>IllegalStateException</b>
             * if no diskSpace is currently available.
             *
             * @param e the element to add
             * @return <b>true</b> (as specified by <b style="color:green;">Collection.add</b>)
             * @throws IllegalStateException if the element cannot be added at this
             *         time due to capacity restrictions
             * @throws ClassCastException if the class of the specified element
             *         prevents it from being added to this queue
             * @throws IllegalArgumentException if some property of this element
             *         prevents it from being added to this queue
             */
            gbool add(const E &e) override {
                if (push(e))
                    return true;
                IllegalStateException("Queue is full").throws(__trace("core.util.Queue"));
            }

            /**
             * Inserts the specified element into this queue if it is possible to do
             * so immediately without violating capacity restrictions.
             * When using a capacity-restricted queue, this method is generally
             * preferable to <b style="color:green;">add</b>, which can fail to insert an element only
             * by throwing an exception.
             *
             * @param e the element to add
             * @return <b>true</b> if the element was added to this queue, else
             *         <b>false</b>
             * @throws ClassCastException if the class of the specified element
             *         prevents it from being added to this queue
             * @throws IllegalArgumentException if some property of this element
             *         prevents it from being added to this queue
             */
            virtual gbool push(const E &e) = 0;

            /**
             * Retrieves and removes the head of this queue.
             *
             * @return the head of this queue
             * @throws NoSuchElementException if this queue is empty
             */
            virtual const E &remove() {
                if (this->size() > 0)
                    return pop();
                NoSuchElementException().throws(__trace("core.util.Queue"));
            }

            /**
             * Retrieves and removes the head of this queue.
             *
             * @return the head of this queue
             * @throws NoSuchElementException if this queue is empty
             */
            virtual const E &pop() = 0;

            /**
             * Retrieves, but does not remove, the head of this queue.
             *
             * @return the head of this queue
             * @throws NoSuchElementException if this queue is empty
             */
            virtual E &get() = 0;

            /**
             * Retrieves, but does not remove, the head of this queue.
             *
             * @return the head of this queue
             * @throws NoSuchElementException if this queue is empty
             */
            virtual const E &get() const = 0;

            /**
             * Removes all of the elements from this queue.
             * The queue will be empty after this call returns.
             *
             * <p>This implementation repeatedly invokes <b style="color:orange;">pop</b> until it
             * returns <b>null</b>.
             */
            void clear() override {
                while (this->size() > 0)
                    pop();
            }

            /**
             * Adds all of the elements in the specified collection to this
             * queue.  Attempts to addAll of a queue to itself result in
             * <b>IllegalArgumentException</b>. Further, the behavior of
             * this operation is undefined if the specified collection is
             * modified while the operation is in progress.
             *
             * <p>This implementation iterates over the specified collection,
             * and adds each element returned by the iterator to this
             * queue, in turn.  A runtime exception encountered while
             * trying to add an element (including, in particular, a
             * <b>null</b> element) may result in only some of the elements
             * having been successfully added when the associated exception is
             * thrown.
             *
             * @param c collection containing elements to be added to this queue
             * @return <b>true</b> if this queue changed as a result of the call
             * @throws ClassCastException if the class of an element of the specified
             *         collection prevents it from being added to this queue
             * @throws IllegalArgumentException if some property of an element of the
             *         specified collection prevents it from being added to this
             *         queue, or if the specified collection is this queue
             * @throws IllegalStateException if not all the elements can be added at
             *         this time due to insertion restrictions
             * @see add(Object)
             */
            gbool addAll(const Collection<E> &c) override {
                if (this == &c)
                    IllegalArgumentException().throws(__trace("core.util.Queue"));
                gbool modified = {};
                for (const E &e: c)
                    if (add(e))
                        modified = true;
                return true;
            }
        };

    }
} // core

#endif //CORE23_QUEUE_H
