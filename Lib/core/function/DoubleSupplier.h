//
// Created by T.N.Brunshweeck on 30/01/2024.
//

#ifndef CORE23_DOUBLESUPPLIER_H
#define CORE23_DOUBLESUPPLIER_H

#include <core/Double.h>
#include <core/function/Supplier.h>
#include <core/function/Functional.h>

namespace core {
    namespace function {


        /**
         * Represents a supplier of <b> double</b> -valued results.  This is the
         * <b> double</b> -producing primitive specialization of <b style="color: orange;"> Supplier</b> .
         *
         * <p>There is no requirement that a new or distinct result be returned each
         * time the supplier is invoked.
         *
         * <p>This is a <a href="package-summary.html">functional interface</a>
         * whose functional method is <b style="color: orange;"> DoubleSupplier.get()</b> .
         *
         * @see Supplier
         */
        class DoubleSupplier : public Functional {
        public:

            /**
             * Gets a result.
             *
             * @return a result
             */
            virtual gdouble get() const = 0;

            /**
             * Obtain new supplier from given class function member and specified compatible class instance
             *
             * @param instance The object used to invoke specified function member
             * @param method The internal function member used by returned supplier
             *
             * @tparam I The type of object callable with given method
             * @tparam M The type of method handle
             */
            template<class I, class M>
            static DoubleSupplier &from(I &&instance, M &&method) {
                // check if given method is valid
                try {
                    Functional::CheckFunction<M, I>();
                    Functional::FunctionUtils<M>::validate(method);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.BooleanSupplier"_S));
                }

                // The return type of given method after calling with given instance and supplier argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R, , typename Class<M>::template Return<I>);

                // The return type must be convertible to double value
                Functional::CheckReturn<gdouble, R>();

                class MethodSupplier : public DoubleSupplier {
                private:
                    I &&inst;
                    M &&meth;

                public:
                    CORE_EXPLICIT MethodSupplier(I &&instance, M &&method) :
                            inst((I &&) instance), meth(Unsafe::forwardInstance<M>(method)) {}

                    /**
                     * Invoke the method of this supplier
                     */
                    R invoke() const {
                        return (inst.*meth)();
                    }

                    gdouble get() const override {
                        return (gdouble) invoke();
                    }

                    /**
                     * two instances are equals iff have same methods
                     * and same instances.
                     */
                    gbool equals(const Object &o) const override {
                        if (this == &o) {
                            return true;
                        }
                        if (!Class<MethodSupplier>::hasInstance(o)) {
                            return false;
                        }
                        MethodSupplier const &p = (MethodSupplier const &) o;
                        return Functional::FunctionUtils<I>::isEquals(inst, p.inst) &&
                               Functional::FunctionUtils<M>::isEquals(meth, p.meth);
                    }

                    Object &clone() const override {
                        return Unsafe::allocateInstance<MethodSupplier>(Unsafe::forwardInstance<I>(inst),
                                                                        Unsafe::forwardInstance<M>(meth));
                    }
                };

                return Unsafe::allocateInstance<MethodSupplier>(Unsafe::forwardInstance<I>(instance),
                                                                Unsafe::forwardInstance<M>(method));
            }

            /**
             * Obtain new supplier from given function (classic function/ lambda function)
             *
             * @param function the internal function used by returned supplier
             *
             * @tparam F The type of function supporting supplier arguments.
             */
            template<class F>
            static DoubleSupplier &from(F &&function) {
                // check if given function is valid
                try {
                    Functional::CheckFunction<F>();
                    Functional::FunctionUtils<F>::validate(function);
                } catch (IllegalArgumentException const &ex) {
                    ex.throws(__trace(u"core.function.DoubleSupplier"_S));
                }

                // The return type of given method after calling with given instance and supplier argument
                // R = Class(instance.method(t, u));
                CORE_ALIAS(R, , typename Class<F>::template Return<>);

                // The return type must be convertible to double value
                Functional::CheckReturn<gdouble, R>();

                class FunctionSupplier CORE_FINAL : public DoubleSupplier {
                private:
                    F &&func;

                public:
                    CORE_EXPLICIT FunctionSupplier(F &&func) : func(Unsafe::forwardInstance<F>(func)) {}

                    R invoke() const {
                        return func();
                    }

                    gdouble get() const override {
                        return (gdouble) invoke();
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o) {
                            return true;
                        }
                        if (!Class<FunctionSupplier>::hasInstance(o)) {
                            return false;
                        }
                        FunctionSupplier const &p = (FunctionSupplier const &) o;
                        return Functional::FunctionUtils<F>::isEquals(func, p.func);
                    }

                    Object &clone() const override {
                        return Unsafe::allocateInstance<FunctionSupplier>(Unsafe::forwardInstance<F>(func));
                    }
                };

                return Unsafe::allocateInstance<FunctionSupplier>(Unsafe::forwardInstance<F>(function));

            }
        };

    } // function
} // core

#endif //CORE23_DOUBLESUPPLIER_H
