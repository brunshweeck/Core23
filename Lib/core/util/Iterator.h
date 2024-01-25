//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_ITERATOR_H
#define CORE23_ITERATOR_H

#include <core/Object.h>
#include <core/UnsupportedOperationException.h>
#include <core/function/Consumer.h>

namespace core {
    namespace util {

        /**
         * An iterator over a collection.  <b>Iterator</b> takes the place of
         * <b style="color: orange;">Enumeration</b> in the library Collections Framework.  Iterators
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
        class Iterator : public Object {
        protected:

            CORE_ALIAS(Unsafe, native::Unsafe);
            CORE_ALIAS(ActionConsumer, , function::Consumer<E>);
            CORE_ALIAS(MutableActionConsumer, , function::Consumer<E&>);

        public:

            /**
             * Returns <b>true</b> if the iteration has more elements.
             * (In other words, returns <b>true</b> if <b style="color: orange;">Iterator.next</b> would
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
             * Removes from the underlying collection the last element returned
             * by this iterator (optional operation).  This method can be called
             * only once per call to <b style="color:orange;">Iterator.next</b>.
             * <p>
             * The behavior of an iterator is unspecified if the underlying collection
             * is modified while the iteration is in progress in any way other than by
             * calling this method, unless an overriding class has specified a
             * concurrent modification policy.
             * <p>
             * The behavior of an iterator is unspecified if this method is called
             * after a call to the <b style="color:orange;">Iterator.forEach</b> method.
             *
             * @implSpec
             * The default implementation throws an INSTANCE of
             * <b style="color:orange;">UnsupportedOperationException</b> and performs no other action.
             *
             * @throws UnsupportedMethodException if the <b>remove</b>
             *         operation is not supported by this iterator
             *
             * @throws IllegalStateException if the <b>next</b> method has not
             *         yet been called, or the <b>remove</b> method has already
             *         been called after the last call to the <b>next</b>
             *         method
             */
            virtual void remove() { UnsupportedOperationException().throws(__trace("core.util.Iterator")); }

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
            virtual void forEach(const ActionConsumer &action) {
                while (hasNext())
                    action.accept(next());
            }
        };

    }
} // core

#endif //CORE23_ITERATOR_H
