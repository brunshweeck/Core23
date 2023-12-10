//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_INTEGER_H
#define CORE23_INTEGER_H

#include <core/Comparable.h>
#include "Math.h"

namespace core {

    /**
     * The <b> Integer</b> class wraps a value of the native type
     * <b> int</b> in an object. An object of type <b> Integer</b>
     * contains a single field whose type is <b> int</b>.
     *
     * <p>In addition, this class provides several methods for converting
     * an <b> int</b> to a <b> String</b> and a <b> String</b> to an
     * <b> int</b>, as well as other constants and methods useful when
     * dealing with an <b> int</b>.
     *
     * <p>This is a <a href="">value-based</a>
     * class; programmers should treat instances that are
     * <b style="color: green"> equal</b> as interchangeable and should not
     * use instances for synchronization, or unpredictable behavior may
     * occur. For example, in a future release, synchronization may fail.
     *
     * <p>Implementation note: The implementations of the "bit twiddling"
     * methods (such as <b style="color: orange"> highestOneBit</b> and
     * <b style="color: orange"> trailingZeros</b>) are
     * based on material from Henry S. Warren, Jr.'s <i>Hacker's
     * Delight</i>, (Addison Wesley, 2002).
     *
     * @author  Brunshweeck Tazeussong
     */
    class Integer CORE_FINAL  : public Object, public Comparable<Integer> {
    private:
        /**
         * The value of the Integer
         */
        gint value = {};

    public:
        /**
         * A constant holding the maximum value a int can have, 2<sup>31</sup> - 1
         */
        static CORE_FAST gint MAX_VALUE = 0x7fffffff;

        /**
         * A constant holding the minimum value a int can have, -2<sup>31</sup>
         */
        static CORE_FAST gint MIN_VALUE = ~0x7fffffff;

        /**
         * Construct new Integer object with default initialization
         *
         * @param value
         *         The value of the Integer.
         */
        CORE_FAST Integer() = default;

        /**
         * Construct new Integer object that represent the specified
         * int value
         *
         * @param value
         *         The value of the Integer.
         */
        CORE_FAST Integer(gint value) : value(value) {}

        /**
         * Parses the string argument as a signed int in the base
         * specified by the second argument. The characters in the
         * string must all be digits, of the specified base except
         * that the first character may be an ASCII minus sign '-'
         * (U+002D) to indicate a negative value or an ASCII plus
         * sign '+' (U+002B) to indicate a positive value.  The
         * resulting int value is returned.
         *
         * <p>
         * An exception of type FormatException is thrown if any of the
         * following situations occurs:
         * <ol>
         *  <li> The first argument is a String of length zero.
         *  <li> The second argument is either small than 2 or great than 36
         *  <li> Any character of the string is not a digit of the specified base,
         *       except that the first character may be a minus sign '-' (U+002D)
         *       or plus sign '+' (U+002B) provided that the string is longer than
         *       length 1.
         *  <li> The value represented by the string is not a value of type int
         * </ol>
         *
         * <p>Examples:
         * <blockquote><pre>
         * parseInt("0", 10) returns 0 <br>
         * parseInt("473", 10) returns 473 <br>
         * parseInt("+42", 10) returns 42 <br>
         * parseInt("-0", 10) returns 0 <br>
         * parseInt("-FF", 16) returns -255 <br>
         * parseInt("1100110", 2) returns 102 <br>
         * parseInt("2147483647", 10) returns 2147483647 <br>
         * parseInt("-2147483648", 10) returns -2147483648 <br>
         * parseInt("2147483648", 10) throws a FormatException <br>
         * parseInt("99", 8) throws a FormatException <br>
         * parseInt("Kona", 10) throws a FormatException <br>
         * parseInt("Kona", 27) returns 411787 <br>
         * </pre></blockquote>
         *
         * @param str
         *        The String containing the int representation to be parsed.
         * @param base
         *         The base to be used while parsing string
         * @throws FormatException
         *          If the string not contains parsable int
         */
        static gint parseInt(const String &str, gint base);

        /**
         * Parses the string argument as a signed decimal int.
         * The characters in the string must all be decimal digits,
         * except that the first character may be an ASCII minus sign
         * '-' (U+002D) to indicate a negative value or an ASCII plus
         * sign '+' (U+002B) to indicate a positive value. The resulting
         * int value is returned, exactly as if the argument and the base
         * 10  were given as arguments to the parseInt(String, gint) method.
         *
         * @param str
         *        The String containing the int representation to be parsed.
         * @throws FormatException
         *          If the string not contains parsable int
         */
        static gint parseInt(const String &str);

        /**
         * Parses the string argument as an unsigned integer in the base
         * specified by the second argument.  An unsigned integer maps the
         * values usually associated with negative numbers to positive
         * numbers larger than MAX_VALUE.
         *
         * The characters in the string must all be digits of the
         * specified radi, except that the first character may be an ASCII plus
         * sign '+' ('\\u002B'). The resulting integer value is returned.
         *
         * <p>
         * An exception of type FormatException is thrown if any of the following
         * situations occurs:
         * <ul>
         * <li>The first argument is a string of length zero.
         *
         * <li>The base  is either smaller than 2 or larger than 36.
         *
         * <li>Any character of the string is not a digit of the specified
         * base , except that the first character may be a plus sign '+' ('\\u002B')
         * provided that the string is longer than length 1.
         *
         * <li>The value represented by the string is larger than the
         * largest unsigned int, 2<sup>32</sup>-1.
         *
         * </ul>
         *
         *
         * @param      str   the String containing the unsigned integer
         *                  representation to be parsed
         * @param      base    the base  to be used while parsing str.
         * @return     the integer represented by the string argument in the
         *             specified base .
         * @throws     NumberFormatException if the String does not contain a parsable int.
         */
        static gint parseUnsignedInt(const String &str, gint base);

        /**
         * Parses the string argument as an unsigned decimal integer. The
         * characters in the string must all be decimal digits, except
         * that the first character may be an ASCII plus sign
         * '+' ('\\u002B'). The resulting integer value
         * is returned, exactly as if the argument and the base  10 were
         * given as arguments to the parseUnsignedInt(String, int) method.
         *
         * @param str   a String containing the unsigned int representation to be parsed
         *
         * @throws FormatException  if the string does not contain a
         *            parsable unsigned integer.
         */
        static gint parseUnsignedInt(const String &str);

        /**
         * Returns a Integer object holding the value extracted from the specified
         * String when parsed with the base given by the second argument. The
         * first argument is interpreted as representing a signed int in
         * the base specified by the second argument, exactly as if the
         * argument were given to the parseInt(String, gint) method. The result
         * is a Integer object that represents the int value specified by the string.
         *
         * @param str
         *        The string to be parsed
         * @param base
         *         The base to be used in interpreting str
         * @throws FormatException
         *          If the String does not contain a parsable int.
         */
        static Integer valueOf(const String &str, gint base);

        /**
         * Returns a Integer object holding the value given by the specified
         * String. The argument is interpreted as representing a signed
         * decimal int, exactly as if the argument were given to the
         * parseInt(String) method. The result is a Integer object that
         * represents the int value specified by the string.
         *
         * @param str
         *        The string to be parsed
         * @throws FormatException
         *          If the String does not contain a parsable int.
         */
        static Integer valueOf(const String &str);

        /**
         * Return an Integer object represented by the given value
         */
        static CORE_FAST Integer valueOf(gint i) { return {i}; }

        /**
         * Decodes a String into a Integer.
         * Accepts decimal, hexadecimal, and octal numbers given by
         * the following grammar:
         *
         * <blockquote>
         * <dl>
         * <dt><i>Decodable String:</i>
         * <dd><i>Sign<sub>opt</sub> DecimalDigits</i>
         * <dd><i>Sign<sub>opt</sub></i> 0x <i>HexDigits</i>
         * <dd><i>Sign<sub>opt</sub></i> 0X <i>HexDigits</i>
         * <dd><i>Sign<sub>opt</sub></i> # <i>HexDigits</i>
         * <dd><i>Sign<sub>opt</sub></i> 0 <i>OctalDigits</i>
         * <dd><i>Sign<sub>opt</sub></i> 0b <i>BinaryDigits</i>
         * <dd><i>Sign<sub>opt</sub></i> 0B <i>BinaryDigits</i>
         *
         * <dt><i>Sign:</i>
         * <dd> -
         * <dd> +
         * </dl>
         * </blockquote>
         *
         * <p>The sequence of characters following an optional
         * sign and/or base specifier ("0x", "0X",
         * "#", "0b", "0B", or leading zero) is parsed as by the
         * parseInt method with the indicated base (10, 16, 8 or 2).
         * This sequence of characters must represent a positive value or
         * a FormatException will be thrown.  The result is negated if first
         * character of the specified String is the minus sign.  No whitespace
         * characters are permitted in the String.
         *
         * @param str
         *        The String to decode.
         * @throws FormatException
         *          If the String does not contain a parsable int.
         *
         * @see Integer.parseInt(String, int)
         */
        static Integer decode(const String &str);

        /**
         * Return the value of this Byte as byte
         */
        CORE_FAST gbyte byteValue() const {
            return (gbyte) value;
        }

        /**
         * Return the value of this Byte as short after
         * widening native conversion
         */
        CORE_FAST gshort shortValue() const {
            return (gshort) value;
        }

        /**
         * Return the value of this Byte as int after
         * widening native conversion
         */
        CORE_FAST gint intValue() const {
            return (gint) value;
        }

        /**
         * Return the value of this Byte as long after
         * widening native conversion
         */
        CORE_FAST glong longValue() const {
            return (glong) value;
        }

        /**
         * Return the value of this Byte as float after
         * widening native conversion
         */
        CORE_FAST gfloat floatValue() const {
            return (gfloat) value;
        }

        /**
         * Return the value of this Byte as double after
         * widening native conversion
         */
        CORE_FAST gdouble doubleValue() const {
            return (gdouble) value;
        }

        /**
         * Return String object representing this Byte's value.
         * The value is converted to signed decimal representation
         * and returned as a string.
         */
        String toString() const override;

        /**
         * Return the String object representing the specified int.
         * The base is assumed to be 10.
         *
         * @param i
         *        The int to be converted
         */
        static String toString(gint i);

        /**
         * Returns a string representation of the first argument in the
         * base specified by the second argument.
         *
         * <p>
         * If the base is smaller than 2 or larger than 36, then the base
         * 10 is used instead.
         *
         * <p>
         * If the first argument is negative, the first element of the
         * result is the ASCII minus character '-' (U+002D). If the first argument is not
         * negative, no sign character appears in the result.
         *
         * <p>The remaining characters of the result represent the magnitude
         * of the first argument. If the magnitude is zero, it is
         * represented by a single zero character '0' (U+0030); otherwise,
         * the first character of the representation of the magnitude will not be the zero
         * character.  The following ASCII characters are used as digits:
         * '0123456789abcdefghijklmnopqrstuvwxyz'
         *
         * These are U+0030 through U+0039 and U+0061 through U+007A. If base is
         * <var>N</var>, then the first <var>N</var> of these characters
         * are used as base - <var>N</var> digits in the order shown. Thus,
         * the digits for hexadecimal (base 16) are 0123456789abcdef.
         *
         * @param   i
         *          an integer to be converted to a string.
         * @param   base
         *          the base to use in the string representation.
         */
        static String toString(gint i, gint base);

        /**
         * Returns a string representation of the first argument as an
         * unsigned integer value in the base specified by the second
         * argument.
         *
         * <p>
         * If the base is smaller than 2 or larger than 36, then the base
         * 10 is used instead.
         *
         * <p>
         * Note that since the first argument is treated as an unsigned
         * value, no leading sign character is printed.
         *
         * <p>
         * If the magnitude is zero, it is represented by a single zero character
         * '0' (U+0030); otherwise, the first character of the representation of the
         * magnitude will not be the zero character.
         *
         * <p>The behavior of base and the characters used as digits
         * are the same as toString.
         *
         * @param   i
         *          An integer to be converted to an unsigned string.
         * @param   base
         *          The base to use in the string representation.
         *
         * @see Integer.toString(gint, gint)
         */
        static String toUnsignedString(gint i, gint base);

        /**
         * Returns a string representation of the argument as an unsigned
         * decimal value.
         *
         * The argument is converted to unsigned decimal representation
         * and returned as a string exactly as if the argument and base
         * 10 were given as arguments to the toUnsignedString(gint, gint) method.
         *
         * @param   i
         *          An integer to be converted to an unsigned string.
         *
         * @see Integer.toUnsignedString(gint, gint)
         */
        static String toUnsignedString(gint i);

        /**
         * Returns a string representation of the integer argument as an
         * unsigned integer in base 16.
         *
         * <p>
         * The unsigned integer value is the argument plus 2<sup>32</sup>
         * if the argument is negative; otherwise, it is equal to the
         * argument.  This value is converted to a string of ASCII digits
         * in hexadecimal (base 16) with no extra leading zeros.
         *
         * <p>
         * The value of the argument can be recovered from the returned
         * string str by calling Integer.parseUnsignedInt(str, 16).
         *
         * <p>
         * If the unsigned magnitude is zero, it is represented by a
         * single zero character '0' (U+0030); otherwise, the first
         * character of the representation of the unsigned magnitude
         * will not be the zero character. The following characters
         * are used as hexadecimal digits: 0123456789abcdef
         *
         * These are the characters U+0030 through U+0039 and U+0061
         * through U+0066.
         *
         * @param   i
         *          An integer to be converted to a string.
         *
         * @see Integer.parseUnsignedInt(String, gint)
         * @see Integer.toUnsignedString(gint, gint)
         */
        static String toHexString(gint i);

        /**
         * Returns a string representation of the integer argument as an
         * unsigned integer in base 8.
         *
         * <p>
         * The unsigned integer value is the argument plus 2<sup>32</sup>
         * if the argument is negative; otherwise, it is equal to the
         * argument.  This value is converted to a string of ASCII digits
         * in octal (base 8) with no extra leading zeros.
         *
         * <p>
         * The value of the argument can be recovered from the returned
         * string str by calling Integer.parseUnsignedInt(str, 8).
         *
         * <p>
         * If the unsigned magnitude is zero, it is represented by a
         * single zero character '0' (U+0030); otherwise, the first
         * character of the representation of the unsigned magnitude
         * will not be the zero character. The following characters
         * are used as octal digits: 01234567
         *
         * These are the characters U+0030 through U+0037.
         *
         * @param   i
         *          An integer to be converted to a string.
         *
         * @see Integer.parseUnsignedInt(String, gint)
         * @see Integer.toUnsignedString(gint, gint)
         */
        static String toOctalString(gint i);

        /**
         * Returns a string representation of the integer argument as an
         * unsigned integer in base 2.
         *
         * <p>
         * The unsigned integer value is the argument plus 2<sup>32</sup>
         * if the argument is negative; otherwise, it is equal to the
         * argument.  This value is converted to a string of ASCII digits
         * in binary (base 2) with no extra leading zeros.
         *
         * <p>
         * The value of the argument can be recovered from the returned
         * string str by calling Integer.parseUnsignedInt(str, 2).
         *
         * <p>
         * If the unsigned magnitude is zero, it is represented by a
         * single zero character '0' (U+0030); otherwise, the first
         * character of the representation of the unsigned magnitude
         * will not be the zero character. The following characters
         * are used as binary digits: 0 and 1
         *
         * These are the characters U+0030 through U+0031.
         *
         * @param   i
         *          An integer to be converted to a string.
         *
         * @see Integer.parseUnsignedInt(String, gint)
         * @see Integer.toUnsignedString(gint, gint)
         */
        static String toBinaryString(gint i);

        /**
         * Return the hash code of this Integer's value
         */
        gint hash() const override { return hash(value); }

        /**
         * Return the hash code of specified int value
         *
         * @param i
         *        The value to hash
         */
        static CORE_FAST gint hash(gint i) { return i; }

        /**
         * Return true if and only if the object argument
         * is Integer object that represent the same value
         * as this object.
         *
         * @param object
         *          The object to compared with
         */
        gbool equals(const Object &object) const override;

        /**
         * Compares two Integer objects numerically.
         *
         * @param other
         *          The Integer to be compared
         */
        gint compareTo(const Integer &other) const override;

        /**
         * Compares two int values numerically.
         *
         * @param x
         *        The first int to compare
         * @param y
         *         The second int to compare
         */
        static CORE_FAST gint compare(gint x, gint y) { return (x == y) ? 0 : ((x < y) ? -1 : 1); }

        /**
         * Compares two int values numerically.
         *
         * @param x
         *        The first int to compare
         * @param y
         *         The second int to compare
         */
        static CORE_FAST gint compareUnsigned(gint x, gint y) { return compare(x + MIN_VALUE, y + MIN_VALUE); }

        /**
         * Converts the argument to a long by an unsigned conversion.
         * In an unsigned conversion to a long, the high-order 32 bits of
         * the int are zero and the low-order 8 bits are equal to the bits
         * of the int argument.
         *
         * <p>
         * Consequently, zero and positive int values are mapped
         * to a numerically equal int value and negative int values
         * are mapped to an int value equal to the input plus 2<sup>32</sup>.
         *
         * @param i
         *        The value to convert to an unsigned int
         */
        static CORE_FAST glong toUnsignedLong(gint i) { return ((glong) i) & 0xffffffffLL; }

        /**
         * Returns an <b> int</b> value with at most a single one-bit, in the
         * position of the highest-order ("leftmost") one-bit in the specified
         * <b> int</b> value.  Returns zero if the specified value has no
         * one-bits in its two's complement binary representation, that is, if it
         * is equal to zero.
         *
         * @param i the value whose highest one bit is to be computed
         * @return an <b> int</b> value with a single one-bit, in the position
         *     of the highest-order one-bit in the specified value, or zero if
         *     the specified value is itself equal to zero.
         */
        static gint highestOneBit(gint i) { return i & (MIN_VALUE >> leadingZeros(i)); }

        /**
         * Returns an <b> int</b> value with at most a single one-bit, in the
         * position of the lowest-order ("rightmost") one-bit in the specified
         * <b> int</b> value.  Returns zero if the specified value has no
         * one-bits in its two's complement binary representation, that is, if it
         * is equal to zero.
         *
         * @param i the value whose lowest one bit is to be computed
         * @return an <b> int</b> value with a single one-bit, in the position
         *     of the lowest-order one-bit in the specified value, or zero if
         *     the specified value is itself equal to zero.
         */
        static CORE_FAST gint lowestOneBit(gint i) {
            // HD, Section 2-1
            return i & -i;
        }

        /**
         * Returns the number of zero bits preceding the highest-order
         * ("leftmost") one-bit in the two's complement binary representation
         * of the specified int value.  Returns 32 if the
         * specified value has no one-bits in its two's complement representation,
         * in other words if it is equal to zero.
         *
         * @param i
         *        The value whose number of leading zeros is to be computed
         */
        static gint leadingZeros(gint i);

        /**
         * Returns the number of zero bits following the lowest-order ("rightmost")
         * one-bit in the two's complement binary representation of the specified
         * int value.  Returns 32 if the specified value has no one-bits in its two's
         * complement representation, in other words if it is equal to zero.
         *
         * @param i
         *        The value whose number of trailing zeros is to be computed
         */
        static gint trailingZeros(gint i);

        /**
         * Returns the number of one-bits in the two's complement binary
         * representation of the specified int value.  This function is
         * sometimes referred to as the <i>population count</i>.
         *
         * @param i
         *        The value whose bits are to be counted
         */
        static gint bitCount(gint i);

        /**
         * Returns the value obtained by rotating the two's complement binary
         * representation of the specified int value left by the
         * specified number of bits.  (Bits shifted out of the left hand, or
         * high-order, side reenter on the right, or low-order.)
         *
         * <p>Note that left rotation with a negative distance is equivalent to
         * right rotation: rotateLeft(val, -distance) == rotateRight(val, distance).
         * Note also that rotation by any multiple of 32 is a no-op, so all but the
         * last five bits of the rotation distance can be ignored, even if the distance
         * is negative: rotateLeft(val, distance) == rotateLeft(val, distance & 0x1F).
         *
         * @param i
         *        The value whose bits are to be rotated left
         * @param n
         *        The number of bit positions to rotate left
         */
        static CORE_FAST gint rotateLeft(gint i, gint n) { return (i << n) | (i >> -n); }

        /**
         * Returns the value obtained by rotating the two's complement binary
         * representation of the specified int value right by the
         * specified number of bits.  (Bits shifted out of the right hand, or
         * low-order, side reenter on the left, or high-order.)
         *
         * <p>Note that right rotation with a negative distance is equivalent to
         * left rotation: rotateRight(val, -distance) == rotateLeft(val, distance).
         * Note also that rotation by any multiple of 32 is a no-op, so all but the
         * last five bits of the rotation distance can be ignored, even if the distance
         * is negative: rotateRight(val, distance) == rotateRight(val, distance & 0x1F).
         *
         * @param i
         *        The value whose bits are to be rotated right
         * @param n
         *        The number of bit positions to rotate right
         */
        static CORE_FAST gint rotateRight(gint i, gint n) { return (i >> n) | (i << -n); }

        /**
         * Returns the value obtained by reversing the order of the bits in the
         * two's complement binary representation of the specified int value.
         *
         * @param i
         *        The value to be reversed
         */
        static gint reverseBits(gint i);

        /**
         * Returns the value obtained by reversing the order of the bytes in the
         * two's complement representation of the specified int value.
         *
         * @param i
         *        The value whose bytes are to be reversed
         */
        static CORE_FAST gint reverseBytes(gint i) {
            return (i << 24) |
                   ((i & 0xff00) << 8) |
                   ((i >> 8) & 0xff00) |
                   (i >> 24);
        }

        /**
         * Returns the signum function of the specified int value.  (The
         * return value is -1 if the specified value is negative; 0 if the
         * specified value is zero; and 1 if the specified value is positive.)
         *
         * @param i
         *        The value whose signum is to be computed
         */
        static CORE_FAST gint signum(gint i) { return (i >> 31) | (-i >> 31); }

        /**
         * Adds two integers together as per the + operator.
         *
         * @param x
         *        The first operand
         * @param y
         *        The second operand
         */
        static CORE_FAST gint sum(gint x, gint y) { return x + y; }

        /**
         * Returns the greater of two int values
         *
         * @param x
         *        The first operand
         * @param y
         *        The second operand
         */
        static CORE_FAST gint max(gint x, gint y) { return Math::max(x, y); }

        /**
         * Returns the smaller of two int values
         *
         * @param x
         *        The first operand
         * @param y
         *        The second operand
         */
        static CORE_FAST gint min(gint x, gint y) { return Math::min(x, y); }

        /**
         * Return sharable copy of this Integer instance
         */
        Object &clone() const override;

        CORE_FAST CORE_ENABLE_IMPLICIT_CAST(gint, value, const)

        CORE_ENABLE_IMPLICIT_CAST(gint &, value)
    };

} // core

#endif //CORE23_INTEGER_H
