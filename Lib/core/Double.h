//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_DOUBLE_H
#define CORE23_DOUBLE_H

#include <core/Comparable.h>

namespace core {

    /**
     * The <b> Double</b> class wraps a value of the primitive type
     * <b> double</b> in an object. An object of type
     * <b> Double</b> contains a single field whose type is
     * <b> double</b>.
     *
     * <p>In addition, this class provides several methods for converting a
     * <b> double</b> to a <b> String</b> and a
     * <b> String</b> to a <b> double</b>, as well as other
     * constants and methods useful when dealing with a
     * <b> double</b>.
     *
     * <p>This is a <a href="">value-based</a>
     * class; programmers should treat instances that are
     * <b style="color: green"> equal</b> as interchangeable and should not
     * use instances for synchronization, or unpredictable behavior may
     * occur. For example, in a future release, synchronization may fail.
     *
     * <h2><a id=equivalenceRelation>Floating-point Equality, Equivalence,
     * and Comparison</a></h2>
     *
     * IEEE 754 floating-point values include finite nonzero values,
     * signed zeros (<b> +0.0</b> and <b> -0.0</b>), signed infinities
     * (<b style="color: green"> positive infinity</b> and
     * <b style="color: green"> negative infinity</b>), and
     * <b style="color: green"> NaN</b> (not-a-number).
     *
     * <p>An <em>equivalence relation</em> on a set of values is a boolean
     * relation on pairs of values that is reflexive, symmetric, and
     * transitive. For more discussion of equivalence relations and object
     * equality, see the <b style="color: orange"> Object.equals</b>
     * specification. An equivalence relation partitions the values it
     * operates over into sets called <i>equivalence classes</i>.  All the
     * members of the equivalence class are equal to each other under the
     * relation. An equivalence class may contain only a single member. At
     * least for some purposes, all the members of an equivalence class
     * are substitutable for each other.  In particular, in a numeric
     * expression equivalent values can be <em>substituted</em> for one
     * another without changing the result of the expression, meaning
     * changing the equivalence class of the result of the expression.
     *
     * <p>Notably, the built-in <b> ==</b> operation on floating-point
     * values is <em>not</em> an equivalence relation. Despite not
     * defining an equivalence relation, the semantics of the IEEE 754
     * <b> ==</b> operator were deliberately designed to meet other needs
     * of numerical computation. There are two exceptions where the
     * properties of an equivalence relation are not satisfied by <b>
     * ==</b> on floating-point values:
     *
     * <ul>
     *
     * <li>If <b> v1</b> and <b> v2</b> are both NaN, then <b> v1
     * == v2</b> has the value <b> false</b>. Therefore, for two NaN
     * arguments the <em>reflexive</em> property of an equivalence
     * relation is <em>not</em> satisfied by the <b> ==</b> operator.
     *
     * <li>If <b> v1</b> represents <b> +0.0</b> while <b> v2</b>
     * represents <b> -0.0</b>, or vice versa, then <b> v1 == v2</b> has
     * the value <b> true</b> even though <b> +0.0</b> and <b> -0.0</b>
     * are distinguishable under various floating-point operations. For
     * example, <b> 1.0/+0.0</b> evaluates to positive infinity while
     * <b> 1.0/-0.0</b> evaluates to <em>negative</em> infinity and
     * positive infinity and negative infinity are neither equal to each
     * other nor equivalent to each other. Thus, while a signed zero input
     * most commonly determines the sign of a zero result, because of
     * dividing by zero, <b> +0.0</b> and <b> -0.0</b> may not be
     * substituted for each other in general. The sign of a zero input
     * also has a non-substitutable effect on the result of some math
     * library methods.
     *
     * </ul>
     *
     * <p>For ordered comparisons using the built-in comparison operators
     * (<b> &lt;</b>, <b> &le;</b>, etc.), NaN values have another anomalous
     * situation: a NaN is neither less than, nor greater than, nor equal
     * to any value, including itself. This means the <i>dichotomy of
     * comparison</i> does <em>not</em> hold.
     *
     * <p>To provide the appropriate semantics for <b> equals</b> and
     * <b> compareTo</b> methods, those methods cannot simply be wrappers
     * around <b> ==</b> or ordered comparison operations. Instead, <b style="color: orange">
     * Double.equals</b> uses <a href=#repEquivalence> representation
     * equivalence</a>, defining NaN arguments to be equal to each other,
     * restoring reflexivity, and defining <b> +0.0</b> to <em>not</em> be
     * equal to <b> -0.0</b>. For comparisons, <b style="color: orange"> Double.compareTo
     * </b> defines a total order where <b> -0.0</b> is less than
     * <b> +0.0</b> and where a NaN is equal to itself and considered
     * greater than positive infinity.
     *
     * <p>The operational semantics of <b> equals</b> and <b>
     * compareTo</b> are expressed in terms of <b style="color: green">
     * bit-wise converting</b> the floating-point values to integral values.
     *
     * <p>The <em>natural ordering</em> implemented by <b style="color: orange">
     * compareTo</b> is <b style="color: green"> Comparable consistent with equals</b>. That
     * is, two objects are reported as equal by <b> equals</b> if and only
     * if <b> compareTo</b> on those objects returns zero.
     *
     * <p>The adjusted behaviors defined for <b> equals</b> and <b>
     * compareTo</b> allow instances of wrapper classes to work properly with
     * conventional data structures. For example, defining NaN
     * values to be <b> equals</b> to one another allows NaN to be used as
     * an element of a <b style="color: orange"> HashSet</b> or as the key of
     * a <b style="color: orange"> HashMap</b>. Similarly, defining <b>
     * compareTo</b> as a total ordering, including <b> +0.0</b>, <b>
     * -0.0</b>, and NaN, allows instances of wrapper classes to be used as
     * elements of a <b style="color: orange"> SortedSet</b> or as keys of a
     * <b style="color: orange"> SortedMap</b>.
     *
     * <p>Comparing numerical equality to various useful equivalence
     * relations that can be defined over floating-point values:
     *
     * <dl>
     * <dt><a id=fpNumericalEq><i>numerical equality</i></a> (<b> ==</b>
     * operator): (<em>Not</em> an equivalence relation)</dt>
     * <dd>Two floating-point values represent the same extended real
     * number. The extended real numbers are the real numbers augmented
     * with positive infinity and negative infinity. Under numerical
     * equality, <b> +0.0</b> and <b> -0.0</b> are equal since they both
     * map to the same real value, 0. A NaN does not map to any real
     * number and is not equal to any value, including itself.
     * </dd>
     *
     * <dt><i>bit-wise equivalence</i>:</dt>
     * <dd>The bits of the two floating-point values are the same. This
     * equivalence relation for <b> double</b> values <b> a</b> and <b>
     * b</b> is implemented by the expression
     * <br><b> Double.toLongBits(a) == Double.toLongBits(b)</b><br>
     * Under this relation, <b> +0.0</b> and <b> -0.0</b> are
     * distinguished from each other and every bit pattern encoding a NaN
     * is distinguished from every other bit pattern encoding a NaN.
     * </dd>
     *
     * <dt><i><a id=repEquivalence>representation equivalence</a></i>:</dt>
     * <dd>The two floating-point values represent the same IEEE 754
     * <i>datum</i>. In particular, for <b style="color: green">
     * finite</b> values, the sign, <b style="color: green">
     * exponent</b>, and significand components of the floating-point values
     * are the same. Under this relation:
     * <ul>
     * <li> <b> +0.0</b> and <b> -0.0</b> are distinguished from each other.
     * <li> every bit pattern encoding a NaN is considered equivalent to each other
     * <li> positive infinity is equivalent to positive infinity; negative
     *      infinity is equivalent to negative infinity.
     * </ul>
     * Expressions implementing this equivalence relation include:
     * <ul>
     * <li><b> Double.toLongBits(a) == Double.toLongBits(b)</b>
     * <li><b> Double.valueOf(a).equals(Double.valueOf(b))</b>
     * <li><b> Double.compare(a, b) == 0</b>
     * </ul>
     * Note that representation equivalence is often an appropriate notion
     * of equivalence to test the behavior of <b style="color: green"> StrictMath math
     * libraries</b>.
     * </dd>
     * </dl>
     *
     * For two binary floating-point values <b> a</b> and <b> b</b>, if
     * neither of <b> a</b> and <b> b</b> is zero or NaN, then the three
     * relations numerical equality, bit-wise equivalence, and
     * representation equivalence of <b> a</b> and <b> b</b> have the same
     * <b> true</b>/<b> false</b> value. In other words, for binary
     * floating-point values, the three relations only differ if at least
     * one argument is zero or NaN.
     *
     * @nls 4.2.3 Floating-Point Types, Formats, and Values
     * @nls 4.2.4. Floating-Point Operations
     * @nls 15.21.1 Numerical Equality Operators &eq; and &ne;
     * @nls 15.20.1 Numerical Comparison Operators <b> &lt;</b>, <b> &le;</b>, <b> &gt;</b>, and <b> &ge;</b>
     *
     * @see <a href="https://standards.ieee.org/ieee/754/6210/">
     *      <cite>IEEE Standard for Floating-Point Arithmetic</cite></a>
     *
     * @author  Brunshweeck Tazeussong
     */
    class Double:public Comparable<Double> {
    private:
        /**
         * The value of Double
         */
        gdouble value;

    public:

        /**
         * A constant holding the largest positive finite value of type
         * <b>double</b>, (2-2<sup> -23 </sup>)&middot;2<sup>127</sup>.
         * It is equal to the hexadecimal floating-point literal
         * <b>0x1.fffffffffffffP+1023</b> and also equal to
         * <b> Double.fromLongBits(0x7fefffffffffffffLL)</b>.
         */
        static CORE_FAST gdouble MAX_VALUE = 0x1.fffffffffffffP+1023; // 1.7976931348623157e+308

        /**
         * A constant holding the smallest positive normal value of type
         * <b>double</b>, 2<sup> -126</sup>.  It is equal to the
         * hexadecimal floating-point literal <b>0x1.0p-1022</b> and also
         * equal to <b>Double.fromLongBits(0x0010000000000000LL)</b>.
         */
        static CORE_FAST gdouble MIN_NORMAL = 0x1.0p-1022; // 2.2250738585072014E-308

        /**
         * A constant holding the smallest positive nonzero value of type
         * <b>double</b>, 2<sup> -149</sup>. It is equal to the
         * hexadecimal floating-point literal <b>0x0.0000000000001P-1022</b>
         * and also equal to <b>Double.fromLongBits(0x1LL)</b>.
         */
        static CORE_FAST gdouble MIN_VALUE = 0x0.0000000000001P-1022; // 4.9e-324

        /**
         * A constant holding the positive infinity of type
         * <b>double</b>. It is equal to the value returned by
         * <b>Double.fromLongBits(0x7ff0000000000000LL)</b>.
         */
        static const gdouble POSITIVE_INFINITY; // infinity

        /**
         * A constant holding the negative infinity of type
         * <b>double</b>. It is equal to the value returned by
         * <b>Double.fromLongBits(0xfff0000000000000LL)</b>.
         */
        static const gdouble NEGATIVE_INFINITY; // -infinity

        /**
         * A constant holding a Not-a-Number (NaN) value of type
         * <b>double</b>.  It is equivalent to the value returned by
         * <b>Double.fromLongBits(0x7ff8000000000000LL)</b>.
         */
        static const gdouble NaN;

        /**
         * The number of bits used to represent a <b>double</b> value.
         */
        static CORE_FAST gint SIZE = 64;

        /**
         * The number of bits in the significand of a <b>double</b> value.
         */
        static CORE_FAST gint PRECISION = 53;

        /**
         * Maximum exponent a finite <b>double</b> variable may have.
         */
        static CORE_FAST gint MAX_EXPONENT = (1 << (SIZE - PRECISION - 1)) - 1; // 1023

        /**
         * Minimum exponent a normalized <b>double</b> variable may have.
         */
        static CORE_FAST gint MIN_EXPONENT = 1 - MAX_EXPONENT; // -1022

        /**
         * The number of logical bits in the significand of a
         * double number, including the implicit bit.
         */
        static CORE_FAST gint SIGNIFICAND_WIDTH = PRECISION;

        /**
         * The exponent the smallest positive double subnormal value would
         * have if it could be normalized.
         */
        static CORE_FAST gint MIN_SUB_EXPONENT = MIN_EXPONENT - (SIGNIFICAND_WIDTH - 1); // -1074

        /**
         * Bias used in representing a double exponent.
         */
        static CORE_FAST gint EXPONENT_BIAS = (1LL << (SIZE - SIGNIFICAND_WIDTH - 1)) - 1; // 1023

        /**
         * Bit mask to isolate the sign bit of a double.
         */
        static CORE_FAST glong SIGN_BIT_MASK = 1LL << (SIZE - 1);

        /**
         * Bit mask to isolate the exponent field of a double.
         */
        static CORE_FAST glong EXPONENT_BIT_MASK = ((1LL << (SIZE - SIGNIFICAND_WIDTH)) - 1) << (SIGNIFICAND_WIDTH - 1);

        /**
         * Bit mask to isolate the significand field of a double.
         */
        static CORE_FAST glong SIGNIFICAND_BIT_MASK = (1LL << (SIGNIFICAND_WIDTH - 1)) - 1;

        /**
         * Bit mask to isolate the magnitude bits (combined exponent and
         * significand fields) of a double.
         */
        static CORE_FAST glong MAGNITUDE_BIT_MASK = EXPONENT_BIT_MASK | SIGNIFICAND_BIT_MASK;

        /**
         * Constructs a newly allocated <b>Double</b> object that
         * represents the primitive <b>double</b> argument.
         *
         * @param value
         *          The value to be represented by the <b>Double</b>.
         */
        CORE_FAST Double(gdouble value) : value(value) {}

        /**
         * Returns <b>true</b> if this <b>Double</b> value is
         * a Not-a-Number (NaN), <b>false</b> otherwise.
         */
        CORE_FAST gbool isNaN() const {
            return isNaN(value);
        }

        /**
         * Returns <b>true</b> if this <b>Double</b> value is
         * infinitely large in magnitude, <b>false</b> otherwise.
         */
        CORE_FAST gbool isInfinite() const {
            return isInfinite(value);
        }

        /**
         * Returns a new <b>double</b> initialized to the value
         * represented by the specified <b>String</b>, as performed
         * by the <b>valueOf</b> method of class
         * <b>Double</b>.
         *
         * @param str   the string to be parsed.
         *
         * @throws FormatException if the string does not contain
         *         a parsable <b>double</b>.
         */
        static gdouble parseDouble(const String &str);

        /**
         * Returns a <b>Double</b> object holding the
         * <b>double</b> value represented by the argument string
         * <b>str</b>.
         *
         * <p>If <b>str</b> is <b>null</b>, then a
         * <b>NullPointerException</b> is thrown.
         *
         * <p>Leading and trailing whitespace characters in <b>str</b>
         * are ignored.  Whitespace is removed as if by the <b>
         * String.strip</b> method; that is, both ASCII space and control
         * characters are removed. The rest of <b>str</b> should
         * constitute a <i>FloatValue</i> as described by the lexical
         * syntax rules:
         *
         * <blockquote>
         * <dl>
         * <dt><i>FloatValue:</i>
         * <dd><i>Sign<sub>opt</sub></i> <b>NaN</b>
         * <dd><i>Sign<sub>opt</sub></i> <b>Infinity</b>
         * <dd><i>Sign<sub>opt</sub> FloatingPointLiteral</i>
         * <dd><i>Sign<sub>opt</sub> HexFloatingPointLiteral</i>
         * <dd><i>SignedInteger</i>
         * </dl>
         *
         * <dl>
         * <dt><i>HexFloatingPointLiteral</i>:
         * <dd> <i>HexSignificand BinaryExponent FloatTypeSuffix<sub>opt</sub></i>
         * </dl>
         *
         * <dl>
         * <dt><i>HexSignificand:</i>
         * <dd><i>HexNumeral</i>
         * <dd><i>HexNumeral</i> <b>.</b>
         * <dd><b>0x</b> <i>HexDigits<sub>opt</sub>
         *     </i><b>.</b><i> HexDigits</i>
         * <dd><b>0X</b><i> HexDigits<sub>opt</sub>
         *     </i><b>.</b> <i>HexDigits</i>
         * </dl>
         *
         * <dl>
         * <dt><i>BinaryExponent:</i>
         * <dd><i>BinaryExponentIndicator SignedInteger</i>
         * </dl>
         *
         * <dl>
         * <dt><i>BinaryExponentIndicator:</i>
         * <dd><b>p</b>
         * <dd><b>P</b>
         * </dl>
         *
         * </blockquote>
         *
         * where <i>Sign</i>, <i>FloatingPointLiteral</i>,
         * <i>HexNumeral</i>, <i>HexDigits</i> and <i>SignedInteger</i>,
         * except that underscores are not accepted between digits.
         * If <b>str</b> does not have the form of
         * a <i>FloatValue</i>, then a <b>NumberFormatException</b>
         * is thrown. Otherwise, <b>str</b> is regarded as
         * representing an exact decimal value in the usual
         * "computerized scientific notation" or as an exact
         * hexadecimal value; this exact numerical value is then
         * conceptually converted to an "infinitely precise"
         * binary value that is then rounded to type <b>double</b>
         * by the usual round-to-nearest rule of IEEE 754 floating-point
         * arithmetic, which includes preserving the sign of a zero
         * value.
         *
         * Note that the round-to-nearest rule also implies overflow and
         * underflow behaviour; if the exact value of <b>str</b> is large
         * enough in magnitude, rounding to <b>double</b> will result in an
         * infinity and if the exact value of <b>str</b> is small enough in magnitude (less
         * than or equal to <b>MIN_VALUE</b>/2), rounding to float will
         * result in a zero.
         *
         * Finally, after rounding a <b>Double</b> object representing
         * this <b>double</b> value is returned.
         *
         * <p> To interpret localized string representations of a
         * floating-point value, use subclasses of <b>core::NumberFormat</b>.
         *
         * <p>Note that trailing format specifiers, specifiers that
         * determine the type of a floating-point literal
         * (<b>1.0f</b> is a <b>float</b> value;
         * <b>1.0d</b> is a <b>double</b> value), do
         * <em>not</em> influence the results of this method.  In other
         * words, the numerical value of the input string is converted
         * directly to the target floating-point type.  The two-step
         * sequence of conversions, string to <b>float</b> followed
         * by <b>float</b> to <b>double</b>, is <em>not</em>
         * equivalent to converting a string directly to
         * <b>double</b>. For example, the <b>float</b>
         * literal <b>0.1f</b> is equal to the <b>double</b>
         * value <b>0.10000000149011612</b>; the <b>float</b>
         * literal <b>0.1f</b> represents a different numerical
         * value than the <b>double</b> literal
         * <b>0.1</b>. (The numerical value 0.1 cannot be exactly
         * represented in a binary floating-point number.)
         *
         * <p>To avoid calling this method on an invalid string and having
         * a <b>NumberFormatException</b> be thrown, the regular
         * expression below can be used to screen the input string:
         *
         * @code
         *  String Digits     = "(\\p{Digit}+)";
         *  String HexDigits  = "(\\p{XDigit}+)";
         *  // an exponent is 'e' or 'E' followed by an optionally
         *  // signed decimal integer.
         *  String Exp        = "[eE][+-]?"+Digits;
         *  String fpRegex    =
         *      ("[\\x00-\\x20]*"+  // Optional leading "whitespace"
         *       "[+-]?(" + // Optional sign character
         *       "NaN|" +           // "NaN" string
         *       "Infinity|" +      // "Infinity" string
         *
         *       // A decimal floating-point string representing a finite positive
         *       // number without a leading sign has at most five basic pieces:
         *       // Digits . Digits ExponentPart FloatTypeSuffix
         *       //
         *       // Since this method allows integer-only strings as input
         *       // in addition to strings of floating-point literals, the
         *       // two sub-patterns below are simplifications of the grammar
         *       // productions.
         *
         *       // Digits ._opt Digits_opt ExponentPart_opt FloatTypeSuffix_opt
         *       "((("+Digits+"(\\.)?("+Digits+"?)("+Exp+")?)|"+
         *
         *       // . Digits ExponentPart_opt FloatTypeSuffix_opt
         *       "(\\.("+Digits+")("+Exp+")?)|"+
         *
         *       // Hexadecimal strings
         *       "((" +
         *        // 0[xX] HexDigits ._opt BinaryExponent FloatTypeSuffix_opt
         *        "(0[xX]" + HexDigits + "(\\.)?)|" +
         *
         *        // 0[xX] HexDigits_opt . HexDigits BinaryExponent FloatTypeSuffix_opt
         *        "(0[xX]" + HexDigits + "?(\\.)" + HexDigits + ")" +
         *
         *        ")[pP][+-]?" + Digits + "))" +
         *       "[fFdD]?))" +
         *       "[\\x00-\\x20]*");// Optional trailing "whitespace"
         *
         *  if (Pattern::matches(fpRegex, myString))
         *      Double.valueOf(myString); // Will not throw NumberFormatException
         *  else {
         *      // Perform suitable alternative action
         *   }
         * @endcode
         *
         * @param str   the string to be parsed.
         *
         * @throws FormatException  if the string does not contain a
         *             parsable number.
         */
        static Double valueOf(const String &str);

        /**
         * Returns a <b>Double</b> instance representing the specified
         * <b>double</b> value.
         *
         * @param d
         *        A double value.
         */
        static Double valueOf(gdouble d);

        /**
         * Returns <b>true</b> if the specified number is a
         * Not-a-Number (NaN) value, <b>false</b> otherwise.
         *
         * <p>
         * This method corresponds to the isNaN operation defined in IEEE
         * 754.
         *
         * @param d   the value to be tested.
         */
        static CORE_FAST gbool isNaN(gdouble d) {
            return (d != d);
        }

        /**
         * Returns <b>true</b> if the specified number is infinitely
         * large in magnitude, <b>false</b> otherwise.
         *
         * <p>
         * This method corresponds to the isInfinite operation defined in
         * IEEE 754.
         *
         * @param d   the value to be tested.
         */
        static CORE_FAST gbool isInfinite(gdouble d) {
            return (d < MIN_VALUE) || (d > MAX_VALUE);
        }

        /**
         * Returns <b>true</b> if the argument is a finite floating-point
         * value; returns <b>false</b> otherwise (for NaN and infinity
         * arguments).
         *
         * <p>
         * This method corresponds to the isFinite operation defined in
         * IEEE 754.
         *
         * @param d the <b>double</b> value to be tested
         */
        static CORE_FAST gbool isFinite(gdouble d) {
            return (d >= MIN_VALUE) && (d <= MAX_VALUE);
        }

        /**
         * Return the value of this Double as byte
         * after narrowing primitive conversion.
         */
        gbyte byteValue() const {
            return (gbyte) value;
        }

        /**
         * Return the value of this Double as short
         * after narrowing primitive conversion.
         */
        CORE_FAST gshort shortValue() const {
            return (gshort) value;
        }

        /**
         * Return the value of this Double as int
         * after narrowing primitive conversion.
         */
        CORE_FAST gint intValue() const {
            return (gint) value;
        }

        /**
         * Return the value of this Double as long
         * after narrowing primitive conversion.
         */
        CORE_FAST glong longValue() const {
            return (glong) value;
        }

        /**
         * Return the value of this Double as float.
         */
        CORE_FAST gfloat floatValue() const {
            return (gfloat) value;
        }

        /**
         * Return the value of this Double as double
         * after widening primitive conversion.
         */
        CORE_FAST gdouble doubleValue() const {
            return (gdouble) value;
        }

        /**
         * Returns a string representation of this <b>Double</b> object.
         * The primitive <b>double</b> value represented by this
         * object is converted to a string exactly as if by the method
         * <b>toString</b> of one argument.
         *
         * @see Double.toString(double)
         */
        String toString() const override;

        /**
         * Returns a string representation of the <b>double</b>
         * argument. All characters mentioned below are ASCII characters.
         * <ul>
         * <li>If the argument is NaN, the result is the string
         *     "<b>NaN</b>".
         * <li>Otherwise, the result is a string that represents the sign and
         * magnitude (absolute value) of the argument. If the sign is negative,
         * the first character of the result is '<b>-</b>'
         * (<b>'\u005Cu002D'</b>); if the sign is positive, no sign character
         * appears in the result. As for the magnitude <i>m</i>:
         * <ul>
         * <li>If <i>m</i> is infinity, it is represented by the characters
         * <b>"Infinity"</b>; thus, positive infinity produces the result
         * <b>"Infinity"</b> and negative infinity produces the result
         * <b>"-Infinity"</b>.
         *
         * <li>If <i>m</i> is zero, it is represented by the characters
         * <b>"0.0"</b>; thus, negative zero produces the result
         * <b>"-0.0"</b> and positive zero produces the result
         * <b>"0.0"</b>.
         *
         * <li> Otherwise <i>m</i> is positive and finite.
         * It is converted to a string in two stages:
         * <ul>
         * <li> <em>Selection of a decimal</em>:
         * A well-defined decimal <i>d</i><sub><i>m</i></sub>
         * is selected to represent <i>m</i>.
         * This decimal is (almost always) the <em>shortest</em> one that
         * rounds to <i>m</i> according to the round to nearest
         * rounding policy of IEEE 754 floating-point arithmetic.
         * <li> <em>Formatting as a string</em>:
         * The decimal <i>d</i><sub><i>m</i></sub> is formatted as a string,
         * either in plain or in computerized scientific notation,
         * depending on its value.
         * </ul>
         * </ul>
         * </ul>
         *
         * <p>A <em>decimal</em> is a number of the form
         * <i>str</i>&times;10<sup><i>i</i></sup>
         * for some (unique) integers <i>str</i> &gt; 0 and <i>i</i> such that
         * <i>str</i> is not a multiple of 10.
         * These integers are the <em>significand</em> and
         * the <em>exponent</em>, respectively, of the decimal.
         * The <em>length</em> of the decimal is the (unique)
         * positive integer <i>n</i> meeting
         * 10<sup><i>n</i>-1</sup> &le; <i>str</i> &lt; 10<sup><i>n</i></sup>.
         *
         * <p>The decimal <i>d</i><sub><i>m</i></sub> for a finite positive <i>m</i>
         * is defined as follows:
         * <ul>
         * <li>Let <i>T</i> be the set of all decimals that round to <i>m</i>
         * according to the usual <em>round to nearest</em> rounding policy of
         * IEEE 754 floating-point arithmetic.
         * <li>Let <i>p</i> be the minimal length over all decimals in <i>T</i>.
         * <li>When <i>p</i> &ge; 2, let <i>T</i> be the set of all decimals
         * in <i>T</i> with length <i>p</i>.
         * Otherwise, let <i>T</i> be the set of all decimals
         * in <i>T</i> with length 1 or 2.
         * <li>Define <i>d</i><sub><i>m</i></sub> as the decimal in <i>T</i>
         * that is closest to <i>m</i>.
         * Or if there are two such decimals in <i>T</i>,
         * select the one with the even significand.
         * </ul>
         *
         * <p>The (uniquely) selected decimal <i>d</i><sub><i>m</i></sub>
         * is then formatted.
         * Let <i>str</i>, <i>i</i> and <i>n</i> be the significand, exponent and
         * length of <i>d</i><sub><i>m</i></sub>, respectively.
         * Further, let <i>e</i> = <i>n</i> + <i>i</i> - 1 and let
         * <i>str</i><sub>1</sub>&hellip;<i>str</i><sub><i>n</i></sub>
         * be the usual decimal expansion of <i>str</i>.
         * Note that <i>str</i><sub>1</sub> &ne; 0
         * and <i>str</i><sub><i>n</i></sub> &ne; 0.
         * Below, the decimal point <b>'.'</b> is <b>'\u005Cu002E'</b>
         * and the exponent indicator <b>'E'</b> is <b>'\u005Cu0045'</b>.
         * <ul>
         * <li>Case -3 &le; <i>e</i> &lt; 0:
         * <i>d</i><sub><i>m</i></sub> is formatted as
         * <code>0.0</code>&hellip;<code>0</code><!--
         * --><i>str</i><sub>1</sub>&hellip;<i>str</i><sub><i>n</i></sub>,
         * where there are exactly -(<i>n</i> + <i>i</i>) zeroes between
         * the decimal point and <i>str</i><sub>1</sub>.
         * For example, 123 &times; 10<sup>-4</sup> is formatted as
         * <b>0.0123</b>.
         * <li>Case 0 &le; <i>e</i> &lt; 7:
         * <ul>
         * <li>Subcase <i>i</i> &ge; 0:
         * <i>d</i><sub><i>m</i></sub> is formatted as
         * <i>str</i><sub>1</sub>&hellip;<i>str</i><sub><i>n</i></sub><!--
         * --><code>0</code>&hellip;<code>0.0</code>,
         * where there are exactly <i>i</i> zeroes
         * between <i>str</i><sub><i>n</i></sub> and the decimal point.
         * For example, 123 &times; 10<sup>2</sup> is formatted as
         * <b>12300.0</b>.
         * <li>Subcase <i>i</i> &lt; 0:
         * <i>d</i><sub><i>m</i></sub> is formatted as
         * <i>str</i><sub>1</sub>&hellip;<!--
         * --><i>str</i><sub><i>n</i>+<i>i</i></sub><code>.</code><!--
         * --><i>str</i><sub><i>n</i>+<i>i</i>+1</sub>&hellip;<!--
         * --><i>str</i><sub><i>n</i></sub>,
         * where there are exactly -<i>i</i> digits to the right of
         * the decimal point.
         * For example, 123 &times; 10<sup>-1</sup> is formatted as
         * <b>12.3</b>.
         * </ul>
         * <li>Case <i>e</i> &lt; -3 or <i>e</i> &ge; 7:
         * computerized scientific notation is used to format
         * <i>d</i><sub><i>m</i></sub>.
         * Here <i>e</i> is formatted as by <b>Integer.toString(int)</b>.
         * <ul>
         * <li>Subcase <i>n</i> = 1:
         * <i>d</i><sub><i>m</i></sub> is formatted as
         * <i>str</i><sub>1</sub><code>.0E</code><i>e</i>.
         * For example, 1 &times; 10<sup>23</sup> is formatted as
         * <b>1.0E23</b>.
         * <li>Subcase <i>n</i> &gt; 1:
         * <i>d</i><sub><i>m</i></sub> is formatted as
         * <i>str</i><sub>1</sub><code>.</code><i>str</i><sub>2</sub><!--
         * -->&hellip;<i>str</i><sub><i>n</i></sub><code>E</code><i>e</i>.
         * For example, 123 &times; 10<sup>-21</sup> is formatted as
         * <b>1.23E-19</b>.
         * </ul>
         * </ul>
         *
         * <p>To create localized string representations of a floating-point
         * value, use subclasses of <b>core::NumberFormat</b>.
         *
         * @param d   the <b>double</b> to be converted.
         */
        static String toString(gdouble d);

        /**
         * Returns a hexadecimal string representation of the
         * <b>double</b> argument. All characters mentioned below
         * are ASCII characters.
         *
         * <ul>
         * <li>If the argument is NaN, the result is the string
         *     "<b>NaN</b>".
         * <li>Otherwise, the result is a string that represents the sign
         * and magnitude of the argument. If the sign is negative, the
         * first character of the result is '<b>-</b>'
         * (<b>'\u005Cu002D'</b>); if the sign is positive, no sign
         * character appears in the result. As for the magnitude <i>m</i>:
         *
         * <ul>
         * <li>If <i>m</i> is infinity, it is represented by the string
         * <b>"Infinity"</b>; thus, positive infinity produces the
         * result <b>"Infinity"</b> and negative infinity produces
         * the result <b>"-Infinity"</b>.
         *
         * <li>If <i>m</i> is zero, it is represented by the string
         * <b>"0x0.0p0"</b>; thus, negative zero produces the result
         * <b>"-0x0.0p0"</b> and positive zero produces the result
         * <b>"0x0.0p0"</b>.
         *
         * <li>If <i>m</i> is a <b>double</b> value with a
         * normalized representation, substrings are used to represent the
         * significand and exponent fields.  The significand is
         * represented by the characters <b>"0x1."</b>
         * followed by a lowercase hexadecimal representation of the rest
         * of the significand as a fraction.  Trailing zeros in the
         * hexadecimal representation are removed unless all the digits
         * are zero, in which case a single zero is used. Next, the
         * exponent is represented by <b>"p"</b> followed
         * by a decimal string of the unbiased exponent as if produced by
         * a call to <b>Integer.toString</b> on the
         * exponent value.
         *
         * <li>If <i>m</i> is a <b>double</b> value with a subnormal
         * representation, the significand is represented by the
         * characters <b>"0x0."</b> followed by a
         * hexadecimal representation of the rest of the significand as a
         * fraction.  Trailing zeros in the hexadecimal representation are
         * removed. Next, the exponent is represented by
         * <b>"p-1022"</b>.  Note that there must be at
         * least one nonzero digit in a subnormal significand.
         *
         * </ul>
         *
         * </ul>
         *
         * <table class="striped">
         * <caption>Examples</caption>
         * <thead>
         * <tr><th scope="col">Floating-point Value</th><th scope="col">Hexadecimal String</th>
         * </thead>
         * <tbody style="text-align:right">
         * <tr><th scope="row"><b>1.0</b></th> <td><b>0x1.0p0</b></td>
         * <tr><th scope="row"><b>-1.0</b></th>        <td><b>-0x1.0p0</b></td>
         * <tr><th scope="row"><b>2.0</b></th> <td><b>0x1.0p1</b></td>
         * <tr><th scope="row"><b>3.0</b></th> <td><b>0x1.8p1</b></td>
         * <tr><th scope="row"><b>0.5</b></th> <td><b>0x1.0p-1</b></td>
         * <tr><th scope="row"><b>0.25</b></th>        <td><b>0x1.0p-2</b></td>
         * <tr><th scope="row"><b>Double.MAX_VALUE</b></th>
         *     <td><b>0x1.fffffffffffffp1023</b></td>
         * <tr><th scope="row"><b>Minimum Normal Value</b></th>
         *     <td><b>0x1.0p-1022</b></td>
         * <tr><th scope="row"><b>Maximum Subnormal Value</b></th>
         *     <td><b>0x0.fffffffffffffp-1022</b></td>
         * <tr><th scope="row"><b>Double.MIN_VALUE</b></th>
         *     <td><b>0x0.0000000000001p-1022</b></td>
         * </tbody>
         * </table>
         * @param d   the <b>double</b> to be converted.
         */
        static String toHexString(gdouble d);

        /**
         * Returns a hash code for this <b>Double</b> object. The
         * result is the exclusive OR of the two halves of the
         * <b>long</b> integer bit representation, exactly as
         * produced by the method <b>doubleToLongBits(double)</b>, of
         * the primitive <b>double</b> value represented by this
         * <b>Double</b> object. That is, the hash code is the value
         * of the expression:
         *
         * <blockquote>
         *  <b>(int)(v^(v>>>32))</b>
         * </blockquote>
         *
         * where <b>v</b> is defined by:
         *
         * <blockquote>
         *  <b>long v = Double.doubleToLongBits(this.doubleValue());</b>
         * </blockquote>
         */
        gint hash() const override {
            return hash(value);
        }

        /**
         * Returns a hash code for a <b>double</b> value; compatible with
         * <b>Double.hash()</b>.
         *
         * @param d the value to hash
         */
        static gint hash(gdouble d);

        /**
         * Compares this object against the specified object.  The result
         * is <b>true</b> if and only if the argument is not
         * <b>null</b> and is a <b>Double</b> object that
         * represents a <b>double</b> that has the same value as the
         * <b>double</b> represented by this object. For this
         * purpose, two <b>double</b> values are considered to be
         * the same if and only if the method <b>doubleToLongBits(double)</b>
         * returns the identical <b>long</b> value when applied to each.
         *
         * <p>
         * This method is defined in terms of <b>
         * doubleToLongBits(double)</b> rather than the <b>==</b> operator
         * on <b>double</b> values since the <b>==</b> operator does
         * <em>not</em> define an equivalence relation and to satisfy the
         * equals contract an equivalence relation must be implemented.
         *
         * @see Double.doubleToLongBits(double)
         */
        gbool equals(const Object &object) const override;

        /**
         * Compares two <b>Double</b> objects numerically.
         *
         * This method imposes a total order on <b>Double</b> objects
         * with two differences compared to the incomplete order defined by
         * the language numerical comparison operators (<b>&lt;, &le;,
         * ==, >=, ></b>) on <b>double</b> values.
         *
         * <ul><li> A NaN is <em>unordered</em> with respect to other
         *          values and unequal to itself under the comparison
         *          operators.  This method chooses to define <b>
         *          Double.NaN</b> to be equal to itself and greater than all
         *          other <b>double</b> values (including <b>
         *          Double.POSITIVE_INFINITY</b>).
         *
         *      <li> Positive zero and negative zero compare equal
         *      numerically, but are distinct and distinguishable values.
         *      This method chooses to define positive zero (<b>+0.0d</b>),
         *      to be greater than negative zero (<b>-0.0d</b>).
         * </ul>

         * This ensures that the <i>natural ordering</i> of <b>Double</b>
         * objects imposed by this method is <i>consistent with
         * equals</i>;
         *
         * @param other   the <b>Double</b> to be compared.
         */
        gint compareTo(const Double &other) const override;

        /**
         * Compares the two specified <b>double</b> values. The sign
         * of the integer value returned is the same as that of the
         * integer that would be returned by the call:
         * <pre>
         *    Double.valueOf(d1).compareTo(Double.valueOf(d2))
         * </pre>
         *
         * @param x he first <b>double</b> to compare
         * @param y the second <b>double</b> to compare
         */
        static CORE_FAST gint compare(gdouble x, gdouble y) {
            return (x < y) ? -1 : ((x > y) ? 1 : 0);
        }

        /**
         * Returns a representation of the specified floating-point value
         * according to the IEEE 754 floating-point "double
         * format" bit layout.
         *
         * <p>Bit 63 (the bit that is selected by the mask
         * <b>0x8000000000000000L</b>) represents the sign of the
         * floating-point number. Bits
         * 62-52 (the bits that are selected by the mask
         * <b>0x7ff0000000000000L</b>) represent the exponent. Bits 51-0
         * (the bits that are selected by the mask
         * <b>0x000fffffffffffffL</b>) represent the significand
         * (sometimes called the mantissa) of the floating-point number.
         *
         * <p>If the argument is positive infinity, the result is
         * <b>0x7ff0000000000000L</b>.
         *
         * <p>If the argument is negative infinity, the result is
         * <b>0xfff0000000000000L</b>.
         *
         * <p>If the argument is NaN, the result is in the range
         * <b>0x7ff0000000000001L</b> through <b>0x7fffffffffffffffL</b> or in the range
         * <b>0xfff0000000000001L</b> through <b>0xffffffffffffffffL</b>.
         *
         * <p>In all cases, the result is a <b>long</b> integer that, when
         * given to the <b>longBitsToDouble(long)</b> method, will produce a
         * floating-point value the same as the argument to
         * <b>doubleToLongBits</b> (except all NaN values are
         * collapsed to a single "canonical" NaN value).
         *
         * @param d   a <b>double</b> precision floating-point number.
         */
        static glong toLongBits(gdouble d);

        /**
         * Returns the <b>double</b> value corresponding to a given
         * bit representation.
         * The argument is considered to be a representation of a
         * floating-point value according to the IEEE 754 floating-point
         * "double format" bit layout.
         *
         * <p>If the argument is <b>0x7ff0000000000000L</b>, the result
         * is positive infinity.
         *
         * <p>If the argument is <b>0xfff0000000000000L</b>, the result
         * is negative infinity.
         *
         * <p>If the argument is any value in the range
         * <b>0x7ff0000000000001L</b> through
         * <b>0x7fffffffffffffffL</b> or in the range
         * <b>0xfff0000000000001L</b> through
         * <b>0xffffffffffffffffL</b>, the result is a NaN.  No IEEE
         * 754 floating-point operation provided  can distinguish
         * between two NaN values of the same type with different bit
         * patterns.
         *
         * <p>In all other cases, let <i>str</i>, <i>e</i>, and <i>m</i> be three
         * values that can be computed from the argument:
         *
         * @code
         * int s = ((bits >> 63) == 0) ? 1 : -1;
         * int e = (int)((bits >> 52) & 0x7ffL);
         * long m = (e == 0) ?
         *                 (bits & 0xfffffffffffffL) << 1 :
         *                 (bits & 0xfffffffffffffL) | 0x10000000000000L;
         * @endcode
         *
         * Then the floating-point result equals the value of the mathematical
         * expression <i>str</i>&middot;<i>m</i>&middot;2<sup><i>e</i>-1075</sup>.
         *
         * <p>Note that this method may not be able to return a
         * <b>double</b> NaN with exactly same bit pattern as the
         * <b>long</b> argument.  IEEE 754 distinguishes between two
         * kinds of NaNs, quiet NaNs and <i>signaling NaNs</i>.
         * Arithmetic operations on signaling NaNs turn them into quiet NaNs with a different,
         * but often similar, bit
         * pattern.  However, on some processors merely copying a
         * signaling NaN also performs that conversion.  In particular,
         * copying a signaling NaN to return it to the calling method
         * may perform this conversion.  So <b>longBitsToDouble</b>
         * may not be able to return a <b>double</b> with a
         * signaling NaN bit pattern.  Consequently, for some
         * <b>long</b> values,
         * <b>doubleToRawLongBits(longBitsToDouble(start))</b> may
         * <i>not</i> equal <b>start</b>.  Moreover, which
         * particular bit patterns represent signaling NaNs is platform
         * dependent; although all NaN bit patterns, quiet or signaling,
         * must be in the NaN range identified above.
         *
         * @param bits   any <b>long</b> integer.
         */
        static gdouble fromLongBits(glong bits);

        /**
         * Return sharable copy of this Double
         */
        Object &clone() const override;

        /**
         * Adds two <b>double</b> values together as per the + operator.
         *
         * <p> This method corresponds to the addition operation
         * defined in IEEE 754.
         *
         * @param x the first operand
         * @param y the second operand
         */
        static CORE_FAST gdouble sum(gdouble x, gdouble y) {
            return x + y;
        }

        /**
         * Returns the greater of two <b>double</b> values.
         *
         * <p>
         * This method corresponds to the maximum operation defined in
         * IEEE 754.
         *
         * @param x the first operand
         * @param y the second operand
         */
        static CORE_FAST gdouble max(gdouble x, gdouble y) {
            return isNaN(x) ? x :
                   ((x == 0.0f) && (y == 0.0) && toLongBits(x) == toLongBits(-0.0)) ? x :
                   (x >= y) ? x : y;
        }

        /**
         * Returns the smaller of two <b>double</b> values.
         *
         * <p>
         * This method corresponds to the minimum operation defined in
         * IEEE 754.
         *
         * @param x the first operand
         * @param y the second operand
         */
        static CORE_FAST gdouble min(gdouble x, gdouble y) {
            return isNaN(x) ? x :
                   ((x == 0.0) && (y == 0.0) && toLongBits(y) == toLongBits(-0.0)) ? y :
                   (x <= y) ? x : y;
        }

        CORE_FAST CORE_ENABLE_IMPLICIT_CAST(gdouble, value, const);

        CORE_ENABLE_IMPLICIT_CAST(gdouble &, value);

        CORE_STATIC_ASSERT(((SIGN_BIT_MASK | EXPONENT_BIT_MASK | SIGNIFICAND_BIT_MASK) == ~0LL) &&
                    (((SIGN_BIT_MASK & EXPONENT_BIT_MASK) == 0LL) &&
                     ((SIGN_BIT_MASK & SIGNIFICAND_BIT_MASK) == 0LL) &&
                     ((EXPONENT_BIT_MASK & SIGNIFICAND_BIT_MASK) == 0LL)) &&
                    ((SIGN_BIT_MASK | MAGNITUDE_BIT_MASK) == ~0LL),
                    "This compiler Compiler is not Supported for Floating operations on architecture(" CORE_ARCH ")");
    };

} // core

#endif //CORE23_DOUBLE_H
