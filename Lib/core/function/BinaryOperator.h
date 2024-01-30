//
// Created by T.N.Brunshweeck on 30/01/2024.
//

#ifndef CORE23_BINARYOPERATOR_H
#define CORE23_BINARYOPERATOR_H

#include <core/function/BiFunction.h>
#include <core/function/UnaryOperator.h>
#include <core/util/Comparator.h>

namespace core {
    namespace function {

        /**
         * Represents an operation upon two operands of the same type, producing a result
         * of the same type as the operands.  This is a specialization of
         * <b style="color: orange;"> BiFunction</b> for the case where the operands and the result are all of
         * the same type.
         *
         * <p>This is a <a href="package-summary.html">functional interface</a>
         * whose functional method is <b style="color: orange;"> BinaryOperator::apply(Object, Object)</b>.
         *
         * @param T the type of the operands and result of the operator
         *
         * @see BiFunction
         * @see UnaryOperator
         */
        template<class T>
        class BinaryOperator : public BiFunction<T, T, T> {
        private:
            CORE_ALIAS(X, Functional::Params<T>);
            CORE_ALIAS(Y, Functional::Params<T>);
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
            static BinaryOperator &from(I &&instance, M &&method) {
                // check if given method is valid
                try {
                    Functional::CheckFunction<M, I, X, Y>();
                    Functional::FunctionUtils<M>::validate(method);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.BinaryOperator"_S));
                }

                // The return type of given method after calling with given instance and function argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R, , typename Class<M>::template Return<I, X, Y>);

                // The return type must be convertible to boolean value
                Functional::CheckReturn<gbool, R>();

                class MethodFunction : public BinaryOperator {
                private:
                    I &&inst;
                    M &&meth;

                public:
                    CORE_EXPLICIT MethodFunction(I &&instance, M &&method) :
                            inst((I &&) instance), meth(Unsafe::forwardInstance<M>(method)) {}

                    /**
                     * Invoke the method of this function
                     */
                    R invoke(X t, Y u) const {
                        return (inst.*meth)(t, u);
                    }

                    Z apply(X t, Y u) const override {
                        return (Z) invoke(t, u);
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
            static BinaryOperator &from(F &&function) {
                // check if given function is valid
                try {
                    Functional::CheckFunction<F, X, Y>();
                    Functional::FunctionUtils<F>::validate(function);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.BinaryOperator"_S));
                }

                // The return type of given method after calling with given instance and function argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R, , typename Class<F>::template Return<X, Y>);

                // The return type must be convertible to boolean value
                Functional::CheckReturn<Z, R>();

                class FunctionFunction CORE_FINAL : public BinaryOperator {
                private:
                    F &&func;

                public:
                    CORE_EXPLICIT FunctionFunction(F &&func) : func(Unsafe::forwardInstance<F>(func)) {}

                    R invoke(X t, Y u) const {
                        return func(t, u);
                    }

                    Z apply(X t, Y u) const override {
                        return (Z) invoke(t, u);
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
             * Returns a {@link BinaryOperator</b>  which returns the greater of two elements
             * according to the specified <b> Comparator</b> .
             *
             * @param <T> the type of the input arguments of the comparator
             * @param comparator a <b> Comparator</b>  for comparing the two values
             * @return a <b> BinaryOperator</b>  which returns the greater of its operands,
             *         according to the supplied <b> Comparator</b> 
             */
            template<class $1, Class<gbool>::OnlyIf<Class<$1>::template isSuper<T>()> = true>
            static BinaryOperator &maxBy(util::Comparator<$1> const &comparator) {
                return from([&comparator](X t, Y u) -> Z { return comparator.compare(t, u) >= 0 ? t : u; });
            }

            /**
             * Transform this binary operator to unary operator with seconds argument set by given
             * value
             * @param defaultValue The value used by second argument of this operator
             */
            UnaryOperator<T> &toUnary(Y defaultValue) const override {
                return UnaryOperator<T>::from([&](X t) -> Z { return apply(t, defaultValue); });
            }

        };

    } // function
} // core

#endif //CORE23_BINARYOPERATOR_H
