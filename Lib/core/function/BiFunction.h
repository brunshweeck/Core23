//
// Created by T.N.Brunshweeck on 27/11/2023.
//

#ifndef CORE23_BIFUNCTION_H
#define CORE23_BIFUNCTION_H

#include <core/function/Function.h>

namespace core {
    namespace function {

        /**
         * Represents a function that accepts two arguments and produces a result.
         * This is the two-arity specialization of <b style="color: orange;"> Function</b> .
         *
         * <p>This is a <a href="">functional interface</a>
         * whose functional method is <b style="color: orange;"> BiFunction.apply(Object, Object)</b> .
         *
         * @param T the type of the first argument to the function
         * @param U the type of the second argument to the function
         * @param R the type of the result of the function
         *
         * @see Function
         */
        template<class T, class U, class R>
        class BiFunction : public Functional {
        private:
            CORE_STATIC_ASSERT(Class<Object>::isSuper<T>() && Class<Object>::isSuper<U>(),
                               "First and second argument type must have core.Object as base class.");
            CORE_STATIC_ASSERT(!(Class<T>::isConstant() || Class<U>::isConstant()),
                               "First and second argument type mustn't have <const> as qualifier");
            CORE_STATIC_ASSERT(!(Class<T>::isVolatile() || Class<U>::isVolatile()),
                               "First and second argument type mustn't have <volatile> as qualifier");

            CORE_ALIAS(X, Functional::Params<T>);
            CORE_ALIAS(Y, Functional::Params<U>);
            CORE_ALIAS(Z, Functional::Return<R>);

        public:

            /**
             * Applies this function to the given arguments.
             *
             * @param t the first function argument
             * @param u the second function argument
             * @return the function result
             */
            virtual Z apply(X t, Y u) const = 0;

            /**
             * Returns a composed function that first applies this function to
             * its input, and then applies the <b> after</b>  function to the result.
             * If evaluation of either function throws an exception, it is relayed to
             * the caller of the composed function.
             *
             * @param <V> the type of output of the <b> after</b>  function, and of the
             *           composed function
             * @param after the function to apply after this function is applied
             * @return a composed function that first applies this function and then
             * applies the <b> after</b>  function
             */
            template<class V, class $2>
            BiFunction<T, U, V> &andThen(const Function<$2, V> &after) const {
                CORE_STATIC_ASSERT(Class<R>::template isSuper<$2>(), "Could not resolve given function");
                return from([&](X t, Y u) -> Z { after.apply(apply(t, u)); });
            };

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
            static BiFunction &from(I &&instance, M &&method) {
                // check if given method is valid
                try {
                    Functional::CheckFunction<M, I, X, Y>();
                    Functional::FunctionUtils<M>::validate(method);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.BiFunction"_S));
                }

                // The return type of given method after calling with given instance and function argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R2, , typename Class<M>::template Return<I, X, Y>);

                // The return type must be convertible to boolean value
                Functional::CheckReturn<gbool, R2>();

                class MethodFunction : public BiFunction {
                private:
                    I &&inst;
                    M &&meth;

                public:
                    CORE_EXPLICIT MethodFunction(I &&instance, M &&method) :
                            inst((I &&) instance), meth(Unsafe::forwardInstance<M>(method)) {}

                    /**
                     * Invoke the method of this function
                     */
                    R2 invoke(X t, Y u) const {
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
            static BiFunction &from(F &&function) {
                // check if given function is valid
                try {
                    Functional::CheckFunction<F, X, Y>();
                    Functional::FunctionUtils<F>::validate(function);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.BiFunction"_S));
                }

                // The return type of given method after calling with given instance and function argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R2, , typename Class<F>::template Return<X, Y>);

                // The return type must be convertible to boolean value
                Functional::CheckReturn<Z, R2>();

                class FunctionFunction CORE_FINAL : public BiFunction {
                private:
                    F &&func;

                public:
                    CORE_EXPLICIT FunctionFunction(F &&func) : func(Unsafe::forwardInstance<F>(func)) {}

                    R2 invoke(X t, Y u) const {
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
             * Transform this binary function to unary function with seconds argument set by given
             * value
             * @param defaultValue The value used by second argument of this function
             */
            virtual Function<T, R> &toUnary(Y defaultValue) const {
                return Function<T, R>::from([&](X t) -> Z { return apply(t, defaultValue); });
            }
        };

    } // core
} // util

#endif //CORE23_BIFUNCTION_H
