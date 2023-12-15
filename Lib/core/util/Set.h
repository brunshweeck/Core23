//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_SET_H
#define CORE23_SET_H

#include <core/util/Collection.h>

namespace core {
    namespace util {

        /**
         * A collection that contains no duplicate elements.  More formally, sets
         * contain no pair of elements <b> e1</b> and <b> e2</b> such that
         * <b> e1.equals(e2)</b>, and at most one null element.  As implied by
         * its name, this interface models the mathematical <i>set</i> abstraction.
         *
         * <p>The <b> Set</b> interface places additional stipulations, beyond those
         * inherited from the <b> Collection</b> interface, on the contracts of all
         * constructors and on the contracts of the <b> add</b>, <b> equals</b> and
         * <b> hashCode</b> methods.  Declarations for other inherited methods are
         * also included here for convenience.  (The specifications accompanying these
         * declarations have been tailored to the <b> Set</b> interface, but they do
         * not contain any additional stipulations.)
         *
         * <p>The additional stipulation on constructors is, not surprisingly,
         * that all constructors must create a set that contains no duplicate elements
         * (as defined above).
         *
         * <p>Note: Great care must be exercised if mutable objects are used as set
         * elements.  The behavior of a set is not specified if the value of an object
         * is changed in a manner that affects <b> equals</b> comparisons while the
         * object is an element in the set.  A special case of this prohibition is
         * that it is not permissible for a set to contain itself as an element.
         *
         * <p>Some set implementations have restrictions on the elements that
         * they may contain.  For example, some implementations prohibit null elements,
         * and some have restrictions on the types of their elements.  Attempting to
         * add an ineligible element throws an unchecked exception, typically
         * <b> CastException</b>.  Attempting
         * to query the presence of an ineligible element may throw an exception,
         * or it may simply return false; some implementations will exhibit the former
         * behavior and some will exhibit the latter.  More generally, attempting an
         * operation on an ineligible element whose completion would not result in
         * the insertion of an ineligible element into the set may throw an
         * exception or it may succeed, at the option of the implementation.
         * Such exceptions are marked as "optional" in the specification for this
         * interface.
         *
         * <h2><a id="unmodifiable">Unmodifiable Sets</a></h2>
         * <p>The <b style="color: orange;"> Set.of</b> and
         * <b style="color: orange;"> Set.copyOf</b> static factory methods
         * provide a convenient way to create unmodifiable sets. The <b> Set</b>
         * instances created by these methods have the following characteristics:
         *
         * <ul>
         * <li>They are <a href=""><i>unmodifiable</i></a>. Elements cannot
         * be added or removed. Calling any mutator method on the Set
         * will always cause <b> UnsupportedMethodException</b> to be thrown.
         * However, if the contained elements are themselves mutable, this may cause the
         * Set to behave inconsistently or its contents to appear to change.
         * <li>They disallow <b> null</b> elements.
         * <li>They are serializable if all elements are serializable.
         * <li>They reject duplicate elements at creation time. Duplicate elements
         * passed to a static factory method result in <b> ArgumentException</b>.
         * <li>The iteration order of set elements is unspecified and is subject to change.
         * <li>They are <a href="">value-based</a>.
         * Programmers should treat instances that are <b style="color: green;"> equal</b>
         * as interchangeable and should not use them for synchronization, or
         * unpredictable behavior may occur. For example, in a future release,
         * synchronization may fail. Callers should make no assumptions
         * about the identity of the returned instances. Factories are free to
         * create new instances or reuse existing ones.
         * <li>They are serialized as specified on the <a href="">Serialized Form</a> page.
         * </ul>
         *
         * @param E the type of elements maintained by this set
         *
         * @author  Brunshweeck Tazeussong
         * @see Collection
         * @see List
         * @see HashSet
         * @see TreeSet
         */
        template<class E>
        class Set : public Collection<E> {
        private:
            CORE_ALIAS(U, native::Unsafe);

            /**
             * Capture<T> represent all type T that extends this value type E.
             * in other word E is base of T (Class<E>::isSuper<T>() is true).
             */
            template<class T>
            CORE_ALIAS(Capture, typename Class<T>::template Iff<Class<E>::template isSuper<T>()>);

        public:

            /**
             * Returns the number of elements in this set (its cardinality).  If this
             * set contains more than <b> Integer.MAX_VALUE</b> elements, returns
             * <b> Integer.MAX_VALUE</b>.
             *
             * @return the number of elements in this set (its cardinality)
             */
            gint size() const override = 0;

            /**
             * Returns <b> true</b> if this set contains the specified element.
             * More formally, returns <b> true</b> if and only if this set
             * contains an element <b> e</b> such that
             * <b> Object.equals(o, e)</b>.
             *
             * @param o element whose presence in this set is to be tested
             * @return <b> true</b> if this set contains the specified element
             * @throws CastException if the type of the specified element
             *         is incompatible with this set (<a href="">optional</a>)
             */
            gbool contains(const E &o) const override { return Collection<E>::contains(o); }

            /**
             * Returns an iterator over the elements in this set.  The elements are
             * returned in no particular order (unless this set is an instance of some
             * class that provides a guarantee).
             *
             * @return an iterator over the elements in this set
             */
            Iterator<const E> &iterator() const override = 0;

            /**
             * Adds the specified element to this set if it is not already present
             * (optional operation).  More formally, adds the specified element
             * <b> e</b> to this set if the set contains no element <b> e2</b>
             * such that
             * <b> Object.equals(e, e2)</b>.
             * If this set already contains the element, the call leaves the set
             * unchanged and returns <b> false</b>.  In combination with the
             * restriction on constructors, this ensures that sets never contain
             * duplicate elements.
             *
             * <p>The stipulation above does not imply that sets must accept all
             * elements; sets may refuse to add any particular element, including
             * <b> null</b>, and throw an exception, as described in the
             * specification for <b style="color: orange;"> Collection.add </b>.
             * Individual set implementations should clearly document any
             * restrictions on the elements that they may contain.
             *
             * @param e element to be added to this set
             * @return <b> true</b> if this set did not already contain the specified
             *         element
             * @throws UnsupportedMethodException if the <b> add</b> operation
             *         is not supported by this set
             * @throws CastException if the class of the specified element
             *         prevents it from being added to this set
             * @throws ArgumentException if some property of the specified element
             *         prevents it from being added to this set
             */
            gbool add(const E &e) override { UnsupportedMethodException().throws(__trace("core.util.Set")); }

            /**
             * Removes the specified element from this set if it is present
             * (optional operation).  More formally, removes an element <b> e</b>
             * such that
             * <b> Object.equals(o, e)</b>, if
             * this set contains such an element.  Returns <b> true</b> if this set
             * contained the element (or equivalently, if this set changed as a
             * result of the call).  (This set will not contain the element once the
             * call returns.)
             *
             * @param o object to be removed from this set, if present
             * @return <b> true</b> if this set contained the specified element
             * @throws CastException if the type of the specified element
             *         is incompatible with this set (<a href="">optional</a>)
             * @throws UnsupportedMethodException if the <b> remove</b> operation
             *         is not supported by this set
             */
            gbool remove(const E &o) override { return Collection<E>::remove(o); }

            /**
             * Returns <b> true</b> if this set contains all of the elements of the
             * specified collection.  If the specified collection is also a set, this
             * method returns <b> true</b> if it is a <i>subset</i> of this set.
             *
             * @param  c collection to be checked for containment in this set
             *
             * @throws CastException if the types of one or more elements
             *         in the specified collection are incompatible with this
             *         set (<a href="">optional</a>)
             * @see    contains(Object)
             */
            gbool containsAll(const Collection<E> &c) const override { return Collection<E>::containsAll(c); }

            /**
             * Adds all of the elements in the specified collection to this set if
             * they're not already present (optional operation).  If the specified
             * collection is also a set, the <b> addAll</b> operation effectively
             * modifies this set so that its value is the <i>union</i> of the two
             * sets.  The behavior of this operation is undefined if the specified
             * collection is modified while the operation is in progress.
             *
             * @param  c collection containing elements to be added to this set
             * @return <b> true</b> if this set changed as a result of the call
             *
             * @throws UnsupportedMethodException if the <b> addAll</b> operation
             *         is not supported by this set
             * @throws CastException if the class of an element of the
             *         specified collection prevents it from being added to this set
             * @throws ArgumentException if some property of an element of the
             *         specified collection prevents it from being added to this set
             * @see add(Object)
             */
            gbool addAll(const Collection<E> &c) override { return Collection<E>::addAll(c); }

            /**
             * Adds all of the elements in the specified collection to this set if
             * they're not already present (optional operation).  If the specified
             * collection is also a set, the <b> addAll</b> operation effectively
             * modifies this set so that its value is the <i>union</i> of the two
             * sets.  The behavior of this operation is undefined if the specified
             * collection is modified while the operation is in progress.
             *
             * @param  c collection containing elements to be added to this set
             * @return <b> true</b> if this set changed as a result of the call
             *
             * @throws UnsupportedMethodException if the <b> addAll</b> operation
             *         is not supported by this set
             * @throws CastException if the class of an element of the
             *         specified collection prevents it from being added to this set
             * @throws ArgumentException if some property of an element of the
             *         specified collection prevents it from being added to this set
             * @see add(Object)-
             */
            template<class T = E>
            gbool addAll(const Collection<Capture<T>> &c) { return Collection<E>::template addAll<T>(c); }

            /**
             * Removes from this set all of its elements that are contained in the
             * specified collection (optional operation).  If the specified
             * collection is also a set, this operation effectively modifies this
             * set so that its value is the <i>asymmetric set difference</i> of
             * the two sets.
             *
             * @param  c collection containing elements to be removed from this set
             * @return <b> true</b> if this set changed as a result of the call
             * @throws UnsupportedMethodException if the <b> removeAll</b> operation
             *         is not supported by this set
             * @throws CastException if the class of an element of this set
             *         is incompatible with the specified collection (<a href="">optional</a>)
             * @see remove(Object)
             * @see contains(Object)
             */
            gbool removeAll(const Collection<E> &c) override {
                gbool modified = false;
                if (size() > c.size()) {
                    for (const E &e: c)
                        modified |= remove(e);
                } else {
                    for (Iterator<const E> &i = iterator(); i.hasNext();) {
                        if (c.contains((const E &) i.next())) {
                            i.remove();
                            modified = true;
                        }
                    }
                }
                return modified;
            }

            /**
             * Retains only the elements in this set that are contained in the
             * specified collection (optional operation).  In other words, removes
             * from this set all of its elements that are not contained in the
             * specified collection.  If the specified collection is also a set, this
             * operation effectively modifies this set so that its value is the
             * <i>intersection</i> of the two sets.
             *
             * @param  c collection containing elements to be retained in this set
             * @return <b> true</b> if this set changed as a result of the call
             * @throws UnsupportedMethodException if the <b> retainAll</b> operation
             *         is not supported by this set
             * @throws CastException if the class of an element of this set
             *         is incompatible with the specified collection (<a href="">optional</a>)
             * @see remove(Object)
             */
            gbool retainAll(const Collection<E> &c) override { return Collection<E>::retainAll(c); }

            /**
             * Removes all of the elements from this set (optional operation).
             * The set will be empty after this call returns.
             *
             * @throws UnsupportedMethodException if the <b> clear</b> method
             *         is not supported by this set
             */
            void clear() override = 0;

            /**
             * Compares the specified object with this set for equality.  Returns
             * <b> true</b> if the specified object is also a set, the two sets
             * have the same size, and every member of the specified set is
             * contained in this set (or equivalently, every member of this set is
             * contained in the specified set).  This definition ensures that the
             * equals method works properly across different implementations of the
             * set interface.
             *
             * @param o object to be compared for equality with this set
             * @return <b> true</b> if the specified object is equal to this set
             */
            gbool equals(const Object &o) const override {
                if (this == &o)
                    return true;
                if (!Class<Set<E>>::hasInstance(o))
                    return false;
                const Collection<E> &c = (Collection<E> &) o;
                if (size() != c.size())
                    return false;
                return containsAll(c);
            }
        };

    }
} // core

#endif //CORE23_SET_H
