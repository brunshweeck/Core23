//
// Created by T.N.Brunshweeck on 30/01/2024.
//

#ifndef CORE23_INTCONSUMER_H
#define CORE23_INTCONSUMER_H

#include <core/function/Functional.h>

namespace core {
    namespace function {

        class IntConsumer : public Functional {
        public:

            /**
             * Performs this operation on the given argument.
             *
             * @param p the input argument
             */
            virtual void accept(gint p) const = 0;

            /**
             * Returns a composed <b>IntConsumer</b> that performs, in sequence, this
             * operation followed by the <b>after</b> operation. If performing either
             * operation throws an exception, it is relayed to the caller of the
             * composed operation.  If performing this operation throws an exception,
             * the <b>after</b> operation will not be performed.
             *
             * @param after the operation to perform after this operation
             */
            IntConsumer &andThen(const IntConsumer &after) const {
                return from([&](gint t) -> void { accept(t), after.accept(t); });
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
            static IntConsumer &from(I &&instance, M &&method) {
                // check if given method is valid
                try {
                    Functional::CheckFunction<M, I, gint>();
                    Functional::FunctionUtils<M>::validate(method);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.IntConsumer"_S));
                }

                class MethodConsumer : public IntConsumer {
                private:
                    I &&inst;
                    M &&meth;

                public:
                    CORE_EXPLICIT MethodConsumer(I &&instance, M &&method) :
                            inst((I &&) instance), meth(Unsafe::forwardInstance<M>(method)) {}

                    /**
                     * Invoke the method of this consumer
                     */
                    void invoke(gint t) const {
                        return CORE_IGNORE((inst.*meth)(t));
                    }

                    void accept(gint t) const override {
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
            static IntConsumer &from(F &&function) {
                // check if given function is valid
                try {
                    Functional::CheckFunction<F, gint>();
                    Functional::FunctionUtils<F>::validate(function);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.IntConsumer"_S));
                }

                class FunctionConsumer CORE_FINAL : public IntConsumer {
                private:
                    F &&func;

                public:
                    CORE_EXPLICIT FunctionConsumer(F &&func) : func(Unsafe::forwardInstance<F>(func)) {}

                    void invoke(gint t) const {
                        return CORE_IGNORE(func(t));
                    }

                    void accept(gint t) const override {
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

    } // function
} // core

#endif //CORE23_INTCONSUMER_H
