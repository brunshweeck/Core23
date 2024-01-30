//
// Created by T.N.Brunshweeck on 28/01/2024.
//

#ifndef CORE23_LINKEDHASHMAP_H
#define CORE23_LINKEDHASHMAP_H

#include <core/util/HashMap.h>

namespace core {
    namespace util {

        /**
         * <p>Hash table and linked list implementation of the <b> Map</b> interface,
         * with predictable iteration order.  This implementation differs from
         * <b> HashMap</b> in that it maintains a doubly-linked list running through
         * all of its entries.  This linked list defines the iteration ordering,
         * which is normally the order in which keys were inserted into the map
         * (<i>insertion-order</i>).  Note that insertion order is not affected
         * if a key is <i>re-inserted</i> into the map.  (A key <b> k</b> is
         * reinserted into a map <b> m</b> if <b> m.put(k, v)</b> is invoked when
         * <b> m.containsKey(k)</b> would return <b> true</b> immediately prior to
         * the invocation.)
         *
         * <p>This implementation spares its clients from the unspecified, generally
         * chaotic ordering provided by <b style="color: orange;"> HashMap</b> (and <b style="color: orange;"> Hashtable</b>),
         * without incurring the increased cost associated with <b style="color: orange;"> TreeMap</b>.  It
         * can be used to produce a copy of a map that has the same order as the
         * original, regardless of the original map's implementation:
         * <pre><b> @code
         * void foo(Map<String, Integer> const &m) {
         *    auto copy = LinkedHashMap<String, Integer>(m);
         *    ...
         * }
         * @encode </b></pre>
         * This technique is particularly useful if a module takes a map on input,
         * copies it, and later returns results whose order is determined by that of
         * the copy.  (Clients generally appreciate having things returned in the same
         * order they were presented.)
         *
         * <p>A special <b style="color: orange;"> constructor</b> is
         * provided to create a linked hash map whose order of iteration is the order
         * in which its entries were last accessed, from least-recently accessed to
         * most-recently (<i>access-order</i>).  This kind of map is well-suited to
         * building LRU caches.  Invoking the <b> put</b>, <b> putIfAbsent</b>,
         * <b> get</b>, <b> getOrDefault</b>, <b> compute</b>, <b> computeIfAbsent</b>,
         * <b> computeIfPresent</b>, or <b> merge</b> methods results
         * in an access to the corresponding entry (assuming it exists after the
         * invocation completes). The <b> replace</b> methods only result in an access
         * of the entry if the value is replaced.  The <b> putAll</b> method generates one
         * entry access for each mapping in the specified map, in the order that
         * key-value mappings are provided by the specified map's entry setValue iterator.
         * <i>No other methods generate entry accesses.</i>  In particular, operations
         * on collection-views do <i>not</i> affect the order of iteration of the
         * backing map.
         *
         * <p>The <b style="color: orange;"> #removeEldestEntry(Map.Entry)</b> method may be overridden to
         * impose a policy for removing stale mappings automatically when new mappings
         * are added to the map.
         *
         * <p>This class provides all of the optional <b> Map</b> operations, and
         * permits null elements.  Like <b> HashMap</b>, it provides constant-time
         * performance for the basic operations (<b> add</b>, <b> contains</b> and
         * <b> remove</b>), assuming the hash function disperses elements
         * properly among the buckets.  Performance is likely to be just slightly
         * below that of <b> HashMap</b>, due to the added expense of maintaining the
         * linked list, with one exception: Iteration over the collection-views
         * of a <b> LinkedHashMap</b> requires time proportional to the <i>size</i>
         * of the map, regardless of its capacity.  Iteration over a <b> HashMap</b>
         * is likely to be more expensive, requiring time proportional to its
         * <i>capacity</i>.
         *
         * <p>A linked hash map has two parameters that affect its performance:
         * <i>initial capacity</i> and <i>load factor</i>.  They are defined precisely
         * as for <b> HashMap</b>.  Note, however, that the penalty for choosing an
         * excessively high value for initial capacity is less severe for this class
         * than for <b> HashMap</b>, as iteration times for this class are unaffected
         * by capacity.
         *
         * <p><strong>Note that this implementation is not synchronized.</strong>
         * If multiple threads access a linked hash map concurrently, and at least
         * one of the threads modifies the map structurally, it <em>must</em> be
         * synchronized externally.  This is typically accomplished by
         * synchronizing on some object that naturally encapsulates the map.
         *
         * If no such object exists, the map should be "wrapped" using the
         * <b style="color: orange;"> Collections#synchronizedMap Collections.synchronizedMap</b>
         * method.  This is best done at creation time, to prevent accidental
         * unsynchronized access to the map:<pre>@code
         *  auto& linkedMap = LinkedHashMap<?,?>(...);
         *  auto m = CollectionUtils::synchronizedMap(linkedMap) @endcode</pre>
         *
         * A structural modification is any operation that adds or deletes one or more
         * mappings or, in the case of access-ordered linked hash maps, affects
         * iteration order.  In insertion-ordered linked hash maps, merely changing
         * the value associated with a key that is already contained in the map is not
         * a structural modification.  <strong>In access-ordered linked hash maps,
         * merely querying the map with <b> get</b> is a structural modification.
         * </strong>)
         *
         * <p>The iterators returned by the <b> iterator</b> method of the collections
         * returned by all of this class's collection view methods are
         * <em>fail-fast</em>: if the map is structurally modified at any time after
         * the iterator is created, in any way except through the iterator's own
         * <b> remove</b> method, the iterator will throw a <b style="color: orange;"> 
         * ConcurrentModificationException</b>.  Thus, in the face of concurrent
         * modification, the iterator fails quickly and cleanly, rather than risking
         * arbitrary, non-deterministic behavior at an undetermined time in the future.
         *
         * <p>Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw <b> ConcurrentModificationException</b> on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness:   <i>the fail-fast behavior of iterators
         * should be used only to detect bugs.</i>
         *
         * @tparam K the type of keys maintained by this map
         * @tparam V the type of mapped values
         *
         * @author  Brunshweeck Tazeussong
         * @see     Object::hash()
         * @see     Collection
         * @see     Map
         * @see     HashMap
         * @see     TreeMap
         * @see     Hashtable
         */
        template<class K, class V>
        class LinkedHashMap : public HashMap<K, V> {
        private:

            /*
             * Implementation note.  A previous version of this class was
             * internally structured a little differently. Because superclass
             * HashMap now uses trees for some of its nodes, class
             * LinkedHashMap.Entry is now treated as intermediary node class
             * that can also be converted to tree form. The name of this
             * class, LinkedHashMap.Entry, is confusing in several ways in its
             * current context, but cannot be changed.  Otherwise, even though
             * it is not exported outside this package, some existing source
             * code is known to have relied on a symbol resolution corner case
             * rule in calls to removeEldestEntry that suppressed compilation
             * errors due to ambiguous usages. So, we keep the name to
             * preserve unmodified compilability.
             *
             * The changes in node classes also require using two fields
             * (head, tail) rather than a pointer to a header node to maintain
             * the doubly-linked before/after list. This class also
             * previously used a different style of callback methods upon
             * access, insertion, and removal.
             */

            /**
             * HashMap.Node subclass for normal LinkedHashMap entries.
             */
            CORE_ALIAS(Entry, , typename HashMap<K, V>::LinkedEntry);

            CORE_ALIAS(ENTRY, , typename HashMap<K, V>::LNODE);

            /**
             * The head (eldest) of the doubly linked list.
             */
            ENTRY head;

            /**
             * The tail (youngest) of the doubly linked list.
             */
            ENTRY tail;

            /**
             * The iteration ordering method for this linked hash map: <b> true</b>
             * for access-order, <b> false</b> for insertion-order.
             *
             * @serial
             */
            gbool accessOrder;

            // link at the end of list
            void linkNodeLast(ENTRY p) {
                ENTRY last = tail;
                if (last == null) {
                    return p;
                } else {
                    p->before = last;
                    last->after = p;
                }
            }

            // apply src's links to dst
            void transfertLink(ENTRY src, ENTRY dst) {
                ENTRY b = dst.before = src.before;
                ENTRY a = dst.after = src.after;
                if (b == null)
                    head = dst;
                else
                    b.after = dst;
                if (a == null)
                    tail = dst;
                else
                    a.before = dst;
            };

            // overrides of HashMap hook methods
            void reinitialize() override {
                HashMap<K, V>::reinitialize();
                head = tail = null;
            }

            using typename HashMap<K, V>::NODE;
            using typename HashMap<K, V>::TNODE;

            NODE newNode(gint hash, const K &key, const V &value, NODE next) override {
                K &keyCopy = Unsafe::copyInstance(key, true);
                V &valueCopy = Unsafe::copyInstance(value, true);
                return Unsafe::allocateInstance<Entry>(hash, keyCopy, valueCopy, next);
            }

            NODE replacementNode(NODE p, NODE next) override {
                K &keyCopy = (K &) keyOf(p);
                V &valueCopy = (V &) valueOf(p);
                ENTRY t = &Unsafe::allocateInstance<Entry>(p->hash, keyCopy, valueCopy, next);
                transferLinks((ENTRY) p, t);
                return t;
            }

            using typename HashMap<K, V>::TreeNode;

            TNODE newTNode(gint hash, const K &key, const V &value, NODE next) override {
                K &keyCopy = Unsafe::copyInstance(key, true);
                V &valueCopy = Unsafe::copyInstance(value, true);
                TNODE p = &Unsafe::allocateInstance<TreeNode>(hash, keyCopy, valueCopy, next);
                linkNodeLast(p);
                return p;
            }

            TNODE replacementTNode(NODE p, NODE next) override {
                K &keyCopy = (K &) keyOf(p);
                V &valueCopy = (V &) valueOf(p);
                ENTRY t = &Unsafe::allocateInstance<TreeNode>(p->hash, keyCopy, valueCopy, next);
                transferLinks((ENTRY) p, t);
                return t;
            }

            using HashMap<K, V>::modNum;

            void afterNodeAccess(NODE e) override {
                ENTRY last;
                if (accessOrder && (last = tail) != e) {
                    ENTRY p = (ENTRY) e;
                    ENTRY b = p->before;
                    ENTRY a = p->after;
                    p.after = null;
                    if (b == null)
                        head = a;
                    else
                        b->after = a;
                    if (a != null)
                        a->before = b;
                    else
                        last = b;
                    if (last == null)
                        head = p;
                    else {
                        p->before = last;
                        last->after = p;
                    }
                    tail = p;
                    ++modNum;
                }
            }

            using HashMap<K, V>::hash;

            void afterNodeInsertion(gbool evict) override {
                ENTRY first;
                if (evict && (first = head) != null && removeEldestEntry(first)) {
                    K key = first.key;
                    removeNode(hash(key), key, null, false, true);
                }
            }

            void afterNodeRemoval(NODE e) override {
                ENTRY p = (ENTRY) e;
                ENTRY b = p->before;
                ENTRY a = p->after;
                p.before = p->after = null;
                if (b == null)
                    head = a;
                else
                    b->after = a;
                if (a == null)
                    tail = b;
                else
                    a->before = b;
            }

            using HashMap<K, V>::valueOf;
            using HashMap<K, V>::keyOf;
            using HashMap<K, V>::entryOf;

        public:

            /**
             * Constructs an empty insertion-ordered <b> LinkedHashMap</b> instance
             * with the specified initial capacity and load factor.
             *
             * @param  initialCapacity the initial capacity
             * @param  loadFactor      the load factor
             * @throws IllegalArgumentException if the initial capacity is negative
             *         or the load factor is nonpositive
             */
            CORE_EXPLICIT LinkedHashMap(gint initialCapacity, gfloat loadFactor) :
                    HashMap<K, V>(initialCapacity, loadFactor), accessOrder(false) {}

            /**
             * Constructs an empty insertion-ordered <b> LinkedHashMap</b> instance
             * with the specified initial capacity and a default load factor (0.75).
             *
             * @param  initialCapacity the initial capacity
             * @throws IllegalArgumentException if the initial capacity is negative
             */
            CORE_EXPLICIT LinkedHashMap(gint initialCapacity) :
                    HashMap<K, V>(initialCapacity), accessOrder(false) {}

            /**
             * Constructs an empty insertion-ordered <b> LinkedHashMap</b> instance
             * with the default initial capacity (16) and load factor (0.75).
             */
            LinkedHashMap() : HashMap<K, V>(), accessOrder(false) {}

            /**
             * Constructs an insertion-ordered <b> LinkedHashMap</b> instance with
             * the same mappings as the specified map.  The <b> LinkedHashMap</b>
             * instance is created with a default load factor (0.75) and an initial
             * capacity sufficient to hold the mappings in the specified map.
             *
             * @param  m the map whose mappings are to be placed in this map
             * @throws NullPointerException if the specified map is null
             */
            CORE_EXPLICIT LinkedHashMap(const Map<K, V> &m) : HashMap<K, V>(), accessOrder(false) {
                try {
                    HashMap<K, V>::addEntries(m, false);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.util.LinkedHashMap"));
                }
            }

            /**
             * Constructs an insertion-ordered <b> LinkedHashMap</b> instance with
             * the same mappings as the specified map.  The <b> LinkedHashMap</b>
             * instance is created with a default load factor (0.75) and an initial
             * capacity sufficient to hold the mappings in the specified map.
             *
             * @param  m the map whose mappings are to be placed in this map
             * @throws NullPointerException if the specified map is null
             */
            LinkedHashMap(const LinkedHashMap &m) : HashMap<K, V>(), accessOrder(false) {
                try {
                    HashMap<K, V>::addEntries(m, false);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.util.LinkedHashMap"));
                }
            }

            /**
             * Constructs an insertion-ordered <b> LinkedHashMap</b> instance with
             * the same mappings as the specified map.  The <b> LinkedHashMap</b>
             * instance is created with a default load factor (0.75) and an initial
             * capacity sufficient to hold the mappings in the specified map.
             *
             * @param  m the map whose mappings are to be placed in this map
             * @throws NullPointerException if the specified map is null
             */
            LinkedHashMap(LinkedHashMap &&m) CORE_NOTHROW: HashMap<K, V>((HashMap<K, V> &&) m) {
                Unsafe::swapValues(head, m.head);
                Unsafe::swapValues(tail, m.tail);
                Unsafe::swapValues(accessOrder, m.accessOrder);
            }

            LinkedHashMap &operator=(const LinkedHashMap &m) {
                if (this != &m) {
                    // call base class method
                    HashMap<K, V>::operator=((HashMap<K, V> const &) m);
                    accessOrder = m.accessOrder;
                    head = m.head;
                    tail = m.tail;
                }
                return *this;
            }

            LinkedHashMap &operator=(LinkedHashMap<K, V> &&m) CORE_NOTHROW {
                if (this != &m) {
                    // call base class method
                    HashMap<K, V>::operator=((HashMap<K, V> &&) m);
                    Unsafe::swapValues(head, m.head);
                    Unsafe::swapValues(tail, m.tail);
                    Unsafe::swapValues(accessOrder, m.accessOrder);
                }
                return *this;
            }

            /**
             * Constructs an empty <b> LinkedHashMap</b> instance with the
             * specified initial capacity, load factor and ordering mode.
             *
             * @param  initialCapacity the initial capacity
             * @param  loadFactor      the load factor
             * @param  accessOrder     the ordering mode - <b> true</b> for
             *         access-order, <b> false</b> for insertion-order
             * @throws IllegalArgumentException if the initial capacity is negative
             *         or the load factor is nonpositive
             */
            LinkedHashMap(gint initialCapacity, gfloat loadFactor, gbool accessOrder) :
                    HashMap<K, V>(initialCapacity, loadFactor), accessOrder(accessOrder) {}


            /**
             * Returns <b> true</b> if this map maps one or more keys to the
             * specified value.
             *
             * @param value value whose presence in this map is to be tested
             * @return <b> true</b> if this map maps one or more keys to the
             *         specified value
             */
            gbool containsValue(const V &value) const override {
                for (ENTRY e = head; e != null; e = e->after) {
                    if (Object::equals(valueOf(e), value)) {
                        return true;
                    }
                }
                return false;
            }

            /**
             * Returns the value to which the specified key is mapped,
             * or <b> null</b> if this map contains no mapping for the key.
             *
             * <p>More formally, if this map contains a mapping from a key
             * <b> k</b> to a value <b> v</b> such that <b> (key==null ? k==null :
             * key.equals(k))</b>, then this method returns <b> v</b>; otherwise
             * it returns <b> null</b>.  (There can be at most one such mapping.)
             *
             * <p>A return value of <b> null</b> does not <i>necessarily</i>
             * indicate that the map contains no mapping for the key; it's also
             * possible that the map explicitly maps the key to <b> null</b>.
             * The <b style="color: orange;"> #containsKey containsKey</b> operation may be used to
             * distinguish these two cases.
             */
            V &get(const K &key) override {
                NODE e = entryOf(key);
                if (e == null) {
                    NoSuchKeyException(key).throws(__trace("core.util.LinkedHashMap"));
                }
                return valueOf(e);
            }

            /**
             * Returns the value to which the specified key is mapped,
             * or <b> null</b> if this map contains no mapping for the key.
             *
             * <p>More formally, if this map contains a mapping from a key
             * <b> k</b> to a value <b> v</b> such that <b> (key==null ? k==null :
             * key.equals(k))</b>, then this method returns <b> v</b>; otherwise
             * it returns <b> null</b>.  (There can be at most one such mapping.)
             *
             * <p>A return value of <b> null</b> does not <i>necessarily</i>
             * indicate that the map contains no mapping for the key; it's also
             * possible that the map explicitly maps the key to <b> null</b>.
             * The <b style="color: orange;"> #containsKey containsKey</b> operation may be used to
             * distinguish these two cases.
             */
            const V &get(const K &key) const override {
                NODE e = entryOf(key);
                if (e == null) {
                    NoSuchKeyException(key).throws(__trace("core.util.LinkedHashMap"));
                }
                if (accessOrder)
                    afterNodeAccess(e);
                return valueOf(e);
            }

            /**
             *
             */
            V &getOrDefault(const K &key, const V &defaultValue) override {
                NODE e = entryOf(key);
                if (e == null) {
                    return Unsafe::copyInstance(defaultValue, true);
                }
                if (accessOrder)
                    afterNodeAccess(e);
                return valueOf(e);
            }

            /**
             *
             */
            const V &getOrDefault(const K &key, const V &defaultValue) const override {
                NODE e = entryOf(key);
                if (e == null) {
                    return Unsafe::copyInstance(defaultValue, true);
                }
                if (accessOrder)
                    afterNodeAccess(e);
                return valueOf(e);
            }

            /**
             *
             */
            void clear() override {
                HashMap<K, V>::clear();
                head = tail = null;
            }

        protected:
            using typename HashMap<K, V>::MapEntry;


            /**
             * Returns <b> true</b> if this map should remove its eldest entry.
             * This method is invoked by <b> put</b> and <b> putAll</b> after
             * inserting a new entry into the map.  It provides the implementor
             * with the opportunity to remove the eldest entry each time a new one
             * is added.  This is useful if the map represents a cache: it allows
             * the map to reduce memory consumption by deleting stale entries.
             *
             * <p>Sample use: this override will allow the map to grow up to 100
             * entries and then delete the eldest entry each time a new entry is
             * added, maintaining a steady state of 100 entries.
             * <pre> @code
             * private:
             *      static CORE_FAST int MAX_ENTRIES = 100;
             *
             * protected:
             *      gbool removeEldestEntry(Map.Entry eldest) {
             *          return size() &gt; MAX_ENTRIES;
             *      }
             * @encode </pre>
             *
             * <p>This method typically does not modify the map in any way,
             * instead allowing the map to modify itself as directed by its
             * return value.  It <i>is</i> permitted for this method to modify
             * the map directly, but if it does so, it <i>must</i> return
             * <b> false</b> (indicating that the map should not attempt any
             * further modification).  The effects of returning <b> true</b>
             * after modifying the map from within this method are unspecified.
             *
             * <p>This implementation merely returns <b> false</b> (so that this
             * map acts like a normal map - the eldest element is never removed).
             *
             * @param    eldest The least recently inserted entry in the map, or if
             *           this is an access-ordered map, the least recently accessed
             *           entry.  This is the entry that will be removed it this
             *           method returns <b> true</b>.  If the map was empty prior
             *           to the <b> put</b> or <b> putAll</b> invocation resulting
             *           in this invocation, this will be the entry that was just
             *           inserted; in other words, if the map contains a single
             *           entry, the eldest entry is also the newest.
             * @return   <b> true</b> if the eldest entry should be removed
             *           from the map; <b> false</b> if it should be retained.
             */
            virtual gbool removeEldestEntry(ENTRY eldest) {
                CORE_IGNORE(eldest);
                return false;
            }

        private:

            using typename HashMap<K, V>::KEYSET;
            using typename HashMap<K, V>::VALUES;
            using typename HashMap<K, V>::ENTRIES;

            using HashMap<K, V>::kSet;
            using HashMap<K, V>::vCollection;
            using HashMap<K, V>::eSet;

        public:

            /**
             * Returns a <b style="color: orange;"> Set</b> view of the keys contained in this map.
             * The setValue is backed by the map, so changes to the map are
             * reflected in the setValue, and vice-versa.  If the map is modified
             * while an iteration over the setValue is in progress (except through
             * the iterator's own <b> remove</b> operation), the results of
             * the iteration are undefined.  The setValue supports element removal,
             * which removes the corresponding mapping from the map, via the
             * <b> Iterator.remove</b>, <b> Set.remove</b>,
             * <b> removeAll</b>, <b> retainAll</b>, and <b> clear</b>
             * operations.  It does not support the <b> add</b> or <b> addAll</b>
             * operations.
             *
             * @return a setValue view of the keys contained in this map
             */
            Set<K> &keySet() const override {
                KEYSET ks = kSet;
                if (ks == null) {
                    ks = Unsafe::allocateInstance<KeySet>((LinkedHashMap &) *this);
                    (KEYSET &) kSet = ks;
                }
                return *ks;
            }

        private:
            class KeySet CORE_FINAL : public Set<K> {
            private:
                LinkedHashMap &This;

            public:
                CORE_EXPLICIT KeySet(LinkedHashMap &root) : This(root) {}

                gint size() const override {
                    return This.len;
                }

                void clear() override {
                    return This.clear();
                }

                Iterator<const K> &iterator() const override {
                    return Unsafe::allocateInstance<KeyItr<const K>>((LinkedHashMap &) *this);
                }

                Iterator<K> &iterator() override {
                    return Unsafe::allocateInstance<KeyItr<>>((LinkedHashMap &) *this);
                }

                gbool contains(const K &o) const override {
                    return This.containsKey(o);
                }

                gbool remove(const K &o) override {
                    return This.deleteEntry(hash(o), o, null, false, true) != null;
                }

                Array<K> toArray() const override {
                    Array<K> a = Array<K>(This.len);
                    gint i = 0;
                    for (ENTRY e = This.head; e != null; e = e->after) {
                        a.set(++i, keyOf(e));
                    }
                    return a;
                }

                void forEach(const Consumer<K> &action) const override {
                    gint mn = This.modNum;
                    for (ENTRY e = This.head; e != null; e = e->after) {
                        action.accept(keyOf(e));
                        if (mn != This.modNum) {
                            ConcurrentException().throws(__trace("core.util.LinkedHashMap.KeySet"));
                        }
                    }
                }

                void forEach(const Consumer<K &> &action) override {
                    gint mn = This.modNum;
                    for (ENTRY e = This.head; e != null; e = e->after) {
                        action.accept(keyOf(e));
                    }
                    if (mn != This.modNum) {
                        ConcurrentException().throws(__trace("core.util.LinkedHashMap.KeySet"));
                    }
                }

                // Map views depending from map,
                // The true cloning is not possible directly via the map view.
                Object &clone() const override {
                    return This.keySet();
                }
            };

        public:

            /**
             * Returns a <b style="color: orange;"> Collection</b> view of the values contained in this map.
             * The collection is backed by the map, so changes to the map are
             * reflected in the collection, and vice-versa.  If the map is
             * modified while an iteration over the collection is in progress
             * (except through the iterator's own <b> remove</b> operation),
             * the results of the iteration are undefined.  The collection
             * supports element removal, which removes the corresponding
             * mapping from the map, via the <b> Iterator.remove</b>,
             * <b> Collection.remove</b>, <b> removeAll</b>,
             * <b> retainAll</b> and <b> clear</b> operations.  It does not
             * support the <b> add</b> or <b> addAll</b> operations.
             *
             * @return a view of the values contained in this map
             */
            Collection<V> &values() const override {
                VALUES vs = vCollection;
                if (vs == null) {
                    vs = Unsafe::allocateInstance<Values>((LinkedHashMap &) *this);
                    (VALUES &) vCollection = vs;
                }
                return *vs;
            }

        private:
            class Values CORE_FINAL : public Collection<V> {
            private:
                LinkedHashMap &This;

            public:
                CORE_EXPLICIT Values(LinkedHashMap &root) : This(root) {}

                gint size() const override {
                    return This.len;
                }

                void clear() override {
                    This.clear();
                }

                Iterator<const V> &iterator() const override {
                    return Unsafe::allocateInstance<ValueItr<const V>>((LinkedHashMap &) *this);
                }

                Iterator<V> &iterator() override {
                    return Unsafe::allocateInstance<ValueItr<>>((LinkedHashMap &) *this);
                }

                gbool contains(const V &o) const override {
                    return This.containsValue(o);
                }

                Array<V> toArray() const override {
                    Array<V> a = Array<V>(This.len);
                    gint i = 0;
                    for (ENTRY e = This.head; e != null; e = e->after) {
                        a.set(++i, valueOf(e));
                    }
                    return a;
                }

                void forEach(const Consumer<V> &action) const override {
                    gint mn = This.modNum;
                    for (ENTRY e = This.head; e != null; e = e->after) {
                        action.accept(valueOf(e));
                        if (mn != This.modNum) {
                            ConcurrentException().throws(__trace("core.util.LinkedHashMap.KeySet"));
                        }
                    }
                }

                void forEach(const Consumer<V &> &action) override {
                    gint mn = This.modNum;
                    for (ENTRY e = This.head; e != null; e = e->after) {
                        action.accept(valueOf(e));
                    }
                    if (mn != This.modNum) {
                        ConcurrentException().throws(__trace("core.util.LinkedHashMap.KeySet"));
                    }
                }

                Object &clone() const override {
                    return This.values();
                }
            };

        public:

            /**
             * Returns a <b style="color: orange;"> Set</b> view of the mappings contained in this map.
             * The setValue is backed by the map, so changes to the map are
             * reflected in the setValue, and vice-versa.  If the map is modified
             * while an iteration over the setValue is in progress (except through
             * the iterator's own <b> remove</b> operation, or through the
             * <b> setValue</b> operation on a map entry returned by the
             * iterator) the results of the iteration are undefined.  The setValue
             * supports element removal, which removes the corresponding
             * mapping from the map, via the <b> Iterator.remove</b>,
             * <b> Set.remove</b>, <b> removeAll</b>, <b> retainAll</b> and
             * <b> clear</b> operations.  It does not support the
             * <b> add</b> or <b> addAll</b> operations.
             *
             * @return a setValue view of the mappings contained in this map
             */
            Set<MapEntry> &entrySet() const override {
                ENTRIES es = eSet;
                if (es == null) {
                    es = Unsafe::allocateInstance<EntrySet>((LinkedHashMap &) *this);
                    (ENTRIES &) eSet = es;
                }
                return *es;
            }

        private:
            class EntrySet CORE_FINAL : public Set<MapEntry> {
            private:
                LinkedHashMap &This;

            public:
                CORE_EXPLICIT EntrySet(LinkedHashMap &root) : This(root) {}

                gint size() const override {
                    return This.len;
                }

                void clear() override {
                    This.clear();
                }

                Iterator<const MapEntry> &iterator() const override {
                    return Unsafe::allocateInstance<EntryItr<const MapEntry>>((LinkedHashMap &) *this);
                }

                Iterator<MapEntry> &iterator() override {
                    return Unsafe::allocateInstance<EntryItr<>>((LinkedHashMap &) *this);
                }

                gbool contains(const MapEntry &o) const override {
                    NODE node = This.entryOf(o.key());
                    return node != null && node->equals(o);
                }

                gbool remove(const MapEntry &o) override {
                    K const &key = o.key();
                    V const &value = o.value();
                    return This.deleteEntry(hash(key), key, value, true, true) != null;
                }

                void forEach(const Consumer<MapEntry> &action) const override {
                    gint mn = This.modNum;
                    for (ENTRY e = This.head; e != null; e = e->after) {
                        action.accept(*e);
                    }
                    if (mn != This.modNum) {
                        ConcurrentException().throws(__trace("core.util.LinkedHashMap.KeySet"));
                    }
                }

                void forEach(const Consumer<MapEntry &> &action) override {
                    gint mn = This.modNum;
                    for (ENTRY e = This.head; e != null; e = e->after) {
                        action.accept(*e);
                    }
                    if (mn != This.modNum) {
                        ConcurrentException().throws(__trace("core.util.LinkedHashMap.KeySet"));
                    }
                }

                Object &clone() const override {
                    return This.entrySet();
                }
            };

        public:
            /**
             * @see HashMap::forEach
             */
            void forEach(const BiConsumer<K, V> &action) const override {
                gint mn = modNum;
                for (ENTRY e = head; e != null; e = e->after) {
                    action.accept(keyOf(e), valueOf(e));
                }
                if (mn != modNum) {
                    ConcurrentException().throws(__trace("core.util.LinkedHashMap"));
                }
            }

            /**
             * @see HashMap::forEach
             */
            void forEach(const BiConsumer<K, V &> &action) override {
                gint mn = modNum;
                for (ENTRY e = head; e != null; e = e->after) {
                    action.accept(keyOf(e), valueOf(e));
                }
                if (mn != modNum) {
                    ConcurrentException().throws(__trace("core.util.LinkedHashMap"));
                }
            }

            void replaceAll(const BiFunction<K, V, V> &function) override {
                gint mn = modNum;
                for (ENTRY e = head; e != null; e = e->after) {
                    e.v = &Unsafe::copyInstance(function.apply(keyOf(e), valueOf(e)));
                }
                if (mn != modNum) {
                    ConcurrentException().throws(__trace("core.util.LinkedHashMap"));
                }
            }

        private:
            template<class T>
            class AbstractItr : public Iterator<T> {
            private:
                LinkedHashMap &This;
                ENTRY next;
                ENTRY last;
                gint modNum;

            public:
                CORE_EXPLICIT AbstractItr(LinkedHashMap &root) :
                        This(root), next(null), last(null), modNum(root.modNum) {}

                gbool hasNext() const override {
                    return next != null;
                }

                virtual ENTRY nextNode() {
                    ENTRY e = next;
                    if (This.modNum != modNum)
                        ConcurrentException().throws(__trace("core.util.LinkedHashMap.AbstractItr"));
                    if (e == null)
                        NoSuchKeyException().throws(__trace("core.util.LinkedHashMap.AbstractItr"));
                    last = e;
                    next = e->after;
                    return e;
                }

                void remove() override {
                    ENTRY p = last;
                    if (p == null)
                        IllegalStateException().throws(__trace("core.util.LinkedHashMap.AbstractItr"));
                    if (This.modNum != modNum)
                        ConcurrentException().throws(__trace("core.util.LinkedHashMap.AbstractItr"));
                    last = null;
                    This.deleteEntry(p.hash, keyOf(p), valueOf(p), true, true);
                    modNum = This.modNum;;
                }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<AbstractItr>::hasInstance(o))
                        return false;
                    AbstractItr const &it = (AbstractItr const &) o;
                    return &it.This == &This && it.next == next;
                }
            };

            template<class T = K>
            class KeyItr CORE_FINAL : public AbstractItr<T> {
            public:
                CORE_EXPLICIT KeyItr(LinkedHashMap &root) : AbstractItr<T>(root) {}

                using AbstractItr<T>::nextNode;

                T &next() override {
                    return keyOf(nextNode());
                }

                Object &clone() const override {
                    return Unsafe::allocateInstance<KeyItr>(*this);
                }
            };

            template<class T = V>
            class ValueItr CORE_FINAL : public AbstractItr<V> {
            public:
                CORE_EXPLICIT ValueItr(LinkedHashMap &root) : AbstractItr<T>(root) {}

                using AbstractItr<T>::nextNode;

                T &next() override {
                    return valueOf(nextNode());
                }

                Object &clone() const override {
                    return Unsafe::allocateInstance<ValueItr>(*this);
                }
            };

            template<class T = MapEntry>
            class EntryItr CORE_FINAL : public AbstractItr<MapEntry> {
            public:
                CORE_EXPLICIT EntryItr(LinkedHashMap &root) : AbstractItr<T>(root) {}

                using AbstractItr<T>::nextNode;

                T &next() override {
                    return *(nextNode());
                }

                Object &clone() const override {
                    return Unsafe::allocateInstance<EntryItr>(*this);
                }
            };

        public:
            Object &clone() const override {
                LinkedHashMap &clone = Unsafe::allocateInstance<LinkedHashMap>();
                try {
                    clone.addEntries(*this, false);
                } catch (const Exception &ex) {
                    // adding value fail
                    Unsafe::destroyInstance(clone);
                    CloneNotSupportedException().throws(__trace("core.util.LinkedHashMap"));
                } catch (const Throwable &th) {
                    th.throws(__trace("core.util.LinkedHashMap"));
                }
            }
        };

    } // util
} // core

#endif //CORE23_LINKEDHASHMAP_H
