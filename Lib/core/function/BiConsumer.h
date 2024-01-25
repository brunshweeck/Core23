//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_BICONSUMER_H
#define CORE23_BICONSUMER_H

#include <core/function/Functional.h>
#include <core/private/Unsafe.h>

namespace core {
    namespace function {

        template<class T, class U>
        class BiConsumer : public Functional {
        private:

            CORE_ALIAS(X, Params < T >);
            CORE_ALIAS(Y, Params < U >);
            CORE_ALIAS(Unsafe, native::Unsafe);

            class MethodHandle : public Object {
            public:
                /**
                 * Used to invoke function
                 */
                virtual void invoke(X x, Y y) const = 0;

                gbool equals(const Object &o) const override = 0;

                Object &clone() const override = 0;

            private:

                /**
                 * load function by address
                 */
                template<class Callable,
                        Class<gbool>::OnlyIf<Class<Callable>::isFunction() && !Class<Callable>::isClass()> = true>
                static MethodHandle &load(Callable f) {
                    CORE_ALIAS(Signature, , Callable);

                    // class used to load simple function
                    class FunctionHandle CORE_FINAL : public MethodHandle {
                    private:
                        glong handle;

                    public:
                        CORE_EXPLICIT FunctionHandle(glong handle) : handle(handle) {}

                        gbool equals(const Object &o) const {
                            if (this == &o)
                                return true;
                            if (Class<FunctionHandle>::hasInstance(o))
                                return false;
                            return handle == ((FunctionHandle const &) o).handle;
                        }

                        Object &clone() const {
                            return Unsafe::allocateInstance<FunctionHandle>(handle);
                        }

                        void invoke(X x, Y y) const override {
                            CORE_IGNORE(CORE_CAST(Signature, handle)(x, y));
                        }

                        String toString() const {
                            return "Function@" + Long::toHexString(handle).toUpperCase();
                        }

                        gint hash() const {
                            return Long::hash(handle);
                        }
                    };

                    glong const handle = CORE_CAST(glong, f);
                    if (handle == 0)
                        IllegalArgumentException("Illegal function address")
                                .throws(__trace("core.function.BiConsumer.MethodHandle.load$FunctionHandle"));

                    return Unsafe::allocateInstance<FunctionHandle>(handle);
                }

                /**
                 * load function by reference (lambdas functions and callables object)
                 */
                template<class Callable,
                        Class<gbool>::OnlyIf<Class<Callable>::isClass()> = true>
                static MethodHandle &load(Callable &&obj) {
                    if (Class<Callable>::isFunction()) {
                        // lambda function without capturing
                        CORE_ALIAS(LambdaSignature, , typename Class<Callable>::NRef);
                        // try convert lambda reference to function address
                        CORE_ALIAS(LambdaAsFunction, , BinarySign < Callable, T, U, void >);
                        CORE_ALIAS(FunctionSignature, , BinarySign < LambdaAsFunction, X, Y, void >);
                        if (!Class<FunctionSignature>::template isSimilar<LambdaSignature>()) {
                            return load(CORE_CAST(FunctionSignature, obj));
                        }
                    }

                    CORE_ALIAS(Signature, typename Class<Callable>::NRef);
                    CORE_ALIAS(Signature2, typename Class<Signature>::Ptr);
                    // lambda function or callable object
                    class CallableHandle CORE_FINAL : public MethodHandle {
                    private:
                        glong handle;

                    public:
                        CORE_EXPLICIT CallableHandle(glong handle) : handle(handle) {}

                        void invoke(X x, Y y) const override {
                            CORE_IGNORE((*CORE_CAST(Signature2, handle))(x, y));
                        }

                        gbool equals(const Object &o) const {
                            if (this == &o)
                                return true;
                            if (Class<CallableHandle>::hasInstance(o))
                                return false;
                            return handle == ((CallableHandle const &) o).handle;
                        }

                        Object &clone() const {
                            return Unsafe::allocateInstance<CallableHandle>(handle);
                        }

                        String toString() const {
                            return "Callable@" + Long::toHexString(handle).toUpperCase();
                        }

                        gint hash() const {
                            return Long::hash(handle);
                        }
                    };

                    glong const handle = CORE_CAST(glong, &obj);

                    return Unsafe::allocateInstance<CallableHandle>(handle);
                }

                /**
                 * load class function member
                 */
                template<class MethodMember, class This,
                        Class<gbool>::OnlyIf<Class<MethodMember>::isFunctionMember()> = true>
                static MethodHandle &load(MethodMember member, This& obj) {

                    class MethodMemberHandle: public MethodHandle {
                    private:
                        glong $this;
                        MethodMember method;

                    public:
                        MethodMemberHandle(glong $this, MethodMember method) : $this($this), method(method) {}

                        void invoke(X x, Y y) const override {
                            CORE_IGNORE((CORE_CAST(typename Class<This>::NCRef*, $this)->*method)(x, y));
                        }

                        gbool equals(const Object &o) const {
                            if(this == &o)
                                return true;
                            if(!Class<MethodMemberHandle>::hasInstance(o))
                                return false;
                            MethodMemberHandle const& m = (MethodMemberHandle const&) o;
                            return $this == m.$this && method == m.method;
                        }

                        Object &clone() const {
                            return Unsafe::allocateInstance<MethodMemberHandle>($this, method);
                        }
                    };

                    glong $this = CORE_CAST(glong, &obj);

                    return Unsafe::allocateInstance<MethodMemberHandle>($this, member);
                };
            };

            CORE_ALIAS(Handle, typename Class<MethodHandle>::Pointer);

            Handle handle;

        public:
        };

    } // core
} // Functor

#endif //CORE23_BICONSUMER_H
