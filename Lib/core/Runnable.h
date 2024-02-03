//
// Created by T.N.Brunshweeck on 31/01/2024.
//

#ifndef CORE23_RUNNABLE_H
#define CORE23_RUNNABLE_H

#include <core/function/Functional.h>

namespace core {

    /**
     * Represents an operation that does not return a result.
     *
     * <p> This is a <b style="color: green;"> functional interface</b>
     * whose functional method is <b style="color: orange;"> Runnable::run()</b>.
     *
     * @author  Brunshweeck Tazeussong
     * @see     core.concurrent.Callable
     */
    class Runnable: public function::Functional {
    public:
        /**
         * Runs this operation.
         */
         virtual void run() const = 0;

        /**
         * Obtain new runnable from given class function member and specified compatible class instance
         *
         * @param instance The object used to invoke specified function member
         * @param method The internal function member used by returned runnable
         *
         * @tparam I The type of object instance used to call given method
         * @tparam M The type of method handle
         */
        template<class I, class M>
        static Runnable &from(I &&instance, M &&method) {
            // check if given method is valid
            try {
                Functional::CheckFunction<M, I>();
                Functional::FunctionUtils<M>::validate(method);
            } catch (IllegalArgumentException const &ex) {
                ex.throws(__trace(u"core.function.Runnable"_S));
            }

            class MethodHandle : public Runnable {
            private:
                I &&inst;
                M &&meth;

            public:
                CORE_EXPLICIT MethodHandle(I &&instance, M &&method) :
                        inst((I &&) instance), meth(Unsafe::forwardInstance<M>(method)) {}

                /**
                 * Invoke the method of this runnable
                 */
                void invoke() const {
                    CORE_IGNORE((inst.*meth)());
                }

                void run() const override {
                    invoke();
                }

                /**
                 * two instances are equals iff have same methods
                 * and same instances.
                 */
                gbool equals(const Object &o) const override {
                    if (this == &o) {
                        return true;
                    }
                    if (!Class<MethodHandle>::hasInstance(o)) {
                        return false;
                    }
                    MethodHandle const &p = (MethodHandle const &) o;
                    return Functional::FunctionUtils<I>::isEquals(inst, p.inst) &&
                           Functional::FunctionUtils<M>::isEquals(meth, p.meth);
                }

                Object &clone() const override {
                    return Unsafe::allocateInstance<MethodHandle>(Unsafe::forwardInstance<I>(inst),
                                                                    Unsafe::forwardInstance<M>(meth));
                }
            };

            return Unsafe::allocateInstance<MethodHandle>(Unsafe::forwardInstance<I>(instance),
                                                            Unsafe::forwardInstance<M>(method));
        }

        /**
         * Obtain new runnable from given function (classic function/ lambda function)
         *
         * @param function the internal function used by returned runnable
         *
         * @tparam F The type of function supporting runnable arguments.
         */
        template<class F>
        static Runnable &from(F &&function) {
            // check if given function is valid
            try {
                Functional::CheckFunction<F>();
                Functional::FunctionUtils<F>::validate(function);
            } catch (IllegalArgumentException const &ex) {
                ex.throws(__trace(u"core.function.Runnable"_S));
            }

            class FunctionHandle CORE_FINAL : public Runnable {
            private:
                F &&func;

            public:
                CORE_EXPLICIT FunctionHandle(F &&func) : func(Unsafe::forwardInstance<F>(func)) {}

                void invoke() const {
                    return CORE_IGNORE(func());
                }

                void run() const override {
                    invoke();
                }

                gbool equals(const Object &o) const override {
                    if (this == &o) {
                        return true;
                    }
                    if (!Class<FunctionHandle>::hasInstance(o)) {
                        return false;
                    }
                    FunctionHandle const &p = (FunctionHandle const &) o;
                    return Functional::FunctionUtils<F>::isEquals(func, p.func);
                }

                Object &clone() const override {
                    return Unsafe::allocateInstance<FunctionHandle>(Unsafe::forwardInstance<F>(func));
                }
            };

            return Unsafe::allocateInstance<FunctionHandle>(Unsafe::forwardInstance<F>(function));

        }
    };

} // core

#endif //CORE23_RUNNABLE_H
