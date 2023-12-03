//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_SUPPLIER_H
#define CORE23_SUPPLIER_H

#include "Functional.h"

namespace core {
    namespace util {

        /**
         * Represents a supplier of results.
         *
         * <p>There is no requirement that a new or distinct result be returned each
         * time the supplier is invoked.
         *
         * <p>This is a <a href="">functional interface</a>
         * whose functional method is <b style="color: orange;">get()</b>.
         *
         * @param T the type of results supplied by this supplier
         */
        template<class R>
        class Supplier : Functional {
        private:
            CORE_STATIC_ASSERT(Class<Object>::isSuper<R>(), "Illegal template type");
            CORE_STATIC_ASSERT(!Class<Void>::isSimilar<R>(), "Illegal Return type");

            CORE_ALIAS(Ret, Return < R >);

            interface Result : public Object {
                virtual Ret show() const = 0;

                template<class Fn, Class<gbool>::Iff<Class<Fn>::isFunction() && !Class<Fn>::isClass()> = 1>
                static Result &of(Fn &&fn) {
                    static native::Unsafe &U = native::Unsafe::U;

                    class _$ : public Result {
                    private:
                        Fn fn;

                    public:
                        CORE_EXPLICIT _$(Fn &&fn) : fn(fn) {}

                        gbool equals(const Object &o) const {
                            return !Class<_$>::hasInstance(o) ? false : CORE_CAST(_$ &, o).fn == fn;
                        }

                        Object &clone() const { return U.createInstance<_$>(*this); }

                        Ret show() const { return fn(); }
                    };

                    return U.createInstance<_$>(U.forwardInstance<Fn>(fn));
                }

                template<class Callable, Class<gbool>::Iff<
                        !Class<Callable>::isFunction() || Class<Callable>::isClass()> = 1>
                static Result &of(Callable &&c) {

                    static native::Unsafe &U = native::Unsafe::U;

                    CORE_ALIAS(_R, Class<Callable>::template Return<>);
                    CORE_ALIAS(_Fn, Sign<Callable,, _R >);
                    CORE_ALIAS(Fn, Sign<_Fn,, _R >);

                    if (!Class<Fn>::template isSimilar<Callable>()) {
                        // simple lambda functions
                        return of(CORE_CAST(Fn, U.forwardInstance<Callable>(c)));
                    }

                    class _$ : public Result {
                    private:
                        Callable c;

                    public:
                        CORE_EXPLICIT _$(Callable &&c) : c(U.forwardInstance<Callable>(c)) {}

                        gbool equals(const Object &o) const {
                            return !Class<_$>::hasInstance(o) ? false : &CORE_CAST(_$ &, o).c == &c;
                        }

                        Object &clone() const { return U.createInstance<_$>(*this); }

                        Ret show() const { return c(); }
                    };

                    return U.createInstance<_$>(U.forwardInstance<Callable>(c));
                }
            };


            CORE_ALIAS(Eval, typename Class<Result>::Ptr);

            /**
             * The Supplier result
             */
            Eval eval;

        public:
            /**
             * Construct new supplier with callable object (class instances or lambda functions)
             *
             * @param c The callable object
             */
            template<class Callable, Class<gbool>::Iff<Class<Callable>::isFunction() || Class<Callable>::isClass()> = 1>
            CORE_IMPLICIT Supplier(Callable &&c) CORE_NOTHROW : eval(0) {
                CORE_STATIC_ASSERT(Class<Callable>::isCallable(), "Invalid callable object");
                CORE_ALIAS(_Ret, typename Class<Callable>::template Return<>);
                CORE_STATIC_ASSERT(Class<_Ret>::template isConvertible<Ret>(), "Incompatible return types");
                eval = &Result::of(native::Unsafe::forwardInstance<Callable>(c));
            }

            /**
             * Construct new supplier with another
             *
             * @param c The other supplier
             */
            Supplier(const Supplier<R> &c) : eval(0) {
                eval = &native::Unsafe::U.copyInstance(*c.eval);
            }

            /**
             * Construct new supplier with another
             *
             * @param c The other supplier
             */
            Supplier(Supplier<R> &&c) CORE_NOTHROW: eval(0) {
                eval = c.eval;
                c.eval = null;
            }

            /**
             * Set supplier eval  with another supplier eval
             *
             * @param c The supplier that it eval will be copied to set this supplier eval
             */
            Supplier<R> &operator=(const Supplier<R> &c) {
                if (this != &c) {
                    Eval eval0 = &native::Unsafe::U.copyInstance(*c.eval);
                    native::Unsafe::U.destroyInstance(*eval);
                    eval = eval0;
                }
                return *this;
            }

            /**
             * Swap supplier eval  with another supplier eval
             *
             * @param c The supplier that it eval will be swapped to set this supplier eval
             */
            Supplier<R> &operator=(Supplier<R> &&c) CORE_NOTHROW {
                Eval eval0 = eval;
                eval = c.eval;
                c.eval = eval0;
                return *this;
            }

            /**
             * Gets a result.
             *
             * @return a result
             */
            inline Ret get() const { (*eval).show(); }

            /**
             * Test if this supplier has same eval with the specified
             * supplier.
             *
             * @param o The other supplier
             */
            gbool equals(const Object &o) const override {
                if (!Class<Supplier>::hasInstance(o))
                    return false;
                return (*eval).equals(*CORE_CAST(const Supplier<R> &, o).eval);
            }

            /**
             * Return shadow copy of this supplier
             */
            Object &clone() const override {
                return native::Unsafe::U.createInstance<Supplier<R>>(*this);
            }
        };

#if CORE_TEMPLATE_TYPE_DEDUCTION
        Supplier() -> Supplier<Object>;
        template<class R, class ...Params> Supplier(R(Params...)) -> Supplier<typename Class<R>::Object>;
#endif
    }
} // core

#endif //CORE23_SUPPLIER_H
