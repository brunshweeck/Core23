//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_LINKEDLIST_H
#define CORE23_LINKEDLIST_H

#include <core/util/List.h>

namespace core {
    namespace util {

        /**
         * Doubly-linked list implementation of the <b>List</b>
         * interfaces.  Implements all optional list operations, and permits all
         * elements
         *
         * <p>All of the operations perform as could be expected for a doubly-linked
         * list.  Operations that index into the list will traverse the list from
         * the beginning or the end, whichever is closer to the specified index.
         *
         * <p><strong>Note that this implementation is not synchronized.</strong>
         * If multiple threads access a linked list concurrently, and at least
         * one of the threads modifies the list structurally, it <i>must</i> be
         * synchronized externally.  (A structural modification is any operation
         * that adds or deletes one or more elements; merely setting the value of
         * an element is not a structural modification.)  This is typically
         * accomplished by synchronizing on some object that naturally
         * encapsulates the list.
         *
         * If no such object exists, the list should be "wrapped" using the
         * <b style="color:orange;">core.util.SynchronizedList</b>
         * method.
         *
         * <p>The iterators returned by this class's <b>iterator</b> and
         * <b>listIterator</b> methods are <i>fail-fast</i>: if the list is
         * structurally modified at any time after the iterator is created, in
         * any way except through the Iterator is own <b>remove</b> or
         * <b>add</b> methods, the iterator will throw a <b style="color:orange;">
         * ConcurrentException</b>.  Thus, in the face of concurrent
         * modification, the iterator fails quickly and cleanly, rather than
         * risking arbitrary, non-deterministic behavior at an undetermined
         * time in the future.
         *
         * <p>Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw <b>ConcurrentException</b> on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness:   <i>the fail-fast behavior of iterators
         * should be used only to detect bugs.</i>
         *
         * @author  Brunshweeck Tazeussong
         * @see     List
         * @see     ArrayList
         *
         * @tparam E the type of elements held in this collection
         */
        template<class E>
        class LinkedList : public List<E> {
        private:

            /**
             * The bidirectional unary tree structure used to store
             * list elements.
             */
            class Node;

            CORE_ALIAS(LnkNode, typename Class<Node>::Ptr);

            /**
             * The number of node created in this list
             */
            gint len = {};

            /**
             * The link to fist node
             */
            LnkNode first = {};

            /**
             * The link to last node
             */
            LnkNode last = {};

            using List<E>::modNum;

            CORE_ALIAS(U, native::Unsafe);

            /**
             * Capture<T> represent all type T that extends this List element type E.
             * in other word E is base of T (Class<E>::isSuper<T>() is true).
             */
            template<class T>
            CORE_ALIAS(Capture, typename Class<T>::template Iff<Class<E>::template isSuper<T>()>);

        public:

            /**
             * Constructs an empty list.
             */
            CORE_FAST LinkedList() = default;

            /**
             * Constructs a list containing the elements of the specified
             * collection, in the order they are returned by the collection's
             * iterator.
             *
             * @param  c the collection whose elements are to be placed into this list
             */
            template<class T = E>
            CORE_EXPLICIT LinkedList(const Collection<Capture<T>> &c) { addAll<T>(c); }

            /**
             * Constructs a list containing the elements of the specified
             * collection, in the order they are returned by the collection's
             * iterator.
             *
             * @param  ll the collection whose elements are to be placed into this list
             */
            LinkedList(const LinkedList &ll) { addAll(ll); }

            /**
             * Constructs a list containing the elements of the specified
             * collection, in the order they are returned by the collection's
             * iterator.
             *
             * @param  ll the collection whose elements are to be placed into this list
             */
            LinkedList(LinkedList &&ll) CORE_NOTHROW {
                U::swapValues(len, ll.len);
                U::swapValues(first, ll.first);
                U::swapValues(last, ll.last);
            }

            /**
             * Set all element of this list witch all of the elements of the specified
             * collection, in the order they are returned by the collection's
             * iterator.
             *
             * @param  ll the collection whose elements are to be placed into this list
             */
            LinkedList &operator=(const LinkedList &ll) {
                if (this != &ll) {
                    LnkNode x1 = first;
                    LnkNode x2 = ll.first;
                    LnkNode x = null;
                    gint i = 0;
                    // self copy of elements (while i < Math::min(len, ll.len))
                    for (gint limit = Math::min(len, ll.len); i < limit; i += 1) {
                        x1->item = x2->item;
                        x = x1;
                        x1 = x1->next;
                        x2 = x2->next;
                    }
                    if (i < ll.len) {
                        // adding supplements of elements
                        while (i < ll.len) {
                            linkLast(*x2->item);
                            x2 = x2->next;
                            i += 1;
                        }
                    } else if (i < len) {
                        // for preserve same size, deleting of other trailing elements
                        last = x;
                        if (last == null) first = null;
                        while (i < len) {
                            x = x1->next;
                            x1->item = null;
                            x1->next = x1->prev = null;
                            x1 = x;
                            i += 1;
                        }
                    }
                    len = ll.len;
                }
                return *this;
            }

            /**
             * Swap all element of this list witch all of the elements of the specified
             * collection, in the order they are returned by the collection's
             * iterator.
             *
             * @param  ll the collection whose elements are to be placed into this list
             */
            LinkedList &operator=(LinkedList &&ll) CORE_NOTHROW {
                if (this == &ll) {
                    U::swapValues(len, ll.len);
                    U::swapValues(first, ll.first);
                    U::swapValues(last, ll.last);
                }
                return *this;
            }

        private:

            /**
             * Links e as first element.
             */
            void linkFirst(E &e) {
                LnkNode f = first;
                LnkNode newNode = &U::createInstance<Node>(null, e, f);
                first = newNode;
                if (f == null) last = newNode; else f->prev = newNode;
                len += 1;
                modNum += 1;
            }

            /**
             * Links e as last element.
             */
            void linkLast(E &e) {
                LnkNode l = last;
                LnkNode newNode = &U::createInstance<Node>(l, e, null);
                last = newNode;
                if (l == null) first = newNode; else l->next = newNode;
                len += 1;
                modNum += 1;
            }

            /**
             * Inserts element e before non-null Node succ.
             */
            void linkBefore(E &e, LnkNode succ) {
                // CORE_ASSERT(succ != null, "core.util.LinkedList");
                LnkNode pred = succ->prev;
                LnkNode newNode = &U::createInstance<Node>(pred, e, succ);
                succ->prev = newNode;
                if (pred == null) first = newNode; else pred->next = newNode;
                len += 1;
                modNum += 1;
            }

            /**
             * Unlinks non-null first node f.
             */
            E &unlinkFirst(LnkNode f) {
                // CORE_ASSERT(f == first && f != null, "core.util.LinkedList");
                E &element = *f->item;
                LnkNode next = f->next;
                f->item = null;
                f->next = null; // help GC
                first = next;
                if (next == null) last = null; else next->prev = null;
                len -= 1;
                modNum += 1;
                return element;
            }

            /**
             * Unlinks non-null last node l.
             */
            E unlinkLast(LnkNode l) {
                // CORE_ASSERT(l == last && l != null, "core.util.LinkedList");
                E &element = *l->item;
                LnkNode prev = l->prev;
                l->item = null;
                l->prev = null; // help GC
                last = prev;
                if (prev == null) first = null; else prev->next = null;
                len -= 1;
                modNum += 1;
                return element;
            }

            /**
             * Unlinks non-null node x.
             */
            E &unlink(LnkNode x) {
                // CORE_ASSERT(x != null, "core.util.LinkedList");
                E &element = *x->item;
                LnkNode next = x->next;
                LnkNode prev = x->prev;
                if (prev == null) first = next;
                else {
                    prev->next = next;
                    x->prev = null;
                }

                if (next == null) last = prev;
                else {
                    next->prev = prev;
                    x->next = null;
                }

                x->item = null;
                len -= 1;
                modNum += 1;
                return element;
            }

        public:

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
             * Returns the number of elements in this list.
             *
             * @return the number of elements in this list
             */
            gint size() const override { return len; }

            /**
             * Appends the specified element to the end of this list.
             *
             * <p>This method is equivalent to <b style="color:orange;">addLast</b>.
             *
             * @param e element to be appended to this list
             * @return <b>true</b> (as specified by <b style="color:orange;">Collection::add</b>)
             */
            gbool add(const E &e) override {
                linkLast(U::copyInstance(e, true));
                return true;
            }

            /**
             * Removes the first occurrence of the specified element from this list,
             * if it is present.  If this list does not contain the element, it is
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
                for (LnkNode x = first; x != null; x = x->next) {
                    if (o.equals(*x->item)) {
                        unlink(x);
                        return true;
                    }
                }
                return false;
            }

            /**
             * Appends all of the elements in the specified collection to the end of
             * this list, in the order that they are returned by the specified
             * collection's iterator.  The behavior of this operation is undefined if
             * the specified collection is modified while the operation is in
             * progress.  (Note that this will occur if the specified collection is
             * this list, and it's nonempty.)
             *
             * @param c collection containing elements to be added to this list
             * @return <b>true</b> if this list changed as a result of the call
             */
            gbool addAll(const Collection<E> &c) override { return addAll(len, c); }

            /**
             * Appends all of the elements in the specified collection to the end of
             * this list, in the order that they are returned by the specified
             * collection's iterator.  The behavior of this operation is undefined if
             * the specified collection is modified while the operation is in
             * progress.  (Note that this will occur if the specified collection is
             * this list, and it's nonempty.)
             *
             * @param c collection containing elements to be added to this list
             * @return <b>true</b> if this list changed as a result of the call
             */
            template<class T = E>
            gbool addAll(const Collection<Capture<T>> &c) { return addAll<T>(len, c); }

            /**
             * Inserts all of the elements in the specified collection into this
             * list, starting at the specified position.  Shifts the element
             * currently at that position (if any) and any subsequent elements to
             * the right (increases their indices).  The new elements will appear
             * in the list in the order that they are returned by the
             * specified collection's iterator.
             *
             * @param index index at which to insert the first element
             *              from the specified collection
             * @param c collection containing elements to be added to this list
             * @return <b>true</b> if this list changed as a result of the call
             * @throws IndexException
             */
            gbool addAll(gint index, const Collection<E> &c) override { return addAll<E>(index, c); }

            /**
             * Inserts all of the elements in the specified collection into this
             * list, starting at the specified position.  Shifts the element
             * currently at that position (if any) and any subsequent elements to
             * the right (increases their indices).  The new elements will appear
             * in the list in the order that they are returned by the
             * specified collection's iterator.
             *
             * @param index index at which to insert the first element
             *              from the specified collection
             * @param c collection containing elements to be added to this list
             * @return <b>true</b> if this list changed as a result of the call
             * @throws IndexException
             */
            template<class T = E>
            gbool addAll(gint index, const Collection<Capture<T>> &c) {
                try {
                    Preconditions::checkIndexForAdding(index, len);
                    ReferenceArray a = c.toArray();
                    gint const aSize = a.length();
                    if (aSize == 0) return false;
                    LnkNode succ = {};
                    LnkNode pred = {};
                    if (index == len) {
                        succ = null;
                        pred = last;
                    } else {
                        succ = nodeAt(index);
                        pred = succ->prev;
                    }
                    for (Object &t: a) {
                        E &e = (E &) t;
                        LnkNode newNode = &U::createInstance<Node>(pred, e, null);
                        if (pred == null) first = newNode; else pred->next = newNode;
                        pred = newNode;
                    }

                    if (succ == null)
                        last = pred;
                    else {
                        pred->next = succ;
                        succ->prev = pred;
                    }
                    len += aSize;
                    modNum += 1;
                    return true;
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.LinkedList")); }
            }

            /**
             * Removes all of the elements from this list.
             * The list will be empty after this call returns.
             */
            void clear() override {
                // Clearing all of the links between nodes is "unnecessary", but:
                // - helps a generational GC if the discarded nodes inhabit
                //   more than one generation
                // - is sure to free memory even if there is a reachable Iterator
                for (LnkNode x = first; x != null;) {
                    LnkNode next = x->next;
                    x->item = null;
                    x->next = null;
                    x->prev = null;
                    x = next;
                }
                first = last = null;
                len = 0;
                modNum += 1;
            }

            /**
             * Returns the element at the specified position in this list.
             *
             * @param index index of the element to return
             * @return the element at the specified position in this list
             * @throws IndexException 
             */
            E &get(gint index) override {
                try {
                    Preconditions::checkIndex(index, len);
                    return *nodeAt(index)->item;
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.LinkedList")); }
            }

            /**
             * Returns the element at the specified position in this list.
             *
             * @param index index of the element to return
             * @return the element at the specified position in this list
             * @throws IndexException 
             */
            const E &get(gint index) const override {
                try {
                    Preconditions::checkIndex(index, len);
                    return *nodeAt(index)->item;
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.LinkedList")); }
            }

            /**
             * Replaces the element at the specified position in this list with the
             * specified element.
             *
             * @param index index of the element to replace
             * @param element element to be stored at the specified position
             * @return the element previously at the specified position
             * @throws IndexException 
             */
            const E &set(gint index, const E &element) override {
                try {
                    Preconditions::checkIndex(index, len);
                    LnkNode x = nodeAt(index);
                    E &oldValue = *x->item;
                    x->item = &U::copyInstance(element, true);
                    return oldValue;
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.LinkedList")); }
            }

            /**
             * Inserts the specified element at the specified position in this list.
             * Shifts the element currently at that position (if any) and any
             * subsequent elements to the right (adds one to their indices).
             *
             * @param index index at which the specified element is to be inserted
             * @param element element to be inserted
             * @throws IndexException 
             */
            void add(gint index, const E &element) override {
                try {
                    Preconditions::checkIndexForAdding(index, len);
                    if (index == len) linkLast(U::copyInstance(element, true));
                    else linkBefore(U::copyInstance(element, true), nodeAt(index));
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.LinkedList")); }
            }

            /**
             * Removes the element at the specified position in this list.  Shifts any
             * subsequent elements to the left (subtracts one from their indices).
             * Returns the element that was removed from the list.
             *
             * @param index the index of the element to be removed
             * @return the element previously at the specified position
             * @throws IndexException 
             */
            const E &removeAt(gint index) override {
                try {
                    Preconditions::checkIndex(index, len);
                    return unlink(nodeAt(index));
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.LinkedList")); }
            }

        private:

            /**
             * Returns the (non-null) Node at the specified element index.
             */
            inline LnkNode nodeAt(gint index) const {
                // CORE_ASSERT(index >= 0 && index <= len, "core.util.LinkedList");
                if (index < (len >> 1)) {
                    LnkNode x = first;
                    for (gint i = 0; i < index; i++) x = x->next;
                    return x;
                } else {
                    LnkNode x = last;
                    for (gint i = len - 1; i > index; i--) x = x->prev;
                    return x;
                }
            }

        public:

            /**
             * Returns the index of the first occurrence of the specified element
             * in this list, or -1 if this list does not contain the element.
             * More formally, returns the lowest index <b>i</b> such that
             * <b>Objects.equals(o, get(i))</b>,
             * or -1 if there is no such index.
             *
             * @param o element to search for
             * @return the index of the first occurrence of the specified element in
             *         this list, or -1 if this list does not contain the element
             */
            gint indexOf(const E &o) const override {
                gint index = 0;
                for (LnkNode x = first; x != null; x = x->next) {
                    if (o.equals(*x->item)) return index;
                    index++;
                }
                return -1;
            }

            /**
             * Returns the index of the last occurrence of the specified element
             * in this list, or -1 if this list does not contain the element.
             * More formally, returns the highest index <b>i</b> such that
             * <b>Objects.equals(o, get(i))</b>,
             * or -1 if there is no such index.
             *
             * @param o element to search for
             * @return the index of the last occurrence of the specified element in
             *         this list, or -1 if this list does not contain the element
             */
            gint lastIndexOf(const E &o) const override {
                gint index = len;
                for (LnkNode x = last; x != null; x = x->prev) {
                    index--;
                    if (o.equals(*x->item)) return index;
                }
                return -1;
            }

            /**
             * Returns a list-iterator of the elements in this list (in proper
             * sequence), starting at the specified position in the list.
             * Obeys the general contract of <b>List.listIterator(gint)</b>.<p>
             *
             * The list-iterator is <i>fail-fast</i>: if the list is structurally
             * modified at any time after the Iterator is created, in any way except
             * through the list-iterator's own <b>remove</b> or <b>add</b>
             * methods, the list-iterator will throw a
             * <b>ConcurrentException</b>.  Thus, in the face of
             * concurrent modification, the iterator fails quickly and cleanly, rather
             * than risking arbitrary, non-deterministic behavior at an undetermined
             * time in the future.
             *
             * @param index index of the first element to be returned from the
             *              list-iterator (by a call to <b>next</b>)
             * @return a ListIterator of the elements in this list (in proper
             *         sequence), starting at the specified position in the list
             * @throws IndexException 
             * @see List::iterator(gint)
             */
            ListIterator<E> &iterator(gint index) override {
                try {
                    Preconditions::checkIndexForAdding(index, len);
                    return U::createInstance < ListItr < E >> ((LinkedList &) *this, index);
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.LinkedList")); }
            }

            /**
             * Returns a list-iterator of the elements in this list (in proper
             * sequence), starting at the specified position in the list.
             * Obeys the general contract of <b>List.listIterator(gint)</b>.<p>
             *
             * The list-iterator is <i>fail-fast</i>: if the list is structurally
             * modified at any time after the Iterator is created, in any way except
             * through the list-iterator's own <b>remove</b> or <b>add</b>
             * methods, the list-iterator will throw a
             * <b>ConcurrentException</b>.  Thus, in the face of
             * concurrent modification, the iterator fails quickly and cleanly, rather
             * than risking arbitrary, non-deterministic behavior at an undetermined
             * time in the future.
             *
             * @param index index of the first element to be returned from the
             *              list-iterator (by a call to <b>next</b>)
             * @return a ListIterator of the elements in this list (in proper
             *         sequence), starting at the specified position in the list
             * @throws IndexException 
             * @see List::iterator(gint)
             */
            ListIterator<const E> &iterator(gint index) const override {
                try {
                    Preconditions::checkIndexForAdding(index, len);
                    return U::createInstance<ListItr<const E>>((LinkedList &) *this, index);
                } catch (const IndexException &ie) { ie.throws(__trace("core.util.LinkedList")); }
            }

        private:

            template<class T = E>
            class ListItr : public ListIterator<T> {
            private:
                /**
                 * The position of last returned element.
                 */
                LnkNode last = {};

                /**
                 * The position of current element
                 */
                LnkNode cursor = {};

                gint index = {};

                LinkedList &root;

                gint modNum = {};

            public:
                ListItr(LinkedList &root, gint index) :
                        cursor(index == root.len ? null : root.nodeAt(index)), root(root), index(index),
                        modNum(root.modNum) {}

                gbool hasNext() const override { return index < root.len; }

                T &next() override {
                    if (modNum != root.modNum)
                        ConcurrentException().throws(__trace("core.util.LinkedList.ListItr"));
                    if (!hasNext())
                        NoSuchItemException().throws(__trace("core.util.LinkedList.ListItr"));
                    last = cursor;
                    cursor = cursor->next;
                    index += 1;
                    return *last->item;
                }

                gbool hasPrevious() const override { return index > 0; }

                T &previous() override {
                    if (modNum != root.modNum)
                        ConcurrentException().throws(__trace("core.util.LinkedList.ListItr"));
                    if (!hasNext())
                        NoSuchItemException().throws(__trace("core.util.LinkedList.ListItr"));
                    last = cursor = (cursor == null) ? root.last : cursor->prev;
                    index -= 1;
                    return *last->item;
                }

                void remove() override {
                    if (last == null)
                        StateException().throws(__trace("core.util.LinkedList.ListItr"));
                    if (modNum != root.modNum)
                        ConcurrentException().throws(__trace("core.util.LinkedList.ListItr"));
                    LnkNode lastNext = last->next;
                    root.unlink(last);
                    if (cursor == last) cursor = lastNext; else index -= 1;
                    last = null;
                    modNum = root.modNum;
                }

                void set(const T &e) override {
                    if (last == null)
                        StateException().throws(__trace("core.util.LinkedList.ListItr"));
                    if (modNum != root.modNum)
                        ConcurrentException().throws(__trace("core.util.LinkedList.ListItr"));
                    last->item = &U::copyInstance(e, true);
                }

                void add(const T &e) override {
                    if (modNum != root.modNum)
                        ConcurrentException().throws(__trace("core.util.LinkedList.ListItr"));
                    last = null;
                    if (cursor == null)
                        root.linkLast(U::copyInstance(e, true));
                    else
                        root.linkBefore(U::copyInstance(e, true), cursor);
                    index += 1;
                    modNum = root.modNum;
                }

            };

            CORE_ALIAS(ITEM, typename Class<E>::Ptr);

            class Node {
            public:
                ITEM item;
                LnkNode next;
                LnkNode prev;

                CORE_FAST Node(LnkNode prev, E &element, LnkNode next) : prev(prev), item(&element), next(next) {}
            };

        public:

            /**
             * Returns a shallow copy of this <b>LinkedList</b>. (The elements
             * themselves are not cloned.)
             *
             * @return a shallow copy of this <b>LinkedList</b> instance
             */
            Object &clone() const override { return U::createInstance<LinkedList>(*this); }

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
             * @return an array containing all of the elements in this list
             *         in proper sequence
             */
            ReferenceArray toArray() const override {
                ReferenceArray a = ReferenceArray(len);
                gint i = 0;
                for (LnkNode x = first; x != null; x = x->next)
                    a.set(i++, *x->item);
                return a;
            }

            ~LinkedList() override {
                clear();
            }
        };

#if CORE_TEMPLATE_TYPE_DEDUCTION
        LinkedList() -> LinkedList<Object>;
        template<class T> LinkedList(Collection<T>) -> LinkedList<T>;
#endif

    }
} // core

#endif //CORE23_LINKEDLIST_H
