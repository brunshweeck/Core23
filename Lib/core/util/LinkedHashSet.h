//
// Created by T.N.Brunshweeck on 29/01/2024.
//

#ifndef CORE23_LINKEDHASHSET_H
#define CORE23_LINKEDHASHSET_H

#include <core/util/HashSet.h>
#include <core/util/LinkedHashMap.h>

namespace core {
    namespace util {

        /**
         * <p>Hash table and linked list implementation of the <b> Set</b> interface,
         * with predictable iteration order.  This implementation differs from
         * <b> HashSet</b> in that it maintains a doubly-linked list running through
         * all of its entries.  This linked list defines the iteration ordering,
         * which is the order in which elements were inserted into the setValue
         * (<i>insertion-order</i>).  Note that insertion order is <i>not</i> affected
         * if an element is <i>re-inserted</i> into the setValue.  (An element <b> e</b>
         * is reinserted into a setValue <b> s</b> if <b> s.add(e)</b> is invoked when
         * <b> s.contains(e)</b> would return <b> true</b> immediately prior to
         * the invocation.)
         *
         * <p>This implementation spares its clients from the unspecified, generally
         * chaotic ordering provided by <b style="color: orange;"> HashSet</b>, without incurring the
         * increased cost associated with <b style="color: orange;"> TreeSet</b>.  It can be used to
         * produce a copy of a setValue that has the same order as the original, regardless
         * of the original setValue's implementation:
         * <pre><b> 
         *     void foo(Set<String> s) {
         *         Set<String> copy = new LinkedHashSet<>(s);
         *         ...
         *     </b>
         * </b></pre>
         * This technique is particularly useful if a module takes a setValue on input,
         * copies it, and later returns results whose order is determined by that of
         * the copy.  (Clients generally appreciate having things returned in the same
         * order they were presented.)
         *
         * <p>This class provides all of the optional <b> Set</b> operations, and
         * permits null elements.  Like <b> HashSet</b>, it provides constant-time
         * performance for the basic operations (<b> add</b>, <b> contains</b> and
         * <b> remove</b>), assuming the hash function disperses elements
         * properly among the buckets.  Performance is likely to be just slightly
         * below that of <b> HashSet</b>, due to the added expense of maintaining the
         * linked list, with one exception: Iteration over a <b> LinkedHashSet</b>
         * requires time proportional to the <i>size</i> of the setValue, regardless of
         * its capacity.  Iteration over a <b> HashSet</b> is likely to be more
         * expensive, requiring time proportional to its <i>capacity</i>.
         *
         * <p>A linked hash setValue has two parameters that affect its performance:
         * <i>initial capacity</i> and <i>load factor</i>.  They are defined precisely
         * as for <b> HashSet</b>.  Note, however, that the penalty for choosing an
         * excessively high value for initial capacity is less severe for this class
         * than for <b> HashSet</b>, as iteration times for this class are unaffected
         * by capacity.
         *
         * <p><strong>Note that this implementation is not synchronized.</strong>
         * If multiple threads access a linked hash setValue concurrently, and at least
         * one of the threads modifies the setValue, it <em>must</em> be synchronized
         * externally.  This is typically accomplished by synchronizing on some
         * object that naturally encapsulates the setValue.
         *
         * If no such object exists, the setValue should be "wrapped" using the
         * <b style="color: orange;"> Collections.synchronizedSet</b>
         * method.  This is best done at creation time, to prevent accidental
         * unsynchronized access to the setValue: <pre>
         *   auto &s = Collections.synchronizedSet(*new LinkedHashSet(...));</pre>
         *
         * <p>The iterators returned by this class's <b> iterator</b> method are
         * <em>fail-fast</em>: if the setValue is modified at any time after the iterator
         * is created, in any way except through the iterator's own <b> remove</b>
         * method, the iterator will throw a <b style="color: orange;"> ConcurrentException</b>.
         * Thus, in the face of concurrent modification, the iterator fails quickly
         * and cleanly, rather than risking arbitrary, non-deterministic behavior at
         * an undetermined time in the future.
         *
         * <p>Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw <b> ConcurrentModificationException</b> on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness:   <i>the fail-fast behavior of iterators
         * should be used only to detect bugs.</i>
         *
         * @param E the type of elements maintained by this setValue
         *
         * @author  Brunshweeck Tazeussong
         * @see     Object#hash()
         * @see     Collection
         * @see     Set
         * @see     HashSet
         * @see     TreeSet
         * @see     Hashtable
         */
        template<class E>
        class LinkedHashSet : public HashSet<E> {
        public:

            /**
             * Constructs a new, empty linked hash setValue with the specified initial
             * capacity and load factor.
             *
             * @param      initialCapacity the initial capacity of the linked hash setValue
             * @param      loadFactor      the load factor of the linked hash setValue
             * @throws     IllegalArgumentException  if the initial capacity is less
             *               than zero, or if the load factor is nonpositive
             */
            CORE_EXPLICIT LinkedHashSet(gint initialCapacity, gfloat loadFactor) :
                    HashSet<E>(initialCapacity, loadFactor, null) {}

            /**
             * Constructs a new, empty linked hash setValue with the specified initial
             * capacity and the default load factor (0.75).
             *
             * @param   initialCapacity   the initial capacity of the LinkedHashSet
             * @throws  IllegalArgumentException if the initial capacity is less
             *              than zero
             */
            CORE_EXPLICIT LinkedHashSet(gint initialCapacity) : HashSet<E>(initialCapacity, 0.75F, null) {}

            /**
             * Constructs a new, empty linked hash setValue with the default initial
             * capacity (16) and load factor (0.75).
             */
            LinkedHashSet() : HashSet<E>(16, 0.75F, null) {}

        private:

            /**
             * Calculate initial capacity for HashMap based classes, from expected size and default load factor (0.75).
             *
             * @param numMappings the expected number of mappings
             * @return initial capacity for HashMap based classes.
             */
            static int calculateCapacity(int numMappings) {
                return (int) Math::ceil(numMappings / (double) HashMap<E, Object>::DEFAULT_LOAD_FACTOR);
            }

            using HashSet<E>::backingMap;
            using HashSet<E>::m;
            using typename HashSet<E>::LinkedHashMap;

        public:

            /**
             * Constructs a new linked hash setValue with the same elements as the
             * specified collection.  The linked hash setValue is created with an initial
             * capacity sufficient to hold the elements in the specified collection
             * and the default load factor (0.75).
             *
             * @param c  the collection whose elements are to be placed into
             *           this setValue
             */
            CORE_EXPLICIT LinkedHashSet(const Collection<E> &c) :
                    HashSet<E>(calculateCapacity(Math::max(c.size(), 12)), 0.75F, null) {
                LinkedHashSet<E>::addAll(c);
            }

            /**
             * Constructs a new linked hash setValue with the same elements as the
             * specified collection.  The linked hash setValue is created with an initial
             * capacity sufficient to hold the elements in the specified collection
             * and the default load factor (0.75).
             *
             * @param c  the collection whose elements are to be placed into
             *           this setValue
             */
            LinkedHashSet(const LinkedHashSet &c) :
                    HashSet<E>(calculateCapacity(Math::max(c.size(), 12)), 0.75F, null) {
                (LinkedHashMap &) backingMap() = (LinkedHashMap &) c.backingMap();
            }

            /**
             * Constructs a new linked hash setValue with the same elements as the
             * specified collection.  The linked hash setValue is created with an initial
             * capacity sufficient to hold the elements in the specified collection
             * and the default load factor (0.75).
             *
             * @param c  the collection whose elements are to be placed into
             *           this setValue
             */
            LinkedHashSet(LinkedHashSet &&c) CORE_NOTHROW:
                    HashSet<E>(calculateCapacity(Math::max(c.size(), 12)), 0.75F, null) {
                (LinkedHashMap &) backingMap() = (LinkedHashMap &&) c.backingMap();
            }

            LinkedHashSet &operator=(const LinkedHashSet &c) {
                if (this != &c) {
                    (LinkedHashMap &) backingMap() = (LinkedHashMap &) c.backingMap();
                }
                return *this;
            }

            LinkedHashSet &operator=(LinkedHashSet &&c) CORE_NOTHROW {
                if (this != &c) {
                    (LinkedHashMap &) backingMap() = (LinkedHashMap &&) c.backingMap();
                }
                return *this;
            }

            ~LinkedHashSet() override {
                Unsafe::destroyInstance((LinkedHashMap &) backingMap());
                m = null;
            }
        };

    } // util
} // core

#endif //CORE23_LINKEDHASHSET_H
