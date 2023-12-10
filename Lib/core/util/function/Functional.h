//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_FUNCTIONAL_H
#define CORE23_FUNCTIONAL_H

#include <core/private/Unsafe.h>
#include <core/Object.h>

namespace core {
    namespace util {

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
            virtual ~Functional() = default;

            CORE_ALIAS(U, native::Unsafe);

            /**
             * Simplified form (signature) of function ( function(Params...) -> Return )
             */
            template<class T>
            CORE_ALIAS(Prim, typename Class<T>::Primitive);
            // 1- Return type (R)
            // objects types -> primitives type
            // objects references -> objects references
            // abstract object -> abstract object reference
            // non-basic objects types -> objects type
            template<class R>
            CORE_ALIAS(Return, typename Class<Prim<R>>::template
                    If<Class<Prim<R>>::isPrimitive() && !Class<R>::isReference(),,
                       typename Class<Prim<R>>::Reference >);
            // 2- Parameters types (P)
            // non-reference type -> const reference type.
            // reference type -> reference type
            template<class P>
            CORE_ALIAS(Params, typename Class<P>::template If<Class<P>::isReference(),, typename Class<P>::CRef >);
            // 3- function signature (F)
            // simple lambda function -> function
            // high lambda function -> high lambda function
            // callable object -> callable object
            //
            template<class F, class R>
            CORE_ALIAS(Sign, typename Class<F>::template If<Class<F>::isFunction(),,
                       typename Class<R()>::template If<Class<F>::template isConvertible<R()>() >>);

            template<class F, class P, class R>
            CORE_ALIAS(UnarySign, typename Class<F>::template If<Class<F>::isFunction(),, typename Class<R(P)>::template
            If<Class<F>::template isConvertible<R(P)>(), typename Class<R(P & )>::template
            If<Class<F>::template isConvertible<R(P & )>(), typename Class<R(P && )>::template
            If<Class<F>::template isConvertible<R(P && )>(), typename Class<R(const P)>::template
            If<Class<F>::template isConvertible<R(const P)>(), typename Class<R(const P &)>::template
            If<Class<F>::template isConvertible<R(const P &)>(), typename Class<R(const P &&)>::template
                    If < Class<F>::template isConvertible<R(const P &&)>(), F >> >>>>>);

            template<class F, class P1, class P2, class R>
            CORE_ALIAS(BinarySign, typename Class<F>::template If<Class<F>::isFunction(),,

                       typename Class<R(P1, P2)>::template If<Class<F>::template isConvertible<R(P1, P2)>(),
                               typename Class<R(P1, P2 & )>::template If<Class<F>::template isConvertible<R(P1,
                                                                                                            P2 & )>(),
                                       typename Class<R(P1, P2 && )>::template If<Class<F>::template isConvertible<R(P1,
                                                                                                                     P2 && )>(),
                               typename Class< R(P1, const P2)>::template If<Class<F>::
                               template isConvertible<R(P1, const P2)>(),
                       typename Class< R(P1, const P2 &)>::template If<Class<F>::
                               template isConvertible<R(P1, const P2&)>(),
                       typename Class< R(P1, const P2 &&)>::template If<Class<F>::
                               template isConvertible<R(P1, const P2&&)>(),

                       typename Class<R(P1 & , P2)>::template If<Class<F>::template isConvertible<R(P1 & , P2)>(),
                               typename Class<R(P1 & , P2 & )>::template If<Class<F>::template isConvertible<R(P1 & ,
                                                                                                               P2 & )>(),
                                       typename Class<R(P1 & , P2 && )>::template If<Class<F>::template isConvertible<R(
                                               P1 & , P2 && )>(),
                               typename Class< R(P1 &, const P2)>::template If<Class<F>::
                               template isConvertible<R(P1&, const P2)>(),
                       typename Class< R(P1 &, const P2 &)>::template If<Class<F>::
                               template isConvertible<R(P1&, const P2&)>(),
                       typename Class< R(P1 &, const P2 &&)>::template If<Class<F>::
                               template isConvertible<R(P1&, const P2&&)>(),

                       typename Class<R(P1 && , P2)>::template If<Class<F>::template isConvertible<R(P1 && , P2)>(),
                               typename Class<R(P1 && , P2 & )>::template If<Class<F>::template isConvertible<R(P1 && ,
                                                                                                                P2 & )>(),
                                       typename Class<R(P1 && ,
                                                        P2 && )>::template If<Class<F>::template isConvertible<R(P1 && ,
                                                                                                                 P2 && )>(),
                               typename Class< R(P1 &&, const P2)>::template If<Class<F>::
                               template isConvertible<R(P1&&, const P2)>(),
                       typename Class< R(P1 &&, const P2 &)>::template If<Class<F>::
                               template isConvertible<R(P1&&, const P2&)>(),
                       typename Class< R(P1 &&, const P2 &&)>::template If<Class<F>::
                               template isConvertible<R(P1&&, const P2&&)>(),

                       typename Class<R(const P1, P2)>::template If<Class<F>::template isConvertible<R(const P1, P2)>(),
                               typename Class<R(const P1, P2 &)>::template If<Class<F>::template isConvertible<R(
                                       const P1, P2 &)>(),
                                       typename Class<R(const P1,
                                                        P2 &&)>::template If<Class<F>::template isConvertible<R(
                                               const P1, P2 &&)>(),
                                               typename Class<R(const P1,
                                                                const P2)>::template If<Class<F>::template isConvertible<R(
                                                       const P1, const P2)>(),
                                                       typename Class<R(const P1,
                                                                        const P2 &)>::template If<Class<F>::template isConvertible<R(
                                                               const P1, const P2 &)>(),
                                                               typename Class<R(const P1,
                                                                                const P2 &&)>::template If<Class<F>::template isConvertible<R(
                                                                       const P1, const P2 &&)>(),

                                                                       typename Class<R(const P1 &,
                                                                                        P2)>::template If<Class<F>::template isConvertible<R(
                                                                               const P1 &, P2)>(),
                                                                               typename Class<R(const P1 &,
                                                                                                P2 &)>::template If<Class<F>::template isConvertible<R(
                                                                                       const P1 &, P2 &)>(),
                                                                                       typename Class<R(const P1 &,
                                                                                                        P2 &&)>::template If<Class<F>::template isConvertible<R(
                                                                                               const P1 &, P2 &&)>(),
                                                                                               typename Class<R(
                                                                                                       const P1 &,
                                                                                                       const P2)>::template If<Class<F>::template isConvertible<R(
                                                                                                       const P1 &,
                                                                                                       const P2)>(),
                                                                                                       typename Class<R(
                                                                                                               const P1 &,
                                                                                                               const P2 &)>::template If<Class<F>::template isConvertible<R(
                                                                                                               const P1 &,
                                                                                                               const P2 &)>(),
                                                                                                               typename Class<R(
                                                                                                                       const P1 &,
                                                                                                                       const P2 &&)>::template If<Class<F>::template isConvertible<R(
                                                                                                                       const P1 &,
                                                                                                                       const P2 &&)>(),

                                                                                                                       typename Class<R(
                                                                                                                               const P1 &&,
                                                                                                                               P2)>::template If<Class<F>::template isConvertible<R(
                                                                                                                               const P1 &&,
                                                                                                                               P2)>(),
                                                                                                                               typename Class<R(
                                                                                                                                       const P1 &&,
                                                                                                                                       P2 &)>::template If<Class<F>::template isConvertible<R(
                                                                                                                                       const P1 &&,
                                                                                                                                       P2 &)>(),
                                                                                                                                       typename Class<R(
                                                                                                                                               const P1 &&,
                                                                                                                                               P2 &&)>::template If<Class<F>::template isConvertible<R(
                                                                                                                                               const P1 &&,
                                                                                                                                               P2 &&)>(),
                                                                                                                                               typename Class<R(
                                                                                                                                                       const P1 &&,
                                                                                                                                                       const P2)>::template If<Class<F>::template isConvertible<R(
                                                                                                                                                       const P1 &&,
                                                                                                                                                       const P2)>(),
                                                                                                                                                       typename Class<R(
                                                                                                                                                               const P1 &&,
                                                                                                                                                               const P2 &)>::template If<Class<F>::template isConvertible<R(
                                                                                                                                                               const P1 &&,
                                                                                                                                                               const P2 &)>(),
                                                                                                                                                               typename Class<R(
                                                                                                                                                                       const P1 &&,
                                                                                                                                                                       const P2 &&)>::template If<Class<F>::template isConvertible<R(
                                                                                                                                                                       const P1 &&,
                                                                                                                                                                       const P2 &&)>(),

                               F >> >>>>> >>>>>> >>>>>> >>>>>> >>>>>> >>>>>>);

        };
    } // core
} // util
#endif //CORE23_FUNCTIONAL_H
