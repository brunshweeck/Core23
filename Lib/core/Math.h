//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_MATH_H
#define CORE23_MATH_H

#include <core/Object.h>

namespace core {

    /**
     * The class <b> Math</b> contains methods for performing basic
     * numeric operations such as the elementary exponential, logarithm,
     * square root, and trigonometric functions.
     *
     * <p>By default many of the <b> Math</b> methods simply call
     * the equivalent method in <b> StrictMath</b> for their
     * implementation.  Code generators are encouraged to use
     * platform-specific native libraries or microprocessor instructions,
     * where available, to provide higher-performance implementations of
     * <b> Math</b> methods.  Such higher-performance
     * implementations still must conform to the specification for
     * <b> Math</b>.
     *
     * <p>The quality of implementation specifications concern two
     * properties, accuracy of the returned result and monotonicity of the
     * method.  Accuracy of the floating-point <b> Math</b> methods is
     * measured in terms of <i>ulps</i>, units in the last place.  For a
     * given floating-point format, an <b style="color: green"> ulp</b> of a
     * specific real number value is the distance between the two
     * floating-point values bracketing that numerical value.  When
     * discussing the accuracy of a method as a whole rather than at a
     * specific argument, the number of ulps cited is for the worst-case
     * error at any argument.  If a method always has an error less than
     * 0.5 ulps, the method always returns the floating-point number
     * nearest the exact result; such a method is <i>correctly
     * rounded</i>.  A correctly rounded method is generally the best a
     * floating-point approximation can be; however, it is impractical for
     * many floating-point methods to be correctly rounded.  Instead, for
     * the <b> Math</b> class, a larger error bound of 1 or 2 ulps is
     * allowed for certain methods.  Informally, with a 1 ulp error bound,
     * when the exact result is a representable number, the exact result
     * should be returned as the computed result; otherwise, either of the
     * two floating-point values which bracket the exact result may be
     * returned.  For exact results large in magnitude, one of the
     * endpoints of the bracket may be infinite.  Besides accuracy at
     * individual arguments, maintaining proper relations between the
     * method at different arguments is also important.  Therefore, most
     * methods with more than 0.5 ulp errors are required to be
     * <i>semi-monotonic</i>: whenever the mathematical function is
     * non-decreasing, so is the floating-point approximation, likewise,
     * whenever the mathematical function is non-increasing, so is the
     * floating-point approximation.  Not all approximations that have 1
     * ulp accuracy will automatically meet the monotonicity requirements.
     *
     * <p>
     * The platform uses signed two's complement integer arithmetic with
     * int and long native types.  The developer should choose
     * the native type to ensure that arithmetic operations consistently
     * produce correct results, which in some cases means the operations
     * will not overflow the range of values of the computation.
     * The best practice is to choose the native type and algorithm to avoid
     * overflow. In cases where the size is <b> int</b> or <b> long</b> and
     * overflow errors need to be detected, the methods whose names end with
     * <b> Exact</b> throw an <b> ArithmeticException</b> when the results overflow.
     *
     * <h2><a id=Ieee754RecommendedOps>IEEE 754 Recommended
     * Operations</a></h2>
     *
     * The 2019 revision of the IEEE 754 floating-point standard includes
     * a section of recommended operations and the semantics of those
     * operations if they are included in a programming environment. The
     * recommended operations present in this class include <b style="color: orange">
     * sin</b>, <b style="color: orange"> cos</b>, <b style="color: orange"> tan</b>,
     * <b style="color: orange"> asin</b>, <b style="color: orange">
     * acos</b>, <b style="color: orange"> atan</b>, <b style="color: orange"> exp</b>, <b style="color: orange">
     * expm1</b>, <b style="color: orange"> log</b>, <b style="color: orange"> log10</b>, <b style="color: orange"> log1p</b>,
     * <b style="color: orange"> sinh</b>, <b style="color: orange"> cosh</b>, <b style="color: orange"> tanh</b>, <b style="color: orange">
     * hypot</b>, and <b style="color: orange"> pow</b>.  (The <b style="color: orange"> sqrt</b>
     * operation is a required part of IEEE 754 from a different section
     * of the standard.) The special case behavior of the recommended
     * operations generally follows the guidance of the IEEE 754
     * standard. However, the <b> pow</b> method defines different
     * behavior for some arguments, as noted in its <b style="color: green"> pow
     * specification</b>. The IEEE 754 standard defines its operations to be
     * correctly rounded, which is a more stringent quality of
     * implementation condition than required for most of the methods in
     * question that are also included in this class.
     *
     * @see <a href="https://standards.ieee.org/ieee/754/6210/">
     *      <cite>IEEE Standard for Floating-Point Arithmetic</cite></a>
     *
     * @author  Brunshweeck Tazeussong
     */
    class Math final: public Object {

    private:
        /**
         * Don't let anyone instantiate this class.
         */
        CORE_FAST Math() = default;

        /** Constant by which to multiply an angular value in degrees to obtain an
         * angular value in radians.
         */
        static CORE_FAST gdouble DEGREES_TO_RADIANS = 0.017453292519943295;

        /** Constant by which to multiply an angular value in radians to obtain an
         * angular value in degrees.
         */
        static CORE_FAST gdouble RADIANS_TO_DEGREES = 57.29577951308232;

        /**
         * Returns a floating-point power of two in the normal range.
         */
        static gdouble powerOfTwoD(gint n);

        /** Returns a floating-point power of two in the normal range.
         */
        static gfloat powerOfTwoF(gint n);

    public:
        /** The double value that is closer than any other to
         * <i>e</i>, the base of the natural logarithms.
         */
        static CORE_FAST gdouble E = 2.718281828459045;

        /** The double value that is closer than any other to
         * <i>pi</i> (&pi;), the ratio of the circumference of a circle to
         * its diameter.
         */
        static CORE_FAST gdouble PI = 3.141592653589793;

        /** The double value that is closer than any other to
         * <i>tau</i> (&tau;), the ratio of the circumference of a circle
         * to its radius.
         * <p>
         * Note:
         * The value of <i>pi</i> is one half that of <i>tau</i>; in other
         * words, <i>tau</i> is double <i>pi</i> .
         */
        static CORE_FAST gdouble TAU = 2.0 * PI;

        /**
         * The double value value that is closer than any other to &epsilon;
         */
        static CORE_FAST gdouble EPSILON = 2.220446049250313E-20;

        static const gdouble INF;

        static const gdouble NaN;

        /** Returns the trigonometric sine of an angle.
         * <p>
         * Special cases:
         * <ul>
         * <li> If the argument is NaN or an infinity, then the
         * result is NaN.
         * <li> If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         * </ul>
         * @param a
         *        An angle, in radians.
         */
        static gdouble sin(gdouble a);

        /** Returns the trigonometric cosine of an angle.
         * <p>
         * Special cases:
         * <ul>
         * <li> If the argument is NaN or an infinity, then the
         * result is NaN.
         * <li> If the argument is zero, then the result is a 1.0 with the
         * same sign as the argument.
         * </ul>
         * @param a
         *        An angle, in radians.
         */
        static gdouble cos(gdouble a);

        /** Returns the trigonometric tangent of an angle.
         * <p>
         * Special cases:
         * <ul>
         * <li> If the argument is NaN or an infinity, then the
         * result is NaN.
         * <li> If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         * </ul>
         * @param a
         *        An angle, in radians.
         */
        static gdouble tan(gdouble a);

        /** Returns the arc sine of a value; the returned angle is in the
         * range -<i>pi</i>/2 through <i>pi</i>/2.
         * <p>
         * Special cases:
         * <ul>
         * <li> If the argument is NaN or an infinity, then the
         * result is NaN.
         * <li> If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         * </ul>
         * <p>
         * The computed result must be within 1 ulp of the exact result.
         * Results must be semi-monotonic.
         * @param   a
         *          The value whose arc sine is to be returned.
         */
        static gdouble asin(gdouble a);

        /** Returns the arc cosine of a value; the returned angle is in the
         * range 0.0 through <i>pi</i>.
         * <p>
         * Special cases:
         * <ul>
         * <li> If the argument is NaN or an infinity, then the
         * result is NaN.
         * <li> If the argument is 1.0, then the result is a positive zero.
         * </ul>
         * @param   a
         *          The value whose arc cosine is to be returned.
         */
        static gdouble acos(gdouble a);

        /** Returns the arc tangent of a value; the returned angle is in the
         * range -<i>pi</i>/2 through <i>pi</i>/2.
         * <p>
         * Special cases:
         * <ul><li>If the argument is NaN, then the result is NaN.
         * <li>If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         * <li>If the argument is infinite, then the result is the closest value
         * to <i>pi</i>/2 with the same sign as the input.
         * </ul>
         * @param   a
         *          The value whose arc tangent is to be returned.
         */
        static gdouble atan(gdouble a);

        /** Converts an angle measured in deg to an approximately
         * equivalent angle measured in radians.  The conversion from
         * deg to radians is generally inexact.
         * @param deg
         *          An angle, in deg
         */
        static CORE_FAST gdouble toRadians(gdouble deg);

        /** Converts an angle measured in radians to an approximately
         * equivalent angle measured in degrees.  The conversion from
         * radians to degrees is generally inexact; users should
         * <i>not</i> expect cos(toRadians(90.0)) to exactly
         * equal 0.0.
         * @param   rad
         *             An angle, in radians
         */
        static gdouble toDegrees(gdouble rad);

        /** Returns Euler's number <i>e</i> raised to the power of a
         * double value.  Special cases:
         * <ul>
         * <li>If the argument is NaN, the result is NaN.
         * <li>If the argument is positive infinity, then the result is
         * positive infinity.
         * <li>If the argument is negative infinity, then the result is
         * positive zero.
         * <li>If the argument is zero, then the result is 1.0.
         * </ul>
         * @param   a
         *          The exponent to raise <i>e</i> to.
         */
        static gdouble exp(gdouble a);

        /** Returns the natural logarithm (base <i>e</i>) of a double
         * value.  Special cases:
         * <ul><li>If the argument is NaN or less than zero, then the result
         * is NaN.
         * <li>If the argument is positive infinity, then the result is
         * positive infinity.
         * <li>If the argument is positive zero or negative zero, then the
         * result is negative infinity.
         * <li>If the argument is 1.0, then the result is positive
         * zero.
         * </ul>
         * @param   a
         *          A value
         */
        static gdouble log(gdouble a);

        /** Returns the base 10 logarithm of a double value.
         * Special cases:
         * <ul><li>If the argument is NaN or less than zero, then the result
         * is NaN.
         * <li>If the argument is positive infinity, then the result is
         * positive infinity.
         * <li>If the argument is positive zero or negative zero, then the
         * result is negative infinity.
         * <li>If the argument is equal to 10<sup><i>n</i></sup> for
         * integer <i>n</i>, then the result is <i>n</i>. In particular,
         * if the argument is 1.0 (10<sup>0</sup>), then the
         * result is positive zero.
         * </ul>
         * @param   a   a value
         */
        static gdouble log10(gdouble a);

        /** Returns the correctly rounded positive square root of a
         * double value.
         * Special cases:
         * <ul><li>If the argument is NaN or less than zero, then the result
         * is NaN.
         * <li>If the argument is positive infinity, then the result is positive
         * infinity.
         * <li>If the argument is positive zero or negative zero, then the
         * result is the same as the argument.</ul>
         * Otherwise, the result is the double value closest to
         * the true mathematical square root of the argument value.
         * <p>
         * This method corresponds to the squareRoot operation defined in
         * IEEE 754.
         * @param   a   a value.
         */
        static gdouble sqrt(gdouble a);


        /** Returns the cube root of a double value.  For
         * positive finite x, cbrt(-x) == -cbrt(x); that is, the cube root
         * of a negative value is the negative of the cube root of that value's magnitude.
         * Special cases:
         * <ul>
         * <li>If the argument is NaN, then the result is NaN.
         * <li>If the argument is infinite, then the result is an infinity
         * with the same sign as the argument.
         * <li>If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         * </ul>
         * @param a
         *        a value.
         */
        static gdouble cbrt(gdouble a);

        /** Computes the remainder operation on two arguments as prescribed
         * by the IEEE 754 standard.
         * The remainder value is mathematically equal to
         * <code>f1 - f2</code> &times; <i>n</i>,
         * where <i>n</i> is the mathematical integer closest to the exact
         * mathematical value of the quotient f1/f2, and if two
         * mathematical integers are equally close to f1/f2,
         * then <i>n</i> is the integer that is even. If the remainder is
         * zero, its sign is the same as the sign of the first argument.
         * Special cases:
         * <ul><li>If either argument is NaN, or the first argument is infinite,
         * or the second argument is positive zero or negative zero, then the
         * result is NaN.
         * <li>If the first argument is finite and the second argument is
         * infinite, then the result is the same as the first argument.</ul>
         * @param   f1   the dividend.
         * @param   f2   the divisor.
         *          f2.
         */
        static gdouble fmod(gdouble f1, gdouble f2);

        /** Returns the smallest (closest to negative infinity)
         * double value that is greater than or equal to the
         * argument and is equal to a mathematical integer. Special cases:
         * <ul><li>If the argument value is already equal to a
         * mathematical integer, then the result is the same as the
         * argument.  <li>If the argument is NaN or an infinity or
         * positive zero or negative zero, then the result is the same as
         * the argument.  <li>If the argument value is less than zero but
         * greater than -1.0, then the result is negative zero.</ul> Note
         * that the value of Math.ceil(x) is exactly the
         * value of -Math.floor(-x).
         * <p>
         * This method corresponds to the roundToIntegralTowardPositive
         * operation defined in IEEE 754.
         * @param   a   a value.
         *          floating-point value that is greater than or equal to
         *          the argument and is equal to a mathematical integer.
         */
        static gdouble ceil(gdouble a);

        /** Returns the largest (closest to positive infinity)
         * double value that is less than or equal to the
         * argument and is equal to a mathematical integer. Special cases:
         * <ul><li>If the argument value is already equal to a
         * mathematical integer, then the result is the same as the
         * argument.  <li>If the argument is NaN or an infinity or
         * positive zero or negative zero, then the result is the same as
         * the argument.</ul>
         * <p>
         * This method corresponds to the roundToIntegralTowardNegative
         * operation defined in IEEE 754.
         * @param   a   a value.
         */
        static gdouble floor(gdouble a);

        /** Returns the double value that is closest in value
         * to the argument and is equal to a mathematical integer. If two
         * double values that are mathematical integers are
         * equally close, the result is the integer value that is
         * even. Special cases:
         * <ul><li>If the argument value is already equal to a mathematical
         * integer, then the result is the same as the argument.
         * <li>If the argument is NaN or an infinity or positive zero or negative
         * zero, then the result is the same as the argument.</ul>
         * <p>
         * This method corresponds to the roundToIntegralTiesToEven
         * operation defined in IEEE 754.
         * @param   a   a double value.
         */
        static gdouble rint(gdouble a);

        /** Returns the angle <i>theta</i> from the conversion of rectangular
         * coordinates (x, y) to polar coordinates (r, <i>theta</i>).
         * This method computes the phase <i>theta</i> by computing an arc tangent
         * of y/x in the range of -<i>pi</i> to <i>pi</i>. Special
         * cases:
         * <ul><li>If either argument is NaN, then the result is NaN.
         * <li>If the first argument is positive zero and the second argument
         * is positive, or the first argument is positive and finite and the
         * second argument is positive infinity, then the result is positive
         * zero.
         * <li>If the first argument is negative zero and the second argument
         * is positive, or the first argument is negative and finite and the
         * second argument is positive infinity, then the result is negative zero.
         * <li>If the first argument is positive zero and the second argument
         * is negative, or the first argument is positive and finite and the
         * second argument is negative infinity, then the result is the
         * double value closest to <i>pi</i>.
         * <li>If the first argument is negative zero and the second argument
         * is negative, or the first argument is negative and finite and the
         * second argument is negative infinity, then the result is the
         * double value closest to -<i>pi</i>.
         * <li>If the first argument is positive and the second argument is
         * positive zero or negative zero, or the first argument is positive
         * infinity and the second argument is finite, then the result is the
         * double value closest to <i>pi</i>/2.
         * <li>If the first argument is negative and the second argument is
         * positive zero or negative zero, or the first argument is negative
         * infinity and the second argument is finite, then the result is the
         * double value closest to -<i>pi</i>/2.
         * <li>If both arguments are positive infinity, then the result is the
         * double value closest to <i>pi</i>/4.
         * <li>If the first argument is positive infinity and the second argument
         * is negative infinity, then the result is the double
         * value closest to 3*<i>pi</i>/4.
         * <li>If the first argument is negative infinity and the second argument
         * is positive infinity, then the result is the double value
         * closest to -<i>pi</i>/4.
         * <li>If both arguments are negative infinity, then the result is the
         * double value closest to -3*<i>pi</i>/4.</ul>
         * <p>The computed result must be within 2 ulps of the exact result.
         * Results must be semi-monotonic.
         * <p>
         * For <i>y</i> with a positive sign and finite nonzero
         * <i>x</i>, the exact mathematical value of atan2 is
         * equal to:
         * <ul>
         * <li>If <i>x</i> > 0, atan(abs(<i>y</i>/<i>x</i>))
         * <li>If <i>x</i> &lt; 0, &pi; - atan(abs(<i>y</i>/<i>x</i>))
         * </ul>
         * @param   y   the ordinate coordinate
         * @param   x   the abscissa coordinate
         */
        static gdouble atan2(gdouble y, gdouble x);

        /** Returns the value of the first argument raised to the power of the
         * second argument. Special cases:
         * <ul><li>If the second argument is positive or negative zero, then the
         * result is 1.0.
         * <li>If the second argument is 1.0, then the result is the same as the
         * first argument.
         * <li>If the second argument is NaN, then the result is NaN.
         * <li>If the first argument is NaN and the second argument is nonzero,
         * then the result is NaN.
         * <li>If
         * <ul>
         * <li>the absolute value of the first argument is greater than 1
         * and the second argument is positive infinity, or
         * <li>the absolute value of the first argument is less than 1 and
         * the second argument is negative infinity,
         * </ul>
         * then the result is positive infinity.
         * <li>If
         * <ul>
         * <li>the absolute value of the first argument is greater than 1 and
         * the second argument is negative infinity, or
         * <li>the absolute value of the
         * first argument is less than 1 and the second argument is positive
         * infinity,
         * </ul>
         * then the result is positive zero.
         * <li>If the absolute value of the first argument equals 1 and the
         * second argument is infinite, then the result is NaN.
         * <li>If
         * <ul>
         * <li>the first argument is positive zero and the second argument
         * is greater than zero, or
         * <li>the first argument is positive infinity and the second
         * argument is less than zero,
         * </ul>
         * then the result is positive zero.
         * <li>If
         * <ul>
         * <li>the first argument is positive zero and the second argument
         * is less than zero, or
         * <li>the first argument is positive infinity and the second
         * argument is greater than zero,
         * </ul>
         * then the result is positive infinity.
         * <li>If
         * <ul>
         * <li>the first argument is negative zero and the second argument
         * is greater than zero but not a finite odd integer, or
         * <li>the first argument is negative infinity and the second
         * argument is less than zero but not a finite odd integer,
         * </ul>
         * then the result is positive zero.
         * <li>If
         * <ul>
         * <li>the first argument is negative zero and the second argument
         * is a positive finite odd integer, or
         * <li>the first argument is negative infinity and the second
         * argument is a negative finite odd integer,
         * </ul>
         * then the result is negative zero.
         * <li>If
         * <ul>
         * <li>the first argument is negative zero and the second argument
         * is less than zero but not a finite odd integer, or
         * <li>the first argument is negative infinity and the second
         * argument is greater than zero but not a finite odd integer,
         * </ul>
         * then the result is positive infinity.
         * <li>If
         * <ul>
         * <li>the first argument is negative zero and the second argument
         * is a negative finite odd integer, or
         * <li>the first argument is negative infinity and the second
         * argument is a positive finite odd integer,
         * </ul>
         * then the result is negative infinity.
         * <li>If the first argument is finite and less than zero
         * <ul>
         * <li> if the second argument is a finite even integer, the
         * result is equal to the result of raising the absolute value of
         * the first argument to the power of the second argument
         * <li>if the second argument is a finite odd integer, the result
         * is equal to the negative of the result of raising the absolute
         * value of the first argument to the power of the second
         * argument
         * <li>if the second argument is finite and not an integer, then
         * the result is NaN.
         * </ul>
         * <li>If both arguments are integers, then the result is exactly equal
         * to the mathematical result of raising the first argument to the power
         * of the second argument if that result can in fact be represented
         * exactly as a double value.</ul>
         * <p>(In the foregoing descriptions, a floating-point value is
         * considered to be an integer if and only if it is finite and a
         * fixed point of the method ceil or, equivalently, a fixed point of the method
         * floor. A value is a fixed point of a one-argument
         * method if and only if the result of applying the method to the
         * value is equal to the value.)
         * <p>The computed result must be within 1 ulp of the exact result.
         * Results must be semi-monotonic.
         * <p>
         * The special cases definitions of this method differ from the
         * special case definitions of the IEEE 754 recommended
         * pow operation for &plusmn;1.0 raised to an infinite
         * power. This method treats such cases as indeterminate and
         * specifies a NaN is returned. The IEEE 754 specification treats
         * the infinite power as a large integer (large-magnitude
         * floating-point numbers are numerically integers, specifically
         * even integers) and therefore specifies 1.0 be returned.
         * @param   a   the base.
         * @param   b   the exponent.
         */
        static gdouble pow(gdouble a, gdouble b);

        /** Returns the closest int to the argument, with ties
         * rounding to positive infinity.
         * <p>
         * Special cases:
         * <ul><li>If the argument is NaN, the result is 0.
         * <li>If the argument is negative infinity or any value less than or
         * equal to the value of Integer.MIN_VALUE, the result is
         * equal to the value of Integer.MIN_VALUE.
         * <li>If the argument is positive infinity or any value greater than or
         * equal to the value of Integer.MAX_VALUE, the result is
         * equal to the value of Integer.MAX_VALUE.</ul>
         * @param   a   a floating-point value to be rounded to an integer.
         */
        static gint round(gfloat a);

        /** Returns the closest long to the argument, with ties
         * rounding to positive infinity.
         * <p>Special cases:
         * <ul><li>If the argument is NaN, the result is 0.
         * <li>If the argument is negative infinity or any value less than or
         * equal to the value of Long.MIN_VALUE, the result is
         * equal to the value of Long.MIN_VALUE.
         * <li>If the argument is positive infinity or any value greater than or
         * equal to the value of Long.MAX_VALUE, the result is
         * equal to the value of Long.MAX_VALUE.</ul>
         * @param   a   a floating-point value to be rounded to a
         *          long.
         */
        static glong round(gdouble a);

        /** Returns a double value with a positive sign, greater
         * than or equal to 0.0 and less than 1.0.
         * Returned values are chosen pseudo-randomly with (approximately)
         * uniform distribution from that range.
         * This new pseudo-random-number generator is used thereafter for
         * all calls to this method and is used nowhere else.
         * <p>This method is properly synchronized to allow correct use by
         * more than one thread. However, if many threads need to comparator
         * pseudorandom numbers at a great rate, it may reduce contention
         * for each thread to have its own pseudorandom-number generator.
         */
        static gdouble random();

        /** Returns the sum of its arguments,
         * throwing an exception if the result overflows an int.
         * @param x the first value
         * @param y the second value
         * @throws ArithmeticException if the result overflows an int
         */
        static gint addExact(gint x, gint y);

        /** Returns the sum of its arguments,
         * throwing an exception if the result overflows a long.
         * @param x the first value
         * @param y the second value
         * @throws ArithmeticException if the result overflows a long
         */
        static glong addExact(glong x, glong y);

        /** Returns the difference of the arguments,
         * throwing an exception if the result overflows an int.
         * @param x the first value
         * @param y the second value to subtract from the first
         * @throws ArithmeticException if the result overflows an int
         */
        static gint subtractExact(gint x, gint y);

        /** Returns the difference of the arguments,
         * throwing an exception if the result overflows a long.
         * @param x the first value
         * @param y the second value to subtract from the first
         * @throws ArithmeticException if the result overflows a long
         */
        static glong subtractExact(glong x, glong y);

        /** Returns the product of the arguments,
         * throwing an exception if the result overflows an int.
         * @param x the first value
         * @param y the second value
         * @throws ArithmeticException if the result overflows an int
         */
        static gint multiplyExact(gint x, gint y);

        /** Returns the product of the arguments, throwing an exception if the result
         * overflows a long.
         * @param x the first value
         * @param y the second value
         * @throws ArithmeticException if the result overflows a long
         */
        static glong multiplyExact(glong x, gint y);

        /** Returns the product of the arguments,
         * throwing an exception if the result overflows a long.
         * @param x the first value
         * @param y the second value
         * @throws ArithmeticException if the result overflows a long
         */
        static glong multiplyExact(glong x, glong y);

        /** Returns the quotient of the arguments, throwing an exception if the
         * result overflows an int.  Such overflow occurs in this method if
         * x is Integer.MIN_VALUE and y is -1.
         * In contrast, if Integer.MIN_VALUE / -1 were evaluated directly,
         * the result would be Integer.MIN_VALUE and no exception would be
         * thrown.
         * <p>
         * If y is zero, an ArithmeticException is thrown
         * <p>
         * The built-in remainder operator "%" is a suitable counterpart
         * both for this method and for the built-in division operator "/".
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if y is zero or the quotient
         * overflows an int
         */
        static gint divideExact(gint x, gint y);

        /** Returns the quotient of the arguments, throwing an exception if the
         * result overflows a long.  Such overflow occurs in this method if
         * x is Long.MIN_VALUE and y is -1.
         * In contrast, if Long.MIN_VALUE / -1 were evaluated directly,
         * the result would be Long.MIN_VALUE and no exception would be
         * thrown.
         * <p>
         * If y is zero, an ArithmeticException is thrown
         * <p>
         * The built-in remainder operator "%" is a suitable counterpart
         * both for this method and for the built-in division operator "/".
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if y is zero or the quotient
         * overflows a long
         */
        static glong divideExact(glong x, glong y);

        /** Returns the largest (closest to positive infinity)
         * int value that is less than or equal to the algebraic quotient.
         * This method is identical to ::floorDiv(int,int) except that it
         * throws an ArithmeticException when the dividend is
         * Integer.MIN_VALUE and the divisor is
         * -1 instead of ignoring the integer overflow and returning
         * Integer.MIN_VALUE.
         * <p>
         * The floor modulus method ::floorMod(int,int) is a suitable
         * counterpart both for this method and for the ::floorDiv(int,int)
         * method.
         * <p>
         * For examples, see ::floorDiv(int, int).
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if the divisor y is zero, or the
         * dividend x is Integer.MIN_VALUE and the divisor y
         * is -1.
         */
        static gint floorDivExact(gint x, gint y);

        /** Returns the largest (closest to positive infinity)
         * long value that is less than or equal to the algebraic quotient.
         * This method is identical to ::floorDiv(long,long) except that it
         * throws an ArithmeticException when the dividend is
         * Long.MIN_VALUE and the divisor is
         * -1 instead of ignoring the integer overflow and returning
         * Long.MIN_VALUE.
         * <p>
         * The floor modulus method ::floorMod(long,long) is a suitable
         * counterpart both for this method and for the ::floorDiv(long,long)
         * method.
         * <p>
         * For examples, see ::floorDiv(int, int).
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if the divisor y is zero, or the
         * dividend x is Long.MIN_VALUE and the divisor y
         * is -1.
         */
        static glong floorDivExact(glong x, glong y);

        /** Returns the smallest (closest to negative infinity)
         * int value that is greater than or equal to the algebraic quotient.
         * This method is identical to ::ceilDiv(int,int) except that it
         * throws an ArithmeticException when the dividend is
         * the divisor is
         * -1 instead of ignoring the integer overflow and returning
         * Integer.MIN_VALUE.
         * <p>
         * The ceil modulus method ::ceilMod(int,int) is a suitable
         * counterpart both for this method and for the ::ceilDiv(int,int)
         * method.
         * <p>
         * For examples, see ::ceilDiv(int, int).
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if the divisor y is zero, or the
         * dividend x is Integer.MIN_VALUE and the divisor y
         * is -1.
         */
        static gint ceilDivExact(gint x, gint y);

        /** Returns the smallest (closest to negative infinity)
         * long value that is greater than or equal to the algebraic quotient.
         * This method is identical to ::ceilDiv(long,long) except that it
         * throws an ArithmeticException when the dividend is
         * the divisor is
         * -1 instead of ignoring the integer overflow and returning
         * Long.MIN_VALUE.
         * <p>
         * The ceil modulus method ::ceilMod(long,long) is a suitable
         * counterpart both for this method and for the ::ceilDiv(long,long)
         * method.
         * <p>
         * For examples, see ::ceilDiv(int, int).
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if the divisor y is zero, or the
         * dividend x is Long.MIN_VALUE and the divisor y
         * is -1.
         */
        static glong ceilDivExact(glong x, glong y);

        /** Returns the argument incremented by one, throwing an exception if the
         * result overflows an int.
         * The overflow only occurs for the maximum value.
         * @param a the value to increment
         * @throws ArithmeticException if the result overflows an int
         */
        static gint incrementExact(gint a);

        /** Returns the argument incremented by one, throwing an exception if the
         * result overflows a long.
         * The overflow only occurs for the maximum value.
         * @param a the value to increment
         * @throws ArithmeticException if the result overflows a long
         */
        static glong incrementExact(glong a);

        /** Returns the argument decremented by one, throwing an exception if the
         * result overflows an int.
         * The overflow only occurs for the minimum value.
         * @param a the value to decrement
         * @throws ArithmeticException if the result overflows an int
         */
        static gint decrementExact(gint a);

        /** Returns the argument decremented by one, throwing an exception if the
         * result overflows a long.
         * The overflow only occurs for the minimum value.
         * @param a the value to decrement
         * @throws ArithmeticException if the result overflows a long
         */
        static glong decrementExact(glong a);

        /** Returns the negation of the argument, throwing an exception if the
         * result overflows an int.
         * The overflow only occurs for the minimum value.
         * @param a the value to negate
         * @throws ArithmeticException if the result overflows an int
         */
        static gint negateExact(gint a);

        /** Returns the negation of the argument, throwing an exception if the
         * result overflows a long.
         * The overflow only occurs for the minimum value.
         * @param a the value to negate
         * @throws ArithmeticException if the result overflows a long
         */
        static glong negateExact(glong a);

        /** Returns the value of the long argument,
         * throwing an exception if the value overflows an int.
         * @param value the long value
         * @throws ArithmeticException if the argument overflows an int
         */
        static gint toIntExact(glong value);

        /** Returns the exact mathematical product of the arguments.
         * @param x the first value
         * @param y the second value
         */
        static glong multiplyFull(gint x, gint y);

        /** Returns as a long the most significant 64 bits of the 128-bit
         * product of two 64-bit factors.
         * @param x the first value
         * @param y the second value
         */
        static glong multiplyHigh(glong x, glong y);

        /** Returns as a long the most significant 64 bits of the unsigned
         * 128-bit product of two unsigned 64-bit factors.
         * @param x the first value
         * @param y the second value
         */
        static glong unsignedMultiplyHigh(glong x, glong y);

        /** Returns the largest (closest to positive infinity)
         * int value that is less than or equal to the algebraic quotient.
         * There is one special case: if the dividend is
         * the divisor is -1,
         * then integer overflow occurs and
         * the result is equal to Integer.MIN_VALUE.
         * <p>
         * Normal integer division operates under the round to zero rounding mode
         * (truncation).  This operation instead acts under the round toward
         * negative infinity (floor) rounding mode.
         * The floor rounding mode gives different results from truncation
         * when the exact quotient is not an integer and is negative.
         * <ul>
         *   <li>If the signs of the arguments are the same, the results of
         *       floorDiv and the / operator are the same.  <br>
         *       For example, floorDiv(4, 3) == 1 and (4 / 3) == 1.</li>
         *   <li>If the signs of the arguments are different, floorDiv
         *       returns the largest integer less than or equal to the quotient
         *       while the / operator returns the smallest integer greater
         *       than or equal to the quotient.
         *       They differ if and only if the quotient is not an integer.<br>
         *       For example, floorDiv(-4, 3) == -2,
         *       whereas (-4 / 3) == -1.
         *   </li>
         * </ul>
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if the divisor y is zero
         */
        static gint floorDiv(gint x, gint y);

        /** Returns the largest (closest to positive infinity)
         * long value that is less than or equal to the algebraic quotient.
         * There is one special case: if the dividend is
         * the divisor is -1,
         * then integer overflow occurs and
         * the result is equal to Long.MIN_VALUE.
         * <p>
         * Normal integer division operates under the round to zero rounding mode
         * (truncation).  This operation instead acts under the round toward
         * negative infinity (floor) rounding mode.
         * The floor rounding mode gives different results from truncation
         * when the exact result is not an integer and is negative.
         * <p>
         * For examples, see ::floorDiv(int, int).
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if the divisor y is zero
         */
        static glong floorDiv(glong x, gint y);

        /** Returns the largest (closest to positive infinity)
         * long value that is less than or equal to the algebraic quotient.
         * There is one special case: if the dividend is
         * the divisor is -1,
         * then integer overflow occurs and
         * the result is equal to Long.MIN_VALUE.
         * <p>
         * Normal integer division operates under the round to zero rounding mode
         * (truncation).  This operation instead acts under the round toward
         * negative infinity (floor) rounding mode.
         * The floor rounding mode gives different results from truncation
         * when the exact result is not an integer and is negative.
         * <p>
         * For examples, see ::floorDiv(int, int).
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if the divisor y is zero
         */
        static glong floorDiv(glong x, glong y);

        /** Returns the floor modulus of the int arguments.
         * <p>
         * The floor modulus is r = x - (floorDiv(x, y) * y),
         * has the same sign as the divisor y or is zero, and
         * is in the range of -abs(y) < r < +abs(y).
         * <p>
         * The relationship between floorDiv and floorMod is such that:
         * <ul>
         *   <li>floorDiv(x, y) * y + floorMod(x, y) == x</li>
         * </ul>
         * <p>
         * The difference in values between floorMod and the % operator
         * is due to the difference between floorDiv and the /
         * operator, as detailed in floorDiv(int, int).
         * <p>
         * Examples:
         * <ul>
         *   <li>Regardless of the signs of the arguments, floorMod(x, y)
         *       is zero exactly when x % y is zero as well.</li>
         *   <li>If neither floorMod(x, y) nor x % y is zero,
         *       they differ exactly when the signs of the arguments differ.<br>
         *       <ul>
         *       <li>floorMod(+4, +3) == +1;   and (+4 % +3) == +1</li>
         *       <li>floorMod(-4, -3) == -1;   and (-4 % -3) == -1</li>
         *       <li>floorMod(+4, -3) == -2;   and (+4 % -3) == +1</li>
         *       <li>floorMod(-4, +3) == +2;   and (-4 % +3) == -1</li>
         *       </ul>
         *   </li>
         * </ul>
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if the divisor y is zero
         */
        static gint floorMod(gint x, gint y);

        /** Returns the floor modulus of the long and int arguments.
         * <p>
         * The floor modulus is r = x - (floorDiv(x, y) * y),
         * has the same sign as the divisor y or is zero, and
         * is in the range of -abs(y) < r < +abs(y).
         * <p>
         * The relationship between floorDiv and floorMod is such that:
         * <ul>
         *   <li>floorDiv(x, y) * y + floorMod(x, y) == x</li>
         * </ul>
         * <p>
         * For examples, see ::floorMod(int, int).
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if the divisor y is zero
         */
        static gint floorMod(glong x, gint y);

        /** Returns the floor modulus of the long arguments.
         * <p>
         * The floor modulus is r = x - (floorDiv(x, y) * y),
         * has the same sign as the divisor y or is zero, and
         * is in the range of -abs(y) < r < +abs(y).
         * <p>
         * The relationship between floorDiv and floorMod is such that:
         * <ul>
         *   <li>floorDiv(x, y) * y + floorMod(x, y) == x</li>
         * </ul>
         * <p>
         * For examples, see ::floorMod(int, int).
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if the divisor y is zero
         */
        static glong floorMod(glong x, glong y);

        /** Returns the smallest (closest to negative infinity)
         * int value that is greater than or equal to the algebraic quotient.
         * There is one special case: if the dividend is
         * the divisor is -1,
         * then integer overflow occurs and
         * the result is equal to Integer.MIN_VALUE.
         * <p>
         * Normal integer division operates under the round to zero rounding mode
         * (truncation).  This operation instead acts under the round toward
         * positive infinity (ceiling) rounding mode.
         * The ceiling rounding mode gives different results from truncation
         * when the exact quotient is not an integer and is positive.
         * <ul>
         *   <li>If the signs of the arguments are different, the results of
         *       ceilDiv and the / operator are the same.  <br>
         *       For example, ceilDiv(-4, 3) == -1 and (-4 / 3) == -1.</li>
         *   <li>If the signs of the arguments are the same, ceilDiv
         *       returns the smallest integer greater than or equal to the quotient
         *       while the / operator returns the largest integer less
         *       than or equal to the quotient.
         *       They differ if and only if the quotient is not an integer.<br>
         *       For example, ceilDiv(4, 3) == 2,
         *       whereas (4 / 3) == 1.
         *   </li>
         * </ul>
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if the divisor y is zero
         */
        static gint ceilDiv(gint x, gint y);

        /** Returns the smallest (closest to negative infinity)
         * long value that is greater than or equal to the algebraic quotient.
         * There is one special case: if the dividend is
         * the divisor is -1,
         * then integer overflow occurs and
         * the result is equal to Long.MIN_VALUE.
         * <p>
         * Normal integer division operates under the round to zero rounding mode
         * (truncation).  This operation instead acts under the round toward
         * positive infinity (ceiling) rounding mode.
         * The ceiling rounding mode gives different results from truncation
         * when the exact result is not an integer and is positive.
         * <p>
         * For examples, see ::ceilDiv(int, int).
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if the divisor y is zero
         */
        static glong ceilDiv(glong x, gint y);

        /** Returns the smallest (closest to negative infinity)
         * long value that is greater than or equal to the algebraic quotient.
         * There is one special case: if the dividend is
         * the divisor is -1,
         * then integer overflow occurs and
         * the result is equal to Long.MIN_VALUE.
         * <p>
         * Normal integer division operates under the round to zero rounding mode
         * (truncation).  This operation instead acts under the round toward
         * positive infinity (ceiling) rounding mode.
         * The ceiling rounding mode gives different results from truncation
         * when the exact result is not an integer and is positive.
         * <p>
         * For examples, see ::ceilDiv(int, int).
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if the divisor y is zero
         */
        static glong ceilDiv(glong x, glong y);

        /** Returns the ceiling modulus of the int arguments.
         * <p>
         * The ceiling modulus is r = x - (ceilDiv(x, y) * y),
         * has the opposite sign as the divisor y or is zero, and
         * is in the range of -abs(y) < r < +abs(y).
         * <p>
         * The relationship between ceilDiv and ceilMod is such that:
         * <ul>
         *   <li>ceilDiv(x, y) * y + ceilMod(x, y) == x</li>
         * </ul>
         * <p>
         * The difference in values between ceilMod and the % operator
         * is due to the difference between ceilDiv and the /
         * operator, as detailed in ceilDiv(int, int).
         * <p>
         * Examples:
         * <ul>
         *   <li>Regardless of the signs of the arguments, ceilMod(x, y)
         *       is zero exactly when x % y is zero as well.</li>
         *   <li>If neither ceilMod(x, y) nor x % y is zero,
         *       they differ exactly when the signs of the arguments are the same.<br>
         *       <ul>
         *       <li>ceilMod(+4, +3) == -2;   and (+4 % +3) == +1</li>
         *       <li>ceilMod(-4, -3) == +2;   and (-4 % -3) == -1</li>
         *       <li>ceilMod(+4, -3) == +1;   and (+4 % -3) == +1</li>
         *       <li>ceilMod(-4, +3) == -1;   and (-4 % +3) == -1</li>
         *       </ul>
         *   </li>
         * </ul>
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if the divisor y is zero
         */
        static gint ceilMod(gint x, gint y);

        /** Returns the ceiling modulus of the long and int arguments.
         * <p>
         * The ceiling modulus is r = x - (ceilDiv(x, y) * y),
         * has the opposite sign as the divisor y or is zero, and
         * is in the range of -abs(y) < r < +abs(y).
         * <p>
         * The relationship between ceilDiv and ceilMod is such that:
         * <ul>
         *   <li>ceilDiv(x, y) * y + ceilMod(x, y) == x</li>
         * </ul>
         * <p>
         * For examples, see ::ceilMod(int, int).
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if the divisor y is zero
         */
        static gint ceilMod(glong x, gint y);

        /** Returns the ceiling modulus of the long arguments.
         * <p>
         * The ceiling modulus is r = x - (ceilDiv(x, y) * y),
         * has the opposite sign as the divisor y or is zero, and
         * is in the range of -abs(y) < r < +abs(y).
         * <p>
         * The relationship between ceilDiv and ceilMod is such that:
         * <ul>
         *   <li>ceilDiv(x, y) * y + ceilMod(x, y) == x</li>
         * </ul>
         * <p>
         * For examples, see ::ceilMod(int, int).
         * @param x the dividend
         * @param y the divisor
         * @throws ArithmeticException if the divisor y is zero
         */
        static glong ceilMod(glong x, glong y);

        /** Returns the absolute value of an int value.
         * If the argument is not negative, the argument is returned.
         * If the argument is negative, the negation of the argument is returned.
         * <p>Note that if the argument is equal to the value of
         * Integer.MIN_VALUE, the most negative representable int
         * value, the result is that same value, which is negative. In
         * contrast, the Math.absExact(int) method throws an
         * ArithmeticException for this value.
         * @param   a   the argument whose absolute value is to be determined
         */
        static CORE_FAST gint abs(gint a) {
            return (a < 0) ? -a : a;
        }

        /** Returns the mathematical absolute value of an int value
         * if it is exactly representable as an int, throwing
         * ArithmeticException if the result overflows the
         * positive int range.
         * <p>Since the range of two's complement integers is asymmetric
         * with one additional negative value , the
         * mathematical absolute value of Integer.MIN_VALUE
         * overflows the positive int range, so an exception is
         * thrown for that argument.
         * @param  a  the argument whose absolute value is to be determined
         * @throws ArithmeticException if the argument is Integer.MIN_VALUE
         */
        static gint absExact(gint a);

        /** Returns the absolute value of a long value.
         * If the argument is not negative, the argument is returned.
         * If the argument is negative, the negation of the argument is returned.
         * <p>Note that if the argument is equal to the value of
         * Long.MIN_VALUE, the most negative representable long
         * value, the result is that same value, which is negative. In
         * contrast, the Math.absExact(long) method throws an
         * ArithmeticException for this value.
         * @param   a   the argument whose absolute value is to be determined
         */
        static CORE_FAST glong abs(glong a) {
            return (a < 0) ? -a : a;
        }

        /** Returns the mathematical absolute value of an long value
         * if it is exactly representable as an long, throwing
         * ArithmeticException if the result overflows the
         * positive long range.
         * <p>Since the range of two's complement integers is asymmetric
         * with one additional negative value , the
         * mathematical absolute value of Long.MIN_VALUE overflows
         * the positive long range, so an exception is thrown for
         * that argument.
         * @param  a  the argument whose absolute value is to be determined
         * @throws ArithmeticException if the argument is Long.MIN_VALUE
         */
        static glong absExact(glong a);

        /** Returns the absolute value of a float value.
         * If the argument is not negative, the argument is returned.
         * If the argument is negative, the negation of the argument is returned.
         * Special cases:
         * <ul><li>If the argument is positive zero or negative zero, the
         * result is positive zero.
         * <li>If the argument is infinite, the result is positive infinity.
         * <li>If the argument is NaN, the result is NaN.</ul>
         * <p> As implied by the above, one valid implementation of
         * this method is given by the expression below which computes a
         * float with the same exponent and significand as the
         * argument but with a guaranteed zero sign bit indicating a
         * positive value:<br>
         * Float.fromIntBits(0x7fffffff & Float.toIntBits(a))
         * @param   a   the argument whose absolute value is to be determined
         */
        static gfloat abs(gfloat a);

        /** Returns the absolute value of a double value.
         * If the argument is not negative, the argument is returned.
         * If the argument is negative, the negation of the argument is returned.
         * Special cases:
         * <ul><li>If the argument is positive zero or negative zero, the result
         * is positive zero.
         * <li>If the argument is infinite, the result is positive infinity.
         * <li>If the argument is NaN, the result is NaN.</ul>
         * <p> As implied by the above, one valid implementation of
         * this method is given by the expression below which computes a
         * double with the same exponent and significand as the
         * argument but with a guaranteed zero sign bit indicating a
         * positive value:<br>
         * Double.fromLongBits((Double.doubleToRawLongBits(a)<<1)>>1)
         * @param   a   the argument whose absolute value is to be determined
         */
        static gdouble abs(gdouble a);

        /** Returns the greater of two int values. That is, the
         * result is the argument closer to the value of
         * Integer.MAX_VALUE. If the arguments have the same value,
         * the result is that same value.
         * @param   a   an argument.
         * @param   b   another argument.
         */
        static CORE_FAST gint max(gint a, gint b) {
            return (a >= b) ? a : b;
        }

        /** Returns the greater of two long values. That is, the
         * result is the argument closer to the value of
         * Long.MAX_VALUE. If the arguments have the same value,
         * the result is that same value.
         * @param   a   an argument.
         * @param   b   another argument.
         */
        static CORE_FAST glong max(glong a, glong b) {
            return (a >= b) ? a : b;
        }

        /** Returns the greater of two float values.  That is,
         * the result is the argument closer to positive infinity. If the
         * arguments have the same value, the result is that same
         * value. If either value is NaN, then the result is NaN.  Unlike
         * the numerical comparison operators, this method considers
         * negative zero to be strictly smaller than positive zero. If one
         * argument is positive zero and the other negative zero, the
         * result is positive zero.
         * <p>
         * This method corresponds to the maximum operation defined in
         * IEEE 754.
         * @param   a   an argument.
         * @param   b   another argument.
         */
        static gfloat max(gfloat a, gfloat b);

        /** Returns the greater of two double values.  That
         * is, the result is the argument closer to positive infinity. If
         * the arguments have the same value, the result is that same
         * value. If either value is NaN, then the result is NaN.  Unlike
         * the numerical comparison operators, this method considers
         * negative zero to be strictly smaller than positive zero. If one
         * argument is positive zero and the other negative zero, the
         * result is positive zero.
         * <p>
         * This method corresponds to the maximum operation defined in
         * IEEE 754.
         * @param   a   an argument.
         * @param   b   another argument.
         */
        static gdouble max(gdouble a, gdouble b);

        /** Returns the smaller of two int values. That is,
         * the result the argument closer to the value of
         * Integer.MIN_VALUE.  If the arguments have the same
         * value, the result is that same value.
         * @param   a   an argument.
         * @param   b   another argument.
         */
        static CORE_FAST gint min(gint a, gint b) {
            return (a <= b) ? a : b;
        }

        /** Returns the smaller of two long values. That is,
         * the result is the argument closer to the value of
         * Long.MIN_VALUE. If the arguments have the same
         * value, the result is that same value.
         * @param   a   an argument.
         * @param   b   another argument.
         */
        static CORE_FAST glong min(glong a, glong b) {
            return (a <= b) ? a : b;
        }

        /** Returns the smaller of two float values.  That is,
         * the result is the value closer to negative infinity. If the
         * arguments have the same value, the result is that same
         * value. If either value is NaN, then the result is NaN.  Unlike
         * the numerical comparison operators, this method considers
         * negative zero to be strictly smaller than positive zero.  If
         * one argument is positive zero and the other is negative zero,
         * the result is negative zero.
         * <p>
         * This method corresponds to the minimum operation defined in
         * IEEE 754.
         * @param   a   an argument.
         * @param   b   another argument.
         */
        static gfloat min(gfloat a, gfloat b);

        /** Returns the smaller of two double values.  That
         * is, the result is the value closer to negative infinity. If the
         * arguments have the same value, the result is that same
         * value. If either value is NaN, then the result is NaN.  Unlike
         * the numerical comparison operators, this method considers
         * negative zero to be strictly smaller than positive zero. If one
         * argument is positive zero and the other is negative zero, the
         * result is negative zero.
         * <p>
         * This method corresponds to the minimum operation defined in
         * IEEE 754.
         * @param   a   an argument.
         * @param   b   another argument.
         */
        static gdouble min(gdouble a, gdouble b);

        /** Returns the size of an ulp of the argument.  An ulp, unit in
         * the last place, of a double value is the positive
         * distance between this floating-point value and the
         * double value next larger in magnitude.  Note that for non-NaN
         * <i>x</i>, <code>ulp(-<i>x</i>) == ulp(<i>x</i>)</code>.
         * <p>Special Cases:
         * <ul>
         * <li> If the argument is NaN, then the result is NaN.
         * <li> If the argument is positive or negative infinity, then the
         * result is positive infinity.
         * <li> If the argument is positive or negative zero, then the result is
         * Double.MIN_VALUE.
         * <li> If the argument is &plusmn;Double.MAX_VALUE, then
         * the result is equal to 2<sup>971</sup>.
         * </ul>
         * @param d the floating-point value whose ulp is to be returned
         *
         */
        static gdouble ulp(gdouble d);

        /** Returns the size of an ulp of the argument.  An ulp, unit in
         * the last place, of a float value is the positive
         * distance between this floating-point value and the
         * float value next larger in magnitude.  Note that for non-NaN
         * <i>x</i>, <code>ulp(-<i>x</i>) == ulp(<i>x</i>)</code>.
         * <p>Special Cases:
         * <ul>
         * <li> If the argument is NaN, then the result is NaN.
         * <li> If the argument is positive or negative infinity, then the
         * result is positive infinity.
         * <li> If the argument is positive or negative zero, then the result is
         * Float.MIN_VALUE.
         * <li> If the argument is &plusmn;Float.MAX_VALUE, then
         * the result is equal to 2<sup>104</sup>.
         * </ul>
         * @param f the floating-point value whose ulp is to be returned
         *
         */
        static gfloat ulp(gfloat f);

        /** Returns the signum function of the argument; zero if the argument
         * is zero, 1.0 if the argument is greater than zero, -1.0 if the
         * argument is less than zero.
         * <p>Special Cases:
         * <ul>
         * <li> If the argument is NaN, then the result is NaN.
         * <li> If the argument is positive zero or negative zero, then the
         *      result is the same as the argument.
         * </ul>
         * @param d the floating-point value whose signum is to be returned
         *
         */
        static gdouble signum(gdouble d);

        /** Returns the signum function of the argument; zero if the argument
         * is zero, 1.0f if the argument is greater than zero, -1.0f if the
         * argument is less than zero.
         * <p>Special Cases:
         * <ul>
         * <li> If the argument is NaN, then the result is NaN.
         * <li> If the argument is positive zero or negative zero, then the
         *      result is the same as the argument.
         * </ul>
         * @param f the floating-point value whose signum is to be returned
         *
         */
        static gfloat signum(gfloat f);

        /** Returns the hyperbolic sine of a double value.
         * The hyperbolic sine of <i>x</i> is defined to be
         * (<i>e<sup>x</sup> - e<sup>-x</sup></i>)/2
         * where <i>e</i> is Math.E Euler's number.
         * <p>Special cases:
         * <ul>
         * <li>If the argument is NaN, then the result is NaN.
         * <li>If the argument is infinite, then the result is an infinity
         * with the same sign as the argument.
         * <li>If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         * </ul>
         *
         * @param   x The number whose hyperbolic sine is to be returned.
         */
        static gdouble sinh(gdouble x);

        /** Returns the hyperbolic cosine of a double value.
         * The hyperbolic cosine of <i>x</i> is defined to be
         * (<i>e<sup>x</sup> + e<sup>-x</sup></i>)/2
         * where <i>e</i> is Math.E Euler's number.
         * <p>Special cases:
         * <ul>
         * <li>If the argument is NaN, then the result is NaN.
         * <li>If the argument is infinite, then the result is positive
         * infinity.
         * <li>If the argument is zero, then the result is 1.0.
         * </ul>
         *
         * @param   x The number whose hyperbolic cosine is to be returned.
         */
        static gdouble cosh(gdouble x);

        /** Returns the hyperbolic tangent of a double value.
         * The hyperbolic tangent of <i>x</i> is defined to be
         * (<i>e<sup>x</sup> - e<sup>-x</sup></i>)/(<i>e<sup>x</sup> + e<sup>-x</sup></i>),
         * in other words, Math.sinh
         * sinh(<i>x</i>)/Math.cosh cosh(<i>x</i>).  Note
         * that the absolute value of the exact tanh is always less than
         * 1.
         * <p>Special cases:
         * <ul>
         * <li>If the argument is NaN, then the result is NaN.
         * <li>If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         * <li>If the argument is positive infinity, then the result is
         * +1.0.
         * <li>If the argument is negative infinity, then the result is
         * -1.0.
         * </ul>
         *
         * @param   x The number whose hyperbolic tangent is to be returned.
         */
        static gdouble tanh(gdouble x);

        /**
         * Returns the argument hyperbolic sine of a value; the returned angle is in the
         * range 0.0 through <i>pi</i>.
         * <p>
         * Special cases:
         * <ul>
         * <li> If the argument is NaN, then the result is NaN.
         * <li> If the argument is a infinity, then the result is a infinity with a same sign.
         * <li> If the argument is zero, then the result is a zero with a same sign.
         * </ul>
         * @param   x
         *          The value whose hyperbolic arc sine is to be returned.
         */
        static gdouble asinh(gdouble x);

        /**
         * Returns the argument of hyperbolic cosine of a value; the returned angle is in the
         * range 0.0 through <i>pi</i>.
         * <p>
         * Special cases:
         * <ul>
         * <li> If the argument is NaN or a negative infinity, then the
         * result is NaN.
         * <li> If the argument is a positive infinity, then the result is a positive infinity.
         * <li> If the argument is 1.0, then the result is a positive zero.
         * </ul>
         * @param   x
         *          The value whose hyperbolic arc cosine is to be returned.
         */
        static gdouble acosh(gdouble x);

        /**
         * Returns the argument hyperbolic tangent of a value; the returned angle is in the
         * range 0.0 through <i>pi</i>.
         * <p>
         * Special cases:
         * <ul>
         * <li> If the argument is NaN, then the result is NaN.
         * <li> If the argument is zero, then the result is a zero with a same sign.
         * <li> If the argument is a +1.0, then the result is a positive infinity.
         * <li> If the argument is a -1.0, then the result is a negative infinity.
         * </ul>
         * @param   x
         *          The value whose hyperbolic arc tangent is to be returned.
         */
        static gdouble atanh(gdouble x);

        /** Returns sqrt(<i>x</i><sup>2</sup> +<i>y</i><sup>2</sup>)
         * without intermediate overflow or underflow.
         * <p>Special cases:
         * <ul>
         * <li> If either argument is infinite, then the result
         * is positive infinity.
         * <li> If either argument is NaN and neither argument is infinite,
         * then the result is NaN.
         * <li> If both arguments are zero, the result is positive zero.
         * </ul>
         * <p>The computed result must be within 1 ulp of the exact
         * result.  If one parameter is held constant, the results must be
         * semi-monotonic in the other parameter.
         * @param x a value
         * @param y a value
         * without intermediate overflow or underflow
         */
        static gdouble hypot(gdouble x, gdouble y);

        /**
         * Returns the first floating-point argument with the sign of the
         * second floating-point argument.
         * <p>
         * This method corresponds to the copySign operation defined in
         * IEEE 754.
         * @param magnitude  the parameter providing the magnitude of the result
         * @param sign   the parameter providing the sign of the result
         * and the sign of sign.
         */
        static gdouble copySign(gdouble magnitude, gdouble sign);

        /** Returns the first floating-point argument with the sign of the
         * second floating-point argument.
         *
         * <p>
         * This method corresponds to the copySign operation defined in
         * IEEE 754.
         * @param magnitude  the parameter providing the magnitude of the result
         * @param sign   the parameter providing the sign of the result
         * and the sign of sign.
         */
        static gfloat copySign(gfloat magnitude, gfloat sign);

        /** Returns the unbiased exponent used in the representation of a
         * float.  Special cases:
         * <ul>
         * <li>If the argument is NaN or infinite, then the result is
         * Float.MAX_EXPONENT + 1.
         * <li>If the argument is zero or subnormal, then the result is
         * Float.MIN_EXPONENT - 1.
         * </ul>
         * <p>
         * This method is analogous to the logB operation defined in IEEE
         * 754, but returns a different value on subnormal arguments.
         * @param f a float value
         */
        static gint exponent(gfloat f);

        /** Returns the unbiased exponent used in the representation of a
         * double.  Special cases:
         * <ul>
         * <li>If the argument is NaN or infinite, then the result is
         * Double.MAX_EXPONENT + 1.
         * <li>If the argument is zero or subnormal, then the result is
         * Double.MIN_EXPONENT - 1.
         * </ul>
         * <p>
         * This method is analogous to the logB operation defined in IEEE
         * 754, but returns a different value on subnormal arguments.
         * @param d a double value
         */
        static gint exponent(gdouble d);

        /** Returns the floating-point value adjacent to d in
         * the direction of positive infinity.  This method is
         * semantically equivalent to nextAfter(d,
         * Double.POSITIVE_INFINITY); however, a nextUp
         * implementation may run faster than its equivalent
         * nextAfter call.
         * <p>Special Cases:
         * <ul>
         * <li> If the argument is NaN, the result is NaN.
         * <li> If the argument is positive infinity, the result is
         * positive infinity.
         * <li> If the argument is zero, the result is
         * Double.MIN_VALUE
         * </ul>
         * <p> This method corresponds to the nextUp
         * operation defined in IEEE 754.
         * @param d starting floating-point value
         * infinity.
         */
        static gdouble nextUp(gdouble d);

        /** Returns the floating-point value adjacent to f in
         * the direction of positive infinity.  This method is
         * semantically equivalent to nextAfter(f,
         * Float.POSITIVE_INFINITY); however, a nextUp
         * implementation may run faster than its equivalent
         * nextAfter call.
         * <p>Special Cases:
         * <ul>
         * <li> If the argument is NaN, the result is NaN.
         * <li> If the argument is positive infinity, the result is
         * positive infinity.
         * <li> If the argument is zero, the result is
         * Float.MIN_VALUE
         * </ul>
         * <p> This method corresponds to the nextUp
         * operation defined in IEEE 754.
         * @param f starting floating-point value
         * infinity.
         */
        static gfloat nextUp(gfloat f);

        /** Returns the floating-point value adjacent to d in
         * the direction of negative infinity.  This method is
         * semantically equivalent to nextAfter(d,
         * Double.NEGATIVE_INFINITY); however, a
         * nextDown implementation may run faster than its
         * equivalent nextAfter call.
         * <p>Special Cases:
         * <ul>
         * <li> If the argument is NaN, the result is NaN.
         * <li> If the argument is negative infinity, the result is
         * negative infinity.
         * <li> If the argument is zero, the result is
         * -Double.MIN_VALUE
         * </ul>
         * <p> This method corresponds to the nextDown
         * operation defined in IEEE 754.
         * @param d  starting floating-point value
         * infinity.
         */
        static gdouble nextDown(gdouble d);

        /** Returns the floating-point value adjacent to f in
         * the direction of negative infinity.  This method is
         * semantically equivalent to nextAfter(f,
         * Float.NEGATIVE_INFINITY); however, a
         * nextDown implementation may run faster than its
         * equivalent nextAfter call.
         * <p>Special Cases:
         * <ul>
         * <li> If the argument is NaN, the result is NaN.
         * <li> If the argument is negative infinity, the result is
         * negative infinity.
         * <li> If the argument is zero, the result is
         * -Float.MIN_VALUE
         * </ul>
         * <p> This method corresponds to the nextDown
         * operation defined in IEEE 754.
         * @param f  starting floating-point value
         * infinity.
         */
        static gfloat nextDown(gfloat f);

        /** Returns d &times; 2<sup>scaleFactor</sup>
         * rounded as if performed by a single correctly rounded
         * floating-point multiply.  If the exponent of the result is
         * between Double.MIN_EXPONENT and Double.MAX_EXPONENT, the answer
         * is calculated exactly.  If the
         * exponent of the result would be larger than
         * Double.MAX_EXPONENT, an infinity is returned.  Note that if
         * the result is subnormal, precision may be lost; that is, when
         * scaleB(x, n) is subnormal, scaleB(scaleB(x, n),
         * -n) may not equal <i>x</i>.  When the result is non-NaN, the
         * result has the same sign as d.
         * <p>Special cases:
         * <ul>
         * <li> If the first argument is NaN, NaN is returned.
         * <li> If the first argument is infinite, then an infinity of the
         * same sign is returned.
         * <li> If the first argument is zero, then a zero of the same
         * sign is returned.
         * </ul>
         * <p> This method corresponds to the scaleB operation
         * defined in IEEE 754.
         * @param d number to be scaled by a power of two.
         * @param scaleFactor power of 2 used to scale d
         */
        static gdouble scaleB(gdouble d, gint scaleFactor);

        /** Returns f &times; 2<sup>scaleFactor</sup>
         * rounded as if performed by a single correctly rounded
         * floating-point multiply.  If the exponent of the result is
         * between Float.MIN_EXPONENT and Float.MAX_EXPONENT, the answer
         * is calculated exactly.  If the
         * exponent of the result would be larger than
         * Float.MAX_EXPONENT, an infinity is returned.  Note that if the
         * result is subnormal, precision may be lost; that is, when
         * scaleB(x, n) is subnormal, scaleB(scaleB(x, n), -n) may not equal <i>x</i>.
         * When the result is non-NaN, the result has the same sign as f.
         * <p>Special cases:
         * <ul>
         * <li> If the first argument is NaN, NaN is returned.
         * <li> If the first argument is infinite, then an infinity of the
         * same sign is returned.
         * <li> If the first argument is zero, then a zero of the same
         * sign is returned.
         * </ul>
         * <p> This method corresponds to the scaleB operation
         * defined in IEEE 754.
         * @param f number to be scaled by a power of two.
         * @param scaleFactor power of 2 used to scale f
         */
        static gfloat scaleB(gfloat f, gint scaleFactor);
    };

} // core

#endif //CORE23_MATH_H
