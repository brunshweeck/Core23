//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_CONSUMER_H
#define CORE23_CONSUMER_H

#include <core/util/function/Functional.h>
#include <core/ArgumentException.h>

namespace core {
    namespace util {

        /**
         * Represents an operation that accepts a single input argument and returns no
         * result. Unlike most other functional interfaces, Consumer is expected
         * to operate via side-effects.
         *
         * <p>This is a <a href="">functional interface</a>
         * whose functional method is <b style="color: orange;">accept(Object)</b>.
         *
         * @param T the type of the input to the operation
         */
        template<class T>
        class Consumer : public Functional {
        private:
            CORE_STATIC_ASSERT(Class<Object>::isSuper<T>(), "Illegal template type");
            CORE_STATIC_ASSERT(!Class<Void>::isSimilar<T>(), "Illegal parameter type");

            CORE_ALIAS(Param, Params<T>);

            CORE_ALIAS(U, native::Unsafe);

            /**
             * Capture<E> represent all type E that are base of T.
             * in other word E is base of T (Class<E>::isSuper<T>() is true).
             */
            template<class E>
            CORE_ALIAS(Capture, typename Class<E>::template Iff<Class<E>::template isSuper<T>()>);

            interface Launcher : Object {
                virtual void launch(Param) const = 0;

                template<class Fn, Class<gbool>::Iff<Class<Fn>::isFunction() && !Class<Fn>::isClass()> = 1>
                static Launcher &of(Fn &&fn) {

                    class _$ : public Launcher {
                    private:
                        Fn fn;

                    public:
                        CORE_EXPLICIT _$(Fn &&fn) : fn(fn) {}

                        gbool equals(const Object &o) const {
                            return Class<_$>::hasInstance(o) && CORE_CAST(_$ &, o).fn == fn;
                        }

                        Object &clone() const { return U::createInstance<_$>(*this); }

                        void launch(Param p) const { CORE_IGNORE(fn(p)); }
                    };

                    return U::createInstance<_$>(U::forwardInstance<Fn>(fn));
                }

                template<class Callable, Class<gbool>::Iff<
                        !Class<Callable>::isFunction() || Class<Callable>::isClass()> = 1>
                static Launcher &of(Callable &&c) {

                    CORE_ALIAS(_Fn, UnarySign<Callable,, T, void >);
                    CORE_ALIAS(Fn, UnarySign<Callable,, Param, void >);

                    if (!Class<Fn>::template isSimilar<Callable>()) {
                        // simple lambda functions
                        return of(CORE_CAST(Fn, U::forwardInstance<Callable>(c)));
                    }

                    class _$ : public Launcher {
                    private:
                        Callable c;

                    public:
                        CORE_EXPLICIT _$(Callable &&c) : c(U::forwardInstance<Callable>(c)) {}

                        gbool equals(const Object &o) const {
                            return Class<_$>::hasInstance(o) && &CORE_CAST(_$ &, o).c == &c;
                        }

                        Object &clone() const { return U::createInstance<_$>(*this); }

                        void launch(Param p) const { CORE_IGNORE(c(p)); }
                    };

                    return U::createInstance<_$>(U::forwardInstance<Callable>(c));
                }
            };

            CORE_ALIAS(Action, typename Class<Launcher>::Ptr);

            /**
             * The consumer action
             */
            Action act;

        public:
            /**
             * Construct new consumer with callable object (class instances or lambda functions)
             *
             * @param c The callable object
             */
            template<class Callable, Class<gbool>::Iff<Class<Callable>::isFunction() || Class<Callable>::isClass()> = 1>
            CORE_IMPLICIT Consumer(Callable &&c) CORE_NOTHROW : act(0) {
                CORE_STATIC_ASSERT(Class<Callable>::template isCallable<Param>(), "Invalid callable object");
                act = &Launcher::of(U::forwardInstance<Callable>(c));
            }

            /**
             * Construct new consumer with another
             *
             * @param c The other consumer
             */
            Consumer(const Consumer<T> &c) : act(0) { act = &U::copyInstance(*c.act); }

            /**
             * Construct new consumer with another
             *
             * @param c The other consumer
             */
            Consumer(Consumer<T> &&c) CORE_NOTHROW: act(c.act) {
                c.act = null;
            }

            /**
             * Set consumer action  with another consumer action
             *
             * @param c The consumer that it action will be copied to set this consumer action
             */
            Consumer<T> &operator=(const Consumer<T> &c) {
                if (this != &c) {
                    Action act0 = &U::copyInstance(*c.act);
                    U::destroyInstance(*act);
                    act = act0;
                }
                return *this;
            }

            /**
             * Swap consumer action  with another consumer action
             *
             * @param c The consumer that it action will be swapped to set this consumer action
             */
            Consumer<T> &operator=(Consumer<T> &&c) CORE_NOTHROW {
                Action act0 = act;
                act = c.act;
                c.act = act0;
                return *this;
            }

            /**
             * Performs this operation on the given argument.
             *
             * @param p the input argument
             */
            inline void accept(Param p) const { (*act).launch(p); }

            /**
             * Test if this consumer has same action with the specified
             * consumer.
             *
             * @param o The other consumer
             */
            gbool equals(const Object &o) const override {
                if (!Class<Consumer>::hasInstance(o))
                    return false;
                return (*act).equals(*CORE_CAST(const Consumer<T> &, o).act);
            }

            /**
             * Return shadow copy of this consumer
             */
            Object &clone() const override {
                return U::createInstance<Consumer<T>>(*this);
            }

            /**
             * Returns a composed <b>Consumer</b> that performs, in sequence, this
             * operation followed by the <b>after</b> operation. If performing either
             * operation throws an exception, it is relayed to the caller of the
             * composed operation.  If performing this operation throws an exception,
             * the <b>after</b> operation will not be performed.
             *
             * @param after the operation to perform after this operation
             */
            Consumer<T> andThen(const Consumer<T> &after) {
                return [after, *this](Param p) {
                    accept(p);
                    after.accept(p);
                };
            }

            /**
             * Destroy this consumer
             */
            ~Consumer() override {
                U::destroyInstance(*act);
                act = null;
            }

            /**
             * Implicit call operator.
             *
             * @param p The parameter value
             */
            virtual void operator()(Param p) { return accept(p); }

        };

#if CORE_TEMPLATE_TYPE_DEDUCTION
        Consumer() -> Consumer<Object>;

        template<class Return, class Param, class ...Params>
        Consumer (Return(Param, Params...))

        ->
        Consumer<typename Class<Param>::Object>;
#endif

    } // core
} // Functor

#endif //CORE23_CONSUMER_H
