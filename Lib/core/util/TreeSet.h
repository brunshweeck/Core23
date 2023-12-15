//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_TREESET_H
#define CORE23_TREESET_H

#include <core/util/Set.h>
#include <core/util/TreeMap.h>

namespace core {
    namespace util {

        /**
         * A Red-Black tree based <b style="color:orange;">Set</b> implementation.
         * The set is sorted according to the <b style="color:green;">natural
         * ordering</b> of its keys, or by a <b style="color:orange;">Comparator</b> provided at set
         * creation time, depending on which constructor is used.
         *
         * <p>This implementation provides guaranteed log(n) time cost for the basic
         * operations (<b>add</b>, <b>remove</b> and <b>contains</b>).
         *
         * <p>Note that the ordering maintained by a set (whether or not an explicit
         * comparator is provided) must be <i>consistent with equals</i> if it is to
         * correctly implement the <b>Set</b> interface.  (See <b>Comparable</b>
         * or <b>Comparator</b> for a precise definition of <i>consistent with
         * equals</i>.)  This is so because the <b>Set</b> interface is defined in
         * terms of the <b>equals</b> operation, but a <b>TreeSet</b> instance
         * performs all element comparisons using its <b>compareTo</b> (or
         * <b>compare</b>) method, so two elements that are deemed equal by this method
         * are, from the standpoint of the set, equal.  The behavior of a set
         * <i>is</i> well-defined even if its ordering is inconsistent with equals; it
         * just fails to obey the general contract of the <b>Set</b> interface.
         *
         * <p><strong>Note that this implementation is not synchronized.</strong>
         * If multiple threads access a tree set concurrently, and at least one
         * of the threads modifies the set, it <i>must</i> be synchronized
         * externally.  This is typically accomplished by synchronizing on some
         * object that naturally encapsulates the set.
         * If no such object exists, the set should be "wrapped" using the
         * <b style="color:orange;">core.util.SynchronizedSortedSet</b>
         * method.
         *
         * <p>The iterators returned by this class's <b>iterator</b> method are
         * <i>fail-fast</i>: if the set is modified at any time after the iterator is
         * created, in any way except through the iterator's own <b>remove</b>
         * method, the iterator will throw a <b style="color:orange;">ConcurrentException</b>.
         * Thus, in the face of concurrent modification, the iterator fails quickly
         * and cleanly, rather than risking arbitrary, non-deterministic behavior at
         * an undetermined time in the future.
         *
         * <p>Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw <b>ConcurrentException</b> on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness:   <i>the fail-fast behavior of iterators
         * should be used only to detect bugs.</i>
         *
         * @param E the type of elements maintained by this set
         *
         * @author  Brunshweeck Tazeussong
         * @see     Collection
         * @see     Set
         * @see     HashSet
         * @see     Comparable
         * @see     Comparator
         * @see     TreeMap
         */
        template<class E>
        class TreeSet : public Set<E>, public SortedStruct<E> {
        private:

            CORE_ALIAS(U, native::Unsafe);

            /**
             * Capture<T> represent all type T that extends this value type E.
             * in other word E is base of T (Class<E>::isSuper<T>() is true).
             */
            template<class T>
            CORE_ALIAS(Capture, typename Class<T>::template Iff<Class<E>::template isSuper<T>()>);

            /**
             * The backing map (The Dummy value used is null instance)
             */
            TreeMap<E, Null> m;

            /**
             * Constructs a set backed by the specified map.
             */
            CORE_EXPLICIT TreeSet(const TreeMap<E, Null> &m) : m(m) {}

        public:

            /**
             * Constructs a new, empty tree set, sorted according to the
             * natural ordering of its elements.  All elements inserted into
             * the set must implement the <b style="color:orange;"> Comparable</b>  interface.
             * Furthermore, all such elements must be <i>mutually
             * comparable</i>: <b> e1.compareTo(e2)</b>  must not throw a
             * <b> CastException</b>  for any elements <b> e1</b>  and
             * <b> e2</b>  in the set.  If the user attempts to add an element
             * to the set that violates this constraint (for example, the user
             * attempts to add a string element to a set whose elements are
             * integers), the <b> add</b>  call will throw a
             * <b> CastException</b> .
             */
            CORE_IMPLICIT TreeSet() : TreeSet(TreeMap<E, Null>{}) {}

            /**
             * Constructs a new, empty tree set, sorted according to the specified
             * comparator.  All elements inserted into the set must be <i>mutually
             * comparable</i> by the specified comparator: <b> comparator.compare(e1,
             * e2)</b>  must not throw a <b> CastException</b>  for any elements
             * <b> e1</b>  and <b> e2</b>  in the set.  If the user attempts to add
             * an element to the set that violates this constraint, the
             * <b> add</b>  call will throw a <b> CastException</b> .
             *
             * @param comparator the comparator that will be used to order this set.
             *        If <b> null</b> , the <b style="color:green;"> natural
             *        ordering</b>  of the elements will be used.
             */
            template<class T = E>
            CORE_EXPLICIT TreeSet(const Comparator<Capture<T>> &comparator): TreeSet(TreeMap<E, Null>(comparator)) {}

            /**
             * Constructs a new tree set containing the elements in the specified
             * collection, sorted according to the <i>natural ordering</i> of its
             * elements.  All elements inserted into the set must implement the
             * <b style="color:orange;"> Comparable</b>  interface.  Furthermore, all such elements must be
             * <i>mutually comparable</i>: <b> e1.compareTo(e2)</b>  must not throw a
             * <b> CastException</b>  for any elements <b> e1</b>  and
             * <b> e2</b>  in the set.
             *
             * @param c collection whose elements will comprise the new set
             * @throws CastException if the elements in <b> c</b>  are
             *         not <b style="color:orange;"> Comparable</b> , or are not mutually comparable
             */
            template<class T = E>
            CORE_EXPLICIT TreeSet(const Collection<Capture<T>> &c): TreeSet() { TreeSet<E>::addAll(c); }

            /**
             * Returns an iterator over the elements in this set in ascending order.
             *
             * @return an iterator over the elements in this set in ascending order
             */
            Iterator<const E> &iterator() const override { return m.keySet().iterator(); }

            /**
             * Returns an iterator over the elements in this set in descending order.
             *
             * @return an iterator over the elements in this set in descending order
             * @since 1.6
             */
            virtual Iterator<const E> &reversedIterator() { return m.reversedKeySet().iterator(); }

            /**
             * Returns the number of elements in this set (its cardinality).
             *
             * @return the number of elements in this set (its cardinality)
             */
            gint size() const override { return m.size(); }

            /**
             * Returns <b> true</b>  if this set contains the specified element.
             * More formally, returns <b> true</b>  if and only if this set
             * contains an element <b> e</b>  such that
             * <b> Objects.equals(o, e)</b> .
             *
             * @param o object to be checked for containment in this set
             * @return <b> true</b>  if this set contains the specified element
             * @throws CastException if the specified object cannot be compared
             *         with the elements currently in the set
             */
            gbool contains(const E &o) const override { return m.containsKey(o); }

            /**
             * Adds the specified element to this set if it is not already present.
             * More formally, adds the specified element <b> e</b>  to this set if
             * the set contains no element <b> e2</b>  such that
             * <b> Objects.equals(e, e2)</b> .
             * If this set already contains the element, the call leaves the set
             * unchanged and returns <b> false</b> .
             *
             * @param e element to be added to this set
             * @return <b> true</b>  if this set did not already contain the specified
             *         element
             * @throws CastException if the specified object cannot be compared
             *         with the elements currently in this set
             */
            gbool add(const E &e) override {
                if (m.containsKey(e))
                    return false;
                m.put(e, null);
                return true;
            }

            /**
             * Removes the specified element from this set if it is present.
             * More formally, removes an element <b> e</b>  such that
             * <b> Objects.equals(o, e)</b> ,
             * if this set contains such an element.  Returns <b> true</b>  if
             * this set contained the element (or equivalently, if this set
             * changed as a result of the call).  (This set will not contain the
             * element once the call returns.)
             *
             * @param o object to be removed from this set, if present
             * @return <b> true</b>  if this set contained the specified element
             * @throws CastException if the specified object cannot be compared
             *         with the elements currently in this set
             */
            gbool remove(const E &o) override { return m.remove(o, null); }

            /**
             * Removes all of the elements from this set.
             * The set will be empty after this call returns.
             */
            void clear() override { m.clear(); }

            /**
             * Adds all of the elements in the specified collection to this set.
             *
             * @param c collection containing elements to be added to this set
             * @return <b> true</b>  if this set changed as a result of the call
             * @throws CastException if the elements provided cannot be compared
             *         with the elements currently in the set
             */
            gbool addAll(const Collection<E> &c) override { return addAll<E>(c); }

            /**
             * Adds all of the elements in the specified collection to this set.
             *
             * @param c collection containing elements to be added to this set
             * @return <b> true</b>  if this set changed as a result of the call
             * @throws CastException if the elements provided cannot be compared
             *         with the elements currently in the set
             */
            template<class T = E>
            gbool addAll(const Collection<Capture<T>> &c) {
                // Use linear-time version if applicable
                if (m.size() == 0 && c.size() > 0 && Class<SortedStruct<T>>::hasInstance(c)) {
                    const SortedStruct<T> &s = (SortedStruct<E> &) c;
                    if (Object::equals(s.comparator(), m.comparator())) {
                        m.buildFromSorted(c.size(), c.iterator(), null);
                        return true;
                    }
                }
                return Set<E>::addAll<T>(c);
            }

            /**
             *
             */
            const Comparator<E> &comparator() const override { return m.comparator(); }

            /**
             * Returns a shallow copy of this <b> TreeSet</b>  instance. (The elements
             * themselves are not cloned.)
             *
             * @return a shallow copy of this set
             */
            Object &clone() const override { return U::createInstance<TreeSet<E>>(*this); }

        };

    }
} // core

#endif //CORE23_TREESET_H
