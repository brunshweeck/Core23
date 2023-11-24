//
// Created by Brunshweeck on 12/09/2023.
//

#include "Math.h"
#include "Integer.h"
#include "Long.h"
#include "Float.h"
#include "Double.h"
#include "ArithmeticException.h"
#include "AssertionError.h"
#include <math.h>

namespace core {
    gdouble Math::sin(gdouble a) {
        return (gdouble) ::sinl(a);
    }

    gdouble Math::cos(gdouble a) {
        return (gdouble) ::cosl(a);
    }

    gdouble Math::tan(gdouble a) {
        return (gdouble) ::tanl(a);
    }

    gdouble Math::asin(gdouble a) {
        return (gdouble) ::asinl(a);
    }

    gdouble Math::acos(gdouble a) {
        return (gdouble) ::acosl(a);
    }

    gdouble Math::atan(gdouble a) {
        return (gdouble) ::atanl(a);
    }

    constexpr gdouble Math::toRadians(gdouble deg) {
        return deg * DEGREES_TO_RADIANS;
    }

    gdouble Math::toDegrees(gdouble rad) {
        return rad * RADIANS_TO_DEGREES;
    }

    gdouble Math::exp(gdouble a) {
        return (gdouble) ::expl(a);
    }

    gdouble Math::log(gdouble a) {
        return (gdouble) ::logl(a);
    }

    gdouble Math::log10(gdouble a) {
        return (gdouble) ::log10l(a);
    }

    const gdouble Math::INF = INFINITY;

    const gdouble Math::NaN = NAN;

    gdouble Math::sqrt(gdouble a) {
        return (gdouble) ::sqrtl(a);
    }

    gdouble Math::cbrt(gdouble a) {
        return (gdouble) ::cbrtl(a);
    }

    namespace {
        CORE_FAST gdouble powerOfTenD(gint n) {
            return n < 0 ? 1 / powerOfTenD(-n) :
                   n >= 100 ? 1e+100 * powerOfTenD(n - 100) :
                   n >= 50 ? 1e10 * powerOfTenD(n - 50) :
                   n >= 10 ? 1e10 * powerOfTenD(n - 10) :
                   n >= 5 ? 1e10 * powerOfTenD(n - 5) :
                   n >= 1 ? 1e+001 * powerOfTenD(n - 1) :
                   1;
        }
    }

    gdouble Math::fmod(gdouble f1, gdouble f2) {
        gdouble q = f1 / f2;
        if ((gdouble) Long::MIN_VALUE <= q && q <= (gdouble) Long::MAX_VALUE) {
            glong ql = (glong) q;
            return f1 - f2 * (gdouble) ql;
        }
        gint e = exponent(q);
        // or 2^e = 10^n => n = e/3
        gdouble pow10 = powerOfTenD(e / 3);
        return q >= 0 ? fmod(f1 - (pow10 * f2), f2) : q < 0 ? fmod(f1 + (pow10 * f2), f2) : Double::NaN;
    }

    gdouble Math::ceil(gdouble a) {
        if (!Double::isFinite(a) || a == 0.0)
            return a;
        if (a < 0 && a > -1)
            return -0.0;
        if (a < 0)
            return -Math::floor(-a);
        if ((gdouble) Long::MIN_VALUE <= a && a <= (gdouble) Long::MAX_VALUE) {
            glong al = (glong) a;
            if (a > 0 && a > (gdouble) al)
                return (gdouble) al + 1.0;
            return (gdouble) al;
        }
        gint e = exponent(a);
        // or 2^e = 10^n => n = e/3
        gdouble pow10 = powerOfTenD(e / 3);
        while (pow10 > a)
            pow10 /= 10;
        return pow10 + ceil(a - pow10);
    }

    gdouble Math::floor(gdouble a) {
        if (!Double::isFinite(a) || a == 0.0)
            return a;
        if (a < 0 && a > -1)
            return -0.0;
        if (a < 0)
            return -Math::ceil(-a);
        if ((gdouble) Long::MIN_VALUE <= a && a <= (gdouble) Long::MAX_VALUE) {
            glong al = (glong) a;
            if (a < 0 && a < (gdouble) al)
                return (gdouble) al - 1.0;
            return (gdouble) al;
        }
        gint e = exponent(a);
        // or 2^e = 10^n => n = e/3
        gdouble pow10 = powerOfTenD(e / 3);
        return copySign(pow10, a) + floor(a - ((a >= 0) ? pow10 : -pow10));
    }

    gdouble Math::rint(gdouble a) {
        if (!Double::isFinite(a) || a == 0.0)
            return a;
        if (a < 0 && a > -1)
            return -0.0;
        if ((gdouble) Long::MIN_VALUE <= a && a <= (gdouble) Long::MAX_VALUE) {
            glong al = (glong) a;
            return (gdouble) al;
        }
        gint e = exponent(a);
        // or 2^e = 10^n => n = e/3
        gdouble pow10 = powerOfTenD(e / 3);
        return copySign(pow10, a) + rint(a - ((a >= 0) ? pow10 : -pow10));
    }

    gdouble Math::atan2(gdouble y, gdouble x) {
        return (gdouble) ::atan2l(y, x);
    }

    gdouble Math::pow(gdouble a, gdouble b) {
        return (gdouble) ::powl(a, b);
    }

    gint Math::round(gfloat a) {
        gint intBits = Float::toIntBits(a);
        gint biasedExp = (intBits & Float::EXPONENT_BIT_MASK) >> (Float::SIGNIFICAND_WIDTH - 1);
        gint shift = (Float::SIGNIFICAND_WIDTH - 2 + Float::EXPONENT_BIAS) - biasedExp;
        if ((shift & -32) == 0) { // shift >= 0 && shift < 32
            // 'a' is a finite number such that pow(2,-32) <= ulp(a) < 1
            gint r = ((intBits & Float::SIGNIFICAND_BIT_MASK) | (Float::SIGNIFICAND_BIT_MASK + 1));
            if (intBits < 0) {
                r = -r;
            }
            // In the comments below each Java expression evaluates to the value
            // the corresponding mathematical expression:
            // (r) evaluates to a / ulp(a)
            // (r >> shift) evaluates to floor(a * 2)
            // ((r >> shift) + 1) evaluates to floor((a + 1/2) * 2)
            // (((r >> shift) + 1) >> 1) evaluates to floor(a + 1/2)
            return ((r >> shift) + 1) >> 1;
        } else {
            // is either
            // - a finite number with abs(a) < exp(2,FloatConsts.SIGNIFICAND_WIDTH-32) < 1/2
            // - a finite number with ulp(a) >= 1 and hence 'a' is a mathematical integer
            // - an infinity or NaN
            return (gint) a;
        }
    }

    glong Math::round(gdouble a) {
        glong longBits = Double::toLongBits(a);
        glong biasedExp = (longBits & Double::EXPONENT_BIT_MASK) >> (Double::SIGNIFICAND_WIDTH - 1);
        glong shift = (Double::SIGNIFICAND_WIDTH - 2 + Double::EXPONENT_BIAS) - biasedExp;
        if ((shift & -64) == 0) { // shift >= 0 && shift < 64
            // 'a' is a finite number such that pow(2,-64) <= ulp(a) < 1
            glong r = ((longBits & Double::SIGNIFICAND_BIT_MASK) | (Double::SIGNIFICAND_BIT_MASK + 1));
            if (longBits < 0) {
                r = -r;
            }
            // In the comments below each Java expression evaluates to the value
            // the corresponding mathematical expression:
            // (r) evaluates to a / ulp(a)
            // (r >> shift) evaluates to floor(a * 2)
            // ((r >> shift) + 1) evaluates to floor((a + 1/2) * 2)
            // (((r >> shift) + 1) >> 1) evaluates to floor(a + 1/2)
            return ((r >> shift) + 1) >> 1;
        } else {
            // 'a' is either
            // - a finite number with abs(a) < exp(2,DoubleConsts.SIGNIFICAND_WIDTH-64) < 1/2
            // - a finite number with ulp(a) >= 1 and hence 'a' is a mathematical integer
            // - an infinity or NaN
            return (glong) a;
        }
    }

    gdouble Math::random() {
        srand(Long::hash((glong) __TIMESTAMP__));
        return Float::fromIntBits(::rand());
    }

    gint Math::addExact(gint x, gint y) {
        gint r = x + y;
        // HD 2-12 Overflow iff both arguments have the opposite sign of the result
        if (((x ^ r) & (y ^ r)) < 0) {
            ArithmeticException("Integer overflow").throws(__trace("core.Math"));
        }
        return r;
    }

    glong Math::addExact(glong x, glong y) {
        glong r = x + y;
        // HD 2-12 Overflow iff both arguments have the opposite sign of the result
        if (((x ^ r) & (y ^ r)) < 0) {
            ArithmeticException("Long overflow").throws(__trace("core.Math"));
        }
        return r;
    }

    gint Math::subtractExact(gint x, gint y) {
        gint r = x - y;
        // HD 2-12 Overflow iff the arguments have different signs and
        // the sign of the result is different from the sign of x
        if (((x ^ y) & (x ^ r)) < 0) {
            ArithmeticException("Integer overflow").throws(__trace("core.Math"));
        }
        return r;
    }

    glong Math::subtractExact(glong x, glong y) {
        glong r = x - y;
        // HD 2-12 Overflow iff the arguments have different signs and
        // the sign of the result is different from the sign of x
        if (((x ^ y) & (x ^ r)) < 0) {
            ArithmeticException("Long overflow").throws(__trace("core.Math"));
        }
        return r;
    }

    gint Math::multiplyExact(gint x, gint y) {
        glong r = (glong) x * (glong) y;
        if ((gint) r != r) {
            ArithmeticException("Integer overflow").throws(__trace("core.Math"));
        }
        return (gint) r;
    }

    glong Math::multiplyExact(glong x, gint y) {
        return multiplyExact(x, (glong) y);
    }

    glong Math::multiplyExact(glong x, glong y) {
        glong r = x * y;
        glong ax = Math::abs(x);
        glong ay = Math::abs(y);
        if (((ax | ay) >> 31 != 0)) {
            // Some bits greater than 2^31 that might cause overflow
            // Check the result using the divide operator
            // and check for the special case of Long.MIN_VALUE * -1
            if (((y != 0) && (r / y != x)) || (x == Long::MIN_VALUE && y == -1)) {
                ArithmeticException("Long overflow").throws(__trace("core.Math"));
            }
        }
        return r;
    }

    gint Math::divideExact(gint x, gint y) {
        gint q = x / y;
        if ((x & y & q) >= 0) {
            return q;
        }
        ArithmeticException("Integer overflow").throws(__trace("core.Math"));
    }

    glong Math::divideExact(glong x, glong y) {
        glong q = x / y;
        if ((x & y & q) >= 0) {
            return q;
        }
        ArithmeticException("Long overflow").throws(__trace("core.Math"));
    }

    gint Math::floorDivExact(gint x, gint y) {
        gint q = x / y;
        if ((x & y & q) >= 0) {
            // if the signs are different and modulo not zero, round down
            if ((x ^ y) < 0 && (q * y != x)) {
                return q - 1;
            }
            return q;
        }
        ArithmeticException("Integer overflow").throws(__trace("core.Math"));
    }

    glong Math::floorDivExact(glong x, glong y) {
        glong q = x / y;
        if ((x & y & q) >= 0) {
            // if the signs are different and modulo not zero, round down
            if ((x ^ y) < 0 && (q * y != x)) {
                return q - 1;
            }
            return q;
        }
        ArithmeticException("Long overflow").throws(__trace("core.Math"));
    }

    gint Math::ceilDivExact(gint x, gint y) {
        gint q = x / y;
        if ((x & y & q) >= 0) {
            // if the signs are the same and modulo not zero, round up
            if ((x ^ y) >= 0 && (q * y != x)) {
                return q + 1;
            }
            return q;
        }
        ArithmeticException("Integer overflow").throws(__trace("core.Math"));
    }

    glong Math::ceilDivExact(glong x, glong y) {
        glong q = x / y;
        if ((x & y & q) >= 0) {
            // if the signs are the same and modulo not zero, round up
            if ((x ^ y) >= 0 && (q * y != x)) {
                return q + 1;
            }
            return q;
        }
        ArithmeticException("Long overflow").throws(__trace("core.Math"));
    }

    gint Math::incrementExact(gint a) {
        if (a == Integer::MAX_VALUE) {
            ArithmeticException("Integer overflow").throws(__trace("core.Math"));
        }
        return a + 1;
    }

    glong Math::incrementExact(glong a) {
        if (a == Long::MAX_VALUE) {
            ArithmeticException("Long overflow").throws(__trace("core.Math"));
        }
        return a + 1L;
    }

    gint Math::decrementExact(gint a) {
        if (a == Integer::MIN_VALUE) {
            ArithmeticException("Integer overflow").throws(__trace("core.Math"));
        }
        return a - 1;
    }

    glong Math::decrementExact(glong a) {
        if (a == Long::MIN_VALUE) {
            ArithmeticException("Long overflow").throws(__trace("core.Math"));
        }
        return a - 1L;
    }

    gint Math::negateExact(gint a) {
        if (a == Integer::MIN_VALUE) {
            ArithmeticException("Integer overflow").throws(__trace("core.Math"));
        }
        return -a;
    }

    glong Math::negateExact(glong a) {
        if (a == Long::MIN_VALUE) {
            ArithmeticException("Long overflow").throws(__trace("core.Math"));
        }
        return -a;
    }

    gint Math::toIntExact(glong value) {
        if ((gint) value != value) {
            ArithmeticException("Integer overflow").throws(__trace("core.Math"));
        }
        return (gint) value;
    }

    glong Math::multiplyFull(gint x, gint y) {
        return (glong) x * (glong) y;
    }

    glong Math::multiplyHigh(glong x, glong y) {
        // Use technique from section 8-2 of Henry S. Warren, Jr.,
        // Hacker's Delight (2nd ed.) (Addison Wesley, 2013), 173-174.
        glong x1 = x >> 32;
        glong x2 = x & 0xFFFFFFFFL;
        glong y1 = y >> 32;
        glong y2 = y & 0xFFFFFFFFL;

        glong z2 = x2 * y2;
        glong t = x1 * y2 + (z2 >> 32);
        glong z1 = t & 0xFFFFFFFFL;
        glong z0 = t >> 32;
        z1 += x2 * y1;

        return x1 * y1 + z0 + (z1 >> 32);
    }

    glong Math::unsignedMultiplyHigh(glong x, glong y) {
        // Compute via multiplyHigh() to leverage the intrinsic
        glong result = Math::multiplyHigh(x, y);
        result += (y & (x >> 63)); // equivalent to `if (x < 0) result += y;`
        result += (x & (y >> 63)); // equivalent to `if (y < 0) result += x;`
        return result;
    }

    gint Math::floorDiv(gint x, gint y) {
        gint q = x / y;
        // if the signs are different and modulo not zero, round down
        if ((x ^ y) < 0 && (q * y != x)) {
            return q - 1;
        }
        return q;
    }

    glong Math::floorDiv(glong x, gint y) {
        return floorDiv(x, (glong) y);
    }

    glong Math::floorDiv(glong x, glong y) {
        glong q = x / y;
        // if the signs are different and modulo not zero, round down
        if ((x ^ y) < 0 && (q * y != x)) {
            return q - 1;
        }
        return q;
    }

    gint Math::floorMod(gint x, gint y) {
        gint r = x % y;
        // if the signs are different and modulo not zero, adjust result
        if ((x ^ y) < 0 && r != 0) {
            return r + y;
        }
        return r;
    }

    gint Math::floorMod(glong x, gint y) {
        // Result cannot overflow the range of int.
        return (gint) floorMod(x, (glong) y);
    }

    glong Math::floorMod(glong x, glong y) {
        glong r = x % y;
        // if the signs are different and modulo not zero, adjust result
        if ((x ^ y) < 0 && r != 0) {
            return r + y;
        }
        return r;
    }

    gint Math::ceilDiv(gint x, gint y) {
        gint q = x / y;
        // if the signs are the same and modulo not zero, round up
        if ((x ^ y) >= 0 && (q * y != x)) {
            return q + 1;
        }
        return q;
    }

    glong Math::ceilDiv(glong x, gint y) {
        return ceilDiv(x, (glong) y);
    }

    glong Math::ceilDiv(glong x, glong y) {
        glong q = x / y;
        // if the signs are the same and modulo not zero, round up
        if ((x ^ y) >= 0 && (q * y != x)) {
            return q + 1;
        }
        return q;
    }

    gint Math::ceilMod(gint x, gint y) {
        gint r = x % y;
        // if the signs are the same and modulo not zero, adjust result
        if ((x ^ y) >= 0 && r != 0) {
            return r - y;
        }
        return r;
    }

    gint Math::ceilMod(glong x, gint y) {
        // Result cannot overflow the range of int.
        return (gint) ceilMod(x, (glong) y);
    }

    glong Math::ceilMod(glong x, glong y) {
        glong r = x % y;
        // if the signs are the same and modulo not zero, adjust result
        if ((x ^ y) >= 0 && r != 0) {
            return r - y;
        }
        return r;
    }

    gint Math::absExact(gint a) {
        if (a == Integer::MIN_VALUE)
            ArithmeticException("Overflow to represent absolute value of Integer::MIN_VALUE")
                    .throws(__trace("core.Math"));
        else
            return abs(a);
    }

    glong Math::absExact(glong a) {
        if (a == Long::MIN_VALUE)
            ArithmeticException("Overflow to represent absolute value of Long::MIN_VALUE")
                    .throws(__trace("core.Math"));
        else
            return abs(a);
    }

    gfloat Math::abs(gfloat a) {
        // Convert to bit field form, zero the sign bit, and convert back
        return Float::fromIntBits(Float::toIntBits(a) & Float::MAGNITUDE_BIT_MASK);
    }

    gdouble Math::abs(gdouble a) {
        // Convert to bit field form, zero the sign bit, and convert back
        return Double::fromLongBits(Double::toLongBits(a) & Double::MAGNITUDE_BIT_MASK);
    }

    gfloat Math::max(gfloat a, gfloat b) {
        if (a != a)
            return a;   // is NaN
        if ((a == 0.0f) &&
            (b == 0.0f) &&
            (Float::toIntBits(a) == Float::toIntBits(-0.0f))) {
            // Raw conversion ok since NaN can't map to -0.0.
            return b;
        }
        return (a >= b) ? a : b;
    }

    gdouble Math::max(gdouble a, gdouble b) {
        if (a != a)
            return a;   // is NaN
        if ((a == 0.0) &&
            (b == 0.0) &&
            (Double::toLongBits(a) == Double::toLongBits(-0.0))) {
            // Raw conversion ok since NaN can't map to -0.0.
            return b;
        }
        return (a >= b) ? a : b;
    }

    gfloat Math::min(gfloat a, gfloat b) {
        if (a != a)
            return a;   // is NaN
        if ((a == 0.0f) && (b == 0.0f) && (Float::toIntBits(b) == Float::toIntBits(-0.0f))) {
            // Raw conversion ok since NaN can't map to -0.0.
            return b;
        }
        return (a <= b) ? a : b;
    }

    gdouble Math::min(gdouble a, gdouble b) {
        if (a != a)
            return a;   //is NaN
        if ((a == 0.0) && (b == 0.0) && (Double::toLongBits(b) == Double::toLongBits(-0.0))) {
            // Raw conversion ok since NaN can't map to -0.0.
            return b;
        }
        return (a <= b) ? a : b;
    }

    gdouble Math::ulp(gdouble d) {
        gint exp = exponent(d);
        switch (exp) {
            case Double::MAX_EXPONENT + 1:
                return Math::abs(d);      // NaN or infinity
            case Double::MIN_EXPONENT - 1:
                return Double::MIN_VALUE; // zero or subnormal
            default : {
                CORE_ASSERT(exp <= Double::MAX_EXPONENT && exp >= Double::MIN_EXPONENT, "core.Math");

                // ulp(x) is usually 2^(SIGNIFICAND_WIDTH-1)*(2^i*logb(x))
                exp = exp - (Double::SIGNIFICAND_WIDTH - 1);
                if (exp >= Double::MIN_EXPONENT) {
                    return powerOfTwoD(exp);
                } else {
                    // return a subnormal result; left shift integer
                    // representation of Double.MIN_VALUE appropriate
                    // number of positions
                    return Double::fromLongBits(1L << (exp - (Double::MIN_EXPONENT - (Double::SIGNIFICAND_WIDTH - 1))));
                }
            }
        }
    }

    gfloat Math::ulp(gfloat f) {
        gint exp = exponent(f);
        switch (exp) {
            case Float::MAX_EXPONENT + 1:
                return Math::abs(f);     // NaN or infinity
            case Float::MIN_EXPONENT - 1:
                return Float::MIN_VALUE; // zero or subnormal
            default : {
                CORE_ASSERT(exp <= Float::MAX_EXPONENT && exp >= Float::MIN_EXPONENT, "core.Math");

                // ulp(x) is usually 2^(SIGNIFICAND_WIDTH-1)*(2^(i*logb(x)))
                exp = exp - (Float::SIGNIFICAND_WIDTH - 1);
                if (exp >= Float::MIN_EXPONENT) {
                    return powerOfTwoF(exp);
                } else {
                    // return a subnormal result; left shift integer
                    // representation of FloatConsts.MIN_VALUE appropriate
                    // number of positions
                    return Float::fromIntBits(1 << (exp - (Float::MIN_EXPONENT - (Float::SIGNIFICAND_WIDTH - 1))));
                }
            }
        }
    }

    gdouble Math::signum(gdouble d) {
        return (d == 0.0 || Double::isNaN(d)) ? d : copySign(1.0, d);
    }

    gfloat Math::signum(gfloat f) {
        return (f == 0.0f || Float::isNaN(f)) ? f : copySign(1.0f, f);
    }

    gdouble Math::sinh(gdouble a) {
        return (gdouble) ::sinhl(a);
    }

    gdouble Math::cosh(gdouble a) {
        return (gdouble) ::coshl(a);
    }

    gdouble Math::tanh(gdouble x) {
        return (gdouble) ::tanhl(x);
    }

    gdouble Math::asinh(gdouble x) {
        return (gdouble) ::asinhl(x);
    }

    gdouble Math::acosh(gdouble x) {
        return (gdouble) ::acoshl(x);
    }

    gdouble Math::atanh(gdouble x) {
        return (gdouble) ::atanhl(x);
    }

    gdouble Math::hypot(gdouble x, gdouble y) {
        return (gdouble) ::hypotl(x, y);
    }

    gdouble Math::copySign(gdouble magnitude, gdouble sign) {
        return Double::fromLongBits((Double::toLongBits(sign) & (Double::SIGN_BIT_MASK)) |
                                    (Double::toLongBits(magnitude) &
                                     (Double::EXPONENT_BIT_MASK | Double::SIGNIFICAND_BIT_MASK)));
    }

    gfloat Math::copySign(gfloat magnitude, gfloat sign) {
        return Float::fromIntBits((Float::toIntBits(sign) & (Float::SIGN_BIT_MASK)) |
                                  (Float::toIntBits(magnitude) &
                                   (Float::EXPONENT_BIT_MASK | Float::SIGNIFICAND_BIT_MASK)));
    }

    gint Math::exponent(gfloat f) {
        /*
         * Bitwise convert f to integer, mask out exponent bits, shift
         * to the right and then subtract out float's bias adjust to
         * get true exponent value
         */
        return ((Float::toIntBits(f) & Float::EXPONENT_BIT_MASK) >> (Float::SIGNIFICAND_WIDTH - 1)) -
               Float::EXPONENT_BIAS;
    }

    gint Math::exponent(gdouble d) {
        /*
         * Bitwise convert d to long, mask out exponent bits, shift
         * to the right and then subtract out double's bias adjust to
         * get true exponent value.
         */
        return (gint) (((Double::toLongBits(d) & Double::EXPONENT_BIT_MASK) >> (Double::SIGNIFICAND_WIDTH - 1)) -
                       Double::EXPONENT_BIAS);
    }

    gdouble Math::nextUp(gdouble d) {
        // Use a single conditional and handle the likely cases first.
        if (d < Double::POSITIVE_INFINITY) {
            // Add +0.0 to get rid of a -0.0 (+0.0 + -0.0 => +0.0).
            glong transducer = Double::toLongBits(d + 0.0);
            return Double::fromLongBits(transducer + ((transducer >= 0L) ? 1L : -1L));
        } else { // d is NaN or +Infinity
            return d;
        }
    }

    gfloat Math::nextUp(gfloat f) {
        // Use a single conditional and handle the likely cases first.
        if (f < Float::POSITIVE_INFINITY) {
            // Add +0.0 to get rid of a -0.0 (+0.0 + -0.0 => +0.0).
            gint transducer = Float::toIntBits(f + 0.0F);
            return Float::fromIntBits(transducer + ((transducer >= 0) ? 1 : -1));
        } else { // f is NaN or +Infinity
            return f;
        }
    }

    gdouble Math::nextDown(gdouble d) {
        if (Double::isNaN(d) || d == Double::NEGATIVE_INFINITY)
            return d;
        else {
            if (d == 0.0)
                return -Double::MIN_VALUE;
            else
                return Double::fromLongBits(Double::toLongBits(d) + ((d > 0.0) ? -1L : +1L));
        }
    }

    gfloat Math::nextDown(gfloat f) {
        if (Float::isNaN(f) || f == Float::NEGATIVE_INFINITY)
            return f;
        else {
            if (f == 0.0f)
                return -Float::MIN_VALUE;
            else
                return Float::fromIntBits(Float::toIntBits(f) + ((f > 0.0f) ? -1 : +1));
        }
    }

    gdouble Math::scaleB(gdouble d, gint scaleFactor) {
        /*
         * When scaling up, it does not matter what order the
         * multiply-store operations are done; the result will be
         * finite or overflow regardless of the operation ordering.
         * However, to get the correct result when scaling down, a
         * particular ordering must be used.
         *
         * When scaling down, the multiply-store operations are
         * sequenced so that it is not possible for two consecutive
         * multiply-stores to return subnormal results.  If one
         * multiply-store result is subnormal, the next multiply will
         * round it away to zero.  This is done by first multiplying
         * by 2 ^ (scaleFactor % n) and then multiplying several
         * times by 2^n as needed where n is the exponent of number
         * that is a convenient power of two.  In this way, at most one
         * real rounding error occurs.
         */

        // magnitude of a power of two so large that scaling a finite
        // nonzero value by it would be guaranteed to over or
        // underflow; due to rounding, scaling down takes an
        // additional power of two which is reflected here
        CORE_FAST
        gint MAX_SCALE = Double::MAX_EXPONENT + -Double::MIN_EXPONENT + Double::SIGNIFICAND_WIDTH + 1;
        gint exp_adjust = 0;
        gint scale_increment = 0;
        gdouble exp_delta = Double::NaN;

        // Make sure scaling factor is in a reasonable range

        if (scaleFactor < 0) {
            scaleFactor = Math::max(scaleFactor, -MAX_SCALE);
            scale_increment = -512;
            exp_delta = powerOfTwoD(-512);
        } else {
            scaleFactor = Math::min(scaleFactor, MAX_SCALE);
            scale_increment = 512;
            exp_delta = powerOfTwoD(512);
        }

        // Calculate (scaleFactor % +/-512), 512 = 2^9, using
        // technique from "Hacker's Delight" section 10-2.
        gint t = (scaleFactor >> (9 - 1)) >> (32 - 9);
        exp_adjust = ((scaleFactor + t) & (512 - 1)) - t;

        d *= powerOfTwoD(exp_adjust);
        scaleFactor -= exp_adjust;

        while (scaleFactor != 0) {
            d *= exp_delta;
            scaleFactor -= scale_increment;
        }
        return d;
    }

    gfloat Math::scaleB(gfloat f, gint scaleFactor) {
        // magnitude of a power of two so large that scaling a finite
        // nonzero value by it would be guaranteed to over or
        // underflow; due to rounding, scaling down takes an
        // additional power of two which is reflected here
        CORE_FAST
        gint MAX_SCALE = Float::MAX_EXPONENT + -Float::MIN_EXPONENT +
                         Float::SIGNIFICAND_WIDTH + 1;

        // Make sure scaling factor is in a reasonable range
        scaleFactor = Math::max(Math::min(scaleFactor, MAX_SCALE), -MAX_SCALE);

        /*
         * Since + MAX_SCALE for float fits well within the double
         * exponent range and + float -> double conversion is exact
         * the multiplication below will be exact. Therefore, the
         * rounding that occurs when the double product is cast to
         * float will be the correctly rounded float result.
         */
        return (gfloat) ((gdouble) f * powerOfTwoD(scaleFactor));
    }

    gdouble Math::powerOfTwoD(gint n) {
        CORE_ASSERT(n >= Double::MIN_EXPONENT && n <= Double::MAX_EXPONENT, "core.Math");
        return Double::fromLongBits((((glong) n + (glong) Double::EXPONENT_BIAS) << (Double::SIGNIFICAND_WIDTH - 1)) &
                                    Double::EXPONENT_BIT_MASK);
    }

    gfloat Math::powerOfTwoF(gint n) {
        CORE_ASSERT(n >= Float::MIN_EXPONENT && n <= Float::MAX_EXPONENT, "core.Math");
        return Float::fromIntBits(
                ((n + Float::EXPONENT_BIAS) << (Float::SIGNIFICAND_WIDTH - 1)) & Float::EXPONENT_BIT_MASK);
    }

} // core