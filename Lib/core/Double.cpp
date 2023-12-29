//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "Double.h"
#include "String.h"
#include "NumberFormatException.h"
#include "Math.h"
#include "Long.h"
#include "CastException.h"

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    gdouble Double::parseDouble(const String &str) {
        if (str.isEmpty())
            NumberFormatException("Invalid number format for input \"\".").throws(__trace("core.Double"));
        gint const len = str.length();
        gint sign = +1;
        gint base = 10;
        gint next = 0;
        gdouble retVal = 0;
        gchar ch = str.charAt(0);
        switch (ch) {
            case '-':
                sign = -1;
            case '+':
                next += 1;
                if (next == len)
                    NumberFormatException("Invalid number format for input \" " + str + "\".")
                            .throws(__trace("core.Double"));
                ch = str.charAt(next);
            default:
                break;
        }
        switch (ch) {
            case '0':
                next += 1;
                if (next == len)
                    // +0 or -0
                    return 0.0 * sign;
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
                        return sign * Math::INF;
                } else if (len - next == 8) {
                    gchar const ch2 = str.charAt(next + 1);
                    gchar const ch3 = str.charAt(next + 2);
                    gchar const ch4 = str.charAt(next + 3);
                    gchar const ch5 = str.charAt(next + 4);
                    gchar const ch6 = str.charAt(next + 5);
                    gchar const ch7 = str.charAt(next + 6);
                    if ((ch2 == 'n' || ch2 == 'N') && (ch3 == 'f' || ch3 == 'F') && (ch4 == 'i' || ch4 == 'I') &&
                        (ch5 == 'f' || ch5 == 'F') && (ch6 == 'i' || ch6 == 'I') && (ch7 == 'y' || ch7 == 'Y'))
                        return sign * Math::INF;
                }
                goto throwIllegalFormat;
            case 'n':
            case 'N':
                // NaN
                if (len - next == 3) {
                    gchar const ch2 = str.charAt(next + 1);
                    gchar const ch3 = str.charAt(next + 2);
                    if ((ch2 == 'a' || ch2 == 'A') && (ch3 == 'n' || ch3 == 'N'))
                        return sign * Math::INF;
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
                    retVal = retVal * base + digit;
                    next += 1;
                }
                return sign * retVal;
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
                        return sign * (retVal + Math::pow(decimal, -count)) * Math::pow(10, sign2 * exponent);
                    } else
                        goto throwIllegalFormat;
                    retVal = retVal * 10 + digit;
                    next += 1;
                }
                return sign * retVal;
            case 16: {
                glong bit64 = 0;
                gint shift = PRECISION - 1;
                gbool rounded = false;
                gbool sticky = false;
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
                    if (shift == 52) {
                        switch (digit) {
                            case 1:
                                bit64 |= (glong) digit << 52;
                                shift -= 4;
                                integerBitsLen = 0;
                                break;
                            case 2:
                            case 3:
                                bit64 |= (glong) digit << 51;
                                shift -= 5;
                                integerBitsLen = 1;
                                break;
                            case 4:
                            case 5:
                            case 6:
                            case 7:
                                bit64 |= (glong) digit << 50;
                                shift -= 6;
                                integerBitsLen = 2;
                                break;
                            default:
                                bit64 |= (glong) digit << 49;
                                shift -= 7;
                                integerBitsLen = 3;
                                break;
                        }
                    } else if (shift >= 0) {
                        bit64 |= (glong) digit << shift;
                        shift -= 4;
                    } else if (shift >= -4) {
                        switch (shift) {
                            case -1:
                                bit64 |= ((glong) digit & 0xE) >> 1;
                                rounded = (digit & 0x1) != 0;
                                break;
                            case -2:
                                bit64 |= ((glong) digit & 0xC) >> 2;
                                rounded = (digit & 0x2) != 0;
                                sticky = (digit & 0x1) != 0;
                                break;
                            case -3:
                                bit64 |= ((glong) digit & 0x8) >> 3;
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
                                bit64 |= (glong) digit << shift;
                                shift -= 4;
                            } else if (shift >= -4) {
                                switch (shift) {
                                    case -1:
                                        bit64 |= ((glong) digit & 0xE) >> 1;
                                        rounded = (digit & 0x1) != 0;
                                        break;
                                    case -2:
                                        bit64 |= ((glong) digit & 0xC) >> 2;
                                        rounded = (digit & 0x2) != 0;
                                        sticky = (digit & 0x1) != 0;
                                        break;
                                    case -3:
                                        bit64 |= ((glong) digit & 0x8) >> 3;
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
                    if (bit64 == 0) {
                        retVal = 0.0 * sign;
                        return retVal;
                    } else {
                        glong eBit64 = sign2 * exponent + integerBitsLen + (decimalBitsLen - 1) * 4;
                        if (eBit64 > MAX_EXPONENT) {
                            // overflow
                            retVal = sign * Math::INF;
                            return retVal;
                        } else if (sign2 * exponent < 0 && (sign2 * exponent) > MIN_EXPONENT) {
                            // normal
                            eBit64 += EXPONENT_BIAS;
                            eBit64 <<= 52;
                            eBit64 &= EXPONENT_BIT_MASK;
                        } else if (eBit64 < (MIN_EXPONENT - 52)) {
                            sticky |= rounded;
                            rounded = false;
                            glong const var = 53 - (eBit64 - (MIN_EXPONENT - 52) + 1);
                            rounded = (bit64 & (1LL << (var - 1))) != 0;
                            if (var > 1)
                                sticky = sticky || (bit64 & ~((~0) << (var - 1))) != 0;
                            bit64 = bit64 >> var;
                            eBit64 = (glong) (MIN_EXPONENT - 1 + EXPONENT_BIAS) << 52;
                        }
                        if (((bit64 & 1) != 0 && rounded && sticky) || (((bit64 & 1) == 0) && rounded))
                            bit64 += 1;
                        if (sign < 0)
                            bit64 |= SIGN_BIT_MASK;
                        bit64 |= eBit64;
                        retVal = fromLongBits(bit64);
                        return retVal;
                    }
                }
            }
        }
        throwIllegalFormat:
        NumberFormatException("Invalid number format for input \" " + str + "\".").throws(__trace("core.Double"));
    }

    Double Double::valueOf(const String &str) {
        try {
            return valueOf(parseDouble(str));
        } catch (const NumberFormatException &nfe) { nfe.throws(__trace("core.Double")); }
    }

    Double Double::valueOf(gdouble d) {
        return d;
    }

    String Double::toString() const {
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
                7, 7, 7,
                8, 8, 8,
                9, 9, 9, 9,
                10, 10, 10,
                11, 11, 11,
                12, 12, 12, 12,
                13, 13, 13,
                14, 14, 14,
                15, 15, 15, 15
        };

        CORE_FAST glong powerOfTeen[] = {
                1LL,
                10LL,
                100LL,
                1000LL,
                10000LL,
                100000LL,
                1000000LL,
                10000000LL,
                100000000LL,
                1000000000LL,
                10000000000LL,
                100000000000LL,
                1000000000000LL,
                10000000000000LL,
                100000000000000LL,
                1000000000000000LL,
                10000000000000000LL,
                100000000000000000LL,
                1000000000000000000LL,
        };

        CORE_FAST gchar DIGITS[] = {
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                'A', 'B', 'C', 'D', 'E', 'F'
        };
    }

    String Double::toString(gdouble d) {
        if (isNaN(d))
            return "NaN";
        if (isInfinite(d))
            return d < 0 ? "infinity" : "-infinity";
        glong bit64 = toLongBits(d);
        if (bit64 == 0)
            // bit64 = 0 00000000000 0000000000000000000000000000000000000000000000000000.
            return "0.0";
        if (bit64 == SIGN_BIT_MASK)
            // bit64 = 1 00000000000 0000000000000000000000000000000000000000000000000000.
            return "-0.0";
        // find the binary precision
        gint const binaryPrecision = (SIGNIFICAND_WIDTH - Long::trailingZeros(bit64 & SIGNIFICAND_BIT_MASK));
        // conversion of binary precision to decimal precision
        gint decimalPrecision = b2dPrecision[binaryPrecision];
        if (powerOfTeen[decimalPrecision] < (1LL << binaryPrecision))
            decimalPrecision += 1;
        gchar digits[sizeof(d) << 3] = {};
        gint next = 0;
        if ((bit64 & SIGN_BIT_MASK) != 0) {
            // negative
            digits[next++] = '-';
            bit64 &= ~SIGN_BIT_MASK;
        }
        // we leave one character used for rounding
        gint const placeholder = next;
        digits[next++] = '0';
        gdouble uVal = fromLongBits(bit64);
        gint exponent = 0;
        gdouble divider = 10; // used to update power of Teen
        gint updater = 1; // used to update exponent
        gint digit = -1; // last digit
        if (uVal < 1) {
            updater = -1;
            divider = 0.1;
        }
        gdouble const tmp = uVal;
        while (uVal >= 10 || uVal < 1) {
            uVal /= divider;
            exponent += updater;
        }
        if (tmp > 1.e+7 || tmp < 1.e-3) {
            // scientific format (x.y x10^z)
            digit = (gint) uVal;
            digits[next++] = DIGITS[digit];
            digits[next++] = '.';
            decimalPrecision -= decimalPrecision > 10 ? 2 : 1;
            do {
                uVal = (uVal - digit) * 10;
                digit = (gint) uVal;
                digits[next++] = DIGITS[digit];
                decimalPrecision -= 1;
            } while (decimalPrecision > 0);
            // round result and remove all trailing zeros
            uVal = (uVal - digit) * 10;
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
                    uVal = (uVal - digit) * 10;
                    digit = (gint) uVal;
                    digits[next++] = DIGITS[digit];
                    exponent -= 1;
                    decimalPrecision -= 1;
                }
                digits[next++] = '.';
            }
            do {
                uVal = (uVal - digit) * 10;
                digit = (gint) uVal;
                digits[next++] = DIGITS[digit];
                decimalPrecision -= 1;
            } while (decimalPrecision > 0);
            // round result and remove all trailing zeros
            uVal = (uVal - digit) * 10;
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
        return String(digits+0, begin, digits[next + 1] != '.' ? next + 1 : next);
    }

    String Double::toHexString(gdouble d) {
        if (!isFinite(d))
            return toString(d);
        glong const bit64 = toLongBits(d);
        gchar digits[32] = {};
        gint next = 0;
        gdouble const uVal = fromLongBits(bit64 & ~SIGN_BIT_MASK);
        if ((bit64 & SIGN_BIT_MASK) != 0)
            digits[next++] = '-';
        digits[next++] = DIGITS[0];
        digits[next++] = 'x';
        digits[next++] = DIGITS[uVal < MIN_NORMAL ? 0 : 1];
        digits[next++] = '.';
        digits[next++] = DIGITS[((bit64 & SIGNIFICAND_BIT_MASK) >> 48) & 0xf];
        digits[next++] = DIGITS[((bit64 & SIGNIFICAND_BIT_MASK) >> 44) & 0xf];
        digits[next++] = DIGITS[((bit64 & SIGNIFICAND_BIT_MASK) >> 40) & 0xf];
        digits[next++] = DIGITS[((bit64 & SIGNIFICAND_BIT_MASK) >> 36) & 0xf];
        digits[next++] = DIGITS[((bit64 & SIGNIFICAND_BIT_MASK) >> 32) & 0xf];
        digits[next++] = DIGITS[((bit64 & SIGNIFICAND_BIT_MASK) >> 28) & 0xf];
        digits[next++] = DIGITS[((bit64 & SIGNIFICAND_BIT_MASK) >> 24) & 0xf];
        digits[next++] = DIGITS[((bit64 & SIGNIFICAND_BIT_MASK) >> 20) & 0xf];
        digits[next++] = DIGITS[((bit64 & SIGNIFICAND_BIT_MASK) >> 16) & 0xf];
        digits[next++] = DIGITS[((bit64 & SIGNIFICAND_BIT_MASK) >> 12) & 0xf];
        digits[next++] = DIGITS[((bit64 & SIGNIFICAND_BIT_MASK) >> 8) & 0x0f];
        digits[next++] = DIGITS[((bit64 & SIGNIFICAND_BIT_MASK) >> 4) & 0x0f];
        digits[next++] = DIGITS[((bit64 & SIGNIFICAND_BIT_MASK) >> 0) & 0x0f];
        // remove all trailing zero
        while (digits[next - 1] == DIGITS[0] && digits[next - 2] != '.')
            next -= 1;
        digits[next++] = 'P';
        gint exponent = uVal < MIN_NORMAL ? MIN_EXPONENT : (gint) (((bit64 & EXPONENT_BIT_MASK) >> 52) - EXPONENT_BIAS);
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

    gint Double::hash(gdouble d) {
        if (isNaN(d))
            return Long::hash(0x7ff8000000000000LL);
        return Long::hash(toLongBits(d));
    }

    gbool Double::equals(const Object &object) const {
        if (Class<Double>::hasInstance(object))
            return compareTo((const Double &) object) == 0;
        return false;
    }

    gint Double::compareTo(const Double &other) const {
        return compare(value, other.value);
    }

    glong Double::toLongBits(gdouble d) {
        return *((glong *) &d);
    }

    gdouble Double::fromLongBits(glong bits) {
        return *((gdouble *) &bits);
    }

    Object &Double::clone() const {
        return U::createInstance<Double>(*this);
    }

    const gdouble Double::NaN = 0.0 / 0.0;
    const gdouble Double::POSITIVE_INFINITY = 1.0 / 0.0;
    const gdouble Double::NEGATIVE_INFINITY = -1.0 / 0.0;
} // core