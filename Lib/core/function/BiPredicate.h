//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_BIPREDICATE_H
#define CORE23_BIPREDICATE_H

#include <core/function/Predicate.h>

namespace core {
    namespace function {

        /**
         * Represents a predicate (boolean-valued function) of two arguments.  This is
         * the two-arity specialization of <b style="color: orange;"> Predicate</b>.
         *
         * <p>This is a <a href="package-summary.html">functional interface</a>
         * whose functional method is <b style="color: orange;"> BiPredicate.test(Object, Object)</b>.
         *
         * @param <T> the type of the first argument to the predicate
         * @param <U> the type of the second argument the predicate
         *
         * @see Predicate
         */
        template<class T, class U>
        class BiPredicate : public Functional {
        private:
            CORE_STATIC_ASSERT(Class<Object>::isSuper<T>() && Class<Object>::isSuper<U>(),
                               "First and second argument type must have core.Object as base class.");
            CORE_STATIC_ASSERT(!(Class<T>::isConstant() || Class<U>::isConstant()),
                               "First and second argument type mustn't have <const> as qualifier");
            CORE_STATIC_ASSERT(!(Class<T>::isVolatile() || Class<U>::isVolatile()),
                               "First and second argument type mustn't have <volatile> as qualifier");

            CORE_ALIAS(X, Functional::Params<T>);
            CORE_ALIAS(Y, Functional::Params<U>);

        public:

            /**
             * Evaluates this predicate on the given arguments.
             *
             * @param t the first input argument
             * @param u the second input argument
             * @return <b> true</b> if the input arguments match the predicate,
             * otherwise <b> false</b>
             */
            virtual gbool test(X t, Y u) const = 0;

            /**
             * Returns a composed predicate that represents a short-circuiting logical
             * AND of this predicate and another.  When evaluating the composed
             * predicate, if this predicate is <b> false</b>, then the <b> other</b>
             * predicate is not evaluated.
             *
             * <p>Any exceptions thrown during evaluation of either predicate are relayed
             * to the caller; if evaluation of this predicate throws an exception, the
             * <b> other</b> predicate will not be evaluated.
             *
             * @param other a predicate that will be logically-ANDed with this
             *              predicate
             * @return a composed predicate that represents the short-circuiting logical
             * AND of this predicate and the <b> other</b> predicate
             */
            template<class $1, class $2>
            BiPredicate &logicalAnd(const BiPredicate<$1, $2> &other) const {
                CORE_STATIC_ASSERT(Class<T>::template isSuper<$1>() && Class<U>::template isSuper<$2>(),
                                   "Could not resolve given predicate");
                return from([&](X t, Y u) -> gbool { return test(t, u) && other.test(t, u); });
            }

            /**
             * Returns a predicate that represents the logical negation of this
             * predicate.
             *
             * @return a predicate that represents the logical negation of this
             * predicate
             */
            BiPredicate &negate() const {
                return from([&](X t, Y u) -> gbool { return !test(t, u); });
            }

            /**
             * Returns a composed predicate that represents a short-circuiting logical
             * OR of this predicate and another.  When evaluating the composed
             * predicate, if this predicate is <b> true</b>, then the <b> other</b>
             * predicate is not evaluated.
             *
             * <p>Any exceptions thrown during evaluation of either predicate are relayed
             * to the caller; if evaluation of this predicate throws an exception, the
             * <b> other</b> predicate will not be evaluated.
             *
             * @param other a predicate that will be logically-ORed with this
             *              predicate
             * @return a composed predicate that represents the short-circuiting logical
             * OR of this predicate and the <b> other</b> predicate
             */
            template<class $1, class $2>
            BiPredicate &logicalOr(const BiPredicate<$1, $2> &other) const {
                CORE_STATIC_ASSERT(Class<T>::template isSuper<$1>() && Class<U>::template isSuper<$2>(),
                                   "Could not resolve given predicate");
                return from([&](X t, Y u) -> gbool { return test(t, u) || other.test(t, u); });
            }

            /**
             * Obtain new predicate from given class function member and specified compatible class instance
             *
             * @param instance The object used to invoke specified function member
             * @param method The internal function member used by returned predicate
             *
             * @tparam I The type of object callable with given method
             * @tparam M The type of method handle
             */
            template<class I, class M>
            static BiPredicate &from(I &&instance, M &&method) {
                // check if given method is valid
                try {
                    Functional::CheckFunction<M, I, X, Y>();
                    Functional::FunctionUtils<M>::validate(method);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.BiPredicate"_S));
                }

                // The return type of given method after calling with given instance and predicate argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R, , typename Class<M>::template Return<I, X, Y>);

                // The return type must be convertible to boolean value
                Functional::CheckReturn<gbool, R>();

                class MethodPredicate : public BiPredicate {
                private:
                    I &&inst;
                    M &&meth;

                public:
                    CORE_EXPLICIT MethodPredicate(I &&instance, M &&method) :
                            inst((I &&) instance), meth(Unsafe::forwardInstance<M>(method)) {}

                    /**
                     * Invoke the method of this predicate
                     */
                    R invoke(X t, Y u) const {
                        return (inst.*meth)(t, u);
                    }

                    gbool test(X t, Y u) const override {
                        return (gbool) invoke(t, u);
                    }

                    /**
                     * two instances are equals iff have same methods
                     * and same instances.
                     */
                    gbool equals(const Object &o) const override {
                        if (this == &o) {
                            return true;
                        }
                        if (!Class<MethodPredicate>::hasInstance(o)) {
                            return false;
                        }
                        MethodPredicate const &p = (MethodPredicate const &) o;
                        return Functional::FunctionUtils<I>::isEquals(inst, p.inst) &&
                               Functional::FunctionUtils<M>::isEquals(meth, p.meth);
                    }

                    Object &clone() const override {
                        return Unsafe::allocateInstance<MethodPredicate>(Unsafe::forwardInstance<I>(inst),
                                                                         Unsafe::forwardInstance<M>(meth));
                    }
                };

                return Unsafe::allocateInstance<MethodPredicate>(Unsafe::forwardInstance<I>(instance),
                                                                 Unsafe::forwardInstance<M>(method));
            }

            /**
             * Obtain new predicate from given function (classic function/ lambda function)
             *
             * @param function the internal function used by returned predicate
             *
             * @tparam F The type of function supporting predicate arguments.
             */
            template<class F>
            static BiPredicate &from(F &&function) {
                // check if given function is valid
                try {
                    Functional::CheckFunction<F, X, Y>();
                    Functional::FunctionUtils<F>::validate(function);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.BiPredicate"_S));
                }

                // The return type of given method after calling with given instance and predicate argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R, , typename Class<F>::template Return<X, Y>);

                // The return type must be convertible to boolean value
                Functional::CheckReturn<gbool, R>();

                class FunctionPredicate CORE_FINAL : public BiPredicate {
                private:
                    F &&func;

                public:
                    CORE_EXPLICIT FunctionPredicate(F &&func) : func(Unsafe::forwardInstance<F>(func)) {}

                    R invoke(X t, Y u) const {
                        return func(t, u);
                    }

                    gbool test(X t, Y u) const override {
                        return (gbool) invoke(t, u);
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o) {
                            return true;
                        }
                        if (!Class<FunctionPredicate>::hasInstance(o)) {
                            return false;
                        }
                        FunctionPredicate const &p = (FunctionPredicate const &) o;
                        return Functional::FunctionUtils<F>::isEquals(func, p.func);
                    }

                    Object &clone() const override {
                        return Unsafe::allocateInstance<FunctionPredicate>(Unsafe::forwardInstance<F>(func));
                    }
                };

                return Unsafe::allocateInstance<FunctionPredicate>(Unsafe::forwardInstance<F>(function));

            }

            /**
             * Transform this binary predicate to unary predicate with seconds argument set by given
             * value
             * @param defaultValue The value used by second argument of this predicate
             */
            Predicate<T> &toUnary(Y defaultValue) const {
                return Predicate<T>::from([&](X t) -> gbool { return test(t, defaultValue); });
            }

            /**
             * Obtain new Predicate of given signature with
             * This predicate.
             * @implNote This operation is possible only if
             * this predicate accept argument supported by the new
             * predicate
             */
            template<class $1, class $2>
            CORE_IMPLICIT operator BiPredicate<$1, $2> &() const {
                // search the arguments types of desired predicate
                CORE_ALIAS($X, Functional::Params<$1>);
                CORE_ALIAS($Y, Functional::Params<$2>);
                // The default signature accepted by this predicate
                CORE_ALIAS(Holder, void(*)(X, Y));
                // check If this predicate accept argument of desired predicate
                Functional::CheckFunction<Holder, $X, $Y>();
                return BiPredicate<$1, $2>::from([&]($X t, $Y u) -> void { accept(t, u); });
            }
        };

    } // core
} // function

#endif //CORE23_BIPREDICATE_H
