//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_HASHSET_H
#define CORE23_HASHSET_H

#include <core/util/Set.h>
#include <core/util/HashMap.h>

namespace core {
    namespace util {

        /**
         * This class implements the <b> Set</b>  interface, backed by a hash table
         * (actually a <b> HashMap</b>  instance).  It makes no guarantees as to the
         * iteration order of the set; in particular, it does not guarantee that the
         * order will remain constant over time.  This class permits the <b> null</b> 
         * element.
         *
         * <p>This class offers constant time performance for the basic operations
         * (<b> add</b> , <b> remove</b> , <b> contains</b>  and <b> size</b> ),
         * assuming the hash function disperses the elements properly among the
         * buckets.  Iterating over this set requires time proportional to the sum of
         * the <b> HashSet</b>  instance's size (the number of elements) plus the
         * "capacity" of the backing <b> HashMap</b>  instance (the number of
         * buckets).  Thus, it's very important not to set the initial capacity too
         * high (or the load factor too low) if iteration performance is important.
         *
         * <p><strong>Note that this implementation is not synchronized.</strong>
         * If multiple threads access a hash set concurrently, and at least one of
         * the threads modifies the set, it <i>must</i> be synchronized externally.
         * This is typically accomplished by synchronizing on some object that
         * naturally encapsulates the set.
         *
         * If no such object exists, the set should be "wrapped" using the
         * <b style="color:orange;"> core.util.SynchronizedSet</b> 
         * method.  This is best done at creation time, to prevent accidental
         * unsynchronized access to the set:<pre>
         *   Set s = SynchronizedSet(new HashSet(...));</pre>
         *
         * <p>The iterators returned by this class's <b> iterator</b>  method are
         * <i>fail-fast</i>: if the set is modified at any time after the iterator is
         * created, in any way except through the iterator's own <b> remove</b> 
         * method, the Iterator throws a <b style="color:orange;"> ConcurrentException</b> .
         * Thus, in the face of concurrent modification, the iterator fails quickly
         * and cleanly, rather than risking arbitrary, non-deterministic behavior at
         * an undetermined time in the future.
         *
         * <p>Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw <b> ConcurrentException</b>  on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness: <i>the fail-fast behavior of iterators
         * should be used only to detect bugs.</i>
         *
         * @param E the type of elements maintained by this set
         *
         * @author  Brunshweeck Tazeussong
         * @see     Collection
         * @see     Set
         * @see     TreeSet
         * @see     HashMap
         */
        template<class E>
        class HashSet : public Set<E> {
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
            HashMap<E, Null> m;

        public:

            /**
             * Constructs a new, empty set; the backing <b> HashMap</b>  instance has
             * default initial capacity (16) and load factor (0.75).
             */
            HashSet() : m() {}

            /**
             * Constructs a new set containing the elements in the specified
             * collection.  The <b> HashMap</b>  is created with default load factor
             * (0.75) and an initial capacity sufficient to contain the elements in
             * the specified collection.
             *
             * @param c the collection whose elements are to be placed into this set
             */
            template<class T>
            CORE_EXPLICIT HashSet(const Collection<Capture<T>> &c): m(Math::max(c.size(), 12)) {
                HashSet<E>::addAll(c);
            }

            /**
             * Constructs a new, empty set; the backing <b> HashMap</b>  instance has
             * the specified initial capacity and the specified load factor.
             *
             * @apiNote
             * To create a <b> HashSet</b>  with an initial capacity that accommodates
             * an expected number of elements, use <b style="color:orange;"> #newHashSet(gint) newHashSet</b> .
             *
             * @param      initialCapacity   the initial capacity of the hash map
             * @param      loadFactor        the load factor of the hash map
             * @throws     IllegalArgumentException if the initial capacity is less
             *             than zero, or if the load factor is nonpositive
             */
             CORE_EXPLICIT HashSet(gint initialCapacity, gfloat loadFactor): m(initialCapacity, loadFactor) {}

            /**
             * Constructs a new, empty set; the backing <b> HashMap</b>  instance has
             * the specified initial capacity and default load factor (0.75).
             *
             * @apiNote
             * To create a <b> HashSet</b>  with an initial capacity that accommodates
             * an expected number of elements, use <b style="color:orange;"> #newHashSet(gint) newHashSet</b> .
             *
             * @param      initialCapacity   the initial capacity of the hash table
             * @throws     IllegalArgumentException if the initial capacity is less
             *             than zero
             */
             CORE_EXPLICIT HashSet(gint initialCapacity): m(initialCapacity) {}

            /**
             * Returns an iterator over the elements in this set.  The elements
             * are returned in no particular order.
             *
             * @return an Iterator over the elements in this set
             * @see ConcurrentModificationException
             */
            Iterator<const E> &iterator() const override { return m.keySet().iterator(); }

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
             * @param o element whose presence in this set is to be tested
             * @return <b> true</b>  if this set contains the specified element
             */
            gbool contains(const E &o) const override { return m.containsKey(o); }

            /**
             * Adds the specified element to this set if it is not already present.
             * More formally, adds the specified element <b> e</b>  to this set if
             * this set contains no element <b> e2</b>  such that
             * <b> Objects.equals(e, e2)</b> .
             * If this set already contains the element, the call leaves the set
             * unchanged and returns <b> false</b> .
             *
             * @param e element to be added to this set
             * @return <b> true</b>  if this set did not already contain the specified
             * element
             */
            gbool add(const E &e) override {
                if(m.containsKey(e))
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
             * @return <b> true</b>  if the set contained the specified element
             */
            gbool remove(const E &o) override { return m.remove(o, null); }

            /**
             * Removes all of the elements from this set.
             * The set will be empty after this call returns.
             */
            void clear() override { m.clear(); }

            /**
             * Returns a shallow copy of this <b> HashSet</b>  instance: the elements
             * themselves are not cloned.
             *
             * @return a shallow copy of this set
             */
            Object &clone() const override { return U::createInstance<HashSet<E>>(*this); }

            /**
             *
             */
            ReferenceArray toArray() const override { return m.keySet().toArray(); }

        };

    }
} // core

#endif //CORE23_HASHSET_H
