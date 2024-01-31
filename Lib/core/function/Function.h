//
// Created by T.N.Brunshweeck on 27/11/2023.
//

#ifndef CORE23_FUNCTION_H
#define CORE23_FUNCTION_H

#include <core/function/Functional.h>

namespace core {
    namespace function {

        /**
         * Represents a function that accepts one argument and produces a result.
         *
         * <p>This is a <a href="package-summary.html">functional interface</a>
         * whose functional method is {@link #apply(Object)}.
         *
         * @tparam T the type of the input to the function
         * @tparam R the type of the result of the function
         */
        template<class T, class R>
        class Function : public Functional {
        private:
            CORE_STATIC_ASSERT(Class<Object>::isSuper<T>(), "Argument type must have core.Object as base class.");
            CORE_STATIC_ASSERT(!(Class<T>::isConstant()), "Argument type mustn't have <const> as qualifier");
            CORE_STATIC_ASSERT(!(Class<T>::isVolatile()), "Argument type mustn't have <volatile> as qualifier");
            CORE_STATIC_ASSERT(!(Class<R>::isVolatile()), "Return type mustn't have <volatile> as qualifier");

            CORE_ALIAS(X, Functional::Params<T>);
            CORE_ALIAS(Z, Functional::Return<R>);

        public:

            /**
             * Applies this function to the given argument.
             *
             * @param t the function argument
             * @return the function result
             */
            virtual Z apply(X t) const = 0;

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
             * @see #andThen(Function)
             */
            template<class V, class $1>
            Function<V, R> &compose(const Function<V, $1> &before) const {
                CORE_STATIC_ASSERT(Class<T>::template isSuper<$1>(), "Could not resolve given function");
                CORE_ALIAS(Y, Functional::Params<V>);
                return Function<V, R>::from([&](Y v) -> Z { return apply(before.apply(v)); });
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
             * @see #compose(Function)
             */
            template<class V, class $2>
            Function<T, V> &andThen(const Function<$2, V> &after) const {
                CORE_STATIC_ASSERT(Class<$2>::template isSuper<R>(), "Could not resolve given function");
                CORE_ALIAS(R2, Functional::Return<V>);
                return Function<T, V>::from([&](X t) -> R2 { return after.apply(apply(t)); });
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
            static Function &from(I &&instance, M &&method) {
                // check if given method is valid
                try {
                    Functional::CheckFunction<M, I, X>();
                    Functional::FunctionUtils<M>::validate(method);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.Function"_S));
                }

                // The return type of given method after calling with given instance and function argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R2, , typename Class<M>::template Return<I, X>);

                // The return type must be convertible to boolean value
                Functional::CheckReturn<Z, R2>();

                class MethodFunction : public Function {
                private:
                    I &&inst;
                    M &&meth;

                public:
                    CORE_EXPLICIT MethodFunction(I &&instance, M &&method) :
                            inst((I &&) instance), meth(Unsafe::forwardInstance<M>(method)) {}

                    /**
                     * Invoke the method of this function
                     */
                    R2 invoke(X t) const {
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
            static Function &from(F &&function) {
                // check if given function is valid
                try {
                    Functional::CheckFunction<F, X>();
                    Functional::FunctionUtils<F>::validate(function);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.Function"_S));
                }

                // The return type of given method after calling with given instance and function argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R2, , typename Class<F>::template Return<X>);

                // The return type must be convertible to boolean value
                Functional::CheckReturn<Z, R2>();

                class FunctionFunction CORE_FINAL : public Function {
                private:
                    F &&func;

                public:
                    CORE_EXPLICIT FunctionFunction(F &&func) : func(Unsafe::forwardInstance<F>(func)) {}

                    R2 invoke(X t) const {
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
             * Obtain new Function of given signature with
             * This function.
             * @implNote This operation is possible only if
             * this function accept argument supported by the new
             * function
             */
            template<class $1, class $2>
            CORE_IMPLICIT operator Function<$1, $2> &() const {
                // search the arguments types of desired function
                CORE_ALIAS($X, Functional::Params<$1>);
                CORE_ALIAS($R, Functional::Return<$2>);
                // The default signature accepted by this function
                CORE_ALIAS(Holder, Z(*)(X));
                // check If this function accept argument of desired function
                Functional::CheckFunction<Holder, $X>();
                // check if the return type of this function is convertible to
                // return type of desired function.
                Functional::CheckReturn<$R, Z>();
                return Function<$1, $2>::from([&]($X t) -> $R { ($R) apply(t); });
            }
        };

    } // core
} // util

#endif //CORE23_FUNCTION_H
