//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_FLOAT_H
#define CORE23_FLOAT_H

#include <core/Comparable.h>

namespace core {

    /**
     * The <b> Float</b> class wraps a value of native type
     * <b> float</b> in an object. An object of type
     * <b> Float</b> contains a single field whose type is
     * <b> float</b>.
     *
     * <p>In addition, this class provides several methods for converting a
     * <b> float</b> to a <b> String</b> and a
     * <b> String</b> to a <b> float</b>, as well as other
     * constants and methods useful when dealing with a
     * <b> float</b>.
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
     * The class <b> Double</b> has a <a
     * href="#equivalenceRelation">discussion of equality,
     * equivalence, and comparison of floating-point values</a> that is
     * equally applicable to <b> float</b> values.
     *
     * @see <a href="https://standards.ieee.org/ieee/754/6210/">
     *      <cite>IEEE Standard for Floating-Point Arithmetic</cite></a>
     *
     * @author  Brunshweeck Tazeussong
     */
    class Float CORE_FINAL : public Object, public Comparable<Float> {
    private:
        /**
         * The value of Float
         */
        gfloat value;

    public:

        /**
         * A constant holding the largest positive finite value of type
         * <b>float</b>, (2-2<sup> -23 </sup>)&middot;2<sup>127</sup>.
         * It is equal to the hexadecimal floating-point literal
         * <b>0x1.fffffeP+127f</b> and also equal to <b> Float.fromIntBits(0x7f7fffff)</b>.
         */
        static CORE_FAST gfloat MAX_VALUE = 0x1.fffffeP+127f; // 3.4028235e+38f

        /**
         * A constant holding the smallest positive normal value of type
         * <b>float</b>, 2<sup> -126</sup>.  It is equal to the
         * hexadecimal floating-point literal <b>0x1.0p-126f</b> and also
         * equal to <b>Float.fromIntBits(0x00800000)</b>.
         */
        static CORE_FAST gfloat MIN_NORMAL = 0x1.0p-126f; // 1.17549435E-38f

        /**
         * A constant holding the smallest positive nonzero value of type
         * <b>float</b>, 2<sup> -149</sup>. It is equal to the
         * hexadecimal floating-point literal <b>0x0.000002P-126f</b>
         * and also equal to <b>Float.fromIntBits(0x1)</b>.
         */
        static CORE_FAST gfloat MIN_VALUE = 0x0.000002P-126f; // 1.4e-45f

        /**
         * A constant holding the positive infinity of type
         * <b>float</b>. It is equal to the value returned by
         * <b>Float.fromIntBits(0x7f800000)</b>.
         */
        static const gfloat POSITIVE_INFINITY;

        /**
         * A constant holding the negative infinity of type
         * <b>float</b>. It is equal to the value returned by
         * <b>Float.fromIntBits(0xff800000)</b>.
         */
        static const gfloat NEGATIVE_INFINITY;

        /**
         * A constant holding a Not-a-Number (NaN) value of type
         * <b>float</b>.  It is equivalent to the value returned by
         * <b>Float.fromIntBits(0x7fc00000)</b>.
         */
        static const gfloat NaN;

        /**
         * The number of bits used to represent a <b>float</b> value.
         */
        static CORE_FAST gint SIZE = 32;

        /**
         * The number of bits in the significand of a <b>float</b> value.
         */
        static CORE_FAST gint PRECISION = 24;

        /**
         * Maximum exponent a finite <b>float</b> variable may have.
         */
        static CORE_FAST gint MAX_EXPONENT = (1 << (SIZE - PRECISION - 1)) - 1; // 127;

        /**
         * Minimum exponent a normalized <b>float</b> variable may have.
         */
        static CORE_FAST gint MIN_EXPONENT = 1 - MAX_EXPONENT; // -126

        /**
         * The number of logical bits in the significand of a
         * float number, including the implicit bit.
         */
        static CORE_FAST gint SIGNIFICAND_WIDTH = PRECISION;

        /**
         * The exponent the smallest positive float subnormal value would
         * have if it could be normalized.
         */
        static CORE_FAST gint MIN_SUB_EXPONENT = MIN_EXPONENT - (SIGNIFICAND_WIDTH - 1); // -149

        /**
         * Bias used in representing a float exponent.
         */
        static CORE_FAST gint EXPONENT_BIAS = (1 << (SIZE - SIGNIFICAND_WIDTH - 1)) - 1; // 127

        /**
         * Bit mask to isolate the sign bit of a float.
         */
        static CORE_FAST gint SIGN_BIT_MASK = 1 << (SIZE - 1);

        /**
         * Bit mask to isolate the exponent field of a float.
         */
        static CORE_FAST gint EXPONENT_BIT_MASK = ((1 << (SIZE - SIGNIFICAND_WIDTH)) - 1) << (SIGNIFICAND_WIDTH - 1);

        /**
         * Bit mask to isolate the significand field of a float.
         */
        static CORE_FAST gint SIGNIFICAND_BIT_MASK = (1 << (SIGNIFICAND_WIDTH - 1)) - 1;

        /**
         * Bit mask to isolate the magnitude bits (combined exponent and
         * significand fields) of a float.
         */
        static CORE_FAST gint MAGNITUDE_BIT_MASK = EXPONENT_BIT_MASK | SIGNIFICAND_BIT_MASK;

        /**
         * Constructs a newly allocated Float object that represents
         * the native float argument.
         *
         * @param value
         *          The value to be represented by the Float.
         */
        CORE_FAST Float(gfloat value) : value(value) {}

        /**
         * Returns true if this Float value is a
         * Not-a-Number (NaN), false otherwise.
         */
        CORE_FAST gbool isNaN() const {
            return isNaN(value);
        }

        /**
         * Returns true if this Float value is infinitely large
         * in magnitude, false otherwise.
         */
        CORE_FAST gbool isInfinite() const {
            return isInfinite(value);
        }

        /**
         * Returns a new <b>float</b> initialized to the value
         * represented by the specified <b>String</b>, as performed
         * by the <b>valueOf</b> method of class <b>Float</b>.
         *
         * @param str
         *          The string to be parsed.
         *
         * @throws FormatException
         *          If the string does not contain a
         *               parsable <b>float</b>.
         * @see Float.valueOf(String)
         */
        static gfloat parseFloat(const String &str);

        /**
         * Returns a <b>Float</b> object holding the <b>float</b> value represented by
         * the argument string <b>str</b>.
         *
         * <p>
         *  Leading and trailing whitespace characters in <b>s</b> are ignored.  Whitespace
         *  is removed as if by the String.strip()</b> method; that is, both ASCII diskSpace and
         *  control characters are removed. The rest of <b>str</b> should constitute a <i>FloatValue</i>
         *  as described by the lexical syntax rules:
         *
         * <blockquote>
         * <dl>
         * <dt><i>FloatValue:</i>
         * <dd><i>Sign<sub>opt</sub></i> <b>nan</b>
         * <dd><i>Sign<sub>opt</sub></i> <b>infinity</b>
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
         * <dd><b>0x</b> <i>HexDigits<sub>opt</sub> </i><b>.</b><i> HexDigits</i>
         * <dd><b>0X</b><i> HexDigits<sub>opt</sub></i><b>.</b> <i>HexDigits</i>
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
         * <i>HexNumeral</i>, <i>HexDigits</i>, <i>SignedInteger</i>.
         * If <b>str</b> does not have the form of a <i>FloatValue</i>, then a <b>FormatException</b>
         * is thrown. Otherwise, <b>s</b> is regarded as representing an exact decimal value in the usual
         * "computerized scientific notation" or as an exact hexadecimal value; this exact numerical value is then
         * conceptually converted to an "infinitely precise" binary value that is then rounded to type <b>float</b>
         * by the usual round-to-nearest rule of IEEE 754 floating-point arithmetic, which includes preserving the
         * sign of a zero value.
         *
         * Note that the round-to-nearest rule also implies overflow and underflow behaviour; if the exact value of
         * <b>str</b> is large enough in magnitude (greater than or equal to (<b>MAX_VALUE+</b>2910/2),
         * rounding to <b>float</b> will result in an infinity and if the exact value of <b>s</b> is small enough
         * in magnitude (less than or equal to MIN_VALUE</b>/2, rounding to float will result in a zero.
         *
         * Finally, after rounding a <b>Float</b> object representing this <b>float</b> value is returned.
         *
         * <p>
         * To interpret localized string representations of a floating-point value, use subclasses of
         * core::NumberFormat</b>.
         *
         * <p>Note that trailing format specifiers, specifiers that determine the type of a floating-point literal
         * (<b>1.0f</b> is a <b>float</b> value; <b>1.0d</b> is a <b>double</b> value), do
         * <em>not</em> influence the results of this method.  In other words, the numerical value of the input
         * string is converted directly to the target floating-point type.  In general, the
         * two-step sequence of conversions, string to <b>double</b> followed by <b>double</b> to <b>float</b>, is
         * <em>not</em> equivalent to converting a string directly to <b>float</b>.  For example, if first converted
         * to an intermediate <b>double</b> and then to
         * <b>float</b>, the string<br>
         * <b>"1.00000017881393421514957253748434595763683319091796875001"</b><br>
         * results in the <b>float</b> value <b>1.0000002f</b>; if the string is converted directly to
         * <b>float</b>, <code>1.000000<b>1</b>f</code> results.
         *
         * <p>
         * To avoid calling this method on an invalid string and having a <b>FormatException</b> be thrown,
         * the documentation for Double.valueOf</b> lists a regular expression which can be used to screen
         * the input.
         *
         * @param str
         *          The string to be parsed.
         *
         * @throws FormatException
         *            If the string does not contain a parsable number.
         */
        static Float valueOf(const String &str);

        /**
         * Returns a <b>Float</b> INSTANCE representing the specified
         * <b>float</b> value.
         *
         * @param f
         *        A float value.
         */
        static Float valueOf(gfloat f);

        /**
         * Returns true if the specified number is a
         * Not-a-Number (NaN) value, false otherwise.
         *
         * @apiNote
         * This method corresponds to the isNaN operation defined in IEEE
         * 754.
         *
         * @param   v   the value to be tested.
         */
        static CORE_FAST gbool isNaN(gfloat f) {
            return (f != f);
        }

        /**
         * Returns true if the specified number is infinitely
         * large in magnitude, false otherwise.
         *
         *  <p>
         * This method corresponds to the isInfinite operation defined in
         * IEEE 754.
         *
         * @param f
         *          The value to be tested.
         */
        static CORE_FAST gbool isInfinite(gfloat f) {
            return (f < MIN_VALUE) || (f > MAX_VALUE);
        }

        /**
         * Returns true if the specified number is infinitely
         * large in magnitude, false otherwise.
         *
         *  <p>
         * This method corresponds to the isInfinite operation defined in
         * IEEE 754.
         *
         * @param f
         *        The value to be tested.
         */
        static CORE_FAST gbool isFinite(gfloat f) {
            return (f >= MIN_VALUE) && (f <= MAX_VALUE);
        }

        /**
         * Return the value of this Float as byte
         * after narrowing native conversion.
         */
        CORE_FAST gbyte byteValue() const {
            return (gbyte) value;
        }

        /**
         * Return the value of this Float as short
         * after narrowing native conversion.
         */
        CORE_FAST gshort shortValue() const {
            return (gshort) value;
        }

        /**
         * Return the value of this Float as gint
         * after narrowing native conversion.
         */
        CORE_FAST gint intValue() const {
            return (gint) value;
        }

        /**
         * Return the value of this Float as glong
         * after narrowing native conversion.
         */
        CORE_FAST glong longValue() const {
            return (glong) value;
        }

        /**
         * Return the value of this Float as float.
         */
        CORE_FAST gfloat floatValue() const {
            return (gfloat) value;
        }

        /**
         * Return the value of this Float as double
         * after widening native conversion.
         */
        CORE_FAST gdouble doubleValue() const {
            return (gdouble) value;
        }

        /**
         * Returns a string representation of this Float object.
         * The native float value represented by this object
         * is converted to a String exactly as if by the method
         * toString of one argument.
         *
         * @return  a String representation of this object.
         * @see Float#toString(float)
         */
        String toString() const override;

        /**
         * Returns a string representation of the <b>float</b> argument.
         * All characters mentioned below are ASCII characters.
         *
         * <ul>
         * <li>If the argument is NaN, the result is the string "<b>nan</b>".
         * <li>Otherwise, the result is a string that represents the sign and
         *     magnitude (absolute value) of the argument. If the sign is
         *     negative, the first character of the result is '<b>-</b>' (<b>'\\u002D'</b>);
         *     if the sign is positive, no sign character appears in the result. As for
         *     the magnitude <i>m</i>:
         *
         * <ul>
         * <li>If <i>m</i> is infinity, it is represented by the characters <b>"infinity"</b>;
         *     thus, positive infinity produces the result <b>"infinity"</b> and negative infinity
         *     produces the result <b>"-infinity"</b>.
         * <li>If <i>m</i> is zero, it is represented by the characters <b>"0.0"</b>; thus, negative
         *     zero produces the result <b>"-0.0"</b> and positive zero produces the result <b>"0.0"</b>.
         *
         * <li> Otherwise <i>m</i> is positive and finite. It is converted to a string in two stages:
         * <ul>
         * <li> <em>Selection of a decimal</em>: A well-defined decimal <i>d</i><sub><i>m</i></sub>
         *      is selected to represent <i>m</i>.
         * This decimal is (almost always) the <em>shortest</em> one that rounds to <i>m</i> according
         * to the round to nearest rounding policy of IEEE 754 floating-point arithmetic.
         * <li> <em>Formatting as a string</em>: The decimal <i>d</i><sub><i>m</i></sub> is formatted
         *      as a string, either in plain or in computerized scientific notation, depending on its value.
         * </ul>
         * </ul>
         * </ul>
         *
         * <p>
         * A <em>decimal</em> is a number of the form <i>s</i>&times;10<sup><i>i</i></sup>
         * for some (unique) integers <i>s</i> &gt; 0 and <i>i</i> such that <i>s</i> is not a multiple of 10.
         * These integers are the <em>significand</em> and the <em>exponent</em>, respectively, of the decimal.
         * The <em>length</em> of the decimal is the (unique) positive integer <i>n</i> meeting
         * 10<sup><i>n</i> -1</sup> &le; <i>s</i> &lt; 10<sup><i>n</i></sup>.
         *
         * <p>
         * The decimal <i>d</i><sub><i>m</i></sub> for a finite positive <i>m</i> is defined as follows:
         * <ul>
         * <li>Let <i>T</i> be the set of all decimals that round to <i>m</i> according to the usual
         * <em>round to nearest</em> rounding policy of IEEE 754 floating-point arithmetic.
         * <li>Let <i>p</i> be the minimal length over all decimals in <i>T</i>.
         * <li>When <i>p</i> &ge; 2, let <i>T</i> be the set of all decimals in <i>T</i> with length <i>p</i>.
         * Otherwise, let <i>T</i> be the set of all decimals in <i>T</i> with length 1 or 2.
         * <li>Define <i>d</i><sub><i>m</i></sub> as the decimal in <i>T</i> that is closest to <i>m</i>.
         * Or if there are two such decimals in <i>T</i>, select the one with the even significand.
         * </ul>
         *
         * <p>The (uniquely) selected decimal <i>d</i><sub><i>m</i></sub> is then formatted.
         * Let <i>s</i>, <i>i</i> and <i>n</i> be the significand, exponent and length of <i>d</i><sub><i>m</i></sub>,
         * respectively. Further, let <i>e</i> = <i>n</i> + <i>i</i> - 1 and let <i>s</i><sub>1</sub>&hellip;<i>s</i>
         * <sub><i>n</i></sub> be the usual decimal expansion of <i>s</i>.
         * Note that <i>s</i><sub>1</sub> &ne; 0 and <i>s</i><sub><i>n</i></sub> &ne; 0.
         * Below, the decimal point <b>'.'</b> is <b>'\\u002E'</b> and the exponent indicator <b>'E'</b> is
         * <b>'\\u0045'</b>.
         * <ul>
         * <li>Case -3 &le; <i>e</i> &lt; 0: <i>d</i><sub><i>m</i></sub> is formatted as
         * <code>0.0</code>&hellip;<code>0</code><!-- --><i>s</i><sub>1</sub>&hellip;<i>s</i><sub><i>n</i></sub>,
         * where there are exactly -(<i>n</i> + <i>i</i>) zeroes between the decimal point and <i>s</i><sub>1</sub>.
         * For example, 123 &times; 10<sup>-4</sup> is formatted as
         * <b>0.0123</b>.
         * <li>Case 0 &le; <i>e</i> &lt; 7:
         * <ul>
         * <li>Subcase <i>i</i> &ge; 0: <i>d</i><sub><i>m</i></sub> is formatted as
         * <i>s</i><sub>1</sub>&hellip;<i>s</i><sub><i>n</i></sub><code>0</code>&hellip;<code>0.0</code>,
         * where there are exactly <i>i</i> zeroes between <i>s</i><sub><i>n</i></sub> and the decimal point.
         * For example, 123 &times; 10<sup>2</sup> is formatted as <b>12300.0</b>.
         * <li>Subcase <i>i</i> &lt; 0: <i>d</i><sub><i>m</i></sub> is formatted as <i>s</i><sub>1</sub>&hellip;
         * <i>s</i><sub><i>n</i>+<i>i</i></sub><code>.</code><i>s</i><sub><i>n</i>+<i>i</i>+1</sub>&hellip;
         * <i>s</i><sub><i>n</i></sub>,where there are exactly -<i>i</i> digits to the right of the decimal point.
         * For example, 123 &times; 10<sup>-1</sup> is formatted as
         * <b>12.3</b>.
         * </ul>
         * <li>Case <i>e</i> &lt; -3 or <i>e</i> &ge; 7: computerized scientific notation is used to format
         * <i>d</i><sub><i>m</i></sub>.
         * Here <i>e</i> is formatted as by <b>Integer.toString</b>.
         *
         * <ul>
         * <li>Sub-case <i>n</i> = 1:
         * <i>d</i><sub><i>m</i></sub> is formatted as
         * <i>s</i><sub>1</sub><code>.0E</code><i>e</i>.
         * For example, 1 &times; 10<sup>23</sup> is formatted as
         * <b>1.0E23</b>.
         * <li>Sub-case <i>n</i> &gt; 1:
         * <i>d</i><sub><i>m</i></sub> is formatted as <i>s</i><sub>1</sub><code>.</code><i>s</i><sub>2</sub>&hellip;
         * <i>s</i><sub><i>n</i></sub><code>E</code><i>e</i>.
         * For example, 123 &times; 10<sup>-21</sup> is formatted as
         * <b>1.23E-19</b>.
         * </ul>
         * </ul>
         *
         * <p>To create localized string representations of a floating-point value, use subclasses of
         * <b>core::NumberFormat</b>.
         *
         * @param f
         *         The <b>float</b> to be converted.
         */
        static String toString(gfloat f);

        /**
         * Returns a hexadecimal string representation of the
         * <b>float</b> argument. All characters mentioned below are
         * ASCII characters.
         *
         * <ul>
         * <li>If the argument is NaN, the result is the string "<b>nan</b>".
         * <li>Otherwise, the result is a string that represents the sign and
         * magnitude (absolute value) of the argument. If the sign is negative,
         * the first character of the result is '<b>-</b>'
         * (<b>'\\u002D'</b>); if the sign is positive, no sign character
         * appears in the result. As for the magnitude <i>m</i>:
         *
         * <ul>
         * <li>If <i>m</i> is infinity, it is represented by the string
         * <b>"infinity"</b>; thus, positive infinity produces the
         * result <b>""</b> and negative infinity produces
         * the result <b>infinity>"-infinity"</b>.
         *
         * <li>If <i>m</i> is zero, it is represented by the string
         * <b>"0x0.0p0"</b>; thus, negative zero produces the result
         * <b>"-0x0.0p0"</b> and positive zero produces the result
         * <b>"0x0.0p0"</b>.
         *
         * <li>If <i>m</i> is a <b>float</b> value with a
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
         * <li>If <i>m</i> is a <b>float</b> value with a subnormal
         * representation, the significand is represented by the
         * characters <b>"0x0."</b> followed by a
         * hexadecimal representation of the rest of the significand as a
         * fraction.  Trailing zeros in the hexadecimal representation are
         * removed. Next, the exponent is represented by
         * <b>"p-126"</b>.  Note that there must be at
         * least one nonzero digit in a subnormal significand.
         *
         * </ul>
         *
         * </ul>
         *
         * <table>
         * <caption>Examples</caption>
         * <thead>
         *   <tr>
         *      <th>Floating-point Value</th>
         *      <th scope="col">Hexadecimal String</th>
         * </thead>
         * <tbody>
         *    <tr>
         *       <th><b> 1.0</b></th>
         *       <td><b> 0x1.0p0</b></td>
         *    <tr>
         *       <th><b> -1.0</b></th>
         *       <td><b> -0x1.0p0</b></td>
         *    <tr>
         *       <th><b>2.0</b></th>
         *       <td><b>0x1.0p1</b></td>
         *    <tr>
         *       <th><b>3.0</b></th>
         *       <td><b>0x1.8p1</b></td>
         *    <tr>
         *       <th><b>0.5</b></th>
         *       <td><b>0x1.0p-1</b></td>
         *    <tr>
         *       <th><b>0.25</b></th>
         *       <td><b>0x1.0p-2</b></td>
         *    <tr>
         *       <th><b>Float.MAX_VALUE</b></th>
         *       <td><b>0x1.fffffep+127</b></td>
         *    <tr>
         *       <th><b>Minimum Normal Value</b></th>
         *       <td><b>0x1.0p-126</b></td>
         *    <tr>
         *       <th><b>Maximum Subnormal Value</b></th>
         *       <td><b>0x0.fffffep-126</b></td>
         *    <tr>
         *       <th><b>Float.MIN_VALUE</b></th>
         *       <td><b>0x0.000002p-126</b></td>
         * </tbody>
         * </table>
         *
         * @param f
         *        The <b>float</b> to be converted.
         */
        static String toHexString(gfloat f);

        /**
         * Returns a hash code for this Float object. The
         * result is the integer bit representation, exactly as produced
         * by the method Float.toIntBits, of the native
         * float value represented by this Float object.
         */
        gint hash() const override {
            return hash(value);
        }

        /**
         * Returns a hash code for a float value;.
         *
         * @param f
         *        The value to hash
         */
        static gint hash(gfloat f);

        /**
         * Compares this object against the specified object.  The result
         * is true if and only if the argument is a Float object that
         * represents a float with the same value as the float represented by
         * this object. For this purpose, two float values are considered to be the
         * same if and only if the method Float.toIntBits(gfloat) returns the identical
         * gint value when applied to each.
         *
         * <p>
         * Note: This method is defined in terms of Float.toIntBits(gfloat) rather than the
         * == operator on float values since the == operator does <em>not</em> define an equivalence
         * relation and to satisfy the equals contract an equivalence relation must be implemented.
         *
         * @param object
         *          The object to be compared
         *
         * @see Float.toIntBits(gfloat)
         */
        gbool equals(const Object &object) const override;

        /**
         * Compares two Float objects numerically.
         *
         * This method imposes a total order on Float objects
         * with two differences compared to the incomplete order defined by
         * the language numerical comparison operators (&lt;, &le, ==, &ge;, &gt;)
         * on float values.
         *
         * <ul>
         * <li> A NaN is <em>unordered</em> with respect to other
         *      values and unequal to itself under the comparison
         *      operators.  This method chooses to define NaN to be equal
         *      to itself and greater than all other double values
         *      (including POSITIVE_INFINITY).
         *
         *      <li> Positive zero and negative zero compare equal
         *      numerically, but are distinct and distinguishable values.
         *      This method chooses to define positive zero (+0.0f),
         *      to be greater than negative zero (-0.0f).
         * </ul>
         *
         * This ensures that the <i>natural ordering</i> of Float
         * objects imposed by this method is <i>consistent with
         * equals</i>; see this discussion</a> for details of floating-point
         * comparison and ordering.
         *
         *
         * @param other
         *          The Float to be compared.
         */
        gint compareTo(const Float& other) const override;

        /**
         * Compares the two specified float values.
         *
         * @param x
         *         The first float to compare.
         * @param y
         *         The second float to compare.
         */
        static CORE_FAST gint compare(gfloat x, gfloat y) {
            return (x < y) ? -1 :
                   ((x > y) ? 1 :
                    ((x == y) ? 0 :
                     (isNaN(x) ? (isNaN(y) ? 0 : +1) :
                      -1)));
        }

        /**
         * Returns a representation of the specified floating-point value
         * according to the IEEE 754 floating-point "single format" bit
         * layout.
         *
         * <p>
         * Bit 31 (the bit that is selected by the mask 0x80000000) represents
         * the sign of the floating-point number.
         * Bits 30-23 (the bits that are selected by the mask 0x7f800000) represent
         * the exponent. Bits 22-0 (the bits that are selected by the mask 0x007fffff)
         * represent the significand (sometimes called the mantissa) of the floating-point
         * number.
         *
         * <p>
         * If the argument is positive infinity, the result is 0x7f800000.
         *
         * <p>
         * If the argument is negative infinity, the result is  0xff800000.
         *
         * <p>
         * If the argument is NaN, the result is 0x7fc00000.
         *
         * <p>
         * In all cases, the result is an integer that, when given to the
         * fromIntBits(gint) method, will produce a floating-point value the same as the
         * argument to toIntBits (except all NaN values are collapsed to a single
         * "canonical" NaN value).
         *
         * @param value
         *          A floating-point number.
         */
        static gint toIntBits(gfloat f);

        /**
         * Return the floating-point binary16 value, encoded in a short,
         * closest in value to the argument.
         *
         * <p>
         * Special cases:
         * <ul>
         * <li> If the argument is zero, the result is a zero with the same sign as the argument.
         * <li> If the argument is infinite, the result is an infinity with the same sign as the argument.
         * <li> If the argument is a NaN, the result is a NaN.
         * </ul>
         *
         * The binary16 format is discussed in more detail in the fromShortBits() method.
         * <p>
         * Note: This method corresponds to the convertFormat operation defined in IEEE 754 from the
         * binary32 format to the binary16 format. The operation of this method is analogous to a native
         * narrowing conversion.
         *
         * @param f
         *        The float value to convert to binary16
         */
        static gshort toShortBits(gfloat f);

        /**
         * Returns the float value corresponding to a given
         * bit representation.
         * The argument is considered to be a representation of a
         * floating-point value according to the IEEE 754 floating-point
         * "single format" bit layout.
         *
         * <p>
         * If the argument is 0x7f800000, the result is positive
         * infinity.
         *
         * <p>
         * If the argument is 0xff800000, the result is negative
         * infinity.
         *
         * <p>
         * If the argument is any value in the range 0x7f800001 through 0x7fffffff or in
         * the range 0xff800001 through
         * 0xffffffff, the result is a NaN.  No IEEE 754
         * floating-point operation provided by can distinguish
         * between two NaN values of the same type with different bit
         * patterns.  Distinct values of NaN are only distinguishable by
         * use of the Float.floatToRawIntBits method.
         *
         * <p>In all other cases, let <i>s</i>, <i>e</i>, and <i>m</i> be three
         * values that can be computed from the argument: <br>
         *
         * <code>
         * gint s = ((bits >> 31) == 0) ? 1 : -1; <br>
         * gint e = ((bits >> 23) & 0xff); <br>
         * gint m = (e == 0) ? (bits & 0x7fffff) << 1 : (bits & 0x7fffff) | 0x800000;
         * </code>
         * <p>
         * Then the floating-point result equals the value of the mathematical
         * expression <i>s</i>&middot;<i>m</i>&middot;2<sup><i>e</i>-150</sup>.
         *
         * <p>Note that this method may not be able to return a
         * float NaN with exactly same bit pattern as the
         * gint argument.  IEEE 754 distinguishes between two
         * kinds of NaNs, quiet NaNs and <i>signaling NaNs</i>.
         * Arithmetic operations on signaling NaNs turn
         * them into quiet NaNs with a different, but often similar, bit
         * pattern.  However, on some processors merely copying a
         * signaling NaN also performs that conversion.  In particular,
         * copying a signaling NaN to return it to the calling method may
         * perform this conversion.  So intBitsToFloat may
         * not be able to return a float with a signaling NaN
         * bit pattern.  Consequently, for some gint values,
         * floatToRawIntBits(intBitsToFloat(start)) may
         * <i>not</i> equal start.  Moreover, which
         * particular bit patterns represent signaling NaNs is platform
         * dependent; although all NaN bit patterns, quiet or signaling,
         * must be in the NaN range identified above.
         *
         * @param   bits   an integer.
         * @return  the float floating-point value with the same bit
         *          pattern.
         */
        static gfloat fromIntBits(gint bits);

        /**
         * Return the float value closest to the numerical value
         * of the argument, a floating-point binary16 value encoded in a
         * short The conversion is exact; all binary16 values can
         * be exactly represented in float.
         *
         * Special cases:
         * <ul>
         * <li> If the argument is zero, the result is a zero with the
         * same sign as the argument.
         * <li> If the argument is infinite, the result is an infinity
         * with the same sign as the argument.
         * <li> If the argument is a NaN, the result is a NaN.
         * </ul>
         *
         * <h4>IEEE 754 binary16 format</h4>
         * The IEEE 754 standard defines binary16 as a 16-bit format, along
         * with the 32-bit binary32 format (corresponding to the float type)
         * and the 64-bit binary64 format (corresponding to the double type).
         * The binary16 format is similar to the other IEEE 754 formats, except smaller,
         * having all the usual IEEE 754 values such as NaN, signed infinities, signed zeros,
         * and sub-normals. The parameters for the
         * binary16 format are N = 11 precision bits, K = 5 exponent bits,
         * <i>E</i><sub><i>max</i></sub> = 15, and
         * <i>E</i><sub><i>min</i></sub> = -14.
         *
         * <p>
         * Note: This method corresponds to the convertFormat operation defined
         * in IEEE 754 from the binary16 format to the binary32 format.
         * The operation of this method is analogous to a native
         * widening conversion.
         *
         * @param bits
         *         The binary 16 value to convert to float
         */
        static gfloat fromShortBits(gshort bits);

        /**
         * Return sharable copy of this Float
         */
        Object &clone() const override;

        /**
         * Adds two float values together as per the + operator.
         *
         * @param x
         *        The first operand
         * @param y
         *        The second operand
         */
        static CORE_FAST gfloat sum(gfloat x, gfloat y) {
            return x + y;
        }

        /**
         * Returns the greater of two float values.
         *
         * @param x
         *        The first operand
         * @param y
         *        The second operand
         */
        static CORE_FAST gfloat max(gfloat x, gfloat y) {
            return (x != x) ? x :
                   ((x == 0.0F) && (y == 0.0F) && toIntBits(x) == toIntBits(-0.0F)) ? x :
                   (x >= y) ? x : y;
        }

        /**
         * Returns the smaller of two float values.
         *
         * @param x
         *        The first operand
         * @param y
         *        The second operand
         */
        static CORE_FAST gfloat min(gfloat x, gfloat y) {
            return (x != x) ? x :
                   ((x == 0.0F) && (y == 0.0F) && toIntBits(y) == toIntBits(-0.0F)) ? y :
                   (x <= y) ? x : y;
        }

        CORE_FAST static gint BYTES = 4;

        CORE_FAST CORE_ENABLE_IMPLICIT_CAST(gfloat, value, const)

        CORE_ENABLE_IMPLICIT_CAST(gfloat &, value, &)

        CORE_STATIC_ASSERT(((SIGN_BIT_MASK | EXPONENT_BIT_MASK | SIGNIFICAND_BIT_MASK) == ~0) &&
                    (((SIGN_BIT_MASK & EXPONENT_BIT_MASK) == 0) &&
                     ((SIGN_BIT_MASK & SIGNIFICAND_BIT_MASK) == 0) &&
                     ((EXPONENT_BIT_MASK & SIGNIFICAND_BIT_MASK) == 0)) &&
                    ((SIGN_BIT_MASK | MAGNITUDE_BIT_MASK) == ~0),
                       "This compiler Compiler is not Supported for Floating operations on architecture(" CORE_ARCH ")");
    };

} // core

#endif //CORE23_FLOAT_H
