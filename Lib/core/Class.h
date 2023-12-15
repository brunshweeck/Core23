//
// Created by Brunshweeck on 11/09/2023 18:02.
// Copyright (c) 2023
//

#ifndef CORE23_CLASS_H
#define CORE23_CLASS_H

#include <core/Private/Templates.h>
#include <core/Private/Literals.h>

namespace core {

    /**
     * Class represent type utility. contains the
     * method to treat directly with class and instance.
     * For other class operations see header file <core/private/Unsafe.h>
     */
    template<class T>
    class Class CORE_FINAL {
    private:

        /**
         * Construct new instance of Class
         */
        CORE_FAST Class() = default;

        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_DEPRECATED

        template<gint x, class Extractor>
        CORE_ALIAS(PARAM_SELECTOR, native::Templates::FnParamSelector<x,, Extractor >);

        template<gint x, class...Params>
        CORE_ALIAS(TPARAM_SELECTOR, native::Templates::VA_ARGS<x,, Params...>);

        template<class Signature>
        CORE_ALIAS(PARAM_EXTRACTOR, native::Templates::FnAnalyzer<Signature>);

        template<gint X, class Y>
        CORE_ALIAS(MODIFY, native::Templates::MODIFY<X,, Y >);

        template<gint X, class Y, gint S>
        CORE_ALIAS(ARRAY_MODIFY, native::Templates::MODIFY<X,, Y, S >);

        template<gint X, class Y>
        CORE_ALIAS(MODIFY_RESULT, typename MODIFY<X,, Y > ::Type);

        template<gint X, class Y, gint S>
        CORE_ALIAS(ARRAY_MODIFY_RESULT, typename ARRAY_MODIFY<X,, Y, S > ::Type);

        template<gint X, class Y>
        CORE_ALIAS(TESTER, native::Templates::TEST<X,, Y >);

        template<gint X, class ...Y>
        CORE_ALIAS(MULTI_TESTER, native::Templates::TEST<X,, Y...>);

        template<gint X, class Fallback, class ...Y>
        CORE_ALIAS(MULTI_ARGS, native::Templates::VA_ARGS<X,, Fallback, Y...>);

        template<gbool X, class Y>
        CORE_ALIAS(STRICT_CONDITIONAL, native::Templates::CONDITIONAL_STRICT<Y,, X >);

        template<gbool X, class Y, class Z>
        CORE_ALIAS(CONDITIONAL, native::Templates::CONDITIONAL<Y,, X, Z >);

        template<gbool X, class Y>
        CORE_ALIAS(STRICT_CONDITIONAL_RESULT, typename STRICT_CONDITIONAL<X,, Y > ::Type);

        template<gbool X, class Y, class Z>
        CORE_ALIAS(CONDITIONAL_RESULT, typename CONDITIONAL<X,, Y, Z > ::Type);

        template<gint X, class Y>
        static CORE_FAST gbool TEST_RESULT() { return TESTER<X, Y>::Value; }

        template<gint X, class ...Y>
        static CORE_FAST gbool MULTI_TEST_RESULT() { return MULTI_TESTER<X, Y...>::Value; }

    public:



        /**
         * Constant version of template type.
         */
        CORE_ALIAS(Constant, MODIFY_RESULT<2,, T >);

        /**
         * Volatile version of template type.
         */
        CORE_ALIAS(Volatile, MODIFY_RESULT<3,, T >);

        /**
         * Reference version of template type.
         */
        CORE_ALIAS(Reference, MODIFY_RESULT<4,, T >);
        CORE_ALIAS(LvalueReference, Reference);
        CORE_ALIAS(RvalueReference, MODIFY_RESULT<44,, T >);

        /**
         * Pointer version of template type.
         */
        CORE_ALIAS(Pointer, MODIFY_RESULT<5,, T >);

        /**
         * Constant Volatile version of template type.
         */
        CORE_ALIAS(ConstVol, MODIFY_RESULT<6,, T >);

        /**
         * Constant Reference version of template type
         */
        CORE_ALIAS(ConstRef, typename Class<Constant>::Reference);
        CORE_ALIAS(ConstLvalueRef, typename Class<Constant>::LvalueReference);
        CORE_ALIAS(ConstRvalueRef, typename Class<Constant>::RvalueReference);

        /**
         * Constant Volatile Reference version of template type.
         */
        CORE_ALIAS(ConstVolRef, typename Class<ConstVol>::Reference);
        CORE_ALIAS(ConstVolLvalueRef, typename Class<ConstVol>::LvalueReference);
        CORE_ALIAS(ConstVolRvalueRef, typename Class<ConstVol>::RvalueReference);

        /**
         * Mutable version of template type
         */
        CORE_ALIAS(NoConstant, MODIFY_RESULT<-2,, T >);

        /**
         * Unreferenced version of template type
         */
        CORE_ALIAS(NoReference, MODIFY_RESULT<-4,, T >);

        /**
         * Unreferenced version of template type (only lvalue)
         */
        CORE_ALIAS(NoRvalueReference, MODIFY_RESULT<-44,, T >);

        /**
         * Non Volatile version of template type
         */
        CORE_ALIAS(NoVolatile, MODIFY_RESULT<-3,, T >);

        /**
         * Non Pointer version of template type
         */
        CORE_ALIAS(NoPointer, MODIFY_RESULT<-5,, NoReference>);

        /**
         * Mutable and Non Volatile version of template type
         */
        CORE_ALIAS(NoConstVol, MODIFY_RESULT<-6,, T >);

        /**
         * Mutable and Unreferenced version of template type
         */
        CORE_ALIAS(NoConstRef, typename Class<NoReference>::NoConstant);
        CORE_ALIAS(NoConstRvalueRef, typename Class<NoRvalueReference>::NoConstant);

        /**
         * Mutable, Non Volatile and Unreferenced version of template type
         */
        CORE_ALIAS(NoConstVolRef, typename Class<NoReference>::NoConstVol);
        CORE_ALIAS(NoConstVolRvalueRef, typename Class<NoRvalueReference>::NoConstVol);

        /**
         * Strict conditional (if ... else error) version of template type.
         *
         * @param Cnd
         *         The createCondition
         */
        template<gbool Cnd>
        CORE_ALIAS(OnlyIf, STRICT_CONDITIONAL_RESULT<Cnd,, T >);

        /**
         * Reversible conditional (if ... else ...) version of template type.
         *
         * @param Cnd
         *         The createCondition
         *
         * @param Fallback
         *         The choice if createCondition failed
         */
        template<gbool Cnd, class Fallback>
        CORE_ALIAS(IfElse, CONDITIONAL_RESULT<Cnd,, T, Fallback >);

        /**
         * static Array version of template type
         */
        template<gint Size>
        CORE_ALIAS(Array, ARRAY_MODIFY_RESULT<9,, NoReference , Size >);

        /**
         * Array Item type of template type
         */
        CORE_ALIAS(NoArray, MODIFY_RESULT<-9,, NoReference >);

        /**
         * VarArgs Item type at specified position.
         * The first position is 1, The last is sizeof...(Args)
         * <p>
         * Note: If Args is empty of index out of range this template
         * type is returned as Fallback type.
         *
         * @tparam Index The position of type (the fist position is 1)
         * @tparam Args The list of types
         */
        template<gint Index, class ...Args>
        CORE_ALIAS(VarArgs, MULTI_ARGS<Index,, T, Args...>);

        CORE_ALIAS(Const, Constant);
        CORE_ALIAS(Vol, Volatile);
        CORE_ALIAS(Ptr, Pointer);
        CORE_ALIAS(Ref, Reference);
        CORE_ALIAS(LvRef, LvalueReference);
        CORE_ALIAS(RvRef, RvalueReference);
        CORE_ALIAS(CRef, ConstRef);
        CORE_ALIAS(CLvRef, ConstLvalueRef);
        CORE_ALIAS(CRvRef, ConstRvalueRef);
        CORE_ALIAS(CVRef, ConstVolRef);
        CORE_ALIAS(CVLvRef, ConstVolLvalueRef);
        CORE_ALIAS(CVRvRef, ConstVolRvalueRef);
        CORE_ALIAS(NConst, NoConstant);
        CORE_ALIAS(NVol, NoVolatile);
        CORE_ALIAS(NRef, NoReference);
        CORE_ALIAS(NRvRef, NoRvalueReference);
        CORE_ALIAS(NCVol, NoConstVol);
        CORE_ALIAS(NCRef, NoConstRef);
        CORE_ALIAS(NCRvRef, NoConstRvalueRef);
        CORE_ALIAS(NCVRef, NoConstVolRef);
        CORE_ALIAS(NCVRvRef, NoConstVolRvalueRef);
        CORE_ALIAS(NPtr, NoPointer);
        CORE_ALIAS(NArr, NoArray);
        template<gbool Cnd>
        CORE_ALIAS(Iff, OnlyIf<Cnd>);
        template<gbool Cnd, class Else>
        CORE_ALIAS(If, IfElse<Cnd,, Else >);

        /**
         * Return true if this type is similar with other type
         * @param U The other type
         */
        template<class U>
        static CORE_FAST gbool isSimilar() { return MULTI_TEST_RESULT<1, T, U>(); }

        /**
         * Return true if this type is constant type
         */
        static CORE_FAST gbool isConstant() { return TEST_RESULT<2, NRef>(); }

        /**
         * Return true if this type is volatile type
         */
        static CORE_FAST gbool isVolatile() { return TEST_RESULT<3, NRef>(); }

        /**
         * Return true if this type is reference type
         */
        static CORE_FAST gbool isReference() { return TEST_RESULT<4, T>(); }

        /**
         * Return true if this type is rvalue reference
         */
        static CORE_FAST gbool isRvalueReference() { return TEST_RESULT<44, T>(); }

        /**
         * Return true if this type is lvalue reference
         */
        static CORE_FAST gbool isLvalueReference() { return TEST_RESULT<444, T>(); }

        /**
         * Return true if this type is pointer type
         */
        static CORE_FAST gbool isPointer() { return TEST_RESULT<5, T>(); }

        /**
         * Return true if this type is static array type
         */
        static CORE_FAST gbool isArray() { return TEST_RESULT<9, NRef>(); }

        /**
         * Return true if this type is complete type (sizable type)
         */
        static CORE_FAST gbool isComplete() { return TEST_RESULT<10, T>(); }

        /**
         * Return true if this type is template type
         */
        static CORE_FAST gbool isTemplate() { return TEST_RESULT<11, T>(); }

        /**
         * Return true if this type is function type
         */
        static CORE_FAST gbool isFunction() { return TEST_RESULT<12, NRef>(); }

        /**
         * Return true if this type is class member type
         */
        static CORE_FAST gbool isMember() { return TEST_RESULT<13, T>(); }

        /**
         * Return true if this type is function member type
         */
        static CORE_FAST gbool isFunctionMember() { return TEST_RESULT<14, T>(); }

        /**
         * Return true if this type is abstract type
         */
        static CORE_FAST gbool isAbstract() { return TEST_RESULT<15, T>(); }

        /**
         * Return true if this type is enum type
         */
        static CORE_FAST gbool isEnum() { return TEST_RESULT<16, T>(); }

        /**
         * Return true if this type is class type
         */
        static CORE_FAST gbool isClass() { return TEST_RESULT<17, NRef>(); }

        /**
         * Return true if this type is aggregate (class, union or array)
         */
        static CORE_FAST gbool isAggregate() { return TEST_RESULT<33, T>() || isClass() || isArray() || __is_union(T); }

        /**
         * Return true if this template type is implement destructor.
         */
        static CORE_FAST gbool isDestructible() { return TEST_RESULT<32, NCVRef>(); }

        /**
         * Return true if instance of this type is constructible with values of specified types
         *
         * @tparam Params The arguments parameters types
         */
        template<class ...Params>
        static CORE_FAST gbool isConstructible() { return MULTI_TEST_RESULT<18, T, Params...>(); }

        /**
         * Return true if value of specified type is assignable to instance of this type
         *
         * @param U
         *        The assigning value type
         */
        template<class To>
        static CORE_FAST gbool isAssignable() { return MULTI_TEST_RESULT<19, T, typename Class<To>::Ref>(); }

        /**
         * Return true if this type is one base of specified type
         *
         * @tparam Derived
         *        The derived type
         */
        template<class Derived>
        static CORE_FAST gbool isSuper() { return MULTI_TEST_RESULT<20, NCVRef, typename Class<Derived>::NCVRef>(); }

        /**
         * Return true if value this type is implicit convertible to specified type
         *
         * @tparam To The Destination type
         */
        template<class To>
        static CORE_FAST gbool isConvertible() { return MULTI_TEST_RESULT<21, T, To>(); }

        /**
         * Return true if this type is not overridable or is marked final (is available for class or union types only)
         */
        static CORE_FAST gbool isFinal() { return TEST_RESULT<34, T>(); }

        /**
         * Return true if this type is polymorphic type
         */
        static CORE_FAST gbool isPolymorphic() { return TEST_RESULT<35, T>(); }

        /**
         * Return true if this type is empty type (for class and union types)
         */
        static CORE_FAST gbool isEmpty() { return TEST_RESULT<36, NRef>(); }

        /**
         * Return true if this type is empty type
         */
        static CORE_FAST gbool isTrivial() { return TEST_RESULT<37, NRef>(); }

        /**
         * Return true if this type is literal type
         */
        static CORE_FAST gbool isLiteral() { return TEST_RESULT<38, NRef>(); }

        /**
         * Return true if this type is native integer type
         */
        static CORE_FAST gbool isInteger() { return TEST_RESULT<22, NCVRef>(); }

        /**
         * Return true if this type is native floating type
         */
        static CORE_FAST gbool isFloating() { return TEST_RESULT<23, NCVRef>(); }

        /**
         * Return true if this type is native character type
         */
        static CORE_FAST gbool isCharacter() { return TEST_RESULT<24, NCVRef>(); }

        /**
         * Return true if this type is native boolean type
         */
        static CORE_FAST gbool isBoolean() { return TEST_RESULT<25, NCVRef>(); }

        /**
         * Return true if this type is native boolean type
         */
        static CORE_FAST gbool isVoid() { return isSimilar<void>(); }

        /**
         * Return true if instance of this type is callable with values of specified type
         *
         * @tparam Params The arguments types
         */
        template<class ...Params>
        static CORE_FAST gbool isCallable() { return MULTI_TEST_RESULT<26, T, Params...>(); }

        /**
         * Return true if this type is native string type
         */
        static CORE_FAST gbool isString() { return TEST_RESULT<27, T>(); }

        /**
         * Return true if this type is native number type
         */
        static CORE_FAST gbool isNumber() { return isInteger() || isFloating(); }

        /**
         * Return true if this type is native integral type
         */
        static CORE_FAST gbool isIntegral() { return isBoolean() || isInteger() || isEnum() || isCharacter(); }

        /**
         * Return true if this template type is native type
         */
        static CORE_FAST gbool isPrimitive() {
            return isVoid() || isIntegral() || isFloating() || isCharacter() || isPointer() || isArray();
        }

        /**
         * Return true if specified object is instance of this template type.
         * Example:
         * <pre>
         * String str = "hello my friend";      <br>
         * Object &obj = str;       <br>
         * Class\<String>::hasInstance(obj) // return true      <br>
         * Class\<StringBuffer>::hasInstance(obj) // return false       <br>
         * Class\<Object>::hasInstance(str) // return true      <br>
         * </pre>
         *
         * @param o
         *          The object instance to be tested
         */
        template<class U>
        static CORE_FAST gbool hasInstance(U &&o) {
            CORE_STATIC_ASSERT(Class<T>::isClass(), "This Test require class type as Target");
            CORE_STATIC_ASSERT(Class<U>::isClass(), "This Test require class type as Arguments");
            CORE_ALIAS(NVRef, typename Class<NCVRef>::Const);
            CORE_ALIAS(NVRefPtr, typename Class<NVRef>::Ptr);
            return CORE_DYN_CAST(NVRefPtr, &o);
        }

        /**
         * Return true if this template type support operator equal to (==)
         */
        static CORE_FAST gbool supportEQ() { return TEST_RESULT<28, T>(); }

        /**
         * Return true if this template type support operator less than (<)
         */
        static CORE_FAST gbool supportLT() { return TEST_RESULT<29, T>(); }

        /**
         * Object represent the class version of this template type
         */
        CORE_ALIAS(Object, MODIFY_RESULT<30,, T >);

        /**
         * native represent the native version of this template type
         */
        CORE_ALIAS(Primitive, MODIFY_RESULT<31,, T >);

        /**
         * Return field represent the return type of value returned after calling
         * instance of this template type as function with specified type parameters.
         *
         * <p>
         * Note: Return exist if and only of this template type is callable with instances
         * given types parameters (if and only if Class\<T>.isCallable\<V...>() return true)
         *
         * @tparam Params The types parameters
         *
         * @see isCallable
         * @see isFunction
         * @see isMethod
         */
        template<class ...Params>
        CORE_ALIAS(Return, typename MULTI_TESTER<26,, T, Params...>::Type::Type);

        /**
         * Return result of multiple boolean tests using logical AND as operator.
         * In other word return true if all of given values is true.
         */
        static CORE_FAST gbool allIsTrue() { return true; }
        static CORE_FAST gbool allIsTrue(gbool b) { return b; }

        /**
         * Return result of multiple boolean tests using logical OR as operator.
         * In other word return true if any of given values is true.
         */
        static CORE_FAST gbool oneIsTrue() { return false; }
        static CORE_FAST gbool oneIsTrue(gbool b) { return b; }

        /**
         * Return result of multiple boolean tests using logical AND as operator.
         * In other word return true if all of given values is true.
         *
         * @param b
         *        The first operand
         * @param v
         *        The others operands
         */
        template<class ...U>
        static CORE_FAST gbool allIsTrue(gbool b, U ...u) { return b && allIsTrue(u...); }

        /**
         * Return result of multiple boolean tests using logical OR as operator.
         * In other word return true if any of given values is true.
         *
         * @param b
         *        The first operand
         * @param v
         *        The others operands
         */
        template<class ...V>
        static CORE_FAST gbool oneIsTrue(gbool b, V &&...v) { return b || oneIsTrue((V &&) v...); }

        /**
         * Return the value at specified position on the given list of values (varArgs).
         * the first position is 1, and the last is n (number of given values).
         * if given index is zero, the fallback value will be returned
         * @param i The index of target value
         * @param defaultValue the fallback value. is returned if given index out of bounds
         *                      in order world (i <= 0 || i > sizeof...(args))
         */
        template<class _ = T>
        static CORE_FAST _ valueExactAt(gint i, _ defaultValue) {
            return defaultValue;
        }

        /**
         * Return the value at specified position on the given list of values (varArgs).
         * the first position is 1, and the last is n (number of given values).
         * if given index is zero, the fallback value will be returned
         * @param i The index of target value
         * @param defaultValue the fallback value. is returned if given index out of bounds
         *                      in order world (i <= 0 || i > sizeof...(args))
         * @param v1 The first value. is returned if given index is 1
         */
        template<class V1, class _ = T>
        static CORE_FAST _ valueExactAt(gint i, _ defaultValue, V1 &&v1) {
            CORE_STATIC_ASSERT(Class<V1>::template isConvertible<_>(), "couldn't cast first argument to target type");
            return i != 1 ? defaultValue : (_) (V1 &&) v1;
        }

        /**
         * Return the value at specified position on the given list of values (varArgs).
         * the first position is 1, and the last is n (number of given values).
         * if given index is zero, the fallback value will be returned
         * @param i The index of target value
         * @param defaultValue the fallback value. is returned if given index out of bounds
         *                      in order world (i <= 0 || i > sizeof...(args))
         * @param v1 The first value. is returned if given index is 1
         * @param v2 The second value. is returned if given index is 2
         */
        template<class V1, class V2, class _ = T>
        static CORE_FAST _ valueExactAt(gint i, _ defaultValue, V1 &&v1, V2 &&v2) {
            CORE_STATIC_ASSERT(Class<V1>::template isConvertible<_>(), "couldn't cast first argument to target type");
            CORE_STATIC_ASSERT(Class<V2>::template isConvertible<_>(), "couldn't cast second argument to target type");
            return i <= 0 ? defaultValue :
                   i == 1 ? (_) (V1 &&) v1 :
                   i == 2 ? (_) (V2 &&) v2 :
                   defaultValue;
        }

        /**
         * Return the value at specified position on the given list of values (varArgs).
         * the first position is 1, and the last is n (number of given values).
         * if given index is zero, the fallback value will be returned
         * @param i The index of target value
         * @param defaultValue the fallback value. is returned if given index out of bounds
         *                      in order world (i <= 0 || i > sizeof...(args))
         * @param v1 The first value. is returned if given index is 1
         * @param v2 The second value. is returned if given index is 2
         * @param v3 The third value. is returned if given index is 3
         */
        template<class V1, class V2, class V3, class _ = T>
        static CORE_FAST _ valueExactAt(gint i, _ defaultValue, V1 &&v1, V2 &&v2, V3 &&v3) {
            CORE_STATIC_ASSERT(Class<V1>::template isConvertible<_>(), "couldn't cast first argument to target type");
            CORE_STATIC_ASSERT(Class<V2>::template isConvertible<_>(), "couldn't cast second argument to target type");
            CORE_STATIC_ASSERT(Class<V3>::template isConvertible<_>(), "couldn't cast third argument to target type");
            return i <= 0 ? defaultValue :
                   i == 1 ? (_) (V1 &&) v1 :
                   i == 2 ? (_) (V2 &&) v2 :
                   i == 3 ? (_) (V3 &&) v3 :
                   defaultValue;
        }

        /**
         * Return the value at specified position on the given list of values (varArgs).
         * the first position is 1, and the last is n (number of given values).
         * if given index is zero, the fallback value will be returned
         * @param i The index of target value
         * @param defaultValue the fallback value. is returned if given index out of bounds
         *                      in order world (i <= 0 || i > sizeof...(args))
         * @param v1 The first value. is returned if given index is 1
         * @param v2 The second value. is returned if given index is 2
         * @param v3 The third value. is returned if given index is 3
         * @param v4 The fourth value. is returned if given index is 4
         */
        template<class V1, class V2, class V3, class V4, class _ = T>
        static CORE_FAST _ valueExactAt(gint i, _ defaultValue, V1 &&v1, V2 &&v2, V3 &&v3, V4 &&v4) {
            CORE_STATIC_ASSERT(Class<V1>::template isConvertible<_>(), "couldn't cast first argument to target type");
            CORE_STATIC_ASSERT(Class<V2>::template isConvertible<_>(), "couldn't cast second argument to target type");
            CORE_STATIC_ASSERT(Class<V3>::template isConvertible<_>(), "couldn't cast third argument to target type");
            CORE_STATIC_ASSERT(Class<V4>::template isConvertible<_>(), "couldn't cast fourth argument to target type");
            return i <= 0 ? defaultValue :
                   i == 1 ? (_) (V1 &&) v1 :
                   i == 2 ? (_) (V2 &&) v2 :
                   i == 3 ? (_) (V3 &&) v3 :
                   i == 4 ? (_) (V4 &&) v4 :
                   defaultValue;
        }

        /**
         * Return the value at specified position on the given list of values (varArgs).
         * the first position is 1, and the last is n (number of given values).
         * if given index is zero, the fallback value will be returned
         * @param i The index of target value
         * @param defaultValue the fallback value. is returned if given index out of bounds
         *                      in order world (i <= 0 || i > sizeof...(args))
         * @param v1 The first value. is returned if given index is 1
         * @param v2 The second value. is returned if given index is 2
         * @param v3 The third value. is returned if given index is 3
         * @param v4 The fourth value. is returned if given index is 4
         * @param v5 The fifth value. is returned if given index is 5
         */
        template<class V1, class V2, class V3, class V4, class V5, class _ = T>
        static CORE_FAST _ valueExactAt(gint i, _ defaultValue, V1 &&v1, V2 &&v2, V3 &&v3, V4 &&v4, V5 &&v5) {
            CORE_STATIC_ASSERT(Class<V1>::template isConvertible<_>(), "couldn't cast first argument to target type");
            CORE_STATIC_ASSERT(Class<V2>::template isConvertible<_>(), "couldn't cast second argument to target type");
            CORE_STATIC_ASSERT(Class<V3>::template isConvertible<_>(), "couldn't cast third argument to target type");
            CORE_STATIC_ASSERT(Class<V4>::template isConvertible<_>(), "couldn't cast fourth argument to target type");
            CORE_STATIC_ASSERT(Class<V5>::template isConvertible<_>(), "couldn't cast fifth argument to target type");
            return i <= 0 ? defaultValue :
                   i == 1 ? (_) (V1 &&) v1 :
                   i == 2 ? (_) (V2 &&) v2 :
                   i == 3 ? (_) (V3 &&) v3 :
                   i == 4 ? (_) (V4 &&) v4 :
                   i == 5 ? (_) (V5 &&) v5 :
                   defaultValue;
        }

        /**
         * Return the value at specified position on the given list of values (varArgs).
         * the first position is 1, and the last is n (number of given values).
         * if given index is zero, the fallback value will be returned
         * @param i The index of target value
         * @param defaultValue the fallback value. is returned if given index out of bounds
         *                      in order world (i <= 0 || i > sizeof...(args))
         * @param v1 The first value. is returned if given index is 1
         * @param v2 The second value. is returned if given index is 2
         * @param v3 The third value. is returned if given index is 3
         * @param v4 The fourth value. is returned if given index is 4
         * @param v5 The fifth value. is returned if given index is 5
         * @param v6 The sixth value. is returned if given index is 6
         */
        template<class V1, class V2, class V3, class V4, class V5, class V6, class _ = T>
        static CORE_FAST _ valueExactAt(gint i, _ defaultValue, V1 &&v1, V2 &&v2, V3 &&v3, V4 &&v4, V5 &&v5, V6 &&v6) {
            CORE_STATIC_ASSERT(Class<V1>::template isConvertible<_>(), "couldn't cast first argument to target type");
            CORE_STATIC_ASSERT(Class<V2>::template isConvertible<_>(), "couldn't cast second argument to target type");
            CORE_STATIC_ASSERT(Class<V3>::template isConvertible<_>(), "couldn't cast third argument to target type");
            CORE_STATIC_ASSERT(Class<V4>::template isConvertible<_>(), "couldn't cast fourth argument to target type");
            CORE_STATIC_ASSERT(Class<V5>::template isConvertible<_>(), "couldn't cast fifth argument to target type");
            CORE_STATIC_ASSERT(Class<V6>::template isConvertible<_>(), "couldn't cast sixth argument to target type");
            return i <= 0 ? defaultValue :
                   i == 1 ? (_) (V1 &&) v1 :
                   i == 2 ? (_) (V2 &&) v2 :
                   i == 3 ? (_) (V3 &&) v3 :
                   i == 4 ? (_) (V4 &&) v4 :
                   i == 5 ? (_) (V5 &&) v5 :
                   i == 6 ? (_) (V6 &&) v6 :
                   defaultValue;
        }

        /**
         * Return the value at specified position on the given list of values (varArgs).
         * the first position is 1, and the last is n (number of given values).
         * if given index is zero, the fallback value will be returned
         * @param i The index of target value
         * @param defaultValue the fallback value. is returned if given index out of bounds
         *                      in order world (i <= 0 || i > sizeof...(args))
         * @param v1 The first value. is returned if given index is 1
         * @param v2 The second value. is returned if given index is 2
         * @param v3 The third value. is returned if given index is 3
         * @param v4 The fourth value. is returned if given index is 4
         * @param v5 The fifth value. is returned if given index is 5
         * @param v6 The sixth value. is returned if given index is 6
         * @param v7 The seventh value. is returned if given index is 7
         */
        template<class V1, class V2, class V3, class V4, class V5, class V6, class V7, class _ = T>
        static CORE_FAST _ valueExactAt(gint i, _ defaultValue, V1 &&v1, V2 &&v2, V3 &&v3, V4 &&v4, V5 &&v5, V6 &&v6,
                                        V7 &&v7) {
            CORE_STATIC_ASSERT(Class<V1>::template isConvertible<_>(), "couldn't cast first argument to target type");
            CORE_STATIC_ASSERT(Class<V2>::template isConvertible<_>(), "couldn't cast second argument to target type");
            CORE_STATIC_ASSERT(Class<V3>::template isConvertible<_>(), "couldn't cast third argument to target type");
            CORE_STATIC_ASSERT(Class<V4>::template isConvertible<_>(), "couldn't cast fourth argument to target type");
            CORE_STATIC_ASSERT(Class<V5>::template isConvertible<_>(), "couldn't cast fifth argument to target type");
            CORE_STATIC_ASSERT(Class<V6>::template isConvertible<_>(), "couldn't cast sixth argument to target type");
            CORE_STATIC_ASSERT(Class<V7>::template isConvertible<_>(), "couldn't cast seventh argument to target type");
            return i <= 0 ? defaultValue :
                   i == 1 ? (_) (V1 &&) v1 :
                   i == 2 ? (_) (V2 &&) v2 :
                   i == 3 ? (_) (V3 &&) v3 :
                   i == 4 ? (_) (V4 &&) v4 :
                   i == 5 ? (_) (V5 &&) v5 :
                   i == 6 ? (_) (V6 &&) v6 :
                   i == 7 ? (_) (V7 &&) v7 :
                   defaultValue;
        }

        /**
         * Return the value at specified position on the given list of values (varArgs).
         * the first position is 1, and the last is n (number of given values).
         * if given index is zero, the fallback value will be returned
         * @param i The index of target value
         * @param defaultValue the fallback value. is returned if given index out of bounds
         *                      in order world (i <= 0 || i > sizeof...(args))
         * @param v1 The first value. is returned if given index is 1
         * @param v2 The second value. is returned if given index is 2
         * @param v3 The third value. is returned if given index is 3
         * @param v4 The fourth value. is returned if given index is 4
         * @param v5 The fifth value. is returned if given index is 5
         * @param v6 The sixth value. is returned if given index is 6
         * @param v7 The seventh value. is returned if given index is 7
         * @param v8 The eighth value. is returned if given index is 8
         */
        template<class V1, class V2, class V3, class V4, class V5, class V6, class V7, class V8, class _ = T>
        static CORE_FAST _ valueExactAt(gint i, _ defaultValue, V1 &&v1, V2 &&v2, V3 &&v3, V4 &&v4, V5 &&v5, V6 &&v6,
                                        V7 &&v7, V8 &&v8) {
            CORE_STATIC_ASSERT(Class<V1>::template isConvertible<_>(), "couldn't cast first argument to target type");
            CORE_STATIC_ASSERT(Class<V2>::template isConvertible<_>(), "couldn't cast second argument to target type");
            CORE_STATIC_ASSERT(Class<V3>::template isConvertible<_>(), "couldn't cast third argument to target type");
            CORE_STATIC_ASSERT(Class<V4>::template isConvertible<_>(), "couldn't cast fourth argument to target type");
            CORE_STATIC_ASSERT(Class<V5>::template isConvertible<_>(), "couldn't cast fifth argument to target type");
            CORE_STATIC_ASSERT(Class<V6>::template isConvertible<_>(), "couldn't cast sixth argument to target type");
            CORE_STATIC_ASSERT(Class<V7>::template isConvertible<_>(), "couldn't cast seventh argument to target type");
            CORE_STATIC_ASSERT(Class<V8>::template isConvertible<_>(), "couldn't cast eighth argument to target type");
            return i <= 0 ? defaultValue :
                   i == 1 ? (_) (V1 &&) v1 :
                   i == 2 ? (_) (V2 &&) v2 :
                   i == 3 ? (_) (V3 &&) v3 :
                   i == 4 ? (_) (V4 &&) v4 :
                   i == 5 ? (_) (V5 &&) v5 :
                   i == 6 ? (_) (V6 &&) v6 :
                   i == 7 ? (_) (V7 &&) v7 :
                   i == 8 ? (_) (V8 &&) v8 :
                   defaultValue;
        }

        /**
         * Return the value at specified position on the given list of values (varArgs).
         * the first position is 1, and the last is n (number of given values).
         * if given index is zero, the fallback value will be returned
         * @param i The index of target value
         * @param defaultValue the fallback value. is returned if given index out of bounds
         *                      in order world (i <= 0 || i > sizeof...(args))
         * @param v1 The first value. is returned if given index is 1
         * @param v2 The second value. is returned if given index is 2
         * @param v3 The third value. is returned if given index is 3
         * @param v4 The fourth value. is returned if given index is 4
         * @param v5 The fifth value. is returned if given index is 5
         * @param v6 The sixth value. is returned if given index is 6
         * @param v7 The seventh value. is returned if given index is 7
         * @param v8 The eighth value. is returned if given index is 8
         * @param v9 The ninth value. is returned if given index is 9
         */
        template<class V1, class V2, class V3, class V4, class V5, class V6, class V7, class V8, class V9, class _ = T>
        static CORE_FAST _ valueExactAt(gint i, _ defaultValue, V1 &&v1, V2 &&v2, V3 &&v3, V4 &&v4, V5 &&v5, V6 &&v6,
                                        V7 &&v7, V8 &&v8, V9 &&v9) {
            CORE_STATIC_ASSERT(Class<V1>::template isConvertible<_>(), "couldn't cast first argument to target type");
            CORE_STATIC_ASSERT(Class<V2>::template isConvertible<_>(), "couldn't cast second argument to target type");
            CORE_STATIC_ASSERT(Class<V3>::template isConvertible<_>(), "couldn't cast third argument to target type");
            CORE_STATIC_ASSERT(Class<V4>::template isConvertible<_>(), "couldn't cast fourth argument to target type");
            CORE_STATIC_ASSERT(Class<V5>::template isConvertible<_>(), "couldn't cast fifth argument to target type");
            CORE_STATIC_ASSERT(Class<V6>::template isConvertible<_>(), "couldn't cast sixth argument to target type");
            CORE_STATIC_ASSERT(Class<V7>::template isConvertible<_>(), "couldn't cast seventh argument to target type");
            CORE_STATIC_ASSERT(Class<V8>::template isConvertible<_>(), "couldn't cast eighth argument to target type");
            CORE_STATIC_ASSERT(Class<V9>::template isConvertible<_>(), "couldn't cast ninth argument to target type");
            return i <= 0 ? defaultValue :
                   i == 1 ? (_) (V1 &&) v1 :
                   i == 2 ? (_) (V2 &&) v2 :
                   i == 3 ? (_) (V3 &&) v3 :
                   i == 4 ? (_) (V4 &&) v4 :
                   i == 5 ? (_) (V5 &&) v5 :
                   i == 6 ? (_) (V6 &&) v6 :
                   i == 7 ? (_) (V7 &&) v7 :
                   i == 8 ? (_) (V8 &&) v8 :
                   i == 9 ? (_) (V9 &&) v9 :
                   defaultValue;
        }

        /**
         * Return the value at specified position on the given list of values (varArgs).
         * the first position is 1, and the last is n (number of given values).
         * if given index is zero, the fallback value will be returned
         * @param i The index of target value
         * @param defaultValue the fallback value. is returned if given index out of bounds
         *                      in order world (i <= 0 || i > sizeof...(args))
         * @param v1 The first value. is returned if given index is 1
         * @param v2 The second value. is returned if given index is 2
         * @param v3 The third value. is returned if given index is 3
         * @param v4 The fourth value. is returned if given index is 4
         * @param v5 The fifth value. is returned if given index is 5
         * @param v6 The sixth value. is returned if given index is 6
         * @param v7 The seventh value. is returned if given index is 7
         * @param v8 The eighth value. is returned if given index is 8
         * @param v9 The ninth value. is returned if given index is 9
         * @param v10 The ninth value. is returned if given index is 10
         * @param v The other values. is returned if given index is 10+n (n > 0 && n <= sizeof...(v))
         */
        template<class V1, class V2, class V3, class V4, class V5, class V6, class V7, class V8, class V9, class V10, class... V, class _ = T>
        static CORE_FAST _ valueExactAt(gint i, _ defaultValue, V1 &&v1, V2 &&v2, V3 &&v3, V4 &&v4, V5 &&v5, V6 &&v6,
                                        V7 &&v7, V8 &&v8, V9 &&v9, V10 &&v10, V &&...v) {
            CORE_STATIC_ASSERT(Class<V1>::template isConvertible<_>(), "couldn't cast first argument to target type");
            CORE_STATIC_ASSERT(Class<V2>::template isConvertible<_>(), "couldn't cast second argument to target type");
            CORE_STATIC_ASSERT(Class<V3>::template isConvertible<_>(), "couldn't cast third argument to target type");
            CORE_STATIC_ASSERT(Class<V4>::template isConvertible<_>(), "couldn't cast fourth argument to target type");
            CORE_STATIC_ASSERT(Class<V5>::template isConvertible<_>(), "couldn't cast fifth argument to target type");
            CORE_STATIC_ASSERT(Class<V6>::template isConvertible<_>(), "couldn't cast sixth argument to target type");
            CORE_STATIC_ASSERT(Class<V7>::template isConvertible<_>(), "couldn't cast seventh argument to target type");
            CORE_STATIC_ASSERT(Class<V8>::template isConvertible<_>(), "couldn't cast eighth argument to target type");
            CORE_STATIC_ASSERT(Class<V9>::template isConvertible<_>(), "couldn't cast ninth argument to target type");
            CORE_STATIC_ASSERT(Class<V10>::template isConvertible<_>(), "couldn't cast ninth argument to target type");
            CORE_STATIC_ASSERT(allIsTrue(Class<V>::template isConvertible<_>()...),
                               "couldn't cast arguments to target type");
            return i <= 0 ? defaultValue :
                   i == 1 ? (_) (V1 &&) v1 :
                   i == 2 ? (_) (V2 &&) v2 :
                   i == 3 ? (_) (V3 &&) v3 :
                   i == 4 ? (_) (V4 &&) v4 :
                   i == 5 ? (_) (V5 &&) v5 :
                   i == 6 ? (_) (V6 &&) v6 :
                   i == 7 ? (_) (V7 &&) v7 :
                   i == 8 ? (_) (V8 &&) v8 :
                   i == 9 ? (_) (V9 &&) v9 :
                   i == 10 ? (_) (V9 &&) v10 :
                   valueExactAt(i - 10, defaultValue, (V &&) v...);
        }

        /**
         * Return template type at specified position.
         * The first position is 1.
         * The current type that represent the fallback type
         * <p>
         * Ex: for the types: String and Complex
         *  <li> Class<Object>::TParams<0, String, Complex> = Object
         *  <li> Class<Object>::TParams<1, String, Complex> = String.
         *  <li> Class<Object>::TParams<2, String, Complex> = Complex.
         *  <li> Class<Object>::TParams<3, String, Complex> = Object
         *  <li> Class<Object>::TParams<*> = Object
         */
        template<gint i, class ...VarArgs>
        CORE_ALIAS(TParams, , typename TPARAM_SELECTOR<i, T, VarArgs...>::Type);

        /**
         * Return function parameter type at specified position.
         * The first position is 1.
         * In error case, The void type that represent it
         * <p>
         * Ex: for signature bool(String, Complex)
         *  <li> Class<bool(String, Complex)>::TParams<0> = void
         *  <li> Class<bool(String, Complex)>::TParams<1> = String.
         *  <li> Class<bool(String, Complex)>::TParams<2> = Complex.
         *  <li> Class<bool(String, Complex)>::TParams<3> = void
         *  <li> Class<bool()>::TParams<*> = void
         * <p>
         * Ex: for signature bool(*)(String, Complex)
         *  <li> Class<bool(*)(String, Complex)>::TParams<0> = void
         *  <li> Class<bool(*)(String, Complex)>::TParams<1> = String.
         *  <li> Class<bool(*)(String, Complex)>::TParams<2> = Complex.
         *  <li> Class<bool(*)(String, Complex)>::TParams<3> = void
         *  <li> Class<bool(*)()>::TParams<*> = T
         * <p>
         * Ex: for signature bool(MyClass::*)(String, Complex)
         *  <li> Class<bool(MyClass::*)(String, Complex)>::TParams<0> = void
         *  <li> Class<bool(MyClass::*)(String, Complex)>::TParams<1> = String.
         *  <li> Class<bool(MyClass::*)(String, Complex)>::TParams<2> = Complex.
         *  <li> Class<bool(MyClass::*)(String, Complex)>::TParams<3> = void
         *  <li> Class<bool(MyClass::*)()>::TParams<*> = void
         * <p>
         * Ex: for non function type
         *  <li> Class<MyType>::TParams<*> = void
         *  <li> Class<MyClass>::TParams<*> = void
         */
        template<gint i>
        CORE_ALIAS(Params, , typename PARAM_SELECTOR<i, PARAM_EXTRACTOR<T>>::Type);

        CORE_WARNING_POP

    };

} // core

#endif //CORE23_CLASS_H
