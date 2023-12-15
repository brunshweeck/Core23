//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_BOOLEAN_H
#define CORE23_BOOLEAN_H

#include <core/Comparable.h>

namespace core {

    /**
     * The Boolean class wrap the value of native type
     * (generic) bool in an object.
     *
     * <p>
     * This class provide many method for converting a gbool
     * to String and String to gbool, etc...
     *
     * @author
     *      Brunshweeck Tazeussong
     */
    class Boolean CORE_FINAL  : public Object, public Comparable<Boolean> {
    private:
        /**
         * The value of the Boolean
         */
        gbool value = {};

    public:

        /**
         * The Boolean object corresponding to the native
         * value 'true'
         */
        static const Boolean TRUE;

        /**
         * The Boolean object corresponding to the native
         * value 'false'
         */
        static const Boolean FALSE;

        /**
         * Construct the Boolean object representing the value
         * argument.
         *
         * @param value
         *         The value of the Boolean
         */
        CORE_FAST Boolean() = default;

        /**
         * Construct the Boolean object representing the value
         * argument.
         *
         * @param value
         *         The value of the Boolean
         */
        CORE_FAST Boolean(gbool value) : value(value) {}

        /**
         * Parses the string argument as a boolean.  The boolean
         * returned represents the value true if the string argument
         * is equal, ignoring case, to the string "true". Otherwise,
         * a false value is returned.
         *
         * <p>
         * Example: Boolean.parseBoolean("True") returns true.<br>
         * Example: Boolean.parseBoolean("yes") returns false.
         *
         * @param str
         *        The String containing the boolean representation to be parsed
         */
        static gbool parseBoolean(const String &str);

        /**
         * Return the value of this Boolean object as gbool
         *
         */
        CORE_FAST gbool booleanValue() const { return value; }

        /**
         * Return the Boolean instance representing by specified
         * value.
         *
         * @param b The boolean value
         */
        static Boolean valueOf(gbool b);

        /**
         * Return the Boolean instance representing by string
         *
         * @param str
         *        The String representation
         */
        static Boolean valueOf(const String &str);

        /**
         * Return a String object representing the specified
         * boolean.
         *
         * @param b
         *        The boolean to be converted
         */
        static String toString(gbool b);

        /**
         * Return a String representation of this Boolean's
         * value.
         */
        String toString() const override;

        /**
         * Return the hash code of this Boolean object
         */
        gint hash() const override;

        /**
         * Return the hash code of specified boolean value
         *
         * @param b
         *        The value to hash
         */
        static CORE_FAST gint hash(gbool b) { return b ? ~0 : 0; }

        /**
         * Return true if and only if the object argument
         * is Boolean object that represent the same value
         * as this object.
         *
         * @param object
         *          The object to be compared with
         */
        gbool equals(const Object &object) const override;

        /**
         * Compares this Boolean instance with another.
         *
         * @param other
         *          The Boolean instance to be compared
         */
        gint compareTo(const Boolean &other) const override;

        /**
         * Compares two boolean values.
         *
         * @param x
         *        The first boolean to compare
         * @param y
         *        The second boolean to compare
         */
        static CORE_FAST gint compare(gbool x, gbool y) { return (x == y) ? 0 : (x ? 1 : -1); }

        /**
         * Return the result of applying logical AND operator to the
         * specified boolean operands.
         *
         * @param x
         *        The first operand
         * @param y
         *        The second operand
         */
        static CORE_FAST gbool AND(gbool x, gbool y) { return x && y; }

        /**
         * Return the result of applying logical OR operator to the
         * specified boolean operands.
         *
         * @param x
         *        The first operand
         * @param y
         *        The second operand
         */
        static CORE_FAST gbool OR(gbool x, gbool y) { return x || y; }

        /**
         * Return the result of applying logical XOR operator to the
         * specified boolean operands.
         *
         * @param x
         *        The first operand
         * @param y
         *        The second operand
         */
        static CORE_FAST gbool XOR(gbool x, gbool y) { return x ^ y; }

        /**
         * Return sharable copy of this Boolean instance
         *
         * @throws MemoryError
         *          If memory allocating failed
         */
        Object &clone() const override;

        CORE_FAST CORE_ENABLE_IMPLICIT_CAST(gbool, value, const);

        CORE_ENABLE_IMPLICIT_CAST(gbool &, value);

    };

} // core

#endif //CORE23_BOOLEAN_H
