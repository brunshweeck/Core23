//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_CONSUMER_H
#define CORE23_CONSUMER_H

#include <core/function/Functional.h>
#include <core/IllegalArgumentException.h>
#include <core/private/Null.h>

namespace core {
    namespace function {

        /**
         * Represents an operation that accepts a single input argument and returns no
         * result. Unlike most other functional interfaces, Consumer is expected
         * to operate via side-effects.
         *
         * <p>This is a <a href="">functional interface</a>
         * whose functional method is <b style="color: orange;">accept(Object)</b>.
         *
         * @param T the type of the input to the operation
         */
        template<class T>
        class Consumer : public Functional {
        private:
            CORE_STATIC_ASSERT(Class<Object>::isSuper<T>(), "Argument type must have core.Object as base class.");
            CORE_STATIC_ASSERT(!(Class<T>::isConstant()), "Argument type mustn't have <const> as qualifier");
            CORE_STATIC_ASSERT(!(Class<T>::isVolatile()), "Argument type mustn't have <volatile> as qualifier");

            CORE_ALIAS(X, Functional::Params< T >);

        public:

            /**
             * Performs this operation on the given argument.
             *
             * @param p the input argument
             */
            virtual void accept(X p) const = 0;

            /**
             * Returns a composed <b>Consumer</b> that performs, in sequence, this
             * operation followed by the <b>after</b> operation. If performing either
             * operation throws an exception, it is relayed to the caller of the
             * composed operation.  If performing this operation throws an exception,
             * the <b>after</b> operation will not be performed.
             *
             * @param after the operation to perform after this operation
             */
            template<class $1>
            Consumer &andThen(const Consumer<$1> &after) const {
                CORE_STATIC_ASSERT(Class<$1>::template isSuper<T>(), "Could not resolve given predicate");
                return from([&](X t) -> void { accept(t), after.accept(t); });
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
            static Consumer &from(I &&instance, M &&method) {
                // check if given method is valid
                try {
                    Functional::CheckFunction<M, I, X>();
                    Functional::FunctionUtils<M>::validate(method);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.Consumer"_S));
                }

                class MethodConsumer : public Consumer {
                private:
                    I &&inst;
                    M &&meth;

                public:
                    CORE_EXPLICIT MethodConsumer(I &&instance, M &&method) :
                            inst((I &&) instance), meth(Unsafe::forwardInstance<M>(method)) {}

                    /**
                     * Invoke the method of this consumer
                     */
                    void invoke(X t) const {
                        return CORE_IGNORE((inst.*meth)(t));
                    }

                    void accept(X t) const override {
                        invoke(t);
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
            static Consumer &from(F &&function) {
                // check if given function is valid
                try {
                    Functional::CheckFunction<F, X>();
                    Functional::FunctionUtils<F>::validate(function);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.Consumer"_S));
                }

                class FunctionConsumer CORE_FINAL : public Consumer {
                private:
                    F &&func;

                public:
                    CORE_EXPLICIT FunctionConsumer(F &&func) : func(Unsafe::forwardInstance<F>(func)) {}

                    void invoke(X t) const {
                        return CORE_IGNORE(func(t));
                    }

                    void accept(X t) const override {
                        invoke(t);
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

        };

#if CORE_TEMPLATE_TYPE_DEDUCTION
        Consumer() -> Consumer<Object>;

        template<class Return, class Param, class ...Params>
        Consumer (Return(Param, Params...))

        ->
        Consumer<typename Class<Param>::Object>;
#endif

    } // core
} // Functor

#endif //CORE23_CONSUMER_H
