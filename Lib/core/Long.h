//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_LONG_H
#define CORE23_LONG_H

#include <core/Comparable.h>
#include "Math.h"

namespace core {

    /**
     * The <b>Long</b> class wraps a value of the native type <b>
     * glong</b> in an object. An object of type <b>Long</b> contains a
     * single field whose type is <b>glong</b>.
     *
     * <p> In addition, this class provides several methods for converting
     * a <b>glong</b> to a <b>String</b> and a <b>String</b> to a <b>
     * glong</b>, as well as other constants and methods useful when dealing
     * with a <b>glong</b>.
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
    class Long CORE_FINAL  : public Object, public Comparable<Long> {
    private:
        /**
         * The value of the Long
         */
        glong value;

    public:
        /**
         * A constant holding the maximum value a glong can have, 2<sup>15</sup> - 1
         */
        static CORE_FAST glong MAX_VALUE = 0x7fffffffffffffff;

        /**
         * A constant holding the minimum value a glong can have, -2<sup>15</sup>
         */
        static CORE_FAST glong MIN_VALUE = ~0x7fffffffffffffff;

        /**
         * Construct new Long object that represent the specified
         * glong value
         *
         * @param value
         *         The value of the Long.
         */
        CORE_FAST Long(glong value) : value(value) {}

        /**
         * Parses the string argument as a signed glong in the base
         * specified by the second argument. The characters in the
         * string must all be digits, of the specified base except
         * that the first character may be an ASCII minus sign '-'
         * (U+002D) to indicate a negative value or an ASCII plus
         * sign '+' (U+002B) to indicate a positive value.  The
         * resulting glong value is returned.
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
         *  <li> The value represented by the string is not a value of type glong
         * </ol>
         *
         * @param str
         *        The String containing the glong representation to be parsed.
         * @param base
         *         The base to be used while parsing string
         * @throws FormatException
         *          If the string not contains parsable glong
         */
        static glong parseLong(const String &str, gint base);

        /**
         * Parses the string argument as a signed decimal glong.
         * The characters in the string must all be decimal digits,
         * except that the first character may be an ASCII minus sign
         * '-' (U+002D) to indicate a negative value or an ASCII plus
         * sign '+' (U+002B) to indicate a positive value. The resulting
         * glong value is returned, exactly as if the argument and the base
         * 10  were given as arguments to the parseLong(String, glong) method.
         *
         * @param str
         *        The String containing the glong representation to be parsed.
         * @throws FormatException
         *          If the string not contains parsable glong
         */
        static glong parseLong(const String &str);

        /**
         * Parses the string argument as an unsigned glong in the
         * base specified by the second argument.  An unsigned integer
         * maps the values usually associated with negative numbers to
         * positive numbers larger than MAX_VALUE.
         *
         * The characters in the string must all be digits of the
         * specified base , except that the first character may be an ASCII plus
         * sign '+' ('\\u002B'). The resulting
         * integer value is returned.
         *
         * <p>An exception of type NumberFormatException is
         * thrown if any of the following situations occurs:
         * <ul>
         * <li>The first argument is null or is a string of
         * length zero.
         *
         * <li>The base is either smaller than 2 or larger than 36.
         *
         * <li>Any character of the string is not a digit of the specified
         * base, except that the first character may be a plus sign
         * '+' ('\\u002B') provided that the string is longer than length 1.
         *
         * <li>The value represented by the string is larger than the
         * largest unsigned glong, 2<sup>64</sup>-1.
         *
         * </ul>
         *
         *
         * @param str   the String containing the unsigned integer
         *                  representation to be parsed
         * @param  base   the base to be used while parsing str.
         * @return     the unsigned glong represented by the string
         *             argument in the specified base.
         * @throws FormatException if the String
         *             does not contain a parsable glong.
         */
        static glong parseUnsignedLong(const String &str, gint base);

        /**
         * Parses the string argument as an unsigned decimal glong. The
         * characters in the string must all be decimal digits, except
         * that the first character may be an ASCII plus sign
         * '+' ('\\u002B'). The resulting integer value
         * is returned, exactly as if the argument and the base 10 were
         * given as arguments to the parseUnsignedLong(String, gint) method.
         *
         * @param str   a String containing the unsigned glong
         *            representation to be parsed
         *
         * @throws FormatException  if the string does not contain a
         *            parsable unsigned integer.
         */
        static glong parseUnsignedLong(const String &str);

        /**
         * Returns a Long object holding the value extracted from the specified
         * String when parsed with the base given by the second argument. The
         * first argument is interpreted as representing a signed glong in
         * the base specified by the second argument, exactly as if the
         * argument were given to the parseLong(String, gint) method. The result
         * is a Long object that represents the glong value specified by the string.
         *
         * @param str
         *        The string to be parsed
         * @param base
         *         The base to be used in interpreting str
         * @throws FormatException
         *          If the String does not contain a parsable glong.
         */
        static Long valueOf(const String &str, gint base);

        /**
         * Returns a Long object holding the value given by the specified
         * String. The argument is interpreted as representing a signed
         * decimal glong, exactly as if the argument were given to the
         * parseLong(String) method. The result is a Long object that
         * represents the glong value specified by the string.
         *
         * @param str
         *        The string to be parsed
         * @throws FormatException
         *          If the String does not contain a parsable glong.
         */
        static Long valueOf(const String &str);

        static Long valueOf(glong l);

        /**
         * Decodes a String into a Long.
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
         * parseLong method with the indicated base (10, 16, 8 or 2).
         * This sequence of characters must represent a positive value or
         * a FormatException will be thrown.  The result is negated if first
         * character of the specified String is the minus sign.  No whitespace
         * characters are permitted in the String.
         *
         * @param str
         *        The String to decode.
         * @throws FormatException
         *          If the String does not contain a parsable glong.
         *
         * @see Long.parseLong(String, glong)
         */
        static Long decode(const String &str);

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
         * Return the value of this Byte as gint after
         * widening native conversion
         */
        CORE_FAST gint intValue() const {
            return (gint) value;
        }

        /**
         * Return the value of this Byte as glong after
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
         * Return the String object representing the specified glong.
         * The base is assumed to be 10.
         *
         * @param l
         *        The glong to be converted
         */
        static String toString(glong l);

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
         *          an glong to be converted to a string.
         * @param   base
         *          the base to use in the string representation.
         */
        static String toString(glong l, gint base);

        /**
         * Returns a string representation of the first argument as an
         * unsigned glong value in the base specified by the second
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
         *          An glong to be converted to an unsigned string.
         * @param   base
         *          The base to use in the string representation.
         *
         * @see Long.toString(glong, gint)
         */
        static String toUnsignedString(glong l, gint base);

        /**
         * Returns a string representation of the argument as an unsigned
         * decimal value.
         *
         * The argument is converted to unsigned decimal representation
         * and returned as a string exactly as if the argument and base
         * 10 were given as arguments to the toUnsignedString(glong, gint) method.
         *
         * @param   i
         *          An glong to be converted to an unsigned string.
         *
         * @see Long.toUnsignedString(glong, gint)
         */
        static String toUnsignedString(glong l);

        /**
         * Returns a string representation of the glong argument as an
         * unsigned glong in base 16.
         *
         * <p>
         * The unsigned glong value is the argument plus 2<sup>64</sup>
         * if the argument is negative; otherwise, it is equal to the
         * argument.  This value is converted to a string of ASCII digits
         * in hexadecimal (base 16) with no extra leading zeros.
         *
         * <p>
         * The value of the argument can be recovered from the returned
         * string str by calling Long.parseUnsignedLong(str, 16).
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
         *          An glong to be converted to a string.
         *
         * @see Long.parseUnsignedLong(String, gint)
         * @see Long.toUnsignedString(glong, gint)
         */
        static String toHexString(glong l);

        /**
         * Returns a string representation of the glong argument as an
         * unsigned glong in base 8.
         *
         * <p>
         * The unsigned glong value is the argument plus 2<sup>64</sup>
         * if the argument is negative; otherwise, it is equal to the
         * argument.  This value is converted to a string of ASCII digits
         * in octal (base 8) with no extra leading zeros.
         *
         * <p>
         * The value of the argument can be recovered from the returned
         * string str by calling Long.parseUnsignedLong(str, 8).
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
         *          An glong to be converted to a string.
         *
         * @see Long.parseUnsignedLong(String, gint)
         * @see Long.toUnsignedString(glong, gint)
         */
        static String toOctalString(glong l);

        /**
         * Returns a string representation of the glong argument as an
         * unsigned glong in base 2.
         *
         * <p>
         * The unsigned glong value is the argument plus 2<sup>64</sup>
         * if the argument is negative; otherwise, it is equal to the
         * argument.  This value is converted to a string of ASCII digits
         * in binary (base 2) with no extra leading zeros.
         *
         * <p>
         * The value of the argument can be recovered from the returned
         * string str by calling Long.parseUnsignedLong(str, 2).
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
         *          An glong to be converted to a string.
         *
         * @see Long.parseUnsignedLong(String, gint)
         * @see Long.toUnsignedString(glong, gint)
         */
        static String toBinaryString(glong l);

        /**
         * Return the hash code of this Long's value
         */
        gint hash() const override { return hash(value); }

        /**
         * Return the hash code of specified glong value
         *
         * @param l
         *        The value to hash
         */
        static CORE_FAST gint hash(glong l) { return (gint) (l ^ (l >> 32)); }

        /**
         * Return true if and only if the object argument
         * is Long object that represent the same value
         * as this object.
         *
         * @param object
         *          The object to compared with
         */
        gbool equals(const Object &object) const override;

        /**
         * Compares two Long objects numerically.
         *
         * @param other
         *          The Long to be compared
         */
        gint compareTo(const Long &other) const override;

        /**
         * Compares two glong values numerically.
         *
         * @param x
         *        The first glong to compare
         * @param y
         *         The second glong to compare
         */
        static CORE_FAST gint compare(glong x, glong y) { return (x == y) ? 0 : ((x < y) ? -1 : 1); }

        /**
         * Compares two glong values numerically.
         *
         * @param x
         *        The first glong to compare
         * @param y
         *         The second glong to compare
         */
        static CORE_FAST gint compareUnsigned(glong x, glong y) { return compare(x + MIN_VALUE, y + MIN_VALUE); }

        /**
         * Returns a <b> glong</b> value with at most a single one-bit, in the
         * position of the highest-order ("leftmost") one-bit in the specified
         * <b> glong</b> value.  Returns zero if the specified value has no
         * one-bits in its two's complement binary representation, that is, if it
         * is equal to zero.
         *
         * @param i the value whose highest one bit is to be computed
         * @return a <b> glong</b> value with a single one-bit, in the position
         *     of the highest-order one-bit in the specified value, or zero if
         *     the specified value is itself equal to zero.
         *
         */
        static glong highestOneBit(glong i) { return i & (MIN_VALUE >> leadingZeros(i)); }

        /**
         * Returns a <b> glong</b> value with at most a single one-bit, in the
         * position of the lowest-order ("rightmost") one-bit in the specified
         * <b> glong</b> value.  Returns zero if the specified value has no
         * one-bits in its two's complement binary representation, that is, if it
         * is equal to zero.
         *
         * @param i the value whose lowest one bit is to be computed
         * @return a <b> glong</b> value with a single one-bit, in the position
         *     of the lowest-order one-bit in the specified value, or zero if
         *     the specified value is itself equal to zero.
         *
         */
        static CORE_FAST glong lowestOneBit(glong i) {
            // HD, Section 2-1
            return i & -i;
        }

        /**
         * Returns the number of zero bits preceding the highest-order
         * ("leftmost") one-bit in the two's complement binary representation
         * of the specified glong value.  Returns 64 if the
         * specified value has no one-bits in its two's complement representation,
         * in other words if it is equal to zero.
         *
         * @param l
         *        The value whose number of leading zeros is to be computed
         */
        static gint leadingZeros(glong l);

        /**
         * Returns the number of zero bits following the lowest-order ("rightmost")
         * one-bit in the two's complement binary representation of the specified
         * glong value.  Returns 64 if the specified value has no one-bits in its two's
         * complement representation, in other words if it is equal to zero.
         *
         * @param l
         *        The value whose number of trailing zeros is to be computed
         */
        static gint trailingZeros(glong l);

        /**
         * Returns the number of one-bits in the two's complement binary
         * representation of the specified glong value.  This function is
         * sometimes referred to as the <i>population count</i>.
         *
         * @param l
         *        The value whose bits are to be counted
         */
        static gint bitCount(glong l);

        /**
         * Returns the value obtained by rotating the two's complement binary
         * representation of the specified glong value left by the
         * specified number of bits.  (Bits shifted out of the left hand, or
         * high-order, side reenter on the right, or low-order.)
         *
         * <p>Note that left rotation with a negative distance is equivalent to
         * right rotation: rotateLeft(val, -distance) == rotateRight(val, distance).
         * Note also that rotation by any multiple of 64 is a no-op, so all but the
         * last five bits of the rotation distance can be ignored, even if the distance
         * is negative: rotateLeft(val, distance) == rotateLeft(val, distance & 0x1F).
         *
         * @param l
         *        The value whose bits are to be rotated left
         * @param n
         *        The number of bit positions to rotate left
         */
        static CORE_FAST glong rotateLeft(glong l, gint n) { return (l << n) | (l >> -n); }

        /**
         * Returns the value obtained by rotating the two's complement binary
         * representation of the specified glong value right by the
         * specified number of bits.  (Bits shifted out of the right hand, or
         * low-order, side reenter on the left, or high-order.)
         *
         * <p>Note that right rotation with a negative distance is equivalent to
         * left rotation: rotateRight(val, -distance) == rotateLeft(val, distance).
         * Note also that rotation by any multiple of 64 is a no-op, so all but the
         * last five bits of the rotation distance can be ignored, even if the distance
         * is negative: rotateRight(val, distance) == rotateRight(val, distance & 0x1F).
         *
         * @param l
         *        The value whose bits are to be rotated right
         * @param n
         *        The number of bit positions to rotate right
         */
        static CORE_FAST glong rotateRight(glong l, gint n) { return (l >> n) | (l << -n); }

        /**
         * Returns the value obtained by reversing the order of the bits in the
         * two's complement binary representation of the specified glong value.
         *
         * @param l
         *        The value to be reversed
         */
        static glong reverseBits(glong l);

        /**
         * Returns the value obtained by reversing the order of the bytes in the
         * two's complement representation of the specified glong value.
         *
         * @param l
         *        The value whose bytes are to be reversed
         */
        static glong reverseBytes(glong l);

        /**
         * Returns the signum function of the specified glong value.  (The
         * return value is -1 if the specified value is negative; 0 if the
         * specified value is zero; and 1 if the specified value is positive.)
         *
         * @param l
         *        The value whose signum is to be computed
         */
        static CORE_FAST gint signum(glong l) { return (gint) ((l >> 63) | (-l >> 63)); }

        /**
         * Adds two longs together as per the + operator.
         *
         * @param x
         *        The first operand
         * @param y
         *        The second operand
         */
        static CORE_FAST glong sum(glong x, glong y) { return x + y; }

        /**
         * Returns the greater of two glong values
         *
         * @param x
         *        The first operand
         * @param y
         *        The second operand
         */
        static CORE_FAST glong max(glong x, glong y) { return Math::max(x, y); }

        /**
         * Returns the smaller of two glong values
         *
         * @param x
         *        The first operand
         * @param y
         *        The second operand
         */
        static CORE_FAST glong min(glong x, glong y) { return Math::min(x, y); }

        /**
         * Return sharable copy of this Long instance
         */
        Object &clone() const override;

        CORE_FAST CORE_ENABLE_IMPLICIT_CAST(glong, value, const)

        CORE_ENABLE_IMPLICIT_CAST(glong &, value)
    };

} // core

#endif //CORE23_LONG_H
