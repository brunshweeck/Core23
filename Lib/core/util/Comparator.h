//
// Created by T.N.Brunshweeck on 27/11/2023.
//

#ifndef CORE23_COMPARATOR_H
#define CORE23_COMPARATOR_H

#include <core/Comparable.h>
#include <core/private/Unsafe.h>
#include <core/AssertionError.h>
#include <core/CloneNotSupportedException.h>

namespace core {
    namespace util {

        /**
         * A comparison function, which imposes a <i>total ordering</i> on
         * some collection of objects.  Comparators can be passed to a sort
         * method (such as <b style="color: orange;"> List.sort </b>) to allow precise control over the sort order.
         * Comparators can also be used to control the order of certain root
         * structures (such as <b style="color: green;"> sorted sets</b> or
         * <b style="color: green;"> sorted maps</b>), or to provide an ordering for
         * collections of objects that don't have a <b style="color: green;">
         * natural ordering</b>.<p>
         *
         * The ordering imposed by a comparator <b> c</b> on a setValue of elements
         * <b> S</b> is said to be <i>consistent with equals</i> if and only if
         * <b> c.compare(e1, e2)==0</b> has the same boolean value as
         * <b> e1.equals(e2)</b> for every <b> e1</b> and <b> e2</b> in
         * <b> S</b>.<p>
         *
         * Caution should be exercised when using a comparator capable of imposing an
         * ordering inconsistent with equals to order a sorted setValue (or sorted map).
         * Suppose a sorted setValue (or sorted map) with an explicit comparator <b> c</b>
         * is used with elements (or keys) drawn from a setValue <b> S</b>.  If the
         * ordering imposed by <b> c</b> on <b> S</b> is inconsistent with equals,
         * the sorted setValue (or sorted map) will behave "strangely."  In particular the
         * sorted set (or sorted map) will violate the general contract for setValue (or
         * map), which is defined in terms of <b> equals</b>.<p>
         *
         * For example, suppose one adds two elements <b> a</b> and <b> b</b> such that
         * <b> (a.equals(b) && c.compare(a, b) != 0)</b>
         * to an empty <b> TreeSet</b> with comparator <b> c</b>.
         * The second <b> add</b> operation will return
         * true (and the size of the tree setValue will increase) because <b> a</b> and
         * <b> b</b> are not equivalent from the tree setValue's perspective, even though
         * this is contrary to the specification of the
         * <b style="color: orange;"> Set.add</b> method.<p>
         *
         * Note: It is generally a good idea for comparators to also implement
         * <b> Serializable</b>, as they may be used as ordering methods in
         * serializable root structures (like <b style="color: orange;"> TreeSet</b>, <b style="color: orange;"> TreeMap</b>).  In
         * order for the root structure to serialize successfully, the comparator (if
         * provided) must implement <b> Serializable</b>.<p>
         *
         * For the mathematically inclined, the <i>relation</i> that defines the
         * <i>imposed ordering</i> that a given comparator <b> c</b> imposes on a
         * given setValue of objects <b> S</b> is:<pre>
         *       <b>(x, y) such that c.compare(x, y) &lt;= 0</b>.
         * </pre> The <i>quotient</i> for this total order is:<pre>
         *       <b>(x, y) such that c.compare(x, y) == 0</b>.
         * </pre>
         *
         * It follows immediately from the contract for <b> compare</b> that the
         * quotient is an <i>equivalence relation</i> on <b> S</b>, and that the
         * imposed ordering is a <i>total order</i> on <b> S</b>.  When we say that
         * the ordering imposed by <b> c</b> on <b> S</b> is <i>consistent with
         * equals</i>, we mean that the quotient for the ordering is the equivalence
         * relation defined by the objects' <b style="color: orange;"> equals(Object)</b> method(s):
         * <pre> <b>(x, y) such that x.equals(y)</b>. </pre>
         *
         * In other words, when the imposed ordering is consistent with
         * equals, the equivalence classes defined by the equivalence relation
         * of the <b> equals</b> method and the equivalence classes defined by
         * the quotient of the <b> compare</b> method are the same.

         * <p>Unlike <b> Comparable</b>, a comparator may optionally permit
         * comparison of null arguments, while maintaining the requirements for
         * an equivalence relation.
         *
         * @param T the type of objects that may be compared by this comparator
         *
         * @author  Brunshweeck Tazeussong
         * @see Comparable
         */
        template<class T>
        class Comparator : public Object {
        public:

            /**
             * Compares its two arguments for order.  Returns a negative integer,
             * zero, or a positive integer as the first argument is less than, equal
             * to, or greater than the second.<p>
             *
             * The implementor must ensure that <b style="color:orange;">
             * signum</b><b>(compare(x, y)) == -signum(compare(y, x))</b> for
             * all <b>x</b> and <b>y</b>.  (This implies that <b>
             * compare(x, y)</b> must throw an exception if and only if <b>
             * compare(y, x)</b> throws an exception.)<p>
             *
             * The implementor must also ensure that the relation is transitive:
             * <b>((compare(x, y)>0) && (compare(y, z)>0))</b> implies
             * <b>compare(x, z)>0</b>.<p>
             *
             * Finally, the implementor must ensure that <b>compare(x,
             * y)==0</b> implies that <b>signum(compare(x,
             * z))==signum(compare(y, z))</b> for all <b>z</b>.
             *
             * @apiNote
             * It is generally the case, but <i>not</i> strictly required that
             * <b>(compare(x, y)==0) == (x.equals(y))</b>.  Generally speaking,
             * any comparator that violates this condition should clearly indicate
             * this fact.  The recommended language is "Note: this comparator
             * imposes orderings that are inconsistent with equals."
             *
             * @param o1 the first object to be compared.
             * @param o2 the second object to be compared.
             * @return a negative integer, zero, or a positive integer as the
             *         first argument is less than, equal to, or greater than the
             *         second.
             * @throws ClassCastException if the arguments' types prevent them from
             *         being compared by this comparator.
             */
            virtual gint compare(const T &o1, const T &o2) const = 0;

            /**
             * Indicates whether some other object is &quot;equal to&quot;
             * this comparator.  This method must obey the general contract of
             * <b style="color:orange;">Object.equals(Object)</b>.  Additionally, this method can
             * return <b>true</b> <i>only</i> if the specified object is also
             * a comparator and it imposes the same ordering as this
             * comparator.  Thus, <b>comp1.equals(comp2)</b> implies that
             * <b style="color:orange;">signum</b><b>(comp1.compare(o1,
             * o2))==signum(comp2.compare(o1, o2))</b> for every object reference
             * <b>o1</b> and <b>o2</b>.<p>
             *
             * Note that it is <i>always</i> safe <i>not</i> to override
             * <b>Object.equals(Object)</b>.  However, overriding this method may,
             * in some cases, improve performance by allowing programs to determine
             * that two distinct comparators impose the same order.
             *
             * @param   obj   the reference object with which to compare.
             * @return  <b>true</b> only if the specified object is also
             *          a comparator and it imposes the same ordering as this
             *          comparator.
             */
            gbool equals(const Object &obj) const override {
                return Object::equals(obj);
            }

        protected:
            CORE_ALIAS(Unsafe, native::Unsafe);
            CORE_ALIAS(Comparable, Comparable<T>);

        public:

            /**
             * Returns a comparator that imposes the reverse ordering of this
             * comparator.
             *
             * @return a comparator that imposes the reverse ordering of this
             *         comparator.
             */
            virtual Comparator<T> &reverse() const {

                /**
                 * The reversed form of current comparator
                 */
                class ReversedOrder CORE_FINAL : public Comparator {
                private:
                    Comparator &c;
                public:

                    CORE_FAST ReversedOrder(Comparator &comparator) : c(comparator) {}

                    gint compare(const T &o2, const T &o1) const override { return c.compare(o1, o2); }

                    gbool equals(const Object &o) const override {
                        if (this == &o)
                            return true;
                        if (!Class<ReversedOrder>::hasInstance(o))
                            return false;
                        const ReversedOrder &ro = CORE_DYN_CAST(const ReversedOrder &, o);
                        return c == ro.c;
                    }

                    Object &clone() const override {
                        return c.reverse();
                    }

                private:
                    Comparator &base() const override {
                        return c;
                    }

                    gbool isReversed() const override {
                        return true;
                    }
                };

                if (equals(naturalOrder()))
                    return reverseOrder();
                else if (equals(reverseOrder()))
                    return naturalOrder();
                else if (isReversed()) {
                    CORE_TRY_ONLY({ return base(); })
                }
                try {
                    Comparator &clone = Unsafe::copyInstance(*this, true);
                    return Unsafe::allocateInstance<ReversedOrder>(clone);
                } catch (CloneNotSupportedException const &) {
                    return Unsafe::allocateInstance<ReversedOrder>((Comparator &) *this);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.util.Comparator"));
                }
            }

            /**
             * Returns a lexicographic-order comparator with another comparator.
             * If this <b>Comparator</b> considers two elements equal, i.e.
             * <b>compare(a, b) == 0</b>, <b>other</b> is used to determine the order.
             *
             * <p>The returned comparator is serializable if the specified comparator
             * is also serializable.
             *
             * @param  other the other comparator to be used when this comparator
             *         compares two objects that are equal.
             * @return a lexicographic-order comparator composed of this and then the
             *         other comparator
             */
            template<class X>
            Comparator &thenComparing(const Comparator<X> &other) const {
                CORE_STATIC_ASSERT(Class<X>::template isSuper<T>(), "Incompatible comparator");
                CORE_ALIAS(XComparator, Comparator<X>);

                if (lessOrder() == *this)
                    return lessOrder();

                if (greatOrder() == *this)
                    return greatOrder();

                if (naturalOrder() == *this) {
                    if (XComparator::naturalOrder() == other)
                        return naturalOrder();

                    if (XComparator::reverseOrder() == other) // (a - b) = 0 -> (b - a) = 0
                        return naturalOrder();
                }

                if (reverseOrder() == *this) {
                    if (XComparator::naturalOrder() == other)
                        return reverseOrder();

                    if (XComparator::reverseOrder() == other) // (a - b) = 0 -> (b - a) = 0
                        return reverseOrder();
                }

                if (isReversed()) {
                    CORE_TRY_ONLY
                    ({
                         if (base() == other)
                             return base();
                     })
                }

                if (other.isReversed()) {
                    CORE_TRY_ONLY
                    ({
                         if (other.base() == other)
                             return other.base();
                     })
                }

                if (XComparator::zeroOrder() == other)
                    return Unsafe::copyInstance(*this, true);

                if (Class<T>::template isSimilar<X>()) {
                    if (zeroOrder() == *this)
                        return (Comparator &) Unsafe::copyInstance(other, true);

                    if (Object::equals(*this, other))
                        return Unsafe::copyInstance(*this, true);
                }

                /**
                 * For unique comparator
                 */
                class DoubleComparator : public Comparator {
                private:
                    Comparator &c;
                    XComparator &x;

                    CORE_FRATERNITY(Comparator);

                public:
                    CORE_EXPLICIT DoubleComparator(Comparator<T> &c, XComparator &x) :
                            c(c), x(x) {}

                    Object &clone() const override {
                        return Unsafe::allocateInstance<DoubleComparator>(*this);
                    }

                    gint compare(const T &o1, const T &o2) const override {
                        gint res = c.compare(o1, o2);
                        if (res != 0) {
                            return res;
                        } else if (&o1 != &o2) {
                            X const &x1 = CORE_DYN_CAST(const X&, o1);
                            X const &x2 = CORE_DYN_CAST(const X&, o2);
                            return x.compare(x1, x2);
                        }
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o)
                            return true;
                        if (!Class<DoubleComparator>::hasInstance(o))
                            return false;
                        DoubleComparator const &mc = CORE_DYN_CAST(DoubleComparator const&, o);
                        return c == mc.c && x == mc.x;
                    }
                };

                if (Class<DoubleComparator>::hasInstance(*this)) {
                    DoubleComparator const &dc = CORE_DYN_CAST(DoubleComparator const&, *this);
                    if (dc.x == other)
                        return (Comparator &) dc;
                }

                if (Class<DoubleComparator>::hasInstance(other)) {
                    DoubleComparator const &dc = CORE_DYN_CAST(DoubleComparator const&, *this);
                    if (dc.c == *this)
                        return (Comparator &) dc;
                }

                Comparator &c = Unsafe::copyInstance(*this, true);
                XComparator &x = Unsafe::copyInstance(other, true);

                return Unsafe::allocateInstance<DoubleComparator>(c, x);
            }

            /**
             * Returns a comparator that imposes the reverse of the <em>natural
             * ordering</em>.
             *
             * <p>The returned comparator is serializable.
             *
             * @param  E the <b style="color:orange;">Comparable</b> type of element to be compared
             * @return a comparator that imposes the reverse of the <i>natural
             *         ordering</i> on <b>Comparable</b> objects.
             * @see Comparable
             */
            static Comparator &reverseOrder() CORE_NOTHROW {

                /**
                 * Compares <b style="color:orange;">Comparable</b> objects in reversed natural order.
                 *
                 * @see Comparable
                 */
                class ReverseOrder CORE_FINAL : public Comparator {
                public:
                    /**
                     * Construct new Comparator that execute the natural ordering
                     */
                    CORE_FAST ReverseOrder() = default;

                    gint compare(const T &o2, const T &o1) const override {

                        CORE_FAST gbool supportLT = Class<T>::supportLT();
                        CORE_FAST gbool isComparable = Class<Comparable>::template isSuper<T>();

                        if (isComparable) {
                            // T extends Comparable class
                            // a.compareTo(b)
                            static CORE_FAST gint
                            (*compare)(const T &, const T &) = CmpSupport<T, isComparable>::compare;
                            return compare(o1, o2);

                        } else if (supportLT) {
                            // T support relational operator less-than (<)
                            // (a < b) ? -1: (b < a ? 1: 0)
                            static CORE_FAST gbool (*isLessThan)(const T &, const T &) = LTSupport<T, supportLT>::isLT;
                            return isLessThan(o1, o2) ? -1 : isLessThan(o2, o1) ? 1 : 0;
                        } else {
                            CORE_RAISE(false, "Natural ordering is not supported", __ctrace());
                        }
                    }

                    Object &clone() const override { return reverseOrder(); }

                    Comparator &reverse() const override {
                        return naturalOrder();
                    }


                };

                /**
                 * Reverse order use one INSTANCE only
                 */
                static ReverseOrder REVERSE_ORDER = {};

                return REVERSE_ORDER;
            }

            /**
             * Returns a comparator that compares <b style="color:orange;">Comparable</b> objects in natural
             * order.
             *
             * <p>The returned comparator is serializable.
             *
             * @param  E the <b style="color:orange;">Comparable</b> type of element to be compared
             * @return a comparator that imposes the <i>natural ordering</i> on <b>
             *         Comparable</b> objects.
             * @see Comparable
             */
            static Comparator &naturalOrder() {

                /**
                 * Compares <b style="color:orange;">Comparable</b> objects in natural order.
                 *
                 * @see Comparable
                 */
                class NaturalOrder CORE_FINAL : public Comparator {
                public:
                    /**
                     * Construct new Comparator that execute the natural ordering
                     */
                    CORE_FAST NaturalOrder() = default;

                    gint compare(const T &o1, const T &o2) const override {

                        CORE_FAST gbool supportLT = Class<T>::supportLT();
                        CORE_FAST gbool isComparable = Class<Comparable>::template isSuper<T>();

                        if (isComparable) {
                            // T extends Comparable class
                            // a.compareTo(b)
                            CORE_ALIAS(Signature, gint(*)(const T&, const T&));
                            Signature compare = CmpSupport<T, isComparable>::compare;
                            return compare(o1, o2);
                        } else if (supportLT) {
                            // T support relational operator less-than (<)
                            // a < b? -1: b < a ? 1 : 0
                            CORE_ALIAS(Signature, gbool(*)(const T&, const T&));
                            Signature isLessThan = LTSupport<T, supportLT>::isLT;
                            return isLessThan(o1, o2) ? -1 : isLessThan(o2, o1) ? 1 : 0;
                        }
                        CORE_RAISE(isComparable || supportLT, "Natural ordering is not supported"_S, __ctrace())
                    }

                    Object &clone() const override { return naturalOrder(); }

                    Comparator &reverse() const override {
                        return reverseOrder();
                    }
                };

                static NaturalOrder NATURAL_ORDER = {};

                return NATURAL_ORDER;
            }

            /**
             * Returns a comparator that not provide order (always equals).
             *
             * <p>The returned comparator is serializable.
             * @return a comparator that not provide order.
             * @see Comparable
             */
            static Comparator &zeroOrder() {

                // zero comparator (always return zero)
                class ZeroOrder CORE_FINAL : public Comparator<T> {
                public:
                    CORE_FAST ZeroOrder() = default;

                    Object &clone() const override { return (Object &) *this; }

                    gint compare(const T &o1, const T &o2) const override {
                        CORE_IGNORE(o1);
                        CORE_IGNORE(o2);
                        return 0;
                    }

                    gbool equals(const Object &o) const override {
                        return this == &o || Class<ZeroOrder>::hasInstance(o);
                    }

                };

                static ZeroOrder ZERO_ORDER = {};

                return ZERO_ORDER;
            }

            /**
             * Returns a comparator that not provide order (always less).
             *
             * <p>The returned comparator is serializable.
             * @return a comparator that not provide order.
             * @see Comparable
             */
            static Comparator &lessOrder() {


                // less comparator (always return -1)
                class LessOrder CORE_FINAL : public Comparator<T> {
                public:
                    CORE_FAST LessOrder() = default;

                    Object &clone() const override { return (Object &) *this; }

                    gint compare(const T &o1, const T &o2) const override {
                        CORE_IGNORE(o1);
                        CORE_IGNORE(o2);
                        return -1;
                    }

                    gbool equals(const Object &o) const override {
                        return this == &o || Class<LessOrder>::hasInstance(o);
                    }

                };

                static LessOrder LESS_ORDER = {};

                return LESS_ORDER;
            }

            /**
             * Returns a comparator that not provide order (always less).
             *
             * <p>The returned comparator is serializable.
             * @return a comparator that not provide order.
             * @see Comparable
             */
            static Comparator &greatOrder() {

                // great comparator (always return 1)
                class GreatOrder CORE_FINAL : public Comparator<T> {
                public:
                    CORE_FAST GreatOrder() = default;

                    Object &clone() const override { return (Object &) *this; }

                    gint compare(const T &o1, const T &o2) const override {
                        CORE_IGNORE(o1);
                        CORE_IGNORE(o2);
                        return 1;
                    }

                    gbool equals(const Object &o) const override {
                        return this == &o || Class<GreatOrder>::hasInstance(o);
                    }

                };

                static GreatOrder GREAT_ORDER = {};

                return GREAT_ORDER;
            }

            /**
             * Convert comparator of other types to comparator of this type.
             *
             * @param c the other comparator
             */
            template<class X>
            static Comparator &of(const Comparator<X> &c) {
                CORE_STATIC_ASSERT(Class<X>::template isSuper<T>(), "Incompatible Comparator");
                CORE_ALIAS(XComparator, Comparator<X>);

                if (XComparator::naturalOrder() == c)
                    return naturalOrder();

                else if (XComparator::reverseOrder() == c)
                    return reverseOrder();

                else if (XComparator::zeroOrder() == c)
                    return zeroOrder();

                else if (XComparator::lessOrder() == c)
                    return lessOrder();

                else if (XComparator::greatOrder() == c)
                    return greatOrder();

                if (Class<T>::template isSimilar<X>())
                    return Unsafe::copyInstance((const Comparator &) c, true);

                else {
                    CORE_TRY_ONLY_EX
                    ({
                         if (c.isReversed())
                             return Comparator::of(c.base()).reverse();
                     })

                }

                return zeroOrder().thenComparing(c);
            }

        private:
            /**
             * Return the comparator used by this comparator in internal.
             *
             * @throws UnsupportedMethodException If the value returned by
             *                                    Comparator.isReversed is false.
             */
            virtual Comparator &base() const {
                UnsupportedOperationException().throws(__trace("core.util.Comparator"));
            }

            /**
             * Return true if this comparator has internal comparator and
             * is reversed form of this internal comparator.
             * (if is true, for retrieve internal comparator use <b>Comparator.base</b> method)
             */
            virtual gbool isReversed() const { return false; };

            template<class X, gbool supportLT>
            class LTSupport CORE_FINAL {
            public:
                static CORE_FAST gbool isLT(const X &x, const X &y) { return x < y; }
            };

            template<class X>
            class LTSupport<X, false> CORE_FINAL {
            public:
                static gbool isLT(const X &x, const X &y) {
                    CORE_IGNORE(x);
                    CORE_IGNORE(y);
                    return false;
                }
            };

            template<class X, gbool isComparable>
            class CmpSupport CORE_FINAL {
            public:
                static gint compare(const X &x, const X &y) { return x.compareTo(y); }
            };

            template<class X>
            class CmpSupport<X, false> CORE_FINAL {
            public:
                static gint compare(const X &x, const X &y) {
                    CORE_IGNORE(x);
                    CORE_IGNORE(y);
                    return 0;
                }
            };

        public:
            /**
             * Return shadow copy of this comparator.
             * Note: all comparator must support this method.
             */
            Object &clone() const override = 0;
        };

    } // core
} // util

#endif //CORE23_COMPARATOR_H
