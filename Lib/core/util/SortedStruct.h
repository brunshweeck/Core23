//
// Created by T.N.Brunshweeck on 11/12/2023.
//

#ifndef CORE23_SORTEDSTRUCT_H
#define CORE23_SORTEDSTRUCT_H

#include <core/util/Comparator.h>

namespace core {
    namespace util {

        /**
         * The <b> SortedStruct</b> interface represent all container class that
         * element are ordered by one comparator or naturally, at example
         * of TreeSet and TreeMap or PriorityQueue. The class deriving form
         * this class must give guarantee that it iterator return element with
         * same order like it.
         *
         * @tparam T The type of element use for sorting
         *
         * @author Brunshweeck Tazeussong
         *
         * @see core.Comparable
         * @see core.util.Comparator
         * @see core.util.TreeSet
         * @see core.util.TreeMap
         * @see core.util.PriorityQueue
         */
        template <class T>
        interface SortedStruct /* $property */ {

            /**
             * Return the comparator used to sort element of this structure.
             *
             * @return The comparator used to sort element of this structure.
             */
            virtual const Comparator<T> &comparator() const = 0;
        };

    } // util
} // core

#endif //CORE23_SORTEDSTRUCT_H
