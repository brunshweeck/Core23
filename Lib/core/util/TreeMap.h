//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_TREEMAP_H
#define CORE23_TREEMAP_H

#include "Map.h"

namespace core {
    namespace util {

        /**
         * A Red-Black tree based <b style="color:orange;">Map</b> implementation.
         * The map is sorted according to the <b style="color:green;">natural
         * ordering</b> of its keys, or by a <b style="color:orange;">Comparator</b> provided at map
         * creation time, depending on which constructor is used.
         *
         * <p>This implementation provides guaranteed log(n) time cost for the
         * <b>containsKey</b>, <b>get</b>, <b>put</b> and <b>remove</b>
         * operations.  Algorithms are adaptations of those in Cormen, Leiserson, and
         * Rivest's <em>Introduction to Algorithms</em>.
         *
         * <p>Note that the ordering maintained by a tree map, like any sorted map, and
         * whether or not an explicit comparator is provided, must be <em>consistent
         * with <b>equals</b></em> if this sorted map is to correctly implement the
         * <b>Map</b> interface.  (See <b>Comparable</b> or <b>Comparator</b> for a
         * precise definition of <em>consistent with equals</em>.)  This is so because
         * the <b>Map</b> interface is defined in terms of the <b>equals</b>
         * operation, but a sorted map performs all key comparisons using its <b>
         * compareTo</b> (or <b>compare</b>) method, so two keys that are deemed equal by
         * this method are, from the standpoint of the sorted map, equal.  The behavior
         * of a sorted map <em>is</em> well-defined even if its ordering is
         * inconsistent with <b>equals</b>; it just fails to obey the general contract
         * of the <b>Map</b> interface.
         *
         * <p><strong>Note that this implementation is not synchronized.</strong>
         * If multiple threads access a map concurrently, and at least one of the
         * threads modifies the map structurally, it <em>must</em> be synchronized
         * externally.  (A structural modification is any operation that adds or
         * deletes one or more mappings; merely changing the value associated
         * with an existing key is not a structural modification.)  This is
         * typically accomplished by synchronizing on some object that naturally
         * encapsulates the map.
         * If no such object exists, the map should be "wrapped" using the
         * <b style="color:orange;">core.util.SynchronizedMap</b>
         * class.
         *
         * <p>The iterators returned by the <b>iterator</b> method of the collections
         * returned by all of this class's "collection view methods" are
         * <em>fail-fast</em>: if the map is structurally modified at any time after
         * the iterator is created, in any way except through the iterator is own
         * <b>remove</b> method, the iterator will throw a <b style="color:orange;">
         * ConcurrentException</b>.  Thus, in the face of concurrent
         * modification, the iterator fails quickly and cleanly, rather than risking
         * arbitrary, non-deterministic behavior at an undetermined time in the future.
         *
         * <p>Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw <b>ConcurrentException</b> on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness:   <em>the fail-fast behavior of iterators
         * should be used only to detect bugs.</em>
         *
         * <p>All <b>Map.Entry</b> pairs returned by methods in this class
         * and its views represent snapshots of mappings at the time they were
         * produced. They do <strong>not</strong> support the <b>Entry.setValue</b>
         * method. (Note however that it is possible to change mappings in the
         * associated map using <b>put</b>.)
         *
         * @param K the type of keys maintained by this map
         * @param V the type of mapped values
         *
         * @author  Brunshweeck Tazeussong
         * @see Map
         * @see HashMap
         * @see Hashtable
         * @see Comparable
         * @see Comparator
         * @see Collection
         */
        template<class K, class V>
        class TreeMap : public Map<K, V> {

        };

    }
} // core

#endif //CORE23_TREEMAP_H
