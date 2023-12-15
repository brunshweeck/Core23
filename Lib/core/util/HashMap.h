//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_HASHMAP_H
#define CORE23_HASHMAP_H

#include <core/Float.h>
#include <core/private/ArraysSupport.h>
#include <core/util/Map.h>

namespace core {
    namespace util {

        /**
         * Hash table based implementation of the <b> Map</b>  interface.  This
         * implementation provides all of the optional map operations, and permits
         * <b> null</b>  values and the <b> null</b>  key.  (The <b> HashMap</b>
         * class is roughly equivalent to <b> Hashtable</b> , except that it is
         * unsynchronized and permits nulls.)  This class makes no guarantees as to
         * the order of the map; in particular, it does not guarantee that the order
         * will remain constant over time.
         *
         * <p>This implementation provides constant-time performance for the basic
         * operations (<b> get</b>  and <b> put</b> ), assuming the hash function
         * disperses the elements properly among the buckets.  Iteration over
         * collection views requires time proportional to the "capacity" of the
         * <b> HashMap</b>  instance (the number of buckets) plus its size (the number
         * of key-value mappings).  Thus, it's very important not to set the initial
         * capacity too high (or the load factor too low) if iteration performance is
         * important.
         *
         * <p>An instance of <b> HashMap</b>  has two parameters that affect its
         * performance: <i>initial capacity</i> and <i>load factor</i>.  The
         * <i>capacity</i> is the number of buckets in the hash table, and the initial
         * capacity is simply the capacity at the time the hash table is created.  The
         * <i>load factor</i> is a measure of how full the hash table is allowed to
         * get before its capacity is automatically increased.  When the number of
         * entries in the hash table exceeds the product of the load factor and the
         * current capacity, the hash table is <i>rehashed</i> (that is, internal data
         * structures are rebuilt) so that the hash table has approximately twice the
         * number of buckets.
         *
         * <p>As a general rule, the default load factor (.75) offers a good
         * tradeoff between time and space costs.  Higher values decrease the
         * space overhead but increase the lookup cost (reflected in most of
         * the operations of the <b> HashMap</b>  class, including
         * <b> get</b>  and <b> put</b> ).  The expected number of entries in
         * the map and its load factor should be taken into account when
         * setting its initial capacity, so as to minimize the number of
         * rehash operations.  If the initial capacity is greater than the
         * maximum number of entries divided by the load factor, no rehash
         * operations will ever occur.
         *
         * <p>If many mappings are to be stored in a <b> HashMap</b>
         * instance, creating it with a sufficiently large capacity will allow
         * the mappings to be stored more efficiently than letting it perform
         * automatic rehashing as needed to grow the table.  Note that using
         * many keys with the same <b> hashCode()</b>  is a sure way to slow
         * down performance of any hash table. To ameliorate impact, when keys
         * are <b style="color:orange;"> Comparable</b> , this class may use comparison order among
         * keys to help break ties.
         *
         * <p><strong>Note that this implementation is not synchronized.</strong>
         * If multiple threads access a hash map concurrently, and at least one of
         * the threads modifies the map structurally, it <i>must</i> be
         * synchronized externally.  (A structural modification is any operation
         * that adds or deletes one or more mappings; merely changing the value
         * associated with a key that an instance already contains is not a
         * structural modification.)  This is typically accomplished by
         * synchronizing on some object that naturally encapsulates the map.
         *
         * If no such object exists, the map should be "wrapped" using the
         * <b style="color:orange;"> core.util.synchronizedMap</b>
         * method.
         *
         * <p>The iterators returned by all of this class's "collection view methods"
         * are <i>fail-fast</i>: if the map is structurally modified at any time after
         * the iterator is created, in any way except through the iterator's own
         * <b> remove</b>  method, the iterator will throw a
         * <b style="color:orange;"> ConcurrentException</b> .  Thus, in the face of concurrent
         * modification, the iterator fails quickly and cleanly, rather than risking
         * arbitrary, non-deterministic behavior at an undetermined time in the
         * future.
         *
         * <p>Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw <b> ConcurrentException</b>  on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness: <i>the fail-fast behavior of iterators
         * should be used only to detect bugs.</i>
         *
         * @tparam K the type of keys maintained by this map
         * @tparam V the type of mapped values
         *
         * @author  Brunshweeck Tazeussong
         * @see     Object.hash()
         * @see     Collection
         * @see     Map
         * @see     TreeMap
         * @see     Hashtable
         */
        template<class K, class V>
        class HashMap : public Map<K, V> {

            /*
             * Implementation notes.
             *
             * This map usually acts as a binned (bucketed) hash table, but
             * when bins get too large, they are transformed into bins of
             * TreeNodes, each structured similarly to those in
             * java.util.TreeMap. Most methods try to use normal bins, but
             * relay to TreeNode methods when applicable (simply by checking
             * instanceof a node).  Bins of TreeNodes may be traversed and
             * used like any others, but additionally support faster lookup
             * when overpopulated. However, since the vast majority of bins in
             * normal use are not overpopulated, checking for existence of
             * tree bins may be delayed in the course of table methods.
             *
             * Tree bins (i.e., bins whose elements are all TreeNodes) are
             * ordered primarily by hashCode, but in the case of ties, if two
             * elements are of the same "class C implements Comparable<C>",
             * type then their compareTo method is used for ordering. (We
             * conservatively check generic types via reflection to validate
             * this -- see method comparableClassFor).  The added complexity
             * of tree bins is worthwhile in providing worst-case O(log n)
             * operations when keys either have distinct hashes or are
             * orderable, Thus, performance degrades gracefully under
             * accidental or malicious usages in which hashCode() methods
             * return values that are poorly distributed, as well as those in
             * which many keys share a hashCode, so long as they are also
             * Comparable. (If neither of these apply, we may waste about a
             * factor of two in time and space compared to taking no
             * precautions. But the only known cases stem from poor user
             * programming practices that are already so slow that this makes
             * little difference.)
             *
             * Because TreeNodes are about twice the size of regular nodes, we
             * use them only when bins contain enough nodes to warrant use
             * (see TREEIFY_THRESHOLD). And when they become too small (due to
             * removal or resizing) they are converted back to plain bins.  In
             * usages with well-distributed user hashCodes, tree bins are
             * rarely used.  Ideally, under random hashCodes, the frequency of
             * nodes in bins follows a Poisson distribution
             * (http://en.wikipedia.org/wiki/Poisson_distribution) with a
             * parameter of about 0.5 on average for the default resizing
             * threshold of 0.75, although with a large variance because of
             * resizing granularity. Ignoring variance, the expected
             * occurrences of list size k are (exp(-0.5) * pow(0.5, k) /
             * factorial(k)). The first values are:
             *
             * 0:    0.60653066
             * 1:    0.30326533
             * 2:    0.07581633
             * 3:    0.01263606
             * 4:    0.00157952
             * 5:    0.00015795
             * 6:    0.00001316
             * 7:    0.00000094
             * 8:    0.00000006
             * more: less than 1 in ten million
             *
             * The root of a tree bin is normally its first node.  However,
             * sometimes (currently only upon Iterator.remove), the root might
             * be elsewhere, but can be recovered following parent links
             * (method TreeNode.root()).
             *
             * All applicable internal methods accept a hash code as an
             * argument (as normally supplied from a public method), allowing
             * them to call each other without recomputing user hashCodes.
             * Most internal methods also accept a "tab" argument, that is
             * normally the current table, but may be a new or old one when
             * resizing or converting.
             *
             * When bin lists are treeified, split, or untreeified, we keep
             * them in the same relative access/traversal order (i.e., field
             * Node.next) to better preserve locality, and to slightly
             * simplify handling of splits and traversals that invoke
             * iterator.remove. When using comparators on insertion, to keep a
             * total ordering (or as close as is required here) across
             * rebalancings, we compare classes and identityHashCodes as
             * tie-breakers.
             *
             * The use and transitions among plain vs tree modes is
             * complicated by the existence of subclass LinkedHashMap. See
             * below for hook methods defined to be invoked upon insertion,
             * removal and access that allow LinkedHashMap internals to
             * otherwise remain independent of these mechanics. (This also
             * requires that a map instance be passed to some utility methods
             * that may create new nodes.)
             *
             * The concurrent-programming-like SSA-based coding style helps
             * avoid aliasing errors amid all of the twisty pointer operations.
             */

        private:

            CORE_ALIAS(U, native::Unsafe);

            /**
             * Capture<T> represent all type T that extends this Map key type K.
             * in other word K is base of T (Class<K>::isSuper<T>() is true).
             */
            template<class T>
            CORE_ALIAS(CaptureK, typename Class<T>::template Iff<Class<K>::template isSuper<T>()>);

            /**
             * Capture<T> represent all type T that extends this Map value type V.
             * in other word V is base of T (Class<V>::isSuper<T>() is true).
             */
            template<class T>
            CORE_ALIAS(CaptureV, typename Class<T>::template Iff<Class<V>::template isSuper<T>()>);

            // Used by table form
            class Node;

            // Used by LinkedHashMap
            class LinkedEntry;

            // Used by Tree Bin form
            class TreeNode;

            /**
             *
             */
            CORE_ALIAS(COMPARATOR, typename Class<Comparator<K>>::Ptr);
            CORE_ALIAS(NODE, typename Class<Node>::Ptr);
            CORE_ALIAS(TNODE, typename Class<TreeNode>::Ptr);
            CORE_ALIAS(LNODE, typename Class<LinkedEntry>::Ptr);
            CORE_ALIAS(ARRAY, typename Class<NODE>::Ptr);
            template<class T, class U>
            CORE_ALIAS(ExEntry, , typename Map<T, U>::Entry);
            CORE_ALIAS(KEY, typename Class<K>::Ptr);
            CORE_ALIAS(VALUE, typename Class<V>::Ptr);
            CORE_ALIAS(ENTRIES, , typename Class<Set<ExEntry<K, V>>>::Ptr);
            CORE_ALIAS(VALUES, typename Class<Collection<V>>::Ptr);
            CORE_ALIAS(KEYSET, typename Class<Set<K>>::Ptr);

            /**
             * The default initial capacity - MUST be a power of two.
             */
            static CORE_FAST gint DEFAULT_CAPACITY = 1 << 4;

            /**
             * The maximum capacity, used if a higher value is implicitly specified
             * by either of the constructors with arguments.
             * MUST be a power of two <= 1 << 30.
             */
            static CORE_FAST gint MAXIMUM_CAPACITY = 1 << 30;

            /**
             * The load factor used when none specified in constructor.
             */
            static CORE_FAST gfloat DEFAULT_FACTOR = 0.75F;

            /**
             * The bin count threshold for using a tree rather than list for a
             * bin.  Bins are converted to trees when adding an element to a
             * bin with at least this many nodes. The value must be greater
             * than 2 and should be at least 8 to mesh with assumptions in
             * tree removal about conversion back to plain bins upon
             * shrinkage.
             */
            static CORE_FAST gint TREEIFY_THRESHOLD = 1 << 3;

            /**
             * The bin count threshold for untreeifying a (split) bin during a
             * resize operation. Should be less than TREEIFY_THRESHOLD, and at
             * most 6 to mesh with shrinkage detection under removal.
             */
            static CORE_FAST gint UNTREEIFY_THRESHOLD = (1 << 3) - 2;

            /**
             * The smallest table capacity for which bins may be treeified.
             * (Otherwise the table is resized if too many nodes in a bin.)
             * Should be at least 4 * TREEIFY_THRESHOLD to avoid conflicts
             * between resizing and treeification thresholds.
             */
            static CORE_FAST gint MIN_TREEIFY_CAPACITY = 1 << 6;

            /**
             * Basic hash bin node, used for most entries.  (See below for
             * TreeNode subclass, and in LinkedHashMap for its Entry subclass.)
             */
            interface Node : public ExEntry<K, V> {
                gint hash = {};
                KEY k = {};
                VALUE v = {};
                NODE next = {};


                Node(gint hash, K &k, V &v, NODE next) : hash(hash), k(&k), v(&v), next(next) {}

                const K &key() const override { return k[0]; }

                V &value() override { return v[0]; }

                const V &value() const override { return v[0]; }

                const V &set(const V &value) override {
                    VALUE oldValue = v;
                    v = &U::copyInstance(value, true);
                    return oldValue[0];
                }
            };

            /**
             * Computes key.hashCode() and spreads (XORs) higher bits of hash
             * to lower.  Because the table uses power-of-two masking, sets of
             * hashes that vary only in bits above the current mask will
             * always collide. (Among known examples are sets of Float keys
             * holding consecutive whole numbers in small tables.)  So we
             * apply a transform that spreads the impact of higher bits
             * downward. There is a tradeoff between speed, utility, and
             * quality of bit-spreading. Because many common sets of hashes
             * are already reasonably distributed (so don't benefit from
             * spreading), and because we use trees to handle large sets of
             * collisions in bins, we just XOR some shifted bits in the
             * cheapest possible way to reduce systematic lossage, as well as
             * to incorporate impact of the highest bits that would otherwise
             * never be used in index calculations because of table bounds.
             */
            static gint hash(const K &key) {
                if (key == null)
                    return 0;
                else {
                    gint h = key.hash();
                    return h ^ (h >> 16);
                }
            }

            /**
             * Returns k.compareTo(x) if x matches kc (k's screened comparable
             * class), else 0.
             */
            static CORE_FAST gint compareComparables(const K &k, const K &x) {
                return !Class<Comparable<K>>::template isSuper<K>() ? 0 : ((Comparable<K> &) k).compareTo(x);
            }

            /**
             * Returns a power of two size for the given target capacity.
             */
            static gint tableSizeFor(gint capacity) {
                gint n = -1 >> Integer::leadingZeros(capacity - 1);
                return (n < 0) ? 1 : (n >= MAXIMUM_CAPACITY) ? MAXIMUM_CAPACITY : n + 1;
            }

            using Map<K, V>::$;

            /**
             * The table, initialized on first use, and resized as
             * necessary. When allocated, length is always a power of two.
             * (We also tolerate length zero in some operations to allow
             * bootstrapping mechanics that are currently not needed.)
             */
            ARRAY table = {};

            /**
             * The number of place allocated form table
             *
             */
            gint capacity = {};

            /**
             * Holds cached entrySet(). Note that AbstractMap fields are used
             * for keySet() and values().
             */
            ENTRIES eSet = {};

            /**
             * The number of key-value mappings contained in this map.
             */
            gint len = {};

            /**
             * The number of times this HashMap has been structurally modified
             * Structural modifications are those that change the number of mappings in
             * the HashMap or otherwise modify its internal structure (e.g.,
             * rehash).  This field is used to make iterators on Collection-views of
             * the HashMap fail-fast.  (See ConcurrentModificationException).
             */
            gint modNum = {};

            /**
             * The next size value at which to resize (capacity * load factor).
             */
            gint threshold = {};

            /**
             * The load factor for the hash table.
             */
            gfloat factor = {};

        public:

            /**
             * Constructs an empty <b> HashMap</b>  with the specified initial
             * capacity and load factor.
             *
             * @param  initialCapacity the initial capacity
             * @param  loadFactor      the load factor
             * @throws ArgumentException if the initial capacity is negative
             *         or the load factor is non-positive
             */
            CORE_EXPLICIT HashMap(gint initialCapacity, gfloat loadFactor) {
                if (initialCapacity < 0)
                    ArgumentException("Illegal initial capacity: " + $(initialCapacity))
                            .throws(__trace("core.util.HashMap"));
                if (initialCapacity > MAXIMUM_CAPACITY)
                    initialCapacity = MAXIMUM_CAPACITY;
                if (loadFactor < 0 || Float::isNaN(loadFactor))
                    ArgumentException("Illegal load factor: " + $(loadFactor)).throws(__trace("core.util.HashMap"));
                factor = loadFactor;
                threshold = tableSizeFor(initialCapacity);
            }

            /**
             * Constructs an empty <b> HashMap</b>  with the specified initial
             * capacity and the default load factor (0.75).
             *
             * @param  initialCapacity the initial capacity.
             * @throws ArgumentException if the initial capacity is negative.
             */
            CORE_EXPLICIT HashMap(gint initialCapacity) : HashMap(initialCapacity, DEFAULT_FACTOR) {}

            /**
             * Constructs an empty <b> HashMap</b>  with the default initial capacity
             * (16) and the default load factor (0.75).
             */
            CORE_FAST HashMap() : factor(DEFAULT_FACTOR) {}

            /**
             * Constructs a new <b> HashMap</b>  with the same mappings as the
             * specified <b> Map</b> .  The <b> HashMap</b>  is created with
             * default load factor (0.75) and an initial capacity sufficient to
             * hold the mappings in the specified <b> Map</b> .
             *
             * @param   m the map whose mappings are to be placed in this map
             */
            template<class T = K, class U = V>
            CORE_EXPLICIT HashMap(const Map<CaptureK<T>, CaptureV<T>> &m): factor(DEFAULT_FACTOR) {
                addEntries(m, false);
            }

            //

            /**
             * Constructs a new <b> HashMap</b>  with the same mappings as the
             * specified <b> Map</b> .  The <b> HashMap</b>  is created with
             * default load factor (0.75) and an initial capacity sufficient to
             * hold the mappings in the specified <b> Map</b> .
             *
             * @param   m the map whose mappings are to be placed in this map
             */
            HashMap(const HashMap<K, V> &m) : factor(DEFAULT_FACTOR) { addEntries(m, false); }

            //

            /**
             * Constructs a new <b> HashMap</b>  with the same mappings as the
             * specified <b> Map</b> .  The <b> HashMap</b>  is created with
             * default load factor (0.75) and an initial capacity sufficient to
             * hold the mappings in the specified <b> Map</b> .
             *
             * @param   m the map whose mappings are to be placed in this map
             */
            HashMap(HashMap<K, V> &&m) CORE_NOTHROW {
                swap(table, m.table);
                swap(capacity, m.capacity);
                swap(len, m.len);
                swap(threshold, m.threshold);
                swap(factor, m.factor);
                m.modNum += 1;
            }

            /**
             * Set structural composition with composition of another HashMap
             *
             * @param   m the map whose mappings are to be placed in this map
             */
            HashMap<K, V> &operator=(const HashMap<K, V> &m) {
                if (this != &m) {
                    clear();
                    addEntries(m, false);
                }
                return *this;
            }

            /**
             * Swap structural composition with composition of another HashMap
             *
             * @param   m the map whose mappings are to be placed in this map
             */
            HashMap<K, V> &operator=(HashMap<K, V> &&m) CORE_NOTHROW {
                if (this != &m) {
                    swap(table, m.table);
                    swap(capacity, m.capacity);
                    swap(len, m.len);
                    swap(threshold, m.threshold);
                    swap(factor, m.factor);
                    modNum += 1;
                    m.modNum += 1;
                }
                return *this;
            }

        private:

            /**
             * Implements Map.putAll and Map constructor.
             *
             * @param m the map
             * @param evict false when initially constructing this map, else
             * true (relayed to method afterNodeInsertion).
             */
            template<class T = K, class U = V>
            void addEntries(const Map<T, U> &m, gbool evict) {
                gint size = m.size();
                if (size > 0) {
                    if (table == null) {
                        // pre-size
                        gdouble dt = Math::ceil(size / (gdouble) factor);
                        gint t = (dt < (gdouble) MAXIMUM_CAPACITY) ? (gint) dt : MAXIMUM_CAPACITY;
                        if (t > threshold)
                            threshold = tableSizeFor(t);
                    } else {
                        // Because of linked-list bucket constraints, we cannot
                        // expand all at once, but can reduce total resize
                        // effort by repeated doubling now vs later
                        while (size > threshold && capacity < MAXIMUM_CAPACITY)
                            resize();
                    }

                    for (const ExEntry<T, U> &e: m.entrySet()) {
                        const K &key = e.key();
                        const V &value = e.value();
                        addEntry(HashMap<K, V>::hash(key), key, value, false, evict);
                    }
                }
            }

            template<class T, class U>
            static void swap(T &t, U &u) {
                T temp = t;
                t = u;
                u = temp;
            }

        public:

            /**
             * Returns the number of key-value mappings in this map.
             *
             * @return the number of key-value mappings in this map
             */
            gint size() const override { return len; }

            /**
             * Returns the value to which the specified key is mapped,
             * or <b> null</b>  if this map contains no mapping for the key.
             *
             * <p>More formally, if this map contains a mapping from a key
             * <b> k</b>  to a value <b> v</b>  such that <b> key.equals(k)</b> ,
             * then this method returns <b> v</b> ; otherwise
             * it throws <b> KeyNotFoundException</b> .  (There can be at most one such mapping.)
             *
             * <p>A return value of <b> null</b>  does not <i>necessarily</i>
             * indicate that the map contains no mapping for the key; it's also
             * possible that the map explicitly maps the key to <b> null</b> .
             * The <b style="color:orange;"> containsKey</b>  operation may be used to
             * distinguish these two cases.
             *
             * @throws KeyNotFoundException If this map not mapping the given key
             *
             * @see put(K, V)
             */
            V &get(const K &key) override {
                NODE e = entryOf(key);
                if (e == null)
                    KeyNotFoundException($(key)).throws(__trace("core.util.HashMap"));
                return e->value();
            }

            /**
             * Returns the value to which the specified key is mapped,
             * or <b> null</b>  if this map contains no mapping for the key.
             *
             * <p>More formally, if this map contains a mapping from a key
             * <b> k</b>  to a value <b> v</b>  such that <b> key.equals(k)</b> ,
             * then this method returns <b> v</b> ; otherwise
             * it throws <b> KeyNotFoundException</b> .  (There can be at most one such mapping.)
             *
             * <p>A return value of <b> null</b>  does not <i>necessarily</i>
             * indicate that the map contains no mapping for the key; it's also
             * possible that the map explicitly maps the key to <b> null</b> .
             * The <b style="color:orange;"> containsKey</b>  operation may be used to
             * distinguish these two cases.
             *
             * @throws KeyNotFoundException If this map not mapping the given key
             *
             * @see put(K, V)
             */
            const V &get(const K &key) const override {
                NODE e = entryOf(key);
                if (e == null)
                    KeyNotFoundException($(key)).throws(__trace("core.util.HashMap"));
                return e->value();
            }

        private:

            /**
             * Implements Map.get and related methods.
             *
             * @param key the key
             * @return the node, or null if none
             */
            NODE entryOf(const K &key) const {
                ARRAY tab = {};
                NODE e = {};
                NODE first = {};
                gint hash = {};
                gint n = {};
                KEY k = {};
                if ((tab = table) != null && (n = capacity) > 0 &&
                    (first = tab[(n - 1) & (hash = HashMap<K, V>::hash(key))]) != null) {
                    if (first->hash == hash && // always check first node
                        ((k = first->k) == &key || (key != null && key.equals(k[0]))))
                        return first;
                    if ((e = first->next) != null) {
                        if (Class<TreeNode>::hasInstance(first[0]))
                            return ((TNODE) first)->entryOf(hash, key);
                        do {
                            if (e->hash == hash && ((k = e->k) == &key || key.equals(k[0])))
                                return e;
                        } while ((e = e->next) != null);
                    }
                }
                return null;
            }

        public:

            /**
             * Returns <b> true</b>  if this map contains a mapping for the
             * specified key.
             *
             * @param   key   The key whose presence in this map is to be tested
             * @return <b> true</b>  if this map contains a mapping for the specified
             * key.
             */
            gbool containsKey(const K &key) const override { return entryOf(key) != null; }

            /**
             * Associates the specified value with the specified key in this map.
             * If the map previously contained a mapping for the key, the old
             * value is replaced.
             *
             * @param key key with which the specified value is to be associated
             * @param value value to be associated with the specified key
             * @return the previous value associated with <b> key</b> , or
             *         <b> the new value </b>  if there was no mapping for <b> key</b> .
             */
            const V &put(const K &key, const V &value) override {
                return addEntry(HashMap<K, V>::hash(key), key, value, false, true);
            }

            /**
             * Associates the specified value with the specified key in this map if and only if
             * specified key is not mapped on this map.
             * If the map previously contained a mapping for the key, the existing
             * value is returned, otherwise the newly value is returned.
             *
             * @param key key with which the specified value is to be associated
             * @param value value to be associated with the specified key
             * @return the newly value associated with <b> key</b> , or
             *         <b> the current value </b>  if there was mapping for <b> key</b> .
             */
            const V &putIfAbsent(const K &key, const V &value) override {
                return addEntry(HashMap<K, V>::hash(key), key, value, true, true);
            }

        private:

            /**
             * Implements Map.put and related methods.
             *
             * @param hash hash for key
             * @param key the key
             * @param value the value to put
             * @param addIfAbsent if true, don't change existing value
             * @param evict if false, the table is in creation mode.
             * @return previous value, or the new value if none
             */
            const V &addEntry(gint hash, const K &key, const V &value, gbool addIfAbsent, gbool evict) {
                ARRAY tab = {};
                NODE p = {};
                gint n = {};
                gint i = {};
                VALUE retVal = {};
                if ((tab = table) == null || (n = capacity) == 0) {
                    tab = resize();
                    n = capacity;
                }
                if ((p = tab[i = (n - 1) & hash]) == null)
                    tab[i] = newNode(hash, key, value, null);
                else {
                    NODE e = {};
                    KEY k = {};
                    if ((p->hash == hash) && (((k = p->k) == &key) || key.equals(k[0]))) {
                        e = p;
                    } else if (Class<TreeNode>::hasInstance(p[0])) {
                        e = ((TNODE) p)->addEntry(*this, tab, hash, key, value);
                    } else {
                        for (gint binCount = 0;; ++binCount) {
                            if ((e = p->next) == null) {
                                p->next = newNode(hash, key, value, null);
                                if (binCount >= TREEIFY_THRESHOLD - 1) // -1 for 1st
                                    treeifyBin(tab, hash);
                                break;
                            }
                            if (e->hash == hash && ((k = e->k) == &key || key.equals(k[0]))) {
                                break;
                            }
                            p = e;
                        }
                    }
                    if (e != null) { // existing mapping for key
                        VALUE oldValue = e->v;
                        if (!addIfAbsent || oldValue == null) {
                            e->v = &U::copyInstance(value, true);
                        }
                        afterNodeAccess(e);
                        return oldValue[0];
                    }
                }
                ++modNum;
                if (++len > threshold)
                    resize();
                afterNodeInsertion(evict);
                return retVal[0];
            }

            /**
             * convert the given capacity to equivalent in bytes
             */
            static CORE_FAST glong L(gint capacity) { return 1LL * capacity * U::ARRAY_REFERENCE_INDEX_SCALE; }

            /**
             * Initializes or doubles table size.  If null, allocates in
             * accord with initial capacity target held in field threshold.
             * Otherwise, because we are using power-of-two expansion, the
             * elements from each bin must either stay at same index, or move
             * with a power of two offset in the new table.
             *
             * @return the table
             */
            ARRAY resize() {
                ARRAY oldTab = table;
                gint oldCap = (oldTab == null) ? 0 : capacity;
                gint oldThr = threshold;
                gint newCap, newThr = 0;
                if (oldCap > 0) {
                    if (oldCap >= MAXIMUM_CAPACITY) {
                        threshold = Integer::MAX_VALUE;
                        return oldTab;
                    } else if ((newCap = oldCap << 1) < MAXIMUM_CAPACITY && oldCap >= DEFAULT_CAPACITY)
                        newThr = oldThr << 1; // double threshold
                } else if (oldThr > 0) // initial capacity was placed in threshold
                    newCap = oldThr;
                else {               // zero initial threshold signifies using defaults
                    newCap = DEFAULT_CAPACITY;
                    newThr = (gint) (DEFAULT_FACTOR * DEFAULT_CAPACITY);
                }
                if (newThr == 0) {
                    gfloat ft = (gfloat) newCap * factor;
                    newThr = (newCap < MAXIMUM_CAPACITY && ft < (gfloat) MAXIMUM_CAPACITY ?
                              (gint) ft : Integer::MAX_VALUE);
                }
                threshold = newThr;
                ARRAY newTab = (ARRAY) U::allocateMemory(L(newCap));
                table = newTab;
                capacity = newCap;
                for (gint i = 0; i < newCap; ++i)
                    newTab[i] = null;

                if (oldTab != null) {
                    for (gint j = 0; j < oldCap; ++j) {
                        NODE e = {};
                        if ((e = oldTab[j]) != null) {
                            oldTab[j] = null;
                            if (e->next == null)
                                newTab[e->hash & (newCap - 1)] = e;
                            else if (Class<TreeNode>::hasInstance(e[0]))
                                ((TNODE) e)->split(*this, newTab, j, oldCap);
                            else { // preserve order
                                NODE loHead = null, loTail = null;
                                NODE hiHead = null, hiTail = null;
                                NODE next = {};
                                do {
                                    next = e->next;
                                    if ((e->hash & oldCap) == 0) {
                                        if (loTail == null)
                                            loHead = e;
                                        else
                                            loTail->next = e;
                                        loTail = e;
                                    } else {
                                        if (hiTail == null)
                                            hiHead = e;
                                        else
                                            hiTail->next = e;
                                        hiTail = e;
                                    }
                                } while ((e = next) != null);
                                if (loTail != null) {
                                    loTail->next = null;
                                    newTab[j] = loHead;
                                }
                                if (hiTail != null) {
                                    hiTail->next = null;
                                    newTab[j + oldCap] = hiHead;
                                }
                            }
                        }
                    }
                }
                U::freeMemory((glong) oldTab);
                return newTab;
            }

            /**
             * Replaces all linked nodes in bin at index for given hash unless
             * table is too small, in which case resizes instead.
             */
            void treeifyBin(ARRAY tab, gint hash) {
                gint n = {}, index = {};
                NODE e = {};
                if (tab == null || (n = capacity) < MIN_TREEIFY_CAPACITY)
                    resize();
                else if ((e = tab[index = (n - 1) & hash]) != null) {
                    TNODE hd = null, tl = null;
                    do {
                        TNODE p = replacementTNode(e, null);
                        if (tl == null)
                            hd = p;
                        else {
                            p->prev = tl;
                            tl->next = p;
                        }
                        tl = p;
                    } while ((e = e->next) != null);
                    if ((tab[index] = hd) != null)
                        hd->treeify(tab, capacity);
                }
            }

        public:

            /**
             * Copies all of the mappings from the specified map to this map.
             * These mappings will replace any mappings that this map had for
             * any of the keys currently in the specified map.
             *
             * @param m mappings to be stored in this map
             */
            void putAll(const Map<K, V> &m) override { addEntries(m, true); }

            /**
             * Copies all of the mappings from the specified map to this map.
             * These mappings will replace any mappings that this map had for
             * any of the keys currently in the specified map.
             *
             * @param m mappings to be stored in this map
             */
            template<class T = K, class U = V>
            void putAll(const Map<T, U> &m) { addEntries(m, true); }

            /**
             * Removes the mapping for the specified key from this map if present.
             *
             * @param  key key whose mapping is to be removed from the map
             * @return the previous value associated with <b> key</b> , or
             *         <b> null</b>  if there was no mapping for <b> key</b> .
             *         (A <b> null</b>  return can also indicate that the map
             *         previously associated <b> null</b>  with <b> key</b> .)
             */
            const V &remove(const K &key) override {
                NODE e = deleteEntry(HashMap<K, V>::hash(key), key, null, false, true);
                if (e == null)
                    KeyNotFoundException($(key)).throws(__trace("core.util.HashMap"));
                return e->value();
            }

        private:

            /**
             * Implements Map.remove and related methods.
             *
             * @param hash hash for key
             * @param key the key
             * @param value the value to match if matchValue, else ignored
             * @param matchValue if true only remove if value is equal
             * @param movable if false do not move other nodes while removing
             * @return the node, or null if none
             */
            NODE deleteEntry(gint hash, const K &key, const Object &value, gbool matchValue, gbool movable) {
                ARRAY tab = {};
                NODE p = {};
                gint n = {}, index = {};
                if ((tab = table) != null && (n = capacity) > 0 && (p = tab[index = (n - 1) & hash]) != null) {
                    NODE node = null, e = {};
                    KEY k = {};
                    VALUE v = {};
                    if (p->hash == hash && ((k = p->k) == &key || key.equals(k[0])))
                        node = p;
                    else if ((e = p->next) != null) {
                        if (Class<TreeNode>::hasInstance(p[0]))
                            node = ((TNODE) p)->entryOf(hash, key);
                        else {
                            do {
                                if (e->hash == hash && ((k = e->k) == &key || key.equals(k[0]))) {
                                    node = e;
                                    break;
                                }
                                p = e;
                            } while ((e = e->next) != null);
                        }
                    }
                    if (node != null && (!matchValue || (v = node->v) == &value || value.equals(v[0]))) {
                        if (Class<TreeNode>::hasInstance(node[0]))
                            ((TNODE) node)->deleteEntry(*this, tab, movable);
                        else if (node == p)
                            tab[index] = node->next;
                        else
                            p->next = node->next;
                        ++modNum;
                        --len;
                        afterNodeRemoval(node);
                        return node;
                    }
                }
                return null;
            }

        public:

            /**
             * Removes all of the mappings from this map.
             * The map will be empty after this call returns.
             */
            void clear() override {
                ARRAY tab;
                modNum++;
                if ((tab = table) != null && len > 0) {
                    len = 0;
                    for (gint i = 0; i < capacity; ++i)
                        tab[i] = null;
                }
            }

            /**
             * Returns <b> true</b>  if this map maps one or more keys to the
             * specified value.
             *
             * @param value value whose presence in this map is to be tested
             * @return <b> true</b>  if this map maps one or more keys to the
             *         specified value
             */
            gbool containsValue(const V &value) const override {
                ARRAY tab;
                VALUE v;
                if ((tab = table) != null && len > 0) {
                    for (gint i = 0; i < capacity; ++i) {
                        NODE e = tab[i];
                        for (; e != null; e = e->next) {
                            if ((v = e->v) == &value || value.equals(v[0]))
                                return true;
                        }
                    }
                }
                return false;
            }

            /**
             * Returns a <b style="color:orange;"> Set</b>  view of the keys contained in this map.
             * The set is backed by the map, so changes to the map are
             * reflected in the set, and vice-versa.  If the map is modified
             * while an iteration over the set is in progress (except through
             * the iterator's own <b> remove</b>  operation), the results of
             * the iteration are undefined.  The set supports element removal,
             * which removes the corresponding mapping from the map, via the
             * <b> Iterator.remove</b> , <b> Set.remove</b> ,
             * <b> removeAll</b> , <b> retainAll</b> , and <b> clear</b> 
             * operations.  It does not support the <b> add</b>  or <b> addAll</b> 
             * operations.
             *
             * @return a set view of the keys contained in this map
             */
            Set<K> &keySet() const override {
                if (Map<K, V>::kSet == null)
                    (KEYSET &) Map<K, V>::kSet = &U::createInstance<KeySet>((HashMap<K, V> &) *this);

                return Map<K, V>::kSet[0];
            }

        private:

            class KeySet CORE_FINAL : public Set<K> {
                HashMap<K, V> &root;

            public:
                CORE_EXPLICIT KeySet(HashMap<K, V> &root) : root(root) {}

                void forEach(const Consumer<K> &action) const override {
                    ARRAY tab;
                    if (root.len > 0 && (tab = root.table) != null) {
                        gint mc = root.modNum;
                        for (gint i = 0; i < root.capacity; ++i) {
                            NODE e = tab[i];
                            for (; e != null; e = e->next)
                                action.accept(e->key());
                        }
                        if (root.modNum != mc)
                            ConcurrentException().throws(__trace("core.util.HashMap.KeyItr"));
                    }
                }

                ReferenceArray<K> toArray() const override {
                    ReferenceArray<K> array = ReferenceArray<K>(root.len);
                    ARRAY tab;
                    gint idx = 0;
                    if (root.len > 0 && (tab = root.table) != null) {
                        for (gint i = 0; i < root.capacity; ++i) {
                            NODE e = tab[i];
                            for (; e != null; e = e->next) {
                                array.value[idx++] = e->k;
                            }
                        }
                    }
                    return array;
                }

                gint size() const override { return root.size(); }

                gbool contains(const K &o) const override { return root.containsKey(o); }

                Iterator<const K> &iterator() const override {
                    return U::createInstance<KeyItr>((HashMap<K, V> &) root);
                }

                gbool remove(const K &o) override {
                    return root.deleteEntry(HashMap<K, V>::hash(o), o, null, false, true) != null;
                }

                void clear() override { root.clear(); }
            };

        public:

            /**
             * Returns a <b style="color:orange;"> Collection</b>  view of the values contained in this map.
             * The collection is backed by the map, so changes to the map are
             * reflected in the collection, and vice-versa.  If the map is
             * modified while an iteration over the collection is in progress
             * (except through the iterator's own <b> remove</b>  operation),
             * the results of the iteration are undefined.  The collection
             * supports element removal, which removes the corresponding
             * mapping from the map, via the <b> Iterator.remove</b> ,
             * <b> Collection.remove</b> , <b> removeAll</b> ,
             * <b> retainAll</b>  and <b> clear</b>  operations.  It does not
             * support the <b> add</b>  or <b> addAll</b>  operations.
             *
             * @return a view of the values contained in this map
             */
            Collection<V> &values() const override {
                if (Map<K, V>::vCollection == null)
                    (VALUES &) Map<K, V>::vCollection = &U::createInstance<Values>((HashMap<K, V> &) *this);
                return Map<K, V>::vCollection[0];
            }

        private:
            class Values CORE_FINAL : public Collection<V> {
                HashMap<K, V> &root;

            public:
                CORE_EXPLICIT Values(HashMap<K, V> &root) : root(root) {}

                gint size() const override { return root.size(); }

                gbool contains(const V &o) const override { return root.containsValue(o); }

                Iterator<const V> &iterator() const override {
                    return U::createInstance<ValueItr>((HashMap<K, V> &) root);
                }

                void forEach(const Consumer<V> &action) const override {
                    ARRAY tab;
                    if (root.len > 0 && (tab = root.table) != null) {
                        gint mc = root.modNum;
                        for (gint i = 0; i < root.capacity; ++i) {
                            NODE e = tab[i];
                            for (; e != null; e = e->next)
                                action.accept(e->value());
                        }
                        if (root.modNum != mc)
                            ConcurrentException().throws(__trace("core.util.HashMap.KeyItr"));
                    }
                }

                void clear() override { root.clear(); }

                ReferenceArray<V> toArray() const override {
                    ReferenceArray<V> array = ReferenceArray<V>(root.len);
                    ARRAY tab;
                    gint idx = 0;
                    if (root.len > 0 && (tab = root.table) != null) {
                        for (gint i = 0; i < root.capacity; ++i) {
                            NODE e = tab[i];
                            for (; e != null; e = e->next) {
                                array.value[idx++] = e->v;
                            }
                        }
                    }
                    return array;
                }
            };

        public:

            /**
             * Returns a <b style="color:orange;"> Set</b>  view of the mappings contained in this map.
             * The set is backed by the map, so changes to the map are
             * reflected in the set, and vice-versa.  If the map is modified
             * while an iteration over the set is in progress (except through
             * the iterator's own <b> remove</b>  operation, or through the
             * <b> setValue</b>  operation on a map entry returned by the
             * iterator) the results of the iteration are undefined.  The set
             * supports element removal, which removes the corresponding
             * mapping from the map, via the <b> Iterator.remove</b> ,
             * <b> Set.remove</b> , <b> removeAll</b> , <b> retainAll</b>  and
             * <b> clear</b>  operations.  It does not support the
             * <b> add</b>  or <b> addAll</b>  operations.
             *
             * @return a set view of the mappings contained in this map
             */
            Set<ExEntry<K, V>> &entrySet() const override {
                if (eSet == null)
                    (ENTRIES &) eSet = &U::createInstance<EntrySet>((HashMap<K, V> &) *this);
                return eSet[0];
            }

        private:
            class EntrySet CORE_FINAL : public Set<ExEntry<K, V>> {
                HashMap<K, V> &root;
            public:
                CORE_EXPLICIT EntrySet(HashMap<K, V> &root) : root(root) {}

                gint size() const override { return root.size(); }

                void clear() override { root.clear(); }

                Iterator<const ExEntry<K, V>> &iterator() const override {
                    return U::createInstance<EntryItr>((HashMap<K, V> &) root);
                }

                gbool contains(const ExEntry<K, V> &o) const override {
                    NODE e = root.entryOf(o.key());
                    if (e == null)
                        return false;
                    return Object::equals(o.value(), e->value());
                }

                gbool remove(const ExEntry<K, V> &o) override {
                    return root.deleteEntry(HashMap<K, V>::hash(o.key()), o.key(), o.value(), true, true) != null;
                }

                void forEach(const Consumer<ExEntry<K, V>> &action) const override {
                    ARRAY tab;
                    if (root.len > 0 && (tab = root.table) != null) {
                        gint mc = root.modNum;
                        for (gint i = 0; i < root.capacity; ++i) {
                            NODE e = tab[i];
                            for (; e != null; e = e->next)
                                action.accept(e[0]);
                        }
                        if (root.modNum != mc)
                            ConcurrentException().throws(__trace("core.util.HashMap.KeyItr"));
                    }
                }

                ReferenceArray<ExEntry<K, V>> toArray() const override {
                    ReferenceArray<ExEntry<K, V>> array = ReferenceArray<ExEntry<K, V>>(root.len);
                    ARRAY tab;
                    gint idx = 0;
                    if (root.len > 0 && (tab = root.table) != null) {
                        for (gint i = 0; i < root.capacity; ++i) {
                            NODE e = tab[i];
                            for (; e != null; e = e->next) {
                                array.value[idx++] = e;
                            }
                        }
                    }
                    return array;
                }
            };

        public:
            V &getOrDefault(const K &key, const V &defaultValue) override {
                NODE e = {};
                return (e = entryOf(key)) != null ? e->value() : U::copyInstance(defaultValue, true);
            }

            const V &getOrDefault(const K &key, const V &defaultValue) const override {
                NODE e = {};
                return (e = entryOf(key)) != null ? e->value() : U::copyInstance(defaultValue, true);
            }

            gbool remove(const K &key, const V &value) override {
                return deleteEntry(HashMap<K, V>::hash(key), key, value, true, true) != null;
            }

            gbool replace(const K &key, const V &oldValue, const V &newValue) override {
                NODE e = {};
                VALUE v = {};
                if ((e = entryOf(key)) != null && ((v = e->v) == &oldValue || v->equals(oldValue))) {
                    e->v = &U::copyInstance(newValue, true);
                    afterNodeAccess(e);
                    return true;
                }
                return false;
            }

            const V &replace(const K &key, const V &value) override {
                NODE e = {};
                if ((e = entryOf(key)) != null) {
                    VALUE oldValue = e->v;
                    e->v = &U::copyInstance(value, true);
                    afterNodeAccess(e);
                    return oldValue[0];
                }
                KeyNotFoundException($(key)).throws(__trace("core.util.HashMap"));
            }

            void forEach(BiConsumer<K, V> action) override {
                ARRAY tab = {};
                if (len > 0 && (tab = table) != null) {
                    gint mc = modNum;
                    for (gint i = 0; i < capacity; ++i) {
                        NODE e = tab[i];
                        for (; e != null; e = e->next) {
//                            action.accept(e->key(), e->value());
                        }
                    }
                    if (modNum != mc)
                        ConcurrentException().throws(__trace("core.util.HashMap"));
                }
            }

            void replaceAll(BiFunction<K, V &, V> function) override {
                ARRAY tab = {};
                if (len > 0 && (tab = table) != null) {
                    gint mc = modNum;
                    for (gint i = 0; i < capacity; ++i) {
                        NODE e = tab[i];
                        for (; e != null; e = e->next) {
//                            e->set(function.apply(e->key(), e->value()));
                        }
                    }
                    if (modNum != mc)
                        ConcurrentException().throws(__trace("core.util.HashMap"));
                }
            }

            /**
             * Returns a shallow copy of this <b> HashMap</b>  instance: the keys and
             * values themselves are not cloned.
             *
             * @return a shallow copy of this map
             */
            Object &clone() const override { return U::createInstance<HashMap<K, V>>(*this); }

        private:
            template<class T>
            interface AbstractItr : public Iterator<const T> {
                NODE next = {};
                NODE current = {};
                gint modNum = {};
                gint index = {};
                HashMap<K, V> &root;

                CORE_EXPLICIT AbstractItr(HashMap<K, V> &root) : root(root) {
                    modNum = root.modNum;
                    ARRAY tab = root.table;
                    current = next = null;
                    index = 0;
                    if (tab != null && root.len > 0) {
                        // advance to the first entry
                        do {} while (index < root.capacity && (next = tab[index++]) == null);
                    }
                }

                gbool hasNext() const override {
                    return next != null;
                }

                NODE nextEntry() {
                    ARRAY tab = {};
                    NODE e = next;
                    if (modNum != root.modNum)
                        ConcurrentException().throws(__trace("core.util.HashMap.AbstractItr"));
                    if (e == null)
                        NoSuchItemException().throws(__trace("core.util.HashMap.AbstractItr"));
                    if ((next = (current = e)->next) == null && (tab = root.table) != null) {
                        do {} while (index < root.capacity && (next = tab[index++]) == null);
                    }
                    return e;
                }

                void remove() override {
                    NODE p = current;
                    if (p == null)
                        StateException().throws(__trace("core.util.HashMap.AbstractItr"));
                    if (modNum != root.modNum)
                        ConcurrentException().throws(__trace("core.util.HashMap.AbstractItr"));
                    current = null;
                    root.deleteEntry(p->hash, p->key(), null, false, false);
                    modNum = root.modNum;
                }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<AbstractItr<T>>::hasInstance(o))
                        return false;
                    AbstractItr<T> &it = (AbstractItr<T> &) o;
                    return &root == &it.root && next == it.next;
                }
            };


            class KeyItr CORE_FINAL : public AbstractItr<K> {
            public:
                CORE_EXPLICIT KeyItr(HashMap<K, V> &root) : AbstractItr<K>(root) {}

                const K &next() override { return KeyItr::nextEntry()->key(); }

                Object &clone() const override { return U::createInstance<KeyItr>(*this); }
            };


            class ValueItr CORE_FINAL : public AbstractItr<V> {
            public:
                CORE_EXPLICIT ValueItr(HashMap<K, V> &root) : AbstractItr<V>(root) {}

                const V &next() override { return ValueItr::nextEntry()->value(); }

                Object &clone() const override { return U::createInstance<ValueItr>(*this); }
            };


            class EntryItr CORE_FINAL : public AbstractItr<ExEntry<K, V>> {
            public:
                CORE_EXPLICIT EntryItr(HashMap<K, V> &root) : AbstractItr<ExEntry<K, V>>(root) {}

                const ExEntry<K, V> &next() override { return EntryItr::nextEntry()[0]; }

                Object &clone() const override { return U::createInstance<EntryItr>(*this); }
            };


            /*
             * The following package-protected methods are designed to be
             * overridden by LinkedHashMap, but not by any other subclass.
             * Nearly all other internal methods are also package-protected
             * but are declared final, so can be used by LinkedHashMap, view
             * classes, and HashSet.
             */

            // Create a regular (non-tree) node
            virtual NODE newNode(gint hash, const K &key, const V &value, NODE next) {
                K &keyCopy = U::copyInstance(key, true);
                V &valueCopy = U::copyInstance(value, true);
                return &U::createInstance<Node>(hash, keyCopy, valueCopy, next);
            }

            // For conversion from TreeNodes to plain nodes
            virtual NODE replacementNode(NODE p, NODE next) {
                K &keyCopy = p->k[0];
                V &valueCopy = p->v[0];
                return &U::createInstance<Node>(p->hash, keyCopy, valueCopy, next);
            }

            // Create a tree bin node
            virtual TNODE newTNode(gint hash, const K &key, const V &value, NODE next) {
                K &keyCopy = U::copyInstance(key, true);
                V &valueCopy = U::copyInstance(value, true);
                return &U::createInstance<TreeNode>(hash, keyCopy, valueCopy, next);
            }

            // For treeifyBin
            virtual TNODE replacementTNode(NODE p, NODE next) {
                K &keyCopy = p->k[0];
                V &valueCopy = p->v[0];
                return &U::createInstance<TreeNode>(p->hash, keyCopy, valueCopy, next);
            }

            // Callbacks to allow LinkedHashMap post-actions
            virtual void afterNodeAccess(NODE p) {}

            virtual void afterNodeInsertion(gbool evict) {}

            virtual void afterNodeRemoval(NODE p) {}

            /**
             * HashMap.Node subclass for normal LinkedHashMap entries.
             */
            interface LinkedEntry : public Node {
                LNODE after = {};
                LNODE before = {};

                CORE_EXPLICIT LinkedEntry(gint hash, K &k, V &v, NODE next) : Node(hash, k, v, next) {}
            };

            // Red-black mechanics
            static CORE_FAST gbool RED = false;
            static CORE_FAST gbool BLACK = true;

            /**
             * Entry for Tree bins. Extends LinkedHashMap.Entry (which in turn
             * extends Node) so can be used as extension of either regular or
             * linked node.
             */
            interface TreeNode CORE_FINAL : public LinkedEntry {
                TNODE parent = {};
                TNODE left = {};
                TNODE right = {};
                TNODE prev = {};
                gbool color = BLACK;

                CORE_EXPLICIT TreeNode(gint hash, K &k, V &v, NODE next) : LinkedEntry(hash, k, v, next) {}

                /**
                 * Returns root of tree containing this node.
                 */
                TNODE root() {
                    for (TNODE r = this, p;;) {
                        if ((p = r->parent) == null)
                            return r;
                        r = p;
                    }
                }

                /**
                 * Ensures that the given root is the first node of its bin.
                 */
                static void toFront(ARRAY tab, TNODE root, gint capacity) {
                    gint n = {};
                    if (root != null && tab != null && (n = capacity) > 0) {
                        gint index = (n - 1) & root->hash;
                        TNODE first = (TNODE) tab[index];
                        if (root != first) {
                            NODE rn = {};
                            tab[index] = root;
                            TNODE rp = root->prev;
                            if ((rn = root->next) != null)
                                ((TNODE) rn)->prev = rp;
                            if (rp != null)
                                rp->next = rn;
                            if (first != null)
                                first->prev = root;
                            root->next = first;
                            root->prev = null;
                        }
                        CORE_ASSERT(checkInvariants(root), "core.util.HashMap.TreeNode");
                    }
                }

                /**
                 * Finds the node starting at root p with the given hash and key.
                 * The kc argument caches comparableClassFor(key) upon first use
                 * comparing keys.
                 */
                TNODE find(gint h, const K &k) {
                    TNODE p = this;
                    do {
                        gint ph, dir;
                        KEY pk;
                        TNODE pl = p->left, pr = p->right, q;
                        if ((ph = p->hash) > h)
                            p = pl;
                        else if (ph < h)
                            p = pr;
                        else if ((pk = p->k) == &k || k.equals(pk[0]))
                            return p;
                        else if (pl == null)
                            p = pr;
                        else if (pr == null)
                            p = pl;
                        else if ((dir = compareComparables(k, pk[0])) != 0)
                            p = (dir < 0) ? pl : pr;
                        else if ((q = pr->find(h, k)) != null)
                            return q;
                        else
                            p = pl;
                    } while (p != null);
                    return null;
                }

                /**
                 * Calls find for root node.
                 */
                TNODE entryOf(gint h, const K &k) {
                    return ((parent != null) ? root() : this)->find(h, k);
                }

                /**
                 * Tie-breaking utility for ordering insertions when equal
                 * hash Codes and non-comparable. We don't require a total
                 * order, just a consistent insertion rule to maintain
                 * equivalence across rebalancings. Tie-breaking further than
                 * necessary simplifies testing a bit.
                 */
                static gint tieBreakOrder(const K &a, const K &b) {
                    gint d;
                    if (null == a || null == b || (d = a.classname().compareTo(b.classname())) == 0)
                        d = (Object::identityHash(a) <= Object::identityHash(b) ? -1 : 1);
                    return d;
                }

                /**
                 * Forms tree of the nodes linked from this node.
                 */
                void treeify(ARRAY tab, gint cap) {
                    TNODE root = null;
                    for (TNODE x = this, next; x != null; x = next) {
                        next = (TNODE) x->next;
                        x->left = x->right = null;
                        if (root == null) {
                            x->parent = null;
                            x->color = RED;
                            root = x;
                        } else {
                            KEY k = x->k;
                            gint h = x->hash;
                            for (TNODE p = root;;) {
                                gint dir, ph;
                                KEY pk = p->k;
                                if ((ph = p->hash) > h)
                                    dir = -1;
                                else if (ph < h)
                                    dir = 1;
                                else if ((dir = compareComparables(k[0], pk[0])) == 0)
                                    dir = tieBreakOrder(k[0], pk[0]);

                                TNODE xp = p;
                                if ((p = (dir <= 0) ? p->left : p->right) == null) {
                                    x->parent = xp;
                                    if (dir <= 0)
                                        xp->left = x;
                                    else
                                        xp->right = x;
                                    root = balanceInsertion(root, x);
                                    break;
                                }
                            }
                        }
                    }
                    toFront(tab, root, cap);
                }

                /**
                 * Returns a list of non-TreeNodes replacing those linked from
                 * this node.
                 */
                NODE untreeify(HashMap<K, V> &map) {
                    NODE hd = null, tl = null;
                    for (NODE q = this; q != null; q = q->next) {
                        NODE p = map.replacementNode(q, null);
                        if (tl == null)
                            hd = p;
                        else
                            tl->next = p;
                        tl = p;
                    }
                    return hd;
                }

                /**
                 * Tree version of putVal.
                 */
                TNODE addEntry(HashMap<K, V> &map, ARRAY tab, gint h, const K &k, const V &v) {
                    gbool searched = false;
                    TNODE root = (parent != null) ? this->root() : this;
                    for (TNODE p = root;;) {
                        gint dir, ph;
                        KEY pk;
                        if ((ph = p->hash) > h)
                            dir = -1;
                        else if (ph < h)
                            dir = 1;
                        else if ((pk = p->k) == &k || k.equals(pk[0]))
                            return p;
                        else if ((dir = compareComparables(k, pk[0])) == 0) {
                            if (!searched) {
                                TNODE q, ch;
                                searched = true;
                                if (((ch = p->left) != null && (q = ch->find(h, k)) != null) ||
                                    ((ch = p->right) != null && (q = ch->find(h, k)) != null))
                                    return q;
                            }
                            dir = tieBreakOrder(k, pk[0]);
                        }

                        TNODE xp = p;
                        if ((p = (dir <= 0) ? p->left : p->right) == null) {
                            NODE xpn = xp->next;
                            TNODE x = map.newTNode(h, k, v, xpn);
                            if (dir <= 0)
                                xp->left = x;
                            else
                                xp->right = x;
                            xp->next = x;
                            x->parent = x->prev = xp;
                            if (xpn != null)
                                ((TNODE) xpn)->prev = x;
                            toFront(tab, balanceInsertion(root, x), map.capacity);
                            return null;
                        }
                    }
                }

                /**
                 * Removes the given node, that must be present before this call.
                 * This is messier than typical red-black deletion code because we
                 * cannot swap the contents of an interior node with a leaf
                 * successor that is pinned by "next" pointers that are accessible
                 * independently during traversal. So instead we swap the tree
                 * linkages. If the current tree appears to have too few nodes,
                 * the bin is converted back to a plain bin. (The test triggers
                 * somewhere between 2 and 6 nodes, depending on tree structure).
                 */
                void deleteEntry(HashMap<K, V> &map, ARRAY tab, gbool movable) {
                    gint n;
                    if (tab == null || (n = map.capacity) == 0)
                        return;
                    gint index = (n - 1) & Node::hash;
                    TNODE first = (TNODE) tab[index], root = first, rl;
                    TNODE succ = (TNODE) this->next, pred = prev;
                    if (pred == null)
                        tab[index] = first = succ;
                    else
                        pred->next = succ;
                    if (succ != null)
                        succ->prev = pred;
                    if (first == null)
                        return;
                    if (root->parent != null)
                        root = root->root();
                    if (root == null || (movable && (root->right == null ||
                                                     (rl = root->left) == null || rl->left == null))) {
                        tab[index] = first->untreeify(map);  // too small
                        return;
                    }
                    TNODE p = this, pl = left, pr = right, replacement;
                    if (pl != null && pr != null) {
                        TNODE s = pr, sl;
                        while ((sl = s->left) != null) // find successor
                            s = sl;
                        gbool c = s->color;
                        s->color = p->color;
                        p->color = c; // swap colors
                        TNODE sr = s->right;
                        TNODE pp = p->parent;
                        if (s == pr) { // p was s's direct parent
                            p->parent = s;
                            s->right = p;
                        } else {
                            TNODE sp = s->parent;
                            if ((p->parent = sp) != null) {
                                if (s == sp->left)
                                    sp->left = p;
                                else
                                    sp->right = p;
                            }
                            if ((s->right = pr) != null)
                                pr->parent = s;
                        }
                        p->left = null;
                        if ((p->right = sr) != null)
                            sr->parent = p;
                        if ((s->left = pl) != null)
                            pl->parent = s;
                        if ((s->parent = pp) == null)
                            root = s;
                        else if (p == pp->left)
                            pp->left = s;
                        else
                            pp->right = s;
                        if (sr != null)
                            replacement = sr;
                        else
                            replacement = p;
                    } else if (pl != null)
                        replacement = pl;
                    else if (pr != null)
                        replacement = pr;
                    else
                        replacement = p;
                    if (replacement != p) {
                        TNODE pp = replacement->parent = p->parent;
                        if (pp == null)
                            (root = replacement)->color = BLACK;
                        else if (p == pp->left)
                            pp->left = replacement;
                        else
                            pp->right = replacement;
                        p->left = p->right = p->parent = null;
                    }

                    TNODE r = p->color == RED ? root : balanceDeletion(root, replacement);

                    if (replacement == p) {  // detach
                        TNODE pp = p->parent;
                        p->parent = null;
                        if (pp != null) {
                            if (p == pp->left)
                                pp->left = null;
                            else if (p == pp->right)
                                pp->right = null;
                        }
                    }
                    if (movable)
                        toFront(tab, r, map.capacity);
                }

                /**
                 * Splits nodes in a tree bin into lower and upper tree bins,
                 * or untreeifies if now too small. Called only from resize;
                 * see above discussion about split bits and indices.
                 *
                 * @param map the map
                 * @param tab the table for recording bin heads
                 * @param index the index of the table being split
                 * @param bit the bit of hash to split on
                 */
                void split(HashMap<K, V> &map, ARRAY tab, gint index, gint bit) {
                    TNODE b = this;
                    // Relink into lo and hi lists, preserving order
                    TNODE loHead = null, loTail = null;
                    TNODE hiHead = null, hiTail = null;
                    gint lc = 0, hc = 0;
                    for (TNODE e = b, next; e != null; e = next) {
                        next = (TNODE) e->next;
                        e->next = null;
                        if ((e->hash & bit) == 0) {
                            if ((e->prev = loTail) == null)
                                loHead = e;
                            else
                                loTail->next = e;
                            loTail = e;
                            ++lc;
                        } else {
                            if ((e->prev = hiTail) == null)
                                hiHead = e;
                            else
                                hiTail->next = e;
                            hiTail = e;
                            ++hc;
                        }
                    }

                    if (loHead != null) {
                        if (lc <= UNTREEIFY_THRESHOLD)
                            tab[index] = loHead->untreeify(map);
                        else {
                            tab[index] = loHead;
                            if (hiHead != null) // (else is already treeified)
                                loHead->treeify(tab, map.capacity);
                        }
                    }
                    if (hiHead != null) {
                        if (hc <= UNTREEIFY_THRESHOLD)
                            tab[index + bit] = hiHead->untreeify(map);
                        else {
                            tab[index + bit] = hiHead;
                            if (loHead != null)
                                hiHead->treeify(tab, map.capacity);
                        }
                    }
                }

                // Red-black tree methods, all adapted from CLR


                static TNODE rotateLeft(TNODE root, TNODE p) {
                    TNODE r, pp, rl;
                    if (p != null && (r = p->right) != null) {
                        if ((rl = p->right = r->left) != null)
                            rl->parent = p;
                        if ((pp = r->parent = p->parent) == null)
                            (root = r)->color = BLACK;
                        else if (pp->left == p)
                            pp->left = r;
                        else
                            pp->right = r;
                        r->left = p;
                        p->parent = r;
                    }
                    return root;
                }

                static TNODE rotateRight(TNODE root, TNODE p) {
                    TNODE l, pp, lr;
                    if (p != null && (l = p->left) != null) {
                        if ((lr = p->left = l->right) != null)
                            lr->parent = p;
                        if ((pp = l->parent = p->parent) == null)
                            (root = l)->color = BLACK;
                        else if (pp->right == p)
                            pp->right = l;
                        else
                            pp->left = l;
                        l->right = p;
                        p->parent = l;
                    }
                    return root;
                }

                static TNODE balanceInsertion(TNODE root, TNODE x) {
                    x->color = RED;
                    for (TNODE xp, xpp, xppl, xppr;;) {
                        if ((xp = x->parent) == null) {
                            x->color = BLACK;
                            return x;
                        } else if ((xp->color == BLACK) || (xpp = xp->parent) == null)
                            return root;
                        if (xp == (xppl = xpp->left)) {
                            if ((xppr = xpp->right) != null && xppr->color == RED) {
                                xppr->color = BLACK;
                                xp->color = BLACK;
                                xpp->color = RED;
                                x = xpp;
                            } else {
                                if (x == xp->right) {
                                    root = rotateLeft(root, x = xp);
                                    xpp = (xp = x->parent) == null ? null : xp->parent;
                                }
                                if (xp != null) {
                                    xp->color = BLACK;
                                    if (xpp != null) {
                                        xpp->color = RED;
                                        root = rotateRight(root, xpp);
                                    }
                                }
                            }
                        } else {
                            if (xppl != null && xppl->color) {
                                xppl->color = BLACK;
                                xp->color = BLACK;
                                xpp->color = RED;
                                x = xpp;
                            } else {
                                if (x == xp->left) {
                                    root = rotateRight(root, x = xp);
                                    xpp = (xp = x->parent) == null ? null : xp->parent;
                                }
                                if (xp != null) {
                                    xp->color = BLACK;
                                    if (xpp != null) {
                                        xpp->color = RED;
                                        root = rotateLeft(root, xpp);
                                    }
                                }
                            }
                        }
                    }
                }

                static TNODE balanceDeletion(TNODE root, TNODE x) {
                    for (TNODE xp, xpl, xpr;;) {
                        if (x == null || x == root)
                            return root;
                        else if ((xp = x->parent) == null) {
                            x->color = BLACK;
                            return x;
                        } else if (x->color == RED) {
                            x->color = BLACK;
                            return root;
                        } else if ((xpl = xp->left) == x) {
                            if ((xpr = xp->right) != null && xpr->color == RED) {
                                xpr->color = BLACK;
                                xp->color = RED;
                                root = rotateLeft(root, xp);
                                xpr = (xp = x->parent) == null ? null : xp->right;
                            }
                            if (xpr == null)
                                x = xp;
                            else {
                                TNODE sl = xpr->left, sr = xpr->right;
                                if ((sr == null || sr->color == BLACK) && (sl == null || sl->color == BLACK)) {
                                    xpr->color = RED;
                                    x = xp;
                                } else {
                                    if (sr == null || sr->color == BLACK) {
                                        if (sl != null)
                                            sl->color = BLACK;
                                        xpr->color = RED;
                                        root = rotateRight(root, xpr);
                                        xpr = (xp = x->parent) == null ?
                                              null : xp->right;
                                    }
                                    if (xpr != null) {
                                        xpr->color = (xp == null) ? BLACK : xp->color;
                                        if ((sr = xpr->right) != null)
                                            sr->color = BLACK;
                                    }
                                    if (xp != null) {
                                        xp->color = BLACK;
                                        root = rotateLeft(root, xp);
                                    }
                                    x = root;
                                }
                            }
                        } else { // symmetric
                            if (xpl != null && xpl->color == RED) {
                                xpl->color = BLACK;
                                xp->color = RED;
                                root = rotateRight(root, xp);
                                xpl = (xp = x->parent) == null ? null : xp->left;
                            }
                            if (xpl == null)
                                x = xp;
                            else {
                                TNODE sl = xpl->left, sr = xpl->right;
                                if ((sl == null || sl->color == BLACK) && (sr == null || sr->color == BLACK)) {
                                    xpl->color = RED;
                                    x = xp;
                                } else {
                                    if (sl == null || sl->color == BLACK) {
                                        if (sr != null)
                                            sr->color = BLACK;
                                        xpl->color = RED;
                                        root = rotateLeft(root, xpl);
                                        xpl = (xp = x->parent) == null ? null : xp->left;
                                    }
                                    if (xpl != null) {
                                        xpl->color = (xp == null) ? BLACK : xp->color;
                                        if ((sl = xpl->left) != null)
                                            sl->color = BLACK;
                                    }
                                    if (xp != null) {
                                        xp->color = BLACK;
                                        root = rotateRight(root, xp);
                                    }
                                    x = root;
                                }
                            }
                        }
                    }
                }

                /**
                 * Recursive invariant check
                 */
                static gbool checkInvariants(TNODE t) {
                    TNODE tp = t->parent, tl = t->left, tr = t->right,
                            tb = t->prev, tn = (TNODE) t->next;
                    if (tb != null && tb->next != t)
                        return false;
                    if (tn != null && tn->prev != t)
                        return false;
                    if (tp != null && t != tp->left && t != tp->right)
                        return false;
                    if (tl != null && (tl->parent != t || tl->hash > t->hash))
                        return false;
                    if (tr != null && (tr->parent != t || tr->hash < t->hash))
                        return false;
                    if (t->color == RED && tl != null && tl->color == RED && tr != null && tr->color == RED)
                        return false;
                    if (tl != null && !checkInvariants(tl))
                        return false;
                    if (tr != null && !checkInvariants(tr))
                        return false;
                    return true;
                }

            };
        };

#if CORE_TEMPLATE_TYPE_DEDUCTION
        template<class K, class V>
        HashMap(Map<K, V>) -> HashMap<K, V>;

        template<class K = Object, class V = Object>
        HashMap(gint, gfloat) -> HashMap<K, V>;

        template<class K = Object, class V = Object>
        HashMap(gint) -> HashMap<K, V>;

        template<class K = Object, class V = Object>
        HashMap() -> HashMap<K, V>;
#endif

    }
} // core

#endif //CORE23_HASHMAP_H
