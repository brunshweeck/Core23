//
// Created by Brunshweeck on 12/09/2023.
//

#include "Long.h"
#include "NumberFormatException.h"
#include "ArgumentException.h"
#include "Integer.h"
#include "CastException.h"
#include <core/private/Unsafe.h>

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    glong Long::parseLong(const String &str, gint base) {
        if (base < 2 || base > 36)
            ArgumentException("Unsupported conversion base.").throws(__trace("core.Long"));
        glong length = str.length();
        if (length == 0)
            NumberFormatException("Illegal number format, for input \"\".").throws(__trace("core.Long"));
        gint i = 0;
        gint signum = 1;
        gchar ch = str.charAt(i);
        if (ch < '0') {
            if (ch == '-') {
                signum = -1;
                i += 1;
            } else if (ch == '+') {
                i += 1;
            }
            if (length == 1)
                NumberFormatException("Illegal number format, for input \"" + str + "\".").throws(__trace("core.Long"));
        }
        glong a = 0; // absolute parsed value
        while (i < length) {
            ch = str.charAt(i);
            glong digit = -1;
            if ('0' <= ch && ch <= '9')
                digit = ch - 48;
            else if ('a' <= ch && ch <= 'z')
                digit = ch - 97 + 10;
            else if ('A' <= ch && ch <= 'Z')
                digit = ch - 65 + 10;
            if (digit == -1)
                NumberFormatException("Illegal number format, for input \"" + str + "\".").throws(__trace("core.Long"));
            a = a * base + digit;
            i += 1;
            if (((a < 0) && (a > MIN_VALUE) && (signum == -1)) || ((signum == 1) && (a < 0))) {
                // signum = -1, max = MIN_VALUE
                // signum = +1, max = MAX_VALUE
                NumberFormatException("Value out of range, for input \"" + str + "\".").throws(__trace("core.Long"));
            }
        }
        return (glong) (signum == -1 && a != MIN_VALUE ? -a : a);
    }

    glong Long::parseLong(const String &str) {
        try {
            return parseLong(str, 10);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Long"));
        }
    }

    glong Long::parseUnsignedLong(const String &str, gint base) {
        if (base < 2 || base > 36)
            ArgumentException("Unsupported conversion base.").throws(__trace("core.Long"));
        glong length = str.length();
        if (length == 0)
            NumberFormatException("Illegal number format, for input \"\".").throws(__trace("core.Long"));
        gint i = 0;
        gchar ch = str.charAt(i);
        if (ch < '0') {
            if (ch == '-')
                NumberFormatException("Illegal leading minus sign, "
                                      "for unsigned input \"" + str + "\".").throws(__trace("core.Long"));
            else if (ch == '+') {
                i += 1;
            }
            if (length == 1)
                NumberFormatException("Illegal number format, for input \"" + str + "\".").throws(__trace("core.Long"));
        }
        if (length < 13 || base == 10 && length < 19) {
            // MAX_VALUE in base 36 has 13 digits
            // MAX_VALUE in base 10 has 19 digits
            glong a = {};
            try {
                a = parseLong(str, base);
            } catch (const NumberFormatException &nfe) {
                nfe.throws(__trace("core.Long"));
            }
            return a;
        } else {
            glong a = 0; // high part
            // check the high part
            while (i < length) {
                ch = str.charAt(i);
                glong digit = -1;
                if ('0' <= ch && ch <= '9')
                    digit = ch - 48;
                else if ('a' <= ch && ch <= 'z')
                    digit = ch - 97 + 10;
                else if ('A' <= ch && ch <= 'Z')
                    digit = ch - 65 + 10;
                if (digit == -1)
                    NumberFormatException("Illegal number format, for input \"" + str + "\".").throws(
                            __trace("core.Long"));
                glong t = a * base + digit;
                if (t < 0)
                    // overflow
                    break;
                a = t;
                i += 1;
            }
            glong b = 0; // low part
            // check the low part
            {
                ch = str.charAt(i);
                glong digit = -1;
                if ('0' <= ch && ch <= '9')
                    digit = ch - 48;
                else if ('a' <= ch && ch <= 'z')
                    digit = ch - 97 + 10;
                else if ('A' <= ch && ch <= 'Z')
                    digit = ch - 65 + 10;
                if (digit == -1)
                    NumberFormatException("Illegal number format, for input \"" + str + "\".").throws(
                            __trace("core.Long"));
                b = digit;
                i += 1;
            }
            glong res = a * base + b;
            gint guard = base * (gint) (a >> 57);
            if (guard >= 128 || res >= 0 && guard >= 92) {
                /*
                 * For purposes of exposition, the programmatic statements
                 * below should be taken to be multi-precision, i.e., not
                 * subject to overflow.
                 *
                 * A) Condition guard >= 128:
                 * If guard >= 128 then a*base >= 2^7 * 2^57 = 2^64 hence always overflow.
                 *
                 * B) Condition guard < 92:
                 * Define left7 = a >> 57.
                 * Given a = (left7 * 2^57) + (a & (2^57 - 1)) then
                 * result <= (base*left7)*2^57 + base*(2^57 - 1) + b.
                 * Thus, if base*left7 < 92, base <= 36, and b < 36,
                 * then result < 92*2^57 + 36*(2^57 - 1) + 36 = 2^64 hence never overflow.
                 *
                 * C) Condition 92 <= guard < 128:
                 * a*base + b >= base*left7*2^57 + b so that a*base + b >= 92*2^57 + 0 > 2^63
                 *
                 * D) Condition guard < 128:
                 * base*a <= (base*left7) * 2^57 + base*(2^57 - 1) so
                 * base*a + b <= (base*left7) * 2^57 + base*(2^57 - 1) + 36 thus
                 * base*a + b < 128 * 2^57 + 36*2^57 - base + 36 whence
                 * base*a + b < 2^64 + 2^6*2^57 = 2^64 + 2^63
                 *
                 * E) Conditions C, D, and result >= 0:
                 * C and D combined imply the mathematical result 2^63 < a*base + b < 2^64 + 2^63. The lower
                 * bound is therefore negative as a signed glong, but the upper bound is too small to overflow again after the
                 * signed glong overflows to positive above 2^64 - 1. Hence, result >= 0 implies overflow given C and D.
                 */
                NumberFormatException("Value out of range, for input \"" + str + "\".").throws(__trace("core.Long"));
            }
            // check if all other characters are valid
            while (i < length) {
                ch = str.charAt(i);
                glong digit = -1;
                if ('0' <= ch && ch <= '9')
                    digit = ch - 48;
                else if ('a' <= ch && ch <= 'z')
                    digit = ch - 97 + 10;
                else if ('A' <= ch && ch <= 'Z')
                    digit = ch - 65 + 10;
                if (digit == -1)
                    NumberFormatException("Illegal number format, for input \"" + str + "\".").throws(
                            __trace("core.Long"));
                i += 1;
            }
            return res;
        }
    }

    glong Long::parseUnsignedLong(const String &str) {
        try {
            glong i = parseUnsignedLong(str, 10);
            return i;
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Long"));
        }
    }

    Long Long::valueOf(const String &str, gint base) {
        try {
            glong i = parseLong(str, base);
            return valueOf(i);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Long"));
        }
    }

    Long Long::valueOf(const String &str) {
        try {
            return valueOf(str, 10);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Long"));
        }
    }

    Long Long::decode(const String &str) {
        glong base = 10; // by default
        glong length = str.length();
        if (length == 0)
            NumberFormatException("Illegal number format, for input \"\".").throws(__trace("core.Long"));
        gint i = 0;
        gint signum = 1;
        gchar ch = str.charAt(i);
        if (ch < '0') {
            if (ch == '-') {
                signum = -1;
                i += 1;
            } else if (ch == '+') {
                i += 1;
            }
            if (length == 1)
                NumberFormatException("Illegal number format, for input \"" + str + "\".").throws(__trace("core.Long"));
            ch = str.charAt(i);
        }
        if (ch == '0') {
            i += 1;
            if (i == length)
                return 0;
            ch = str.charAt(i);
            // check base
            if (ch == 'x' || ch == 'X')
                base = 16;
            else if (ch == 'b' || ch == 'B')
                base = 2;
            else
                base = 8;
            i += 1;
            if (i == length) {
                // 0b, 0B, 0x and 0X are illegal
                NumberFormatException("Illegal number format, for input \"" + str + "\".").throws(__trace("core.Long"));
            }
        }
        glong a = 0; // absolute parsed value
        while (i < length) {
            ch = str.charAt(i);
            glong digit = -1;
            if ('0' <= ch && ch <= '9')
                digit = ch - 48;
            else if ('a' <= ch && ch <= 'z')
                digit = ch - 97 + 10;
            else if ('A' <= ch && ch <= 'Z')
                digit = ch - 65 + 10;
            if (digit == -1)
                NumberFormatException("Illegal number format, for input \"" + str + "\".").throws(__trace("core.Long"));
            a = a * base + digit;
            i += 1;
            if (((signum == 1) && (a < 0)) || ((signum == -1) && (a < 0) && (a > MIN_VALUE)))
                NumberFormatException("Value out of range, for input \"" + str + "\".").throws(__trace("core.Long"));
        }
        return (glong) (signum == -1 && a != MIN_VALUE ? -a : a);
    }

    String Long::toString() const {
        try {
            return toString(value);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Long"));
        }
    }

    String Long::toString(glong i) {
        return toString(i, 10);
    }

    String Long::toString(glong i, gint base) {
        if (base < 2 || base > 36)
            base = 10;
        gchar digits[64 + 1] = {0}; // 64 chars max + 1 sign char
        glong a = Math::abs(i);
        gint j = 64;
        do {
            gint q = (gint) (a % base);
            glong r = a / base;
            a = r;
            digits[j--] = q + (q < 10 ? 48 : 87);
        } while (a > 0);
        if (i < 0)
            digits[j--] = u'-';
        return String(digits, j + 1, 65);
    }

    String Long::toUnsignedString(glong i, gint base) {
        if (i >= 0)
            return toString(i, base);
        gchar digits[64 + 1] = {};
        gint j = 64;
        gint r;
        while (i < 0) {
            r = (gint) ((i & 0xffffffffffffffff) % base); // auto convert to unsigned!
            digits[j--] = (gchar) ((r < 10) ? (48 + r) : (87 + r));
            i = i / base;
        }
        digits[j--] = '-';
        return String(digits, j + 1, 64 + 1);
    }

    String Long::toUnsignedString(glong i) {
        return toUnsignedString(i, 10);
    }

    String Long::toHexString(glong i) {
        return toString(i, 16);
    }

    String Long::toOctalString(glong i) {
        return toString(i, 8);
    }

    String Long::toBinaryString(glong i) {
        return toString(i, 2);
    }

    Object &Long::clone() const {
        return U::createInstance<Long>(*this);
    }

    gint Long::leadingZeros(glong l) {
        return (l >> 32) == 0 ? 32 + Integer::leadingZeros((gint) l) : Integer::leadingZeros((gint) (l >> 32));
    }

    gint Long::trailingZeros(glong l) {
        return ((gint) l) == 0 ? 32 + Integer::trailingZeros((gint) (l >> 32)) : Integer::trailingZeros((gint) l);
    }

    gint Long::bitCount(glong l) {
        l = l - ((l >> 1) & 0x5555555555555555L);
        l = (l & 0x3333333333333333L) + ((l >> 2) & 0x3333333333333333L);
        l = (l + (l >> 4)) & 0x0f0f0f0f0f0f0f0fL;
        l = l + (l >> 8);
        l = l + (l >> 16);
        l = l + (l >> 32);
        return (gint) l & 0x7f;
    }

    glong Long::reverseBits(glong l) {
        l = (l & 0x5555555555555555L) << 1 | (l >> 1) & 0x5555555555555555L;
        l = (l & 0x3333333333333333L) << 2 | (l >> 2) & 0x3333333333333333L;
        l = (l & 0x0f0f0f0f0f0f0f0fL) << 4 | (l >> 4) & 0x0f0f0f0f0f0f0f0fL;
        return reverseBytes(l);
    }

    glong Long::reverseBytes(glong l) {
        l = (l & 0x00ff00ff00ff00ffL) << 8 | (l >> 8) & 0x00ff00ff00ff00ffL;
        return (l << 48) | ((l & 0xffff0000L) << 16) | ((l >> 16) & 0xffff0000L) | (l >> 48);
    }

    Long Long::valueOf(glong l) {
        return l;
    }

    gbool Long::equals(const Object &object) const {
        if (Class<Long>::hasInstance(object))
            return value == ((Long &) object).value;
        return false;
    }

    gint Long::compareTo(const Long &other) const {
        return compare(value, other.value);
    }

} // core