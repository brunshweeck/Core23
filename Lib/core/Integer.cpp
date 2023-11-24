//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "Integer.h"
#include "ArgumentException.h"
#include "NumberFormatException.h"
#include "Long.h"
#include "Math.h"

namespace core {
    gint Integer::parseInt(const String &str, gint base) {
        if (base < 2 || base > 36)
            ArgumentException("Unsupported conversion base.").throws(__trace("core.Integer"));
        gint length = str.length();
        if (length == 0)
            NumberFormatException("Illegal number format, for input \"\".")
                    .throws(__trace("core.Integer"));
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
                NumberFormatException("Illegal number format, for input \"" + str + "\".")
                        .throws(__trace("core.Integer"));
        }
        glong a = 0; // absolute parsed value
        while (i < length) {
            ch = str.charAt(i);
            gint digit = -1;
            if ('0' <= ch && ch <= '9')
                digit = ch - 48;
            else if ('a' <= ch && ch <= 'z')
                digit = ch - 97 + 10;
            else if ('A' <= ch && ch <= 'Z')
                digit = ch - 65 + 10;
            if (digit == -1)
                NumberFormatException("Illegal number format, for input \"" + str + "\".")
                        .throws(__trace("core.Integer"));
            a = a * base + digit;
            i += 1;
            if (a > MAX_VALUE || a < MIN_VALUE)
                NumberFormatException("Value out of range, for input \"" + str + "\".")
                        .throws(__trace("core.Integer"));
        }
        return (gint) (signum == -1 ? -a : a);
    }

    gint Integer::parseInt(const String &str) {
        try {
            return parseInt(str, 10);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Integer"));
        }
    }

    gint Integer::parseUnsignedInt(const String &str, gint base) {
        if (base < 2 || base > 36)
            ArgumentException("Unsupported conversion base.").throws(__trace("core.Integer"));
        gint length = str.length();
        if (length == 0)
            NumberFormatException("Illegal number format, for input \"\".").throws(__trace("core.Integer"));
        gint i = 0;
        gchar ch = str.charAt(i);
        if (ch < '0') {
            if (ch == '-')
                NumberFormatException("Illegal leading minus sign, for unsigned input \"" + str + "\".")
                        .throws(__trace("core.Integer"));
        }
        if (length < 6 || base == 10 && length < 10) {
            // MAX_VALUE in base 36 has 6 digits
            // MAX_VALUE in base 10 has 10 digits
            gint a;
            try {
                a = parseInt(str, base);
            } catch (const NumberFormatException &nfe) {
                nfe.throws(__trace("core.Integer"));
            }
            return a;
        } else {
            glong a;
            try {
                a = Long::parseLong(str, base);
            } catch (const NumberFormatException &nfe) {
                nfe.throws(__trace("core.Integer"));
            }
            if ((a & 0xffffff000000LL) != 0LL)
                NumberFormatException("Value out of range, for input \"" + str + "\".").throws(
                        __trace("core.Integer"));
            return (gint) a;
        }
    }

    gint Integer::parseUnsignedInt(const String &str) {
        try {
            gint i = parseUnsignedInt(str, 10);
            return valueOf(i);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Integer"));
        }
    }

    Integer Integer::valueOf(const String &str, gint base) {
        try {
            gint i = parseInt(str, base);
            return valueOf(i);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Integer"));
        }
    }

    Integer Integer::valueOf(const String &str) {
        try {
            return valueOf(str, 10);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Integer"));
        }
    }

    Integer Integer::decode(const String &str) {
        gint base = 10; // by default
        gint length = str.length();
        if (length == 0)
            NumberFormatException("Illegal number format, for input \"\".")
                    .throws(__trace("core.Integer"));
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
                NumberFormatException("Illegal number format, for input \"" + str + "\".")
                        .throws(__trace("core.Integer"));
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
                NumberFormatException("Illegal number format, for input \"" + str + "\".")
                        .throws(__trace("core.Integer"));
            }
        }
        glong a = 0; // absolute parsed value
        while (i < length) {
            ch = str.charAt(i);
            gint digit = -1;
            if ('0' <= ch && ch <= '9')
                digit = ch - 48;
            else if ('a' <= ch && ch <= 'z')
                digit = ch - 97 + 10;
            else if ('A' <= ch && ch <= 'Z')
                digit = ch - 65 + 10;
            if (digit == -1)
                NumberFormatException("Illegal number format, for input \"" + str + "\".")
                        .throws(__trace("core.Integer"));
            a = a * base + digit;
            i += 1;
            if (a > MAX_VALUE || a < MIN_VALUE)
                NumberFormatException("Value out of range, for input \"" + str + "\".")
                        .throws(__trace("core.Integer"));
        }
        return (gint) (signum == -1 ? -a : a);
    }

    String Integer::toString() const {
        return toString(value);
    }

    String Integer::toString(gint i) {
        return toString(i, 10);
    }

    String Integer::toString(gint i, gint base) {
        if (base < 2 || base > 36)
            base = 10;
        gchar digits[32 + 1]; // 32 chars max + 1 sign char
        glong a = Math::abs((glong) i);
        gint j = 32;
        do {
            gint r = (gint) (a % base);
            a = a / base;
            digits[j--] = (gchar) ((r < 10) ? (48 + r) : (97 + r));
        } while (a > 0);
        if (i < 0)
            digits[j--] = u'-';
        return String(digits, j + 1, 32 + 1);
    }

    String Integer::toUnsignedString(gint i, gint base) {
        try {
            if (i < 0)
                return Long::toString(toUnsignedLong(i), base);
            return toString(i, base);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Integer"));
        }
    }

    String Integer::toUnsignedString(gint i) {
        try {
            return toUnsignedString(i, 10);
        } catch (const NumberFormatException &nfe) {
            nfe.throws(__trace("core.Integer"));
        }
    }

    String Integer::toHexString(gint i) {
        return toString(i, 16);
    }

    String Integer::toOctalString(gint i) {
        return toString(i, 8);
    }

    String Integer::toBinaryString(gint i) {
        return toString(i, 2);
    }

    gint Integer::leadingZeros(gint i) {
        if (i <= 0)
            return i == 0 ? 32 : 0;
        gint n = 31;
        if (i >= 1 << 16) {
            n -= 16;
            i >>= 16;
        }
        if (i >= 1 << 8) {
            n -= 8;
            i >>= 8;
        }
        if (i >= 1 << 4) {
            n -= 4;
            i >>= 4;
        }
        if (i >= 1 << 2) {
            n -= 2;
            i >>= 2;
        }
        return n - (i >> 1);
    }

    gint Integer::trailingZeros(gint i) {
        i = ~i & (i - 1);
        if (i <= 0) return i & 32;
        gint n = 1;
        if (i > 1 << 16) {
            n += 16;
            i >>= 16;
        }
        if (i > 1 << 8) {
            n += 8;
            i >>= 8;
        }
        if (i > 1 << 4) {
            n += 4;
            i >>= 4;
        }
        if (i > 1 << 2) {
            n += 2;
            i >>= 2;
        }
        return n + (i >> 1);
    }

    gint Integer::bitCount(gint i) {
        i = i - ((i >> 1) & 0x55555555);
        i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
        i = (i + (i >> 4)) & 0x0f0f0f0f;
        i = i + (i >> 8);
        i = i + (i >> 16);
        return i & 0x3f;
    }

    gint Integer::reverseBits(gint i) {
        i = (i & 0x55555555) << 1 | (i >> 1) & 0x55555555;
        i = (i & 0x33333333) << 2 | (i >> 2) & 0x33333333;
        i = (i & 0x0f0f0f0f) << 4 | (i >> 4) & 0x0f0f0f0f;
        return reverseBytes(i);
    }

    Object &Integer::clone() const {
        native::Unsafe::U.createInstance<Integer>(*this);
    }

    Integer Integer::valueOf(gint i) {
        return i;
    }
} // core