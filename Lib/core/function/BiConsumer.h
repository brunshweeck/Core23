//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_BICONSUMER_H
#define CORE23_BICONSUMER_H

#include <core/function/Consumer.h>

namespace core {
    namespace function {

        /**
         * Represents an operation that accepts two input arguments and returns no
         * result.  This is the two-arity specialization of <b style="color: orange;"> Consumer</b>.
         * Unlike most other functional interfaces, <b> BiConsumer</b> is expected
         * to operate via side-effects.
         *
         * <p>This is a <a href="package-summary.html">functional interface</a>
         * whose functional method is <b style="color: orange;"> BiConsumer.accept(Object, Object)</b>.
         *
         * @param T the type of the first argument to the operation
         * @param U the type of the second argument to the operation
         *
         * @see Consumer
         */
        template<class T, class U>
        class BiConsumer : public Functional {
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
             * Performs this operation on the given arguments.
             *
             * @param t the first input argument
             * @param u the second input argument
             */
            virtual void accept(X t, Y u) const = 0;

            /**
             * Returns a composed <b> BiConsumer</b> that performs, in sequence, this
             * operation followed by the <b> after</b> operation. If performing either
             * operation throws an exception, it is relayed to the caller of the
             * composed operation.  If performing this operation throws an exception,
             * the <b> after</b> operation will not be performed.
             *
             * @param after the operation to perform after this operation
             * @return a composed <b> BiConsumer</b> that performs in sequence this
             * operation followed by the <b> after</b> operation
             */
            template<class $1, class $2>
            BiConsumer &andThen(const BiConsumer<$1, $2> &after) const {
                CORE_STATIC_ASSERT(Class<$1>::template isSuper<T>() && Class<$2>::template isSuper<U>(),
                                   "Could not resolve given consumer");
                return from([&](X t, Y u) -> void { accept(t, u), after.accept(t, u); });
            }

            /**
             * Obtain new consumer from given class function member and specified compatible class instance
             *
             * @param instance The object used to invoke specified function member
             * @param method The internal function member used by returned consumer
             *
             * @tparam I The type of object callable with given method
             * @tparam M The type of method handle
             */
            template<class I, class M>
            static BiConsumer &from(I &&instance, M &&method) {
                // check if given method is valid
                try {
                    Functional::CheckFunction<M, I, X, Y>();
                    Functional::FunctionUtils<M>::validate(method);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.BiConsumer"_S));
                }

                class MethodConsumer : public BiConsumer {
                private:
                    I &&inst;
                    M &&meth;

                public:
                    CORE_EXPLICIT MethodConsumer(I &&instance, M &&method) :
                            inst((I &&) instance), meth(Unsafe::forwardInstance<M>(method)) {}

                    /**
                     * Invoke the method of this consumer
                     */
                    void invoke(X t, Y u) const {
                        return CORE_IGNORE((inst.*meth)(t, u));
                    }

                    void accept(X t, Y u) const override {
                        invoke(t, u);
                    }

                    /**
                     * two instances are equals iff have same methods
                     * and same instances.
                     */
                    gbool equals(const Object &o) const override {
                        if (this == &o) {
                            return true;
                        }
                        if (!Class<MethodConsumer>::hasInstance(o)) {
                            return false;
                        }
                        MethodConsumer const &p = (MethodConsumer const &) o;
                        return Functional::FunctionUtils<I>::isEquals(inst, p.inst) &&
                               Functional::FunctionUtils<M>::isEquals(meth, p.meth);
                    }

                    Object &clone() const override {
                        return Unsafe::allocateInstance<MethodConsumer>(Unsafe::forwardInstance<I>(inst),
                                                                        Unsafe::forwardInstance<M>(meth));
                    }
                };

                return Unsafe::allocateInstance<MethodConsumer>(Unsafe::forwardInstance<I>(instance),
                                                                Unsafe::forwardInstance<M>(method));
            }

            /**
             * Obtain new consumer from given function (classic function/ lambda function)
             *
             * @param function the internal function used by returned consumer
             *
             * @tparam F The type of function supporting consumer arguments.
             */
            template<class F>
            static BiConsumer &from(F &&function) {
                // check if given function is valid
                try {
                    Functional::CheckFunction<F, X, Y>();
                    Functional::FunctionUtils<F>::validate(function);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.BiConsumer"_S));
                }

                class FunctionConsumer CORE_FINAL : public BiConsumer {
                private:
                    F &&func;

                public:
                    CORE_EXPLICIT FunctionConsumer(F &&func) : func(Unsafe::forwardInstance<F>(func)) {}

                    void invoke(X t, Y u) const {
                        return CORE_IGNORE(func(t, u));
                    }

                    void accept(X t, Y u) const override {
                        invoke(t, u);
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o) {
                            return true;
                        }
                        if (!Class<FunctionConsumer>::hasInstance(o)) {
                            return false;
                        }
                        FunctionConsumer const &p = (FunctionConsumer const &) o;
                        return Functional::FunctionUtils<F>::isEquals(func, p.func);
                    }

                    Object &clone() const override {
                        return Unsafe::allocateInstance<FunctionConsumer>(Unsafe::forwardInstance<F>(func));
                    }
                };

                return Unsafe::allocateInstance<FunctionConsumer>(Unsafe::forwardInstance<F>(function));

            }

            /**
             * Transform this binary consumer to unary predicate with seconds argument set by given
             * value
             * @param defaultValue The value used by second argument of this consumer
             */
            Consumer<T> &toUnary(Y defaultValue) const {
                return Consumer<T>::from([&](X t) -> gbool { return test(t, defaultValue); });
            }

            /**
             * Obtain new Consumer of given signature with
             * This consumer.
             * @implNote This operation is possible only if
             * this consumer accept argument supported by the new
             * consumer
             */
            template<class $1, class $2>
            CORE_IMPLICIT operator BiConsumer<$1, $2> &() const {
                // search the arguments types of desired consumer
                CORE_ALIAS($X, Functional::Params<$1>);
                CORE_ALIAS($Y, Functional::Params<$2>);
                // The default signature accepted by this consumer
                CORE_ALIAS(Holder, void(*)(X, Y));
                // check If this consumer accept argument of desired consumer
                Functional::CheckFunction<Holder, $X, $Y>();
                return BiConsumer<$1, $2>::from([&]($X t, $Y u) -> void { accept(t, u); });
            }
        };

    } // core
} // Functor

#endif //CORE23_BICONSUMER_H
