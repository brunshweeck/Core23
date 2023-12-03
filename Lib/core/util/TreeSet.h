//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_TREESET_H
#define CORE23_TREESET_H

#include "Set.h"

namespace core {
    namespace util {

        /**
         * A Red-Black tree based <b style="color:orange;">Set</b> implementation.
         * The set is sorted according to the <b style="color:green;">natural
         * ordering</b> of its keys, or by a <b style="color:orange;">Comparator</b> provided at set
         * creation time, depending on which constructor is used.
         *
         * <p>This implementation provides guaranteed log(n) time cost for the basic
         * operations (<b>add</b>, <b>remove</b> and <b>contains</b>).
         *
         * <p>Note that the ordering maintained by a set (whether or not an explicit
         * comparator is provided) must be <i>consistent with equals</i> if it is to
         * correctly implement the <b>Set</b> interface.  (See <b>Comparable</b>
         * or <b>Comparator</b> for a precise definition of <i>consistent with
         * equals</i>.)  This is so because the <b>Set</b> interface is defined in
         * terms of the <b>equals</b> operation, but a <b>TreeSet</b> instance
         * performs all element comparisons using its <b>compareTo</b> (or
         * <b>compare</b>) method, so two elements that are deemed equal by this method
         * are, from the standpoint of the set, equal.  The behavior of a set
         * <i>is</i> well-defined even if its ordering is inconsistent with equals; it
         * just fails to obey the general contract of the <b>Set</b> interface.
         *
         * <p><strong>Note that this implementation is not synchronized.</strong>
         * If multiple threads access a tree set concurrently, and at least one
         * of the threads modifies the set, it <i>must</i> be synchronized
         * externally.  This is typically accomplished by synchronizing on some
         * object that naturally encapsulates the set.
         * If no such object exists, the set should be "wrapped" using the
         * <b style="color:orange;">core.util.SynchronizedSortedSet</b>
         * method.
         *
         * <p>The iterators returned by this class's <b>iterator</b> method are
         * <i>fail-fast</i>: if the set is modified at any time after the iterator is
         * created, in any way except through the iterator's own <b>remove</b>
         * method, the iterator will throw a <b style="color:orange;">ConcurrentException</b>.
         * Thus, in the face of concurrent modification, the iterator fails quickly
         * and cleanly, rather than risking arbitrary, non-deterministic behavior at
         * an undetermined time in the future.
         *
         * <p>Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw <b>ConcurrentException</b> on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness:   <i>the fail-fast behavior of iterators
         * should be used only to detect bugs.</i>
         *
         * @param E the type of elements maintained by this set
         *
         * @author  Brunshweeck Tazeussong
         * @see     Collection
         * @see     Set
         * @see     HashSet
         * @see     Comparable
         * @see     Comparator
         * @see     TreeMap
         */
        template<class E>
        class TreeSet : public Set<E> {

        };

    }
} // core

#endif //CORE23_TREESET_H
