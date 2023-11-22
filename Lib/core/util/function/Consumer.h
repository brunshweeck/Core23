//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_CONSUMER_H
#define CORE23_CONSUMER_H

#include "Functional.h"

namespace core {
    namespace Function {

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
        class Consumer : public UnaryFunction<T, Void> {
        private:
            CORE_STATIC_ASSERT(!Class<Void>::isSimilar<T>(), "Consumer not accept void types as parameter");

            /**
             * The simplified parameter type
             *
             * @see UnaryFunction.TS
             */
            CORE_ALIAS(Arg, typename UnaryFunction<T,, Void > ::TS);

            /**
             * The simplified form of lambda function (ln -> fn)
             *
             * @see UnaryFunction.Fn
             */
            template<class Ln, class RS, class TS>
            CORE_ALIAS(Lambda, typename UnaryFunction<T,, Void > ::template Fn<Ln, RS, Class<TS>::NCRef>);

            interface Caller : public virtual Object {
                virtual void call() const = 0;
            };

            template<class Fn>
            class FnCaller : public virtual Caller {
            };

            template<class Ln>
            class LnCaller : public virtual Caller {
            };

            template<class Mn, class X>
            class MnCaller : public virtual Caller {
            };

            glong handle;

        public:
            template<class Callable,
                    Class<gbool>::Iff<Class<Callable>::template isCallable<Arg>()> = 1,
                    Class<gbool>::Iff<Class<Callable>::isFunctionMember()> = 0>
            Consumer(Callable &&c) {
                CORE_ALIAS(RS, typename Class<Callable>::template Return<Arg>);
                CORE_ALIAS(Ln, Lambda<Callable,, RS, T >);
                CORE_ALIAS(X, Lambda<Ln,, RS, Arg >);
                CORE_ALIAS(XCaller, Class<LnCaller<X>>::template
                        If<Class<X>::template isSimilar<Callable>(),, FnCaller<X>>);

                if (Class<XCaller>::template isSimilar<FnCaller>()) {}

                handle = (glong) new XCaller((Callable &&) c);
            }

        private:
            /**
             * Construct new Consumer with method handle, and instance used
             * to call given method.
             *
             * @tparam Method The method signature
             * @tparam X The type of object used to call given method
             *
             * @param m The method handle
             * @param o The object reference used to call given method
             */
            template<class Method, class X>
            CORE_EXPLICIT Consumer(Method &&m, X &&o) {
                handle = (glong) new MnCaller<Method, X>();
            }

        public:

            /**
             * Initialize new Consumer with another
             *
             * @param c The other consumer
             */
            Consumer(const Consumer &c) {
                handle = 0;
            }

            /**
             * Initialize new Consumer with another
             *
             * @param c The other consumer
             */
            Consumer(Consumer &&c) CORE_NOTHROW {
                glong handle0 = handle;
                handle = c.handle;
                c.handle = handle0;
            }

            /**
             * Initialize new Consumer with another
             *
             * @param c The other consumer
             */
            Consumer &operator=(const Consumer &c) {
                if(c.handle != handle) {
//                    handle = &((Caller &) Unsafe.getObject(c.handle)).clone();
                }
                return *this;
            }

            /**
             * Initialize new Consumer with another
             *
             * @param c The other consumer
             */
            Consumer &operator=(Consumer &&c) CORE_NOTHROW {
                glong handle0 = handle;
                handle = c.handle;
                c.handle = handle0;
                return *this;
            }

            /**
             * Accept the given argument and execute action
             * @param arg object to be accepted
             */
            virtual void accept(Arg arg) const {
                // ((Caller &) Unsafe::getObject(handle)).call();
            }

            /**
             * Return true iff specified object is a Consumer instance
             * and has same properties as this Consumer.
             *
             * @param object The object to be compared
             */
            gbool equals(const Object &object) const override {
                if(!Class<Consumer>::hasInstance(object))
                    return false;
//                return ((Caller &) Unsafe::getObject(handle)).equals(
//                        ((Caller &) Unsafe::getObject(CORE_DYN_CAST(Consumer &, object))));
            }

            /**
             * Construct new Consumer with method handle, and instance used
             * to call given method.
             *
             * @tparam Method The method signature
             * @tparam X The type of object used to call given method
             *
             * @param m The method handle
             * @param o The object reference used to call given method
             */
            template<class Method, class X>
            static Consumer fromMethod(Method &&m, X &&obj) {
                /**
                 * Check If specified method is callable with this consumer parameter by the given object:
                 * <code> obj.method(parameter) </code>
                 */
                CORE_STATIC_ASSERT((Class<Method>::template isCallable<X, T>()),
                                   "Could not call given method with given object and consumer parameter");
                return Consumer((Method &&) m, (X &&) obj);
            }

        };

    } // core
} // Functor

#endif //CORE23_CONSUMER_H
