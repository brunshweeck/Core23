//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_COLLECTION_H
#define CORE23_COLLECTION_H

#include <core/NoSuchItemException.h>
#include <core/StateException.h>
#include <core/UnsupportedMethodException.h>
#include <core/native/ReferenceArray.h>
#include <core/util/ConcurrentException.h>
#include <core/util/Iterator.h>
#include <core/util/SortedStruct.h>
#include <core/util/Comparator.h>
#include <core/util/function/Consumer.h>
#include <core/util/function/Predicate.h>
#include <core/StringBuffer.h>

namespace core {
    namespace util {

        /**
         * The root interface in the <i>collection hierarchy</i>.  A collection
         * represents a group of objects, known as its <i>elements</i>.  Some
         * collections allow duplicate elements and others do not.  Some are ordered
         * and others unordered.  The core library does not provide any <i>direct</i>
         * implementations of this interface: it provides implementations of more
         * specific sub-interfaces like <b>Set</b> and <b>List</b>.  This interface
         * is typically used to pass collections around and manipulate them where
         * maximum generality is desired.
         *
         * <p>
         * <i>Bags</i> or <i>multi-sets</i> (unordered collections that may contain
         * duplicate elements) should implement this interface directly.
         *
         * <p>
         * All general-purpose <b>Collection</b> implementation classes (which
         * typically implement <b>Collection</b> indirectly through one of its
         * sub-interfaces) should provide two "standard" constructors: a void (no
         * arguments) constructor, which creates an empty collection, and a
         * constructor with a single argument of type <b>Collection</b>, which
         * creates a new collection with the same elements as its argument.  In
         * effect, the latter constructor allows the user to copy any collection,
         * producing an equivalent collection of the desired implementation type.
         *
         * <p>
         * Certain methods are specified to be <i>optional</i>. If a collection
         * implementation doesn't implement a particular operation, it should define
         * the corresponding method to throw <b>UnsupportedMethodException</b>.
         * Such methods are marked "optional operation" in method specifications of
         * the collections interfaces.
         *
         * <p>
         * Some collection implementations have restrictions on the elements that they
         * may contain. For example, some have restrictions on the types of their elements.
         * Attempting to add an ineligible element throws an unchecked exception, typically
         * <b>CastException</b>.  Attempting to query the presence of an ineligible element
         * may throw an exception, or it may simply return false; some implementations will
         * exhibit the former behavior and some will exhibit the latter.  More generally,
         * attempting an operation on an ineligible element whose completion would not result
         * in the insertion of an ineligible element into the collection may throw an
         * exception or it may succeed, at the option of the implementation.
         * Such exceptions are marked as "optional" in the specification for this
         * interface.
         *
         * <p>
         * It is up to each collection to determine its own synchronization
         * policy.  In the absence of a stronger guarantee by the
         * implementation, undefined behavior may result from the invocation
         * of any method on a collection that is being mutated by another
         * thread; this includes direct invocations, passing the collection to
         * a method that might perform invocations, and using an existing
         * iterator to examine the collection.
         *
         * <p>
         * Many methods in Collections Framework interfaces are defined in
         * terms of the <b style="color: orange;">equals</b> method.
         * For example, the specification for the <b style="color: orange;">contains(Object o)</b>
         * method says: "returns <b>true</b> if and only if this collection contains at least one
         * element <b>e</b> such that <b>o.equals(e)</b>."  This specification should
         * <i>not</i> be construed to imply that invoking <b>Collection.contains</b>
         * with a non-null argument <b>o</b> will cause <b>o.equals(e)</b> to be
         * invoked for any element <b>e</b>.  Implementations are free to implement
         * optimizations whereby the <b>equals</b> invocation is avoided, for
         * example, by first comparing the hash codes of the two elements.  (The
         * <b style="color: orange;">Hashable::hashCode()</b> specification guarantees that two objects
         * with unequal hash codes cannot be equal.)  More generally, implementations of
         * the various Collections Framework interfaces are free to take advantage of
         * the specified behavior of underlying <b style="color: orange;">Object</b> methods wherever the
         * implementor deems it appropriate.
         *
         * <p>Some collection operations which perform recursive traversal of the
         * collection may fail with an exception for self-referential instances where
         * the collection directly or indirectly contains itself. This includes the
         * <b>clone()</b>, <b>equals()</b>, <b>hashCode()</b> and <b>toString()</b>
         * methods. Implementations may optionally handle the self-referential scenario,
         * however most current implementations do not do so.
         *
         * <h2> View Collections </h2>
         *
         * <p>
         * Most collections manage storage for elements they contain. By contrast, <i>view
         * collections</i> themselves do not store elements, but instead they rely on a
         * backing collection to store the actual elements. Operations that are not handled
         * by the view collection itself are delegated to the backing collection. Examples as
         * provided by <b style="color: orange;">List.subList</b>,
         * <b style="color: orange;"> Set.subSet </b>, or
         * <b style="color: orange;"> Map.entries </b>.
         * Any changes made to the backing collection are visible in the view collection.
         * Correspondingly, any changes made to the view collection &mdash; if changes
         * are permitted &mdash; are written through to the backing collection.
         * Although they technically aren't collections, instances of
         * <b style="color: orange;">Iterator</b> and <b style="color: orange;">Iterator</b> can also allow modifications
         * to be written through to the backing collection, and in some cases,
         * modifications to the backing collection will be visible to the Iterator
         * during iteration.
         *
         * <h2> Unmodifiable Collections </h2>
         *
         * <p>
         * Certain methods of this interface are considered "destructive" and are called
         * "mutator" methods in that they modify the group of objects contained within
         * the collection on which they operate. They can be specified to throw
         * <b>UnsupportedMethodException</b> if this collection implementation
         * does not support the operation. Such methods should (but are not required
         * to) throw an <b>UnsupportedMethodException</b> if the invocation would
         * have no effect on the collection. For example, consider a collection that
         * does not support the <b style="color: orange;"> add </b> operation. What will happen if the
         * <b style="color: orange;"> addAll </b> method is invoked on this collection, with an empty
         * collection as the argument? The addition of zero elements has no effect,
         * so it is permissible for this collection simply to do nothing and not to throw
         * an exception. However, it is recommended that such cases throw an exception
         * unconditionally, as throwing only in certain cases can lead to
         * programming errors.
         *
         * <p>
         * An <i>unmodifiable collection</i> is a collection, all of whose
         * mutator methods (as defined above) are specified to throw
         * <b>UnsupportedMethodException</b>. Such a collection thus cannot be
         * modified by calling any methods on it. For a collection to be properly
         * unmodifiable, any view collections derived from it must also be unmodifiable.
         * For example, if a List is unmodifiable, the List returned by
         * <b style="color: orange;"> List.subList </b> is also unmodifiable.
         *
         * <p>
         * An unmodifiable collection is not necessarily immutable. If the
         * contained elements are mutable, the entire collection is clearly
         * mutable, even though it might be unmodifiable. For example, consider
         * two unmodifiable lists containing mutable elements. The result of calling
         * <b>list1.equals(list2)</b> might differ from one call to the next if
         * the elements had been mutated, even though both lists are unmodifiable.
         * However, if an unmodifiable collection contains all immutable elements,
         * it can be considered effectively immutable.
         *
         * <h2> Unmodifiable View Collections </h2>
         *
         * <p>
         * An <i>unmodifiable view collection</i> is a collection that is unmodifiable
         * and that is also a view onto a backing collection. Its mutator methods throw
         * <b>UnsupportedMethodException</b>, as described above, while
         * reading and querying methods are delegated to the backing collection.
         * The effect is to provide read-only access to the backing collection.
         * This is useful for a component to provide users with read access to
         * an internal collection, while preventing them from modifying such
         * collections unexpectedly.
         *
         * <p>
         * Note that changes to the backing collection might still be possible,
         * and if they occur, they are visible through the unmodifiable view. Thus,
         * an unmodifiable view collection is not necessarily immutable. However,
         * if the backing collection of an unmodifiable view is effectively immutable,
         * or if the only reference to the backing collection is through an
         * unmodifiable view, the view can be considered effectively immutable.
         *
         * <h2> Serializability of Collections </h2>
         *
         * <p>
         * Serializability of collections is optional. As such, none of the collections
         * interfaces are declared to implement the <b style="color: orange;">core.Serializable</b>
         * interface. However, serializability is regarded as being generally useful, so most collection
         * implementations are serializable.
         *
         * <p>
         * The collection implementations that are public classes (such as <b>ArrayList</b>
         * or <b>HashMap</b>) are declared to implement the <b>Serializable</b> interface if they
         * are in fact serializable. Some collections implementations are not public classes,
         * such as the <a href="">unmodifiable collections.</a> In such cases, the
         * serializability of such collections is described in the specification of the method
         * that creates them, or in some other suitable place. In cases where the serializability
         * of a collection is not specified, there is no guarantee about the serializability of such
         * collections. In particular, many <a href="">view collections</a> are not serializable.
         *
         * <p>
         * A collection implementation that implements the <b>Serializable</b> interface cannot
         * be guaranteed to be serializable. The reason is that in general, collections
         * contain elements of other types, and it is not possible to determine statically
         * whether instances of some element type are actually serializable. For example, consider
         * a serializable <b>Collection<E></b>, where <b>E</b> does not implement the
         * <b>Serializable</b> interface. The collection may be serializable, if it contains only
         * elements of some serializable subtype of <b>E</b>, or if it is empty. Collections are
         * thus said to be <i>conditionally serializable,</i> as the serializability of the collection
         * as a whole depends on whether the collection itself is serializable and on whether all
         * contained elements are also serializable.
         *
         * <p>
         * An additional case occurs with instances of <b style="color: orange;">sorted Set</b> and
         * <b style="color: orange;">sorted Map</b>. These collections can be created with a
         * <b style="color: orange;">Comparator</b> that imposes an ordering on
         * the set elements or map keys. Such a collection is serializable only if the provided
         * <b>Comparator</b> is also serializable.
         *
         * <p>
         * Note: The default method implementations (inherited or otherwise) do not apply any
         * synchronization protocol.  If a <b>Collection</b> implementation has a
         * specific synchronization protocol, then it must override default
         * implementations to apply that protocol.
         *
         * @param E the type of elements in this collection
         *
         * @author  Brunshweeck Tazeussong
         *
         * @see     Set
         * @see     List
         * @see     Map
         * @see     HashSet
         * @see     TreeSet
         * @see     ArrayList
         * @see     LinkedList
         * @see     Vector
         * @see     Arrays
         */
        template<class E>
        interface Collection : public Object {
        private:
            CORE_ALIAS(U, native::Unsafe);

            /**
             * Capture<T> represent all type T that extends this value type E.
             * in other word E is base of T (Class<E>::isSuper<T>() is true).
             */
            template<class T>
            CORE_ALIAS(Capture, typename Class<T>::template Iff<Class<E>::template isSuper<T>()>);

        public:

            /**
             * Returns the number of elements in this collection.  If this collection
             * contains more than <b>Integer.MAX_VALUE</b> elements, returns
             * <b>Integer.MAX_VALUE</b>.
             */
            virtual gint size() const = 0;

            /**
             * Returns <b>true</b> if this collection contains no elements.
             */
            inline gbool isEmpty() const { return size() == 0; }

            /**
             * Returns <b>true</b> if this collection contains the specified element.
             * More formally, returns <b>true</b> if and only if this collection
             * contains at least one element <b>e</b> such that
             * <b>Object.equals(o, e)</b>.
             *
             * @param o element whose presence in this collection is to be tested
             *
             * @throws CastException if the type of the specified element
             *         is incompatible with this collection (optional)
             */
            virtual gbool contains(const E &o) const {
                Iterator<const E> &it = iterator();
                while (it.hasNext()) if (o.equals(it.next())) return true;
                return false;
            }

            /**
             * Returns an iterator over the elements in this collection.  There are no
             * guarantees concerning the order in which the elements are returned
             * (unless this collection is an instance of some class that provides a
             * guarantee).
             */
            virtual Iterator<const E> &iterator() const = 0;

            /**
             * Performs the given action for each element of the <b>Iterable</b>
             * until all elements have been processed or the action throws an
             * exception.  Actions are performed in the order of iteration, if that
             * order is specified.  Exceptions thrown by the action are relayed to the
             * caller.
             * <p>
             * The behavior of this method is unspecified if the action performs
             * side-effects that modify the underlying source of elements, unless an
             * overriding class has specified a concurrent modification policy.
             *
             * @implSpec
             * <p>The default implementation behaves as if:
             * <pre><b>
             *     for (T t : this)
             *         action.accept(t);
             * </b></pre>
             *
             * @param action The action to be performed for each element
             */
            virtual void forEach(const Consumer<E> &action) const {
                Iterator<const E> &it = iterator();
                while (it.hasNext()) action.accept(it.next());
            }

            /**
             * Returns an array containing all of the elements in this collection.
             * If this collection makes any guarantees as to what order its elements
             * are returned by its iterator, this method must return the elements in
             * the same order. The returned array's <b style="color: green;"> runtime component type</b>
             * is <b> Object</b>.
             *
             * <p>The returned array will be "safe" in that no references to it are
             * maintained by this collection.  (In other words, this method must
             * allocate a new array even if this collection is backed by an array).
             * The caller is thus free to modify the returned array.
             *
             * @note
             * This method acts as a bridge between array-based and collection-based APIs.
             * It returns an array whose runtime type is <b> ReferenceArray </b>.
             *
             * @return an array, whose <b style="color: green;"> runtime component type</b>
             *          is <b> Object</b>, containing all of the elements in this collection
             */
            virtual ReferenceArray toArray() const {
                // Estimate size of array; be prepared to see more or fewer elements
                ReferenceArray r = ReferenceArray(size());
                Iterator<const E> &it = iterator();
                for (gint i = 0; i < r.length(); i++) {
                    if (!it.hasNext()) {
                        // fewer elements than expected
                        ReferenceArray copy = ReferenceArray(i);
                        for (gint j = 0; j < i; ++j)
                            copy.set(j, r[j]);
                        return copy;
                    }
                    r.set(i, (Object &) it.next());
                }
                return r;
            }

            /**
             * Ensures that this collection contains the specified element (optional
             * operation).  Returns <b>true</b> if this collection changed as a
             * result of the call.  (Returns <b>false</b> if this collection does
             * not permit duplicates and already contains the specified element.)
             *
             * <p>
             * Collections that support this operation may place limitations on what
             * elements may be added to this collection.  In particular, some
             * collections will refuse to add <b>null</b> elements, and others will
             * impose restrictions on the type of elements that may be added.
             * Collection classes should clearly specify in their documentation any
             * restrictions on what elements may be added.
             *
             * <p>
             * If a collection refuses to add a particular element for any reason
             * other than that it already contains the element, it <i>must</i> throw
             * an exception (rather than returning <b>false</b>).  This preserves
             * the invariant that a collection always contains the specified element
             * after this call returns.
             *
             * @param e element whose presence in this collection is to be ensured
             * @return <b>true</b> if this collection changed as a result of the
             *         call
             * @throws UnsupportedMethodException if the <b>add</b> operation
             *         is not supported by this collection
             * @throws CastException if the class of the specified element
             *         prevents it from being added to this collection
             * @throws ArgumentException if some property of the element
             *         prevents it from being added to this collection
             * @throws StateException if the element cannot be added at this
             *         time due to insertion restrictions
             */
            virtual gbool add(const E &e) { UnsupportedMethodException().throws(__trace("core.util.Collection")); }

            /**
             * Removes a single instance of the specified element from this
             * collection, if it is present (optional operation).  More formally,
             * removes an element <b>e</b> such that <b>Object::equals(o, e)</b>, if
             * this collection contains one or more such elements.  Returns
             * <b>true</b> if this collection contained the specified element (or
             * equivalently, if this collection changed as a result of the call).
             *
             * @param o element to be removed from this collection, if present
             * @return <b>true</b> if an element was removed as a result of this call
             * @throws CastException if the type of the specified element
             *         is incompatible with this collection (optional)
             * @throws UnsupportedMethodException if the <b>remove</b> operation
             *         is not supported by this collection
             */
            virtual gbool remove(const E &o) {
                Iterator<const E> &it = iterator();
                while (it.hasNext()) {
                    if (o.equals(it.next())) {
                        it.remove();
                        return true;
                    }
                }
                return false;
            }

            /**
             * Returns <b>true</b> if this collection contains all of the elements
             * in the specified collection.
             *
             * @param  c collection to be checked for containment in this collection
             * @throws CastException if the types of one or more elements
             *         in the specified collection are incompatible with this
             *         collection (optional)
             * @see    contains(Object)
             */
            virtual gbool containsAll(const Collection &c) const {
                if (c.size() == 0)
                    return false;
                if (this == &c)
                    return true;
                for (const E &e: c)
                    if (!contains((const E &) e))
                        return false;
                return true;
            }

            /**
             * Adds all of the elements in the specified collection to this collection
             * (optional operation).  The behavior of this operation is undefined if
             * the specified collection is modified while the operation is in progress.
             * (This implies that the behavior of this call is undefined if the
             * specified collection is this collection, and this collection is
             * nonempty.)
             *
             * @param c collection containing elements to be added to this collection
             * @return <b>true</b> if this collection changed as a result of the call
             * @throws UnsupportedMethodException if the <b>addAll</b> operation
             *         is not supported by this collection
             * @throws CastException if the class of an element of the specified
             *         collection prevents it from being added to this collection
             * @throws ArgumentException if some property of an element of the
             *         specified collection prevents it from being added to this
             *         collection
             * @throws StateException if not all the elements can be added at
             *         this time due to insertion restrictions
             * @see add(Object)
             */
            virtual gbool addAll(const Collection &c) {
                gbool modified = false;
                for (const E &e: c)
                    if (add((const E &) e))
                        modified = true;
                return modified;
            }

            /**
             * Removes all of this collection's elements that are also contained in the
             * specified collection (optional operation).  After this call returns,
             * this collection will contain no elements in common with the specified
             * collection.
             *
             * @param c collection containing elements to be removed from this collection
             * @return <b>true</b> if this collection changed as a result of the
             *         call
             * @throws UnsupportedMethodException if the <b>removeAll</b> method
             *         is not supported by this collection
             * @throws CastException if the types of one or more elements
             *         in this collection are incompatible with the specified
             *         collection (optional)
             * @see remove(Object)
             * @see contains(Object)
             */
            virtual gbool removeAll(const Collection &c) {
                gbool modified = false;
                Iterator<const E> &it = iterator();
                while (it.hasNext()) {
                    if (c.contains(it.next())) {
                        it.remove();
                        modified = true;
                    }
                }
                return modified;
            }

            /**
             * Removes all of the elements of this collection that satisfy the given
             * predicate.  Errors or runtime exceptions thrown during iteration or by
             * the predicate are relayed to the caller.
             *
             * <p>
             * Note: The default implementation traverses all elements of the collection using
             * its <b style="color: orange;">iterator</b>.  Each matching element is removed using
             * <b style="color: orange;">Iterator::remove()</b>.  If the collection's iterator does not
             * support removal then an <b>UnsupportedMethodException</b> will be
             * thrown on the first matching element.
             *
             * @param filter a predicate which returns <b>true</b> for elements to be
             *        removed
             * @return <b>true</b> if any elements were removed
             * @throws UnsupportedMethodException if elements cannot be removed
             *         from this collection.  Implementations may throw this exception if a
             *         matching element cannot be removed or if, in general, removal is not
             *         supported.
             */
            virtual gbool removeIf(const Predicate<E> &filter) {
                gbool modified = false;
                Iterator<const E> &it = iterator();
                while (it.hasNext()) {
                    if (filter.test(it.next())) {
                        it.remove();
                        modified = true;
                    }
                }
                return modified;
            }

            /**
             * Retains only the elements in this collection that are contained in the
             * specified collection (optional operation).  In other words, removes from
             * this collection all of its elements that are not contained in the
             * specified collection.
             *
             * @param c collection containing elements to be retained in this collection
             * @return <b>true</b> if this collection changed as a result of the call
             * @throws UnsupportedMethodException if the <b>retainAll</b> operation
             *         is not supported by this collection
             * @throws CastException if the types of one or more elements
             *         in this collection are incompatible with the specified
             *         collection (optional)
             * @see remove(Object)
             * @see contains(Object)
             */
            virtual gbool retainAll(const Collection<E> &c) {
                gbool modified = false;
                Iterator<const E> &it = iterator();
                while (it.hasNext()) {
                    if (!c.contains((const E &) it.next())) {
                        it.remove();
                        modified = true;
                    }
                }
                return modified;
            }

            /**
             * Removes all of the elements from this collection (optional operation).
             * The collection will be empty after this method returns.
             *
             * @throws UnsupportedMethodException if the <b>clear</b> operation
             *         is not supported by this collection
             */
            virtual void clear() {
                Iterator<const E> &it = iterator();
                while (it.hasNext()) {
                    it.next();
                    it.remove();
                }
            }

            /**
             * Compares the specified object with this collection for equality. <p>
             *
             * While the <b>Collection</b> interface adds no stipulations to the
             * general contract for the <b>Object.equals</b>, programmers who
             * implement the <b>Collection</b> interface "directly" (in other words,
             * create a class that is a <b>Collection</b> but is not a <b>Set</b>
             * or a <b>List</b>) must exercise care if they choose to override the
             * <b>Object.equals</b>.  It is not necessary to do so, and the simplest
             * course of action is to rely on <b>Object</b>'s implementation, but
             * the implementor may wish to implement a "value comparison" in place of
             * the default "reference comparison."  (The <b>List</b> and
             * <b>Set</b> interfaces mandate such value comparisons.)<p>
             *
             * The general contract for the <b>Object.equals</b> method states that
             * equals must be symmetric (in other words, <b>a.equals(b)</b> if and
             * only if <b>b.equals(a)</b>).  The contracts for <b>List.equals</b>
             * and <b>Set.equals</b> state that lists are only equal to other lists,
             * and sets to other sets.  Thus, a custom <b>equals</b> method for a
             * collection class that implements neither the <b>List</b> nor
             * <b>Set</b> interface must return <b>false</b> when this collection
             * is compared to any list or set.  (By the same logic, it is not possible
             * to write a class that correctly implements both the <b>Set</b> and
             * <b>List</b> interfaces.)
             *
             * @param o object to be compared for equality with this collection
             * @return <b>true</b> if the specified object is equal to this
             * collection
             *
             * @see Object::equals(Object)
             * @see Set.equals(Object)
             * @see List.equals(Object)
             */
            gbool equals(const Object &o) const override { return Object::equals(o); }

            /**
             * Returns a string representation of this collection.  The string
             * representation consists of a list of the collection's elements in the
             * order they are returned by its iterator, enclosed in square brackets
             * (<b> "[]"</b>).  Adjacent elements are separated by the characters
             * <b> ", "</b> (comma and space).  Elements are converted to strings as
             * by <b style="color: orange;"> String.valueOf(Object)</b>.
             */
            String toString() const override {
                Iterator<const E> &it = iterator();
                if (!it.hasNext())
                    return "[]";

                StringBuffer sb = {};
                sb.append('[');
                for (;;) {
                    const E &e = it.next();
                    if (Collection<E>::equals(e)) sb.append("?"); else sb.append(e);
                    if (!it.hasNext()) return sb.append(']').toString();
                    sb.append(',').append(' ');
                }
            }

        protected:
            // Native iteration
            // for(auto& o: c) {
            //    ...
            // }
            //
            template<class T = E>
            class Itr CORE_FINAL : public Object {
            private:
                const Collection &root;
                ReferenceArray array = {};
                gint cursor = {};

            public:
                /**
                 * Create new Native Iterator instance to mark begin of iterations
                 */
                Itr(const Collection &root, const ReferenceArray &a) : root(root), array(a) {}

                /**
                 * Create new Native Iterator instance to mark the end of iterations
                 */
                Itr(const Collection &root) : root(root), cursor(root.size()) {}

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<Itr>::hasInstance(o))
                        return false;
                    const Itr &it = (Itr &) o;
                    return it.cursor == cursor && &it.root == &root;
                }

                /**
                 * Return the next element (it similar to core.util.Iterator.next())
                 */
                inline T &operator*() {
                    if (cursor == array.length())
                        StateException("").throws(__trace("core.util.Collection.Itr"));
                    return (T &) array[cursor++];
                }

                inline Itr &operator++() { return *this; }
            };

        public:
            /**
             * Return The native iterator (The C iterator) used
             * to mark the beginning of foreach statement.
             * by default collection not support modification during each
             */
            inline Itr<const E> begin() const { return Itr<const E>(*this, toArray()); }

            /**
             * Return The native iterator (The C iterator) used
             * to mark the ending of foreach statement.
             * by default collection not support modification during each
             */
            inline Itr<const E> end() const { return Itr<const E>(*this); }

            CORE_STATIC_ASSERT(Class<Object>::template isSuper<E>(),
                               "The valid parameters type must be a class deriving from core.Object");

            CORE_STATIC_ASSERT(!Class<E>::isReference() && !Class<E>::isConstant() && !Class<E>::isVolatile(),
                               "The valid parameters type mustn't have qualifiers (const, volatile, &, &&, ...)");
        };

    }
} // core

#endif //CORE23_COLLECTION_H
