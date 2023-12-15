//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_MAP_H
#define CORE23_MAP_H

#include <core/KeyNotFoundException.h>
#include <core/util/Set.h>
#include <core/util/ConcurrentException.h>
#include <core/util/function/BiConsumer.h>
#include <core/util/function/BiFunction.h>

namespace core {
    namespace util {

        /**
         * An object that maps keys to values.  A map cannot contain duplicate keys;
         * each key can map to at most one value.
         *
         * <p>This interface takes the place of the <b> Dictionary</b> class, which
         * was a totally abstract class rather than an interface.
         *
         * <p>The <b> Map</b> interface provides three <i>collection views</i>, which
         * allow a map's contents to be viewed as a set of keys, collection of values,
         * or set of key-value mappings.  The <i>order</i> of a map is defined as
         * the order in which the iterators on the map's collection views return their
         * elements.  Some map implementations, like the <b> TreeMap</b> class, make
         * specific guarantees as to their order; others, like the <b> HashMap</b>
         * class, do not.
         *
         * <p>Note: great care must be exercised if mutable objects are used as map
         * keys.  The behavior of a map is not specified if the value of an object is
         * changed in a manner that affects <b> equals</b> comparisons while the
         * object is a key in the map.  A special case of this prohibition is that it
         * is not permissible for a map to contain itself as a key.  While it is
         * permissible for a map to contain itself as a value, extreme caution is
         * advised: the <b> equals</b> and <b> hashCode</b> methods are no longer
         * well defined on such a map.
         *
         * <p>All general-purpose map implementation classes should provide two
         * "standard" constructors: a void (no arguments) constructor which creates an
         * empty map, and a constructor with a single argument of type <b> Map</b>,
         * which creates a new map with the same key-value mappings as its argument.
         * In effect, the latter constructor allows the user to copy any map,
         * producing an equivalent map of the desired class.  There is no way to
         * enforce this recommendation (as interfaces cannot contain constructors) but
         * all of the general-purpose map implementations in the JDK comply.
         *
         * <p>The "destructive" methods contained in this interface, that is, the
         * methods that modify the map on which they operate, are specified to throw
         * <b> UnsupportedMethodException</b> if this map does not support the
         * operation.  If this is the case, these methods may, but are not required
         * to, throw an <b> UnsupportedMethodException</b> if the invocation would
         * have no effect on the map.  For example, invoking the <b style="color: orange;"> addAll(Map)</b>
         * method on an unmodifiable map may, but is not required to, throw the
         * exception if the map whose mappings are to be "superimposed" is empty.
         *
         * <p>Some map implementations have restrictions on the keys and values they
         * may contain.  Attempting
         * to insert an ineligible key or value throws an unchecked exception,
         * typically <b> CastException</b>.
         * Attempting to query the presence of an ineligible key or value may throw an
         * exception, or it may simply return false; some implementations will exhibit
         * the former behavior and some will exhibit the latter.  More generally,
         * attempting an operation on an ineligible key or value whose completion
         * would not result in the insertion of an ineligible element into the map may
         * throw an exception or it may succeed, at the option of the implementation.
         * Such exceptions are marked as "optional" in the specification for this
         * interface.
         *
         * <p>Many methods in Collections Framework interfaces are defined
         * in terms of the <b style="color: orange;"> equals</b> method.  For
         * example, the specification for the <b style="color: orange;">
         * containsKey(Object key)</b> method says: "returns <b> true</b> if and
         * only if this map contains a mapping for a key <b> k</b> such that
         * <b>key.equals(k)</b>." This specification should
         * <i>not</i> be construed to imply that invoking <b> Map.containsKey</b>
         * with a argument <b> key</b> will cause <b> key.equals(k)</b> to
         * be invoked for any key <b> k</b>.  Implementations are free to
         * implement optimizations whereby the <b> equals</b> invocation is avoided,
         * for example, by first comparing the hash codes of the two keys.
         * More generally, implementations of the various Collections Framework interfaces are
         * free to take advantage of the specified behavior of underlying <b style="color: orange;">
         * Object</b> methods wherever the implementor deems it appropriate.
         *
         * <p>Some map operations which perform recursive traversal of the map may fail
         * with an exception for self-referential instances where the map directly or
         * indirectly contains itself. This includes the <b> clone()</b>,
         * <b> equals()</b>, <b> hashCode()</b> and <b> toString()</b> methods.
         * Implementations may optionally handle the self-referential scenario, however
         * most current implementations do not do so.
         *
         * <h2><a id="unmodifiable">Unmodifiable Maps</a></h2>
         * <p>The <b style="color: orange;"> Map.of</b>,
         * <b style="color: orange;"> Map.ofEntries</b>, and
         * <b style="color: orange;"> Map.copyOf</b>
         * static factory methods provide a convenient way to create unmodifiable maps.
         * The <b> Map</b>
         * instances created by these methods have the following characteristics:
         *
         * <ul>
         * <li>They are <a href=""><i>unmodifiable</i></a>. Keys and values
         * cannot be added, removed, or updated. Calling any mutator method on the Map
         * will always cause <b> UnsupportedMethodException</b> to be thrown.
         * <li>They are serializable if all keys and values are serializable.
         * <li>They reject duplicate keys at creation time. Duplicate keys
         * passed to a static factory method result in <b> ArgumentException</b>.
         * <li>The iteration order of mappings is unspecified and is subject to change.
         * <li>They are <a href="">value-based</a>.
         * Programmers should treat instances that are <b style="color: green;"> equal</b>
         * as interchangeable and should not use them for synchronization, or
         * unpredictable behavior may occur. For example, in a future release,
         * synchronization may fail. Callers should make no assumptions
         * about the identity of the returned instances. Factories are free to
         * create new instances or reuse existing ones.
         * <li>They are serialized as specified on the <a href="">Serialized Form</a>
         * page.
         * </ul>
         *
         * @param K the type of keys maintained by this map
         * @param V the type of mapped values
         *
         * @author  Brunshweeck Tazeussong
         * @see HashMap
         * @see TreeMap
         * @see Hashtable
         * @see Collection
         * @see Set
         */
        template<class K, class V>
        interface Map : public Object {

            class Entry;

            /**
             * Returns the number of key-value mappings in this map.  If the
             * map contains more than <b>Integer.MAX_VALUE</b> elements, returns
             * <b>Integer.MAX_VALUE</b>.
             */
            virtual gint size() const = 0;

            /**
             * Returns <b>true</b> if this map contains no key-value mappings.
             */
            inline gbool isEmpty() const { return size() == 0; }

            /**
             * Returns <b>true</b> if this map contains a mapping for the specified
             * key.  More formally, returns <b>true</b> if and only if
             * this map contains a mapping for a key <b>k</b> such that
             * <b>Objects.equals(key, k)</b>.  (There can be
             * at most one such mapping.)
             *
             * @param key key whose presence in this map is to be tested
             * @return <b>true</b> if this map contains a mapping for the specified
             *         key
             * @throws CastException if the key is of an inappropriate type for
             *         this map (<a href="">optional</a>)
             */
            virtual gbool containsKey(const K &key) const {
                Iterator<const Entry> &it = entrySet().iterator();
                while (it.hasNext()) {
                    const Entry &e = it.next();
                    if (Object::equals(key, e.key()))
                        return true;
                }
                return false;
            }

            /**
             * Returns <b>true</b> if this map maps one or more keys to the
             * specified value.  More formally, returns <b>true</b> if and only if
             * this map contains at least one mapping to a value <b>v</b> such that
             * <b>Objects.equals(value, v)</b>.  This operation
             * will probably require time linear in the map size for most
             * implementations of the <b>Map</b> interface.
             *
             * @param value value whose presence in this map is to be tested
             * @return <b>true</b> if this map maps one or more keys to the
             *         specified value
             * @throws CastException if the value is of an inappropriate type for
             *         this map (<a href="">optional</a>)
             */
            virtual gbool containsValue(const V &value) const {
                Iterator<const Entry> &it = entrySet().iterator();
                while (it.hasNext()) {
                    const Entry &e = it.next();
                    if (Object::equals(value, e.value()))
                        return true;
                }
                return false;
            }

            /**
             * Returns the value to which the specified key is mapped,
             * or <b>null</b> if this map contains no mapping for the key.
             *
             * <p>More formally, if this map contains a mapping from a key
             * <b>k</b> to a value <b>v</b> such that
             * <b>Objects.equals(key, k)</b>,
             * then this method returns <b>v</b>; otherwise
             * it returns <b>null</b>.  (There can be at most one such mapping.)
             *
             * <p>If this map permits null values, then a return value of
             * <b>null</b> does not <i>necessarily</i> indicate that the map
             * contains no mapping for the key; it's also possible that the map
             * explicitly maps the key to <b>null</b>.  The <b style="color:orange;">#containsKey
             * containsKey</b> operation may be used to distinguish these two cases.
             *
             * @param key the key whose associated value is to be returned
             * @return the value to which the specified key is mapped, or
             *         <b>null</b> if this map contains no mapping for the key
             * @throws CastException if the key is of an inappropriate type for
             *         this map (<a href="">optional</a>)
             */
            virtual V &get(const K &key) {
                Iterator<const Entry> &it = entrySet().iterator();
                while (it.hasNext()) {
                    const Entry &e = it.next();
                    if (Object::equals(key, e.key()))
                        return (V &) e.value();
                }
                KeyNotFoundException($(key)).throws(__trace("core.util.Map"));
            }

            /**
             * Returns the value to which the specified key is mapped,
             * or <b>null</b> if this map contains no mapping for the key.
             *
             * <p>More formally, if this map contains a mapping from a key
             * <b>k</b> to a value <b>v</b> such that
             * <b>Objects.equals(key, k)</b>,
             * then this method returns <b>v</b>; otherwise
             * it returns <b>null</b>.  (There can be at most one such mapping.)
             *
             * <p>If this map permits null values, then a return value of
             * <b>null</b> does not <i>necessarily</i> indicate that the map
             * contains no mapping for the key; it's also possible that the map
             * explicitly maps the key to <b>null</b>.  The <b style="color:orange;">#containsKey
             * containsKey</b> operation may be used to distinguish these two cases.
             *
             * @param key the key whose associated value is to be returned
             * @return the value to which the specified key is mapped, or
             *         <b>null</b> if this map contains no mapping for the key
             * @throws CastException if the key is of an inappropriate type for
             *         this map (<a href="">optional</a>)
             */
            virtual const V &get(const K &key) const {
                Iterator<const Entry> &it = entrySet().iterator();
                while (it.hasNext()) {
                    const Entry &e = it.next();
                    if (Object::equals(key, e.key()))
                        return e.value();
                }
                KeyNotFoundException($(key)).throws(__trace("core.util.Map"));
            }

            /**
             * Associates the specified value with the specified key in this map
             * (optional operation).  If the map previously contained a mapping for
             * the key, the old value is replaced by the specified value.  (A map
             * <b>m</b> is said to contain a mapping for a key <b>k</b> if and only
             * if <b style="color:orange;">#containsKey(Object) m.containsKey(k)</b> would return
             * <b>true</b>.)
             *
             * @param key key with which the specified value is to be associated
             * @param value value to be associated with the specified key
             * @return the previous value associated with <b>key</b>, or
             *         <b>null</b> if there was no mapping for <b>key</b>.
             *         (A <b>null</b> return can also indicate that the map
             *         previously associated <b>null</b> with <b>key</b>,
             *         if the implementation supports <b>null</b> values.)
             * @throws UnsupportedMethodException if the <b>put</b> operation
             *         is not supported by this map
             * @throws CastException if the class of the specified key or value
             *         prevents it from being stored in this map
             * @throws ArgumentException if some property of the specified key
             *         or value prevents it from being stored in this map
             */
            virtual const V &put(const K &key, const V &value) {
                UnsupportedMethodException().throws(__trace("core.util.Map"));
            }

            /**
             * If the specified key is not already associated with a value (or is mapped
             * to {@code null}) associates it with the given value and returns
             * {@code null}, else returns the current value.
             *
             * @implSpec
             * The default implementation is equivalent to, for this {@code map}:
             *
             * <pre> {@code
             * V v = map.get(key);
             * if (v == null)
             *     v = map.put(key, value);
             *
             * return v;
             * }</pre>
             *
             * <p>The default implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             *
             * @param key key with which the specified value is to be associated
             * @param value value to be associated with the specified key
             * @return the previous value associated with the specified key, or
             *         {@code null} if there was no mapping for the key.
             *         (A {@code null} return can also indicate that the map
             *         previously associated {@code null} with the key,
             *         if the implementation supports null values.)
             * @throws UnsupportedMethodException if the {@code put} operation
             *         is not supported by this map
             *         (<a href="{@docRoot}/java.base/java/util/Collection.html#optional-restrictions">optional</a>)
             * @throws CastException if the key or value is of an inappropriate
             *         type for this map (<a href="">optional</a>)
             * @throws ArgumentException if some property of the specified key
             *         or value prevents it from being stored in this map (<a href="">optional</a>)
             */
            virtual const V &putIfAbsent(const K &key, const V &value) {
                if (!containsKey(key))
                    return put(key, value);
                return value;
            }

            /**
             * Removes the mapping for a key from this map if it is present
             * (optional operation).   More formally, if this map contains a mapping
             * from key <b>k</b> to value <b>v</b> such that
             * <b>Objects.equals(key, k)</b>, that mapping
             * is removed.  (The map can contain at most one such mapping.)
             *
             * <p>Returns the value to which this map previously associated the key,
             * or <b>null</b> if the map contained no mapping for the key.
             *
             * <p>If this map permits null values, then a return value of
             * <b>null</b> does not <i>necessarily</i> indicate that the map
             * contained no mapping for the key; it's also possible that the map
             * explicitly mapped the key to <b>null</b>.
             *
             * <p>The map will not contain a mapping for the specified key once the
             * call returns.
             *
             * @param key key whose mapping is to be removed from the map
             * @return the previous value associated with <b>key</b>, or
             *         <b>null</b> if there was no mapping for <b>key</b>.
             * @throws UnsupportedMethodException if the <b>remove</b> operation
             *         is not supported by this map
             * @throws CastException if the key is of an inappropriate type for
             *         this map (<a href="">optional</a>)
             */
            virtual const V &remove(const K &key) {
                Iterator<const Entry> &it = entrySet().iterator();
                while (it.hasNext()) {
                    const Entry &e = it.next();
                    if (Object::equals(key, e.key())) {
                        it.remove();
                        return e.value();
                    }
                }
                KeyNotFoundException($(key)).throws(__trace("core.util.Map"));
            }

            /**
             * Copies all of the mappings from the specified map to this map
             * (optional operation).  The effect of this call is equivalent to that
             * of calling <b style="color:orange;">#put(Object,Object) put(k, v)</b> on this map once
             * for each mapping from key <b>k</b> to value <b>v</b> in the
             * specified map.  The behavior of this operation is undefined if the
             * specified map is modified while the operation is in progress.
             *
             * @param m mappings to be stored in this map
             * @throws UnsupportedMethodException if the <b>putAll</b> operation
             *         is not supported by this map
             * @throws CastException if the class of a key or value in the
             *         specified map prevents it from being stored in this map
             * @throws ArgumentException if some property of a key or value in
             *         the specified map prevents it from being stored in this map
             */
            virtual void putAll(const Map<K, V> &m) {
                for (const Entry &e: m.entrySet())
                    put(e.key(), e.value());
            }

            /**
             * Removes all of the mappings from this map (optional operation).
             * The map will be empty after this call returns.
             *
             * @throws UnsupportedMethodException if the <b>clear</b> operation
             *         is not supported by this map
             */
            virtual void clear() { entrySet().clear(); }

        protected:

            /**
             * Convert any value to string, using String::valueOf(o)
             */
            template<class T>
            static String $(T &&o) CORE_NOTHROW { return String::valueOf(U::forwardInstance<T>(o)); }

            CORE_ALIAS(U, , native::Unsafe);
            CORE_ALIAS(ENTRIES, , typename Class<Set<Entry>>::Ptr);
            CORE_ALIAS(VALUES, typename Class<Collection<V>>::Ptr);
            CORE_ALIAS(KEYSET, typename Class<Set<K>>::Ptr);

            /**
             * Each of these fields are initialized to contain an instance of the
             * appropriate view the first time this view is requested.  The views are
             * stateless, so there's no reason to create more than one of each.
             *
             * <p>Since there is no synchronization performed while accessing these fields,
             * it is expected that core.util.Map view classes using these fields have
             * no non-final fields (or any fields at all except for outer-this). Adhering
             * to this rule would make the races on these fields benign.
             *
             * <p>It is also imperative that implementations read the field only once,
             * as in:
             *
             * <pre> <b>
             * @code
             * Set<K> &keySet() {
             *   KEYSET ks = kSet;  // single racy read
             *   if (ks == null) {
             *     ks = new KeySet(*this);
             *     kSet = ks;
             *   }
             *   return *ks;
             * }
             * @endcode
             * </b></pre>
             */
            KEYSET kSet = {};

            VALUES vCollection = {};

        public:


            /**
             * Returns a <b style="color:orange;">Set</b> view of the keys contained in this map.
             * The set is backed by the map, so changes to the map are
             * reflected in the set, and vice-versa.  If the map is modified
             * while an iteration over the set is in progress (except through
             * the iterator's own <b>remove</b> operation), the results of
             * the iteration are undefined.  The set supports element removal,
             * which removes the corresponding mapping from the map, via the
             * <b>Iterator.remove</b>, <b>Set.remove</b>,
             * <b>removeAll</b>, <b>retainAll</b>, and <b>clear</b>
             * operations.  It does not support the <b>add</b> or <b>addAll</b>
             * operations.
             *
             * @return a set view of the keys contained in this map
             */
            virtual Set<K> &keySet() const {
                class KeySet CORE_FINAL : public Set<K> {
                    Map<K, V> &root;

                public:
                    CORE_EXPLICIT KeySet(Map<K, V> &root) : root(root) {}

                    Iterator<const K> &iterator() const override {
                        class KeyItr CORE_FINAL : public Iterator<const K> {
                            Iterator<const Entry> &itr;

                        public:
                            CORE_EXPLICIT KeyItr(Iterator<const Entry> &itr) : itr(itr) {}

                            gbool hasNext() const override { return itr.hasNext(); }

                            const K &next() override { return itr.next().key(); }

                            void remove() override { itr.remove(); }
                        };

                        return U::createInstance<KeyItr>(root.entrySet().iterator());
                    }

                    gint size() const override { return root.size(); }

                    gbool contains(const K &o) const override { return root.containsKey(o); }

                    void clear() override { root.clear(); }
                };

                if (kSet == null)
                    (KEYSET &) kSet = &U::createInstance<KeySet>((Map<K, V> &) *this);
                return kSet[0];
            }

            /**
             * Returns a <b style="color:orange;">Collection</b> view of the values contained in this map.
             * The collection is backed by the map, so changes to the map are
             * reflected in the collection, and vice-versa.  If the map is
             * modified while an iteration over the collection is in progress
             * (except through the iterator's own <b>remove</b> operation),
             * the results of the iteration are undefined.  The collection
             * supports element removal, which removes the corresponding
             * mapping from the map, via the <b>Iterator.remove</b>,
             * <b>Collection.remove</b>, <b>removeAll</b>,
             * <b>retainAll</b> and <b>clear</b> operations.  It does not
             * support the <b>add</b> or <b>addAll</b> operations.
             *
             * @return a collection view of the values contained in this map
             */
            virtual Collection<V> &values() const {
                class Values CORE_FINAL : public Collection<V> {
                    Map<K, V> &root;
                public:
                    CORE_EXPLICIT Values(Map<K, V> &root) : root(root) {}

                    Iterator<const V> &iterator() const override {
                        class ValueItr CORE_FINAL : public Iterator<const V> {
                            Iterator<const Entry> &itr;
                        public:
                            CORE_EXPLICIT ValueItr(Iterator<const Entry> &itr) : itr(itr) {}

                            gbool hasNext() const override { return itr.hasNext(); }

                            const V &next() override { return itr.next().value(); }

                            void remove() override { itr.remove(); }
                        };

                        return U::createInstance<ValueItr>(root.entrySet().iterator());
                    }

                    gint size() const override { return root.size(); }

                    gbool contains(const V &o) const override { return root.containsValue(o); }

                    void clear() override { root.clear(); }
                };

                if (vCollection == null)
                    (VALUES &) vCollection = &U::createInstance<Values>((Map<K, V> &) *this);
                return vCollection[0];
            }

            /**
             * Returns a <b style="color:orange;">Set</b> view of the mappings contained in this map.
             * The set is backed by the map, so changes to the map are
             * reflected in the set, and vice-versa.  If the map is modified
             * while an iteration over the set is in progress (except through
             * the iterator's own <b>remove</b> operation, or through the
             * <b>setValue</b> operation on a map entry returned by the
             * iterator) the results of the iteration are undefined.  The set
             * supports element removal, which removes the corresponding
             * mapping from the map, via the <b>Iterator.remove</b>,
             * <b>Set.remove</b>, <b>removeAll</b>, <b>retainAll</b> and
             * <b>clear</b> operations.  It does not support the
             * <b>add</b> or <b>addAll</b> operations.
             *
             * @return a set view of the mappings contained in this map
             */
            virtual Set<Entry> &entrySet() const = 0;

            /**
             * A map entry (key-value pair). The Entry may be unmodifiable, or the
             * value may be modifiable if the optional <b>setValue</b> method is
             * implemented. The Entry may be independent of any map, or it may represent
             * an entry of the entry-set view of a map.
             * <p>
             * Instances of the <b>Map.Entry</b> interface may be obtained by iterating
             * the entry-set view of a map. These instances maintain a connection to the
             * original, backing map. This connection to the backing map is valid
             * <i>only</i> for the duration of iteration over the entry-set view.
             * During iteration of the entry-set view, if supported by the backing map,
             * a change to a <b>Map.Entry</b>'s value via the
             * <b style="color:orange;">setValue</b> method will be visible in the backing map.
             * The behavior of such a <b>Map.Entry</b> instance is undefined outside of
             * iteration of the map's entry-set view. It is also undefined if the backing
             * map has been modified after the <b>Map.Entry</b> was returned by the
             * iterator, except through the <b>Map.Entry.setValue</b> method. In particular,
             * a change to the value of a mapping in the backing map might or might not be
             * visible in the corresponding <b>Map.Entry</b> element of the entry-set view.
             */
            interface Entry : public Object {
                /**
                 * Returns the key corresponding to this entry.
                 *
                 * @return the key corresponding to this entry
                 * @throws StateException implementations may, but are not
                 *         required to, throw this exception if the entry has been
                 *         removed from the backing map.
                 */
                virtual const K &key() const = 0;

                /**
                 * Returns the value corresponding to this entry.  If the mapping
                 * has been removed from the backing map (by the iterator's
                 * <b>remove</b> operation), the results of this call are undefined.
                 *
                 * @return the value corresponding to this entry
                 * @throws StateException implementations may, but are not
                 *         required to, throw this exception if the entry has been
                 *         removed from the backing map.
                 */
                virtual V &value() = 0;

                /**
                 * Returns the value corresponding to this entry.  If the mapping
                 * has been removed from the backing map (by the iterator's
                 * <b>remove</b> operation), the results of this call are undefined.
                 *
                 * @return the value corresponding to this entry
                 * @throws StateException implementations may, but are not
                 *         required to, throw this exception if the entry has been
                 *         removed from the backing map.
                 */
                virtual const V &value() const = 0;

                /**
                 * Replaces the value corresponding to this entry with the specified
                 * value (optional operation).  (Writes through to the map.)  The
                 * behavior of this call is undefined if the mapping has already been
                 * removed from the map (by the iterator's <b>remove</b> operation).
                 *
                 * @param value new value to be stored in this entry
                 * @return old value corresponding to the entry
                 * @throws UnsupportedMethodException if the <b>put</b> operation
                 *         is not supported by the backing map
                 * @throws CastException if the class of the specified value
                 *         prevents it from being stored in the backing map
                 * @throws ArgumentException if some property of this value
                 *         prevents it from being stored in the backing map
                 * @throws StateException implementations may, but are not
                 *         required to, throw this exception if the entry has been
                 *         removed from the backing map.
                 */
                virtual const V &set(const V &value) = 0;

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<Entry>::hasInstance(o))
                        return false;
                    const Entry &e = (Entry &) o;
                    return Object::equals(key(), e.key()) &&
                           Object::equals(value(), e.value());
                }
            };

            /**
             * Compares the specified object with this map for equality.  Returns
             * <b>true</b> if the given object is also a map and the two maps
             * represent the same mappings.  More formally, two maps <b>m1</b> and
             * <b>m2</b> represent the same mappings if
             * <b>m1.entries().equals(m2.entries())</b>.  This ensures that the
             * <b>equals</b> method works properly across different implementations
             * of the <b>Map</b> interface.
             *
             * @param o object to be compared for equality with this map
             * @return <b>true</b> if the specified object is equal to this map
             */
            gbool equals(const Object &o) const override {
                if (this == &o)
                    return true;
                if (!Class<Map<K, V>>::hasInstance(o))
                    return false;
                const Map<K, V> &m = (Map<K, V> &) o;
                if (size() != m.size())
                    return false;
                try {
                    for (const Entry &e: entrySet()) {
                        const K &key = e.key();
                        const V &value = e.value();
                        if (!Object::equals(m.get(key)))
                            return false;
                    }
                } catch (KeyNotFoundException &knfe) {
                    return false;
                }
                return true;
            }

            /**
             * Returns a string representation of this map.  The string representation
             * consists of a list of key-value mappings in the order returned by the
             * map's {@code entrySet} view's iterator, enclosed in braces
             * ({@code "{}"}).  Adjacent mappings are separated by the characters
             * {@code ", "} (comma and space).  Each key-value mapping is rendered as
             * the key followed by an equals sign ({@code "="}) followed by the
             * associated value.  Keys and values are converted to strings as by
             * {@link String#valueOf(Object)}.
             *
             * @return a string representation of this map
             */
            String toString() const override {
                Iterator<const Entry> &i = entrySet().iterator();
                if (!i.hasNext())
                    return "{}";

                StringBuffer sb = {};
                sb.append('{');
                for (;;) {
                    const Entry &e = i.next();
                    const K &key = e.key();
                    const V &value = e.value();
                    if (&(Object &) key == this)
                        sb.append("{...}"_S);
                    else
                        sb.append(key);

                    sb.append('=');
                    if (&(Object &) value == this)
                        sb.append("{...}"_S);
                    else
                        sb.append(value);

                    if (!i.hasNext())
                        return sb.append('}').toString();
                    sb.append(',').append(' ');
                }
            }

            /**
             * Returns the value to which the specified key is mapped, or
             * <b>defaultValue</b> if this map contains no mapping for the key.
             *
             * @implSpec
             * The implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             *
             * @param key the key whose associated value is to be returned
             * @param defaultValue the mapping of the key
             * @return the value to which the specified key is mapped, or
             * <b>defaultValue</b> if this map contains no mapping for the key
             * @throws CastException if the key is of an inappropriate type for
             * this map (<a href="">optional</a>)
             */
            virtual CORE_REQUIRED_RESULT V &getOrDefault(const K &key, const V &defaultValue) {
                try { return get(key); }
                catch (KeyNotFoundException &/*knfe*/) { return native::Unsafe::copyInstance(defaultValue, true); }
            }

            /**
             * Returns the value to which the specified key is mapped, or
             * <b>defaultValue</b> if this map contains no mapping for the key.
             *
             * @implSpec
             * The implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             *
             * @param key the key whose associated value is to be returned
             * @param defaultValue the mapping of the key
             * @return the value to which the specified key is mapped, or
             * <b>defaultValue</b> if this map contains no mapping for the key
             * @throws CastException if the key is of an inappropriate type for
             * this map (<a href="">optional</a>)
             */
            virtual CORE_REQUIRED_RESULT const V &getOrDefault(const K &key, const V &defaultValue) const {
                try {
                    return get(key);
                } catch (KeyNotFoundException &/*knfe*/) { return native::Unsafe::copyInstance(defaultValue, true); }
            };

            /**
             * Performs the given action for each entry in this map until all entries
             * have been processed or the action throws an exception.   Unless
             * otherwise specified by the implementing class, actions are performed in
             * the order of entry set iteration (if an iteration order is specified.)
             * Exceptions thrown by the action are relayed to the caller.
             *
             * @implSpec
             * The implementation is equivalent to, for this <b>map</b>:
             * <pre> <b>
             * for (Map.Entry<K, V> entry : map.entries())
             *     action.accept(entry.key(), entry.value());
             * </b></pre>
             *
             * The implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             *
             * @param action The action to be performed for each entry
             * @throws ConcurrentException if an entry is found to be
             * removed during iteration
             */
            virtual void forEach(BiConsumer<K, V> action) {};

            /**
             * Replaces each entry's value with the result of invoking the given
             * function on that entry until all entries have been processed or the
             * function throws an exception.  Exceptions thrown by the function are
             * relayed to the caller.
             *
             * <p>The implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             *
             * @param function the function to apply to each entry
             * @throws UnsupportedMethodException if the <b>set</b> operation
             *         is not supported by this map's entry set iterator.
             * @throws CastException if the class of a replacement value
             *         prevents it from being stored in this map
             *         (<a href="">optional</a>)
             * @throws ArgumentException if some property of a replacement value
             *         prevents it from being stored in this map
             *         (<a href="">optional</a>)
             * @throws ConcurrentException if an entry is found to be
             *         removed during iteration
             */
            virtual void replaceAll(BiFunction<K, V &, V> function) {};

            /**
             * Removes the entry for the specified key only if it is currently
             * mapped to the specified value.
             *
             * @implSpec
             * The implementation is equivalent to, for this <b>map</b>:
             *
             * <pre> <b>
             * if (map.containsKey(key) && Objects.equals(map.get(key), value)) {
             *     map.remove(key);
             *     return true;
             * </b> else
             *     return false;
             * </b></pre>
             *
             * <p>The implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             *
             * @param key key with which the specified value is associated
             * @param value value expected to be associated with the specified key
             * @return <b>true</b> if the value was removed
             * @throws UnsupportedMethodException if the <b>remove</b> operation
             *         is not supported by this map
             *         (<a href="">optional</a>)
             * @throws CastException if the key or value is of an inappropriate
             *         type for this map
             *         (<a href="">optional</a>)
             */
            virtual gbool remove(const K &key, const V &value) {
                try {
                    V &v = get(key);
                    if (Object::equals(v, value)) {
                        remove(key);
                        return true;
                    }
                    return false;
                } catch (const KeyNotFoundException &) { return false; }
            }

            /**
             * Replaces the entry for the specified key only if currently
             * mapped to the specified value.
             *
             * @implSpec
             * The implementation is equivalent to, for this <b>map</b>:
             *
             * <pre> <b>
             * if (map.containsKey(key) && Objects.equals(map.get(key), oldValue)) {
             *     map.put(key, newValue);
             *     return true;
             * </b> else
             *     return false;
             * </b></pre>
             *
             * <p>The implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             *
             * @param key key with which the specified value is associated
             * @param oldValue value expected to be associated with the specified key
             * @param newValue value to be associated with the specified key
             * @return <b>true</b> if the value was replaced
             * @throws UnsupportedMethodException if the <b>put</b> operation
             *         is not supported by this map
             *         (<a href="">optional</a>)
             * @throws CastException if the class of a specified key or value
             *         prevents it from being stored in this map
             * @throws ArgumentException if some property of a specified key
             *         or value prevents it from being stored in this map
             */
            virtual gbool replace(const K &key, const V &oldValue, const V &newValue) {
                try {
                    V &v = get(key);
                    if (!Object::equals(v, oldValue)) {
                        put(key, newValue);
                        return true;
                    }
                    return false;
                } catch (const KeyNotFoundException &) { return false; }
            }

            /**
             * Replaces the entry for the specified key only if it is
             * currently mapped to some value.
             *
             * @implSpec
             * The implementation is equivalent to, for this <b>map</b>:
             *
             * <pre> <b>
             * if (map.containsKey(key)) {
             *     return map.put(key, value);
             * </b></pre>
             *
             * <p>The implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             *
             * @param key key with which the specified value is associated
             * @param value value to be associated with the specified key
             * @return the previous value associated with the specified key
             * @throws UnsupportedMethodException if the <b>put</b> operation
             *         is not supported by this map
             *         (<a href="">optional</a>)
             * @throws CastException if the class of the specified key or value
             *         prevents it from being stored in this map
             *         (<a href="">optional</a>)
             * @throws ArgumentException if some property of the specified key
             *         or value prevents it from being stored in this map
             */
            virtual const V &replace(const K &key, const V &value) {
                V &v = get(key);
                if (!Object::equals(v, value)) return put(key, value);
                return v;
            };
        };

#if CORE_TEMPLATE_TYPE_DEDUCTION
        template<class K, class V>
        Map(Map<K, V>) -> Map<K, V>;
#endif

    }
} // core

#endif //CORE23_MAP_H
