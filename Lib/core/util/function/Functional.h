//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_FUNCTIONAL_H
#define CORE23_FUNCTIONAL_H

#include <core/Object.h>

namespace core {
    namespace Function {

        /**
         * An informative interface type used to indicate that an interface
         * type declaration is intended to be a <i>functional interface</i> as
         * defined by the core library Specification.
         *
         * Conceptually, a functional interface has exactly one abstract
         * method.  If an interface declares an abstract method overriding one of the
         * public methods of Object, that also does <em>not</em> count toward the interface's
         * abstract method count since any implementation of the interface will have an
         * implementation from Object or elsewhere.
         *
         * <p>
         * Note that instances of functional interfaces can be created with
         * lambda expressions, method references, or constructor references.
         *
         */
        interface Functional : Object {
        protected:
            /**
             * Construct new Functional Object
             */
            CORE_FAST Functional() {};
        };

        template<class R>
        interface ZeroFunction : Functional {
            CORE_STATIC_ASSERT(Class<Object>::isSuper<R>(), "Illegal Template type");
            CORE_STATIC_ASSERT(!Class<R>::isConstant(), "Return type must be not const");
            CORE_STATIC_ASSERT(!Class<R>::isVolatile(), "Return type must be not volatile");

        protected:
            /**
             * Simplified form of Return type (R -> RS)
             * Integer -> gint (non-reference to primitive)
             * Integer& -> Integer& (no effect)
             * AbstractClass -> AbstractClass&
             */
            CORE_ALIAS(RP, typename Class<R>::Primitive);
            CORE_ALIAS(RS, Class<RP>::template If<Class<RP>::isPrimitive() && !Class<R>::isReference(),,
                       Class<RP>::Reference >);

            /**
             * Simplified form of Lambda function
             * R(*)() -> R(*)()
             * []() -> R ==> R(*)()
             * [...]() -> R ==> [...]() -> R
             */
            template<class Ln, class RS>
            CORE_ALIAS(Fn, typename Class<Ln>::template If<Class<Ln>::isFunction(),,
                       typename Class<RS()>::template If<Class<Ln>::template isConvertible<RS()>(), Ln>>);

            CORE_FAST ZeroFunction() {}

        public:
            virtual RS operator()() const = 0;
        };

        template<class T, class R>
        interface UnaryFunction : Functional {
            CORE_STATIC_ASSERT(Class<Object>::isSuper<T>(), "Illegal Parameter type");
            CORE_STATIC_ASSERT(!Class<Void>::isSimilar<T>(), "Parameter type must be non-void");
            CORE_STATIC_ASSERT(!Class<T>::isVolatile(), "Functional interface not Support volatile parameter");

            CORE_STATIC_ASSERT(Class<Object>::isSuper<R>(), "Illegal Return type");
            CORE_STATIC_ASSERT(!Class<R>::isConstant(), "Return type must be explicitly not const");
            CORE_STATIC_ASSERT(!Class<R>::isVolatile(), "Functional interface not Support volatile parameter");

        protected:
            /**
             * Simplified form of Return type (R -> RS)
             * Integer -> gint (non-reference to primitive)
             * Integer& -> Integer& (no effect)
             * AbstractClass -> AbstractClass&
             */
            CORE_ALIAS(RP, typename Class<R>::Primitive);
            CORE_ALIAS(RS, Class<RP>::template If<Class<RP>::isPrimitive() && !Class<R>::isReference(),,
                       Class<RP>::Reference >);

            /**
             * Simplified form of Lambda function (Ln -> Fn)
             * R(*)(T) -> R(*)(T)
             * [](T) -> R ==> R(*)(T)
             * [...](T) -> R ==> [...](T) -> R
             */
            template<class Ln, class RS, class TS>
            CORE_ALIAS(Fn, typename Class<Ln>::template If<Class<Ln>::isFunction(),, typename Class<RS(TS)>::template
            If<Class<Ln>::template isConvertible<RS(TS)>(), typename Class<RS(TS & )>::template
            If<Class<Ln>::template isConvertible<RS(TS & )>(), typename Class<RS(TS && )>::template
            If<Class<Ln>::template isConvertible<RS(TS && )>(), typename Class<RS(const TS)>::template
            If<Class<Ln>::template isConvertible<RS(const TS)>(), typename Class<RS(const TS &)>::template
            If<Class<Ln>::template isConvertible<RS(const TS &)>(), typename Class<RS(const TS &&)>::template
                    If < Class<Ln>::template isConvertible<RS(const TS &&)>(), Ln >> >>>>>);

            /**
             * Simplified form of Parameter type (T -> TS)
             * Integer -> gint
             * Integer -> Integer&
             * AbstractClass -> const AbstractClass&
             */
            CORE_ALIAS(TP, typename Class<T>::Primitive);
            CORE_ALIAS(TS, Class<TP>::template If<Class<TP>::isPrimitive() && !Class<T>::isReference(),,
                       Class<TP>::Reference >);

            CORE_FAST UnaryFunction() {}

        public:
            virtual RS operator()(TS t) const = 0;
        };

        template<class T, class U, class R>
        interface BinaryFunction : Functional {

            CORE_STATIC_ASSERT(Class<Object>::isSuper<T>(), "Illegal First Parameter type");
            CORE_STATIC_ASSERT(!Class<Void>::isSimilar<T>(), "First Parameter type must be non-void");
            CORE_STATIC_ASSERT(!Class<T>::isVolatile(), "Functional interface not Support volatile parameter");

            CORE_STATIC_ASSERT(Class<Object>::isSuper<U>(), "Illegal Second Parameter type");
            CORE_STATIC_ASSERT(!Class<Void>::isSimilar<U>(), "Second Parameter type must be non-void");
            CORE_STATIC_ASSERT(!Class<U>::isVolatile(), "Functional interface not Support volatile parameter");

            CORE_STATIC_ASSERT(Class<Object>::isSuper<R>(), "Illegal Return type");
            CORE_STATIC_ASSERT(!Class<R>::isConstant(), "Return type must be explicitly not const");
            CORE_STATIC_ASSERT(!Class<R>::isVolatile(), "Functional interface not Support volatile parameter");

        protected:
            /**
             * Simplified form of Return type (R -> RS)
             * Integer -> gint (non-reference to primitive)
             * Integer& -> Integer& (no effect)
             * AbstractClass -> AbstractClass&
             */
            CORE_ALIAS(RP, typename Class<R>::Primitive);
            CORE_ALIAS(RS, Class<RP>::template If<Class<RP>::isPrimitive() && !Class<R>::isReference(),,
                       Class<RP>::Reference >);

            /**
             * Simplified form of Lambda function (Ln -> Fn)
             * R(*)(T) -> R(*)(T)
             * [](T) -> R ==> R(*)(T)
             * [...](T) -> R ==> [...](T) -> R
             */
            template<class Ln, class RS, class TS, class US>
            CORE_ALIAS(Fn, typename Class<Ln>::template If<Class<Ln>::isFunction(),,
                       
                       typename Class<RS(TS, US)>::template If<Class<Ln>::template isConvertible<RS(TS, US)>(),
                       typename Class<RS(TS, US&)>::template If<Class<Ln>::template isConvertible<RS(TS, US&)>(),
                       typename Class<RS(TS, US&&)>::template If<Class<Ln>::template isConvertible<RS(TS, US&&)>(),
                       typename Class<RS(TS, const US)>::template If<Class<Ln>::template isConvertible<RS(TS, const US)>(),
                       typename Class<RS(TS, const US&)>::template If<Class<Ln>::template isConvertible<RS(TS, const US&)>(),
                       typename Class<RS(TS, const US&&)>::template If<Class<Ln>::template isConvertible<RS(TS, const US&&)>(),

                       typename Class<RS(TS&, US)>::template If<Class<Ln>::template isConvertible<RS(TS&, US)>(),
                       typename Class<RS(TS&, US&)>::template If<Class<Ln>::template isConvertible<RS(TS&, US&)>(),
                       typename Class<RS(TS&, US&&)>::template If<Class<Ln>::template isConvertible<RS(TS&, US&&)>(),
                       typename Class<RS(TS&, const US)>::template If<Class<Ln>::template isConvertible<RS(TS&, const US)>(),
                       typename Class<RS(TS&, const US&)>::template If<Class<Ln>::template isConvertible<RS(TS&, const US&)>(),
                       typename Class<RS(TS&, const US&&)>::template If<Class<Ln>::template isConvertible<RS(TS&, const US&&)>(),

                       typename Class<RS(TS&&, US)>::template If<Class<Ln>::template isConvertible<RS(TS&&, US)>(),
                       typename Class<RS(TS&&, US&)>::template If<Class<Ln>::template isConvertible<RS(TS&&, US&)>(),
                       typename Class<RS(TS&&, US&&)>::template If<Class<Ln>::template isConvertible<RS(TS&&, US&&)>(),
                       typename Class<RS(TS&&, const US)>::template If<Class<Ln>::template isConvertible<RS(TS&&, const US)>(),
                       typename Class<RS(TS&&, const US&)>::template If<Class<Ln>::template isConvertible<RS(TS&&, const US&)>(),
                       typename Class<RS(TS&&, const US&&)>::template If<Class<Ln>::template isConvertible<RS(TS&&, const US&&)>(),
                       
                       typename Class<RS(const TS, US)>::template If<Class<Ln>::template isConvertible<RS(const TS, US)>(),
                       typename Class<RS(const TS, US&)>::template If<Class<Ln>::template isConvertible<RS(const TS, US&)>(),
                       typename Class<RS(const TS, US&&)>::template If<Class<Ln>::template isConvertible<RS(const TS, US&&)>(),
                       typename Class<RS(const TS, const US)>::template If<Class<Ln>::template isConvertible<RS(const TS, const US)>(),
                       typename Class<RS(const TS, const US&)>::template If<Class<Ln>::template isConvertible<RS(const TS, const US&)>(),
                       typename Class<RS(const TS, const US&&)>::template If<Class<Ln>::template isConvertible<RS(const TS, const US&&)>(),

                       typename Class<RS(const TS&, US)>::template If<Class<Ln>::template isConvertible<RS(const TS&, US)>(),
                       typename Class<RS(const TS&, US&)>::template If<Class<Ln>::template isConvertible<RS(const TS&, US&)>(),
                       typename Class<RS(const TS&, US&&)>::template If<Class<Ln>::template isConvertible<RS(const TS&, US&&)>(),
                       typename Class<RS(const TS&, const US)>::template If<Class<Ln>::template isConvertible<RS(const TS&, const US)>(),
                       typename Class<RS(const TS&, const US&)>::template If<Class<Ln>::template isConvertible<RS(const TS&, const US&)>(),
                       typename Class<RS(const TS&, const US&&)>::template If<Class<Ln>::template isConvertible<RS(const TS&, const US&&)>(),

                       typename Class<RS(const TS&&, US)>::template If<Class<Ln>::template isConvertible<RS(const TS&&, US)>(),
                       typename Class<RS(const TS&&, US&)>::template If<Class<Ln>::template isConvertible<RS(const TS&&, US&)>(),
                       typename Class<RS(const TS&&, US&&)>::template If<Class<Ln>::template isConvertible<RS(const TS&&, US&&)>(),
                       typename Class<RS(const TS&&, const US)>::template If<Class<Ln>::template isConvertible<RS(const TS&&, const US)>(),
                       typename Class<RS(const TS&&, const US&)>::template If<Class<Ln>::template isConvertible<RS(const TS&&, const US&)>(),
                       typename Class<RS(const TS&&, const US&&)>::template If<Class<Ln>::template isConvertible<RS(const TS&&, const US&&)>(),

                               Ln >>>>>>> >>>>>> >>>>>> >>>>>> >>>>>> >>>>>>);

            /**
             * Simplified form of Parameter type (T -> TS), (U -> US)
             * Integer -> gint
             * Integer -> Integer&
             * AbstractClass -> const AbstractClass&
             */
            CORE_ALIAS(TP, typename Class<T>::Primitive);
            CORE_ALIAS(TS, Class<TP>::template If<Class<TP>::isPrimitive() && !Class<T>::isReference(),,
                       Class<TP>::Reference >);

            CORE_ALIAS(UP, typename Class<U>::Primitive);
            CORE_ALIAS(US, Class<UP>::template If<Class<UP>::isPrimitive() && !Class<U>::isReference(),,
                       Class<UP>::Reference >);

            CORE_FAST BinaryFunction() {}

        public:
            virtual RS operator()(TS t, US u) const = 0;
        };

    } // core

    using namespace Function;
} // Functor

#endif //CORE23_FUNCTIONAL_H
