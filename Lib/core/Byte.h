//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_BYTE_H
#define CORE23_BYTE_H

#include <core/Comparable.h>

namespace core {

    /**
     * The Byte class wrap the value of native type
     * (generic) byte in an object.
     *
     * <p>
     * This class provide many method for converting a gbyte
     * to String and String to gbyte, etc...
     *
     * @author
     *      Brunshweeck Tazeussong
     */
    class Byte CORE_FINAL : public Object, public Comparable<Byte> {
    private:
        /**
         * The value of the Byte
         */
        gbyte value;

    public:
        /**
         * A constant holding the maximum value a byte can have, 2<sup>7</sup> - 1
         */
        static CORE_FAST gbyte MAX_VALUE = 127;

        /**
         * A constant holding the minimum value a byte can have, -2<sup>7</sup>
         */
        static CORE_FAST gbyte MIN_VALUE = -128;

        /**
         * Construct new Byte object that represent the specified
         * byte value
         *
         * @param value
         *         The value of the Byte.
         */
        CORE_FAST Byte(gbyte value) : value(value) {}

        /**
         * Parses the string argument as a signed byte in the base
         * specified by the second argument. The characters in the
         * string must all be digits, of the specified base except
         * that the first character may be an ASCII minus sign '-'
         * (U+002D) to indicate a negative value or an ASCII plus
         * sign '+' (U+002B) to indicate a positive value.  The
         * resulting byte value is returned.
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
         *  <li> The value represented by the string is not a value of type byte
         * </ol>
         *
         * @param s
         *        The String containing the byte representation to be parsed.
         * @param base
         *         The base to be used while parsing string
         * @throws FormatException
         *          If the string not contains parsable byte
         */
        static gbyte parseByte(const String &s, gint base);

        /**
         * Parses the string argument as a signed decimal byte.
         * The characters in the string must all be decimal digits,
         * except that the first character may be an ASCII minus sign
         * '-' (U+002D) to indicate a negative value or an ASCII plus
         * sign '+' (U+002B) to indicate a positive value. The resulting
         * byte value is returned, exactly as if the argument and the base
         * 10  were given as arguments to the parseByte(String, gint) method.
         *
         * @param s
         *        The String containing the byte representation to be parsed.
         * @throws FormatException
         *          If the string not contains parsable byte
         */
        static gbyte parseByte(const String &s);

        /**
         * Returns a Byte object holding the value extracted from the specified
         * String when parsed with the base given by the second argument. The
         * first argument is interpreted as representing a signed byte in
         * the base specified by the second argument, exactly as if the
         * argument were given to the parseByte(String, gint) method. The result
         * is a Byte object that represents the byte value specified by the string.
         *
         * @param s
         *        The string to be parsed
         * @param base
         *         The base to be used in interpreting s
         * @throws FormatException
         *          If the String does not contain a parsable byte.
         */
        static Byte valueOf(const String &s, gint base);

        /**
         * Returns a Byte object holding the value given by the specified
         * String. The argument is interpreted as representing a signed
         * decimal byte, exactly as if the argument were given to the
         * parseByte(String) method. The result is a Byte object that
         * represents the byte value specified by the string.
         *
         * @param s
         *        The string to be parsed
         * @throws FormatException
         *          If the String does not contain a parsable byte.
         */
        static Byte valueOf(const String &s);

        static Byte valueOf(gbyte b);

        /**
         * Decodes a String into a Byte.
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
         * parseByte method with the indicated base (10, 16, 8 or 2).
         * This sequence of characters must represent a positive value or
         * a FormatException will be thrown.  The result is negated if first
         * character of the specified String is the minus sign.  No whitespace
         * characters are permitted in the String.
         *
         * @param s
         *        The String to decode.
         * @throws FormatException
         *          If the String does not contain a parsable byte.
         *
         * @see Byte.parseByte(String, int)
         */
        static Byte decode(const String &s);

        /**
         * Return the value of this Byte as byte
         */
        CORE_FAST gbyte byteValue() const { return value; }

        /**
         * Return the value of this Byte as short after
         * widening native conversion
         */
        CORE_FAST gshort shortValue() const { return (gshort) value; }

        /**
         * Return the value of this Byte as int after
         * widening native conversion
         */
        CORE_FAST gint intValue() const { return (gint) value; }

        /**
         * Return the value of this Byte as long after
         * widening native conversion
         */
        CORE_FAST glong longValue() const { return (glong) value; }

        /**
         * Return the value of this Byte as float after
         * widening native conversion
         */
        CORE_FAST gfloat floatValue() const { return (gfloat) value; }

        /**
         * Return the value of this Byte as double after
         * widening native conversion
         */
        CORE_FAST gdouble doubleValue() const { return (gdouble) value; }

        /**
         * Return String object representing this Byte's value.
         * The value is converted to signed decimal representation
         * and returned as a string.
         */
        String toString() const override;

        /**
         * Return the String object representing the specified byte.
         * The base is assumed to be 10.
         *
         * @param b
         *        The byte to be converted
         */
        static String toString(gbyte b);

        /**
         * Return the hash code of this Byte's value
         */
        gint hash() const override;

        /**
         * Return the hash code of specified byte value
         *
         * @param b
         *        The value to hash
         */
        static CORE_FAST gint hash(gbyte b) { return (gint) b; }

        /**
         * Return true if and only if the object argument
         * is Byte object that represent the same value
         * as this object.
         *
         * @param object
         *          The object to compared with
         */
        gbool equals(const Object &object) const override;

        /**
         * Compares two Byte objects numerically.
         *
         * @param other
         *          The Byte to be compared
         */
        gint compareTo(const Byte &other) const override;

        /**
         * Compares two byte values numerically.
         *
         * @param x
         *        The first byte to compare
         * @param y
         *         The second byte to compare
         */
        static CORE_FAST gint compare(gbyte x, gbyte y) { return x - y; }

        /**
         * Compares two byte values numerically.
         *
         * @param x
         *        The first byte to compare
         * @param y
         *         The second byte to compare
         */
        static CORE_FAST gint compareUnsigned(gbyte x, gbyte y) { return toUnsignedInt(x) - toUnsignedInt(y); }

        /**
         * Converts the argument to an int by an unsigned conversion.
         * In an unsigned conversion to an int, the high-order 24 bits of
         * the int are zero and the low-order 8 bits are equal to the bits
         * of the byte argument.
         *
         * <p>
         * Consequently, zero and positive byte values are mapped
         * to a numerically equal int value and negative byte values
         * are mapped to an int value equal to the input plus 2<sup>8</sup>.
         *
         * @param b
         *        The value to convert to an unsigned int
         */
        static CORE_FAST gint toUnsignedInt(gbyte b) { return ((gint) b) & 0xff; }

        /**
         * Converts the argument to a long by an unsigned conversion.
         * In an unsigned conversion to a long, the high-order 56 bits of
         * the int are zero and the low-order 8 bits are equal to the bits
         * of the byte argument.
         *
         * <p>
         * Consequently, zero and positive byte values are mapped
         * to a numerically equal int value and negative byte values
         * are mapped to an int value equal to the input plus 2<sup>8</sup>.
         *
         * @param b
         *        The value to convert to an unsigned int
         */
        static CORE_FAST glong toUnsignedLong(gbyte b) { return ((glong) b) & 0xffLL; }

        /**
         * Return sharable copy of this Byte instance
         */
        Object &clone() const override;

        CORE_FAST CORE_ENABLE_IMPLICIT_CAST(gbyte, value, const)

        CORE_ENABLE_IMPLICIT_CAST(gbyte &, value)
    };

} // core

#endif //CORE23_BYTE_H
