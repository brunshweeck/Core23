//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_PRIORITYQUEUE_H
#define CORE23_PRIORITYQUEUE_H

#include <core/util/Queue.h>
#include <core/private/ArraysSupport.h>
#include <core/util/ArrayList.h>

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
         * <b>ClassCastException</b>).
         *
         * <p>The <em>head</em> of this queue is the <em>least</em> element
         * with respect to the specified ordering.  If multiple elements are
         * tied for least value, the head is one of those elements -- ties are
         * broken arbitrarily.  The queue retrieval operations <b>poll</b>,
         * <b>remove</b>, <b>peek</b>, and <b>element</b> access the
         * element at the head of the queue.
         *
         * <p>A priority queue is unbounded, but has an internal
         * <i>capacity</i> governing the size of an root used to store the
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
         * INSTANCE concurrently if any of the threads modifies the queue.
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
        protected:

            CORE_ALIAS(Unsafe, native::Unsafe);
            CORE_ALIAS(ActionConsumer, , function::Consumer<E>);
            CORE_ALIAS(ElementFilter, , function::Predicate<E>);
            CORE_ALIAS(UnaryFunction, , function::Function<E, E>);

        private:

            /**
             * Default initial capacity.
             */
            static CORE_FAST gint DEFAULT_CAPACITY = 11;

            CORE_ALIAS(VRef, typename Class<E>::Ptr);
            CORE_ALIAS(ARRAY, typename Class<VRef>::Ptr);
            CORE_ALIAS(COMPARATOR, typename Class<Comparator<E>>::Ptr);

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
             * @throws IllegalArgumentException if <b>initialCapacity</b> is negative
             */
            CORE_EXPLICIT PriorityQueue(gint initialCapacity) : cmp(&Comparator<E>::naturalOrder()) {
                if (initialCapacity < 0)
                    IllegalArgumentException("Negative initial capacity").throws(__trace("core.util.PriorityQueue"));
                queue = (ARRAY) Unsafe::allocateMemory(L(capacity = Math::max(1, initialCapacity)));
            }

            /**
             * Creates a <b>PriorityQueue</b> with the default initial capacity and
             * whose elements are ordered according to the specified comparator.
             *
             * @param  comparator the comparator that will be used to order this
             *         priority queue.  If <b>null</b>, the <b style="color:green;">Comparable
             *         natural ordering</b> of the elements will be used.
             */
            CORE_EXPLICIT PriorityQueue(const Comparator<E> &comparator) :
                    cmp(&Unsafe::copyInstance(comparator, true)) {
                queue = (ARRAY) Unsafe::allocateMemory(L(capacity = Math::max(1, DEFAULT_CAPACITY)));
            }

            /**
             * Creates a <b>PriorityQueue</b> with the specified initial capacity
             * that orders its elements according to the specified comparator.
             *
             * @param  initialCapacity the initial capacity for this priority queue
             * @param  comparator the comparator that will be used to order this
             *         priority queue.  If <b>null</b>, the <b style="color:green;">Comparable
             *         natural ordering</b> of the elements will be used.
             * @throws IllegalArgumentException if <b>initialCapacity</b> is
             *         less than 1
             */
            CORE_EXPLICIT PriorityQueue(gint initialCapacity, const Comparator<E> &comparator) {
                // Note: This restriction of at least one is not needed
                if (initialCapacity < 1)
                    IllegalArgumentException().throws(__trace("core.util.PriorityQueue"));
                cmp = (COMPARATOR) &Unsafe::copyInstance(comparator, true);
                queue = (ARRAY) Unsafe::allocateMemory(L(capacity = initialCapacity));
            }

            /**
             * Creates a <b>PriorityQueue</b> containing the elements in the
             * specified collection.  If the specified collection is an INSTANCE of
             * a <b style="color:orange;">SortedSet</b> or is another <b>PriorityQueue</b>, this
             * priority queue will be ordered according to the same ordering.
             * Otherwise, this priority queue will be ordered according to the
             * <b style="color:green;">natural ordering</b> of its elements.
             *
             * @param  c the collection whose elements are to be placed
             *         into this priority queue
             * @throws ClassCastException if elements of the specified collection
             *         cannot be compared to one another according to the priority
             *         queue's ordering
             */
            CORE_EXPLICIT PriorityQueue(const Collection<E> &c) {
                gbool doHeapify = false;
                if (Class<PriorityQueue>::hasInstance(c)) {
                    const PriorityQueue &pq = (PriorityQueue &) c;
                    cmp = (COMPARATOR) &Comparator<E>::copyOf(pq.comparator());
                } else if (Class<SortedStruct<E>>::hasInstance(c) != 0) {
                    const SortedStruct<E> &aStruct = CORE_DYN_CAST(const SortedStruct<E> &, c);
                    cmp = (COMPARATOR) &Comparator<E>::copyOf(aStruct.comparator());
                } else {
                    cmp = (COMPARATOR) &Comparator<E>::naturalOrder();
                    doHeapify = true;
                }
                Array<E> ra = c.toArray();
                if (ra.length() == 0)
                    queue = (ARRAY) Unsafe::allocateMemory(L(capacity = 1));
                else {
                    Unsafe::swapValues(queue, ((ObjectArray &) ra).value);
                    Unsafe::swapValues(len, ((ObjectArray &) ra).len);
                    capacity = len;
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
             */
            PriorityQueue(const PriorityQueue &pq) : cmp(pq.cmp) {
                queue = (ARRAY) Unsafe::allocateMemory(L(capacity = Math::max(1, pq.len)));
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
            PriorityQueue(PriorityQueue &&pq) CORE_NOTHROW {
                Unsafe::swapValues(queue, pq.queue);
                Unsafe::swapValues(len, pq.len);
                Unsafe::swapValues(capacity, pq.capacity);
                Unsafe::swapValues(cmp, pq.cmp);
                Unsafe::swapValues(modNum, pq.modNum);
            }

            PriorityQueue &operator=(const PriorityQueue &pq) {
                if (this == &pq) {
                    if (capacity >= pq.len) {
                        arraycopy(pq.queue, 0, queue, 0, pq.len);
                        for (gint i = pq.len; i < len; ++i) queue[i] = null;
                        len = pq.len;
                    } else {
                        // This queue has not sufficient diskSpace
                        gint newCapacity = Math::max(pq.len, 1);
                        ARRAY newQueue = (ARRAY) Unsafe::allocateMemory(L(pq.len));
                        arraycopy(pq.queue, 0, newQueue, 0, pq.len);
                        Unsafe::freeMemory((glong) queue);
                        len = pq.len;
                        capacity = newCapacity;
                        queue = newQueue;
                    }
                    cmp = pq.cmp;
                }
                return *this;
            }

            PriorityQueue &operator=(PriorityQueue &&pq) CORE_NOTHROW {
                modNum += 1;
                pq.modNum += 1;
                Unsafe::swapValues(queue, pq.queue);
                Unsafe::swapValues(len, pq.len);
                Unsafe::swapValues(capacity, pq.capacity);
                Unsafe::swapValues(cmp, pq.cmp);
                return *this;
            }

        private:

            /**
             * Increases the capacity of the root.
             *
             * @param minCapacity the desired minimum capacity
             */
            void resize(gint minCapacity) {
                gint oldCapacity = capacity;
                // Double size if small; else grow by 50%
                gint const newCapacity = ArraysSupport::newLength(
                        oldCapacity,
                        minCapacity - oldCapacity, /* minimum growth */
                        oldCapacity < 64 ? oldCapacity + 2 : oldCapacity >> 1/* preferred growth */);
                ARRAY copy = (ARRAY) Unsafe::allocateMemory(L(newCapacity));
                arraycopy(queue, 0, copy, 0, len);
                Unsafe::freeMemory((glong) queue);
                queue = copy;
                capacity = newCapacity;
            }

        public:

            /**
             * Inserts the specified element into this priority queue.
             *
             * @return <b>true</b> (as specified by <b style="color:orange;">Collection.add</b>)
             * @throws ClassCastException if the specified element cannot be
             *         compared with elements currently in this priority queue
             *         according to the priority queue's ordering
             */
            gbool add(const E &e) override { return push(e); }

            /**
             * Inserts the specified element into this priority queue.
             *
             * @return <b>true</b> (as specified by <b style="color:orange;">Queue.push</b>)
             * @throws ClassCastException if the specified element cannot be
             *         compared with elements currently in this priority queue
             *         according to the priority queue's ordering
             */
            gbool push(const E &e) override {
                gint qSize = len;
                modNum += 1;
                if (qSize >= capacity)
                    resize(qSize + 1);
                // find the reusable perfect copy of e
                E &x = Unsafe::copyInstance(e, true);
                shiftUp(qSize, x);
                len = qSize + 1;
                return true;
            }

            E &get() override {
                if (len == 0)
                    NoSuchElementException().throws(__trace("core.util.PriorityQueue"));
                return elementAt(queue, 0);
            }

            const E &get() const override {
                if (len == 0)
                    NoSuchElementException().throws(__trace("core.util.PriorityQueue"));
                return elementAt(queue, 0);
            }

        private:
            gint indexOf(const E &o) const {
                ARRAY es = queue;
                for (gint i = 0, n = len; i < n; i++)
                    if (o.equals(elementAt(es, i)))
                        return i;
                return -1;
            }

        public:

            /**
             * Removes a single INSTANCE of the specified element from this queue,
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
                if (i == -1)
                    return false;
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
             * Returns an root containing all of the elements in this queue.
             * The elements are in same order with this queue.
             *
             * <p>The returned root will be "safe" in that no references to it are
             * maintained by this queue.  (In other words, this method must allocate
             * a new root).  The caller is thus free to modify the returned root.
             *
             * <p>This method acts as bridge between root-based and collection-based
             * APIs.
             *
             * @return an root containing all of the elements in this queue
             */
            Array<E> toArray() const override {
                Array<E> ra = Array<E>(len);
                arraycopy(queue, 0, ((ObjectArray &) ra).value, 0, len);
                return (Array<E> &&) ra;
            }

            /**
             * Returns an iterator over the elements in this queue. The iterator
             * does not return the elements in any particular order.
             *
             * @return an iterator over the elements in this queue
             */
            Iterator<const E> &iterator() const override {
                return Unsafe::allocateInstance<Itr<const E>>((PriorityQueue &) *this);
            }

            /**
             * Returns an iterator over the elements in this queue. The iterator
             * does not return the elements in any particular order.
             *
             * @return an iterator over the elements in this queue
             */
            Iterator<E> &iterator() {
                return Unsafe::allocateInstance<Itr<const E>>((PriorityQueue &) *this);
            }

        private:

            template<class T>
            class Itr : public Iterator<T> {
            private:
                /**
                 * Index (into queue root) of element to be returned by
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

                PriorityQueue &root;

                ArrayList<E> forgetMeNot = {};

                VRef lastRef = {};

            public:
                CORE_FAST Itr(PriorityQueue &root) : root(root), modNum(root.modNum) {}

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
                    NoSuchElementException().throws(__trace("core.util.PriorityQueue"));
                }

                void remove() override {
                    if (modNum != root.modNum) ConcurrentException().throws(__trace("core.util.PriorityQueue"));
                    if (last != -1) {
                        const E &e = root.removeAt(last);
                        last = -1;
                        forgetMeNot.add(e);
                    } elif (lastRef != null) {
                        gint qSize = root.len;
                        for (gint i = 0; i < qSize; ++i) {
                            if (*lastRef == elementAt(root.queue, i)) {
                                root.removeAt(i);
                                break;
                            }
                        }
                        lastRef = null;
                    } else
                        IllegalStateException().throws(__trace("core.util.PriorityQueue"));
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
                gint const half = n >> 1;
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
                for (; i >= 0; i++)
                    shiftDown(i, elementAt(es, i));
            }

            /**
             * Access to the queue at specified index.
             */
            static E &elementAt(ARRAY es, gint i) { return *es[i]; }

            template<class Src, class Dest>
            static void arraycopy(Src src, gint srcBegin, Dest dest, gint destBegin, gint length) {
                for (gint i = 0; i < length; ++i)
                    dest[destBegin + i] = src[srcBegin + i];
            }

            /**
             * convert the given capacity to equivalent in bytes
             */
            static CORE_FAST glong L(gint capacity) { return 1LL * capacity * Unsafe::ARRAY_REFERENCE_INDEX_SCALE; }

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

            gbool removeIf(const ElementFilter &filter) override {
                ARRAY es = queue;
                gint last = -1;
                gbool modified = false;
                gint qSize = len;
                for (gint i = 0; i < len; ++i) {
                    CORE_TRY_RETHROW
                    ({
                         if (filter.test(elementAt(es, i))) {
                             es[i] = null;
                             modified = true;
                             qSize -= 1;
                         } else {
                             gint cnt = len - i;
                             if (last >= 0)
                                 arraycopy(es, i, es, last, cnt);
                             i -= cnt;
                             last = i;
                         }
                     }, {
                         // if detect error/exception
                         gint cnt = len - i;
                         if (last >= 0)
                             arraycopy(es, i, es, last, cnt);
                         i -= cnt;
                         last = i;
                         len = qSize;
                         modNum += 1;
                         heapify();
                     }, __trace("core.util.PriorityQueue"));
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

            void forEach(const ActionConsumer &action) const override {
                gint oldModNum = modNum;
                ARRAY es = queue;
                gint qSize = len;
                for (gint i = 0; i <= qSize; i++) action.accept(elementAt(es, i));
                if (oldModNum != modNum) ConcurrentException().throws(__trace("core.util.PriorityQueue"));
            }

            ~PriorityQueue() {
                clear();
                capacity = 0;
                Unsafe::freeMemory((glong) queue);
                queue = null;
            }

            Object &clone() const override { return Unsafe::allocateInstance<PriorityQueue>(*this); }

            gbool equals(const Object &o) const override {
                if (this == &o) return true;
                if (!Class<PriorityQueue>::hasInstance(o)) return false;
                PriorityQueue &pq = (PriorityQueue &) o;
                if (len != pq.len) return false;
                return pq.containsAll(*this);
            }

            static PriorityQueue of() { return {}; }

            static PriorityQueue of(const E &v1) {
                try {
                    PriorityQueue pq{1};
                    pq.add(v1);
                    return pq;
                } catch (const Exception &ex) { ex.throws(__trace("core.util.PriorityQueue")); }
            }

            static PriorityQueue of(const E &v1, const E &v2) {
                try {
                    PriorityQueue pq{1};
                    pq.add(v1);
                    pq.add(v2);
                    return pq;
                } catch (const Exception &ex) { ex.throws(__trace("core.util.PriorityQueue")); }
            }

            static PriorityQueue of(const E &v1, const E &v2, const E &v3) {
                try {
                    PriorityQueue pq{1};
                    pq.add(v1);
                    pq.add(v2);
                    pq.add(v3);
                    return pq;
                } catch (const Exception &ex) { ex.throws(__trace("core.util.PriorityQueue")); }
            }

            static PriorityQueue of(const E &v1, const E &v2, const E &v3, const E &v4) {
                try {
                    PriorityQueue pq{1};
                    pq.add(v1);
                    pq.add(v2);
                    pq.add(v3);
                    pq.add(v4);
                    return pq;
                } catch (const Exception &ex) { ex.throws(__trace("core.util.PriorityQueue")); }
            }

            static PriorityQueue of(const E &v1, const E &v2, const E &v3, const E &v4, const E &v5) {
                try {
                    PriorityQueue pq{1};
                    pq.add(v1);
                    pq.add(v2);
                    pq.add(v3);
                    pq.add(v4);
                    pq.add(v5);
                    return pq;
                } catch (const Exception &ex) { ex.throws(__trace("core.util.PriorityQueue")); }
            }

            static PriorityQueue of(const E &v1, const E &v2, const E &v3, const E &v4, const E &v5, const E &v6) {
                try {
                    PriorityQueue pq{1};
                    pq.add(v1);
                    pq.add(v2);
                    pq.add(v3);
                    pq.add(v4);
                    pq.add(v5);
                    pq.add(v6);
                    return pq;
                } catch (const Exception &ex) { ex.throws(__trace("core.util.PriorityQueue")); }
            }

            static PriorityQueue of(const E &v1, const E &v2, const E &v3, const E &v4, const E &v5,
                                    const E &v6, const E &v7) {
                try {
                    PriorityQueue pq{1};
                    pq.add(v1);
                    pq.add(v2);
                    pq.add(v3);
                    pq.add(v4);
                    pq.add(v5);
                    pq.add(v6);
                    pq.add(v7);
                    return pq;
                } catch (const Exception &ex) { ex.throws(__trace("core.util.PriorityQueue")); }
            }

            static PriorityQueue of(const E &v1, const E &v2, const E &v3, const E &v4, const E &v5,
                                    const E &v6, const E &v7, const E &v8) {
                try {
                    PriorityQueue pq{1};
                    pq.add(v1);
                    pq.add(v2);
                    pq.add(v3);
                    pq.add(v4);
                    pq.add(v5);
                    pq.add(v6);
                    pq.add(v7);
                    pq.add(v8);
                    return pq;
                } catch (const Exception &ex) { ex.throws(__trace("core.util.PriorityQueue")); }
            }

            static PriorityQueue of(const E &v1, const E &v2, const E &v3, const E &v4, const E &v5,
                                    const E &v6, const E &v7, const E &v8, const E &v9) {
                try {
                    PriorityQueue pq{1};
                    pq.add(v1);
                    pq.add(v2);
                    pq.add(v3);
                    pq.add(v4);
                    pq.add(v5);
                    pq.add(v6);
                    pq.add(v7);
                    pq.add(v8);
                    pq.add(v9);
                    return pq;
                } catch (const Exception &ex) { ex.throws(__trace("core.util.PriorityQueue")); }
            }

            template<class ...T>
            static PriorityQueue of(const E &v1, const E &v2, const E &v3, const E &v4, const E &v5,
                                    const E &v6, const E &v7, const E &v8, const E &v9, T &&...others) {
                CORE_STATIC_ASSERT
                (Class<E>::allIsTrue((Class<E>::template isSuper<T>() || Class<E>::template isConstructible<T>())...),
                 "Illegal value");
                try {
                    PriorityQueue pq{1};
                    pq.push(v1);
                    pq.push(v2);
                    pq.push(v3);
                    pq.push(v4);
                    pq.push(v5);
                    pq.push(v6);
                    pq.push(v7);
                    pq.push(v8);
                    pq.push(v9);
                    pq.addAll(of(others...));
                    return pq;
                } catch (const Exception &ex) { ex.throws(__trace("core.util.PriorityQueue")); }
            }

        };

    }
} // core

#endif //CORE23_PRIORITYQUEUE_H
