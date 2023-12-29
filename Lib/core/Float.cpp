//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "Float.h"
#include "String.h"
#include "NumberFormatException.h"
#include "Math.h"
#include "Integer.h"
#include "AssertionError.h"

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    gfloat Float::parseFloat(const String &str) {
        if (str.isEmpty())
            NumberFormatException("Invalid number format for input \"\".").throws(__trace("core.Float"));
        gint const len = str.length();
        gint sign = +1;
        gint base = 10;
        gint next = 0;
        gfloat retVal = 0;
        gchar ch = str.charAt(0);
        switch (ch) {
            case '-':
                sign = -1;
            case '+':
                next += 1;
                if (next == len)
                    NumberFormatException("Invalid number format for input \" " + str + "\".")
                            .throws(__trace("core.Float"));
                ch = str.charAt(next);
            default:
                break;
        }
        switch (ch) {
            case '0':
                next += 1;
                if (next == len)
                    // +0 or -0
                    return 0.0F * (gfloat) sign;
                ch = str.charAt(next);
                if (ch == 'x' || ch == 'X') {
                    // hex format
                    next += 1;
                    base = 16;
                    if (next == len)
                        goto throwIllegalFormat;
                } else if (ch == 'b' || ch == 'B') {
                    // binary format 0b...
                    next += 1;
                    if (next == len)
                        goto throwIllegalFormat;
                    // check if is integer
                    if (str.indexOf('.', next) >= 0)
                        goto throwIllegalFormat;
                    base = 2;
                } else {
                    // check if is integer
                    if (str.indexOf('.', next) < 0)
                        base = 8;
                }
                break;
            case 'i':
            case 'I':
                // inf or infinity
                if (len - next == 3) {
                    gchar const ch2 = str.charAt(next + 1);
                    gchar const ch3 = str.charAt(next + 2);
                    if ((ch2 == 'n' || ch2 == 'N') && (ch3 == 'f' || ch3 == 'F'))
                        return (gfloat) (sign * Math::INF);
                } else if (len - next == 8) {
                    gchar const ch2 = str.charAt(next + 1);
                    gchar const ch3 = str.charAt(next + 2);
                    gchar const ch4 = str.charAt(next + 3);
                    gchar const ch5 = str.charAt(next + 4);
                    gchar const ch6 = str.charAt(next + 5);
                    gchar const ch7 = str.charAt(next + 6);
                    if ((ch2 == 'n' || ch2 == 'N') && (ch3 == 'f' || ch3 == 'F') && (ch4 == 'i' || ch4 == 'I') &&
                        (ch5 == 'f' || ch5 == 'F') && (ch6 == 'i' || ch6 == 'I') && (ch7 == 'y' || ch7 == 'Y'))
                        return (gfloat) (sign * Math::INF);
                }
                goto throwIllegalFormat;
            case 'n':
            case 'N':
                // NaN
                if (len - next == 3) {
                    gchar const ch2 = str.charAt(next + 1);
                    gchar const ch3 = str.charAt(next + 2);
                    if ((ch2 == 'a' || ch2 == 'A') && (ch3 == 'n' || ch3 == 'N'))
                        return (gfloat) (sign * Math::INF);
                }
                goto throwIllegalFormat;
            default:
                break;
        }
        switch (base) {
            case 2:
            case 8:
                // integral formats
                while (next < len) {
                    ch = str.charAt(next);
                    if (ch < '0' || ch > '9')
                        goto throwIllegalFormat;
                    gchar const digit = ch - '0';
                    retVal = retVal * (gfloat) base + (gfloat) digit;
                    next += 1;
                }
                return (gfloat) sign * retVal;
            case 10:
                // decimal format
                while (next < len) {
                    ch = str.charAt(next);
                    gchar digit = -1;
                    if (ch >= '0' && ch <= '9')
                        digit = ch - '0';
                    else if (ch == 'e' || ch == 'E' || ch == '.') {
                        next += 1;
                        gint decimal = 0;
                        gint exponent = 0;
                        gint sign2 = +1;
                        gint count = 0;
                        if (ch == '.') {
                            // decimal part
                            gint maxCount = 9;
                            while (next < len) {
                                ch = str.charAt(next);
                                if (ch == 'e' || ch == 'E')
                                    break;
                                if (ch >= '0' && ch <= '9')
                                    digit = ch - '0';
                                else
                                    goto throwIllegalFormat;
                                if (maxCount > 0) {
                                    if (count < maxCount) {
                                        decimal = decimal * 10 + digit;
                                        count += 1;
                                    } else if (count == maxCount) {
                                        gint tmp = decimal * 10 + digit;
                                        if (tmp > 0) {
                                            // overflow
                                            maxCount += 1;
                                            decimal = tmp;
                                            count += 1;
                                        } else if (digit >= 5 && (tmp = decimal + 1) > decimal) {
                                            decimal = tmp;
                                            maxCount = 0;
                                        }
                                    }
                                }
                                next += 1;
                            }
                        }
                        if (ch == 'e' || ch == 'E') {
                            // exponent part
                            next += 1;
                            if (next == len)
                                goto throwIllegalFormat;
                            ch = str.charAt(next);
                            switch (ch) {
                                case '-':
                                    sign2 = -1;
                                case '+':
                                    next += 1;
                                    if (next == len)
                                        goto throwIllegalFormat;
                                default:
                                    break;
                            }
                            while (next < len) {
                                ch = str.charAt(next);
                                if (ch >= '0' && ch <= '9')
                                    digit = ch - '0';
                                else
                                    goto throwIllegalFormat;
                                if (exponent <= 999999)
                                    exponent = exponent * 10 + digit;
                                next += 1;
                            }
                        }
                        return (gfloat) (sign * (retVal + Math::pow(decimal, -count)) *
                                         Math::pow(10, sign2 * exponent));
                    } else
                        goto throwIllegalFormat;
                    retVal = retVal * 10 + (gfloat) digit;
                    next += 1;
                }
                return (gfloat) sign * retVal;
            case 16: {
                gint bit32 = 0;
                gint shift = PRECISION - 1;
                gbool rounded = false;
                gbool sticky = false;
                // after 'p'
                gint decimalBitsLen = 0; // after '.'
                gint integerBitsLen = 0; // before '.'
                // hex format
                while (next < len) {
                    ch = str.charAt(next);
                    gchar digit = -1;
                    if (ch >= '0' && ch <= '9')
                        digit = ch - '0';
                    else if (ch >= 'a' && ch <= 'f')
                        digit = ch - 'a';
                    else if (ch >= 'A' && ch <= 'F')
                        digit = ch - 'A';
                    else if (ch == 'p' || ch == 'P' || ch == '.')
                        break;
                    else
                        goto throwIllegalFormat;
                    if (shift == 23) {
                        switch (digit) {
                            case 1:
                                bit32 |= (gint) digit << 23;
                                shift -= 4;
                                integerBitsLen = 0;
                                break;
                            case 2:
                            case 3:
                                bit32 |= (gint) digit << 22;
                                shift -= 5;
                                integerBitsLen = 1;
                                break;
                            case 4:
                            case 5:
                            case 6:
                            case 7:
                                bit32 |= (gint) digit << 21;
                                shift -= 6;
                                integerBitsLen = 2;
                                break;
                            default:
                                bit32 |= (gint) digit << 20;
                                shift -= 7;
                                integerBitsLen = 3;
                                break;
                        }
                    } else if (shift >= 0) {
                        bit32 |= (gint) digit << shift;
                        shift -= 4;
                    } else if (shift >= -4) {
                        switch (shift) {
                            case -1:
                                bit32 |= ((gint) digit & 0xE) >> 1;
                                rounded = (digit & 0x1) != 0;
                                break;
                            case -2:
                                bit32 |= ((gint) digit & 0xC) >> 2;
                                rounded = (digit & 0x2) != 0;
                                sticky = (digit & 0x1) != 0;
                                break;
                            case -3:
                                bit32 |= ((gint) digit & 0x8) >> 3;
                                rounded = (digit & 0x4) != 0;
                                sticky = (digit & 0x3) != 0;
                                break;
                            case -4:
                                rounded = (digit & 0x8) != 0;
                                sticky = (digit & 0x7) != 0;
                                break;
                        }
                        shift -= 4;
                    } else if (!sticky)
                        sticky |= digit != 0;
                    decimalBitsLen += 1;
                    next += 1;
                }
                {
                    gint digit = 0;
                    next += 1;
                    gint exponent = 0;
                    gint sign2 = +1;
                    if (ch == '.') {
                        // decimal part
                        while (next < len) {
                            ch = str.charAt(next);
                            if (ch == 'p' || ch == 'P')
                                break;
                            if (ch >= '0' && ch <= '9')
                                digit = ch - '0';
                            else if (ch >= 'a' && ch <= 'z')
                                digit = ch - 'a';
                            else if (ch >= 'A' && ch <= 'Z')
                                digit = ch - 'A';
                            else
                                goto throwIllegalFormat;
                            if (shift >= 0) {
                                bit32 |= (gint) digit << shift;
                                shift -= 4;
                            } else if (shift >= -4) {
                                switch (shift) {
                                    case -1:
                                        bit32 |= ((gint) digit & 0xE) >> 1;
                                        rounded = (digit & 0x1) != 0;
                                        break;
                                    case -2:
                                        bit32 |= ((gint) digit & 0xC) >> 2;
                                        rounded = (digit & 0x2) != 0;
                                        sticky = (digit & 0x1) != 0;
                                        break;
                                    case -3:
                                        bit32 |= ((gint) digit & 0x8) >> 3;
                                        rounded = (digit & 0x4) != 0;
                                        sticky = (digit & 0x3) != 0;
                                        break;
                                    case -4:
                                        rounded = (digit & 0x8) != 0;
                                        sticky = (digit & 0x7) != 0;
                                        break;
                                }
                                shift -= 4;
                            } else if (!sticky)
                                sticky |= digit != 0;
                            decimalBitsLen += 1;
                            next += 1;
                        }
                    }
                    if (ch == 'p' || ch == 'P') {
                        // exponent part
                        next += 1;
                        if (next == len)
                            goto throwIllegalFormat;
                        ch = str.charAt(next);
                        switch (ch) {
                            case '-':
                                sign2 = -1;
                            case '+':
                                next += 1;
                                if (next == len)
                                    goto throwIllegalFormat;
                            default:
                                break;
                        }
                        while (next < len) {
                            ch = str.charAt(next);
                            if (ch >= '0' && ch <= '9')
                                digit = ch - '0';
                            else
                                goto throwIllegalFormat;
                            if (exponent <= 999999)
                                exponent = exponent * 10 + digit;
                            next += 1;
                        }
                    }
                    if (bit32 == 0) {
                        retVal = 0.0F * (gfloat) sign;
                        return retVal;
                    } else {
                        gint eBit32 = sign2 * exponent + integerBitsLen + (decimalBitsLen - 1) * 4;
                        if (eBit32 > MAX_EXPONENT) {
                            // overflow
                            retVal = (gfloat) (sign * Math::INF);
                            return retVal;
                        } else if (sign2 * exponent < 0 && (sign2 * exponent) > MIN_EXPONENT) {
                            // normal
                            eBit32 += EXPONENT_BIAS;
                            eBit32 <<= 23;
                            eBit32 &= EXPONENT_BIT_MASK;
                        } else if (eBit32 < (MIN_EXPONENT - 23)) {
                            sticky |= rounded;
                            rounded = false;
                            gint const var = 24 - (eBit32 - (MIN_EXPONENT - 23) + 1);
                            rounded = (bit32 & (1LL << (var - 1))) != 0;
                            if (var > 1)
                                sticky = sticky || (bit32 & ~((~0) << (var - 1))) != 0;
                            bit32 = bit32 >> var;
                            eBit32 = (gint) (MIN_EXPONENT - 1 + EXPONENT_BIAS) << 23;
                        }
                        if (((bit32 & 1) != 0 && rounded && sticky) || (((bit32 & 1) == 0) && rounded))
                            bit32 += 1;
                        if (sign < 0)
                            bit32 |= SIGN_BIT_MASK;
                        bit32 |= eBit32;
                        retVal = fromIntBits(bit32);
                        return retVal;
                    }
                }
            }
        }
        throwIllegalFormat:
        NumberFormatException("Invalid number format for input \" " + str + "\".").throws(__trace("core.Float"));
    }

    Float Float::valueOf(const String &str) {
        try {
            return valueOf(parseFloat(str));
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Float"));
        }
    }

    Float Float::valueOf(gfloat d) {
        return d;
    }

    String Float::toString() const {
        return toString(value);
    }

    namespace {
        CORE_FAST gint b2dPrecision[] = {
                0, 0, 0, 0,
                1, 1, 1,
                2, 2, 2,
                3, 3, 3, 3,
                4, 4, 4,
                5, 5, 5,
                6, 6, 6, 6,
                7,
        };

        CORE_FAST gint powerOfTeen[] = {
                1,
                10,
                100,
                1000,
                10000,
                100000,
                1000000,
                10000000,
                100000000,
                1000000000,
        };

        CORE_FAST gchar DIGITS[] = {
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                'A', 'B', 'C', 'D', 'E', 'F'
        };
    }

    String Float::toString(gfloat d) {
        if (isNaN(d))
            return "NaN";
        if (isInfinite(d))
            return d < 0 ? "infinity" : "-infinity";
        gint bit32 = toIntBits(d);
        if (bit32 == 0)
            // bit32 = 0 00000000 00000000000000000000000.
            return "0.0";
        if (bit32 == SIGN_BIT_MASK)
            // bit32 = 1 00000000 00000000000000000000000.
            return "-0.0";
        // find the binary precision
        gint const binaryPrecision = (SIGNIFICAND_WIDTH - Integer::trailingZeros(bit32 & SIGNIFICAND_BIT_MASK));
        // conversion of binary precision to decimal precision
        gint decimalPrecision = b2dPrecision[binaryPrecision];
        if (powerOfTeen[decimalPrecision] < (1 << binaryPrecision))
            decimalPrecision += 1;
        gchar digits[sizeof(d) << 3] = {};
        gint next = 0;
        if ((bit32 & SIGN_BIT_MASK) != 0) {
            // negative
            digits[next++] = '-';
            bit32 &= ~SIGN_BIT_MASK;
        }
        // we leave one character used for rounding
        gint const placeholder = next;
        digits[next++] = '0';
        gfloat uVal = fromIntBits(bit32);
        gint exponent = 0;
        gfloat divider = 10; // used to update power of Teen
        gint updater = 1; // used to update exponent
        gint digit = {}; // last digit
        if (uVal < 1) {
            updater = -1;
            divider = 0.1;
        }
        gfloat const tmp = uVal;
        while (uVal >= 10 || uVal < 1) {
            uVal /= divider;
            exponent += updater;
        }
        if (tmp > 1.e+7 || tmp < 1.e-3) {
            // scientific format (x.y x10^z)
            digit = (gint) uVal;
            digits[next++] = DIGITS[digit];
            digits[next++] = '.';
            decimalPrecision -= decimalPrecision > 7 ? 2 : 1;
            do {
                uVal = (uVal - (gfloat) digit) * 10;
                digit = (gint) uVal;
                digits[next++] = DIGITS[digit];
                decimalPrecision -= 1;
            } while (decimalPrecision > 0);
            // round result and remove all trailing zeros
            uVal = (uVal - (gfloat) digit) * 10;
            digit = (gint) uVal;
            if (digit >= 5) {
                // round value and remove all trailing zeros
                gbool finished = false;
                while (!finished && next > 0) {
                    next -= 1;
                    if (digits[next] != '.') {
                        digit = digits[next] - 48;
                        digits[next] = DIGITS[(digit + 1) % 10];
                        finished = digit != 9;
                    }
                }
            } else if (digits[next - 1] == '0') {
                // remove all trailing zeros
                next -= 1;
                while (digits[next] == '0')
                    next -= 1;
                if (digits[next] == '.')
                    next -= 1;
            } else {
                // no problems found
                next -= 1;
            }
            next += 1;
            digits[next++] = 'E';
            digits[next++] = exponent < 0 ? '-' : '+';
            exponent = Math::abs(exponent);
            if (exponent >= 1000)
                digits[next++] = DIGITS[exponent / 1000];
            if (exponent >= 100)
                digits[next++] = DIGITS[exponent % 1000 / 100];
            if (exponent >= 10)
                digits[next++] = DIGITS[exponent % 100 / 10];
            digits[next++] = DIGITS[exponent % 10];
            digits[next] = 0;
        } else {
            // decimal format
            if (tmp < 1) {
                digits[next++] = '0';
                digits[next++] = '.';
                digit = 0;
                uVal = tmp;
            } else {
                digit = (gint) uVal;
                digits[next++] = DIGITS[digit];
                decimalPrecision -= 1;
                while (exponent > 0) {
                    uVal = (uVal - (gfloat) digit) * 10;
                    digit = (gint) uVal;
                    digits[next++] = DIGITS[digit];
                    exponent -= 1;
                    decimalPrecision -= 1;
                }
                digits[next++] = '.';
            }
            do {
                uVal = (uVal - (gfloat) digit) * 10;
                digit = (gint) uVal;
                digits[next++] = DIGITS[digit];
                decimalPrecision -= 1;
            } while (decimalPrecision > 0);
            // round result and remove all trailing zeros
            uVal = (uVal - (gfloat) digit) * 10;
            digit = (gint) uVal;
            if (digit >= 5) {
                // round value and remove all trailing zeros
                gbool finished = false;
                while (!finished && next > 0) {
                    next -= 1;
                    if (digits[next] != '.') {
                        digit = digits[next] - 48;
                        digits[next] = DIGITS[(digit + 1) % 10];
                        finished = digit != 9;
                    }
                }
            } else if (digits[next - 1] == '0') {
                // remove all trailing zeros
                next -= 1;
                while (digits[next] == '0')
                    next -= 1;
                if (digits[next] == '.')
                    next -= 1;
            } else {
                // no problems found
                next -= 1;
            }
        }
        gint begin = 0;
        if (digits[placeholder] == DIGITS[0]) {
            // the space that has be leaved in begin has not used
            if (placeholder > 0) {
                digits[placeholder] = digits[placeholder - 1];
                begin = placeholder;
            } else
                begin = 1;
        }
        return String(digits + 0, begin, digits[next + 1] != '.' ? next + 1 : next);
    }

    String Float::toHexString(gfloat d) {
        if (!isFinite(d))
            return toString(d);
        gint const bit32 = toIntBits(d);
        gchar digits[32] = {};
        gint next = 0;
        gfloat const uVal = fromIntBits(bit32 & ~SIGN_BIT_MASK);
        if ((bit32 & SIGN_BIT_MASK) != 0)
            digits[next++] = '-';
        digits[next++] = DIGITS[0];
        digits[next++] = 'x';
        digits[next++] = DIGITS[uVal < MIN_NORMAL ? 0 : 1];
        digits[next++] = '.';
        digits[next++] = DIGITS[((bit32 & SIGNIFICAND_BIT_MASK) >> 20) & 0xf];
        digits[next++] = DIGITS[((bit32 & SIGNIFICAND_BIT_MASK) >> 16) & 0xf];
        digits[next++] = DIGITS[((bit32 & SIGNIFICAND_BIT_MASK) >> 12) & 0xf];
        digits[next++] = DIGITS[((bit32 & SIGNIFICAND_BIT_MASK) >> 8) & 0x0f];
        digits[next++] = DIGITS[((bit32 & SIGNIFICAND_BIT_MASK) >> 4) & 0x0f];
        digits[next++] = DIGITS[((bit32 & SIGNIFICAND_BIT_MASK) >> 0) & 0x0f];
        // remove all trailing zero
        while (digits[next - 1] == DIGITS[0] && digits[next - 2] != '.')
            next -= 1;
        digits[next++] = 'P';
        gint exponent = uVal < MIN_NORMAL ? MIN_EXPONENT : (gint) (((bit32 & EXPONENT_BIT_MASK) >> 23) - EXPONENT_BIAS);
        digits[next++] = exponent < 0 ? '-' : '+';
        exponent = Math::abs(exponent);
        if (exponent >= 1000)
            digits[next++] = DIGITS[exponent / 1000];
        if (exponent >= 100)
            digits[next++] = DIGITS[exponent % 1000 / 100];
        if (exponent >= 10)
            digits[next++] = DIGITS[exponent % 100 / 10];
        digits[next++] = DIGITS[exponent % 10];
        digits[next] = 0;
        return String(digits, 0, next);
    }

    gint Float::hash(gfloat d) {
        if (isNaN(d))
            return Integer::hash(0x7fc00000);
        return toIntBits(d);
    }

    gbool Float::equals(const Object &object) const {
        if (Class<Float>::hasInstance(object))
            return compareTo((const Float &) object) == 0;
        return false;
    }

    gint Float::compareTo(const Float &other) const {
        return compare(value, other.value);
    }

    gint Float::toIntBits(gfloat d) {
        return *((gint *) &d);
    }

    gfloat Float::fromIntBits(gint bits) {
        return *((gfloat *) &bits);
    }

    Object &Float::clone() const {
        return U::createInstance<Float>(*this);
    }

    gshort Float::toShortBits(gfloat f) {
        gint const bit32 = toIntBits(f);
        gshort const sign = (gshort) ((bit32 & SIGN_BIT_MASK) >> 16);
        if (isNaN(f)) {
            // Preserve sign and attempt to preserve significand bits
            return (gshort) (
                    sign
                    | 0x7c00 // max exponent + 1
                    // Preserve high-order-bit of float NaN in the
                    // binary16 result NaN (tenth bit); OR in remaining
                    // bits into lower 9 bits of binary 16 significand.
                    | (bit32 & 0x007fe000) >> 13 // 10 bits
                    | (bit32 & 0x00001ff0) >> 4  //  9 bits
                    | (bit32 & 0x0000000f)       // 4 bits
            );
        }
        gfloat const uVal = Math::abs(f);
        // The overflow threshold is binary16 MAX_VALUE + 1/2 ulp
        if (uVal >= (0x1.ffcp15F + 0x0.002p15F))
            return (gshort) (sign | 0x7c00); // Positive or negative infinity
        // Smallest magnitude nonzero representable binary16 value
        // is equal to 0x1.0p-24; half-way and smaller rounds to zero.
        if (uVal <= 0x1.0p-24F * 0.5F) { // Covers float zeros and sub-normals.
            return sign; // Positive or negative zero
        }
        // Dealing with finite values in exponent range of binary16
        // (when rounding is done, could still round up)
        gint exp = ((toIntBits(f) & EXPONENT_BIT_MASK) >> (SIGNIFICAND_WIDTH - 1)) - EXPONENT_BIAS;
//        CORE_ASSERT(-25 <= exp && exp <= 15, "core.Float");
        // For binary16 subNormals, beside forcing exp to -15, retain
        // the difference exponentDiff = E_min - exp.  This is the excess
        // shift value, in addition to 13, to be used in the
        // computations below.  Further the (hidden) msb with value 1
        // in f must be involved as well.
        gint exponentDiff = 0;
        gint msb = 0x00000000;
        if (exp < -14) {
            exponentDiff = -14 - exp;
            exp = -15;
            msb = 0x00800000;
        }
        gint const fSignificandBits = bit32 & 0x007fffff | msb;

        // Significand bits as if using rounding to zero (truncation).
        gshort significandBits = (gshort) (fSignificandBits >> (13 + exponentDiff));

        // For round to nearest even, determining whether to
        // round up (in magnitude) is a function of the least
        // significant bit (LSB), the next bit position (the round
        // position), and the sticky bit (whether there are any
        // nonzero bits in the exact result to the right of the round
        // digit). An increment occurs in three cases:
        //
        // LSB  Round Sticky
        // 0    1     1
        // 1    1     0
        // 1    1     1
        // See "Computer Arithmetic Algorithms," Koren, Table 4.9
        gint const lsb = fSignificandBits & (1 << (13 + exponentDiff));
        gint const round = fSignificandBits & (1 << (12 + exponentDiff));
        gint const sticky = fSignificandBits & ((1 << (12 + exponentDiff)) - 1);

        if (round != 0 && ((lsb | sticky) != 0))
            significandBits++;

        // No bits set in significand beyond the *first* exponent bit,
        // not just the significant; quantity is added to the exponent
        // to implement a carry-out from rounding the significand.
        CORE_ASSERT((0xf800 & significandBits) == 0x0, "core.Float");
        return (gshort) (sign | (((exp + 15) << 10) + significandBits));
    }

    gfloat Float::fromShortBits(gshort bits) {
        /**
         * The binary16 format has 1 sign bit, 5 exponent bits, and 10
         * significand bits. The exponent bias is 15.
         */
        gint const bin16arg = (gint) bits;
        gint const bin16SignBit = 0x8000 & bin16arg;
        gint const bin16ExpBits = 0x7c00 & bin16arg;
        gint const bin16SignificandBits = 0x03FF & bin16arg;

        // Shift left difference in the number of significand bits in
        // the float and binary16 formats
        CORE_FAST gint SIGNIFICAND_SHIFT = (SIGNIFICAND_WIDTH - 11);

        float const sign = (bin16SignBit != 0) ? -1.0F : 1.0F;

        // Extract binary16 exponent, remove its bias, add in the bias
        // of a float exponent and shift to correct bit location
        // (significand width includes the implicit bit so shift one
        // less).
        gint const bin16Exp = (bin16ExpBits >> 10) - 15;
        if (bin16Exp == -15) {
            // For subnormal binary16 values and 0, the numerical
            // value is 2^24 * the significand as an integer (no
            // implicit bit).
            return sign * (0x1p-24F * (gfloat) bin16SignificandBits);
        } else if (bin16Exp == 16) {
            return (bin16SignificandBits == 0) ? sign * POSITIVE_INFINITY :
                   fromIntBits((bin16SignBit << 16) | 0x7f800000 |
                               // Preserve NaN significand bits
                               (bin16SignificandBits << SIGNIFICAND_SHIFT));
        }
//        CORE_ASSERT(-15 < bin16Exp && bin16Exp < 16, "Core::Float");
        gint const floatExpBits = (bin16Exp + EXPONENT_BIAS) << (SIGNIFICAND_WIDTH - 1);

        // Compute and combine result sign, exponent, and significand bits.
        return fromIntBits((bin16SignBit << 16) | floatExpBits | (bin16SignificandBits << SIGNIFICAND_SHIFT));
    }

    const gfloat Float::NaN = 0.0F / 0.0F;
    const gfloat Float::POSITIVE_INFINITY = 1.0F / 0.0F;
    const gfloat Float::NEGATIVE_INFINITY = -1.0F / 0.0F;

} // core