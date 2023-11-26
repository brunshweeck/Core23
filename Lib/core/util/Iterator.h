//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_ITERATOR_H
#define CORE23_ITERATOR_H

#include <core/Object.h>
#include <core/UnsupportedMethodException.h>
#include <core/util/function/Consumer.h>

namespace core {

    /**
     * An iterator over a collection.  <b>Iterator</b> takes the place of
     * <b style="color: orange;">Enumeration</b> in the core library Collections Framework.  Iterators
     * differ from enumerations in two ways:
     *
     * <ul>
     *      <li> Iterators allow the caller to remove elements from the
     *           underlying collection during the iteration with well-defined
     *           semantics.
     *      <li> Method names have been improved.
     * </ul>
     *
     * @param E the type of elements returned by this iterator
     *
     * @author  Brunshweeck Tazeussong
     * @see Collection
     */
    template<class E>
    class Iterator: public Object {

        /**
         * Returns <b>true</b> if the iteration has more elements.
         * (In other words, returns <b>true</b> if <b style="color: orange;">next</b> would
         * return an element rather than throwing an exception.)
         */
        virtual gbool hasNext() const = 0;

        /**
         * Returns the next element in the iteration.
         *
         * @throws NoSuchElementException if the iteration has no more elements
         */
        virtual E &next() = 0;

        /**
         * Performs the given action for each remaining element until all elements
         * have been processed or the action throws an exception.  Actions are
         * performed in the order of iteration, if that order is specified.
         * Exceptions thrown by the action are relayed to the caller.
         * <p>
         * The behavior of an iterator is unspecified if the action modifies the
         * collection in any way (even by calling the <b style="color: orange;">remove</b> method
         * or other mutator methods of <b>Iterator</b> subtypes),
         * unless an overriding class has specified a concurrent modification policy.
         * <p>
         * Subsequent behavior of an iterator is unspecified if the action throws an
         * exception.
         *
         * <p>
         * Note:
         * <p>The default implementation behaves as if:
         * <pre> <b> while (hasNext()) action.accept(next()); </b></pre>
         *
         * @param action The action to be performed for each element
         */
        virtual void forEach(const Consumer<E> &action) { while (hasNext()) action.accept(next()); }
    };

} // core

#endif //CORE23_ITERATOR_H
