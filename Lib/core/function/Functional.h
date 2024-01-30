//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_FUNCTIONAL_H
#define CORE23_FUNCTIONAL_H

#include <core/private/Unsafe.h>
#include <core/Object.h>

namespace core {
    namespace function {

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
        class Functional : public Object {
        private:

            /**
             * Simplified form (signature) of function ( function(Params...) -> Return )
             */
            template<class T>
            CORE_ALIAS(Prim, typename Class<T>::Primitive);

        protected:
            template<class DesiredReturn, class ExpectedReturn>
            class CheckReturn CORE_FINAL {
                CORE_STATIC_ASSERT(
                        Class<ExpectedReturn>::template isSimilar<DesiredReturn>() ||
                        Class<ExpectedReturn>::template isConvertible<DesiredReturn>() ||
                        Class<ExpectedReturn>::isReference() && Class<DesiredReturn>::isReference() &&
                        Class<ExpectedReturn>::template isSuper<DesiredReturn>(),
                        "The given function return type is not compatible with this Functional return type"
                );
            };

            template<class ExpectedCallable, class... Args>
            class CheckFunction CORE_FINAL {
                CORE_STATIC_ASSERT(
                        Class<ExpectedCallable>::isFunction() ||
                        Class<Exception>::isFunctionMember() ||
                        Class<Exception>::isClass(),
                        "Could not resolve function signature"
                );
                CORE_STATIC_ASSERT(
                        Class<ExpectedCallable>::template isCallable<Args...>(),
                        "The given function signature cannot accept this Functional arguments"
                );
            };

            template<class ExpectedFunction,
                    gbool isPointer = Class<ExpectedFunction>::isPointer(),
                    gbool supportEQ = isPointer || Class<ExpectedFunction>::supportEQ()>
            class FunctionUtils CORE_FINAL {
            public:
                static void validate(const ExpectedFunction &f) {
                    if (!(f == null))
                        return;
                    IllegalArgumentException("Could not access to function address.")
                            .throws(__trace("core.function.FunctionUtils"));
                }

                static gbool isEquals(const ExpectedFunction& f1, const ExpectedFunction& f2) {
                    return f1 == f2;
                }
            };

            template<class ExpectedFunction>
            class FunctionUtils<ExpectedFunction, false, true> CORE_FINAL {
            public:
                static void validate(const ExpectedFunction &f) {
                    CORE_IGNORE(f);
                }

                static gbool isEquals(const ExpectedFunction& f1, const ExpectedFunction& f2) {
                    if(&f1 == &f2){
                        return true;
                    }
                    return f1 == f2;
                }
            };

            template<class ExpectedFunction>
            class FunctionUtils<ExpectedFunction, false, false> CORE_FINAL {
            public:
                static void validate(const ExpectedFunction &f) {
                    CORE_IGNORE(f);
                }

                static gbool isEquals(const ExpectedFunction& f1, const ExpectedFunction& f2) {
                    return &f1 == &f2;
                }
            };


        public:
            /**
             * <h2> Function Return type </h2>
             * <p> Transform function argument type to easy form
             * <li> Numbers, Characters, Booleans and Voids types to primitive from (ex: Integer to gint; Void to void)
             * <li> Object types:
             * <ol><li> Abstract type to Reference form (with '&' qualifier)
             *     <li> without '&' qualifier (has no effect)
             *     <li> with '&' qualifier (has no effect) </ol>
             */
            template<class R>
            CORE_ALIAS(Return, typename Class<Prim<R>>::template IfElse<
            (Class<Prim<R>>::isPrimitive() && !Class<R>::isReference()) ||
            (!Class<R>::isReference() && Class<Prim<R>>::isFinal() && !Class<Prim<R>>::isAbstract()), ,
                       typename Class<Prim<R>>::Reference >);


            /**
             * <h2> Function Argument type </h2>
             * <p> Transform function argument type to easy form
             * <li> without '&' qualifier to type with 'const &' qualifier
             * <li> with '&' qualifier (has no effect)
             */
            template<class P>
            CORE_ALIAS(Params, typename Class<P>::template If<Class<P>::isReference(),, typename Class<P>::CRef >);

        private:

// for supplier
            template<class T, class R>
            CORE_ALIAS(ZeroLevel1, , typename Class<R(*)()>::template IfElse<Class<T>::
            template isConvertible<R(*)()>(), T>);

// for unary function
            template<class T, class P, class R>
            CORE_ALIAS(UnaryLevel1, , typename Class<R(*)(P)>::template IfElse<Class<T>::
            template isConvertible<R(*)(P)>(), T>);

            template<class F, class P, class R, class T = UnaryLevel1<F, P, R>>
            CORE_ALIAS(UnaryLevel2, , typename Class<R(*)(const P)>::template IfElse<Class<T>::
            template isConvertible<R(*)(const P)>(), T>);

            template<class F, class P, class R, class T = UnaryLevel2<F, P, R>>
            CORE_ALIAS(UnaryLevel3, , typename Class<R(*)(const P &)>::template IfElse<Class<T>::
            template isConvertible<R(*)(const P &)>(), T>);

            template<class F, class P, class R, class T = UnaryLevel3<F, P, R>>
            CORE_ALIAS(UnaryLevel4, , typename Class<R(*)(const P &&)>::template IfElse<Class<T>::
            template isConvertible<R(*)(const P &&)>(), T>);

            template<class F, class P, class R, class T = UnaryLevel4<F, P, R>>
            CORE_ALIAS(UnaryLevel5, , typename Class<R(*)(P & )>::template IfElse<Class<T>::
            template isConvertible<R(*)(P & )>(), T>);

            template<class F, class P, class R, class T = UnaryLevel5<F, P, R>>
            CORE_ALIAS(UnaryLevel6, , typename Class<R(*)(P && )>::template IfElse<Class<T>::
            template isConvertible<R(*)(P && )>(), T>);

// for binary function
            template<class T, class P1, class P2, class R>
            CORE_ALIAS(BinaryLevel1, , typename Class<R(*)(P1, P2)>::template IfElse<Class<T>::
            template isConvertible<R(*)(P1, P2)>(), T>);

// --- 1

            template<class F, class P1, class P2, class R, class T = BinaryLevel1<F, P1, P2, R>>
            CORE_ALIAS(BinaryLevel2, , typename Class<R(*)(const P1, const P2)>::
            template IfElse<Class<T>::template isConvertible<R(*)(const P1, const P2)>(), T>);

            template<class F, class P1, class P2, class R, class T = BinaryLevel2<F, P1, P2, R>>
            CORE_ALIAS(BinaryLevel3, , typename Class<R(*)(const P1, const P2 &)>::
            template IfElse<Class<T>::template isConvertible<R(*)(const P1, const P2 &)>(), T>);

            template<class F, class P1, class P2, class R, class T = BinaryLevel3<F, P1, P2, R>>
            CORE_ALIAS(BinaryLevel4, , typename Class<R(*)(const P1, const P2 &&)>::
            template IfElse<Class<T>::template isConvertible<R(*)(const P1, const P2 &&)>(), T>);

            template<class F, class P1, class P2, class R, class T = BinaryLevel4<F, P1, P2, R>>
            CORE_ALIAS(BinaryLevel5, , typename Class<R(*)(const P1, P2 &)>::
            template IfElse<Class<T>::template isConvertible<R(*)(const P1, P2 &)>(), T>);

            template<class F, class P1, class P2, class R, class T = BinaryLevel5<F, P1, P2, R>>
            CORE_ALIAS(BinaryLevel6, , typename Class<R(*)(const P1, P2 &&)>::
            template IfElse<Class<T>::template isConvertible<R(*)(const P1, P2 &&)>(), T>);

// --- 2

            template<class F, class P1, class P2, class R, class T = BinaryLevel6<F, P1, P2, R>>
            CORE_ALIAS(BinaryLevel7, , typename Class<R(*)(P1 &, const P2)>::
            template IfElse<Class<T>::template isConvertible<R(*)(P1 &, const P2)>(), T>);

            template<class F, class P1, class P2, class R, class T = BinaryLevel7<F, P1, P2, R>>
            CORE_ALIAS(BinaryLevel8, , typename Class<R(*)(P1 &, const P2 &)>::
            template IfElse<Class<T>::template isConvertible<R(*)(P1 &, const P2 &)>(), T>);

            template<class F, class P1, class P2, class R, class T = BinaryLevel8<F, P1, P2, R>>
            CORE_ALIAS(BinaryLevel9, , typename Class<R(*)(P1 &, const P2 &&)>::
            template IfElse<Class<T>::template isConvertible<R(*)(P1 &, const P2 &&)>(), T>);

            template<class F, class P1, class P2, class R, class T = BinaryLevel9<F, P1, P2, R>>
            CORE_ALIAS(BinaryLevel10, , typename Class<R(*)(P1 & , P2 & )>::
            template IfElse<Class<T>::template isConvertible<R(*)(P1 & , P2 & )>(), T>);

            template<class F, class P1, class P2, class R, class T = BinaryLevel10<F, P1, P2, R>>
            CORE_ALIAS(BinaryLevel11, , typename Class<R(*)(P1 & , P2 && )>::
            template IfElse<Class<T>::template isConvertible<R(*)(P1 & , P2 && )>(), T>);

// --- 3

            template<class F, class P1, class P2, class R, class T = BinaryLevel11<F, P1, P2, R>>
            CORE_ALIAS(BinaryLevel12, , typename Class<R(*)(P1 &&, const P2)>::
            template IfElse<Class<T>::template isConvertible<R(*)(P1 &&, const P2)>(), T>);

            template<class F, class P1, class P2, class R, class T = BinaryLevel12<F, P1, P2, R>>
            CORE_ALIAS(BinaryLevel13, , typename Class<R(*)(P1 &&, const P2 &)>::
            template IfElse<Class<T>::template isConvertible<R(*)(P1 &&, const P2 &)>(), T>);

            template<class F, class P1, class P2, class R, class T = BinaryLevel13<F, P1, P2, R>>
            CORE_ALIAS(BinaryLevel14, , typename Class<R(*)(P1 &&, const P2 &&)>::
            template IfElse<Class<T>::template isConvertible<R(*)(P1 &&, const P2 &&)>(), T>);

            template<class F, class P1, class P2, class R, class T = BinaryLevel14<F, P1, P2, R>>
            CORE_ALIAS(BinaryLevel15, , typename Class<R(*)(P1 && , P2 & )>::
            template IfElse<Class<T>::template isConvertible<R(*)(P1 && , P2 & )>(), T>);

            template<class F, class P1, class P2, class R, class T = BinaryLevel15<F, P1, P2, R>>
            CORE_ALIAS(BinaryLevel16, , typename Class<R(*)(P1 && , P2 && )>::
            template IfElse<Class<T>::template isConvertible<R(*)(P1 && , P2 && )>(), T>);

        public:
            /**
             * <h2> Function Signature </h2>
             * <p> Transform function signature to easy form
             * <li> global function (has no effect)
             * <li> lambda function:
             * <ol><li> without capture to global function
             *     <li> with capture (has no effect) </ol>
             * <li> callable object (has no effect)
             */
            template<class T, class R>
            CORE_ALIAS(ZeroSign, , ZeroLevel1<T, R>);

            /**
             * @see ZeroSign
             */
            template<class T, class P, class R>
            CORE_ALIAS(UnarySign, , UnaryLevel6<T, P, R>);

            /**
             * @see ZeroSign
             */
            template<class T, class P1, class P2, class R>
            CORE_ALIAS(BinarySign, , BinaryLevel16<T, P1, P2, R>);

        };
    } // core
} // util
#endif //CORE23_FUNCTIONAL_H
