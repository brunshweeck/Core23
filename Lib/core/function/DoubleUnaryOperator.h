//
// Created by T.N.Brunshweeck on 30/01/2024.
//

#ifndef CORE23_DOUBLEUNARYOPERATOR_H
#define CORE23_DOUBLEUNARYOPERATOR_H

#include <core/function/Functional.h>

namespace core {
    namespace function {

        class DoubleUnaryOperator : public Functional {
        public:

            /**
             * Applies this function to the given argument.
             *
             * @param t the function argument
             * @return the function result
             */
            virtual gdouble apply(gdouble t) const = 0;

            /**
             * Returns a composed function that first applies the <b> before</b>
             * function to its input, and then applies this function to the result.
             * If evaluation of either function throws an exception, it is relayed to
             * the caller of the composed function.
             *
             * @tparam V the type of input to the <b> before</b>  function, and to the
             *           composed function
             * @param before the function to apply before this function is applied
             * @return a composed function that first applies the <b> before</b>
             * function and then applies this function
             *
             * @see #andThen(DoubleUnaryOperator)
             */
            DoubleUnaryOperator &compose(const DoubleUnaryOperator &before) const {
                return DoubleUnaryOperator::from([&](gdouble v) -> gdouble { return apply(before.apply(v)); });
            }

            /**
             * Returns a composed function that first applies this function to
             * its input, and then applies the <b> after</b>  function to the result.
             * If evaluation of either function throws an exception, it is relayed to
             * the caller of the composed function.
             *
             * @tparam V the type of output of the <b> after</b>  function, and of the
             *           composed function
             * @param after the function to apply after this function is applied
             * @return a composed function that first applies this function and then
             * applies the <b> after</b>  function
             *
             * @see #compose(DoubleUnaryOperator)
             */
            DoubleUnaryOperator &andThen(const DoubleUnaryOperator &after) const {
                return DoubleUnaryOperator::from([&](gdouble t) -> gdouble { return after.apply(apply(t)); });
            }

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
            static DoubleUnaryOperator &from(I &&instance, M &&method) {
                // check if given method is valid
                try {
                    Functional::CheckFunction<M, I, gdouble>();
                    Functional::FunctionUtils<M>::validate(method);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.DoubleUnaryOperator"_S));
                }

                // The return type of given method after calling with given instance and function argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R2, , typename Class<M>::template Return<I, gdouble>);

                // The return type must be convertible to boolean value
                Functional::CheckReturn<gdouble, R2>();

                class MethodFunction : public DoubleUnaryOperator {
                private:
                    I &&inst;
                    M &&meth;

                public:
                    CORE_EXPLICIT MethodFunction(I &&instance, M &&method) :
                            inst((I &&) instance), meth(Unsafe::forwardInstance<M>(method)) {}

                    /**
                     * Invoke the method of this function
                     */
                    R2 invoke(gdouble t) const {
                        return (inst.*meth)(t);
                    }

                    gdouble apply(gdouble t) const override {
                        return (gdouble) invoke(t);
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
            static DoubleUnaryOperator &from(F &&function) {
                // check if given function is valid
                try {
                    Functional::CheckFunction<F, gdouble>();
                    Functional::FunctionUtils<F>::validate(function);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.DoubleUnaryOperator"_S));
                }

                // The return type of given method after calling with given instance and function argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R2, , typename Class<F>::template Return<gdouble>);

                // The return type must be convertible to boolean value
                Functional::CheckReturn<gdouble, R2>();

                class FunctionFunction CORE_FINAL : public DoubleUnaryOperator {
                        private:
                        F &&func;

                        public:
                        CORE_EXPLICIT FunctionFunction(F &&func) : func(Unsafe::forwardInstance<F>(func)) {}

                        R2 invoke(gdouble t) const {
                            return func(t);
                        }

                        gdouble apply(gdouble t) const override {
                            return (gdouble) invoke(t);
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
        };

    } // function
} // core

#endif //CORE23_DOUBLEUNARYOPERATOR_H
