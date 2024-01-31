//
// Created by T.N.Brunshweeck on 30/01/2024.
//

#ifndef CORE23_INTBINARYOPERATOR_H
#define CORE23_INTBINARYOPERATOR_H

#include <core/function/Functional.h>
#include <core/function/IntUnaryOperator.h>

namespace core {
    namespace function {

        class IntBinaryOperator  : public Functional {
        public:

            /**
             * Applies this function to the given arguments.
             *
             * @param t the first function argument
             * @param u the second function argument
             * @return the function result
             */
            virtual gint applyAsInt(gint t, gint u) const = 0;

            /**
             * Obtain new function from given class function member and specified compatible class instance
             *
             * @param instance The object used to invoke specified function member
             * @param method The internal function member used by returned function
             *
             * @tparam I The type of object callable with given method
             * @tparam M The type of method handle
             */
            template<class I, class M>
            static IntBinaryOperator &from(I &&instance, M &&method) {
                // check if given method is valid
                try {
                    Functional::CheckFunction<M, I, gint, gint>();
                    Functional::FunctionUtils<M>::validate(method);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.IntBinaryOperator"_S));
                }

                // The return type of given method after calling with given instance and function argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R2, , typename Class<M>::template Return<I, gint, gint>);

                // The return type must be convertible to boolean value
                Functional::CheckReturn<gbool, R2>();

                class MethodFunction : public IntBinaryOperator {
                private:
                    I &&inst;
                    M &&meth;

                public:
                    CORE_EXPLICIT MethodFunction(I &&instance, M &&method) :
                            inst((I &&) instance), meth(Unsafe::forwardInstance<M>(method)) {}

                    /**
                     * Invoke the method of this function
                     */
                    R2 invoke(gint t, gint u) const {
                        return (inst.*meth)(t, u);
                    }

                    gint applyAsInt(gint t, gint u) const override {
                        return (gint) invoke(t, u);
                    }

                    /**
                     * two instances are equals iff have same methods
                     * and same instances.
                     */
                    gbool equals(const Object &o) const override {
                        if (this == &o) {
                            return true;
                        }
                        if (!Class<MethodFunction>::hasInstance(o)) {
                            return false;
                        }
                        MethodFunction const &p = (MethodFunction const &) o;
                        return Functional::FunctionUtils<I>::isEquals(inst, p.inst) &&
                               Functional::FunctionUtils<M>::isEquals(meth, p.meth);
                    }

                    Object &clone() const override {
                        return Unsafe::allocateInstance<MethodFunction>(Unsafe::forwardInstance<I>(inst),
                                                                        Unsafe::forwardInstance<M>(meth));
                    }
                };

                return Unsafe::allocateInstance<MethodFunction>(Unsafe::forwardInstance<I>(instance),
                                                                Unsafe::forwardInstance<M>(method));
            }

            /**
             * Obtain new function from given function (classic function/ lambda function)
             *
             * @param function the internal function used by returned function
             *
             * @tparam F The type of function supporting function arguments.
             */
            template<class F>
            static IntBinaryOperator &from(F &&function) {
                // check if given function is valid
                try {
                    Functional::CheckFunction<F, gint, gint>();
                    Functional::FunctionUtils<F>::validate(function);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.IntBinaryOperator"_S));
                }

                // The return type of given method after calling with given instance and function argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R2, , typename Class<F>::template Return<gint, gint>);

                // The return type must be convertible to boolean value
                Functional::CheckReturn<gint, R2>();

                class FunctionFunction CORE_FINAL : public IntBinaryOperator {
                private:
                    F &&func;

                public:
                    CORE_EXPLICIT FunctionFunction(F &&func) : func(Unsafe::forwardInstance<F>(func)) {}

                    R2 invoke(gint t, gint u) const {
                        return func(t, u);
                    }

                    gint applyAsInt(gint t, gint u) const override {
                        return (gint) invoke(t, u);
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o) {
                            return true;
                        }
                        if (!Class<FunctionFunction>::hasInstance(o)) {
                            return false;
                        }
                        FunctionFunction const &p = (FunctionFunction const &) o;
                        return Functional::FunctionUtils<F>::isEquals(func, p.func);
                    }

                    Object &clone() const override {
                        return Unsafe::allocateInstance<FunctionFunction>(Unsafe::forwardInstance<F>(func));
                    }
                };

                return Unsafe::allocateInstance<FunctionFunction>(Unsafe::forwardInstance<F>(function));

            }

            /**
             * Transform this binary function to unary function with seconds argument set by given
             * value
             * @param defaultValue The value used by second argument of this function
             */
            virtual IntUnaryOperator &toUnary(gint defaultValue) const {
                return IntUnaryOperator::from([=](gint t) -> gint { return applyAsInt(t, defaultValue); });
            }
        };

    } // function
} // core

#endif //CORE23_INTBINARYOPERATOR_H
