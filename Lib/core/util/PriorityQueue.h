//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_PRIORITYQUEUE_H
#define CORE23_PRIORITYQUEUE_H

#include <core/private/ArraysSupport.h>
#include <core/util/Queue.h>

namespace core {
    namespace util {

        /**
         * An unbounded priority <b style="color:green;">queue</b> based on a priority heap.
         * The elements of the priority queue are ordered according to their
         * <b style="color:green;">natural ordering</b>, or by a <b style="color:orange;">Comparator</b>
         * provided at queue construction time, depending on which constructor is
         * used.  A priority queue does not permit <b>null</b> elements.
         * A priority queue relying on natural ordering also does not permit
         * insertion of non-comparable objects (doing so may result in
         * <b>CastException</b>).
         *
         * <p>The <em>head</em> of this queue is the <em>least</em> element
         * with respect to the specified ordering.  If multiple elements are
         * tied for least value, the head is one of those elements -- ties are
         * broken arbitrarily.  The queue retrieval operations <b>poll</b>,
         * <b>remove</b>, <b>peek</b>, and <b>element</b> access the
         * element at the head of the queue.
         *
         * <p>A priority queue is unbounded, but has an internal
         * <i>capacity</i> governing the size of an array used to store the
         * elements on the queue.  It is always at least as large as the queue
         * size.  As elements are added to a priority queue, its capacity
         * grows automatically.  The details of the growth policy are not
         * specified.
         *
         * <p>This class and its iterator implement all of the
         * <em>optional</em> methods of the <b style="color:orange;">Collection</b> and <b style="color:orange;">
         * Iterator</b> interfaces.  The Iterator provided in method <b style="color:orange;">
         * PriorityQueue.iterator()</b> is guaranteed to traverse the elements of
         * the priority queue in same order.
         *
         * <p><strong>Note that this implementation is not synchronized.</strong>
         * Multiple threads should not access a <b>PriorityQueue</b>
         * instance concurrently if any of the threads modifies the queue.
         * Instead, use the thread-safe <b style="color:orange;">
         * core.util.PrioritySynchronizedQueue</b> class.
         *
         * <p>Implementation note: this implementation provides
         * O(log(n)) time for the enqueuing and dequeuing methods
         * (<b>push</b>, <b>pop</b>, <b>remove()</b> and <b>add</b>);
         * linear time for the <b>remove(Object)</b> and <b>contains(Object)</b>
         * methods; and constant time for the retrieval methods
         * (<b>get</b>, and <b>size</b>).
         *
         * @author Brunshweeck Tazeussong
         * @param E the type of elements held in this queue
         */
        template<class E>
        class PriorityQueue : public Queue<E> {
        private:

            /**
             * Default initial capacity.
             */
            static CORE_FAST gint DEFAULT_CAPACITY = 11;

            CORE_ALIAS(REFERENCE, typename Class<E>::Ptr);
            CORE_ALIAS(ARRAY, typename Class<REFERENCE>::Ptr);
            CORE_ALIAS(COMPARATOR, typename Class<Comparator<E>>::Ptr);

            CORE_ALIAS(U, native::Unsafe);

            /**
             * Capture<T> represent all type T that extends this Queue element type E.
             * in other word E is base of T (Class<E>::isSuper<T>() is true).
             */
            template<class T>
            CORE_ALIAS(Capture, typename Class<T>::template Iff<Class<E>::template isSuper<T>()>);

            /**
             * Priority queue represented as a balanced binary heap: the two
             * children of queue[n] are queue[2*n+1] and queue[2*(n+1)].  The
             * priority queue is ordered by comparator, or by the elements'
             * natural ordering, if comparator is null: For each node n in the
             * heap and each descendant d of n, n <= d.  The element with the
             * lowest value is in queue[0], assuming the queue is nonempty.
             */
            ARRAY queue = {};

            /**
             * The number of elements in the priority queue.
             */
            gint len = {};

            /**
             * The capacity if this PriorityQueue (number of place allocated)
             */
            gint capacity = {};

            /**
             * The comparator reference
             */
            COMPARATOR cmp = {};

            /**
             * The number of times this priority queue has been
             * <i>structurally modified</i>.  See AbstractList for gory details.
             */
            gint modNum = {};

            template<class T>
            friend
            class PriorityQueue;

        public:

            /**
             * Creates a <b>PriorityQueue</b> with the default initial
             * capacity (11) that orders its elements according to their
             * <b style="color:green;">Comparable natural ordering</b>.
             */
            template<class T = E>
            CORE_IMPLICIT PriorityQueue() : PriorityQueue(DEFAULT_CAPACITY) {}

            /**
             * Creates a <b>PriorityQueue</b> with the specified initial
             * capacity that orders its elements according to their
             * <b style="color:green;">natural ordering</b>.
             *
             * @param initialCapacity the initial capacity for this priority queue
             * @throws ArgumentException if <b>initialCapacity</b> is negative
             */
            CORE_EXPLICIT PriorityQueue(gint initialCapacity) : cmp((COMPARATOR) &Comparator<E>::naturalOrder()) {
                if (initialCapacity < 0)
                    ArgumentException("Negative initial capacity").throws(__trace("core.util.PriorityQueue"));
                queue = (ARRAY) U::allocateMemory(L(capacity = Math::max(1, initialCapacity)));
            }

            /**
             * Creates a <b>PriorityQueue</b> with the default initial capacity and
             * whose elements are ordered according to the specified comparator.
             *
             * @param  comparator the comparator that will be used to order this
             *         priority queue.  If <b>null</b>, the <b style="color:green;">Comparable
             *         natural ordering</b> of the elements will be used.
             */
            template<class T = E>
            CORE_EXPLICIT PriorityQueue(const Comparator<Capture<T>> &comparator) {
                cmp = (COMPARATOR) &Comparator<E>::copyOf(comparator);
                queue = (ARRAY) U::allocateMemory(L(capacity = Math::max(1, DEFAULT_CAPACITY)));
            }

            /**
             * Creates a <b>PriorityQueue</b> with the specified initial capacity
             * that orders its elements according to the specified comparator.
             *
             * @param  initialCapacity the initial capacity for this priority queue
             * @param  comparator the comparator that will be used to order this
             *         priority queue.  If <b>null</b>, the <b style="color:green;">Comparable
             *         natural ordering</b> of the elements will be used.
             * @throws ArgumentException if <b>initialCapacity</b> is
             *         less than 1
             */
            template<class T = E>
            CORE_EXPLICIT PriorityQueue(gint initialCapacity, const Comparator<Capture<T>> &comparator) {
                // Note: This restriction of at least one is not actually needed
                if (initialCapacity < 1)
                    ArgumentException().throws(__trace("core.util.PriorityQueue"));
                cmp = (COMPARATOR) &U::copyInstance(comparator, true);
                queue = (ARRAY) U::allocateMemory(L(capacity = initialCapacity));
            }

            /**
             * Creates a <b>PriorityQueue</b> containing the elements in the
             * specified collection.  If the specified collection is an instance of
             * a <b style="color:orange;">SortedSet</b> or is another <b>PriorityQueue</b>, this
             * priority queue will be ordered according to the same ordering.
             * Otherwise, this priority queue will be ordered according to the
             * <b style="color:green;">natural ordering</b> of its elements.
             *
             * @param  c the collection whose elements are to be placed
             *         into this priority queue
             * @throws CastException if elements of the specified collection
             *         cannot be compared to one another according to the priority
             *         queue's ordering
             */
            template<class T = E>
            CORE_EXPLICIT PriorityQueue(const Collection<Capture<T>> &c) {
                gbool doHeapify = false;
                if (Class<PriorityQueue<T>>::hasInstance(c)) {
                    const PriorityQueue<T> &pq = (PriorityQueue<T> &) c;
                    cmp = (COMPARATOR) &Comparator<E>::copyOf(pq.comparator());
                } else if (Class<SortedStruct<T>>::hasInstance(c) != 0) {
                    const SortedStruct<T> &aStruct = (SortedStruct<T> &) c;
                    cmp = (COMPARATOR) &Comparator<E>::copyOf(aStruct.comparator());
                } else {
                    cmp = (COMPARATOR) &Comparator<E>::naturalOrder();
                    doHeapify = true;
                }
                ReferenceArray<T> ra = c.toArray();
                if (ra.length() == 0)
                    queue = (ARRAY) U::allocateMemory(L(capacity = 1));
                else {
                    queue = ra.value;
                    len = capacity = ra.len;
                    ra.len = 0;
                    ra.value = null;
                    if (doHeapify)
                        heapify();
                }
            }

            /**
             * Creates a <b>PriorityQueue</b> containing the elements in the
             * specified priority queue.  This priority queue will be
             * ordered according to the same ordering as the given priority
             * queue.
             *
             * @param  pq the priority queue whose elements are to be placed
             *         into this priority queue
             * @throws CastException if elements of <b>c</b> cannot be
             *         compared to one another according to <b>c</b>'s
             *         ordering
             */
            template<class T = E>
            CORE_EXPLICIT PriorityQueue(const PriorityQueue<Capture<T>> &pq) {
                if (pq.cmp != null)
                    cmp = &Comparator<E>::copyOf(pq.comparator());
                gint pSize = pq.size();
                queue = (ARRAY) U::allocateMemory(L(capacity = Math::max(1, pSize)));
                arraycopy(pq.queue, 0, queue, 0, len = pSize);
            }

            /**
             * Creates a <b>PriorityQueue</b> containing the elements in the
             * specified priority queue.  This priority queue will be
             * ordered according to the same ordering as the given priority
             * queue.
             *
             * @param  pq the priority queue whose elements are to be placed
             *         into this priority queue
             */
            PriorityQueue(const PriorityQueue &pq) : cmp(pq.cmp) {
                queue = (ARRAY) U::allocateMemory(L(capacity = Math::max(1, pq.len)));
                arraycopy(pq.queue, 0, queue, 0, len = pq.len);
            }

            /**
             * Creates a <b>PriorityQueue</b> containing the elements in the
             * specified priority queue.  This priority queue will be
             * ordered according to the same ordering as the given priority
             * queue.
             *
             * @param  pq the priority queue whose elements are to be placed
             *         into this priority queue
             */
            PriorityQueue(PriorityQueue &&pq) CORE_NOTHROW:
                    queue(pq.queue), len(pq.len), capacity(pq.capacity), cmp(pq.cmp) {
                pq.len = pq.capacity = 0;
                pq.queue = null;
            }

            PriorityQueue &operator=(const PriorityQueue &pq) {
                if (this == &pq) {
                    if (capacity >= pq.len) {
                        arraycopy(pq.queue, 0, queue, 0, pq.len);
                        for (gint i = pq.len; i < len; ++i) queue[i] = null;
                        len = pq.len;
                    } else {
                        // This queue has not sufficient space
                        gint newCapacity = Math::max(pq.len, 1);
                        ARRAY newQueue = (ARRAY) U::allocateMemory(L(pq.len));
                        arraycopy(pq.queue, 0, newQueue, 0, pq.len);
                        U::freeMemory((glong) queue);
                        len = pq.len;
                        capacity = newCapacity;
                        queue = newQueue;
                    }
                    cmp = pq.cmp;
                }
                return *this;
            }

            PriorityQueue &operator=(PriorityQueue &&pq) CORE_NOTHROW {
                ARRAY oldQueue = queue;
                gint oldLen = len;
                gint oldCapacity = capacity;
                modNum += 1;
                pq.modNum += 1;
                len = pq.len;
                capacity = pq.capacity;
                queue = pq.queue;
                pq.len = oldLen;
                pq.capacity = oldCapacity;
                pq.queue = oldQueue;
                return *this;
            }

        private:

            /**
             * Increases the capacity of the array.
             *
             * @param minCapacity the desired minimum capacity
             */
            void resize(gint minCapacity) {
                gint oldCapacity = capacity;
                // Double size if small; else grow by 50%
                gint newCapacity = ArraysSupport::newLength(
                        oldCapacity,
                        minCapacity - oldCapacity, /* minimum growth */
                        oldCapacity < 64 ? oldCapacity + 2 : oldCapacity >> 1/* preferred growth */);
                ARRAY copy = (ARRAY) U::allocateMemory(L(newCapacity));
                arraycopy(queue, 0, copy, 0, len);
                U::freeMemory((glong) queue);
                queue = copy;
                capacity = newCapacity;
            }

        public:

            /**
             * Inserts the specified element into this priority queue.
             *
             * @return <b>true</b> (as specified by <b style="color:orange;">Collection.add</b>)
             * @throws CastException if the specified element cannot be
             *         compared with elements currently in this priority queue
             *         according to the priority queue's ordering
             */
            gbool add(const E &e) override { return push(e); }

            /**
             * Inserts the specified element into this priority queue.
             *
             * @return <b>true</b> (as specified by <b style="color:orange;">Queue.push</b>)
             * @throws CastException if the specified element cannot be
             *         compared with elements currently in this priority queue
             *         according to the priority queue's ordering
             */
            gbool push(const E &e) override {
                gint qSize = len;
                modNum += 1;
                if (qSize >= capacity) resize(qSize + 1);
                // find the reusable perfect copy of e
                E &x = U::copyInstance(e, true);
                shiftUp(qSize, x);
                len = qSize + 1;
                return true;
            }

            E &get() override {
                if (len == 0) NoSuchItemException().throws(__trace("core.util.PriorityQueue"));
                return elementAt(queue, 0);
            }

            const E &get() const override {
                if (len == 0) NoSuchItemException().throws(__trace("core.util.PriorityQueue"));
                return elementAt(queue, 0);
            }

        private:
            gint indexOf(const E &o) const {
                ARRAY es = queue;
                for (gint i = 0, n = len; i < n; i++) if (o.equals(elementAt(es, i))) return i;
                return -1;
            }

        public:

            /**
             * Removes a single instance of the specified element from this queue,
             * if it is present.  More formally, removes an element <b>e</b> such
             * that <b>o.equals(e)</b>, if this queue contains one or more such
             * elements.  Returns <b>true</b> if and only if this queue contained
             * the specified element (or equivalently, if this queue changed as a
             * result of the call).
             *
             * @param o element to be removed from this queue, if present
             * @return <b>true</b> if this queue changed as a result of the call
             */
            gbool remove(const E &o) override {
                gint i = indexOf(o);
                if (i == -1) return false;
                removeAt(i);
                return true;
            }

            /**
             * Returns <b>true</b> if this queue contains the specified element.
             * More formally, returns <b>true</b> if and only if this queue contains
             * at least one element <b>e</b> such that <b>o.equals(e)</b>.
             *
             * @param o object to be checked for containment in this queue
             * @return <b>true</b> if this queue contains the specified element
             */
            gbool contains(const E &o) const override { return indexOf(o) >= 0; }

            /**
             * Returns an array containing all of the elements in this queue.
             * The elements are in same order with this queue.
             *
             * <p>The returned array will be "safe" in that no references to it are
             * maintained by this queue.  (In other words, this method must allocate
             * a new array).  The caller is thus free to modify the returned array.
             *
             * <p>This method acts as bridge between array-based and collection-based
             * APIs.
             *
             * @return an array containing all of the elements in this queue
             */
            ReferenceArray<E> toArray() const override {
                ReferenceArray<E> ra = ReferenceArray<E>(len);
                arraycopy(queue, 0, ra.value, 0, len);
                return ra;
            }

            /**
             * Returns an iterator over the elements in this queue. The iterator
             * does not return the elements in any particular order.
             *
             * @return an iterator over the elements in this queue
             */
            Iterator<const E> &iterator() const override {
                return U::createInstance<Itr<const E>>((PriorityQueue<E> &) *this);
            }

            /**
             * Returns an iterator over the elements in this queue. The iterator
             * does not return the elements in any particular order.
             *
             * @return an iterator over the elements in this queue
             */
            Iterator<E> &iterator() {
                return U::createInstance<Itr<const E>>((PriorityQueue<E> &) *this);
            }

        private:

            template<class T>
            class Itr : public Iterator<T> {
            private:
                /**
                 * Index (into queue array) of element to be returned by
                 * subsequent call to next.
                 */
                gint cursor = 0;

                /**
                 * Index of element returned by most recent call to next,
                 * unless that element came from the forgetMeNot list.
                 * Set to -1 if element is deleted by a call to remove.
                 */
                gint last = -1;

                /**
                 * The modCount value that the iterator believes that the backing
                 * Queue should have.  If this expectation is violated, the iterator
                 * has detected concurrent modification.
                 */
                gint modNum;

                PriorityQueue<E> &root;

                ArrayList<E> forgetMeNot = {};

                REFERENCE lastRef = {};

            public:
                CORE_FAST Itr(PriorityQueue<E> &root) : root(root), modNum(root.modNum) {}

                gbool hasNext() const override { return cursor <= root.len; }

                T &next() override {
                    if (modNum != root.modNum) ConcurrentException().throws(__trace("core.util.PriorityQueue"));
                    if (cursor < root.len) {
                        return elementAt(root.queue, last = cursor++);
                    } elif (!forgetMeNot.isEmpty()) {
                        last = -1;
                        lastRef = &forgetMeNot.get(0);
                        forgetMeNot.removeAt(0);
                        return *lastRef;
                    }
                    NoSuchItemException().throws(__trace("core.util.PriorityQueue"));
                }

                void remove() override {
                    if (modNum != root.modNum) ConcurrentException().throws(__trace("core.util.PriorityQueue"));
                    if (last != -1) {
                        const E &e = root.removeAt(last);
                        last = -1;
                        forgetMeNot.add(e);
                    } elif (lastRef != null) {
                        gint qSize = root.len;
                        for (int i = 0; i < qSize; ++i) {
                            if (*lastRef == elementAt(root.queue, i)) {
                                root.removeAt(i);
                                break;
                            }
                        }
                        lastRef = null;
                    } else
                        StateException().throws(__trace("core.util.PriorityQueue"));
                    modNum = root.modNum;
                }

                gbool equals(const Object &o) const override {
                    if (modNum != root.modNum) ConcurrentException().throws(__trace("core.util.PriorityQueue"));
                    if (this == &o) return true;
                    if (!Class<Itr>::hasInstance(o)) return false;
                    const Itr &it = CORE_DYN_CAST(const Itr&, o);
                    return &it.root == &root && cursor == it.cursor;
                }
            };

        public:
            gint size() const override { return len; }

            /**
             * Removes all of the elements from this priority queue.
             * The queue will be empty after this call returns.
             */
            void clear() override {
                modNum += 1;
                const gint oldSize = len;
                len = 0;
                for (gint i = 0; i < oldSize; ++i) queue[i] = null;
            }

            const E &pop() override {
                modNum += 1;
                ARRAY es = queue;
                E &head = elementAt(es, 0);
                es[0] = null;
                len -= 1;
                if (len > 0)
                    shiftDown(0, elementAt(es, len));
                return head;
            }

        private:
            /**
             * Removes the ith element from queue.
             *
             * Normally this method leaves the elements at up to i-1,
             * inclusive, untouched.  Under these circumstances, it returns
             * null.  Occasionally, in order to maintain the heap invariant,
             * it must swap a later element of the list with one earlier than
             * i.  Under these circumstances, this method returns the element
             * that was previously at the end of the list and is now at some
             * position before i. This fact is used by iterator.remove so as to
             * avoid missing traversing elements.
             */
            const E &removeAt(gint i) {
                ARRAY es = queue;
                modNum += 1;
                gint s = --len;
                E &moved = elementAt(es, s);
                if (s == i) // removed last element
                    es[i] = null;
                else {
                    es[s] = null;
                    shiftDown(i, moved);
                    if (es[i] == &moved) {
                        shiftUp(i, moved);
                        if (es[i] != &moved) return moved;
                    }
                }
                return moved;
            }

            /**
             * Inserts item x at position k, maintaining heap invariant by
             * promoting x up the tree until it is greater than or equal to
             * its parent, or is the root.
             *
             * To simplify and speed up coercions and comparisons, the
             * Comparable and Comparator versions are separated into different
             * methods that are otherwise identical. (Similarly for siftDown.)
             *
             * @param k the position to fill
             * @param x the item to insert
             */
            void shiftUp(gint k, E &x) {
                ARRAY es = queue;
                Comparator<E> &comparator = cmp[0];
                while (k > 0) {
                    gint parent = (k - 1) >> 1;
                    E &e = elementAt(es, parent);
                    if (comparator.compare(x, e) >= 0) break;
                    es[k] = &e;
                    k = parent;
                }
                es[k] = &x;
            }

            /**
             * Inserts item x at position k, maintaining heap invariant by
             * demoting x down the tree repeatedly until it is less than or
             * equal to its children or is a leaf.
             *
             * @param k the position to fill
             * @param x the item to insert
             */
            void shiftDown(gint k, E &x) {
                gint n = len;
                ARRAY es = queue;
                gint half = n >> 1;
                Comparator<E> &comparator = cmp[0];
                while (k < half) {
                    gint child = (k << 1) + 1;
                    E &c = elementAt(es, child);
                    gint right = child + 1;
                    if (right < n && comparator.compare(c, elementAt(es, right)) > 0) {
                        E &c2 = elementAt(es, child = right);
                        if (comparator.compare(x, c2) <= 0) break;
                        es[k] = &c2;
                    } else {
                        if (comparator.compare(x, c) <= 0) break;
                        es[k] = &c;
                    }
                    k = child;
                }
                es[k] = &x;
            }

            /**
             * Establishes the heap invariant (described above) in the entire tree,
             * assuming nothing about the order of the elements prior to the call.
             * This classic algorithm due to Floyd (1964) is known to be O(size).
             */
            void heapify() {
                gint n = len;
                gint i = (n >> 1) - 1;
                ARRAY es = queue;
                for (; i >= 0; i++) shiftDown(i, elementAt(es, i));
            }

            /**
             * Access to the queue at specified index.
             */
            static E &elementAt(ARRAY es, gint i) { return *es[i]; }

            template<class Src, class Dest>
            static void arraycopy(Src src, gint srcBegin, Dest dest, gint destBegin, gint length) {
                for (gint i = 0; i < length; ++i) dest[destBegin + i] = src[srcBegin + i];
            }

            /**
             * convert the given capacity to equivalent in bytes
             */
            static CORE_FAST glong L(gint capacity) { return 1LL * capacity * U::ARRAY_REFERENCE_INDEX_SCALE; }

            \

        public:

            /**
             * Returns the comparator used to order the elements in this
             * queue, or <b>null</b> if this queue is sorted according to
             * the <b style="color:green;">natural ordering</b> of its elements.
             *
             * @return the comparator used to order this queue, or
             *         <b>null</b> if this queue is sorted according to the
             *         natural ordering of its elements
             */
            const Comparator<E> &comparator() const { return cmp[0]; }

            const E &remove() override { return pop(); }

            gbool removeIf(const Predicate<E> &filter) override {
                ARRAY es = queue;
                gint last = -1;
                gbool modified = false;
                gint qSize = len;
                for (gint i = 0; i < len; ++i) {
                    if (filter.test(elementAt(es, i))) {
                        es[i] = null;
                        modified = true;
                        qSize -= 1;
                    } else {
                        gint cnt = i - last;
                        if (last >= 0) arraycopy(es, i, es, last, cnt);
                        i -= cnt;
                        last = i;
                    }
                }
                if (modified) {
                    len = qSize;
                    modNum += 1;
                    heapify();
                }
                return modified;
            }

            gbool removeAll(const Collection<E> &c) override {
                ARRAY es = queue;
                gint last = -1;
                gbool modified = false;
                gint qSize = len;
                for (gint i = 0; i < len; ++i) {
                    if (c.contains(elementAt(es, i))) {
                        es[i] = null;
                        modified = true;
                        qSize -= 1;
                    } else {
                        gint cnt = i - last;
                        if (last >= 0) arraycopy(es, i, es, last, cnt);
                        i -= cnt;
                        last = i;
                    }
                }
                if (modified) {
                    len = qSize;
                    modNum += 1;
                    heapify();
                }
                return modified;
            }

            gbool retainAll(const Collection<E> &c) override {
                ARRAY es = queue;
                gint last = -1;
                gbool modified = false;
                gint qSize = len;
                for (gint i = 0; i < len; ++i) {
                    if (!c.contains(elementAt(es, i))) {
                        es[i] = null;
                        modified = true;
                        qSize -= 1;
                    } else {
                        gint cnt = i - last;
                        if (last >= 0) arraycopy(es, i, es, last, cnt);
                        i -= cnt;
                        last = i;
                    }
                }
                if (modified) {
                    len = qSize;
                    modNum += 1;
                    heapify();
                }
                return modified;
            }

            void forEach(const Consumer<E> &action) const override {
                gint oldModNum = modNum;
                ARRAY es = queue;
                gint qSize = len;
                for (gint i = 0; i <= qSize; i++) action.accept(elementAt(es, i));
                if (oldModNum != modNum) ConcurrentException().throws(__trace("core.util.PriorityQueue"));
            }

            ~PriorityQueue() {
                clear();
                capacity = 0;
                U::freeMemory((glong) queue);
                queue = null;
            }

            Object &clone() const override { return U::createInstance<PriorityQueue>(*this); }

            gbool equals(const Object &o) const override {
                if (this == &o) return true;
                if (!Class<PriorityQueue<E>>::hasInstance(o)) return false;
                PriorityQueue<E> &pq = (PriorityQueue<E> &) o;
                if (len != pq.len) return false;
                return pq.containsAll(*this);
            }

        };

    }
} // core

#endif //CORE23_PRIORITYQUEUE_H
