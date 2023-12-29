//
// Created by T.N.Brunshweeck on 27/11/2023.
//

#ifndef CORE23_COMPARATOR_H
#define CORE23_COMPARATOR_H

#include <core/Comparable.h>
#include <core/private/Unsafe.h>
#include <core/CloneNotSupportedException.h>
#include <core/AssertionError.h>

namespace core {
    namespace util {

        /**
         * A comparison function, which imposes a <i>total ordering</i> on
         * some collection of objects.  Comparators can be passed to a sort
         * method (such as <b style="color: orange;"> List.sort </b>) to allow precise control over the sort order.
         * Comparators can also be used to control the order of certain array
         * structures (such as <b style="color: green;"> sorted sets</b> or
         * <b style="color: green;"> sorted maps</b>), or to provide an ordering for
         * collections of objects that don't have a <b style="color: green;"> Comparable
         * natural ordering</b>.<p>
         *
         * The ordering imposed by a comparator <b> c</b> on a set of elements
         * <b> S</b> is said to be <i>consistent with equals</i> if and only if
         * <b> c.compare(e1, e2)==0</b> has the same boolean value as
         * <b> e1.equals(e2)</b> for every <b> e1</b> and <b> e2</b> in
         * <b> S</b>.<p>
         *
         * Caution should be exercised when using a comparator capable of imposing an
         * ordering inconsistent with equals to order a sorted set (or sorted map).
         * Suppose a sorted set (or sorted map) with an explicit comparator <b> c</b>
         * is used with elements (or keys) drawn from a set <b> S</b>.  If the
         * ordering imposed by <b> c</b> on <b> S</b> is inconsistent with equals,
         * the sorted set (or sorted map) will behave "strangely."  In particular the
         * sorted set (or sorted map) will violate the general contract for set (or
         * map), which is defined in terms of <b> equals</b>.<p>
         *
         * For example, suppose one adds two elements <b> a</b> and <b> b</b> such that
         * <b> (a.equals(b) && c.compare(a, b) != 0)</b>
         * to an empty <b> TreeSet</b> with comparator <b> c</b>.
         * The second <b> add</b> operation will return
         * true (and the size of the tree set will increase) because <b> a</b> and
         * <b> b</b> are not equivalent from the tree set's perspective, even though
         * this is contrary to the specification of the
         * <b style="color: orange;"> Set.add</b> method.<p>
         *
         * Note: It is generally a good idea for comparators to also implement
         * <b> Serializable</b>, as they may be used as ordering methods in
         * serializable array structures (like <b style="color: orange;"> TreeSet</b>, <b style="color: orange;"> TreeMap</b>).  In
         * order for the array structure to serialize successfully, the comparator (if
         * provided) must implement <b> Serializable</b>.<p>
         *
         * For the mathematically inclined, the <i>relation</i> that defines the
         * <i>imposed ordering</i> that a given comparator <b> c</b> imposes on a
         * given set of objects <b> S</b> is:<pre>
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
        interface Comparator : public Object {

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
             * @throws CastException if the arguments' types prevent them from
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
            gbool equals(const Object &o) const override = 0;

        private:

            template<class E>
            CORE_ALIAS(Capture, typename Class<T>::template Iff<Class<E>::template isSuper<T>()>);

            CORE_ALIAS(U, native::Unsafe);

        public:

            /**
             * Returns a comparator that imposes the reverse ordering of this
             * comparator.
             *
             * @return a comparator that imposes the reverse ordering of this
             *         comparator.
             */
            virtual const Comparator<T> &reverse() const {
                if (Class<NaturalOrderComparator<>>::hasInstance(*this))
                    return reverseOrder();
                else if (Class<ReverseComparator<>>::hasInstance(*this))
                    return naturalOrder();
                else if (Class<ReverseComparator2<>>::hasInstance(*this))
                    return (Comparator &) CORE_DYN_CAST(const ReverseComparator2<>&, *this).cmp;
                else
                    return U::createInstance<ReverseComparator2<>>(U::copyInstance(*this, true));
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
            template<class E = T>
            Comparator<T> &thenComparing(const Comparator<Capture<E>> &other) const {
                class MultiComparator : public Comparator<T> {
                private:
                    Comparator<T> &root;
                    Comparator<E> &other;

                public:
                    CORE_EXPLICIT MultiComparator(Comparator<T> &root, Comparator<E> &other) : root(root),
                                                                                               other(other) {}

                    Object &clone() const override {
                        return U::createInstance<MultiComparator>(*this);
                    }

                    gint compare(const T &o1, const T &o2) const override {
                        gint res = root.compare(o1, o2);
                        return res != 0 ? res :
                               other.compare(CORE_DYN_CAST(const E&, o1), CORE_DYN_CAST(const E&, o2));
                    }

                    gbool equals(const Object &o) const override {
                        if (!Class<MultiComparator>::hasInstance(o))
                            return false;
                        MultiComparator &cmp = CORE_CAST(MultiComparator &, o);
                        return root.equals(cmp.root) && other.equals(cmp.other);
                    }
                };

                return U::createInstance<MultiComparator>(U::copyInstance(*this, true),
                                                          U::copyInstance(other, true));
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
                 * Reverse order use one instance only
                 */
                static ReverseComparator<> REVERSE_ORDER = {};

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
            static Comparator &naturalOrder() CORE_NOTHROW {
                /**
                 * Natural order use one instance only
                 */
                static NaturalOrderComparator<> NATURAL_ORDER = {};

                return NATURAL_ORDER;
            }

            /**
             * Convert comparator of other types to comparator of this type.
             *
             * @param c the other comparator
             */
            template<class E = T>
            static Comparator &copyOf(const Comparator<Capture<E>> &c) {
                if (Class<T>::template isSimilar<E>())
                    return U::copyInstance(CORE_DYN_CAST(const Comparator&, c), true);

                if (Class<typename Comparator<E>::NaturalOrderComparator>::hasInstance(c))
                    return naturalOrder();

                if (Class<typename Comparator<E>::ReverseComparator>::hasInstance(c))
                    return reverseOrder();

                if (Class<typename Comparator<E>::ReverseComparator2>::hasInstance(c))
                    return reverseOrder();

                // zero comparator (always return zero)
                class FalseComparator CORE_FINAL : public Comparator<T> {
                public:
                    CORE_FAST FalseComparator() = default;

                    Object &clone() const override { return (Object &) *this; }

                    gint compare(const T & /*o1*/, const T & /*o2*/) const override { return 0; }

                    gbool equals(const Object &o) const override { return Class<FalseComparator>::hasInstance(o); }

                };

                static FalseComparator INSTANCE = {};

                return INSTANCE.thenComparing(c);
            }

        private:

            /**
             * Compares <b style="color:orange;">Comparable</b> objects in natural order.
             *
             * @see Comparable
             */
            template<class E = T>
            class NaturalOrderComparator CORE_FINAL : public Comparator<E> {
            public:
                /**
                 * Construct new Comparator that execute the natural ordering
                 */
                CORE_FAST NaturalOrderComparator() = default;

                gint compare(const T &o1, const T &o2) const override {
                    CORE_FAST gbool supportLT = Class<T>::supportLT();
                    CORE_FAST gbool isComparable = Class<Comparable<T>>::template isSuper<T>();
                    if (isComparable) {
                        // T extends Comparable class
                        // a.compareTo(b)
                        static CORE_FAST gint (*compare)(const T &, const T &) = CmpSupport<T, isComparable>::compare;
                        return compare(o1, o2);
                    } else if (supportLT) {
                        // T support relational operator less-than (<)
                        // a < b ? -1 : b < a ? 1 : 0
                        static CORE_FAST gbool (*isLessThan)(const T &, const T &) = LTSupport<T, supportLT>::isLT;
                        return isLessThan(o1, o2) ? -1 : isLessThan(o2, o1) ? 1 : 0;
                    } else {
                        // error
                        AssertionError(CORE_DYN_CAST(const Object&, o1).classname() + " not support natural ordering")
                                .throws(__trace("core.util.NaturalOrderComparator"));
                    }
                }

                gbool equals(const Object &o) const override { return Class<NaturalOrderComparator>::hasInstance(o); }

                Object &clone() const override { return naturalOrder(); }
            };

            /**
             * //
             */
            template<class E = T>
            class ReverseComparator CORE_FINAL : public Comparator<E> {
            public:
                /**
                 * Construct new Comparator that execute the natural ordering
                 */
                CORE_FAST ReverseComparator() = default;

                gint compare(const T &o2, const T &o1) const override {
                    CORE_FAST gbool supportLT = Class<T>::supportLT();
                    CORE_FAST gbool isComparable = Class<Comparable<T>>::template isSuper<T>();
                    if (isComparable) {
                        // T extends Comparable class
                        // a.compareTo(b)
                        static CORE_FAST gint (*compare)(const T &, const T &) = CmpSupport<T, isComparable>::compare;
                        return compare(o1, o2);
                    } else if (supportLT) {
                        // T support relational operator less-than (<)
                        // a < b ? -1 : b < a ? 1 : 0
                        static CORE_FAST gbool (*isLessThan)(const T &, const T &) = LTSupport<T, supportLT>::isLT;
                        return isLessThan(o1, o2) ? -1 : isLessThan(o2, o1) ? 1 : 0;
                    } else {
                        // error
                        AssertionError(CORE_DYN_CAST(const Object&, o1).classname() + " not support natural ordering")
                                .throws(__trace("core.util.ReverseComparator"));
                    }
                }

                gbool equals(const Object &o) const override { return Class<ReverseComparator>::hasInstance(o); }

                Object &clone() const override { return reverseOrder(); }

            };

            /**
             * //
             */
            template<class E = T>
            class ReverseComparator2 CORE_FINAL : public Comparator<E> {
            public:
                Comparator<E> &cmp;

                CORE_FAST ReverseComparator2(const Comparator<E> &cmp) : cmp(U::copyInstance(cmp, true)) {}

                gint compare(const T &o2, const T &o1) const override { return cmp.compare(o2, o1); }

                gbool equals(const Object &o) const override {
                    return Class<ReverseComparator2>::hasInstance(o) &&
                           cmp == CORE_DYN_CAST(const ReverseComparator2&, o).cmp;
                }

                Object &clone() const override { return U::createInstance<ReverseComparator2>(cmp); }
            };

            template<class C, gbool supportLT>
            class LTSupport CORE_FINAL {
            public:
                static CORE_FAST gbool isLT(const C &c1, const C &c2) { return c1 < c2; }
            };

            template<class C>
            class LTSupport<C, false> CORE_FINAL {
            public:
                static CORE_FAST gbool isLT(const C & /*c1*/, const C & /*c2*/) { return false; }
            };

            template<class C, gbool isComparable>
            class CmpSupport CORE_FINAL {
            public:
                static gint compare(const C &c1, const C &c2) { return c1.compareTo(c2); }
            };

            template<class C>
            class CmpSupport<C, false> CORE_FINAL {
            public:
                static gint compare(const C &  /*c1*/, const C &  /*c2*/) { return 0; }
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
