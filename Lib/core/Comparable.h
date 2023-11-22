//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_COMPARABLE_H
#define CORE23_COMPARABLE_H

#include <core/Object.h>

namespace core {

    /**
     * This interface imposes a total ordering on the objects of each class that
     * implements it.  This ordering is referred to as the class's <i>natural
     * ordering</i>, and the class's <b> compareTo</b> method is referred to as
     * its <i>natural comparison method</i>.<p>
     *
     * Lists (and arrays) of objects that implement this interface can be sorted
     * automatically by <b style="color: orange"> Collections.sort</b> (and
     * <b style="color: orange"> Arrays.sort</b>).  Objects that implement this
     * interface can be used as keys in a <b style="color: green"> sorted map</b> or as
     * elements in a <b style="color: green"> sorted set</b>, without the need to
     * specify a <b style="color: green"> comparator</b>.<p>
     *
     * The natural ordering for a class <b> C</b> is said to be <i>consistent
     * with equals</i> if and only if <b> e1.compareTo(e2) == 0</b> has
     * the same boolean value as <b> e1.equals(e2)</b> for every
     * <b> e1</b> and <b> e2</b> of class <b> C</b>.
     *
     * <p>
     * It is strongly recommended (though not required) that natural orderings be
     * consistent with equals.  This is so because sorted sets (and sorted maps)
     * without explicit comparators behave "strangely" when they are used with
     * elements (or keys) whose natural ordering is inconsistent with equals.  In
     * particular, such a sorted set (or sorted map) violates the general contract
     * for set (or map), which is defined in terms of the <b> equals</b>
     * method.<p>
     *
     * For example, if one adds two keys <b> a</b> and <b> b</b> such that
     * <b> (!a.equals(b) && a.compareTo(b) == 0)</b> to a sorted
     * set that does not use an explicit comparator, the second <b> add</b>
     * operation returns false (and the size of the sorted set does not increase)
     * because <b> a</b> and <b> b</b> are equivalent from the sorted set's
     * perspective.<p>
     *
     * Virtually all Java core classes that implement <b> Comparable</b>
     * have natural orderings that are consistent with equals.  One
     * exception is <b style="color: orange"> BigDecimal</b>, whose <b style="color: green">
     * natural ordering</b> equates <b>
     * BigDecimal</b> objects with equal numerical values and different
     * representations (such as 4.0 and 4.00). For <b style="color: orange">
     * BigDecimal.equals()</b> to return true,
     * the representation and numerical value of the two <b>
     * BigDecimal</b> objects must be the same.<p>
     *
     * For the mathematically inclined, the <i>relation</i> that defines
     * the natural ordering on a given class C is:<pre><b>
     *       {(x, y) such that x.compareTo(y) <= 0}.
     * </b></pre> The <i>quotient</i> for this total order is: <pre><b>
     *       {(x, y) such that x.compareTo(y) == 0}.
     * </b></pre>
     *
     * It follows immediately from the contract for <b> compareTo</b> that the
     * quotient is an <i>equivalence relation</i> on <b> C</b>, and that the
     * natural ordering is a <i>total order</i> on <b> C</b>.  When we say that a
     * class's natural ordering is <i>consistent with equals</i>, we mean that the
     * quotient for the natural ordering is the equivalence relation defined by
     * the class's <b style="color: orange"> equals(Object)</b> method:<pre>
     *     {(x, y) such that x.equals(y)}. </pre><p>
     *
     * In other words, when a class's natural ordering is consistent with
     * equals, the equivalence classes defined by the equivalence relation
     * of the <b> equals</b> method and the equivalence classes defined by
     * the quotient of the <b> compareTo</b> method are the same.
     *
     * @param T the type of objects that this object may be compared to
     *
     * @author  Brunshweeck Tazeussong
     * @see Comparator
     */
    template<class T>
    interface Comparable : Object {

        /**
         * Compares this object with the specified object for order.  Returns a
         * negative integer, zero, or a positive integer as this object is less
         * than, equal to, or greater than the specified object.
         *
         * <p>The implementor must ensure <b style="color: orange"> signum</b>
         * <b> (x.compareTo(y)) == -signum(y.compareTo(x))</b> for
         * all <b> x</b> and <b> y</b>.  (This implies that <b>
         * x.compareTo(y)</b> must throw an exception if and only if <b>
         * y.compareTo(x)</b> throws an exception.)
         *
         * <p>The implementor must also ensure that the relation is transitive:
         * <b> (x.compareTo(y) > 0 && y.compareTo(z) > 0)</b> implies
         * <b> x.compareTo(z) > 0</b>.
         *
         * <p>Finally, the implementor must ensure that <b>
         * x.compareTo(y)==0</b> implies that <b> signum(x.compareTo(z))
         * == signum(y.compareTo(z))</b>, for all <b> z</b>.
         *
         * @warning
         * It is strongly recommended, but <i>not</i> strictly required that
         * <b> (x.compareTo(y)==0) == (x.equals(y))</b>.  Generally speaking, any
         * class that implements the <b> Comparable</b> interface and violates
         * this condition should clearly indicate this fact.  The recommended
         * language is "Note: this class has a natural ordering that is
         * inconsistent with equals."
         *
         * @param   o the object to be compared.
         * @return  a negative integer, zero, or a positive integer as this object
         *          is less than, equal to, or greater than the specified object.
         *
         * @throws CastException if the specified object's type prevents it
         *         from being compared to this object.
         */
        virtual gint compareTo(const T &o) const = 0;
    };

} // core

#endif //CORE23_COMPARABLE_H
