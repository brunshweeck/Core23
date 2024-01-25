//
// Created by T.N.Brunshweeck on 29/11/2023.
//

#ifndef CORE23_LISTITERATOR_H
#define CORE23_LISTITERATOR_H

#include "Iterator.h"

namespace core {
    namespace util {

        /**
         * An iterator for lists that allows the programmer
         * to traverse the list in either direction, modify
         * the list during iteration, and obtain the iterator is
         * current position in the list. A <b>ListIterator</b>
         * has no current element; its <I>cursor position</I> always
         * lies between the element that would be returned by a call
         * to <b>previous()</b> and the element that would be
         * returned by a call to <b>next()</b>.
         * An iterator for a list of length <b>n</b> has <b>n+1</b> possible
         * cursor positions, as illustrated by the carets (<b>^</b>) below:
         * <PRE>
         *                      Element(0)   Element(1)   Element(2)   ... Element(n-1).
         * cursor positions:  ^            ^            ^            ^                  ^
         * </PRE>
         * Note that the <b style="color:orange;">ListIterator.remove</b> and <b style="color:orange;">ListIterator.set(Object)</b> methods are
         * <i>not</i> defined in terms of the cursor position;  they are defined to
         * operate on the last element returned by a call to <b style="color:orange;">ListIterator.next</b> or
         * <b style="color:orange;">ListIterator.previous()</b>.
         *
         * @tparam E the type of elements returned by this list iterator
         *
         * @author  Brunshweeck Tazeussong
         * @see Collection
         * @see List
         * @see Iterator
         */
        template<class E>
        class ListIterator: public Iterator<E> {
        protected:

            CORE_ALIAS(Unsafe, native::Unsafe);
            CORE_ALIAS(ActionConsumer, , function::Consumer<E>);
            CORE_ALIAS(MutableActionConsumer, , function::Consumer<E&>);

        public:

            /**
             * Returns <b>true</b> if this list iterator has more elements when
             * traversing the list in the forward direction. (In other words,
             * returns <b>true</b> if <b style="color:orange;">ListIterator.next</b> would return an element rather
             * than throwing an exception.)
             *
             * @return <b>true</b> if the list iterator has more elements when
             *         traversing the list in the forward direction
             */
            gbool hasNext() const override = 0;

            /**
             * Returns the next element in the list and advances the cursor position.
             * This method may be called repeatedly to iterate through the list,
             * or intermixed with calls to <b style="color:orange;">ListIterator.previous</b> to go back and forth.
             * (Note that alternating calls to <b>next</b> and <b>previous</b>
             * will return the same element repeatedly.)
             *
             * @return the next element in the list
             * @throws NoSuchElementException if the iteration has no next element
             */
            E &next() override = 0;

            /**
             * Returns <b>true</b> if this list iterator has more elements when
             * traversing the list in the reverse direction.  (In other words,
             * returns <b>true</b> if <b style="color:orange;">ListIterator.previous</b> would return an element
             * rather than throwing an exception.)
             *
             * @return <b>true</b> if the list iterator has more elements when
             *         traversing the list in the reverse direction
             */
             virtual gbool hasPrevious() const = 0;

            /**
             * Returns the previous element in the list and moves the cursor
             * position backwards.  This method may be called repeatedly to
             * iterate through the list backwards, or intermixed with calls to
             * <b style="color:orange;">ListIterator.next</b> to go back and forth.  (Note that alternating calls
             * to <b>next</b> and <b>previous</b> will return the same
             * element repeatedly.)
             *
             * @return the previous element in the list
             * @throws NoSuchElementException if the iteration has no previous
             *         element
             */
             virtual E& previous() = 0;

            /**
             * Removes from the list the last element that was returned by <b style="color:orange;">
             * ListIterator.next</b> or <b style="color:orange;">ListIterator.previous</b> (optional operation).  This call can
             * only be made once per call to <b>next</b> or <b>previous</b>.
             * It can be made only if <b style="color:orange;">ListIterator.add</b> has not been
             * called after the last call to <b>next</b> or <b>previous</b>.
             *
             * @throws UnsupportedMethodException if the <b>remove</b>
             *         operation is not supported by this list iterator
             * @throws IllegalStateException if neither <b>next</b> nor
             *         <b>previous</b> have been called, or <b>remove</b> or
             *         <b>add</b> have been called after the last call to
             *         <b>next</b> or <b>previous</b>
             */
            void remove() override = 0;

            /**
             * Replaces the last element returned by <b style="color:orange;">ListIterator.next</b> or
             * <b style="color:orange;">ListIterator.previous</b> with the specified element (optional operation).
             * This call can be made only if neither <b style="color:orange;">ListIterator.remove</b> nor <b style="color:orange;">
             * ListIterator.add</b> have been called after the last call to <b>next</b> or
             * <b>previous</b>.
             *
             * @param e the element with which to replace the last element returned by
             *          <b>next</b> or <b>previous</b>
             * @throws UnsupportedMethodException if the <b>set</b> operation
             *         is not supported by this list iterator
             * @throws ClassCastException if the class of the specified element
             *         prevents it from being added to this list
             * @throws IllegalArgumentException if some aspect of the specified
             *         element prevents it from being added to this list
             * @throws IllegalStateException if neither <b>next</b> nor
             *         <b>previous</b> have been called, or <b>remove</b> or
             *         <b>add</b> have been called after the last call to
             *         <b>next</b> or <b>previous</b>
             */
             virtual void set(const E& e) = 0;

            /**
             * Inserts the specified element into the list (optional operation).
             * The element is inserted immediately before the element that
             * would be returned by <b style="color:orange;">ListIterator.next</b>, if any, and after the element
             * that would be returned by <b style="color:orange;">ListIterator.previous</b>, if any.  (If the
             * list contains no elements, the new element becomes the sole element
             * on the list.)  The new element is inserted before the implicit
             * cursor: a subsequent call to <b>next</b> would be unaffected, and a
             * subsequent call to <b>previous</b> would return the new element.
             * (This call increases by one the value that would be returned by a
             * call to <b>nextIndex</b> or <b>previousIndex</b>.)
             *
             * @param e the element to insert
             * @throws UnsupportedMethodException if the <b>add</b> method is
             *         not supported by this list iterator
             * @throws ClassCastException if the class of the specified element
             *         prevents it from being added to this list
             * @throws IllegalArgumentException if some aspect of this element
             *         prevents it from being added to this list
             */
             virtual void add(const E& e) = 0;

            /**
             * Performs the given action for each remaining element until all elements
             * have been processed or the action throws an exception.  Actions are
             * performed in the order of iteration, if that order is specified.
             * Exceptions thrown by the action are relayed to the caller.
             * <p>
             * The behavior of an iterator is unspecified if the action modifies the
             * collection in any way (even by calling the <b style="color:orange;">Iterator.remove</b> method
             * or other mutator methods of <b>Iterator</b> subtypes),
             * unless an overriding class has specified a concurrent modification policy.
             * <p>
             * Subsequent behavior of an iterator is unspecified if the action throws an
             * exception.
             *
             * @implSpec
             * <p>The default implementation behaves as if:
             * <pre> <b>
             *     while (hasNext())
             *         action.accept(next());
             * </b>
             * </pre>
             *
             * @param action The action to be performed for each element
             */
            void forEach(const ActionConsumer &action) override {
                while (hasNext())
                    action.accept(next());
            }

            /**
             * Performs the given action for each remaining element until all elements
             * have been processed or the action throws an exception.  Actions are
             * performed in the order of iteration, if that order is specified.
             * Exceptions thrown by the action are relayed to the caller.
             * <p>
             * The behavior of an iterator is unspecified if the action modifies the
             * collection in any way (even by calling the <b style="color:orange;">Iterator.remove</b> method
             * or other mutator methods of <b>Iterator</b> subtypes),
             * unless an overriding class has specified a concurrent modification policy.
             * <p>
             * Subsequent behavior of an iterator is unspecified if the action throws an
             * exception.
             *
             * @implSpec
             * <p>The default implementation behaves as if:
             * <pre> <b>
             *     while (hasNext())
             *         action.accept(next());
             * </b>
             * </pre>
             *
             * @param action The action to be performed for each element
             */
            virtual void forEach(const MutableActionConsumer &action) {
                while (hasNext())
                    action.accept(next());
            }
        };

    } // core
} // util

#endif //CORE23_LISTITERATOR_H
