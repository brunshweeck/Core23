//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_ARRAYLIST_H
#define CORE23_ARRAYLIST_H

#include <core/Math.h>
#include <core/util/List.h>
#include <core/private/ArraysSupport.h>

namespace core {
    namespace util {

        /**
         * Resizable-array implementation of the <b> List</b> interface.  Implements
         * all optional list operations, and permits all elements.
         * In addition to implementing the <b> List</b> interface,
         * this class provides methods to manipulate the size of the array that is
         * used internally to store the list.  (This class is roughly equivalent to
         * <b> Vector</b>, except that it is unsynchronized.)
         *
         * <p>The <b> size</b>, <b> isEmpty</b>, <b> get</b>, <b> set</b>,
         * <b> iterator</b>, and <b> listIterator</b> operations run in constant
         * time.  The <b> add</b> operation runs in <i>amortized constant time</i>,
         * that is, adding n elements requires O(n) time.  All of the other operations
         * run in linear time (roughly speaking).  The constant factor is low compared
         * to that for the <b> LinkedList</b> implementation.
         *
         * <p>Each <b> ArrayList</b> instance has a <i>capacity</i>.  The capacity is
         * the size of the array used to store the elements in the list.  It is always
         * at least as large as the list size.  As elements are added to an ArrayList,
         * its capacity grows automatically.  The details of the growth policy are not
         * specified beyond the fact that adding an element has constant amortized
         * time cost.
         *
         * <p>An application can increase the capacity of an <b> ArrayList</b> instance
         * before adding a large number of elements using the <b> ensureCapacity</b>
         * operation.  This may reduce the amount of incremental reallocation.
         *
         * <p><strong>Note that this implementation is not synchronized.</strong>
         * If multiple threads access an <b> ArrayList</b> instance concurrently,
         * and at least one of the threads modifies the list structurally, it
         * <i>must</i> be synchronized externally.  (A structural modification is
         * any operation that adds or deletes one or more elements, or explicitly
         * resizes the backing array; merely setting the value of an element is not
         * a structural modification.)  This is typically accomplished by
         * synchronizing on some object that naturally encapsulates the list.
         *
         * <p id="fail-fast">
         * The iterators returned by this class's <b style="color: orange;"> iterator</b>
         * methods are <em>fail-fast</em>:
         * if the list is structurally modified at any time after the iterator is
         * created, in any way except through the iterator's own
         * <b style="color: orange;"> remove</b> or
         * <b style="color: orange;"> add</b> methods, the iterator will throw a
         * <b style="color: orange;"> ConcurrentException</b>.  Thus, in the face of
         * concurrent modification, the iterator fails quickly and cleanly, rather
         * than risking arbitrary, non-deterministic behavior at an undetermined
         * time in the future.
         *
         * <p>Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw <b> ConcurrentException</b> on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness:  <i>the fail-fast behavior of iterators
         * should be used only to detect bugs.</i>
         *
         * @param E the type of elements in this list
         *
         * @author  Brunshweeck Tazeussong
         * @see     Collection
         * @see     List
         * @see     LinkedList
         * @see     Vector
         */
        template<class E>
        class ArrayList : public List<E> {
        private:

            /**
             * Default initial capacity.
             */
            static CORE_FAST gint DEFAULT_CAPACITY = 10;

            CORE_ALIAS(VRef, typename Class<E>::Ptr);
            CORE_ALIAS(ARRAY, typename Class<VRef>::Ptr);

            CORE_ALIAS(U, native::Unsafe);

            /**
             * Capture<T> represent all type T that extends this List element type E.
             * in other word E is base of T (Class<E>::isSuper<T>() is true).
             */
            template<class T>
            CORE_ALIAS(Capture, typename Class<T>::template Iff<Class<E>::template isSuper<T>()>);

            /**
             * Shared empty array instance used for empty instances.
             */
            ARRAY data = {};

            /**
             * The size of the ArrayList (the number of elements it contains).
             */
            gint len = 0;

            /**
             * The capacity of this ArrayList (number of place allocated)
             */
            gint capacity = {};

            using List<E>::modNum;

        public:

            /**
             * Constructs an empty list with the specified initial capacity.
             *
             * @param  initialCapacity  the initial capacity of the list
             * @throws ArgumentException if the specified initial capacity
             *         is negative
             */
            CORE_EXPLICIT ArrayList(gint initialCapacity) {
                if (initialCapacity < 0)
                    ArgumentException("Illegal capacity: " + String::valueOf(initialCapacity))
                            .throws(__trace("core.util.ArrayList"));
                data = (ARRAY) U::allocateMemory(L(capacity = initialCapacity));
            }

            /**
             * Constructs an empty list with an initial capacity of ten.
             */
            CORE_IMPLICIT ArrayList() {
                data = (ARRAY) U::allocateMemory(L(capacity = DEFAULT_CAPACITY));
            }

            /**
             * Constructs a list containing the elements of the specified
             * collection, in the order they are returned by the collection's
             * iterator.
             *
             * @param c the collection whose elements are to be placed into this list
             */
            template<class T = E>
            CORE_EXPLICIT ArrayList(const Collection<Capture<T>> &c) {
                if (Class<ArrayList<T>>::hasInstance(c)) {
                    // Optimized form
                    const ArrayList<T> &a = CORE_CAST(ArrayList<T> &, c);
                    if ((len = a.len) != 0) {
                        data = (ARRAY) U::allocateMemory(L(capacity = Math::max(len, DEFAULT_CAPACITY)));
                        for (gint i = 0; i < len && a.isSet(i); ++i) data[i] = a.data[i];
                    } else {
                        capacity = 0;
                        data = null;
                    }
                } else {
                    ReferenceArray a = c.toArray();
                    if ((len = a.length()) != 0) {
                        data = (ARRAY) U::allocateMemory(L(capacity = Math::max(len, DEFAULT_CAPACITY)));
                        for (gint i = 0; i < len; ++i) {
                            data[i] = &(E &) a[i];
                        }
                    } else {
                        capacity = 0;
                        data = null;
                    }
                }
            }

            /**
             * Constructs a list containing the elements of the specified
             * collection, in the order they are returned by the collection's
             * iterator.
             *
             * @param a the collection whose elements are to be placed into this list
             */
            ArrayList(const ArrayList<E> &a) {
                if ((len = a.len) != 0) {
                    data = (ARRAY) U::allocateMemory(L(capacity = Math::max(len, DEFAULT_CAPACITY)));
                    for (gint i = 0; i < len && a.isSet(i); ++i) data[i] = a.data[i];
                } else {
                    capacity = 0;
                    data = null;
                }
            }

            /**
             * Constructs a list containing the elements of the specified
             * collection, in the order they are returned by the collection's
             * iterator.
             *
             * @param a the collection whose elements are to be placed into this list
             */
            ArrayList(ArrayList<E> &&a) CORE_NOTHROW {
                U::swapValues(data, a.data);
                U::swapValues(len, a.len);
                U::swapValues(capacity, a.capacity);
            }

            /**
             * Constructs a list containing the elements of the specified
             * collection, in the order they are returned by the collection's
             * iterator.
             *
             * @param a the collection whose elements are to be placed into this list
             */
            ArrayList &operator=(const ArrayList<E> &a) {
                if (this != &a) {
                    if (capacity < a.len)
                        resize(len + (a.len - capacity));
                    arraycopy(a.data, 0, data, 0, a.len);
                    for (gint i = a.len; i < len; ++i) data[i] = null;
                    len = a.len;
                }
                return *this;
            }

            /**
             * Constructs a list containing the elements of the specified
             * collection, in the order they are returned by the collection's
             * iterator.
             *
             * @param a the collection whose elements are to be placed into this list
             */
            ArrayList &operator=(ArrayList<E> &&a) CORE_NOTHROW {
                if (this != &a) {
                    U::swapValues(data, a.data);
                    U::swapValues(len, a.len);
                    U::swapValues(capacity, a.capacity);
                }
                return *this;
            }

        private:

            /**
             * Increases the capacity to ensure that it can hold at least the
             * number of elements specified by the minimum capacity argument.
             *
             * @param minCapacity the desired minimum capacity
             * @throws MemoryError if minCapacity is less than zero
             */
            void resize(gint minCapacity) {
                gint oldCapacity = capacity;
                gint const newCapacity = oldCapacity > 0 ?
                                         ArraysSupport::newLength(oldCapacity, minCapacity - oldCapacity,
                                                                  oldCapacity >> 1) :
                                         Math::max(DEFAULT_CAPACITY, minCapacity);
                {
                    /* array copy */
                    ARRAY newData = (ARRAY) U::allocateMemory(L(newCapacity));
                    arraycopy(data, 0, newData, 0, len);
                    U::freeMemory((glong) data);
                    data = newData;
                }
                capacity = newCapacity;
            }

            /**
             * convert the given capacity to equivalent in bytes
             */
            static CORE_FAST glong L(gint capacity) { return 1LL * capacity * U::ARRAY_REFERENCE_INDEX_SCALE; }

            template<class T = E>
            static T &elementAt(typename ArrayList<Capture<T>>::ARRAY es, gint i) { return (T &) *es[i]; }

        public:

            /**
             * Returns the number of elements in this list.
             */
            gint size() const override { return len; }

            /**
             * Returns <b>true</b> if this list contains the specified element.
             * More formally, returns <b>true</b> if and only if this list contains
             * at least one element <b>e</b> such that
             * <b>Objects.equals(o, e)</b>.
             *
             * @param o element whose presence in this list is to be tested
             * @return <b>true</b> if this list contains the specified element
             */
            gbool contains(const E &o) const override { return indexOf(o) >= 0; }

            /**
             * Returns the index of the first occurrence of the specified element
             * in this list, or -1 if this list does not contain the element.
             * More formally, returns the lowest index <b>i</b> such that
             * <b>Objects.equals(o, get(i))</b>,
             * or -1 if there is no such index.
             */
            gint indexOf(const E &o) const override { return findIndex(o, 0, len, true); }

            /**
             * Returns the index of the last occurrence of the specified element
             * in this list, or -1 if this list does not contain the element.
             * More formally, returns the highest index <b>i</b> such that
             * <b>Objects.equals(o, get(i))</b>,
             * or -1 if there is no such index.
             */
            gint lastIndexOf(const E &o) const override { return findIndex(o, 0, len, false); }

        private:
            gint findIndex(const E &o, gint start, gint end, gbool first) const {
                ARRAY es = data;
                if (first) {
                    for (gint i = start; i < end; i++)
                        if (o.equals(elementAt(es, i)))
                            return i;
                } else {
                    for (gint i = end - 1; i >= start; i--)
                        if (o.equals(elementAt(es, i)))
                            return i;
                }
                return -1;
            }

        public:

            /**
             * Returns a shallow copy of this <b>ArrayList</b> instance.  (The
             * elements themselves are not copied.)
             *
             * @return a clone of this <b>ArrayList</b> instance
             */
            Object &clone() const override {
                ArrayList<E> &a = U::createInstance<ArrayList<E>>(0);
                ARRAY dataCopy = (ARRAY) U::allocateMemory(1LL * capacity * U::ARRAY_REFERENCE_INDEX_SCALE);
                U::copyMemory((glong) data, (glong) dataCopy, 1LL * len * U::ARRAY_REFERENCE_INDEX_SCALE);
                a.len = len;
                a.capacity = capacity;
                a.data = dataCopy;
                return a;
            }

            /**
             * Returns an array containing all of the elements in this list
             * in proper sequence (from first to last element).
             *
             * <p>The returned array will be "safe" in that no references to it are
             * maintained by this list.  (In other words, this method must allocate
             * a new array).  The caller is thus free to modify the returned array.
             *
             * <p>This method acts as bridge between array-based and collection-based
             * APIs.
             *
             * @return an array containing all of the elements in this list in
             *         proper sequence
             */
            ReferenceArray toArray() const override {
                ReferenceArray a = ReferenceArray(len);
                for (gint i = 0; i < len; ++i)
                    a.set(i, elementAt(data, i));
                return a;
            }

            /**
             * Returns the element at the specified position in this list.
             *
             * @param  index index of the element to return
             * @return the element at the specified position in this list
             * @throws IndexException
             */
            E &get(gint index) override {
                try {
                    Preconditions::checkIndex(index, len);
                    return *data[index];
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList")); }
            }

            /**
             * Returns the element at the specified position in this list.
             *
             * @param  index index of the element to return
             * @return the element at the specified position in this list
             * @throws IndexException
             */
            const E &get(gint index) const override {
                try { Preconditions::checkIndex(index, len); }
                catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList")); }
                return *data[index];
            }

            /**
             * Replaces the element at the specified position in this list with
             * the specified element.
             *
             * @param index index of the element to replace
             * @param element element to be stored at the specified position
             * @return the element previously at the specified position
             * @throws IndexException
             */
            const E &set(gint index, const E &element) override {
                try {
                    Preconditions::checkIndex(index, len);
                    E &oldValue = *data[index];
                    data[index] = &U::copyInstance(element, true);
                    return oldValue;
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList")); }
            }

            /**
             * Appends the specified element to the end of this list.
             *
             * @param e element to be appended to this list
             * @return <b>true</b> (as specified by <b style="color:orange;">Collection.add</b>)
             */
            gbool add(const E &e) override {
                try {
                    modNum += 1;
                    add(e, len);
                    return true;
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList")); }
            }

            /**
             * Inserts the specified element at the specified position in this
             * list. Shifts the element currently at that position (if any) and
             * any subsequent elements to the right (adds one to their indices).
             *
             * @param index index at which the specified element is to be inserted
             * @param element element to be inserted
             * @throws IndexException
             */
            void add(gint index, const E &element) override {
                try {
                    Preconditions::checkIndexForAdding(index, len);
                    modNum += 1;
                    add(element, index);
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList")); }
            }

        private:

            /**
             * This helper method split out from add(E) to keep method
             * bytecode size under 35,
             * which helps when add(E) is called in a C1-compiled loop.
             */
            inline void add(const E &e, gint index) {
                if (index == capacity) resize(len + 1);
                data[index] = &U::copyInstance(e, true);
                len += 1;
            }

            template<class Src, class Dest>
            static void arraycopy(Src src, gint srcBegin, Dest dest, gint destBegin, gint length) {
                for (gint i = 0; i < length; ++i) dest[destBegin + i] = src[srcBegin + i];
            }

        public:

            /**
             * Removes the element at the specified position in this list.
             * Shifts any subsequent elements to the left (subtracts one from their
             * indices).
             *
             * @param index the index of the element to be removed
             * @return the element that was removed from the list
             * @throws IndexException 
             */
            const E &removeAt(gint index) override {
                try {
                    Preconditions::checkIndex(index, len);
                    modNum += 1;
                    ARRAY es = data;
                    const E &oldValue = elementAt(es, index);
                    gint newSize = {};
                    if ((newSize = len - 1) > index)
                        arraycopy(es, index + 1, es, index, newSize - index);
                    es[len = newSize] = null;
                    return oldValue;
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList")); }
            }

            /**
             * 
             */
            gbool equals(const Object &o) const override {
                if (this == &o)
                    return true;
                if (!Class<List<E>>::hasInstance(o))
                    return false;
                gint oldModNum = modNum;
                // ArrayList can be subclassed and given arbitrary behavior, but we can
                // still deal with the common case where o is ArrayList precisely
                gbool equal = Class<ArrayList>::hasInstance(o) ?
                              isPerfectlyEquals(CORE_CAST(ArrayList<E> &, o)) :
                              isEqualsForRange(CORE_CAST(List<E> &, o), 0, len);
                checkModNum(oldModNum);
                return equal;
            }

        private:

            template<class T = E>
            gbool isEqualsForRange(const List<Capture<T>> &other, gint from, gint to) const {
                ARRAY es = data;
                if (to > capacity)
                    ConcurrentException().throws(__trace("core.util.ArrayList"));
                Iterator<const E> &oit = other.iterator();
                for (; from < to; from++)
                    if (!oit.hasNext() || !Object::equals(elementAt(es, from), oit.next())) return false;
                return !oit.hasNext();
            }

            template<class T = E>
            gbool isPerfectlyEquals(const ArrayList<Capture<T>> &other) const {
                gint otherModNum = other.modNum;
                gint s = len;
                gbool equal = {};
                if ((equal = (s == other.len))) {
                    ARRAY otherEs = other.data;
                    ARRAY es = data;
                    if (s > capacity || s > other.capacity)
                        ConcurrentException().throws(__trace("core.util.ArrayList"));
                    for (gint i = 0; i < s; i++)
                        if (!(equal &= Object::equals(elementAt(es, i), elementAt(otherEs, i))))
                            break;
                }
                other.checkModNum(otherModNum);
                return equal;
            }

            void checkModNum(gint oldModNum) const {
                if (oldModNum != modNum) ConcurrentException().throws(__trace("core.util.ArrayList"));
            }

            template<class T>
            friend
            class ArrayList;

        public:

            /**
             * Removes the first occurrence of the specified element from this list,
             * if it is present.  If the list does not contain the element, it is
             * unchanged.  More formally, removes the element with the lowest index
             * <b>i</b> such that
             * <b>Objects.equals(o, get(i))</b>
             * (if such an element exists).  Returns <b>true</b> if this list
             * contained the specified element (or equivalently, if this list
             * changed as a result of the call).
             *
             * @param o element to be removed from this list, if present
             * @return <b>true</b> if this list contained the specified element
             */
            gbool remove(const E &o) override {
                gint i = {};
                if ((i = indexOf(o)) < 0) {
                    return false;
                } else {
                    removeAt(i);
                    return true;
                }
            }

            /**
             * Removes all of the elements from this list.  The list will
             * be empty after this call returns.
             */
            void clear() override {
                modNum += 1;
                ARRAY es = data;
                for (gint i = len - 1; i >= 0; ++i)
                    es[i] = null;
            }

            /**
             * Appends all of the elements in the specified collection to the end of
             * this list, in the order that they are returned by the
             * specified collection's Iterator.  The behavior of this operation is
             * undefined if the specified collection is modified while the operation
             * is in progress.  (This implies that the behavior of this call is
             * undefined if the specified collection is this list, and this
             * list is nonempty.)
             *
             * @param c collection containing elements to be added to this list
             * @return <b>true</b> if this list changed as a result of the call
             */
            gbool addAll(const Collection<E> &c) override {
                return addAll < E > (c);
            }

            /**
             * Appends all of the elements in the specified collection to the end of
             * this list, in the order that they are returned by the
             * specified collection's Iterator.  The behavior of this operation is
             * undefined if the specified collection is modified while the operation
             * is in progress.  (This implies that the behavior of this call is
             * undefined if the specified collection is this list, and this
             * list is nonempty.)
             *
             * @param c collection containing elements to be added to this list
             * @return <b>true</b> if this list changed as a result of the call
             */
            template<class T = E>
            gbool addAll(const Collection<Capture<T>> &c) {
                ReferenceArray a = c.toArray();
                gint const aSize = a.length();
                if (aSize == 0)
                    return false;
                modNum += 1;
                gint oldSize = len;
                if (aSize > capacity - oldSize)
                    resize(len + aSize);
                for (gint i = 0; i < aSize; ++i)
                    data[i + oldSize] = & (E &)a[i];
                len = oldSize + aSize;
                return true;
            }

            /**
             * Inserts all of the elements in the specified collection into this
             * list, starting at the specified position.  Shifts the element
             * currently at that position (if any) and any subsequent elements to
             * the right (increases their indices).  The new elements will appear
             * in the list in the order that they are returned by the
             * specified collection's iterator.
             *
             * @param index index at which to insert the first element from the
             *              specified collection
             * @param c collection containing elements to be added to this list
             * @return <b>true</b> if this list changed as a result of the call
             * @throws IndexException
             */
            gbool addAll(gint index, const Collection<E> &c) override {
                return addAll<E>(index, c);
            }

            /**
             * Inserts all of the elements in the specified collection into this
             * list, starting at the specified position.  Shifts the element
             * currently at that position (if any) and any subsequent elements to
             * the right (increases their indices).  The new elements will appear
             * in the list in the order that they are returned by the
             * specified collection's iterator.
             *
             * @param index index at which to insert the first element from the
             *              specified collection
             * @param c collection containing elements to be added to this list
             * @return <b>true</b> if this list changed as a result of the call
             * @throws IndexException
             */
            template<class T = E>
            gbool addAll(gint index, const Collection<Capture<T>> &c) {
                try {
                    Preconditions::checkIndexForAdding(index, len);
                    ReferenceArray a = c.toArray();
                    gint aSize = a.length();
                    if (aSize == 0)
                        return false;
                    modNum += 1;
                    gint oldSize = len;
                    if (aSize > capacity - oldSize)
                        resize(len + aSize);
                    arraycopy(data, index, data, index + aSize, aSize);
                    for (gint i = 0; i < aSize; ++i)
                        data[i + index] = &(T &) a[i];
                    len = oldSize + aSize;
                    return true;
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList")); }
            }

        private:

            /**
             * Removes from this list all of the elements whose index is between
             * <b>fromIndex</b>, inclusive, and <b>toIndex</b>, exclusive.
             * Shifts any succeeding elements to the left (reduces their index).
             * This call shortens the list by <b>(toIndex - fromIndex)</b> elements.
             * (If <b>toIndex==fromIndex</b>, this operation has no effect.)
             *
             * @throws IndexException if <b>fromIndex</b> or
             *         <b>toIndex</b> is out of range
             *         (<b>fromIndex < 0 ||
             *          toIndex > size() ||
             *          toIndex < fromIndex</b>)
             */
            void removingForRange(gint from, gint to) {
                try {
                    Preconditions::checkIndexFromRange(from, to, len);
                    modNum += 1;
                    shift(from, to);
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList")); }
            }

            void shift(gint from, gint to) {
                arraycopy(data, to, data, from, len - to);
                for (gint end = len, i = (len -= to - from); i < end; i++) data[i] = null;
            }

        public:

            /**
             * Removes from this list all of its elements that are contained in the
             * specified collection.
             *
             * @param c collection containing elements to be removed from this list
             * @return <b>true</b> if this list changed as a result of the call
             * @throws CastException if the class of an element of this list
             *         is incompatible with the specified collection (<a href="">optional</a>)
             * @see Collection::contains(Object)
             */
            gbool removeAll(const Collection<E> &c) override {
                return removeAll < E > (c);
            }

            /**
             * Removes from this list all of its elements that are contained in the
             * specified collection.
             *
             * @param c collection containing elements to be removed from this list
             * @return <b>true</b> if this list changed as a result of the call
             * @throws CastException if the class of an element of this list
             *         is incompatible with the specified collection (<a href="">optional</a>)
             * @see Collection::contains(Object)
             */
            template<class T = E>
            gbool removeAll(const Collection<Capture<T>> &c) {
                return removeAllForRange(c, false, 0, len);
            }

            /**
             * Retains only the elements in this list that are contained in the
             * specified collection.  In other words, removes from this list all
             * of its elements that are not contained in the specified collection.
             *
             * @param c collection containing elements to be retained in this list
             * @return <b>true</b> if this list changed as a result of the call
             * @throws CastException if the class of an element of this list
             *         is incompatible with the specified collection (<a href="">optional</a>)
             * @see Collection::contains(Object)
             */
            gbool retainAll(const Collection<E> &c) override {
                return retainAll < E > (c);
            }

            /**
             * Retains only the elements in this list that are contained in the
             * specified collection.  In other words, removes from this list all
             * of its elements that are not contained in the specified collection.
             *
             * @param c collection containing elements to be retained in this list
             * @return <b>true</b> if this list changed as a result of the call
             * @throws CastException if the class of an element of this list
             *         is incompatible with the specified collection (<a href="">optional</a>)
             * @see Collection::contains(Object)
             */
            template<class T = E>
            gbool retainAll(const Collection<Capture<T>> &c) {
                return removeAllForRange(c, true, 0, len);
            }

        private:
            template<class T = E>
            gbool removeAllForRange(const Collection<Capture<T>> &c, gbool save, gint from, gint end) {
                ARRAY es = data;
                gint r = {};
                // Optimize for initial run of survivors
                for (r = from;; r++) {
                    if (r == end) return false;
                    if (c.contains(elementAt(es, r)) != save) break;
                }
                gint w = r++;
                try {
                    for (VRef e; r < end; r++)
                        if (c.contains(*(e = es[r])) == save)
                            es[w++] = e;
                } catch (const Exception &ex) {
                    // Preserve behavioral compatibility with AbstractCollection,
                    // even if c.contains() throws.
                    arraycopy(es, r, es, w, end - r);
                    w += end - r;
                    modNum += end - w;
                    shift(w, end);
                    ex.throws(__trace("core.util.ArrayList"));
                }
                modNum += end - w;
                shift(w, end);
                return true;
            }

        public:

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence), starting at the specified position in the list.
             * The specified index indicates the first element that would be
             * returned by an initial call to <b style="color:orange;"> next</b>.
             * An initial call to <b style="color:orange;">ListIterator#previous previous</b> would
             * return the element with the specified index minus one.
             *
             * <p>The returned list iterator is <a href=""><i>fail-fast</i></a>.
             *
             * @throws IndexException 
             */
            ListIterator<E> &iterator(gint index) override {
                try {
                    Preconditions::checkIndexForAdding(index, len);
                    return U::createInstance < ListItr < E >> ((ArrayList &) *this, index);
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList")); }
            }

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence), starting at the specified position in the list.
             * The specified index indicates the first element that would be
             * returned by an initial call to <b style="color:orange;"> next</b>.
             * An initial call to <b style="color:orange;">ListIterator#previous previous</b> would
             * return the element with the specified index minus one.
             *
             * <p>The returned list iterator is <a href=""><i>fail-fast</i></a>.
             *
             * @throws IndexException 
             */
            ListIterator<const E> &iterator(gint index) const override {
                try {
                    Preconditions::checkIndexForAdding(index, len);
                    return U::createInstance<ListItr<const E>>((ArrayList &) *this, index);
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList")); }
            }

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence).
             *
             * <p>The returned list iterator is <a href=""><i>fail-fast</i></a>.
             *
             * @see ::iterator(gint)
             */
            ListIterator<E> &iterator() override {
                return U::createInstance < ListItr < E >> ((ArrayList &) *this, 0);
            }

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence).
             *
             * <p>The returned list iterator is <a href=""><i>fail-fast</i></a>.
             *
             * @see ::iterator(gint)
             */
            ListIterator<const E> &iterator() const override {
                return U::createInstance<ListItr<const E>>((ArrayList &) *this, 0);
            }

        private:

            /**
             * An optimized version of ListIterator
             */
            template<class T>
            class ListItr : public ListIterator<T> {
            private:
                /**
                 * Index of next element to return
                 */
                gint cursor = {};

                /**
                 * Index of last element returned
                 */
                gint last = -1;

                gint modNum = {};

                ArrayList &root;

            public:
                CORE_EXPLICIT ListItr(ArrayList &root, gint index) : modNum(root.modNum), cursor(index), root(root) {}

                gbool hasNext() const override {
                    return cursor != root.len;
                }

                T &next() override {
                    try {
                        root.checkModNum(modNum);
                        gint i = cursor;
                        if (i > root.len)
                            NoSuchItemException().throws(__trace("core.util.ArrayList.ListItr"));
                        ARRAY es = root.data;
                        if (i >= root.capacity)
                            ConcurrentException().throws(__trace("core.util.ArrayList.ListItr"));
                        cursor += 1;
                        return elementAt(es, last = i);
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.ListItr")); }
                }

                gbool hasPrevious() const override {
                    return cursor != 0;
                }

                T &previous() override {
                    try {
                        root.checkModNum(modNum);
                        gint i = cursor - 1;
                        if (i < 0) NoSuchItemException().throws(__trace("core.util.ArrayList.ListItr"));
                        ARRAY es = root.data;
                        if (i >= root.capacity) ConcurrentException().throws(__trace("core.util.ArrayList.ListItr"));
                        cursor = i;
                        return elementAt(es, last = i);
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.ListItr")); }
                }

                void remove() override {
                    if (last < 0)
                        StateException().throws(__trace("core.util.ArrayList.ListItr"));
                    try {
                        root.checkModNum(modNum);
                        root.removeAt(last);
                        cursor = last;
                        last = -1;
                        modNum = root.modNum;
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.ListItr")); }
                    catch (const IndexException &ie) { ConcurrentException().throws(__trace("core.util.ArrayList.ListItr")); }
                }

                void set(const T &e) override {
                    if (last < 0)
                        StateException().throws(__trace("core.util.ArrayList.ListItr"));
                    try {
                        root.checkModNum(modNum);
                        root.set(last, e);
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.ListItr")); }
                    catch (const IndexException &ie) { ConcurrentException().throws(__trace("core.util.ArrayList.ListItr")); }
                }

                void add(const T &e) override {
                    try {
                        root.checkModNum(modNum);
                        gint i = cursor;
                        root.add(i, e);
                        cursor = i + 1;
                        last = -1;
                        modNum = root.modNum;
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.ListItr")); }
                    catch (const IndexException &ie) { ConcurrentException().throws(__trace("core.util.ArrayList.ListItr")); }

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

        public:

            /**
             * Returns a view of the portion of this list between the specified
             * <b>fromIndex</b>, inclusive, and <b>toIndex</b>, exclusive.  (If
             * <b>fromIndex</b> and <b>toIndex</b> are equal, the returned list is
             * empty.)  The returned list is backed by this list, so non-structural
             * changes in the returned list are reflected in this list, and vice-versa.
             * The returned list supports all of the optional list operations.
             *
             * <p>This method eliminates the need for explicit range operations (of
             * the sort that commonly exist for arrays).  Any operation that expects
             * a list can be used as a range operation by passing a subList view
             * instead of a whole list.  For example, the following idiom
             * removes a range of elements from a list:
             * <pre>
             *      list.subList(from, to).clear();
             * </pre>
             * Similar idioms may be constructed for <b style="color:orange;">#indexOf(Object)</b> and
             * <b style="color:orange;">lastIndexOf(Object)</b>, and all of the algorithms in the
             * <b style="color:orange;">Collections</b> class can be applied to a subList.
             *
             * <p>The semantics of the list returned by this method become undefined if
             * the backing list (i.e., this list) is <i>structurally modified</i> in
             * any way other than via the returned list.  (Structural modifications are
             * those that change the size of this list, or otherwise perturb it in such
             * a fashion that iterations in progress may yield incorrect results.)
             *
             * @throws IndexException 
             * @throws ArgumentException 
             */
            List<E> &subList(gint from, gint to) const override {
                try {
                    Preconditions::checkIndexFromRange(from, to, len);
                    return U::createInstance < SubList < E >> ((ArrayList &) *this, from, to);
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList")); }
            }

        private:
            template<class T = E>
            class SubList : public List<T> {
            private:
                ArrayList<T> &root;
                SubList<T> &parent;
                gint offset;
                gint len;
                using List<T>::modNum;

            public:

                /**
                 * Constructs a sublist of an arbitrary ArrayList.
                 */
                CORE_EXPLICIT SubList(ArrayList<T> &root, gint from, gint to) :
                        root(root), parent(*this), len(to - from), offset(from) { modNum = root.modNum; }

                /**
                 * Constructs a sublist of another SubList.
                 */
                CORE_EXPLICIT SubList(SubList<T> &parent, gint from, gint to) :
                        root(parent.root), parent(parent), offset(parent.offset + from),
                        len(to - from) { modNum = parent.modNum; }

                const T &set(gint index, const T &element) override {
                    try {
                        Preconditions::checkIndex(index, len);
                        root.checkModNum(modNum);
                        T &oldValue = elementAt(root.data, index + offset);
                        root.data[index + offset] = &U::copyInstance(element, true);
                        return oldValue;
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                    catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList.SubList")); }
                }

                T &get(gint index) override {
                    try {
                        Preconditions::checkIndex(index, len);
                        root.checkModNum(modNum);
                        return *root.data[index + offset];
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                    catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList.SubList")); }
                }

                const T &get(gint index) const override {
                    try {
                        Preconditions::checkIndex(index, len);
                        root.checkModNum(modNum);
                        return *root.data[index + offset];
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                    catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList.SubList")); }
                }

                gint size() const override {
                    try {
                        root.checkModNum(modNum);
                        return len;
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                }

                gbool add(const T &e) override {
                    add(len, e);
                    return true;
                }

                void add(gint index, const T &element) override {
                    try {
                        Preconditions::checkIndex(index, len);
                        root.checkModNum(modNum);
                        root.add(index + offset, element);
                        update(1);
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                    catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList.SubList")); }
                }

                const T &removeAt(gint index) override {
                    try {
                        Preconditions::checkIndex(index, len);
                        root.checkModNum(modNum);
                        const T &retVal = root.removeAt(index + offset);
                        update(-1);
                        return retVal;
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                    catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList.SubList")); }
                }

                gbool addAll(const Collection<T> &c) override {
                    return addAll(len, c);
                }

                gbool addAll(gint index, const Collection<T> &c) override {
                    try {
                        Preconditions::checkIndexForAdding(index, len);
                        gint cSize = c.size();
                        if (cSize == 0) return false;
                        root.checkModNum(modNum);
                        root.addAll(index, c);
                        update(cSize);
                        return true;
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                    catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList.SubList")); }
                }

                void replaceAll(const UnaryOperator<T> &op) override {
                    root.replaceAllForRange(op, offset, len + offset);
                }

                gbool removeAll(const Collection<T> &c) override {
                    try {
                        root.checkModNum(modNum);
                        gint oldSize = root.len;
                        gbool retVal = root.removeAllForRange(c, false, offset, len + offset);
                        if (retVal) update(root.len - oldSize);
                        return retVal;
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                }

                gbool retainAll(const Collection<T> &c) override {
                    try {
                        root.checkModNum(modNum);
                        gint oldSize = root.len;
                        gbool retVal = root.removeAllForRange(c, true, offset, len + offset);
                        if (retVal) update(root.len - oldSize);
                        return retVal;
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                }

                gbool removeIf(const Predicate<T> &filter) override {
                    try {
                        root.checkModNum(modNum);
                        gint oldSize = root.len;
                        gbool retVal = root.removeIfForRange(filter, offset, len + offset);
                        if (retVal) update(root.len - oldSize);
                        return retVal;
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                }

                ReferenceArray toArray() const override {
                    try {
                        root.checkModNum(modNum);
                        ReferenceArray a = ReferenceArray(len);
                        for (gint i = 0; i < len; ++i)
                            a.set(i, *root.data[i + offset]);
                        return a;
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<List<T>>::hasInstance(o))
                        return false;
                    try {
                        gbool equal = root.isEqualsForRange(CORE_CAST(List<T> &, o), offset, offset + len);
                        root.checkModNum(modNum);
                        return equal;
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                }

                gint indexOf(const T &o) const override {
                    try {
                        root.checkModNum(modNum);
                        return root.findIndex(o, offset, len + offset, true);
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                }

                gint lastIndexOf(const T &o) const override {
                    try {
                        root.checkModNum(modNum);
                        return root.findIndex(o, offset, len + offset, false);
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                }

                gbool contains(const T &o) const override {
                    return indexOf(o) >= 0;
                }

                ListIterator<const T> &iterator() const override {
                    return iterator(offset);
                }

                ListIterator<T> &iterator() override {
                    return iterator(offset);
                }

                ListIterator<T> &iterator(gint index) override {
                    class _$ : public ArrayList<E>::ListItr<T> {
                        SubList<E> &parent;

                    public:
                        _$(SubList<E> &parent, gint index) :
                                ArrayList<E>::ListItr<T>(parent.root, index + parent.offset), parent(parent) {}

                        void remove() override {
                            ArrayList<E>::ListItr<T>::remove();
                            parent.update(-1);
                        }

                        void add(const T &e) override {
                            ArrayList<E>::ListItr<T>::add(e);
                            parent.update(+1);
                        }
                    };
                    try {
                        Preconditions::checkIndexForAdding(index, len);
                        root.checkModNum(modNum);
                        return U::createInstance<_$>((SubList &) *this, index);
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                    catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList.SubList")); }
                }

                ListIterator<const T> &iterator(gint index) const override {
                    class _$ : public ArrayList<E>::ListItr<const T> {
                        SubList<E> &parent;

                    public:
                        _$(SubList<E> &parent, gint index) :
                                ArrayList<E>::ListItr<const T>(parent.root, index + parent.offset), parent(parent) {}

                        void remove() override {
                            ArrayList<E>::ListItr<const T>::remove();
                            parent.update(-1);
                        }

                        void add(const T &e) override {
                            ArrayList<E>::ListItr<const T>::add(e);
                            parent.update(+1);
                        }
                    };
                    try {
                        Preconditions::checkIndexForAdding(index, len);
                        root.checkModNum(modNum);
                        return U::createInstance<_$>((SubList &) *this, index);
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                    catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList.SubList")); }
                }

                List<T> &subList(gint from, gint to) const override {
                    try {
                        Preconditions::checkIndexFromRange(from, to, len);
                        root.checkModNum(modNum);
                        return U::createInstance<SubList<T>>((SubList<T> &) *this, from, to);
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                    catch (const IndexException &ie) { ie.throws(__trace("core.util.ArrayList.SubList")); }
                }

                gbool remove(const T &o) override {
                    try {
                        root.checkModNum(modNum);
                        gint i = indexOf(o);
                        if (i < 0) return false;
                        removeAt(i);
                        return true;
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                }

                gbool containsAll(const Collection<T> &c) const override {
                    auto &itr = iterator();
                    while (itr.hasNext()) if (!c.contains(itr.next())) return false;
                    return true;
                }

                void clear() override {
                    try {
                        removingForRange(0, len);
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                }

                void forEach(const Consumer<T> &action) const override {
                    gint oldModNum = modNum;
                    ARRAY es = root.data;
                    gint size = len;
                    for (gint i = 0; i < size && oldModNum == modNum; ++i) action.accept(elementAt(es, i + offset));
                    if (oldModNum != modNum) ConcurrentException().throws(__trace("core.util.ArrayList.SubList"));
                }

                void forEach(const Consumer<T &> &action) override {
                    gint oldModNum = modNum;
                    ARRAY es = root.data;
                    gint size = len;
                    for (gint i = 0; i < size && oldModNum == modNum; ++i) action.accept(elementAt(es, i + offset));
                    if (oldModNum != modNum)
                        ConcurrentException().throws(__trace("core.util.ArrayList.SubList"));
                }

                Object &clone() const override {
                    return U::createInstance<SubList<T>>(CORE_CAST(SubList<T> &, *this), 0, len);
                }

            private:
                void removingForRange(gint from, gint to) {
                    try {
                        root.checkModNum(modNum);
                        root.removingForRange(from + offset, to + offset);
                        update(from - to);
                    } catch (const ConcurrentException &cme) { cme.throws(__trace("core.util.ArrayList.SubList")); }
                }

                void update(gint s) {
                    len += s;
                    modNum = root.modNum;
                    if (&parent != this)
                        parent.update(s);
                }
            };

            /**
             * Removes all elements satisfying the given predicate, from index
             * i (inclusive) to index end (exclusive).
             */
            gbool removeIfForRange(const Predicate<E> &filter, gint from, gint to) {
                gint oldModNum = modNum;
                ARRAY es = data;
                gint i = from;
                gint end = to;
                // Optimize for initial run of survivors
                for (; i < end && !filter.test(elementAt(es, i)); i++);
                // Tolerate predicates that reentrantly access the collection for
                // read (but writers still get CME), so traverse once to find
                // elements to delete, a second pass to physically expunge.
                if (i < end) {
                    gint const beg = i;
                    glong *deathRow = (glong *) nBits(end - beg);
                    deathRow[0] = 1L;   // set bit 0
                    for (i = beg + 1; i < end; i++)
                        if (filter.test(elementAt(es, i))) setBit(deathRow, i - beg);
                    if (modNum != oldModNum) {
                        U::freeMemory((glong) deathRow);
                        ConcurrentException().throws(__trace("core.util.ArrayList"));
                    }
                    modNum++;
                    gint w = beg;
                    for (i = beg; i < end; i++) if (isClear(deathRow, i - beg)) es[w++] = es[i];
                    shift(w, end);
                    U::freeMemory((glong) deathRow);
                    return true;
                } else {
                    if (modNum != oldModNum) ConcurrentException().throws(__trace("core.util.ArrayList"));
                    return false;
                }
            }


            // A tiny bit set implementation

            static glong nBits(gint n) {
                return U::allocateMemory(1LL * (((n - 1) >> 6) + 1) * U::ARRAY_LONG_INDEX_SCALE);
            }

            static void setBit(glong bits[], gint i) { bits[i >> 6] |= 1L << i; }

            static gbool isClear(const glong bits[], gint i) { return (bits[i >> 6] & (1L << i)) == 0; }

            void replaceAllForRange(const UnaryOperator<E> &op, gint from, gint to) {
                gint oldModNum = modNum;
                ARRAY es = data;
                gint i = from;
                gint const end = to;
                for (; modNum == oldModNum && i < end; i++)
                    es[i] = &U::copyInstance<E>(op.apply(elementAt(es, i)), true);
                if (modNum != oldModNum)
                    ConcurrentException().throws(__trace("core.util.ArrayList"));
            }

        public:
            gbool removeIf(const Predicate<E> &filter) override {
                return removeIfForRange(filter, 0, len);
            }

            gbool containsAll(const Collection<E> &c) const override {
                if(c.size() == 0)
                    return false;
                if(this == &c)
                    return true;
                Iterator<const E> &itr = c.iterator();
                while (itr.hasNext()) {
                    if (!contains(itr.next())) {
                        return false;
                    }
                }
                return true;
            }

            void replaceAll(const UnaryOperator<E> &op) override { return replaceAllForRange(op, 0, len); }

            void forEach(const Consumer<E> &action) const override {
                gint oldModNum = modNum;
                ARRAY es = data;
                gint size = len;
                for (gint i = 0; modNum == oldModNum && i < size; i++)
                    action.accept(elementAt(es, i));
                if (modNum != oldModNum)
                    ConcurrentException().throws(__trace("core.util.ArrayList"));
            }

            void forEach(const Consumer<E &> &action) override {
                gint oldModNum = modNum;
                ARRAY es = data;
                gint size = len;
                for (gint i = 0; modNum == oldModNum && i < size; i++) action.accept(elementAt(es, i));
                if (modNum != oldModNum) ConcurrentException().throws(__trace("core.util.ArrayList"));
            }

            ~ArrayList() override {
                len = capacity = 0;
                U::freeMemory((glong) data);
                data = null;
            }
        };

#if CORE_TEMPLATE_TYPE_DEDUCTION
        ArrayList() -> ArrayList<Object>;
        ArrayList(gint) -> ArrayList<Object>;
        template<class T> ArrayList(const Collection<T> &) -> ArrayList<T>;
#endif

    }
} // core

#endif //CORE23_ARRAYLIST_H
