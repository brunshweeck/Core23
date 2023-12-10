//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_PREDICATE_H
#define CORE23_PREDICATE_H

#include <core/util/function/Functional.h>
#include <core/ArgumentException.h>

namespace core {
    namespace util {

        /**
         * Represents a predicate (boolean-valued function) of one argument.
         *
         * <p>This is a <a href="">functional interface</a>
         * whose functional method is <b style="color: orange;"> test(Object)</b>.
         *
         * @param T the type of the input to the predicate
         */
        template<class T>
        class Predicate : public Functional {
        private:
            CORE_STATIC_ASSERT(Class<Object>::isSuper<T>(), "Illegal template type");
            CORE_STATIC_ASSERT(!Class<Void>::isSimilar<T>(), "Illegal parameter type");

            CORE_ALIAS(Param, Params < T >);

            interface Launcher : public Object {
                virtual gbool test(Param) const = 0;

                template<class Fn, Class<gbool>::Iff<Class<Fn>::isFunction() && !Class<Fn>::isClass()> = 1>
                static Launcher &of(Fn &&fn) {

                    class _$ : public Launcher {
                    private:
                        Fn fn;

                    public:
                        CORE_EXPLICIT _$(Fn &&fn) : fn(fn) {}

                        gbool equals(const Object &o) const {
                            return Class<_$>::hasInstance(o) && CORE_CAST(_$ &, o).fn == fn;
                        }

                        Object &clone() const { return U::createInstance<_$>(*this); }

                        gbool test(Param p) const { return fn(p); }
                    };

                    return U::createInstance<_$>(U::forwardInstance<Fn>(fn));
                }

                template<class Callable, Class<gbool>::Iff<
                        !Class<Callable>::isFunction() || Class<Callable>::isClass()> = 1>
                static Launcher &of(Callable &&c) {

                    CORE_ALIAS(_R, Class<Callable>::template Return<Param>);
                    CORE_ALIAS(_Fn, UnarySign<Callable,, T, _R >);
                    CORE_ALIAS(Fn, UnarySign<_Fn,, Param, _R >);

                    if (!Class<Fn>::template isSimilar<Callable>()) {
                        // simple lambda functions
                        return of(CORE_CAST(Fn, U::forwardInstance<Callable>(c)));
                    }

                    class _$ : public Launcher {
                    private:
                        Callable c;

                    public:
                        CORE_EXPLICIT _$(Callable &&c) : c(U::forwardInstance<Callable>(c)) {}

                        gbool equals(const Object &o) const {
                            return Class<_$>::hasInstance(o) && &CORE_CAST(_$ &, o).c == &c;
                        }

                        Object &clone() const { return U::createInstance<_$>(*this); }

                        gbool test(Param p) const { return c(p); }
                    };

                    return U::createInstance<_$>(U::forwardInstance<Callable>(c));
                }
            };

            CORE_ALIAS(Tester, typename Class<Launcher>::Ptr);

            /**
             * The predicate tester
             */
            Tester tester;

        public:
            /**
             * Construct new predicate with callable object (class instances or lambda functions)
             *
             * @param c The callable object
             */
            template<class Callable, Class<gbool>::Iff<Class<Callable>::isFunction() || Class<Callable>::isClass()> = 1>
            CORE_IMPLICIT Predicate(Callable &&c) CORE_NOTHROW : tester(0) {
                CORE_STATIC_ASSERT(Class<Callable>::template isCallable<Param>(), "Invalid callable object");
                CORE_ALIAS(R, typename Class<Callable>::template Return<Param>);
                CORE_STATIC_ASSERT(Class<R>::template isConvertible<gbool>(), "Predicate require boolean callable");
                tester = &Launcher::of(U::forwardInstance<Callable>(c));
            }

            /**
             * Construct new predicate with another
             *
             * @param c The other predicate
             */
            Predicate(const Predicate<T> &c) : tester(0) {
                tester = &U::copyInstance(*c.tester);
            }

            /**
             * Construct new predicate with another
             *
             * @param c The other predicate
             */
            Predicate(Predicate<T> &&c) CORE_NOTHROW: tester(0) {
                tester = c.tester;
                c.tester = null;
            }

            /**
             * Set predicate tester  with another predicate tester
             *
             * @param c The predicate that it tester will be copied to set this predicate tester
             */
            Predicate<T> &operator=(const Predicate<T> &c) {
                if (this != &c) {
                    Tester tester0 = &U::copyInstance(*c.tester);
                    U::destroyInstance(*tester);
                    tester = tester0;
                }
                return *this;
            }

            /**
             * Swap predicate tester  with another predicate tester
             *
             * @param c The predicate that it tester will be swapped to set this predicate tester
             */
            Predicate<T> &operator=(Predicate<T> &&c) CORE_NOTHROW {
                Tester tester0 = tester;
                tester = c.tester;
                c.tester = tester0;
                return *this;
            }

            /**
             * Evaluates this predicate on the given argument.
             *
             * @param p the input argument
             *
             * @return <b>true</b> if the input argument matches the predicate,
             * otherwise <b>false</b>
             */
            inline gbool test(Param p) const { return (*tester).test(p); }

            /**
             * Test if this predicate has same tester with the specified
             * predicate.
             *
             * @param o The other predicate
             */
            gbool equals(const Object &o) const override {
                if (!Class<Predicate>::hasInstance(o))
                    return false;
                return (*tester).equals(*CORE_CAST(const Predicate<T> &, o).tester);
            }

            /**
             * Return shadow copy of this predicate
             */
            Object &clone() const override {
                return U::createInstance<Predicate<T>>(*this);
            }

            /**
             * Returns a composed predicate that represents a short-circuiting logical
             * AND of this predicate and another.  When evaluating the composed
             * predicate, if this predicate is <b>false</b>, then the <b>other</b>
             * predicate is not evaluated.
             *
             * <p>Any exceptions thrown during evaluation of either predicate are relayed
             * to the caller; if evaluation of this predicate throws an exception, the
             * <b>other</b> predicate will not be evaluated.
             *
             * @param other a predicate that will be logically-ANDed with this
             *              predicate
             *
             * @return a composed predicate that represents the short-circuiting logical
             * AND of this predicate and the <b>other</b> predicate
             */
            Predicate<T> logicalAnd(const Predicate<T> &other) {
                return [other, *this](Param p) { return test(p) && other.test(p); };
            }

            /**
             * Returns a composed predicate that represents a short-circuiting logical
             * AND of this predicate and another.  When evaluating the composed
             * predicate, if this predicate is <b>false</b>, then the <b>other</b>
             * predicate is not evaluated.
             *
             * <p>Any exceptions thrown during evaluation of either predicate are relayed
             * to the caller; if evaluation of this predicate throws an exception, the
             * <b>other</b> predicate will not be evaluated.
             *
             * @param other a predicate that will be logically-ANDed with this
             *              predicate
             *
             * @return a composed predicate that represents the short-circuiting logical
             * AND of this predicate and the <b>other</b> predicate
             */
            template<class E,
                    Class<gbool>::Iff<Class<E>::template isSuper<T>() && !Class<E>::template isSimilar<T>()> = 1>
            Predicate<T> logicalAnd(const Predicate<E> &other) {
                return [other, *this](Param p) { return test(p) && other.test(p); };
            }

            /**
             * Returns a composed predicate that represents a short-circuiting logical
             * OR of this predicate and another.  When evaluating the composed
             * predicate, if this predicate is <b>true</b>, then the <b>other</b>
             * predicate is not evaluated.
             *
             * <p>Any exceptions thrown during evaluation of either predicate are relayed
             * to the caller; if evaluation of this predicate throws an exception, the
             * <b>other</b> predicate will not be evaluated.
             *
             * @param other a predicate that will be logically-ORed with this
             *              predicate
             *
             * @return a composed predicate that represents the short-circuiting logical
             * OR of this predicate and the <b>other</b> predicate
             */
            Predicate<T> logicalOr(const Predicate<T> &other) {
                return [other, *this](Param p) { return test(p) || other.test(p); };
            }

            /**
             * Returns a composed predicate that represents a short-circuiting logical
             * OR of this predicate and another.  When evaluating the composed
             * predicate, if this predicate is <b>true</b>, then the <b>other</b>
             * predicate is not evaluated.
             *
             * <p>Any exceptions thrown during evaluation of either predicate are relayed
             * to the caller; if evaluation of this predicate throws an exception, the
             * <b>other</b> predicate will not be evaluated.
             *
             * @param other a predicate that will be logically-ORed with this
             *              predicate
             *
             * @return a composed predicate that represents the short-circuiting logical
             * OR of this predicate and the <b>other</b> predicate
             */
            template<class E,
                    Class<gbool>::Iff<Class<E>::template isSuper<T>() && !Class<E>::template isSimilar<T>()> = 1>
            Predicate<T> logicalOr(const Predicate<E> &other) {
                return [other, *this](Param p) { return test(p) || other.test(p); };
            }

            /**
             * Returns a predicate that represents the logical negation of this
             * predicate.
             *
             * @return a predicate that represents the logical negation of this
             * predicate
             */
            Predicate<T> negate() {
                return [*this](Param x) { return !test(x); };
            }

            /**
             * Returns a predicate that tests if two arguments are equal according
             * to <b style="color:orange;">Object.equals(Object, Object)</b>.
             *
             * @param T the type of arguments to the predicate
             * @param targetRef the object reference with which to compare for equality,
             *               which may be <b>null</b>
             * @return a predicate that tests if two arguments are equal according
             * to <b style="color:orange;">Objects.equals(Object, Object)</b>
             */
            static Predicate<T> isEqual(const Object &targetRef) {
                return [&targetRef](Param p) { return targetRef.equals(p); };
            }

            /**
             * Destroy this predicate
             */
            ~Predicate() override {
                U::destroyInstance(*tester);
                tester = null;
            }
        };

#if CORE_TEMPLATE_TYPE_DEDUCTION
        Predicate() -> Predicate<Object>;
        template<class Return, class Param, class ...Params>
        Predicate(Return(Param, Params...)) -> Predicate<typename Class<Param>::Object>;
#endif
    }

} // core

#endif //CORE23_PREDICATE_H
