//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_TREEMAP_H
#define CORE23_TREEMAP_H

#include <core/util/Map.h>

namespace core {
    namespace util {

        /**
         * A Red-Black tree based <b style="color:orange;">Map</b> implementation.
         * The map is sorted according to the <b style="color:green;">natural
         * ordering</b> of its keys, or by a <b style="color:orange;">Comparator</b> provided at map
         * creation time, depending on which constructor is used.
         *
         * <p>This implementation provides guaranteed log(n) time cost for the
         * <b>containsKey</b>, <b>get</b>, <b>put</b> and <b>remove</b>
         * operations.  Algorithms are adaptations of those in Cormen, Leiserson, and
         * Rivest's <em>Introduction to Algorithms</em>.
         *
         * <p>Note that the ordering maintained by a tree map, like any sorted map, and
         * whether or not an explicit comparator is provided, must be <em>consistent
         * with <b>equals</b></em> if this sorted map is to correctly implement the
         * <b>Map</b> interface.  (See <b>Comparable</b> or <b>Comparator</b> for a
         * precise definition of <em>consistent with equals</em>.)  This is so because
         * the <b>Map</b> interface is defined in terms of the <b>equals</b>
         * operation, but a sorted map performs all key comparisons using its <b>
         * compareTo</b> (or <b>compare</b>) method, so two keys that are deemed equal by
         * this method are, from the standpoint of the sorted map, equal.  The behavior
         * of a sorted map <em>is</em> well-defined even if its ordering is
         * inconsistent with <b>equals</b>; it just fails to obey the general contract
         * of the <b>Map</b> interface.
         *
         * <p><strong>Note that this implementation is not synchronized.</strong>
         * If multiple threads access a map concurrently, and at least one of the
         * threads modifies the map structurally, it <em>must</em> be synchronized
         * externally.  (A structural modification is any operation that adds or
         * deletes one or more mappings; merely changing the value associated
         * with an existing key is not a structural modification.)  This is
         * typically accomplished by synchronizing on some object that naturally
         * encapsulates the map.
         * If no such object exists, the map should be "wrapped" using the
         * <b style="color:orange;">core.util.SynchronizedMap</b>
         * class.
         *
         * <p>The iterators returned by the <b>iterator</b> method of the collections
         * returned by all of this class's "collection view methods" are
         * <em>fail-fast</em>: if the map is structurally modified at any time after
         * the iterator is created, in any way except through the iterator is own
         * <b>remove</b> method, the iterator will throw a <b style="color:orange;">
         * ConcurrentException</b>.  Thus, in the face of concurrent
         * modification, the iterator fails quickly and cleanly, rather than risking
         * arbitrary, non-deterministic behavior at an undetermined time in the future.
         *
         * <p>Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw <b>ConcurrentException</b> on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness:   <em>the fail-fast behavior of iterators
         * should be used only to detect bugs.</em>
         *
         * <p>All <b>Map.Entry</b> pairs returned by methods in this class
         * and its views represent snapshots of mappings at the time they were
         * produced. They do <strong>not</strong> support the <b>Entry.setValue</b>
         * method. (Note however that it is possible to change mappings in the
         * associated map using <b>put</b>.)
         *
         * @param K the type of keys maintained by this map
         * @param V the type of mapped values
         *
         * @author  Brunshweeck Tazeussong
         * @see Map
         * @see HashMap
         * @see Hashtable
         * @see Comparable
         * @see Comparator
         * @see Collection
         */
        template<class K, class V>
        class TreeMap : public Map<K, V>, public SortedStruct<K> {
        protected:
            CORE_ALIAS(Unsafe, native::Unsafe);

            template<class E>
            CORE_ALIAS(ActionConsumer, function::Consumer<E>);

            template<class T, class U>
            CORE_ALIAS(BinaryActionConsumer,, function::BiConsumer<T, U>);

            template<class T, class U>
            CORE_ALIAS(BinaryFunction,, function::BiFunction<T, U, V>);

        private:

            class Entry;

            CORE_ALIAS(KeyComparator, Comparator<K>);
            CORE_ALIAS(COMPARATOR, typename Class<KeyComparator>::Ptr);
            CORE_ALIAS(ENTRY, typename Class<Entry>::Ptr);
            CORE_ALIAS(MapEntry, , typename Map<K, V>::Entry);
            CORE_ALIAS(KEY, typename Class<K>::Ptr);
            CORE_ALIAS(VALUE, typename Class<V>::Ptr);

            /**
             * The comparator used to maintain order in this tree map, or
             * null if it uses the natural ordering of its keys.
             *
             */
            COMPARATOR cmp = {};

            /**
             * The root binary tree
             */
            ENTRY root = {};

            /**
             * The number of entries in the tree
             */
            gint len = {};

            /**
             * The number of structural modifications to the tree.
             */
            gint modNum = {};

        public:

            /**
             * Constructs a new, empty tree map, using the natural ordering of its
             * keys.  All keys inserted into the map must implement the <b style="color:orange;">
             * Comparable</b> interface.  Furthermore, all such keys must be
             * <em>mutually comparable</em>: <b> k1.compareTo(k2)</b> must not throw
             * a <b> ClassCastException</b> for any keys <b> k1</b> and
             * <b> k2</b> in the map.  If the user attempts to put a key into the
             * map that violates this constraint (for example, the user attempts to
             * put a string key into a map whose keys are integers), the
             * <b> put(Object key, Object value)</b> call will throw a
             * <b> ClassCastException</b>.
             */
            CORE_IMPLICIT TreeMap() : cmp(&Comparator<K>::naturalOrder()) {}

            /**
             * Constructs a new, empty tree map, ordered according to the given
             * comparator.  All keys inserted into the map must be <em>mutually
             * comparable</em> by the given comparator: <b> comparator.compare(k1,
             * k2)</b> must not throw a <b> ClassCastException</b> for any keys
             * <b> k1</b> and <b> k2</b> in the map.  If the user attempts to put
             * a key into the map that violates this constraint, the <b> put(Object
             * key, Object value)</b> call will throw a
             * <b> ClassCastException</b>.
             *
             * @param comparator the comparator that will be used to order this map.
             */
            CORE_EXPLICIT TreeMap(const KeyComparator &comparator) : cmp(&Unsafe::copyInstance(comparator, true)) {}

            /**
             * Constructs a new tree map containing the same mappings as the given
             * map, ordered according to the <em>natural ordering</em> of its keys.
             * All keys inserted into the new map must implement the <b style="color:orange;">
             * Comparable</b> interface.  Furthermore, all such keys must be
             * <em>mutually comparable</em>: <b> k1.compareTo(k2)</b> must not throw
             * a <b> ClassCastException</b> for any keys <b> k1</b> and
             * <b> k2</b> in the map.  This method runs in n*log(n) time.
             *
             * @param  m the map whose mappings are to be placed in this map
             * @throws ClassCastException if the keys in m are not <b style="color:orange;"> Comparable</b>,
             *         or are not mutually comparable
             */
            CORE_EXPLICIT TreeMap(const Map<K, V> &m) {
                if (!Class<SortedStruct<K>>::hasInstance(m)) {
                    KeyComparator &comparator = KeyComparator::naturalOrder();
                    cmp = &comparator;
                    putAll(m);
                } else {
                    KeyComparator &comparator = CORE_DYN_CAST(SortedStruct<K> &, m).comparator();
                    cmp = &comparator;
                    buildFromSorted(m.size(), m.entrySet().iterator(), null);
                }
            }

            /**
             * Constructs a new tree map containing the same mappings and
             * using the same ordering as the specified sorted map.  This
             * method runs in linear time.
             *
             * @param  m the sorted map whose mappings are to be placed in this map,
             *         and whose comparator is to be used to sort this map
             */
            TreeMap(const TreeMap<K, V> &m) : cmp((COMPARATOR) m.cmp) {
                buildFromSorted(m.size(), m.entrySet().iterator(), null);
            }

            /**
             * Constructs a new tree map containing the same mappings and
             * using the same ordering as the specified sorted map.  This
             * method runs in linear time.
             *
             * @param  m the sorted map whose mappings are to be placed in this map,
             *         and whose comparator is to be used to sort this map
             */
            TreeMap(TreeMap<K, V> &&m) CORE_NOTHROW {
                Unsafe::swapValues(root, m.root);
                Unsafe::swapValues(len, m.len);
                Unsafe::swapValues(cmp, m.cmp);
            }

            TreeMap<K, V> &operator=(const TreeMap<K, V> &m) {
                if (this != &m) {
                    clear();
                    cmp = m.cmp;
                    buildFromSorted(m.size(), m.entrySet().iterator(), null);
                }
                return *this;
            }

            TreeMap<K, V> &operator=(TreeMap<K, V> &&m) CORE_NOTHROW {
                if (this != &m) {
                    Unsafe::swapValues(root, m.root);
                    Unsafe::swapValues(len, m.len);
                    Unsafe::swapValues(cmp, m.cmp);
                    modNum += 1;
                    m.modNum += 1;
                }
                return *this;
            }

            /**
             * Returns the number of key-value mappings in this map.
             *
             * @return the number of key-value mappings in this map
             */
            gint size() const override { return len; }

            /**
             * Returns <b> true</b> if this map contains a mapping for the specified
             * key.
             *
             * @param key key whose presence in this map is to be tested
             * @return <b> true</b> if this map contains a mapping for the
             *         specified key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             */
            gbool containsKey(const K &key) const override { return entryOf(key) != null; }

            /**
             * Returns <b> true</b> if this map maps one or more keys to the
             * specified value.  More formally, returns <b> true</b> if and only if
             * this map contains at least one mapping to a value <b> v</b> such
             * that <b> (value==null ? v==null : value.equals(v))</b>.  This
             * operation will probably require time linear in the map size for
             * most implementations.
             *
             * @param value value whose presence in this map is to be tested
             * @return <b> true</b> if a mapping to <b> value</b> exists;
             *         <b> false</b> otherwise
             */
            gbool containsValue(const V &value) const override {
                for (ENTRY e = firstEntry(); e != null; e = successor(e))
                    if (Object::equals(value, e->value()))
                        return true;
                return false;
            }

            /**
             * Returns the value to which the specified key is mapped,
             * or throws <b> NoSuchKeyException </b> if this map contains no mapping for the key.
             *
             * <p>More formally, if this map contains a mapping from a key
             * <b> k</b> to a value <b> v</b> such that <b> key</b> compares
             * equal to <b> k</b> according to the map's ordering, then this
             * method returns <b> v</b>; otherwise it  throws <b> NoSuchKeyException </b>.
             * (There can be at most one such mapping.)
             *
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             *
             * @throws KeyNotFoundException
             */
            V &get(const K &key) override {
                ENTRY e;
                if ((e = entryOf(key)) == null)
                    NoSuchKeyException($(key)).throws(__trace("core.util.TreeMap"));
                return e->value();
            }

            /**
             * Returns the value to which the specified key is mapped,
             * or throws <b> NoSuchKeyException </b> if this map contains no mapping for the key.
             *
             * <p>More formally, if this map contains a mapping from a key
             * <b> k</b> to a value <b> v</b> such that <b> key</b> compares
             * equal to <b> k</b> according to the map's ordering, then this
             * method returns <b> v</b>; otherwise it  throws <b> NoSuchKeyException </b>.
             * (There can be at most one such mapping.)
             *
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             *
             * @throws KeyNotFoundException
             */
            const V &get(const K &key) const override {
                ENTRY e;
                if ((e = entryOf(key)) == null)
                    NoSuchKeyException($(key)).throws(__trace("core.util.TreeMap"));
                return e->value();
            }

            /**
             *
             */
            V &getOrDefault(const K &key, const V &defaultValue) override {
                ENTRY e;
                if ((e = entryOf(key)) == null)
                    return Unsafe::copyInstance(defaultValue, true);
                return e->value();
            }

            /**
             *
             */
            const V &getOrDefault(const K &key, const V &defaultValue) const override {
                ENTRY e;
                if ((e = entryOf(key)) == null)
                    return Unsafe::copyInstance(defaultValue, true);
                return e->value();
            }

            /**
             * Returns the comparator used to order the keys in this map.
             *
             * @return Returns the comparator used to order the keys in this map.
             */
            const KeyComparator &comparator() const override { return *cmp; }

            /**
             * Copies all of the mappings from the specified map to this map.
             * These mappings replace any mappings that this map had for any
             * of the keys currently in the specified map.
             *
             * @param  map mappings to be stored in this map
             * @throws ClassCastException if the class of a key or value in
             *         the specified map prevents it from being stored in this map
             */
            void putAll(const Map<K, V> &m) override {
                gint mSize = m.size();
                if (len == 0 && mSize != 0 && Class<SortedStruct<K>>::hasInstance(m)) {
                    const KeyComparator &comparator2 = CORE_DYN_CAST(const SortedStruct<K>&, m).comparator();
                    if (Object::equals(comparator(), comparator2)) {
                        modNum += 1;
                        buildFromSorted(mSize, m.entrySet().iterator(), null);
                    }
                    return;
                }
                Map<K, V>::putAll(m);
            }

        private:

            /**
             * Returns this map's entry for the given key, or <b> null</b> if the map
             * does not contain an entry for the key.
             *
             * @return this map's entry for the given key, or <b> null</b> if the map
             *         does not contain an entry for the key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             */
            ENTRY entryOf(const K &key) const {
                // Offload comparator-based version for the sake of performance
                const KeyComparator &cpr = comparator();
                ENTRY p = root;
                while (p != null) {
                    gint ret = cpr.compare(key, p->key());
                    if (ret < 0)
                        p = p->left;
                    else if (ret > 0)
                        p = p->right;
                    else return p;
                }
                return null;
            }

        public:

            /**
             * Associates the specified value with the specified key in this map.
             * If the map previously contained a mapping for the key, the old
             * value is replaced.
             *
             * @param key key with which the specified value is to be associated
             * @param value value to be associated with the specified key
             *
             * @return the previous value associated with <b> key</b>, or
             *         <b> the new value</b> if there was no mapping for <b> key</b>.
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             */
            const V &put(const K &key, const V &value) override { return addEntry(key, value, true); }

            /**
             * @override
             */
            const V &putIfAbsent(const K &key, const V &value) override { return addEntry(key, value, false); }

        private:
            void addEntry(K &key, V &value, ENTRY parent, gbool addToLeft) {
                ENTRY e = &Unsafe::allocateInstance<Entry>(key, value, parent);
                (addToLeft ? parent->left : parent->right) = e;
                len += 1;
                modNum += 1;
            }

            const V &addEntry(const K &key, const V &value, gbool replaceOld) {
                ENTRY t = root;
                if (t == null) {
                    // add to an empty map
                    K &k = Unsafe::copyInstance(key, true);
                    V &v = Unsafe::copyInstance(value, true);
                    root = &Unsafe::allocateInstance<Entry>(k, v, null);
                    len = 1;
                    modNum += 1;
                    return root->value();
                }
                ENTRY parent = null;
                gint ret = {};
                const KeyComparator &cpr = comparator();
                do {
                    parent = t;
                    ret = cpr.compare(key, t->key());
                    if (ret < 0)
                        t = t->left;
                    else if (ret > 0)
                        t = t->right;
                    else {
                        // is not absent
                        return replaceOld ? (t->set(value)) : (t->value());
                    }
                } while (t != null);
                K &k = Unsafe::copyInstance(key, true);
                V &v = Unsafe::copyInstance(value, true);
                addEntry(k, v, parent, ret < 0);
                return v;
            }

        public:

            /**
             * Removes the mapping for this key from this TreeMap if present.
             *
             * @param  key key for which mapping should be removed
             * @return the previous value associated with <b> key</b>, or
             *         throws <b> NoSuchKeyException </b> if there was no mapping for <b> key</b>.
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             *
             * @throws KeyNotFoundException
             */
            const V &remove(const K &key) override {
                ENTRY p = entryOf(key);
                if (p == null)
                    NoSuchKeyException($(key)).throws(__trace("core.util.TreeMap"));
                const V &oldValue = p->value();
                deleteEntry(p);
                return oldValue;
            }

            /**
             * @override
             */
            gbool remove(const K &key, const V &value) override {
                ENTRY p = entryOf(key);
                if (p == null)
                    return false;
                if (!Object::equals(value, p->value()))
                    return false;
                deleteEntry(p);
                return true;
            }

            /**
             * Removes all of the mappings from this map.
             * The map will be empty after this call returns.
             */
            void clear() override {
                modNum += 1;
                len = 0;
                root = null;
            }

            /**
             * Returns a shallow copy of this <b> TreeMap</b> INSTANCE. (The keys and
             * values themselves are not cloned.)
             *
             * @return a shallow copy of this map
             */
            Object &clone() const override {
                TreeMap<K, V> &copy = Unsafe::allocateInstance<TreeMap>();
                CORE_TRY_RETHROW_EXCEPTION(
                        { copy.putAll(*this); },
                        { Unsafe::destroyInstance(copy); },
                        __trace("core.util.TreeMap"));
                return copy;
            }

        private:

            CORE_ALIAS(ENTRIES, , typename Class<Set<MapEntry>>::Ptr);
            CORE_ALIAS(VALUES, typename Class<Collection<V>>::Ptr);
            CORE_ALIAS(KEYSET, typename Class<Set<K>>::Ptr);

            /**
             * Fields initialized to contain an INSTANCE of the entry set view
             * the first time this view is requested.  Views are stateless, so
             * there's no reason to create more than one.
             */
            ENTRIES eSet = {};

            ENTRIES reSet = {};

            KEYSET rkSet = {};

        public:

            /**
             * Returns a <b style="color:orange;"> Set</b> view of the keys contained in this map.
             *
             * <p>The set's iterator returns the keys in ascending order.
             *
             * <p>The set is backed by the map, so changes to the map are
             * reflected in the set, and vice-versa.  If the map is modified
             * while an iteration over the set is in progress (except through
             * the iterator's own <b> remove</b> operation), the results of
             * the iteration are undefined.  The set supports element removal,
             * which removes the corresponding mapping from the map, via the
             * <b> Iterator.remove</b>, <b> Set.remove</b>,
             * <b> removeAll</b>, <b> retainAll</b>, and <b> clear</b>
             * operations.  It does not support the <b> add</b> or <b> addAll</b>
             * operations.
             */
            Set<K> &keySet() const override {
                if (Map<K, V>::kSet == null)
                    (KEYSET &) Map<K, V>::kSet = &Unsafe::allocateInstance<KeySet>((TreeMap<K, V> &) *this);
                return (Set<K> &) Map<K, V>::kSet[0];
            }

            /**
             * Returns a reverse order <b style="color:orange;"> Set</b> view of the keys contained in this map.
             * The set's iterator returns the keys in descending order.
             * The set is backed by the map, so changes to the map are reflected in
             * the set, and vice-versa.  If the map is modified while an iteration
             * over the set is in progress (except through the iterator's own <b>
             * remove</b> operation), the results of the iteration are undefined.  The
             * set supports element removal, which removes the corresponding mapping
             * from the map, via the <b> Iterator.remove</b>, <b> Set.remove</b>,
             * <b> removeAll</b>, <b> retainAll</b>, and <b> clear</b> operations.
             * It does not support the <b> add</b> or <b> addAll</b> operations.
             *
             * @return a reverse order navigable set view of the keys in this map
             */
            Set<K> &reversedKeySet() {
                if (rkSet == null)
                    (KEYSET &) rkSet = &Unsafe::allocateInstance<ReverseKeySet>((TreeMap<K, V> &) *this);
                return (Set<K> &) rkSet[0];
            }

            /**
             * Returns a <b style="color:orange;"> Collection</b> view of the values contained in this map.
             *
             * <p>The collection's iterator returns the values in ascending order
             * of the corresponding keys.
             *
             * <p>The collection is backed by the map, so changes to the map are
             * reflected in the collection, and vice-versa.  If the map is
             * modified while an iteration over the collection is in progress
             * (except through the iterator's own <b> remove</b> operation),
             * the results of the iteration are undefined.  The collection
             * supports element removal, which removes the corresponding
             * mapping from the map, via the <b> Iterator.remove</b>,
             * <b> Collection.remove</b>, <b> removeAll</b>,
             * <b> retainAll</b> and <b> clear</b> operations.  It does not
             * support the <b> add</b> or <b> addAll</b> operations.
             */
            Collection<V> &values() const override {
                if (Map<K, V>::vCollection == null)
                    (VALUES &) Map<K, V>::vCollection = &Unsafe::allocateInstance<Values>((TreeMap<K, V> &) *this);
                return (Collection<V> &) Map<K, V>::vCollection[0];
            }

            /**
             * Returns a <b style="color:orange;"> Set</b> view of the mappings contained in this map.
             *
             * <p>The set's iterator returns the entries in ascending key order.
             *
             * <p>The set is backed by the map, so changes to the map are
             * reflected in the set, and vice-versa.  If the map is modified
             * while an iteration over the set is in progress (except through
             * the iterator's own <b> remove</b> operation, or through the
             * <b> setValue</b> operation on a map entry returned by the
             * iterator) the results of the iteration are undefined.  The set
             * supports element removal, which removes the corresponding
             * mapping from the map, via the <b> Iterator.remove</b>,
             * <b> Set.remove</b>, <b> removeAll</b>, <b> retainAll</b> and
             * <b> clear</b> operations.  It does not support the
             * <b> add</b> or <b> addAll</b> operations.
             */
            Set<MapEntry> &entrySet() const override {
                if (eSet == null)
                    (ENTRIES &) eSet = &Unsafe::allocateInstance<EntrySet>((TreeMap<K, V> &) *this);
                return (Set<MapEntry> &) eSet[0];
            }

            /**
             * Returns a reverse order <b style="color:orange;"> Set</b> view of the mappings contained in this map.
             *
             * <p>The set's iterator returns the entries in descending key order.
             *
             * <p>The set is backed by the map, so changes to the map are
             * reflected in the set, and vice-versa.  If the map is modified
             * while an iteration over the set is in progress (except through
             * the iterator's own <b> remove</b> operation, or through the
             * <b> setValue</b> operation on a map entry returned by the
             * iterator) the results of the iteration are undefined.  The set
             * supports element removal, which removes the corresponding
             * mapping from the map, via the <b> Iterator.remove</b>,
             * <b> Set.remove</b>, <b> removeAll</b>, <b> retainAll</b> and
             * <b> clear</b> operations.  It does not support the
             * <b> add</b> or <b> addAll</b> operations.
             */
            Set<MapEntry> &reverseEntrySet() const {
                if (reSet == null)
                    (ENTRIES &) reSet = &Unsafe::allocateInstance<ReverseEntrySet>((TreeMap<K, V> &) *this);
                return (Set<MapEntry> &) reSet[0];
            }

        private:

            // Red-black mechanics
            static CORE_FAST gbool RED = false;
            static CORE_FAST gbool BLACK = true;

            /**
             * Node in the Tree.  Doubles as a means to pass key-value pairs back to
             * user (see Map.Entry).
             */
            class Entry CORE_FINAL : public MapEntry {
            public:
                KEY k = {};
                VALUE v = {};
                ENTRY left = {};
                ENTRY right = {};
                ENTRY parent = {};
                gbool color = BLACK;

                /**
                 * Make a new cell with given key, value, and parent, and with
                 * <b> null</b> child links, and BLACK color.
                 */
                CORE_FAST Entry(K &k, V &v, ENTRY parent) : k(&k), v(&v), parent(parent) {}

                /**
                 * Returns the key.
                 *
                 * @return the key
                 */
                const K &key() const override { return k[0]; }

                /**
                 * Returns the value associated with the key.
                 *
                 * @return the value associated with the key
                 */
                V &value() override { return v[0]; }

                /**
                 * Returns the value associated with the key.
                 *
                 * @return the value associated with the key
                 */
                const V &value() const override { return v[0]; }

                /**
                 * Replaces the value currently associated with the key with the given
                 * value.
                 *
                 * @return the value associated with the key before this method was
                 *         called
                 */
                const V &set(const V &value) override {
                    VALUE oldValue = v;
                    if (oldValue != &value) v = &Unsafe::copyInstance(value, true);
                    return oldValue[0];
                }

                gbool equals(const Object &o) const override {
                    if (this == &o) return true;
                    if (!Class<Map<K, V>>::hasInstance(o)) return false;
                    const MapEntry &e = (MapEntry &) o;
                    return Object::equals(key(), e.key()) && Object::equals(value(), e.value());
                }

                gint hash() const override {
                    return key().hash() ^ value().hash();
                }

                Object &clone() const override {
                    Entry &e = Unsafe::allocateInstance<Entry>(*this);
                    return e;
                }

                String toString() const override {
                    StringBuffer sb = StringBuffer(25);
                    return sb
                            .append("{key=")
                            .append(key())
                            .append(", ")
                            .append("value=")
                            .append(value())
                            .append("}")
                            .toString();
                }

                ~Entry() override {
                    color = BLACK;
                    if (left != null)
                        left->parent = null;
                    if (right != null)
                        right->parent = null;
                    if (parent != null)
                        (parent->left == this ? parent->left : parent->right) = null;
                    left = right = parent = null;
                    k = null;
                    v = null;
                }
            };

            /**
             * Returns the first Entry in the TreeMap (according to the TreeMap's
             * key-sort function).  Returns null if the TreeMap is empty.
             */
            ENTRY firstEntry() const {
                ENTRY p = root;
                if (p != null)
                    while (p->left != null)
                        p = p->left;
                return p;
            }

            /**
             * Returns the last Entry in the TreeMap (according to the TreeMap's
             * key-sort function).  Returns null if the TreeMap is empty.
             */
            ENTRY lastEntry() const {
                ENTRY p = root;
                if (p != null)
                    while (p->right != null)
                        p = p->right;
                return p;
            }

            /**
             * Returns the successor of the specified Entry, or null if no such.
             */
            static ENTRY successor(ENTRY t) {
                if (t == null)
                    return null;
                else if (t->right != null) {
                    ENTRY p = t->right;
                    while (p->left != null)
                        p = p->left;
                    return p;
                } else {
                    ENTRY p = t->parent;
                    ENTRY ch = t;
                    while (p != null && ch == p->right) {
                        ch = p;
                        p = p->parent;
                    }
                    return p;
                }
            }

            /**
             * Returns the predecessor of the specified Entry, or null if no such.
             */
            static ENTRY predecessor(ENTRY t) {
                if (t == null)
                    return null;
                else if (t->left != null) {
                    ENTRY p = t->left;
                    while (p->right != null)
                        p = p->right;
                    return p;
                } else {
                    ENTRY p = t->parent;
                    ENTRY ch = t;
                    while (p != null && ch == p->left) {
                        ch = p;
                        p = p->parent;
                    }
                    return p;
                }
            }

            /**
             * Balancing operations.
             *
             * Implementations of rebalancings during insertion and deletion are
             * slightly different than the CLR version.  Rather than using dummy
             * nilnodes, we use a set of accessors that deal properly with null.  They
             * are used to avoid messiness surrounding nullness checks in the main
             * algorithms.
             */

            static bool colorOf(ENTRY p) { return (p == null ? BLACK : p->color); }

            static ENTRY parentOf(ENTRY p) { return (p == null ? null : p->parent); }

            static void setColor(ENTRY p, gbool c) { if (p != null) p->color = c; }

            static ENTRY leftOf(ENTRY p) { return (p == null) ? null : p->left; }

            static ENTRY rightOf(ENTRY p) { return (p == null) ? null : p->right; }

            /** From CLR */
            void rotateLeft(ENTRY p) {
                if (p != null) {
                    ENTRY r = p->right;
                    p->right = r->left;
                    if (r->left != null)
                        r->left->parent = p;
                    r->parent = p->parent;
                    if (p->parent == null)
                        root = r;
                    else if (p->parent->left == p)
                        p->parent->left = r;
                    else
                        p->parent->right = r;
                    r->left = p;
                    p->parent = r;
                }
            }

            /** From CLR */
            void rotateRight(ENTRY p) {
                if (p != null) {
                    ENTRY l = p->left;
                    p->left = l->right;
                    if (l->right != null) l->right->parent = p;
                    l->parent = p->parent;
                    if (p->parent == null)
                        root = l;
                    else if (p->parent->right == p)
                        p->parent->right = l;
                    else p->parent->left = l;
                    l->right = p;
                    p->parent = l;
                }
            }

            /** From CLR */
            void fixAfterInsertion(ENTRY x) {
                x->color = RED;

                while (x != null && x != root && x->parent->color == RED) {
                    if (parentOf(x) == leftOf(parentOf(parentOf(x)))) {
                        ENTRY y = rightOf(parentOf(parentOf(x)));
                        if (colorOf(y) == RED) {
                            setColor(parentOf(x), BLACK);
                            setColor(y, BLACK);
                            setColor(parentOf(parentOf(x)), RED);
                            x = parentOf(parentOf(x));
                        } else {
                            if (x == rightOf(parentOf(x))) {
                                x = parentOf(x);
                                rotateLeft(x);
                            }
                            setColor(parentOf(x), BLACK);
                            setColor(parentOf(parentOf(x)), RED);
                            rotateRight(parentOf(parentOf(x)));
                        }
                    } else {
                        ENTRY y = leftOf(parentOf(parentOf(x)));
                        if (colorOf(y) == RED) {
                            setColor(parentOf(x), BLACK);
                            setColor(y, BLACK);
                            setColor(parentOf(parentOf(x)), RED);
                            x = parentOf(parentOf(x));
                        } else {
                            if (x == leftOf(parentOf(x))) {
                                x = parentOf(x);
                                rotateRight(x);
                            }
                            setColor(parentOf(x), BLACK);
                            setColor(parentOf(parentOf(x)), RED);
                            rotateLeft(parentOf(parentOf(x)));
                        }
                    }
                }
                root->color = BLACK;
            }

            /**
             * Delete node p, and then rebalance the tree.
             */
            void deleteEntry(ENTRY p) {
                modNum++;
                len--;

                // If strictly internal, copy successor's element to p and then make p
                // point to successor.
                if (p->left != null && p->right != null) {
                    ENTRY s = successor(p);
                    p->k = s->k;
                    p->v = s->v;
                    p = s;
                } // p has 2 children

                // Start fixup at replacement node, if it exists.
                ENTRY replacement = (p->left != null ? p->left : p->right);

                if (replacement != null) {
                    // Link replacement to parent
                    replacement->parent = p->parent;
                    if (p->parent == null)
                        root = replacement;
                    else if (p == p->parent->left)
                        p->parent->left = replacement;
                    else
                        p->parent->right = replacement;

                    // Null out links, so they are OK to use by fixAfterDeletion.
                    p->left = p->right = p->parent = null;

                    // Fix replacement
                    if (p->color == BLACK)
                        fixAfterDeletion(replacement);
                } else if (p->parent == null) { // return if we are the only node.
                    root = null;
                } else { //  No children. Use self as phantom replacement and unlink.
                    if (p->color == BLACK)
                        fixAfterDeletion(p);

                    if (p->parent != null) {
                        if (p == p->parent->left)
                            p->parent->left = null;
                        else if (p == p->parent->right)
                            p->parent->right = null;
                        p->parent = null;
                    }
                }
            }

            /** From CLR */
            void fixAfterDeletion(ENTRY x) {
                while (x != root && colorOf(x) == BLACK) {
                    if (x == leftOf(parentOf(x))) {
                        ENTRY sib = rightOf(parentOf(x));

                        if (colorOf(sib) == RED) {
                            setColor(sib, BLACK);
                            setColor(parentOf(x), RED);
                            rotateLeft(parentOf(x));
                            sib = rightOf(parentOf(x));
                        }

                        if (colorOf(leftOf(sib)) == BLACK &&
                            colorOf(rightOf(sib)) == BLACK) {
                            setColor(sib, RED);
                            x = parentOf(x);
                        } else {
                            if (colorOf(rightOf(sib)) == BLACK) {
                                setColor(leftOf(sib), BLACK);
                                setColor(sib, RED);
                                rotateRight(sib);
                                sib = rightOf(parentOf(x));
                            }
                            setColor(sib, colorOf(parentOf(x)));
                            setColor(parentOf(x), BLACK);
                            setColor(rightOf(sib), BLACK);
                            rotateLeft(parentOf(x));
                            x = root;
                        }
                    } else { // symmetric
                        ENTRY sib = leftOf(parentOf(x));

                        if (colorOf(sib) == RED) {
                            setColor(sib, BLACK);
                            setColor(parentOf(x), RED);
                            rotateRight(parentOf(x));
                            sib = leftOf(parentOf(x));
                        }

                        if (colorOf(rightOf(sib)) == BLACK &&
                            colorOf(leftOf(sib)) == BLACK) {
                            setColor(sib, RED);
                            x = parentOf(x);
                        } else {
                            if (colorOf(leftOf(sib)) == BLACK) {
                                setColor(rightOf(sib), BLACK);
                                setColor(sib, RED);
                                rotateLeft(sib);
                                sib = leftOf(parentOf(x));
                            }
                            setColor(sib, colorOf(parentOf(x)));
                            setColor(parentOf(x), BLACK);
                            setColor(leftOf(sib), BLACK);
                            rotateRight(parentOf(x));
                            x = root;
                        }
                    }
                }

                setColor(x, BLACK);
            }

            /**
             * Linear time tree building algorithm from sorted root.  Can accept keys
             * and/or values from iterator or stream. This leads to too many
             * parameters, but seems better than alternatives.  The four formats
             * that this method accepts are:
             *
             *    1) An iterator of Map.Entries.  (it != null, defaultVal == null).
             *    2) An iterator of keys.         (it != null, defaultVal != null).
             *
             * It is assumed that the comparator of the TreeMap is already set prior
             * to calling this method.
             *
             * @param size the number of keys (or key-value pairs) to be read from
             *        the iterator or stream
             * @param it If non-null, new entries are created from entries
             *        or keys read from this iterator.
             * @param str If non-null, new entries are created from keys and
             *        possibly values read from this stream in serialized form.
             *        Exactly one of it and str should be non-null.
             */
            template<class T>
            void buildFromSorted(gint size, Iterator<T> &it, const Object &defaultVal) {
                len = size;
                root = buildFromSorted(0, 0, size - 1, computeRedLevel(size), it, defaultVal);
            }

            /**
             * Recursive "helper method" that does the real work of the
             * previous method.  Identically named parameters have
             * identical definitions.  Additional parameters are documented below.
             * It is assumed that the comparator and size fields of the TreeMap are
             * already set prior to calling this method.  (It ignores both fields.)
             *
             * @param level the current level of tree. Initial call should be 0.
             * @param lo the first element index of this subtree. Initial should be 0.
             * @param hi the last element index of this subtree.  Initial should be
             *        size-1.
             * @param redLevel the level at which nodes should be red.
             *        Must be equal to computeRedLevel for tree of this size.
             */
            template<class T>
            ENTRY
            buildFromSorted(gint level, gint lo, gint hi, gint redLevel, Iterator<T> &it, const Object &defaultVal) {
                /*
                 * Strategy: The root is the middlemost element. To get to it, we
                 * have to first recursively construct the entire left subtree,
                 * so as to grab all of its elements. We can then proceed with right
                 * subtree.
                 *
                 * The lo and hi arguments are the minimum and maximum
                 * indices to pull out of the iterator or stream for the current subtree.
                 * They are not indexed; we just proceed sequentially,
                 * ensuring that items are extracted in corresponding order.
                 */

                if (hi < lo) return null;

                gint const mid = (lo + hi) >> 1;

                ENTRY left = null;
                if (lo < mid)
                    left = buildFromSorted(level + 1, lo, mid - 1, redLevel, it, defaultVal);

                // extract key and/or value from iterator or stream
                KEY key = {};
                VALUE value = {};
                if (defaultVal == null) {
                    MapEntry &entry = (MapEntry &) it.next();
                    key = (KEY) &entry.key();
                    value = (VALUE) &entry.value();
                } else {
                    key = (KEY) &it.next();
                    value = (VALUE) &defaultVal;
                }

                ENTRY middle = &Unsafe::allocateInstance<Entry>(key[0], value[0], null);

                // color nodes in non-full bottommost level red
                if (level == redLevel)
                    middle->color = RED;

                if (left != null) {
                    middle->left = left;
                    left->parent = middle;
                }

                if (mid < hi) {
                    ENTRY right = buildFromSorted(level + 1, mid + 1, hi, redLevel, it, defaultVal);
                    middle->right = right;
                    right->parent = middle;
                }

                return middle;
            }

            /**
             * Finds the level down to which to assign all nodes BLACK.  This is the
             * last `full' level of the complete binary tree produced by buildTree.
             * The remaining nodes are colored RED. (This makes a `nice' set of
             * color assignments wrt future insertions.) This level number is
             * computed by finding the number of splits needed to reach the zeroth
             * node.
             *
             * @param size the (non-negative) number of keys in the tree to be built
             */
            static gint computeRedLevel(gint size) { return 31 - Integer::leadingZeros(size + 1); }

            CORE_FRATERNITY_T1(TreeSet);

        public:

            /**
             *
             */
            gbool replace(const K &key, const V &oldValue, const V &newValue) override {
                ENTRY p = entryOf(key);
                if (p == null)
                    return false;
                if (Object::equals(oldValue, p->value()))
                    p->set(newValue);
                return true;
            }

            /**
             * @throws KeyNotFoundException if specified key is not mapped by this map
             */
            const V &replace(const K &key, const V &value) override {
                ENTRY p = entryOf(key);
                if (p != null)
                    return p->set(value);
                NoSuchKeyException($(key)).throws(__trace("core.util.TreeMap"));
            }

            /**
             *
             */
//            void replaceAll(BiFunction<K, V &, V> /*function*/) override {
//            }

            /**
             *
             */
            void forEach(const BinaryActionConsumer<K, V> &action) override {
                //
            }

        private:

            // View class support
            class Values CORE_FINAL : public Collection<V> {
            private:
                TreeMap<K, V> &root;

            public:
                CORE_FAST Values(TreeMap<K, V> &root) : root(root) {}

                Iterator<const V> &iterator() const override {
                    return Unsafe::allocateInstance<ValueItr>((TreeMap<K, V> &) root, root.firstEntry());
                }

                gint size() const override { return root.len; }

                gbool contains(const V &o) const override { return root.containsValue(o); }

                gbool remove(const V &o) override {
                    for (ENTRY e = root.firstEntry(); e != null; e = root.successor(e)) {
                        if (Object::equals(e->value(), o)) {
                            root.deleteEntry(e);
                            return true;
                        }
                    }
                    return false;
                }

                void clear() override { root.clear(); }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<Values>::hasInstance(o))
                        return false;
                    const Values &values = (Values &) o;
                    if (&values.root == &root)
                        return true;
                    Iterator<const V> &it1 = iterator();
                    Iterator<const V> &it2 = values.iterator();
                    while (it1.hasNext() && it2.hasNext())
                        if (!Object::equals(it1.next(), it2.next()))
                            return false;
                    return !it1.hasNext() && !it2.hasNext();
                }
            };

            class EntrySet CORE_FINAL : public Set<MapEntry>, public SortedStruct<MapEntry> {
            private:
                TreeMap<K, V> &root;

            public:
                CORE_FAST EntrySet(TreeMap<K, V> &root) : root(root) {}

                Iterator<const MapEntry> &iterator() const override {
                    return Unsafe::allocateInstance<EntryItr>((TreeMap<K, V> &) root, root.firstEntry());
                }

                gint size() const override { return root.len; }

                gbool contains(const MapEntry &o) const override {
                    ENTRY e = root.entryOf(o.key());
                    return (e != null) && Object::equals(e->value(), o.value());
                }

                gbool remove(const MapEntry &o) override {
                    ENTRY e = root.entryOf(o.key());
                    if ((e != null) && Object::equals(e->value(), o.value())) {
                        root.deleteEntry(e);
                        return true;
                    }
                    return false;
                }

                void clear() override { root.clear(); }

                const Comparator<MapEntry> &comparator() const override {
                    return Unsafe::allocateInstance<EntryComparator>((KeyComparator &)root.comparator());
                }
            };

            class ReverseEntrySet CORE_FINAL : public Set<MapEntry>, public SortedStruct<MapEntry> {
            private:
                TreeMap<K, V> &root;

            public:
                CORE_FAST ReverseEntrySet(TreeMap<K, V> &root) : root(root) {}

                Iterator<const MapEntry> &iterator() const override {
                    return Unsafe::allocateInstance<ReverseEntryItr>((TreeMap<K, V> &) root, root.lastEntry());
                }

                gint size() const override { return root.len; }

                gbool contains(const MapEntry &o) const override {
                    ENTRY e = root.entryOf(o.key());
                    return (e != null) && Object::equals(e->value(), o.value());
                }

                gbool remove(const MapEntry &o) override {
                    ENTRY e = root.entryOf(o.key());
                    if ((e != null) && Object::equals(e->value(), o.value())) {
                        root.deleteEntry(e);
                        return true;
                    }
                    return false;
                }

                void clear() override { root.clear(); }

                const Comparator<MapEntry> &comparator() const override {
                    return Unsafe::allocateInstance<EntryComparator>(root.comparator().reverse());
                }
            };

            class KeySet CORE_FINAL : public Set<K>, public SortedStruct<K> {
            private:
                TreeMap<K, V> &root;

            public:
                CORE_FAST KeySet(TreeMap<K, V> &root) : root(root) {}

                Iterator<const K> &iterator() const override {
                    return Unsafe::allocateInstance<KeyItr>((TreeMap<K, V> &) root, root.firstEntry());
                }

                gint size() const override { return root.len; }

                gbool contains(const K &o) const override { return root.containsKey(o); }

                gbool remove(const K &o) override {
                    ENTRY e = root.entryOf(o);
                    if (e == null)
                        return false;
                    root.deleteEntry(e);
                    return true;
                }

                void clear() override { return root.clear(); }

                const Comparator<K> &comparator() const override {
                    return root.comparator();
                }
            };

            class ReverseKeySet CORE_FINAL : public Set<K>, public SortedStruct<K> {
            private:
                TreeMap<K, V> &root;

            public:
                CORE_FAST ReverseKeySet(TreeMap<K, V> &root) : root(root) {}

                Iterator<const K> &iterator() const override {
                    return Unsafe::allocateInstance<ReverseKeyItr>((TreeMap<K, V> &) root, root.lastEntry());
                }

                gint size() const override { return root.len; }

                gbool contains(const K &o) const override { return root.containsKey(o); }

                gbool remove(const K &o) override {
                    ENTRY e = root.entryOf(o);
                    if (e == null)
                        return false;
                    root.deleteEntry(e);
                    return true;
                }

                void clear() override { return root.clear(); }

                const Comparator<K> &comparator() const override {
                    return root.comparator().reverse();
                }
            };

            /**
             * The TreeMap iterators
             */
            class EntryComparator CORE_FINAL : public Comparator<MapEntry> {
            private:
                /**
                 * The base of comparator
                 */
                KeyComparator &keyComparator;
                gbool reversed = false;

            public:
                CORE_FAST EntryComparator(KeyComparator &keyComparator) : keyComparator(keyComparator) {}

                gint compare(const MapEntry &o1, const MapEntry &o2) const override {
                    return keyComparator.compare(o1.key(), o2.key());
                }

                Object &clone() const override {
                    return Unsafe::allocateInstance<EntryComparator>(keyComparator);
                }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (Class<EntryComparator>::hasInstance(o)) {
                        const EntryComparator &cpr = (EntryComparator &) o;
                        return keyComparator == cpr.keyComparator;
                    }
                    return false;
                }

                Comparator<MapEntry> &reverse() const override {
                    EntryComparator &c = Unsafe::allocateInstance<EntryComparator>(keyComparator.reverse());
                    c.reversed = !reversed;
                    return c;
                }

            private:
                Comparator<MapEntry> &base() const override {
                    if(!reversed)
                        UnsupportedOperationException().throws(__trace("core.util.TreeMap"));
                    EntryComparator &c = Unsafe::allocateInstance<EntryComparator>(keyComparator.reverse());
                    c.reversed = !reversed;
                    return c;
                }

                gbool isReversed() const override {
                    return reversed;
                }
            };

            /**
             * Base class for TreeMap Iterators
             */
            template<class T>
            class AbstractItr : public Iterator<const T> {
            private:
                ENTRY cursor = {};
                ENTRY last = {};
                gint modNum;
                TreeMap<K, V> &root;

            public:
                CORE_FAST AbstractItr(TreeMap<K, V> &root, ENTRY first) :
                        root(root), cursor(first), last(null), modNum(root.modNum) {}

                gbool hasNext() const override { return cursor != null; }

                ENTRY nextEntry() {
                    ENTRY e = cursor;
                    if (e == null) NoSuchElementException().throws(__trace("core.util.TreeMap.AbstractItr"));
                    if (modNum != root.modNum) ConcurrentException().throws(__trace("core.util.TreeMap.AbstractItr"));
                    cursor = successor(cursor);
                    last = e;
                    return e;
                }

                ENTRY prevEntry() {
                    ENTRY e = cursor;
                    if (e == null) NoSuchElementException().throws(__trace("core.util.TreeMap.AbstractItr"));
                    if (modNum != root.modNum) ConcurrentException().throws(__trace("core.util.TreeMap.AbstractItr"));
                    cursor = predecessor(cursor);
                    last = e;
                    return e;
                }

                void remove() override {
                    if (last == null) IllegalStateException().throws(__trace("core.util.TreeMap.AbstractItr"));
                    if (modNum != root.modNum) ConcurrentException().throws(__trace("core.util.TreeMap.AbstractItr"));
                    // deleted entries are replaced by their successors
                    if (last->left != null && last->right != null) cursor = last;
                    root.deleteEntry(last);
                    modNum = root.modNum;
                    last = null;
                }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<AbstractItr>::hasInstance(o))
                        return false;
                    const AbstractItr &itr = (AbstractItr &) o;
                    return &itr.root == &root && itr.cursor == cursor;
                }
            };

            class ValueItr : public AbstractItr<V> {
            public:
                CORE_FAST ValueItr(TreeMap<K, V> &root, ENTRY first) : AbstractItr<V>(root, first) {}

                const V &next() override {
                    try { return ValueItr::nextEntry()->value(); }
                    catch (const Exception &e) { e.throws(__trace("core.util.TreeMap.ValueItr")); }
                }

                Object &clone() const override {
                    return Unsafe::allocateInstance<ValueItr>(*this);
                }
            };

            class KeyItr : public AbstractItr<K> {
            public:
                CORE_FAST KeyItr(TreeMap<K, V> &root, ENTRY first) : AbstractItr<K>(root, first) {}

                const K &next() override {
                    try { return KeyItr::nextEntry()->key(); }
                    catch (const Exception &e) { e.throws(__trace("core.util.TreeMap.KeyItr")); }
                }

                Object &clone() const override {
                    return Unsafe::allocateInstance<KeyItr>(*this);
                }
            };

            class EntryItr : public AbstractItr<MapEntry> {
            public:
                CORE_FAST EntryItr(TreeMap<K, V> &root, ENTRY first) : AbstractItr<MapEntry>(root, first) {}

                const MapEntry &next() override {
                    try { return EntryItr::nextEntry()[0]; }
                    catch (const Exception &e) { e.throws(__trace("core.util.TreeMap.EntryItr")); }
                }

                Object &clone() const override {
                    return Unsafe::allocateInstance<EntryItr>(*this);
                }
            };

            class ReverseKeyItr : public AbstractItr<K> {
            public:
                CORE_FAST ReverseKeyItr(TreeMap<K, V> &root, ENTRY first) : AbstractItr<K>(root, first) {}

                const K &next() override {
                    try { return ReverseKeyItr::prevEntry()->key(); }
                    catch (const Exception &e) { e.throws(__trace("core.util.TreeMap.ReverseKeyItr")); }
                }

                Object &clone() const override {
                    return Unsafe::allocateInstance<ReverseKeyItr>(*this);
                }
            };

            class ReverseEntryItr : public AbstractItr<MapEntry> {
            public:
                CORE_FAST ReverseEntryItr(TreeMap<K, V> &root, ENTRY first) : AbstractItr<MapEntry>(root, first) {}

                const MapEntry &next() override {
                    try { return ReverseEntryItr::prevEntry()[0]; }
                    catch (const Exception &e) { e.throws(__trace("core.util.TreeMap.ReverseEntryItr")); }
                }

                Object &clone() const override {
                    return Unsafe::allocateInstance<ReverseEntryItr>(*this);
                }
            };

            using Map<K, V>::$;

        public:
            virtual ~TreeMap() {
                clear();
                eSet = reSet = null;
                Map<K, V>::kSet = rkSet = null;
                Map<K, V>::vCollection = null;
                modNum = 0;
            }
        };

#if CORE_TEMPLATE_TYPE_DEDUCTION
        template<class K, class V>
        TreeMap(Map<K, V>) -> TreeMap<K, V>;

        template<class K, class V = Object>
        TreeMap(Comparator<K>) -> TreeMap<K, V>;

        template<class K = Object, class V = Object>
        TreeMap(gint) -> TreeMap<K, V>;

        template<class K = Object, class V = Object>
        TreeMap() -> TreeMap<K, V>;
#endif

    }
} // core

#endif //CORE23_TREEMAP_H
