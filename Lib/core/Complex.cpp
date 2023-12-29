//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "Complex.h"
#include "ArithmeticException.h"
#include "Math.h"

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    CORE_WARNING_PUSH
    CORE_WARNING_DISABLE_DEPRECATED

    gbyte Complex::byteValue() const {
        if (!isReal())
            ArithmeticException("Couldn't convert composed complex number to real number")
                    .throws(__trace("core.Complex"));
        return (gbyte) intValue();
    }

    gshort Complex::shortValue() const {
        if (!isReal())
            ArithmeticException("Couldn't convert composed complex number to real number")
                    .throws(__trace("core.Complex"));
        return (gshort) intValue();
    }

    gint Complex::intValue() const {
        if (!isReal())
            ArithmeticException("Couldn't convert composed complex number to real number")
                    .throws(__trace("core.Complex"));
        return (gint) real();
    }

    glong Complex::longValue() const {
        if (!isReal())
            ArithmeticException("Couldn't convert composed complex number to real number")
                    .throws(__trace("core.Complex"));
        return (glong) real();
    }

    gfloat Complex::floatValue() const {
        if (!isReal())
            ArithmeticException("Couldn't convert composed complex number to real number")
                    .throws(__trace("core.Complex"));
        return (gfloat) real();
    }

    gdouble Complex::doubleValue() const {
        if (!isReal())
            ArithmeticException("Couldn't convert composed complex number to real number")
                    .throws(__trace("core.Complex"));
        return real();
    }

    gbool Complex::equals(const Object &object) const {
        if (!Class<Complex>::hasInstance(object))
            return false;
        const Complex &z = (Complex &) object;
        return (rvalue == z.rvalue || Double::isNaN(rvalue) && Double::isNaN(z.rvalue)) &&
               (ivalue == z.ivalue || Double::isNaN(ivalue) && Double::isNaN(ivalue));
    }

    Object &Complex::clone() const {
        return U::createInstance<Complex>(*this);
    }

    String Complex::toString() const {
        if (ivalue == 0)
            return Double::toString(rvalue);
        if (ivalue == 1)
            return Double::toString(rvalue) + "+i";
        if (ivalue == -1)
            return Double::toString(rvalue) + "-i";
        if (Double::isInfinite(ivalue))
            return Double::toString(rvalue) + (ivalue > 0 ? "+(" : "-(") + Double::toString(Math::INF) + ")i";
        if (Double::isNaN(ivalue))
            return Double::toString(rvalue) + (ivalue > 0 ? "+(" : "-(") + Double::toString(Math::NaN) + ")i";
        return Double::toString(rvalue) + (ivalue > 0 ? "+" : "") + Double::toString(ivalue) + "i";
    }

    gint Complex::hash() const {
        return (Double::hash(rvalue) * 31 ^ 1) + Double::hash(ivalue);
    }

    Complex Complex::abs() const {
        return Complex(Math::abs(rvalue), Math::abs(ivalue));
    }

    gdouble Complex::norm() const {
        return Math::sqrt(Math::pow(rvalue, 2) + Math::pow(ivalue, 2));
    }

    gdouble Complex::arg() const {
        if (!isFinite())
            return Math::NaN;
        if (ivalue == 0 && rvalue == 0)
            return 0;
        gdouble const n = norm();
        if (Double::isNaN(n))
            return Math::NaN;
        gdouble const rad = Math::acos(rvalue / ivalue);
        if (Math::signum(rvalue) >= 0)
            return Math::signum(ivalue) > 0 ? rad : -rad;
        return Math::signum(ivalue) > 0 ? Math::PI - rad : Math::PI + rad;
    }

    Complex Complex::sqrt() const {
        /*
         * n = norm(Z)
         * theta = arg(Z)
         *
         * Z = n * e^(i * theta)
         * Z2 = sqrt(Z) = sqrt(n * e^(i * theta))
         *              = sqrt(n) * sqrt(e^(i * theta))
         *              = sqrt(n) * (e^(i * theta)) ^ (1/2)
         *              = sqrt(n) * e^(i * theta/2)
         * norm(Z2) = sqrt(n)
         * arg(Z2) = theta/2
         */
        return ofPolar(Math::sqrt(norm()), arg() / 2);
    }

    Complex Complex::pow(gdouble d) const {
        /*
         * n = norm(Z)
         * theta = arg(Z)
         *
         * Z = n * e^(i * theta)
         * Z2 = Z^a = (n * e^(i * theta))^a
         *              = n^a * (e^(i * theta))^a
         *              = n^a * e^(i * theta * a)
         * norm(Z2) = n^a
         * arg(Z2) = theta * a
         */
        return ofPolar(Math::pow(norm(), d), arg() * d);
    }

    Complex Complex::ofPolar(gdouble norm, gdouble arg) {
        return Complex(norm * Math::cos(arg), norm * Math::sin(arg));
    }

    Complex Complex::valueOf(gdouble d) {
        return Complex(d);
    }

    Complex Complex::valueOf(const String & /*str*/) {
        static String pattern = R"(^[+-]?(p{\digit}+([eE]([+-]?)p{\digit}(p{\digit}*))|0[xX]p{\xdigit}+(.p{\digit}*[pP]([+-]?)p{\digit}+)?))";
        return 0;
    }

    gint Complex::compareTo(const Complex &other) const {
        gint r = Double::compare(rvalue, other.rvalue);
        if (r != 0) return r;
        return Double::compare(ivalue, other.ivalue);
    }
}

#if defined(CORE_COMPILER_MSVC) && !defined(__clang__)
struct _C_double_complex {
    gdouble _Val[2];
};
#endif

namespace core {
    Complex::Complex(GENERIC_CPLEX cplex) : rvalue(*((gdouble *) &cplex)), ivalue(*((gdouble *) &cplex + 1)) {}

    Complex Complex::reverse() const { return Complex(imag(), real()); }

    Complex Complex::conjugate() const { return Complex(real(), -imag()); }

    Complex Complex::negate() const { return Complex(-real(), -imag()); }

    Complex Complex::plus(const Complex &z) const { return Complex(real() + z.real(), imag() + z.imag()); }

    Complex Complex::minus(const Complex &z) const { return Complex(real() - z.real(), imag() - z.imag()); }

    Complex Complex::mult(const Complex &z) const {
        return Complex(real() * z.real() - imag() * z.imag(), imag() * z.real() + real() * z.imag());
    }

    Complex Complex::div(const Complex &z) const {
        return mult(z.conjugate()).div(z.real() * z.real() + z.imag() * z.imag());
    }

    Complex Complex::div(gdouble d) const { return Complex(real() / d, imag() / d); }

    Complex operator+(const Complex &z1, const Complex &z2) { return z1.plus(z2); }

    Complex operator-(const Complex &z1, const Complex &z2) { return z1.minus(z2); }

    Complex operator*(const Complex &z1, const Complex &z2) { return z1.mult(z2); }

    Complex operator/(const Complex &z1, const Complex &z2) { return z1.div(z2); }

    Complex operator+(const Complex &z) { return z; }

    Complex operator-(const Complex &z) { return z.negate(); }

    CORE_WARNING_DISABLE_UDL

    Complex operator "" J(unsigned long long int imag) { return Complex(0, (gdouble) imag); }

    Complex operator "" J(long double imag) { return Complex(0, (gdouble) imag); }

    Complex operator "" j(unsigned long long int imag) { return Complex(0, (gdouble) imag); }

    Complex operator "" j(long double imag) { return Complex(0, (gdouble) imag); }

    CORE_WARNING_POP
} // core