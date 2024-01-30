//
// Created by T.N.Brunshweeck on 30/01/2024.
//

#ifndef CORE23_UNARYOPERATOR_H
#define CORE23_UNARYOPERATOR_H

#include <core/function/Function.h>

namespace core {
    namespace function {

        /**
         * Represents an operation on a single operand that produces a result of the
         * same type as its operand.  This is a specialization of <b> Function</b>  for
         * the case where the operand and result are of the same type.
         *
         * <p>This is a <a href="">functional interface</a>
         * whose functional method is <b style="color: orange;"> UnaryOperator::apply(Object)</b> .
         *
         * @param <T> the type of the operand and result of the operator
         *
         * @see Function
         */
        template<class T>
        class UnaryOperator : public Function<T, T> {
        private:
            CORE_ALIAS(X, Functional::Params<T>);
            CORE_ALIAS(Z, Functional::Return<T>);

        public:

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
            static UnaryOperator &from(I &&instance, M &&method) {
                // check if given method is valid
                try {
                    Functional::CheckFunction<M, I, X>();
                    Functional::FunctionUtils<M>::validate(method);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.UnaryOperator"_S));
                }

                // The return type of given method after calling with given instance and function argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R, , typename Class<M>::template Return<I, X>);

                // The return type must be convertible to boolean value
                Functional::CheckReturn<Z, R>();

                class MethodFunction : public UnaryOperator {
                private:
                    I &&inst;
                    M &&meth;

                public:
                    CORE_EXPLICIT MethodFunction(I &&instance, M &&method) :
                            inst((I &&) instance), meth(Unsafe::forwardInstance<M>(method)) {}

                    /**
                     * Invoke the method of this function
                     */
                    R invoke(X t) const {
                        return (inst.*meth)(t);
                    }

                    Z apply(X t) const override {
                        return (Z) invoke(t);
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
            static UnaryOperator &from(F &&function) {
                // check if given function is valid
                try {
                    Functional::CheckFunction<F, X>();
                    Functional::FunctionUtils<F>::validate(function);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.UnaryOperator"_S));
                }

                // The return type of given method after calling with given instance and function argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R, , typename Class<F>::template Return<X>);

                // The return type must be convertible to boolean value
                Functional::CheckReturn<Z, R>();

                class FunctionFunction CORE_FINAL : public UnaryOperator {
                private:
                    F &&func;

                public:
                    CORE_EXPLICIT FunctionFunction(F &&func) : func(Unsafe::forwardInstance<F>(func)) {}

                    R invoke(X t) const {
                        return func(t);
                    }

                    Z apply(X t) const override {
                        return (Z) invoke(t);
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
             * Returns a unary operator that always returns its input argument.
             *
             * @return a unary operator that always returns its input argument
             */
            static UnaryOperator &identity() {
                return from([](X t) -> Z { return (Z) t; });
            }
        };

    } // function
} // core

#endif //CORE23_UNARYOPERATOR_H
