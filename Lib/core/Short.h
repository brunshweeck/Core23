//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_SHORT_H
#define CORE23_SHORT_H

#include "Comparable.h"

namespace core {

    /**
     * The Short class wrap the value of native type
     * (generic) short in an object.
     *
     * <p>
     * This class provide many method for converting a gshort
     * to String and String to gshort, etc...
     *
     * @author
     *      Brunshweeck Tazeussong
     */
    class Short CORE_FINAL : public Object, public Comparable<Short> {
    private:
        /**
         * The value of the Short
         */
        gshort value;

    public:
        /**
         * A constant holding the maximum value a short can have, 2<sup>15</sup> - 1
         */
        static CORE_FAST gshort  MAX_VALUE = 32767;

        /**
         * A constant holding the minimum value a short can have, -2<sup>15</sup>
         */
        static CORE_FAST gshort  MIN_VALUE = -32768;

        /**
         * Construct new Short object that represent the specified
         * short value
         *
         * @param value
         *         The value of the Short.
         */
        CORE_FAST Short(gshort value) : value(value) {}

        /**
         * Parses the string argument as a signed short in the base
         * specified by the second argument. The characters in the
         * string must all be digits, of the specified base except
         * that the first character may be an ASCII minus sign '-'
         * (U+002D) to indicate a negative value or an ASCII plus
         * sign '+' (U+002B) to indicate a positive value.  The
         * resulting short value is returned.
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
         *  <li> The value represented by the string is not a value of type short
         * </ol>
         *
         * @param str
         *        The String containing the short representation to be parsed.
         * @param base
         *         The base to be used while parsing string
         * @throws FormatException
         *          If the string not contains parsable short
         */
        static gshort parseShort(const String &str, gint base);

        /**
         * Parses the string argument as a signed decimal short.
         * The characters in the string must all be decimal digits,
         * except that the first character may be an ASCII minus sign
         * '-' (U+002D) to indicate a negative value or an ASCII plus
         * sign '+' (U+002B) to indicate a positive value. The resulting
         * short value is returned, exactly as if the argument and the base
         * 10  were given as arguments to the parseShort(String, gint) method.
         *
         * @param str
         *        The String containing the short representation to be parsed.
         * @throws FormatException
         *          If the string not contains parsable short
         */
        static gshort parseShort(const String &str);

        /**
         * Returns a Short object holding the value extracted from the specified
         * String when parsed with the base given by the second argument. The
         * first argument is interpreted as representing a signed short in
         * the base specified by the second argument, exactly as if the
         * argument were given to the parseShort(String, gint) method. The result
         * is a Short object that represents the short value specified by the string.
         *
         * @param str
         *        The string to be parsed
         * @param base
         *         The base to be used in interpreting str
         * @throws FormatException
         *          If the String does not contain a parsable short.
         */
        static Short valueOf(const String &str, gint base);

        /**
         * Returns a Short object holding the value given by the specified
         * String. The argument is interpreted as representing a signed
         * decimal short, exactly as if the argument were given to the
         * parseShort(String) method. The result is a Short object that
         * represents the short value specified by the string.
         *
         * @param str
         *        The string to be parsed
         * @throws FormatException
         *          If the String does not contain a parsable short.
         */
        static Short valueOf(const String &str);

        static Short valueOf(gshort i);

        /**
         * Decodes a String into a Short.
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
         * parseShort method with the indicated base (10, 16, 8 or 2).
         * This sequence of characters must represent a positive value or
         * a FormatException will be thrown.  The result is negated if first
         * character of the specified String is the minus sign.  No whitespace
         * characters are permitted in the String.
         *
         * @param str
         *        The String to decode.
         * @throws FormatException
         *          If the String does not contain a parsable short.
         *
         * @see Short.parseShort(String, gint)
         */
        static Short decode(const String &str);

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
         * Return the String object representing the specified short.
         * The base is assumed to be 10.
         *
         * @param i
         *        The short to be converted
         */
        static String toString(gshort i);

        /**
         * Return the hash code of this Short's value
         */
        gint hash() const override;

        /**
         * Return the hash code of specified short value
         *
         * @param i
         *        The value to hash
         */
        static CORE_FAST gint hash(gshort i) {
            return (gint) i;
        }

        /**
         * Return true if and only if the object argument
         * is Short object that represent the same value
         * as this object.
         *
         * @param object
         *          The object to compared with
         */
        gbool equals(const Object &object) const override;

        /**
         * Compares two Short objects numerically.
         *
         * @param other
         *          The Short to be compared
         */
        gint compareTo(const Short& other) const override;

        /**
         * Compares two short values numerically.
         *
         * @param x
         *        The first short to compare
         * @param y
         *         The second short to compare
         */
        static CORE_FAST gint compare(gshort x, gshort y) {
            return x - y;
        }

        /**
         * Compares two short values numerically.
         *
         * @param x
         *        The first short to compare
         * @param y
         *         The second short to compare
         */
        static CORE_FAST gint compareUnsigned(gshort x, gshort y) {
            return toUnsignedInt(x) - toUnsignedInt(y);
        }

        /**
         * Returns the value obtained by reversing the order of the bytes in the
         * two's complement representation of the specified short value.
         *
         * @param i
         *        The value whose bytes are to be reversed
         */
        static CORE_FAST gshort reverseBytes(gshort i) {
            return (gshort) (((i & 0xFF00) >> 8 ) | (i << 8));
        }

        /**
         * Converts the argument to an gint by an unsigned conversion.
         * In an unsigned conversion to an gint, the high-order 16 bits of
         * the gint are zero and the low-order 8 bits are equal to the bits
         * of the short argument.
         *
         * <p>
         * Consequently, zero and positive short values are mapped
         * to a numerically equal gint value and negative short values
         * are mapped to an gint value equal to the input plus 2<sup>16</sup>.
         *
         * @param i
         *        The value to convert to an unsigned gint
         */
        static CORE_FAST gint toUnsignedInt(gshort i) {
            return ((gint) i) & 0xffff;
        }

        /**
         * Converts the argument to a glong by an unsigned conversion.
         * In an unsigned conversion to a glong, the high-order 48 bits of
         * the gint are zero and the low-order 8 bits are equal to the bits
         * of the short argument.
         *
         * <p>
         * Consequently, zero and positive short values are mapped
         * to a numerically equal gint value and negative short values
         * are mapped to an gint value equal to the input plus 2<sup>16</sup>.
         *
         * @param i
         *        The value to convert to an unsigned gint
         */
        static CORE_FAST glong toUnsignedLong(gshort i) {
            return ((glong) i) & 0xffffLL;
        }

        /**
         * Return sharable copy of this Short instance
         */
        Object &clone() const override;

        CORE_FAST CORE_ENABLE_IMPLICIT_CAST(gshort, value, const)

        CORE_ENABLE_IMPLICIT_CAST(gshort &, value)
    };

} // core

#endif //CORE23_SHORT_H
