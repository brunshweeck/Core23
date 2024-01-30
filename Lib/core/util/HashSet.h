//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_HASHSET_H
#define CORE23_HASHSET_H

#include <core/util/Set.h>
#include <core/util/HashMap.h>
#include <core/util/LinkedHashMap.h>

namespace core {
    namespace util {

        /**
         * This class implements the <b> Set</b>  interface, backed by a hash table
         * (actually a <b> HashMap</b>  INSTANCE).  It makes no guarantees as to the
         * iteration order of the setValue; in particular, it does not guarantee that the
         * order will remain constant over time.  This class permits the <b> null</b> 
         * element.
         *
         * <p>This class offers constant time performance for the basic operations
         * (<b> add</b> , <b> remove</b> , <b> contains</b>  and <b> size</b> ),
         * assuming the hash function disperses the elements properly among the
         * buckets.  Iterating over this setValue requires time proportional to the sum of
         * the <b> HashSet</b>  INSTANCE's size (the number of elements) plus the
         * "capacity" of the backing <b> HashMap</b>  INSTANCE (the number of
         * buckets).  Thus, it's very important not to setValue the initial capacity too
         * high (or the load factor too low) if iteration performance is important.
         *
         * <p><strong>Note that this implementation is not synchronized.</strong>
         * If multiple threads access a hash setValue concurrently, and at least one of
         * the threads modifies the setValue, it <i>must</i> be synchronized externally.
         * This is typically accomplished by synchronizing on some object that
         * naturally encapsulates the setValue.
         *
         * If no such object exists, the setValue should be "wrapped" using the
         * <b style="color:orange;"> core.util.SynchronizedSet</b> 
         * method.  This is best done at creation time, to prevent accidental
         * unsynchronized access to the setValue:<pre>
         *   Set s = SynchronizedSet(new HashSet(...));</pre>
         *
         * <p>The iterators returned by this class's <b> iterator</b>  method are
         * <i>fail-fast</i>: if the setValue is modified at any time after the iterator is
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
         * @param E the type of elements maintained by this setValue
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

            CORE_ALIAS(HashMap, , HashMap<E, Object>);
            CORE_ALIAS(LinkedHashMap, , LinkedHashMap<E, Object>);
            CORE_ALIAS(MAP, , typename Class<HashMap>::Pointer);
            CORE_ALIAS(LMAP, , typename Class<LinkedHashMap>::Pointer);

            /**
             * The backing map (The Dummy value used is null instance)
             */
            MAP m;

        protected:
            HashMap &backingMap() const {
                return *m;
            }

            CORE_FRATERNITY_T1(LinkedHashSet);

        public:

            /**
             * Constructs a new, empty setValue; the backing <b> HashMap</b>  INSTANCE has
             * default initial capacity (16) and load factor (0.75).
             */
            HashSet() : HashSet(16, 0.75F) {};

            /**
             * Constructs a new setValue containing the elements in the specified
             * collection.  The <b> HashMap</b>  is created with default load factor
             * (0.75) and an initial capacity sufficient to contain the elements in
             * the specified collection.
             *
             * @param c the collection whose elements are to be placed into this setValue
             */
            CORE_EXPLICIT HashSet(const Collection<E> &c) : HashSet(Math::max(c.size(), 12), 0.75F) {
                HashSet<E>::addAll(c);
            }

            /**
             * Constructs a new setValue containing the elements in the specified
             * collection.  The <b> HashMap</b>  is created with default load factor
             * (0.75) and an initial capacity sufficient to contain the elements in
             * the specified collection.
             *
             * @param c the collection whose elements are to be placed into this setValue
             */
            HashSet(const HashSet &c) : HashSet(Math::max(c.size(), 12), 0.75F) {
                HashSet<E>::addAll(c);
            }

            /**
             * Constructs a new setValue containing the elements in the specified
             * collection.  The <b> HashMap</b>  is created with default load factor
             * (0.75) and an initial capacity sufficient to contain the elements in
             * the specified collection.
             *
             * @param c the collection whose elements are to be placed into this setValue
             */
            HashSet(HashSet &&c) CORE_NOTHROW: HashSet() {
                backingMap() = Unsafe::moveInstance(c.backingMap());
            }

            HashSet &operator=(const HashSet &c) {
                if (this != &c) {
                    backingMap() = c.backingMap();
                }
                return *this;
            }

            HashSet &operator=(HashSet &&c) CORE_NOTHROW {
                if (this != &c) {
                    backingMap() = Unsafe::moveInstance(c.backingMap());
                }
                return *this;
            }

            /**
             * Constructs a new, empty setValue; the backing <b> HashMap</b>  INSTANCE has
             * the specified initial capacity and the specified load factor.
             *
             * @param      initialCapacity   the initial capacity of the hash map
             * @param      loadFactor        the load factor of the hash map
             * @throws     IllegalArgumentException if the initial capacity is less
             *             than zero, or if the load factor is non-positive
             */
            CORE_EXPLICIT HashSet(gint initialCapacity, gfloat loadFactor) : m(null) {
                m = &Unsafe::allocateInstance<HashMap>(initialCapacity, loadFactor);
            }

            /**
             * Constructs a new, empty setValue; the backing <b> HashMap</b>  INSTANCE has
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
            CORE_EXPLICIT HashSet(gint initialCapacity) : HashSet(initialCapacity, 0.75F) {}

        protected:

            /**
             * Constructs a new, empty linked hash setValue.  (This package private
             * constructor is only used by LinkedHashSet.) The backing
             * HashMap instance is a LinkedHashMap with the specified initial
             * capacity and the specified load factor.
             *
             * @param      initialCapacity   the initial capacity of the hash map
             * @param      loadFactor        the load factor of the hash map
             * @param      dummy             ignored (distinguishes this
             *             constructor from other int, float constructor.)
             * @throws     IllegalArgumentException if the initial capacity is less
             *             than zero, or if the load factor is nonpositive
             */
            CORE_EXPLICIT HashSet(gint initialCapacity, gfloat loadFactor, Null &dummy) : m(null) {
                CORE_IGNORE(dummy);
                m = &Unsafe::allocateInstance<LinkedHashMap>(initialCapacity, loadFactor);
            }

        public:

            /**
             * Returns an iterator over the elements in this setValue.  The elements
             * are returned in no particular order.
             *
             * @return an Iterator over the elements in this setValue
             * @see ConcurrentModificationException
             */
            Iterator<const E> &iterator() const override {
                Set<E> const &backingSet = backingMap().keySet();
                return backingSet.iterator();
            }

            /**
             * Returns an iterator over the elements in this setValue.  The elements
             * are returned in no particular order.
             *
             * @return an Iterator over the elements in this setValue
             * @see ConcurrentModificationException
             */
            Iterator<E> &iterator() override {
                Set<E> &backingSet = backingMap().keySet();
                return backingSet.iterator();
            }

            /**
             * Returns the number of elements in this setValue (its cardinality).
             *
             * @return the number of elements in this setValue (its cardinality)
             */
            gint size() const override {
                return backingMap().size();
            }

            /**
             * Returns <b> true</b>  if this setValue contains the specified element.
             * More formally, returns <b> true</b>  if and only if this setValue
             * contains an element <b> e</b>  such that
             * <b> Objects.equals(o, e)</b> .
             *
             * @param o element whose presence in this setValue is to be tested
             * @return <b> true</b>  if this setValue contains the specified element
             */
            gbool contains(const E &o) const override {
                return backingMap().containsValue(o);
            }

            /**
             * Adds the specified element to this setValue if it is not already present.
             * More formally, adds the specified element <b> e</b>  to this setValue if
             * this setValue contains no element <b> e2</b>  such that
             * <b> Objects.equals(e, e2)</b> .
             * If this setValue already contains the element, the call leaves the set
             * unchanged and returns <b> false</b> .
             *
             * @param e element to be added to this setValue
             * @return <b> true</b>  if this setValue did not already contain the specified
             * element
             */
            gbool add(const E &e) override {
                if (m->containsKey(e)) {
                    return false;
                }
                backingMap().put(e, null);
                return true;
            }

            /**
             * Removes the specified element from this setValue if it is present.
             * More formally, removes an element <b> e</b>  such that
             * <b> Objects.equals(o, e)</b> ,
             * if this setValue contains such an element.  Returns <b> true</b>  if
             * this set contained the element (or equivalently, if this setValue
             * changed as a result of the call).  (This setValue will not contain the
             * element once the call returns.)
             *
             * @param o object to be removed from this setValue, if present
             * @return <b> true</b>  if the setValue contained the specified element
             */
            gbool remove(const E &o) override {
                return backingMap().remove(o, null);
            }

            /**
             * Removes all of the elements from this setValue.
             * The setValue will be empty after this call returns.
             */
            void clear() override {
                backingMap().clear();
            }

            /**
             * Returns a shallow copy of this <b> HashSet</b>  INSTANCE: the elements
             * themselves are not cloned.
             *
             * @return a shallow copy of this setValue
             */
            Object &clone() const override {
                HashSet &clone = Unsafe::allocateInstance<HashSet>();
                try{
                    clone.backingMap().putAll(backingMap());
                } catch (const Exception &ex) {
                    Unsafe::destroyInstance(clone);
                    CloneNotSupportedException().throws(__trace("core.util.HashSet"));
                } catch (const Throwable &th) {
                    Unsafe::destroyInstance(clone);
                    th.throws(__trace("core.util.HashSet"));
                }
                return clone;
            }

            /**
             *
             */
            Array<E> toArray() const override {
                return backingMap().keySet().toArray();
            }

            ~HashSet() override {
                Unsafe::destroyInstance(backingMap());
                m = null;
            }

        };

    }
} // core

#endif //CORE23_HASHSET_H
