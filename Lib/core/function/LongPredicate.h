//
// Created by T.N.Brunshweeck on 30/01/2024.
//

#ifndef CORE23_LONGPREDICATE_H
#define CORE23_LONGPREDICATE_H

#include <core/Long.h>
#include <core/function/Functional.h>

namespace core {
    namespace function {

        class LongPredicate : public Functional {
        public:

            /**
             * Evaluates this predicate on the given argument.
             *
             * @param p the input argument
             *
             * @return <b>true</b> if the input argument matches the predicate,
             * otherwise <b>false</b>
             */
            virtual gbool test(glong t) const = 0;

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
            LongPredicate &logicalAnd(const LongPredicate &other) const {
                return from([&](glong t) -> gbool { return test(t) && other.test(t); });
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
            LongPredicate &logicalOr(const LongPredicate &other) const {
                return from([&](glong t) -> gbool { return test(t) || other.test(t); });
            }

            /**
             * Returns a predicate that represents the logical negation of this
             * predicate.
             *
             * @return a predicate that represents the logical negation of this
             * predicate
             */
            virtual LongPredicate &negate() const {
                return from([&](glong t) -> gbool { return !test(t); });
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
            static LongPredicate &from(I &&instance, M &&method) {
                // check if given method is valid
                try {
                    Functional::CheckFunction<M, I, glong>();
                    Functional::FunctionUtils<M>::validate(method);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.LongPredicate"_S));
                }

                // The return type of given method after calling with given instance and predicate argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R, , typename Class<M>::template Return<I, glong>);

                // The return type must be convertible to boolean value
                Functional::CheckReturn<gbool, R>();

                class MethodPredicate : public LongPredicate {
                private:
                    I &&inst;
                    M &&meth;

                public:
                    CORE_EXPLICIT MethodPredicate(I &&instance, M &&method) :
                            inst((I &&) instance), meth(Unsafe::forwardInstance<M>(method)) {}

                    /**
                     * Invoke the method of this predicate
                     */
                    R invoke(glong t) const {
                        return (inst.*meth)(t);
                    }

                    gbool test(glong t) const override {
                        return (gbool) invoke(t);
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
            static LongPredicate &from(F &&function) {
                // check if given function is valid
                try {
                    Functional::CheckFunction<F, glong>();
                    Functional::FunctionUtils<F>::validate(function);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.LongPredicate"_S));
                }

                // The return type of given method after calling with given instance and predicate argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R, , typename Class<F>::template Return<glong>);

                // The return type must be convertible to boolean value
                Functional::CheckReturn<gbool, R>();

                class FunctionPredicate CORE_FINAL : public LongPredicate {
                private:
                    F &&func;

                public:
                    CORE_EXPLICIT FunctionPredicate(F &&func) : func(Unsafe::forwardInstance<F>(func)) {}

                    R invoke(glong t) const {
                        return func(t);
                    }

                    gbool test(glong t) const override {
                        return (gbool) invoke(t);
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
        };

    } // function
} // core

#endif //CORE23_LONGPREDICATE_H
