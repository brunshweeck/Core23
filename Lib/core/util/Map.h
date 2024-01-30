//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_MAP_H
#define CORE23_MAP_H

#include <core/NoSuchKeyException.h>
#include <core/function/BiConsumer.h>
#include <core/function/BiFunction.h>
#include <core/util/ConcurrentException.h>
#include <core/util/Set.h>

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
         * allow a map's contents to be viewed as a setValue of keys, collection of values,
         * or setValue of key-value mappings.  The <i>order</i> of a map is defined as
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
         * <b> UnsupportedOperationException</b> if this map does not support the
         * operation.  If this is the case, these methods may, but are not required
         * to, throw an <b> UnsupportedOperationException</b> if the invocation would
         * have no effect on the map.  For example, invoking the <b style="color: orange;"> addAll(Map)</b>
         * method on an unmodifiable map may, but is not required to, throw the
         * exception if the map whose mappings are to be "superimposed" is empty.
         *
         * <p>Some map implementations have restrictions on the keys and values they
         * may contain.  Attempting
         * to insert an ineligible key or value throws an unchecked exception,
         * typically <b> ClassCastException</b>.
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
         * will always cause <b> UnsupportedOperationException</b> to be thrown.
         * <li>They are serializable if all keys and values are serializable.
         * <li>They reject duplicate keys at creation time. Duplicate keys
         * passed to a static factory method result in <b> IllegalArgumentException</b>.
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
        class Map : public Object {
        public:

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
            virtual gbool isEmpty() const { return size() == 0; }

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
             * @throws ClassCastException if the key is of an inappropriate type for
             *         this map (<a href="">optional</a>)
             */
            virtual gbool containsKey(const K &key) const {
                Iterator<Entry> &it = entrySet().iterator();
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
             * @throws ClassCastException if the value is of an inappropriate type for
             *         this map (<a href="">optional</a>)
             */
            virtual gbool containsValue(const V &value) const {
                Iterator<Entry> &it = entrySet().iterator();
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
             * explicitly maps the key to <b>null</b>.  The <b style="color:orange;"> containsKey
             * containsKey</b> operation may be used to distinguish these two cases.
             *
             * @param key the key whose associated value is to be returned
             * @return the value to which the specified key is mapped, or
             *         <b>null</b> if this map contains no mapping for the key
             * @throws ClassCastException if the key is of an inappropriate type for
             *         this map (<a href="">optional</a>)
             */
            virtual V &get(const K &key) {
                Iterator<Entry> &it = entrySet().iterator();
                while (it.hasNext()) {
                    const Entry &e = it.next();
                    if (Object::equals(key, e.key()))
                        return (V &) e.value();
                }
                NoSuchKeyException($(key)).throws(__trace("core.util.Map"));
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
             * explicitly maps the key to <b>null</b>.  The <b style="color:orange;"> containsKey
             * containsKey</b> operation may be used to distinguish these two cases.
             *
             * @param key the key whose associated value is to be returned
             * @return the value to which the specified key is mapped, or
             *         <b>null</b> if this map contains no mapping for the key
             * @throws ClassCastException if the key is of an inappropriate type for
             *         this map (<a href="">optional</a>)
             */
            virtual const V &get(const K &key) const {
                Iterator<Entry> &it = entrySet().iterator();
                while (it.hasNext()) {
                    const Entry &e = it.next();
                    if (Object::equals(key, e.key()))
                        return e.value();
                }
                NoSuchKeyException($(key)).throws(__trace("core.util.Map"));
            }

            /**
             * Associates the specified value with the specified key in this map
             * (optional operation).  If the map previously contained a mapping for
             * the key, the old value is replaced by the specified value.  (A map
             * <b>m</b> is said to contain a mapping for a key <b>k</b> if and only
             * if <b style="color:orange;"> containsKey(Object) m.containsKey(k)</b> would return
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
             * @throws ClassCastException if the class of the specified key or value
             *         prevents it from being stored in this map
             * @throws IllegalArgumentException if some property of the specified key
             *         or value prevents it from being stored in this map
             */
            virtual const V &put(const K &key, const V &value) {
                CORE_IGNORE(key);
                CORE_IGNORE(value);
                UnsupportedOperationException().throws(__trace("core.util.Map"));
            }

            /**
             * If the specified key is not already associated with a value (or is mapped
             * to <b> null</b>) associates it with the given value and returns
             * <b> null</b>, else returns the current value.
             *
             * @implSpec
             * The default implementation is equivalent to, for this <b> map</b>:
             *
             * <pre> @code
             * V v = map.get(key);
             * if (v == null)
             *     v = map.put(key, value);
             *
             * return v;
             * @endcode </pre>
             *
             * <p>The default implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             *
             * @param key key with which the specified value is to be associated
             * @param value value to be associated with the specified key
             * @return the previous value associated with the specified key, or
             *         <b> null</b> if there was no mapping for the key.
             *         (A <b> null</b> return can also indicate that the map
             *         previously associated <b> null</b> with the key,
             *         if the implementation supports null values.)
             * @throws UnsupportedMethodException if the <b> put</b> operation
             *         is not supported by this map (<a href="">optional</a>)
             * @throws ClassCastException if the key or value is of an inappropriate
             *         type for this map (<a href="">optional</a>)
             * @throws IllegalArgumentException if some property of the specified key
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
             * @throws ClassCastException if the key is of an inappropriate type for
             *         this map (<a href="">optional</a>)
             */
            virtual const V &remove(const K &key) {
                Iterator<Entry> &it = entrySet().iterator();
                while (it.hasNext()) {
                    const Entry &e = it.next();
                    if (Object::equals(key, e.key())) {
                        it.remove();
                        return e.value();
                    }
                }
                NoSuchKeyException($(key)).throws(__trace("core.util.Map"));
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
             * @throws ClassCastException if the class of a key or value in the
             *         specified map prevents it from being stored in this map
             * @throws IllegalArgumentException if some property of a key or value in
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
            static String $(const T &o) CORE_NOTHROW {
                return String::valueOf(o);
            }

            CORE_ALIAS(ENTRIES, , typename Class<Set<Entry>>::Ptr);
            CORE_ALIAS(VALUES, typename Class<Collection<V>>::Ptr);
            CORE_ALIAS(KEYSET, typename Class<Set<K>>::Ptr);

            /**
             * Each of these fields are initialized to contain an INSTANCE of the
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
             * The setValue is backed by the map, so changes to the map are
             * reflected in the setValue, and vice-versa.  If the map is modified
             * while an iteration over the setValue is in progress (except through
             * the iterator's own <b>remove</b> operation), the results of
             * the iteration are undefined.  The setValue supports element removal,
             * which removes the corresponding mapping from the map, via the
             * <b>Iterator.remove</b>, <b>Set.remove</b>,
             * <b>removeAll</b>, <b>retainAll</b>, and <b>clear</b>
             * operations.  It does not support the <b>add</b> or <b>addAll</b>
             * operations.
             *
             * @return a setValue view of the keys contained in this map
             */
            virtual Set<K> &keySet() const {
                KEYSET ks = kSet;
                if (ks == null) {
                    ks = &Unsafe::allocateInstance<KeySet>((Map &) *this);
                    (KEYSET &) kSet = ks;
                }
                return *ks;
            }

        private:
            class KeySet CORE_FINAL : public Set<K> {
            private:
                Map &This;

            public:
                CORE_EXPLICIT KeySet(Map &root) : This(root) {}

                Iterator<const K> &iterator() const override {
                    return Unsafe::allocateInstance<KeyItr<const K>>(This.entrySet().iterator());
                }

                Iterator<K> &iterator() override {
                    return Unsafe::allocateInstance<KeyItr<>>(This.entrySet().iterator());
                }

                gint size() const override {
                    return This.size();
                }

                gbool contains(const K &o) const override {
                    return This.containsKey(o);
                }

                void clear() override {
                    This.clear();
                }

                Object &clone() const override {
                    return This.keySet();
                }
            };

            template<class T = K>
            class KeyItr CORE_FINAL : public Iterator<T> {
            private:
                Iterator<Entry> &itE;

            public:
                CORE_EXPLICIT KeyItr(Iterator<Entry> &itr) : itE(itr) {}

                gbool hasNext() const override {
                    return itE.hasNext();
                }

                T &next() override {
                    return (T &) itE.next().key();
                }

                void remove() override {
                    itE.remove();
                }

                gbool equals(const Object &o) const override {
                    if (this == &o) {
                        return true;
                    }
                    if (!Class<KeyItr>::hasInstance(o)) {
                        return false;
                    }
                    KeyItr const &it = (const KeyItr &) o;
                    return itE == it.itE;
                }

                Object &clone() const override {
                    return Unsafe::allocateInstance<KeyItr>(*this);
                }
            };

        public:

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
                VALUES vs = vCollection;
                if (vs == null) {
                    vs = &Unsafe::allocateInstance<Values>((Map &) *this);
                    (VALUES &) vCollection = vs;
                }
                return *vs;
            }

        private:
            class Values CORE_FINAL : public Collection<V> {
            private:
                Map &This;

            public:
                CORE_EXPLICIT Values(Map &root) : This(root) {}

                Iterator<const V> &iterator() const override {
                    return Unsafe::allocateInstance<ValueItr<const V>>(This.entrySet().iterator());
                }

                Iterator<V> &iterator() override {
                    return Unsafe::allocateInstance<ValueItr<>>(This.entrySet().iterator());
                }

                gint size() const override {
                    return This.size();
                }

                gbool contains(const V &o) const override {
                    return This.containsValue(o);
                }

                void clear() override {
                    This.clear();
                }

                Object &clone() const override {
                    return This.values();
                }

                gbool equals(const Object &o) const override {
                    return this == &o;
                }
            };

            template<class T = V>
            class ValueItr CORE_FINAL : public Iterator<T> {
            private:
                Iterator<Entry> &itE;

            public:
                CORE_EXPLICIT ValueItr(Iterator<Entry> &itr) : itE(itr) {}

                gbool hasNext() const override {
                    return itE.hasNext();
                }

                T &next() override {
                    return itE.next().value();
                }

                void remove() override {
                    itE.remove();
                }

                gbool equals(const Object &o) const override {
                    if (this == &o) {
                        return true;
                    }
                    if (!Class<ValueItr>::hasInstance(o)) {
                        return false;
                    }
                    ValueItr const &it = (const ValueItr &) o;
                    return it.itE == itE;
                }

                Object &clone() const override {
                    return Unsafe::allocateInstance<ValueItr>(*this);
                }
            };

        public:

            /**
             * Returns a <b style="color:orange;">Set</b> view of the mappings contained in this map.
             * The setValue is backed by the map, so changes to the map are
             * reflected in the setValue, and vice-versa.  If the map is modified
             * while an iteration over the setValue is in progress (except through
             * the iterator's own <b>remove</b> operation, or through the
             * <b>setValue</b> operation on a map entry returned by the
             * iterator) the results of the iteration are undefined.  The setValue
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
             * an entry of the entry-setValue view of a map.
             * <p>
             * Instances of the <b>Map.Entry</b> interface may be obtained by iterating
             * the entry-setValue view of a map. These instances maintain a connection to the
             * original, backing map. This connection to the backing map is valid
             * <i>only</i> for the duration of iteration over the entry-setValue view.
             * During iteration of the entry-setValue view, if supported by the backing map,
             * a change to a <b>Map.Entry</b>'s value via the
             * <b style="color:orange;">setValue</b> method will be visible in the backing map.
             * The behavior of such a <b>Map.Entry</b> INSTANCE is undefined outside of
             * iteration of the map's entry-setValue view. It is also undefined if the backing
             * map has been modified after the <b>Map.Entry</b> was returned by the
             * iterator, except through the <b>Map.Entry.setValue</b> method. In particular,
             * a change to the value of a mapping in the backing map might or might not be
             * visible in the corresponding <b>Map.Entry</b> element of the entry-setValue view.
             */
            class Entry : public Object {
            public:
                /**
                 * Returns the key corresponding to this entry.
                 *
                 * @return the key corresponding to this entry
                 * @throws IllegalStateException implementations may, but are not
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
                 * @throws IllegalStateException implementations may, but are not
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
                 * @throws IllegalStateException implementations may, but are not
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
                 * @throws ClassCastException if the class of the specified value
                 *         prevents it from being stored in the backing map
                 * @throws IllegalArgumentException if some property of this value
                 *         prevents it from being stored in the backing map
                 * @throws IllegalStateException implementations may, but are not
                 *         required to, throw this exception if the entry has been
                 *         removed from the backing map.
                 */
                virtual const V &setValue(const V &value) = 0;

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<Entry>::hasInstance(o))
                        return false;
                    const Entry &e = (Entry &) o;
                    return Object::equals(key(), e.key()) &&
                           Object::equals(value(), e.value());
                }

                /**
                 * Returns a comparator that compares <b style="color: orange;"> Map.Entry</b> in natural order on key.
                 *
                 * @return a comparator that compares <b style="color: orange;"> Map.Entry</b> in natural order on key.
                 * @see Comparable
                 */
                static Comparator<Entry> &comparingByKey() {
                    class EntryComparator CORE_FINAL : public Comparator<Entry> {
                    public:
                        gint compare(const Entry &o1, const Entry &o2) const override {
                            K const &key1 = o1.key();
                            K const &key2 = o2.key();
                            return Comparable<K>::naturalOrder().compare(key1, key2);
                        }

                        Object &clone() const override {
                            return (Object &) *this;
                        }
                    };

                    static EntryComparator BY_KEY{};
                    return BY_KEY;
                }

                /**
                 * Returns a cmp that compares <b style="color: orange;"> Map.Entry</b> by key using the given
                 * <b style="color: orange;"> Comparator</b>.
                 *
                 * <p>The returned cmp is serializable if the specified cmp
                 * is also serializable.
                 *
                 * @param  cmp the key <b style="color: orange;"> Comparator</b>
                 * @return a cmp that compares <b style="color: orange;"> Map.Entry</b> by the key.
                 */
                template<class T = K, Class<gbool>::OnlyIf<Class<T>::template isSuper<K>()> = true>
                static Comparator<Entry> &comparingByKey(const Comparator<T> &cmp) {
                    class EntryComparator CORE_FINAL : public Comparator<Entry> {
                    private:
                        Comparator<T> &cmp;

                    public:
                        CORE_EXPLICIT EntryComparator(Comparator<T> &cmp) : cmp(cmp) {}

                        gint compare(const Entry &o1, const Entry &o2) const override {
                            K const &key1 = o1.key();
                            K const &key2 = o2.key();
                            return cmp.compare(key1, key2);
                        }

                        Comparator<Entry> &reverse() const override {
                            return Unsafe::allocateInstance<EntryComparator>(cmp.reverse());
                        }

                        Object &clone() const override {
                            return (Object &) *this;
                        }
                    };

                    if (cmp == Comparator<T>::naturalOrder() || cmp == Comparator<T>::reverseOrder())
                        return Unsafe::allocateInstance<EntryComparator>((Comparator<T> &) cmp);
                    else
                        return Unsafe::allocateInstance<EntryComparator>(Unsafe::copyInstance(cmp, true));
                }

                /**
                 * Returns a comparator that compares <b style="color: orange;"> Map.Entry</b> in natural order on value.
                 *
                 * @return a comparator that compares <b style="color: orange;"> Map.Entry</b> in natural order on value.
                 * @see Comparable
                 */
                static Comparator<Entry> &comparingByValue() {
                    class EntryComparator CORE_FINAL : public Comparator<Entry> {
                    public:
                        gint compare(const Entry &o1, const Entry &o2) const override {
                            V const &value1 = o1.value();
                            V const &value2 = o2.value();
                            return Comparable<V>::naturalOrder().compare(value1, value2);
                        }

                        Object &clone() const override {
                            return (Object &) *this;
                        }
                    };

                    static EntryComparator BY_VALUE{};
                    return BY_VALUE;
                }

                /**
                 * Returns a cmp that compares <b style="color: orange;"> Map.Entry</b> by value using the given
                 * <b style="color: orange;"> Comparator</b>.
                 *
                 * @param  cmp the value <b style="color: orange;"> Comparator</b>
                 * @return a cmp that compares <b style="color: orange;"> Map.Entry</b> by the value.
                 */
                template<class T = V, Class<gbool>::OnlyIf<Class<T>::template isSuper<V>()> = true>
                static Comparator<Entry> &comparingByValue(const Comparator<T> &cmp) {
                    class EntryComparator CORE_FINAL : public Comparator<Entry> {
                    private:
                        Comparator<T> &cmp;

                    public:
                        CORE_EXPLICIT EntryComparator(Comparator<T> &cmp) : cmp(cmp) {}

                        gint compare(const Entry &o1, const Entry &o2) const override {
                            V const &value1 = o1.value();
                            V const &value2 = o2.value();
                            return cmp.compare(value1, value2);
                        }

                        Comparator<Entry> &reverse() const override {
                            return Unsafe::allocateInstance<EntryComparator>(cmp.reverse());
                        }

                        Object &clone() const override {
                            return (Object &) *this;
                        }
                    };
                    if (cmp == Comparator<T>::naturalOrder() || cmp == Comparator<T>::reverseOrder())
                        return Unsafe::allocateInstance<EntryComparator>((Comparator<T> &) cmp);
                    else
                        return Unsafe::allocateInstance<EntryComparator>(Unsafe::copyInstance(cmp, true));
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
                CORE_TRY_ONLY
                ({
                     for (const Entry &e: entrySet()) {
                         const K &key = e.key();
                         const V &value = e.value();
                         if (!Object::equals(value, m.get(key)))
                             return false;
                     }
                     return true;
                 })
                return false;
            }

            /**
             * Returns a string representation of this map.  The string representation
             * consists of a list of key-value mappings in the order returned by the
             * map's <b> entrySet</b> view's iterator, enclosed in braces
             * (<b> "{}"</b>).  Adjacent mappings are separated by the characters
             * <b> ", "</b> (comma and diskSpace).  Each key-value mapping is rendered as
             * the key followed by an equals sign (<b> "="</b>) followed by the
             * associated value.  Keys and values are converted to strings as by
             * <b style="color:orange;"> String#valueOf(Object)</b>.
             *
             * @return a string representation of this map
             */
            String toString() const override {
                Iterator<Entry> &i = entrySet().iterator();
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
             * @throws ClassCastException if the key is of an inappropriate type for
             * this map (<a href="">optional</a>)
             */
            virtual V &getOrDefault(const K &key, const V &defaultValue) {
                CORE_TRY_ONLY({ return get(key); })
                return Unsafe::copyInstance(defaultValue, true);
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
             * @throws ClassCastException if the key is of an inappropriate type for
             * this map (<a href="">optional</a>)
             */
            virtual const V &getOrDefault(const K &key, const V &defaultValue) const {
                CORE_TRY_ONLY({ return get(key); })
                return Unsafe::copyInstance(defaultValue, true);
            };

            /**
             * Performs the given action for each entry in this map until all entries
             * have been processed or the action throws an exception.   Unless
             * otherwise specified by the implementing class, actions are performed in
             * the order of entry setValue iteration (if an iteration order is specified.)
             * Exceptions thrown by the action are relayed to the caller.
             *
             * @implSpec
             * The implementation is equivalent to, for this <b>map</b>:
             * <pre> <b> @code
             * for (Map.Entry<K, V> entry : map.entries())
             *     action.accept(entry.key(), entry.value());
             * @endcode </b></pre>
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
            virtual void forEach(const BiConsumer<K, V> &action) const {
                for (Entry &entry: entrySet()) {
                    KEY k = null;
                    VALUE v = null;
                    try {
                        k = (KEY) &entry.key();
                        v = &entry.value();
                    } catch (IllegalStateException const &ise) {
                        // this usually means the entry is no longer in the map.
                        ConcurrentException(ise).throws(__trace("core.util.Map"));
                    }
                    action.accept(*k, *v);
                }
            };

            /**
             * Performs the given action for each entry in this map until all entries
             * have been processed or the action throws an exception.   Unless
             * otherwise specified by the implementing class, actions are performed in
             * the order of entry setValue iteration (if an iteration order is specified.)
             * Exceptions thrown by the action are relayed to the caller.
             *
             * @implSpec
             * The implementation is equivalent to, for this <b>map</b>:
             * <pre> <b> @code
             * for (Map.Entry<K, V> entry : map.entries())
             *     action.accept(entry.key(), entry.value());
             * @endcode </b></pre>
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
            virtual void forEach(const BiConsumer<K, V &> &action) {
                for (Entry &entry: entrySet()) {
                    KEY k = null;
                    VALUE v = null;
                    try {
                        k = (KEY) &entry.key();
                        v = &entry.value();
                    } catch (IllegalStateException const &ise) {
                        // this usually means the entry is no longer in the map.
                        ConcurrentException(ise).throws(__trace("core.util.Map"));
                    }
                    action.accept(*k, *v);
                }
            };

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
             * @throws UnsupportedMethodException if the <b>setValue</b> operation
             *         is not supported by this map's entry setValue iterator.
             * @throws ClassCastException if the class of a replacement value
             *         prevents it from being stored in this map
             *         (<a href="">optional</a>)
             * @throws IllegalArgumentException if some property of a replacement value
             *         prevents it from being stored in this map
             *         (<a href="">optional</a>)
             * @throws ConcurrentException if an entry is found to be
             *         removed during iteration
             */
            virtual void replaceAll(const BiFunction<K, V, V> &function) {
                for (Entry &entry: entrySet()) {
                    KEY k;
                    VALUE v;
                    try {
                        k = (KEY) &entry.key();
                        v = &entry.value();
                    } catch (IllegalStateException const &ise) {
                        // this usually means the entry is no longer in the map.
                        ConcurrentException(ise).throws(__trace("core.util.Map"));
                    }

                    CORE_ALIAS(Z, typename Functional::template Return<V>);

                    // ise thrown from function is not a cme.
                    Z z = function.apply(*k, *v);

                    try {
                        entry.setValue(z);
                    } catch (IllegalStateException const &ise) {
                        // this usually means the entry is no longer in the map.
                        ConcurrentException(ise).throws(__trace("core.util.Map"));
                    }
                }
            };

            /**
             * Removes the entry for the specified key only if it is currently
             * mapped to the specified value.
             *
             * @implSpec
             * The implementation is equivalent to, for this <b>map</b>:
             *
             * <pre> <b> @code
             * if (map.containsKey(key) &&
             *      Objects.equals(map.get(key), value)) {
             *     map.remove(key);
             *     return true;
             * } else
             *     return false;
             * @endcode </b></pre>
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
             *         is not supported by this map (<a href="">optional</a>)
             * @throws ClassCastException if the key or value is of an inappropriate
             *         type for this map (<a href="">optional</a>)
             */
            virtual gbool remove(const K &key, const V &value) {
                CORE_TRY_ONLY
                ({
                     V &v = get(key);
                     if (Object::equals(v, value)) {
                         remove(key);
                         return true;
                     }
                 })
                return false;
            }

            /**
             * Replaces the entry for the specified key only if currently
             * mapped to the specified value.
             *
             * @implSpec
             * The implementation is equivalent to, for this <b>map</b>:
             *
             * <pre> <b> @code
             * if (map.containsKey(key) &&
             *      Objects.equals(map.get(key), oldValue)) {
             *     map.put(key, newValue);
             *     return true;
             * } else
             *     return false;
             * @endcode </b></pre>
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
             * @throws ClassCastException if the class of a specified key or value
             *         prevents it from being stored in this map
             * @throws IllegalArgumentException if some property of a specified key
             *         or value prevents it from being stored in this map
             */
            virtual gbool replace(const K &key, const V &oldValue, const V &newValue) {
                CORE_TRY_ONLY
                ({
                     V &v = get(key);
                     if (!Object::equals(v, oldValue)) {
                         put(key, newValue);
                         return true;
                     }
                 })
                return false;
            }

            /**
             * Replaces the entry for the specified key only if it is
             * currently mapped to some value.
             *
             * @implSpec
             * The implementation is equivalent to, for this <b>map</b>:
             *
             * <pre> <b> @code
             * if (map.containsKey(key)) {
             *     return map.put(key, value);
             * } @endcode </b></pre>
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
             * @throws ClassCastException if the class of the specified key or value
             *         prevents it from being stored in this map
             *         (<a href="">optional</a>)
             * @throws IllegalArgumentException if some property of the specified key
             *         or value prevents it from being stored in this map
             */
            virtual const V &replace(const K &key, const V &value) {
                CORE_TRY_ONLY
                ({
                     V &v = get(key);
                     if (Object::equals(v, value))
                         return v;
                 })
                return put(key, value);
            };

        private:
            CORE_ALIAS(KEY, typename Class<const K>::Ptr);
            CORE_ALIAS(VALUE, typename Class<V>::Ptr);

            class SimpleEntry CORE_FINAL : public Entry {
            private:
                KEY k;
                VALUE v;
            public:
                CORE_EXPLICIT SimpleEntry(const K &key, V &value) : k(&key), v(&value) {}

                const K &key() const override {
                    return *k;
                }

                V &value() override {
                    return *v;
                }

                const V &value() const override {
                    return *v;
                }

                const V &setValue(const V &value) override {
                    if (Object::equals(*v, value))
                        return *v;
                    v = Unsafe::copyInstance(value, true);
                }
            };

            template<class T= K, class U= V>
            class ImmutableMap CORE_FINAL : public Map<T, U> {
            private:
                Array<Entry> array;
                ENTRIES entries = {};

            public:
                CORE_EXPLICIT ImmutableMap(const Array<Entry> &array) : array(Unsafe::moveInstance(array)) {}

                gint size() const override {
                    return array.length();
                }

                class EntrySet CORE_FINAL : public Set<Entry> {
                private:
                    ImmutableMap &root;

                public:
                    CORE_EXPLICIT EntrySet(ImmutableMap &root) : root(root) {}

                    gint size() const override {
                        return root.size();
                    }

                    gbool contains(const Entry &o) const override {
                        for (const Entry &entry: root.array) {
                            if (Object::equals(entry, o))
                                return true;
                        }
                        return false;
                    }

                    class EntryItr CORE_FINAL : public Iterator<Entry> {
                    private:
                        EntrySet &set;
                        gint i = 0;

                    public:
                        CORE_EXPLICIT EntryItr(EntrySet &set) : set(set) {}

                        gbool hasNext() const override {
                            return i < set.size();
                        }

                        Entry &next() override {
                            if (!hasNext())
                                NoSuchElementException().throws(__trace(classname()));
                            return set.root.array[i++];
                        }

                        gbool equals(const Object &o) const override {
                            if (this == &o)
                                return true;
                            if (!Class<EntryItr>::hasInstance(o))
                                return false;
                            EntryItr const &it = CORE_DYN_CAST(EntryItr const&, o);
                            return i == it.i && &set == &it.set;
                        }

                        Object &clone() const override {
                            return Unsafe::allocateInstance<EntryItr>(*this);
                        }
                    };

                    Iterator<const Entry> &iterator() const override {
                        return Unsafe::allocateInstance<EntryItr>((EntrySet &) *this);
                    }

                    void clear() override {
                        UnsupportedOperationException().throws(__trace(classname()));
                    }
                };

                Set<Entry> &entrySet() const override {
                    if (entries == null) {
                        (ENTRIES &) entries = &Unsafe::allocateInstance<EntrySet>((ImmutableMap &) *this);
                    }
                    return *entries;
                }
            };

        public:
            static Entry &entry(const K &key, const V &value) {
                CORE_TRY_RETHROW_EXCEPTION
                ({
                     K &newKey = Unsafe::copyInstance(key, true);
                     V &newValue = Unsafe::copyInstance(value, true);
                     return Unsafe::allocateInstance<SimpleEntry>(newKey, newValue);
                 }, , __trace("core.util.Map"))
            }
        };

#if CORE_TEMPLATE_TYPE_DEDUCTION
        template<class K, class V>
        Map(Map<K, V>) -> Map<K, V>;
#endif

    }
} // core

#endif //CORE23_MAP_H
