//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_HASHTABLE_H
#define CORE23_HASHTABLE_H

#include "Map.h"

namespace core {
    namespace util {

        /**
         * This class implements a hash table, which maps keys to values. Any
         * object can be used as a key or as a value. <p>
         *
         * To successfully store and retrieve objects from a hashtable, the
         * objects used as keys must implement the {@code hash}
         * method and the {@code equals} method. <p>
         *
         * An instance of {@code Hashtable} has two parameters that affect its
         * performance: <i>initial capacity</i> and <i>load factor</i>.  The
         * <i>capacity</i> is the number of <i>buckets</i> in the hash table, and the
         * <i>initial capacity</i> is simply the capacity at the time the hash table
         * is created.  Note that the hash table is <i>open</i>: in the case of a "hash
         * collision", a single bucket stores multiple entries, which must be searched
         * sequentially.  The <i>load factor</i> is a measure of how full the hash
         * table is allowed to get before its capacity is automatically increased.
         * The initial capacity and load factor parameters are merely hints to
         * the implementation.  The exact details as to when and whether the rehash
         * method is invoked are implementation-dependent.<p>
         *
         * Generally, the default load factor (0.75) offers a good tradeoff between
         * time and space costs.  Higher values decrease the space overhead but
         * increase the time cost to look up an entry (which is reflected in most
         * {@code Hashtable} operations, including {@code get} and {@code put}).<p>
         *
         * The initial capacity controls a tradeoff between wasted space and the
         * need for {@code rehash} operations, which are time-consuming.
         * No {@code rehash} operations will <i>ever</i> occur if the initial
         * capacity is greater than the maximum number of entries the
         * {@code Hashtable} will contain divided by its load factor.  However,
         * setting the initial capacity too high can waste space.<p>
         *
         * If many entries are to be made into a {@code Hashtable},
         * creating it with a sufficiently large capacity may allow the
         * entries to be inserted more efficiently than letting it perform
         * automatic rehashing as needed to grow the table. <p>
         *
         * This example creates a hashtable of numbers. It uses the names of
         * the numbers as keys:
         * <pre>   {@code
         *   Hashtable<String, Integer> numbers = {};
         *   numbers.put("one", 1);
         *   numbers.put("two", 2);
         *   numbers.put("three", 3);}</pre>
         *
         * <p>To retrieve a number, use the following code:
         * <pre>   {@code
         *   Integer n = numbers.get("two");
         *   if (n != null) {
         *     System.out.println("two = " + n);
         *   }}</pre>
         *
         * <p>The iterators returned by the {@code iterator} method of the collections
         * returned by all of this class's "collection view methods" are
         * <em>fail-fast</em>: if the Hashtable is structurally modified at any time
         * after the iterator is created, in any way except through the iterator's own
         * {@code remove} method, the iterator will throw a {@link
         * ConcurrentModificationException}.  Thus, in the face of concurrent
         * modification, the iterator fails quickly and cleanly, rather than risking
         * arbitrary, non-deterministic behavior at an undetermined time in the future.
         * The Enumerations returned by Hashtable's {@link #keys keys} and
         * {@link #elements elements} methods are <em>not</em> fail-fast; if the
         * Hashtable is structurally modified at any time after the enumeration is
         * created then the results of enumerating are undefined.
         *
         * <p>Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw {@code ConcurrentModificationException} on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness: <i>the fail-fast behavior of iterators
         * should be used only to detect bugs.</i>
         *
         * <p>As of the Java 2 platform v1.2, this class was retrofitted to
         * implement the {@link Map} interface, making it a member of the
         * <a href="{@docRoot}/java.base/java/util/package-summary.html#CollectionsFramework">
         *
         * Java Collections Framework</a>.  Unlike the new collection
         * implementations, {@code Hashtable} is synchronized.  If a
         * thread-safe implementation is not needed, it is recommended to use
         * {@link HashMap} in place of {@code Hashtable}.  If a thread-safe
         * highly-concurrent implementation is desired, then it is recommended
         * to use {@link java.util.concurrent.ConcurrentHashMap} in place of
         * {@code Hashtable}.
         *
         * @param K the type of keys maintained by this map
         * @param V the type of mapped values
         *
         * @author  Brunshweeck Tazeussong
         * @see     Object::equals(Object)
         * @see     Object::hash()
         * @see     Hashtable::rehash()
         * @see     Collection
         * @see     Map
         * @see     HashMap
         * @see     TreeMap
         */
        template<class K, class V>
        class HashTable : public Map<K, V> {
        private:
            using typename Map<K, V>::Entry;

            /**
             *
             */
            CORE_ALIAS(COMPARATOR, typename Class<Comparator<K>>::Ptr);
            CORE_ALIAS(ENTRY, typename Class<Entry>::Ptr);
            CORE_ALIAS(TABLE, typename Class<ENTRY>::Ptr);
            CORE_ALIAS(KEY, typename Class<K>::Ptr);
            CORE_ALIAS(VALUE, typename Class<V>::Ptr);
            CORE_ALIAS(ENTRIES, , typename Class<Set<Entry>>::Ptr);
            CORE_ALIAS(VALUES, typename Class<Collection<V>>::Ptr);
            CORE_ALIAS(KEYSET, typename Class<Set<K>>::Ptr);

            /**
             * The hash table data.
             */
            TABLE table;

            /**
             * The total number of entries in the hash table.
             */
            gint len;

            /**
             * The size of table
             */
            gint capacity;

            /**
             * The table is rehashed when its size exceeds this threshold.  (The
             * value of this field is (int)(capacity * loadFactor).)
             */
            gint threshold;

            /**
             * The load factor for the hashtable.
             */
            gfloat loadFactor;

            /**
             * The number of times this Hashtable has been structurally modified
             * Structural modifications are those that change the number of entries in
             * the Hashtable or otherwise modify its internal structure (e.g.,
             * rehash).  This field is used to make iterators on Collection-views of
             * the Hashtable fail-fast.  (See ConcurrentModificationException).
             */
            gint modNum;

            /**
             * The maximum size of array to allocate.
             * Some VMs reserve some header words in an array.
             * Attempts to allocate larger arrays may result in
             * OutOfMemoryError: Requested array size exceeds VM limit
             */
            static CORE_FAST gint MAX_ARRAY_SIZE = Integer::MAX_VALUE - 8;

        public:

            /**
             * Constructs a new, empty hashtable with the specified initial
             * capacity and the specified load factor.
             *
             * @param      initialCapacity   the initial capacity of the hashtable.
             * @param      loadFactor        the load factor of the hashtable.
             * @throws     IllegalArgumentException  if the initial capacity is less
             *             than zero, or if the load factor is nonpositive.
             */
            CORE_EXPLICIT HashTable(gint initialCapacity, gfloat loadFactor) :
                    table(null), len(0), capacity(0), threshold(0), loadFactor(0.0F), modNum(0) {
                if (initialCapacity < 0)
                    IllegalArgumentException("Illegal Capacity: " + String::valueOf(initialCapacity))
                            .throws(__trace("core.util.HashTable"));
                if (loadFactor <= 0 || Float::isNaN(loadFactor))
                    IllegalArgumentException("Illegal Load: " + String::valueOf(loadFactor))
                            .throws(__trace("core.util.HashTable"));

                if (initialCapacity == 0)
                    initialCapacity = 1;
                HashTable::loadFactor = loadFactor;
                table = (TABLE) Unsafe::allocateMemory((capacity = initialCapacity) * Unsafe::ADDRESS_SIZE);
                threshold = (int) Math::min((gint) ((gfloat) initialCapacity * loadFactor), MAX_ARRAY_SIZE + 1);
            }

            /**
             * Constructs a new, empty hashtable with the specified initial capacity
             * and default load factor (0.75).
             *
             * @param     initialCapacity   the initial capacity of the hashtable.
             * @throws    IllegalArgumentException if the initial capacity is less
             *              than zero.
             */
            CORE_EXPLICIT HashTable(gint initialCapacity) : HashTable(initialCapacity, 0.75F) {}

            /**
             * Constructs a new, empty hashtable with a default initial capacity (11)
             * and load factor (0.75).
             */
            HashTable() : HashTable(11, 0.75F) {}

            /**
             * Constructs a new hashtable with the same mappings as the given
             * Map.  The hashtable is created with an initial capacity sufficient to
             * hold the mappings in the given Map and a default load factor (0.75).
             *
             * @param t the map whose mappings are to be placed in this map.
             * @throws NullPointerException if the specified map is null.
             * @since   1.2
             */
            CORE_EXPLICIT HashTable(const Map<K, V> &m) : HashTable(Math::max(2 * m.size(), 11), 0.75F) {}

        protected:

            /**
             * A constructor chained from {@link Properties} keeps Hashtable fields
             * uninitialized since they are not used.
             *
             * @param dummy a dummy parameter
             */
            CORE_EXPLICIT HashTable(Null &dummy) :
                    table(null), len(0), capacity(0), threshold(0), loadFactor(0.0F), modNum(0) {}

        public:
            /**
             * Returns the number of keys in this hashtable.
             *
             * @return  the number of keys in this hashtable.
             */
            gint size() const override {
                /* Synchronized */
                return len;
            }

            /**
             * Tests if this hashtable maps no keys to values.
             *
             * @return  {@code true} if this hashtable maps no keys to values;
             *          {@code false} otherwise.
             */
            gbool isEmpty() const override {
                /* Synchronized */
                return len == 0;
            }

        };

    }
} // core

#endif //CORE23_HASHTABLE_H
