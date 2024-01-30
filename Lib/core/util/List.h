//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_LIST_H
#define CORE23_LIST_H

#include <core/util/Collection.h>
#include <core/util/Preconditions.h>
#include <core/util/ListIterator.h>
#include <core/function/Function.h>
#include <core/IndexException.h>

namespace core {
    namespace util {

        /**
         * An ordered collection (also known as a <i>sequence</i>).  The user of this
         * interface has precise control over where in the list each element is
         * inserted.  The user can access elements by their integer index (position in
         * the list), and search for elements in the list.<p>
         *
         * Unlike sets, lists typically allow duplicate elements.  More formally,
         * lists typically allow pairs of elements <b> e1</b> and <b> e2</b>
         * such that <b> e1.equals(e2)</b>.  It is not inconceivable
         * that someone might wish to implement a list that prohibits duplicates, by
         * throwing runtime exceptions when the user attempts to insert them, but we
         * expect this usage to be rare.<p>
         *
         * The <b> List</b> interface places additional stipulations, beyond those
         * specified in the <b> Collection</b> interface, on the contracts of the
         * <b> iterator</b>, <b> add</b>, <b> remove</b>, <b> equals</b>, and
         * <b> hashCode</b> methods.  Declarations for other inherited methods are
         * also included here for convenience.<p>
         *
         * The <b> List</b> interface provides four methods for positional (indexed)
         * access to list elements.  Lists (like arrays) are zero based.  Note
         * that these operations may execute in time proportional to the index value
         * for some implementations (the <b> LinkedList</b> class, for
         * example). Thus, iterating over the elements in a list is typically
         * preferable to indexing through it if the caller does not know the
         * implementation.<p>
         *
         * The <b> List</b> interface provides a special iterator, called a
         * <b> ListIterator</b>, that allows element insertion and replacement, and
         * bidirectional access in addition to the normal operations that the
         * <b> Iterator</b> interface provides.  A method is provided to obtain a
         * list iterator that starts at a specified position in the list.<p>
         *
         * The <b> List</b> interface provides two methods to search for a specified
         * object.  From a performance standpoint, these methods should be used with
         * caution.  In many implementations they will perform costly linear
         * searches.<p>
         *
         * The <b> List</b> interface provides two methods to efficiently insert and
         * remove multiple elements at an arbitrary point in the list.<p>
         *
         * Note: While it is permissible for lists to contain themselves as elements,
         * extreme caution is advised: the <b> equals</b> and <b> hashCode</b>
         * methods are no longer well defined on such a list.
         *
         * <p>Some list implementations have restrictions on the elements that
         * they may contain.  For example, some have restrictions on the types of their elements.
         * Attempting to add an ineligible element throws an unchecked exception, typically
         * <b> ClassCastException</b>.  Attempting to query the presence of an ineligible element may
         * throw an exception, or it may simply return false; some implementations will exhibit the former
         * behavior and some will exhibit the latter.  More generally, attempting an
         * operation on an ineligible element whose completion would not result in
         * the insertion of an ineligible element into the list may throw an
         * exception or it may succeed, at the option of the implementation.
         * Such exceptions are marked as "optional" in the specification for this
         * interface.
         *
         * <h2><a id="">Unmodifiable Lists</a></h2>
         * <p>The <b style="color: orange;"> List.of</b> and
         * <b style="color: orange;"> List.copyOf</b> static factory methods
         * provide a convenient way to create unmodifiable lists. The <b> List</b>
         * instances created by these methods have the following characteristics:
         *
         * <ul>
         * <li>They are <a href=""><i>unmodifiable</i></a>. Elements cannot
         * be added, removed, or replaced. Calling any mutator method on the List
         * will always cause <b> UnsupportedOperationException</b> to be thrown.
         * However, if the contained elements are themselves mutable,
         * this may cause the List's contents to appear to change.
         * <li>They are serializable if all elements are serializable.
         * <li>The order of elements in the list is the same as the order of the
         * provided arguments, or of the elements in the provided root.
         * <li>They are <a href="">value-based</a>.
         * Programmers should treat instances that are <b style="color: green;"> equal</b>
         * as interchangeable and should not use them for synchronization, or
         * unpredictable behavior may occur. For example, in a future release,
         * synchronization may fail. Callers should make no assumptions about the
         * identity of the returned instances. Factories are free to
         * create new instances or reuse existing ones.
         * <li>They are serialized as specified on the
         * <a href="">Serialized Form</a> page.
         * </ul>
         *
         * @param E the type of elements in this list
         *
         * @author  Brunshweeck Tazeussong
         * @see Collection
         * @see Set
         * @see ArrayList
         * @see LinkedList
         * @see Vector
         */
        template<class E>
        class List : public Collection<E> {
        public:

            /**
             * Returns the number of elements in this list.  If this list contains
             * more than <b> Integer.MAX_VALUE</b> elements, returns
             * <b> Integer.MAX_VALUE</b>.
             *
             * @return the number of elements in this list
             */
            using Collection<E>::size;

            /**
             * Returns <b> true</b> if this list contains the specified element.
             * More formally, returns <b> true</b> if and only if this list contains
             * at least one element <b> e</b> such that
             * <b> Object.equals(o, e)</b>.
             *
             * @param o element whose presence in this list is to be tested
             * @return <b> true</b> if this list contains the specified element
             * @throws ClassCastException if the type of the specified element
             *         is incompatible with this list (<a href="">optional</a>)
             */
            using Collection<E>::contains;

            /**
             * Returns an iterator over the elements in this list in proper sequence.
             */
            ListIterator<const E> &iterator() const override {
                return iterator(0);
            }

            /**
             * Returns an root containing all of the elements in this list in proper
             * sequence (from first to last element).
             *
             * <p>The returned root will be "safe" in that no references to it are
             * maintained by this list.  (In other words, this method must
             * allocate a new root even if this list is backed by an root).
             * The caller is thus free to modify the returned root.
             *
             * <p>This method acts as bridge between root-based and collection-based
             * APIs.
             *
             * @return an root containing all of the elements in this list in proper
             *         sequence
             */
            using Collection<E>::toArray;

            /**
             * Appends the specified element to the end of this list (optional operation).
             *
             * <p>Lists that support this operation may place limitations on what
             * elements may be added to this list.  In particular, some
             * lists will impose restrictions on the type of elements that may be added.  List
             * classes should clearly specify in their documentation any restrictions
             * on what elements may be added.
             *
             * @param e element to be appended to this list
             * @return <b> true</b> (as specified by <b style="color: orange;"> Collection.add</b>)
             * @throws UnsupportedMethodException if the <b> add</b> operation
             *         is not supported by this list
             * @throws ClassCastException if the class of the specified element
             *         prevents it from being added to this list
             * @throws IllegalArgumentException if some property of this element
             *         prevents it from being added to this list
             */
            gbool add(const E &e) override {
                add(size(), e);
                return true;
            }

            /**
             * Removes the first occurrence of the specified element from this list,
             * if it is present (optional operation).  If this list does not contain
             * the element, it is unchanged.  More formally, removes the element with
             * the lowest index <b> i</b> such that
             * <b> Object.equals(o, get(i))</b>
             * (if such an element exists).  Returns <b> true</b> if this list
             * contained the specified element (or equivalently, if this list changed
             * as a result of the call).
             *
             * @param o element to be removed from this list, if present
             * @return <b> true</b> if this list contained the specified element
             * @throws ClassCastException if the type of the specified element
             *         is incompatible with this list (<a href="">optional</a>)
             * @throws UnsupportedMethodException if the <b> remove</b> operation
             *         is not supported by this list
             */
            gbool remove(const E &o) override {
                gint i = indexOf(o);
                if (i >= 0) {
                    removeAt(i);
                    return true;
                }
                return false;
            }

            /**
             * Returns <b> true</b> if this list contains all of the elements of the
             * specified collection.
             *
             * @param  c collection to be checked for containment in this list
             *
             * @see List.contains(Object)
             */
            using Collection<E>::containsAll;

            /**
             * Appends all of the elements in the specified collection to the end of
             * this list, in the order that they are returned by the specified
             * collection's iterator (optional operation).  The behavior of this
             * operation is undefined if the specified collection is modified while
             * the operation is in progress.  (Note that this will occur if the
             * specified collection is this list, and it's nonempty.)
             *
             * @param c collection containing elements to be added to this list
             * @return <b> true</b> if this list changed as a result of the call
             * @throws UnsupportedMethodException if the <b> addAll</b> operation
             *         is not supported by this list
             * @throws ClassCastException if the class of an element of the specified
             *         collection prevents it from being added to this list
             * @throws IllegalArgumentException if some property of an element of the
             *         specified collection prevents it from being added to this list
             * @see List.add(Object)
             */
            gbool addAll(const Collection<E> &c) override {
                return addAll(size(), c);
            }

            /**
             * Inserts all of the elements in the specified collection into this
             * list at the specified position (optional operation).  Shifts the
             * element currently at that position (if any) and any subsequent
             * elements to the right (increases their indices).  The new elements
             * will appear in this list in the order that they are returned by the
             * specified collection's iterator.  The behavior of this operation is
             * undefined if the specified collection is modified while the
             * operation is in progress.  (Note that this will occur if the specified
             * collection is this list, and it's nonempty.)
             *
             * @param index index at which to insert the first element from the
             *              specified collection
             * @param c collection containing elements to be added to this list
             * @return <b> true</b> if this list changed as a result of the call
             * @throws UnsupportedMethodException if the <b> addAll</b> operation
             *         is not supported by this list
             * @throws ClassCastException if the class of an element of the specified
             *         collection prevents it from being added to this list
             * @throws IllegalArgumentException if some property of an element of the
             *         specified collection prevents it from being added to this list
             * @throws IndexException if the index is out of range (<b> index < 0 || index > size()</b>)
             */
            virtual gbool addAll(gint index, const Collection<E> &c) {
                try {
                    Preconditions::checkIndexForAdding(index, size());
                    gbool modified = false;
                    for (const E &e: c) {
                        add(index++, e);
                        modified = true;
                    }
                    return modified;
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.util.List"));
                }
            }

            /**
             * Removes from this list all of its elements that are contained in the
             * specified collection (optional operation).
             *
             * @param c collection containing elements to be removed from this list
             * @return <b> true</b> if this list changed as a result of the call
             * @throws UnsupportedMethodException if the <b> removeAll</b> operation
             *         is not supported by this list
             * @throws ClassCastException if the class of an element of this list
             *         is incompatible with the specified collection (<a href="">optional</a>)
             * @see List.remove(Object)
             * @see List.contains(Object)
             */
            using Collection<E>::removeAll;

            /**
             * Retains only the elements in this list that are contained in the
             * specified collection (optional operation).  In other words, removes
             * from this list all of its elements that are not contained in the
             * specified collection.
             *
             * @param c collection containing elements to be retained in this list
             * @return <b> true</b> if this list changed as a result of the call
             * @throws UnsupportedMethodException if the <b> retainAll</b> operation
             *         is not supported by this list
             * @throws ClassCastException if the class of an element of this list
             *         is incompatible with the specified collection (<a href="">optional</a>)
             * @see List.remove(Object)
             * @see List.contains(Object)
             */
            using Collection<E>::retainAll;

            /**
             * Replaces each element of this list with the result of applying the
             * operator to that element.  Errors or runtime exceptions thrown by
             * the operator are relayed to the caller.
             *
             * @implSpec
             * The default implementation is equivalent to, for this <b> list</b>:
             * <pre><b>
             *     Iterator<E> li = list.iterator();
             *     while (li.hasNext()) {
             *         li.setValue(operator.apply(li.next()));
             *     </b>
             * </b></pre>
             *
             * If the list's list-iterator does not support the <b> setValue</b> operation
             * then an <b> UnsupportedOperationException</b> will be thrown when
             * replacing the first element.
             *
             * @param operator the operator to apply to each element
             * @throws UnsupportedMethodException if this list is unmodifiable.
             *         Implementations may throw this exception if an element
             *         cannot be replaced or if, in general, modification is not
             *         supported (<a href="">optional</a>)
             */
            virtual void replaceAll(const Function<E, E> &op) {
                ListIterator<E> &it = iterator();
                while (it.hasNext()) {
                    it.set(op.apply(it.next()));
                }
            }

            /**
             * Removes all of the elements from this list (optional operation).
             * The list will be empty after this call returns.
             *
             * @throws UnsupportedMethodException if the <b> clear</b> operation
             *         is not supported by this list
             */
            using Collection<E>::clear;

            /**
             * Compares the specified object with this list for equality.  Returns
             * <b> true</b> if and only if the specified object is also a list, both
             * lists have the same size, and all corresponding pairs of elements in
             * the two lists are <i>equal</i>.  (Two elements <b> e1</b> and
             * <b> e2</b> are <i>equal</i> if <b> Object.equals(e1, e2)</b>.)
             * In other words, two lists are defined to be
             * equal if they contain the same elements in the same order.  This
             * definition ensures that the equals method works properly across
             * different implementations of the <b> List</b> interface.
             *
             * @param o the object to be compared for equality with this list
             * @return <b> true</b> if the specified object is equal to this list
             */
            gbool equals(const Object &o) const override {
                if (&o == this) {
                    return true;
                }
                if (!Class<List>::hasInstance(o)) {
                    return false;
                }

                ListIterator<const E> &e1 = iterator();
                ListIterator<const E> &e2 = ((const List &) o).iterator();
                while (e1.hasNext() && e2.hasNext()) {
                    if (!Object::equals(e1.next(), e2.next())) {
                        return false;
                    }
                }
                return !(e1.hasNext() || e2.hasNext());
            }

            /**
             * Returns the element at the specified position in this list.
             *
             * @param index index of the element to return
             * @return the element at the specified position in this list
             * @throws IndexException if the index is out of range
             *         (<b> index < 0 || index >= size()</b>)
             */
            virtual E &get(gint index) = 0;

            /**
             * Returns the element at the specified position in this list.
             *
             * @param index index of the element to return
             * @return the element at the specified position in this list
             * @throws IndexException if the index is out of range
             *         (<b> index < 0 || index >= size()</b>)
             */
            virtual const E &get(gint index) const = 0;

            /**
             * Replaces the element at the specified position in this list with the
             * specified element (optional operation).
             *
             * @param index index of the element to replace
             * @param element element to be stored at the specified position
             * @return the element previously at the specified position
             * @throws UnsupportedMethodException if the <b> setValue</b> operation
             *         is not supported by this list
             * @throws ClassCastException if the class of the specified element
             *         prevents it from being added to this list
             * @throws IllegalArgumentException if some property of the specified
             *         element prevents it from being added to this list
             * @throws IndexException if the index is out of range
             *         (<b> index < 0 || index >= size()</b>)
             */
            virtual const E &set(gint index, const E &element) {
                CORE_IGNORE(index);
                CORE_IGNORE(element);
                UnsupportedOperationException().throws(__trace("core.util.List"));
            }

            /**
             * Inserts the specified element at the specified position in this list
             * (optional operation).  Shifts the element currently at that position
             * (if any) and any subsequent elements to the right (adds one to their
             * indices).
             *
             * @param index index at which the specified element is to be inserted
             * @param element element to be inserted
             * @throws UnsupportedMethodException if the <b> add</b> operation
             *         is not supported by this list
             * @throws ClassCastException if the class of the specified element
             *         prevents it from being added to this list
             * @throws IllegalArgumentException if some property of the specified
             *         element prevents it from being added to this list
             * @throws IndexException if the index is out of range
             *         (<b> index < 0 || index > size()</b>)
             */
            virtual void add(gint index, const E &element) {
                CORE_IGNORE(index);
                CORE_IGNORE(element);
                UnsupportedOperationException().throws(__trace("core.util.List"));
            }

            /**
             * Removes the element at the specified position in this list (optional
             * operation).  Shifts any subsequent elements to the left (subtracts one
             * from their indices).  Returns the element that was removed from the
             * list.
             *
             * @param index the index of the element to be removed
             * @return the element previously at the specified position
             * @throws UnsupportedMethodException if the <b> remove</b> operation
             *         is not supported by this list
             * @throws IndexException if the index is out of range
             *         (<b> index < 0 || index >= size()</b>)
             */
            virtual const E &removeAt(gint index) {
                CORE_IGNORE(index);
                UnsupportedOperationException().throws(__trace("core.util.List"));
            }

            /**
             * Returns the index of the first occurrence of the specified element
             * in this list, or -1 if this list does not contain the element.
             * More formally, returns the lowest index <b> i</b> such that
             * <b> Object.equals(o, get(i))</b>,
             * or -1 if there is no such index.
             *
             * @param o element to search for
             * @return the index of the first occurrence of the specified element in
             *         this list, or -1 if this list does not contain the element
             * @throws ClassCastException if the type of the specified element
             *         is incompatible with this list
             *         (<a href="">optional</a>)
             */
            virtual gint indexOf(const E &o) const {
                ListIterator<const E> &itr = iterator();
                gint i = 0;
                while (itr.hasNext()) {
                    if (Object::equals(o, itr.next())) {
                        return i;
                    }
                    i += 1;
                }
                return -1;
            }

            /**
             * Returns the index of the last occurrence of the specified element
             * in this list, or -1 if this list does not contain the element.
             * More formally, returns the highest index <b> i</b> such that
             * <b> Object.equals(o, get(i))</b>,
             * or -1 if there is no such index.
             *
             * @param o element to search for
             * @return the index of the last occurrence of the specified element in
             *         this list, or -1 if this list does not contain the element
             * @throws ClassCastException if the type of the specified element
             *         is incompatible with this list
             *         (<a href="">optional</a>)
             */
            virtual gint lastIndexOf(const E &o) const {
                ListIterator<const E> &itr = iterator(size());
                gint i = 0;
                while (itr.hasPrevious()) {
                    if (Object::equals(o, itr.previous())) {
                        return i;
                    }
                    i += 1;
                }
                return -1;
            }

            /**
             * Returns a list iterator over the elements in this list (in proper sequence).
             */
            ListIterator<E> &iterator() override {
                return iterator(0);
            }

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
             * <pre><b> @code
             *     for (T t : this)
             *         action.accept(t);
             * @endcode </b></pre>
             *
             * @param action The action to be performed for each element
             */
            using Collection<E>::forEach;

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence), starting at the specified position in the list.
             * The specified index indicates the first element that would be
             * returned by an initial call to <b style="color: orange;">next</b>.
             * An initial call to <b style="color: orange;"> previous</b> would
             * return the element with the specified index minus one.
             *
             * @param index index of the first element to be returned from the
             *        list iterator (by a call to <b style="color: orange;"> next</b>)
             * @throws IndexException if the index is out of range
             *         (<b> index < 0 || index > size()</b>)
             */
            virtual ListIterator<E> &iterator(gint index) {
                try {
                    Preconditions::checkIndexForAdding(index, size());
                } catch (const IndexException &ie) {
                    ie.throws(__trace("core.util.List"));
                }
                CORE_ALIAS(Itr, ListItr < E >);
                return Unsafe::allocateInstance<Itr>((List &) *this, index);
            }

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence), starting at the specified position in the list.
             * The specified index indicates the first element that would be
             * returned by an initial call to <b style="color: orange;">next</b>.
             * An initial call to <b style="color: orange;"> previous</b> would
             * return the element with the specified index minus one.
             *
             * @param index index of the first element to be returned from the
             *        list iterator (by a call to <b style="color: orange;"> next</b>)
             * @throws IndexException if the index is out of range
             *         (<b> index < 0 || index > size()</b>)
             */
            virtual ListIterator<const E> &iterator(gint index) const {
                try {
                    Preconditions::checkIndexForAdding(index, size());
                } catch (const IndexException &ie) {
                    ie.throws(__trace("core.util.List"));
                }
                CORE_ALIAS(Itr, ListItr<const E>);
                return Unsafe::allocateInstance<Itr>((List &) *this, index);
            }

            /**
             * Returns a view of the portion of this list between the specified
             * <b> from</b>, inclusive, and <b> to</b>, exclusive.  (If
             * <b> from</b> and <b> to</b> are equal, the returned list is
             * empty.)  The returned list is backed by this list, so non-structural
             * changes in the returned list are reflected in this list, and vice-versa.
             * The returned list supports all of the optional list operations supported
             * by this list.<p>
             *
             * This method eliminates the need for explicit range operations (of
             * the sort that commonly exist for arrays).  Any operation that expects
             * a list can be used as a range operation by passing a subList view
             * instead of a whole list.  For example, the following idiom
             * removes a range of elements from a list:
             * <pre><b>
             *      list.subList(from, to).clear();
             * </b></pre>
             * Similar idioms may be constructed for <b> indexOf</b> and
             * <b> lastIndexOf</b>, and all of the algorithms in the
             * <b> Collections</b> class can be applied to a subList.<p>
             *
             * The semantics of the list returned by this method become undefined if
             * the backing list (i.e., this list) is <i>structurally modified</i> in
             * any way other than via the returned list.  (Structural modifications are
             * those that change the size of this list, or otherwise perturb it in such
             * a fashion that iterations in progress may yield incorrect results.)
             *
             * @param from low endpoint (inclusive) of the subList
             * @param to high endpoint (exclusive) of the subList
             * @throws IndexException for an illegal endpoint index value
             *         (<b> from < 0 || to > size ||
             *         from > to</b>)
             */
            virtual List &subList(gint from, gint to) const {
                try {
                    Preconditions::checkIndexFromRange(from, to, size());
                    return Unsafe::allocateInstance<SubList<>>((List &) *this, from, to);
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.List")); }
            }

            /**
             * Get the element at the given index
             */
            inline E &operator[](gint index) { return get(index); }

            /**
             * Get the element at the given index
             */
            inline const E &operator[](gint index) const { return get(index); }

        protected:

            /**
             * The number of times this list has been <i>structurally modified</i>.
             * Structural modifications are those that change the size of the
             * list, or otherwise perturb it in such a fashion that iterations in
             * progress may yield incorrect results.
             *
             * <p>This field is used by the iterator and list iterator implementation
             * returned by the <b>iterator</b> methods.
             * If the value of this field changes unexpectedly, the iterator (or list
             * iterator) will throw a <b>ConcurrentException</b> in
             * response to the <b>next</b>, <b>remove</b>, <b>previous</b>,
             * <b>setValue</b> or <b>add</b> operations.  This provides
             * <i>fail-fast</i> behavior, rather than non-deterministic behavior in
             * the face of concurrent modification during iteration.
             *
             * <p><b>Use of this field by subclasses is optional.</b> If a subclass
             * wishes to provide fail-fast iterators (and list iterators), then it
             * merely has to increment this field in its <b>add(gint, E)</b> and
             * <b>remove(gint)</b> methods (and any other methods that it overrides
             * that result in structural modifications to the list).  A single call to
             * <b>add(gint, E)</b> or <b>remove(gint)</b> must add no more than
             * one to this field, or the iterators (and list iterators) will throw
             * bogus <b>ConcurrentExceptions</b>.  If an implementation
             * does not wish to provide fail-fast iterators, this field may be
             * ignored.
             */
            gint modNum = {};

        private:
            template<class T>
            class ListItr : public ListIterator<T> {
            private:
                /**
                 * The target of iteration
                 */
                List &root;

                /**
                 * Index of element to be returned by subsequent call to next.
                 */
                gint cursor;

                /**
                 * Index of element returned by most recent call to next or
                 * previous.  Reset to -1 if this element is deleted by a call
                 * to remove.
                 */
                gint last;

                /**
                 * The modCount value that the iterator believes that the backing
                 * List should have.  If this expectation is violated, the iterator
                 * has detected concurrent modification.
                 */
                gint modNum;

            public:
                ListItr(List &root, gint cursor) :
                        root(root), cursor(cursor), last(-1), modNum(root.modNum) {}

                gbool hasNext() const override {
                    return cursor != root.size();
                }

                T &next() override {
                    if (modNum != root.modNum) {
                        ConcurrentException().throws(__trace("core.util.List.ListItr"));
                    }
                    try {
                        gint i = cursor;
                        T &retVal = root.get(i);
                        last = i;
                        cursor = i + 1;
                        return retVal;
                    } catch (const IndexException &ie) {
                        if (modNum != root.modNum)
                            ConcurrentException().throws(__trace("core.util.List.ListItr"));
                        NoSuchElementException().throws(__trace("core.util.List.ListItr"));
                    }
                }

                gbool hasPrevious() const override { return cursor != 0; }

                T &previous() override {
                    if (modNum != root.modNum)
                        ConcurrentException().throws(__trace("core.util.List.ListItr"));
                    try {
                        gint i = cursor - 1;
                        T &retVal = root.get(i);
                        last = cursor = i;
                        return retVal;
                    } catch (const IndexException &ie) {
                        if (modNum != root.modNum)
                            ConcurrentException().throws(__trace("core.util.List.ListItr"));
                        NoSuchElementException().throws(__trace("core.util.List.ListItr"));
                    }
                }

                void remove() override {
                    if (last < 0) {
                        IllegalStateException().throws(__trace("core.util.List.ListItr"));
                    }
                    if (modNum != root.modNum)
                        ConcurrentException().throws(__trace("core.util.List.ListItr"));
                    try {
                        root.removeAt(last);
                        if (last < cursor)
                            cursor -= 1;
                        last = -1;
                        modNum = root.modNum;
                    } catch (const IndexException &ie) {
                        ConcurrentException().throws(__trace("core.util.List.ListItr"));
                    }
                }

                void set(const T &e) override {
                    if (last < 0)
                        IllegalStateException().throws(__trace("core.util.List.ListItr"));
                    if (modNum != root.modNum)
                        ConcurrentException().throws(__trace("core.util.List.ListItr"));
                    try {
                        root.set(last, e);
                        modNum = root.modNum;
                    } catch (const IndexException &ie) {
                        if (modNum != root.modNum)
                            ConcurrentException().throws(__trace("core.util.List.ListItr"));
                        NoSuchElementException().throws(__trace("core.util.List.ListItr"));
                    }
                }

                void add(const T &e) override {
                    if (modNum != root.modNum)
                        ConcurrentException().throws(__trace("core.util.List.ListItr"));
                    try {
                        gint i = cursor;
                        root.add(i, e);
                        last = -1;
                        cursor = i + 1;
                        modNum = root.modNum;
                    } catch (const IndexException &ie) {
                        ConcurrentException().throws(__trace("core.util.List.ListItr"));
                    }
                }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<ListItr>::hasInstance(o))
                        return false;
                    ListItr &itr = CORE_CAST(ListItr &, o);
                    if (&itr.root != &root)
                        return false;
                    return cursor == itr.cursor;
                }
            };

            template<class T = E>
            class SubList : public List<T> {
            private:
                List &root;
                SubList &parent;
                gint offset;
                gint len;
                using List::modNum;

            public:

                /**
                 * Constructs a sublist of an arbitrary AbstractList, which is
                 * not a SubList itself.
                 */
                SubList(List &root, gint from, gint to) :
                        root(root), parent(*this), len(to - from), offset(from) { modNum = root.modNum; }

                /**
                 * Constructs a sublist of another SubList.
                 */
                SubList(SubList &parent, gint from, gint to) :
                        root(parent.root), parent(parent), len(to - from),
                        offset(from + parent.offset) { modNum = parent.modNum; }

                const E &set(gint index, const E &element) override {
                    try {
                        Preconditions::checkIndex(index, len);
                        if (modNum != root.modNum)
                            ConcurrentException().throws(__trace("core.util.List.SubList"));
                        return root.set(index + offset, element);
                    } catch (const IndexException &ie) { ie.throws(__trace("core.util.List.SubList")); }
                }

                E &get(gint index) override {
                    try {
                        Preconditions::checkIndex(index, len);
                        if (modNum != root.modNum) ConcurrentException().throws(__trace("core.util.List.SubList"));
                        return root.get(index + offset);
                    } catch (const IndexException &ie) { ie.throws(__trace("core.util.List.SubList")); }
                }

                const E &get(gint index) const override {
                    try {
                        Preconditions::checkIndex(index, len);
                        if (modNum != root.modNum) ConcurrentException().throws(__trace("core.util.List.SubList"));
                        return root.get(index + offset);
                    } catch (const IndexException &ie) { ie.throws(__trace("core.util.List.SubList")); }
                }

                gint size() const override {
                    if (modNum != root.modNum) ConcurrentException().throws(__trace("core.util.List.SubList"));\
                    return len;
                }

                void add(gint index, const E &element) override {
                    try {
                        Preconditions::checkIndexForAdding(index, len);
                        if (modNum != root.modNum) ConcurrentException().throws(__trace("core.util.List.SubList"));
                        root.add(index + offset, element);
                        update(1);
                    } catch (const IndexException &ie) { ie.throws(__trace("core.util.List.SubList")); }
                }

                const E &removeAt(gint index) override {
                    try {
                        Preconditions::checkIndex(index, len);
                        if (modNum != root.modNum) ConcurrentException().throws(__trace("core.util.List.SubList"));
                        const E &retVal = root.removeAt(index + offset);
                        update(-1);
                        return retVal;
                    } catch (const IndexException &ie) { ie.throws(__trace("core.util.List.SubList")); }
                }

                gbool addAll(gint index, const Collection<E> &c) override {
                    try {
                        Preconditions::checkIndexForAdding(index, len);
                        gint cSize = c.size();
                        if (cSize == 0) return false;
                        if (modNum != root.modNum) ConcurrentException().throws(__trace("core.util.List.SubList"));
                        root.addAll(index + offset, c);
                        update(cSize);
                        return true;
                    } catch (const IndexException &ie) { ie.throws(__trace("core.util.List.SubList")); }
                }

                ListIterator<E> &iterator(gint index) override {
                    class _$ : public List::ListItr<E> {
                        SubList &parent;
                    public:
                        _$(SubList &parent, gint cursor) :
                                List::ListItr<E>(parent.root, cursor + parent.offset), parent(parent) {}

                        void remove() override {
                            List::ListItr<E>::remove();
                            parent.update(-1);
                        }

                        void add(const E &e) override {
                            List::ListItr<E>::add(e);
                            parent.update(+1);
                        }
                    };
                    try {
                        Preconditions::checkIndexForAdding(index, len);
                        if (modNum != root.modNum) ConcurrentException().throws(__trace("core.util.List.SubList"));
                        return native::Unsafe::allocateInstance<_$>((SubList &) *this, index);
                    } catch (const IndexException &ie) { ie.throws(__trace("core.util.List.SubList")); }

                }

                ListIterator<const E> &iterator(gint index) const override {
                    class _$ : public List::ListItr<const E> {
                        SubList &parent;
                    public:
                        _$(SubList &parent, gint cursor) :
                                List::ListItr<const E>(parent.root, cursor + parent.offset), parent(parent) {}

                        void remove() override {
                            List::ListItr<const E>::remove();
                            parent.update(-1);
                        }

                        void add(const E &e) override {
                            List::ListItr<const E>::add(e);
                            parent.update(+1);
                        }
                    };
                    try {
                        Preconditions::checkIndexForAdding(index, len);
                        if (modNum != root.modNum) ConcurrentException().throws(__trace("core.util.List.SubList"));
                        return native::Unsafe::allocateInstance<_$>((SubList &) *this, index);
                    } catch (const IndexException &ie) { ie.throws(__trace("core.util.List.SubList")); }
                }

                List &subList(gint from, gint to) const override {
                    try {
                        Preconditions::checkIndexFromRange(from, to, len);
                        if (modNum != root.modNum) ConcurrentException().throws(__trace("core.util.List.SubList"));
                        return native::Unsafe::allocateInstance<SubList>((SubList &) *this, from, to);
                    } catch (const IndexException &ie) { ie.throws(__trace("core.util.List.SubList")); }
                }

                void update(gint s) {
                    len += s;
                    modNum = root.modNum;
                    if (&parent != this)
                        parent.update(s);
                }
            };
        };

#if CORE_TEMPLATE_TYPE_DEDUCTION
        template<class E> List(const List<E> &) -> List<E>;
#endif

    }
} // core

#endif //CORE23_LIST_H
