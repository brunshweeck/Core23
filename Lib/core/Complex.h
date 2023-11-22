//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_COMPLEX_H
#define CORE23_COMPLEX_H

#include <core/Double.h>

namespace core {

    class Complex: public Comparable<Complex> {

    private:
        /**
         * The real part of Complex
         */
        gdouble rvalue;

        /**
         * The imaginary part of Complex
         */
        gdouble ivalue;

    public:
        /**
         * Initialize newly Complex object with specified real part.
         * The imaginary part is set to zero
         *
         * @param real
         *          The real part of Complex
         */
        CORE_FAST Complex(gdouble real) : rvalue(real), ivalue(0) {}

        /**
         * Initialize newly Complex object with specified literal complex
         */
        CORE_IMPLICIT Complex(GENERIC_COMPLEX);

        /**
         * Initialize newly Complex object with specified real and imaginary parts.
         *
         * @param real
         *          The real part of Complex
         * @param imag
         *          The imaginary part of Complex
         */
        CORE_FAST CORE_EXPLICIT Complex(gdouble real, gdouble imag) : rvalue(real), ivalue(imag) {}

        /**
         * Return the real part of this Complex
         */
        CORE_FAST gdouble real() const {
            return rvalue;
        }

        /**
         * Return the imaginary part of this Complex
         */
        CORE_FAST gdouble imag() const {
            return ivalue;
        }

        /**
         * Return true iff this Complex is real pure.
         * <p>
         * Note: Complex number is real pure iff imaginary part is zero
         */
        CORE_FAST gbool isReal() const {
            return ivalue == 0;
        }

        /**
         * Return true iff this Complex is imaginary pure.
         * <p>
         * Note: Complex number is imaginary pure iff real part is zero
         */
        CORE_FAST gbool isImag() const {
            return rvalue == 0;
        }

        /**
         * Return true if this Complex is NaN.
         * <p>
         * Note: Complex number is not-a-number if any complex part is not-a-number
         */
        CORE_FAST gbool isNaN() const {
            return Double::isNaN(rvalue) || Double::isNaN(ivalue);
        }

        /**
         * Return true if this Complex is infinite.
         * <p>
         * Note: Complex number is infinite if any complex part is infinite
         */
        CORE_FAST gbool isInfinite() const {
            return Double::isInfinite(rvalue) || Double::isInfinite(ivalue);
        }

        /**
         * Return true if this Complex is finite.
         * <p>
         * Note: Complex number is finite if all complex parts are finite
         */
        CORE_FAST gbool isFinite() const {
            return Double::isFinite(rvalue) && Double::isFinite(ivalue);
        }

        /**
         * Return the reverse form of this Complex
         */
        Complex reverse() const;

        /**
         * Return the conjugate of this Complex
         */
        Complex conjugate() const;

        /**
         * Return the negation of this Complex
         */
        Complex negate() const;

        /**
         * Return the sum between this Complex and another.
         *
         * @param cplex
         *          The Other complex
         */
        Complex plus(const Complex &cplex) const;

        /**
         * Return the subtraction between this Complex and another.
         *
         * @param cplex
         *          The Other complex
         */
        Complex minus(const Complex &cplex) const;

        /**
         * Return the multiplication between this Complex and another.
         *
         * @param cplex
         *          The Other complex
         */
        Complex mult(const Complex &cplex) const;

        /**
         * Return the multiplication between this Complex and another.
         *
         * @param cplex
         *          The Other complex
         */
        Complex div(const Complex &cplex) const;

        /**
         * Return the division between this Complex and another.
         *
         * @param cplex
         *          The Other complex
         */
        Complex div(gdouble d) const;

        /**
         * Return the value of this Complex as byte
         * after narrowing primitive conversion.
         *
         * @throws InternalError
         *              Iff this Complex is not real pure.
         */
        CORE_DEPRECATED gbyte byteValue() const;

        /**
         * Return the value of this Complex as short
         * after narrowing primitive conversion.
         *
         * @throws InternalError
         *              Iff this Complex is not real pure.
         */
        CORE_DEPRECATED gshort shortValue() const;

        /**
         * Return the value of this Complex as int
         * after narrowing primitive conversion.
         *
         * @throws InternalError
         *              Iff this Complex is not real pure.
         */
        CORE_DEPRECATED gint intValue() const;

        /**
         * Return the value of this Complex as long
         * after narrowing primitive conversion.
         *
         * @throws InternalError
         *              Iff this Complex is not real pure.
         */
        CORE_DEPRECATED glong longValue() const;

        /**
         * Return the value of this Complex as float
         * after narrowing primitive conversion.
         *
         * @throws InternalError
         *              Iff this Complex is not real pure.
         */
        CORE_DEPRECATED gfloat floatValue() const;

        /**
         * Return the value of this Complex as double
         * after narrowing primitive conversion.
         *
         * @throws InternalError
         *              Iff this Complex is not real pure.
         */
        CORE_DEPRECATED gdouble doubleValue() const;

        /**
         * Return true iff specified Object is Complex
         * instance that has same part values as this
         * Complex.
         *
         * @param object
         *          The Object to be compared
         */
        gbool equals(const Object &object) const override;

        /**
         * Return sharable copy of this Complex
         */
        Object &clone() const override;

        /**
         * Return String representation of this Complex.
         * <p>
         * If this Complex is not-a-number, this function return "NaN"
         */
        String toString() const override;

        /**
         * Return hash code of this Complex
         */
        gint hash() const override;

        /**
         * Return absolute value of this Complex
         */
        Complex abs() const;

        /**
         * Return the norm of this Complex
         */
        gdouble norm() const;

        /**
         * Return the argument (in radians) of this Complex
         */
        gdouble arg() const;

        /**
         * Return square root of this Complex
         */
        Complex sqrt() const;

        /**
         * Return the value of this Complex raised to the power of the specified argument
         */
        Complex pow(gdouble d) const;

        /**
         * Construct new Complex with polar form.
         *
         * @param norm
         *          The norm of Complex (absolute)
         * @param arg
         *          The argument (in radians) of Complex
         */
        static Complex ofPolar(gdouble norm, gdouble arg);

        /**
         * Return the Complex value representing the specified
         * value.
         */
        static Complex valueOf(gdouble d);

        /**
         * Return Complex representing by specified String
         */
        static Complex valueOf(const String &str);

        gint compareTo(const Complex &o) const override;
    };

} // core

#endif //CORE23_COMPLEX_H
