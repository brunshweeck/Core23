//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include <core/private/Preconditions.h>
#include "String.h"
#include "Character.h"
#include "Integer.h"
#include "Long.h"
#include "Float.h"
#include "Double.h"
#include "Short.h"
#include "Byte.h"
#include "ArgumentException.h"
#include "AssertionError.h"
#include "IndexException.h"
#include <core/util/Locale.h>

namespace core {

    using native::Unsafe;
    using util::Preconditions;
    using util::Locale;

    namespace {

        Unsafe &U = Unsafe::U;

        CORE_ALIAS(PBYTE, typename Class<gbyte>::Ptr);
        CORE_ALIAS(PCBYTE, typename Class<const gbyte>::Ptr);
        CORE_ALIAS(PCBYTE2, typename Class<const gchar>::Ptr);
        CORE_ALIAS(PCBYTE4, typename Class<const gint>::Ptr);

        void putChar(PBYTE dst, glong idx, gint ch) {
            if (!dst || idx < 0)
                return;
            glong index = idx * 2LL;
            if (!Character::isValidCodePoint(ch)) {
                putChar(dst, idx, '?');
            } else if (Character::isSupplementary(ch)) {
                putChar(dst, idx, Character::highSurrogate(ch));
                putChar(dst, idx + 1LL, Character::lowSurrogate(ch));
            } else {
                gbyte hb = Character::highByte(ch);
                gbyte lb = Character::lowByte(ch);
                dst[index] = hb;
                dst[index + 1LL] = lb;
            }
        }

        gchar nextChar(PCBYTE src, glong idx) {
            if (!src || idx < 0)
                return Character::MIN_VALUE;
            glong index = idx * 2LL;
            return Character::joinBytes(src[index], src[index + 1]);
        }

        PBYTE generate(gint count) {
            if (count <= 0)
                return null;
            return (PBYTE) U.allocateMemory(Integer::toUnsignedLong(count) << 1LL);
        }

        gint decodeUTF8_UTF16(PCBYTE in, PBYTE out, glong limit) {
            glong i = 0;
            glong j = 0;
            for (i = 0; i < limit;) {
                gbyte b1 = in[i];
                if (b1 >= 0) {
                    putChar(out, j, b1);
                    i += 1;
                    j += 1;
                } else if (b1 >> 5 == -2) {
                    if ((b1 & 0x1e) == 0) {
                        putChar(out, j, '?');
                    } else {
                        gbyte b2 = in[i + 1];
                        if ((b2 & 0xc0) != 0x80) {
                            putChar(out, j, '?');
                        } else {
                            gchar ch = (gchar) ((b1 << 6) ^ b2) ^ (((gbyte) 0xC0 << 6) ^ ((gbyte) 0x80 << 0));
                            putChar(out, j, ch);
                        }
                    }
                    i += 2;
                    j += 1;
                } else if (b1 >> 4 == -2) {
                    gbyte b2 = in[i + 1];
                    gbyte b3 = in[i + 2];
                    if ((b1 == (gbyte) 0xe0 && (b2 & 0xe0) == 0x80) || (b2 & 0xc0) != 0x80 || (b3 & 0xc0) != 0x80) {
                        putChar(out, j, '?');
                    } else {
                        gchar ch = (gchar) (b1 << 12) ^ (b2 << 6) ^
                                   (b3 ^ (((gbyte) 0xE0 << 12) ^ ((gbyte) 0x80 << 6) ^ ((gbyte) 0x80 << 0)));
                        if (Character::isSurrogate(ch)) {
                            putChar(out, j, '?');
                        } else {
                            putChar(out, j, ch);
                        }
                    }
                    i += 3;
                    j += 1;
                } else if (b1 >> 3 == -2) {
                    gbyte b2 = in[i + 1];
                    gbyte b3 = in[i + 2];
                    gbyte b4 = in[i + 3];
                    gint ch = (b1 << 18 ^ (b2 << 12) ^ (b3 << 6) ^ (b4 ^ (
                            ((gbyte) 0xF0 << 18) ^ ((gbyte) 0x80 << 12) ^ ((gbyte) 0x80 << 6) ^ ((gbyte) 0x80 << 0))));
                    if ((b2 & 0xc0) != 0x80 || (b3 & 0xc0) != 0x80 || (b4 & 0xc0) != 0x80 || ch <= 0xFFFF) {
                        putChar(out, j, '?');
                    } else {
                        putChar(out, j, ch);
                    }
                    i += 4;
                    j += 2;
                } else {
                    putChar(out, j, '?');
                    i += 1;
                    j += 1;
                }
            }
            return (gint) Math::min(j, (glong) Integer::MAX_VALUE);
        }

        gint charCount(gint ch) {
            return Character::isSupplementary(ch) && Character::isValidCodePoint(ch) ? 2 : 1;
        }

        void arraycopy(PCBYTE src, gint offset1, PBYTE dst, gint offset2, gint count) {
            if (!src || !dst || count == 0 || offset1 < 0 || offset2 < 0) {
                return;
            }
            glong maxCount = count * 2LL;
            glong maxOffset1 = offset1 * 2LL;
            glong maxOffset2 = offset2 * 2LL;
            if (count > 0)
                for (glong i = maxCount - 1; i >= 0; i -= 1) {
                    dst[i + maxOffset2] = src[i + maxOffset1];
                }
            else
                for (glong i = maxCount + 1; i <= 0; i += 1) {
                    dst[-i + maxOffset2] = src[-i + maxOffset1];
                }
        }

        template<class T, class U>
        void exchange(T &t, U &u) {
            T t2 = t;
            t = (T) u;
            u = (U) t2;
        }

        /**
         * Examines whether a character is 'cased'.
         *
         * A character C is defined to be 'cased' if and only if at least one of
         * following are true for C: uppercase==true, or lowercase==true, or
         * general_category==titlecase_letter.
         *
         * The uppercase and lowercase property values are specified in the data
         * file DerivedCoreProperties.txt in the Unicode Character Database.
         */
        gbool isCasedChar(gint ch) {
            Character::Category type = Character::category(ch);
            if (type == Character::Category::LOWERCASE_LETTER
                || type == Character::Category::UPPERCASE_LETTER
                || type == Character::Category::TITLECASE_LETTER)
                return true;
            else if ((ch >= 0x02B0) && (ch <= 0x02B8)) {
                // MODIFIER LETTER SMALL H..MODIFIER LETTER SMALL Y
                return true;
            } else if ((ch >= 0x02C0) && (ch <= 0x02C1)) {
                // MODIFIER LETTER GLOTTAL STOP..MODIFIER LETTER REVERSED GLOTTAL STOP
                return true;
            } else if ((ch >= 0x02E0) && (ch <= 0x02E4)) {
                // MODIFIER LETTER SMALL GAMMA..MODIFIER LETTER SMALL REVERSED GLOTTAL STOP
                return true;
            } else if (ch == 0x0345) {
                // COMBINING GREEK YPOGEGRAMMENI
                return true;
            } else if (ch == 0x037A) {
                // GREEK YPOGEGRAMMENI
                return true;
            } else if ((ch >= 0x1D2C) && (ch <= 0x1D61)) {
                // MODIFIER LETTER CAPITAL A..MODIFIER LETTER SMALL CHI
                return true;
            } else if ((ch >= 0x2160) && (ch <= 0x217F)) {
                // ROMAN NUMERAL ONE..ROMAN NUMERAL ONE THOUSAND
                // SMALL ROMAN NUMERAL ONE..SMALL ROMAN NUMERAL ONE THOUSAND
                return true;
            } else if ((ch >= 0x24B6) && (ch <= 0x24E9)) {
                // CIRCLED LATIN CAPITAL LETTER A..CIRCLED LATIN CAPITAL LETTER Z
                // CIRCLED LATIN SMALL LETTER A..CIRCLED LATIN SMALL LETTER Z
                return true;
            } else {
                return false;
            }
        }

        CORE_FAST int FINAL_CASED = 1;
        CORE_FAST int AFTER_SOFT_DOTTED = 2;
        CORE_FAST int MORE_ABOVE = 3;
        CORE_FAST int AFTER_I = 4;
        CORE_FAST int NOT_BEFORE_DOT = 5;

        // combining class definitions
        CORE_FAST int COMBINING_CLASS_ABOVE = 230;

        interface Entry {
            gchar ch;
            gchar lower[3];
            gchar upper[3];
            gchar lang[3];
            gint condition;
        };

        CORE_FAST Entry entry[] = {
                //# ================================================================================
                //# Conditional mappings
                //# ================================================================================
                {0x03A3, {0x03C2},                 {0x03A3}, {},    FINAL_CASED}, // # GREEK CAPITAL LETTER SIGMA
                {0x0130, {0x0069, 0x0307},         {0x0130}, {},    0}, // # LATIN CAPITAL LETTER I WITH DOT ABOVE

                //# ================================================================================
                //# Locale-sensitive mappings
                //# ================================================================================
                //# Lithuanian
                {0x0307, {0x0307},                 {},       u"lt", AFTER_SOFT_DOTTED}, // # COMBINING DOT ABOVE
                {0x0049, {0x0069, 0x0307},         {0x0049}, u"lt", MORE_ABOVE}, // # LATIN CAPITAL LETTER I
                {0x004A, {0x006A, 0x0307},         {0x004A}, u"lt", MORE_ABOVE}, // # LATIN CAPITAL LETTER J
                {0x012E, {0x012F, 0x0307},         {0x012E}, u"lt", MORE_ABOVE}, // # LATIN CAPITAL LETTER I WITH OGONEK
                {0x00CC, {0x0069, 0x0307, 0x0300}, {0x00CC}, u"lt", 0}, // # LATIN CAPITAL LETTER I WITH GRAVE
                {0x00CD, {0x0069, 0x0307, 0x0301}, {0x00CD}, u"lt", 0}, // # LATIN CAPITAL LETTER I WITH ACUTE
                {0x0128, {0x0069, 0x0307, 0x0303}, {0x0128}, u"lt", 0}, // # LATIN CAPITAL LETTER I WITH TILDE

                //# ================================================================================
                //# Turkish and Azeri
                {0x0130, {0x0069},                 {0x0130}, u"tr", 0}, // # LATIN CAPITAL LETTER I WITH DOT ABOVE
                {0x0130, {0x0069},                 {0x0130}, u"az", 0}, // # LATIN CAPITAL LETTER I WITH DOT ABOVE
                {0x0307, {},                       {0x0307}, u"tr", AFTER_I}, // # COMBINING DOT ABOVE
                {0x0307, {},                       {0x0307}, u"az", AFTER_I}, // # COMBINING DOT ABOVE
                {0x0049, {0x0131},                 {0x0049}, u"tr", NOT_BEFORE_DOT}, // # LATIN CAPITAL LETTER I
                {0x0049, {0x0131},                 {0x0049}, u"az", NOT_BEFORE_DOT}, // # LATIN CAPITAL LETTER I
                {0x0069, {0x0069},                 {0x0130}, u"tr", 0}, // # LATIN SMALL LETTER I
                {0x0069, {0x0069},                 {0x0130}, u"az", 0},  // # LATIN SMALL LETTER I
        };

        /**
         * Implements the "Final_Cased" condition
         *
         * Specification: Within the closest word boundaries containing C, there is a cased
         * letter before C, and there is no cased letter after C.
         */
        gbool isFinalCased(PCBYTE str, gint index, const String &lang, gint len = 0) {
            int ch = 0;

            // Look for a preceding 'cased' letter
            for (int i = index; i > 0 && i < len; i -= charCount(ch)) {

                ch = i == 0 ? 0 : nextChar(str, i - 1);
                if (Character::isLowSurrogate(ch) && i > 1) {
                    gint ch0 = nextChar(str, i - 2);
                    if (Character::isHighSurrogate(ch0))
                        ch = Character::joinSurrogates(ch0, ch);
                }

                if (isCasedChar(ch)) {
                    ch = nextChar(str, index);
                    if (Character::isHighSurrogate(ch)) {
                        gchar ch2 = nextChar(str, index + 1);
                        if (Character::isLowSurrogate(ch2))
                            ch = Character::joinSurrogates(ch2, ch2);
                    }
                    // Check that there is no 'cased' letter after the index
                    for (i = index + charCount(ch); i < len && i > 0; i += charCount(ch)) {
                        if (isCasedChar(ch)) {
                            return false;
                        }
                        ch = nextChar(str, index);
                        if (Character::isHighSurrogate(ch)) {
                            gchar ch2 = nextChar(str, index + 1);
                            if (Character::isLowSurrogate(ch2))
                                ch = Character::joinSurrogates(ch2, ch2);
                        }
                    }
                    return true;
                }
            }

            return false;
        }

        gbool isSoftDotted(int ch) {
            switch (ch) {
                case 0x0069: // Soft_Dotted # L&       LATIN SMALL LETTER I
                case 0x006A: // Soft_Dotted # L&       LATIN SMALL LETTER J
                case 0x012F: // Soft_Dotted # L&       LATIN SMALL LETTER I WITH OGONEK
                case 0x0268: // Soft_Dotted # L&       LATIN SMALL LETTER I WITH STROKE
                case 0x0456: // Soft_Dotted # L&       CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
                case 0x0458: // Soft_Dotted # L&       CYRILLIC SMALL LETTER JE
                case 0x1D62: // Soft_Dotted # L&       LATIN SUBSCRIPT SMALL LETTER I
                case 0x1E2D: // Soft_Dotted # L&       LATIN SMALL LETTER I WITH TILDE BELOW
                case 0x1ECB: // Soft_Dotted # L&       LATIN SMALL LETTER I WITH DOT BELOW
                case 0x2071: // Soft_Dotted # L&       SUPERSCRIPT LATIN SMALL LETTER I
                    return true;
                default:
                    return false;
            }
        }

        /**
         * Implements the "After_Soft_Dotted" condition
         *
         * Specification: The last preceding character with combining class
         * of zero before C was Soft_Dotted, and there is no intervening
         * combining character class 230 (ABOVE).
         */
        gbool isAfterSoftDotted(PCBYTE str, gint index) {
            int ch = 0;
            Character::CombiningClass cc = Character::CombiningClass::UNDEFINED;

            // Look for the last preceding character
            for (int i = index; i > 0; i -= charCount(ch)) {

                ch = i == 0 ? 0 : nextChar(str, i - 1);
                if (Character::isLowSurrogate(ch) && i > 1) {
                    gint ch0 = nextChar(str, i - 2);
                    if (Character::isHighSurrogate(ch0))
                        ch = Character::joinSurrogates(ch0, ch);
                }

                if (isSoftDotted(ch)) {
                    return true;
                }
                cc = Character::combiningClass(ch);
                if ((cc == Character::CombiningClass::UNDEFINED) || (cc == Character::CombiningClass::ABOVE)) {
                    return false;
                }

            }

            return false;
        }

        /**
         * Implements the "More_Above" condition
         *
         * Specification: C is followed by one or more characters of combining
         * class 230 (ABOVE) in the combining character sequence.
         */
        gbool isMoreAbove(PCBYTE str, gint index, gint len) {
            int ch = 0;
            Character::CombiningClass cc;

            gint ch0 = nextChar(str, index);
            if (Character::isHighSurrogate(ch0)) {
                gchar ch1 = nextChar(str, index + 1);
                if (Character::isLowSurrogate(ch1))
                    ch0 = Character::joinSurrogates(ch0, ch1);
            }

            // Look for a following ABOVE combining class character
            for (int i = index + charCount(ch0); i < len; i += charCount(ch)) {

                ch = nextChar(str, index);
                if (Character::isHighSurrogate(ch)) {
                    gchar ch2 = nextChar(str, index + 1);
                    if (Character::isLowSurrogate(ch2))
                        ch = Character::joinSurrogates(ch0, ch2);
                }
                cc = Character::combiningClass(ch);

                if (cc == Character::CombiningClass::ABOVE) {
                    return true;
                } else if (cc == Character::CombiningClass::UNDEFINED) {
                    return false;
                }
            }

            return false;
        }

        /**
         * Implements the "After_I" condition
         *
         * Specification: The last preceding base character was an uppercase I,
         * and there is no intervening combining character class 230 (ABOVE).
         */
        gbool isAfterI(PCBYTE str, gint index) {
            int ch;
            Character::CombiningClass cc = Character::CombiningClass::UNDEFINED;

            // Look for the last preceding base character
            for (int i = index; i > 0; i -= charCount(ch)) {

                ch = i == 0 ? 0 : nextChar(str, i - 1);
                if (Character::isLowSurrogate(ch) && i > 1) {
                    gint ch0 = nextChar(str, i - 2);
                    if (Character::isHighSurrogate(ch0))
                        ch = Character::joinSurrogates(ch0, ch);
                }

                if (ch == 'I') {
                    return true;
                } else {
                    cc = Character::combiningClass(ch);
                    if ((cc == Character::CombiningClass::UNDEFINED) || (cc == Character::CombiningClass::ABOVE)) {
                        return false;
                    }
                }
            }

            return false;
        }

        /**
         * Implements the "Before_Dot" condition
         *
         * Specification: C is followed by <b> U+0307 COMBINING DOT ABOVE</b>.
         * Any sequence of characters with a combining class that is
         * neither 0 nor 230 may intervene between the current character
         * and the combining dot above.
         */
        gbool isBeforeDot(PCBYTE str, gint index, gint len) {
            int ch;
            Character::CombiningClass cc = Character::CombiningClass::UNDEFINED;

            gint ch0 = nextChar(str, index);
            if (Character::isHighSurrogate(ch0)) {
                gchar ch1 = nextChar(str, index + 1);
                if (Character::isLowSurrogate(ch1))
                    ch0 = Character::joinSurrogates(ch0, ch1);
            }

            // Look for a following COMBINING DOT ABOVE
            for (int i = index + charCount(ch0); i < len; i += charCount(ch)) {

                ch = nextChar(str, index);
                if (Character::isHighSurrogate(ch)) {
                    gchar ch2 = nextChar(str, index + 1);
                    if (Character::isLowSurrogate(ch2))
                        ch = Character::joinSurrogates(ch0, ch2);
                }

                if (ch == u'\u0307') {
                    return true;
                } else {
                    cc = Character::combiningClass(ch);
                    if ((cc == Character::CombiningClass::UNDEFINED) || (cc == Character::CombiningClass::ABOVE)) {
                        return false;
                    }
                }
            }

            return false;
        }

        gbool conditionIsTrue(PCBYTE str, gint index, const String &lang, gint condition, gint len) {
            switch (condition) {
                case FINAL_CASED:
                    return isFinalCased(str, index, lang);
                case AFTER_SOFT_DOTTED:
                    return isAfterSoftDotted(str, index);
                case MORE_ABOVE:
                    return isMoreAbove(str, index, len);
                case AFTER_I:
                    return isAfterI(str, index);
                case NOT_BEFORE_DOT:
                    return !isBeforeDot(str, index, len);
                default:
                    return true;
            }
        }

        gint lookupTable(PCBYTE str, gint index, const String &lang, gbool lower, gint &len) {
            gint ch = nextChar(str, index);
            gchar *ret = null;
            gint idx = -1;
            gbool ok = false;
            for (const Entry &e: entry) {
                idx += 1;
                if (e.ch != ch) {
                    if (ok) {
                        idx -= 1;
                        break;
                    }
                    continue;
                }
                if (e.lang[0] == 0 || (lang.equals(e.lang) && conditionIsTrue(str, index, lang, e.condition, len))) {
                    ret = (gchar *) (lower ? e.lower : e.upper);
                    len = ret[0] == 0 ? 0 : ret[1] == 0 ? 1 : ret[2] == 0 ? 2 : 3;
                    ok = true;
                    if (e.lang[0] != 0)
                        break;
                }
            }
            return ok ? idx : -1;
        }
    }

    void String::wrap(glong addr, gint bpc, glong count) {
        value = null;
        len = 0;
        hashcode = 0;
        isHashed = false;
        if (addr == 0)
            ArgumentException("Null pointer").throws(__trace("core.String"));
        if ((bpc != 1) && (bpc != 2) && (bpc != 4))
            AssertionError("Unsupported character type").throws(__trace("core.String"));
        if (count == 0) return;
        gint length = 0;
        if (count < 0) {
            count = 0;
            switch (bpc) {
                case 1: {
                    PCBYTE b = (PCBYTE) addr;
                    while (b[count] != 0)
                        count += 1;
                    length = decodeUTF8_UTF16(b, null, count);
                    value = generate(length);
                    len = length;
                    decodeUTF8_UTF16(b, value, count);
                    break;
                }
                case 2: {
                    PCBYTE2 b = (PCBYTE2) addr;
                    while (b[count] != 0)
                        count += 1;
                    length = (gint) Math::min((glong) Integer::MAX_VALUE, count);
                    value = generate(length);
                    len = length;
                    for (gint i = 0; i < length; ++i) {
                        putChar(value, i, b[i]);
                    }
                    break;
                }
                case 4: {
                    PCBYTE4 b = (PCBYTE4) addr;
                    gint count2 = 0;
                    while (b[count] != 0) {
                        count2 += charCount(b[count]);
                        count += 1;
                        if (count2 < 0) {
                            count2 = Integer::MAX_VALUE;
                            break;
                        }
                    }
                    length = Math::min(Integer::MAX_VALUE, count2);
                    value = generate(length);
                    len = length;
                    gint j = 0;
                    for (gint i = 0; i < count; ++i) {
                        putChar(value, j, b[i]);
                        j += charCount(b[i]);
                    }
                    break;
                }
                default:
                    return;
            }
        } else {
            switch (bpc) {
                case 1: {
                    PCBYTE b = (PCBYTE) addr;
                    length = decodeUTF8_UTF16(b, null, count);
                    value = generate(length);
                    len = length;
                    decodeUTF8_UTF16(b, value, count);
                    break;
                }
                case 2: {
                    PCBYTE2 b = (PCBYTE2) addr;
                    length = (gint) Long::min(Integer::MAX_VALUE, count);
                    value = generate(length);
                    len = length;
                    for (gint i = 0; i < length; ++i) {
                        putChar(value, i, b[i]);
                    }
                    break;
                }
                case 4: {
                    PCBYTE4 b = (PCBYTE4) addr;
                    gint count2 = 0;
                    count = (gint) Long::min(Integer::MAX_VALUE, count);
                    for (gint i = 0; i < count; ++i) {
                        count2 += charCount(b[i]);
                    }
                    length = (gint) Long::min(Integer::MAX_VALUE, count2);
                    value = generate(length);
                    len = length;
                    gint j = 0;
                    for (gint i = 0; i < count; ++i) {
                        putChar(value, j, b[i]);
                        j += charCount(b[i]);
                    }
                    break;
                }
                default:
                    return;
            }
        }
    }

    void String::wrap(glong addr, gint bpc, glong count, gint offset, gint limit) {
        value = null;
        len = 0;
        hashcode = 0;
        isHashed = false;
        if (addr == 0)
            ArgumentException("Null pointer").throws(__trace("core.String"));
        if (limit < 0)
            ArgumentException("Negative length").throws(__trace("core.String"));
        if (offset < 0)
            ArgumentException("Negative offset").throws(__trace("core.String"));
        if ((bpc != 1) && (bpc != 2) && (bpc != 4))
            AssertionError("Unsupported character type").throws(__trace("core.String"));
        if (count == 0) return;
        gint length = 0;
        if (count < 0) {
            count = Math::min(Integer::MAX_VALUE, limit);
        } else {
            count = Math::min(count, (glong) limit);
        }
        addr += offset * (glong) bpc;
        switch (bpc) {
            case 1: {
                PCBYTE b = (PCBYTE) addr;
                length = decodeUTF8_UTF16(b, null, count);
                value = generate(length);
                len = length;
                decodeUTF8_UTF16(b, value, count);
                break;
            }
            case 2: {
                PCBYTE2 b = (PCBYTE2) addr;
                length = (gint) count;
                value = generate(length);
                len = length;
                for (gint i = 0; i < length; ++i) {
                    gchar ch = b[i];
                    putChar(value, i, ch);
                }
                break;
            }
            case 4: {
                PCBYTE4 b = (PCBYTE4) addr;
                gint count2 = 0;
                count = (gint) Long::min(Integer::MAX_VALUE, count);
                for (gint i = 0; i < count; ++i) {
                    count2 += charCount(b[i]);
                }
                length = (gint) Long::min(Integer::MAX_VALUE, count2);
                value = generate(length);
                len = length;
                gint j = 0;
                for (gint i = 0; i < count; ++i) {
                    putChar(value, j, b[i]);
                    j += charCount(b[i]);
                }
                break;
            }
            default:
                return;
        }
    }

    String::String(const String &original) {
        value = generate(original.len);
        len = original.length();
        hashcode = original.hashcode;
        isHashed = original.isHashed;
        arraycopy(original.value, 0, value, 0, len);
    }

    String::String(String &&original) CORE_NOTHROW {
        exchange(value, original.value);
        exchange(len, original.len);
        exchange(hashcode, original.hashcode);
        exchange(isHashed, original.isHashed);
    }

    String &String::operator=(const String &str) {
        if (this != &str) {
            if (len != str.len) {
                U.freeMemory((glong) value);
                value = null;
                len = 0;
            }
            value = generate(str.len);
            len = str.len;
            hashcode = str.hashcode;
            isHashed = str.isHashed;
            arraycopy(str.value, 0, value, 0, len);
        }
        return *this;
    }

    String &String::operator=(String &&str) CORE_NOTHROW {
        if (this != &str) {
            exchange(value, str.value);
            exchange(len, str.len);
            exchange(hashcode, str.hashcode);
            exchange(isHashed, str.isHashed);
        }
        return *this;
    }

    gchar String::charAt(gint index) const {
        try {
            Preconditions::checkIndex(index, len);
        } catch (IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
        gchar ch = nextChar(value, index);
        return ch;
    }

    gint String::codePointAt(gint index) const {
        try {
            Preconditions::checkIndex(index, len);
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
        gchar ch = nextChar(value, index);
        if (Character::isSurrogate(ch))
            return ch;
        gchar ch2 = nextChar(value, index + 1);
        if (Character::isSurrogatePair(ch, ch2))
            return Character::joinSurrogates(ch, ch2);
        return ch;
    }

    gbool String::equals(const Object &object) const {
        if (!Class<String>::hasInstance(object))
            return false;
        const String &str = (String &) object;
        if (len != str.len)
            return false;
        return compareTo(str) == 0;
    }

    gbool String::equals(const String &str) const {
        if (Object::equals(str))
            return true;
        if (len != str.len)
            return false;
        return compareTo(str) == 0;
    }

    gbool String::equalsIgnoreCase(const String &str) const {
        if (len != str.len)
            return false;
        return compareToIgnoreCase(str) == 0;
    }

    gint String::compareTo(const String &other) const {
        if (this == &other)
            return 0;
        gint length = Math::min(len, other.len);
        for (gint i = 0; i < length; ++i) {
            gchar ch1 = nextChar(value, i);
            gchar ch2 = nextChar(other.value, i);
            if (ch1 != ch2)
                return ch1 - ch2;
        }
        return len == other.len ? 0 :
               length == len ? -nextChar(other.value, length) :
               nextChar(value, length);
    }

    gint String::compareToIgnoreCase(const String &other) const {
        if (this == &other)
            return 0;
        gint length = Math::min(len, other.len);
        for (gint i = 0; i < length; ++i) {
            gchar ch1 = nextChar(value, i);
            gchar ch2 = nextChar(other.value, i);
            if (ch1 != ch2) {
                ch1 = Character::toLowerCase(ch1);
                ch2 = Character::toLowerCase(ch2);
                if (ch1 != ch2)
                    return ch1 - ch2;
            }
        }
        return len == other.len ? 0 :
               length == len ? -nextChar(other.value, length) :
               nextChar(value, length);
    }

    gbool String::startsWith(const String &str, gint offset) const {
        if (offset >= 0 && str.len > 0 && str.len <= len) {
            gbool isFound = true;
            for (gint i = offset; i < len; ++i) {
                for (gint j = 0; j < str.len; ++j) {
                    gchar ch1 = nextChar(value, i + j);
                    gchar ch2 = nextChar(str.value, j);
                    if (ch1 != ch2) {
                        isFound = false;
                        break;
                    }
                }
                if (isFound)
                    return true;
            }
        }
        return false;
    }

    gbool String::startsWith(const String &str) const {
        return startsWith(str, 0);
    }

    gbool String::endsWith(const String &str) const {
        return startsWith(str, length() - str.length());
    }

    gint String::hash() const {
        if (!isHashed) {
            gint &hash = (gint &) hashcode;
            hash = 0;
            for (gint i = 0; i < len; ++i) {
                gchar ch = nextChar(value, i);
                hash += ch * 31 ^ (len - (i + 1));
            }
        }
        return hashcode;
    }

    gint String::indexOf(gint ch) const {
        return indexOf(ch, 0);
    }

    gint String::indexOf(gint ch, gint startIndex) const {
        if (startIndex >= 0)
            for (gint i = startIndex; i < len; ++i) {
                gchar ch2 = nextChar(value, i);
                if (ch2 == ch)
                    return i;
            }
        return -1;
    }

    gint String::lastIndexOf(gint ch) const {
        return lastIndexOf(ch, length() - 1);
    }

    gint String::lastIndexOf(gint ch, gint startIndex) const {
        if (startIndex >= len)
            startIndex = len - 1;
        if (startIndex >= 0)
            for (gint i = startIndex; i >= 0; --i) {
                gchar ch2 = nextChar(value, i);
                if (ch2 == ch)
                    return i;
            }
        return -1;
    }

    gint String::indexOf(const String &str) {
        return indexOf(str, 0);
    }

    gint String::indexOf(const String &str, gint startIndex) {
        if (startIndex < 0 || startIndex + str.len > len || str.isEmpty())
            return -1;
        for (gint i = startIndex; i < len; ++i) {
            gbool isFound = true;
            for (gint j = 0; j < str.len; ++j) {
                gchar ch1 = nextChar(value, i + j);
                gchar ch2 = nextChar(str.value, j);
                if (ch1 != ch2) {
                    isFound = false;
                    break;
                }
            }
            if (isFound) {
                return i;
            }
        }
        return -1;
    }

    gint String::lastIndexOf(const String &str) {
        return lastIndexOf(str, len - 1);
    }

    gint String::lastIndexOf(const String &str, gint startIndex) {
        if (startIndex >= len)
            startIndex = len - 1;
        if (startIndex < 0 || startIndex + 1 < str.len || str.isEmpty())
            return -1;
        for (gint i = startIndex; i >= 0; --i) {
            gbool isFound = true;
            for (gint j = 0; j < str.len; ++j) {
                gchar ch1 = nextChar(value, i - j);
                gchar ch2 = nextChar(str.value, str.len - 1 - j);
                if (ch1 != ch2) {
                    isFound = false;
                    break;
                }
            }
            if (isFound) {
                return i + 1 - str.len;
            }
        }
        return -1;
    }

    String String::subString(gint startIndex) const {
        try {
            return subString(startIndex, length());
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
    }

    String String::subString(gint startIndex, gint endIndex) const {
        try {
            Preconditions::checkIndexFromRange(startIndex, endIndex, len);
            if (endIndex > len)
                endIndex = len;
            String str;
            str.len = endIndex - startIndex;
            str.value = generate(str.len);
            arraycopy(value, startIndex, str.value, 0, str.len);
            return str;
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
    }

    String String::concat(const String &str) const {
        String str2;
        str2.len = len + str.len;
        if (str2.len < 0)
            MemoryError("Overflow: String length out of range").throws(__trace("core.String"));
        str2.value = generate(str2.len);
        arraycopy(value, 0, str2.value, 0, len);
        arraycopy(str.value, 0, str2.value, len, str.len);
        return str2;
    }

    String String::replace(gchar oldChar, gchar newChar) {
        String str;
        str.value = generate(len);
        for (gint i = 0; i < len; ++i) {
            gchar ch = nextChar(value, i);
            putChar(str.value, i, (ch == oldChar) ? newChar : ch);
        }
        return str;
    }

    String String::replace(const String &str, const String &replacement) {
        gint cnt = count(str);
        if (cnt == 0 || isEmpty() || str.len == 0)
            return *this;
        String str2;
        str2.len = len + (replacement.len - str.len) * cnt;
        if (str2.len < 0)
            MemoryError("Overflow: String length out of range").throws(__trace("core.String"));
        str2.value = generate(str2.len);
        gint k = 0;
        for (gint i = 0; i < len; ++i) {
            gbool isFound = false;
            if (cnt > 0) {
                isFound = true;
                for (gint j = 0; j < str.len; ++j) {
                    gchar ch1 = nextChar(value, i + j);
                    gchar ch2 = nextChar(str.value, j);
                    if (ch1 != ch2) {
                        isFound = false;
                        break;
                    }
                }
            }
            if (isFound) {
                cnt -= 1;
                arraycopy(replacement.value, 0, str2.value, k, replacement.len);
                k += replacement.len;
                i += str.len - 1;
            } else {
                gchar ch = nextChar(value, i);
                putChar(str2.value, k, ch);
                k += 1;
            }
        }
        return str;
    }

    String String::toLowerCase() const {
        String str;
        str.len = len;
        str.value = generate(len);
        for (gint i = 0; i < len; ++i) {
            gchar ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint codePoint = Character::joinSurrogates(ch, ch2);
                    gint lowerCase = Character::toLowerCase(codePoint);
                    putChar(str.value, i, lowerCase);
                    if (Character::isSupplementary(lowerCase))
                        i += 1;
                }
            } else {
                gchar lowerCase = Character::toLowerCase(ch);
                putChar(str.value, i, lowerCase);
            }
        }
        return str;
    }

    String String::toLowerCase(const util::Locale &locale) const {
        String str;
        str.len = len;
        String lang = locale.language();
        gbool isLocaleDependant = lang.equals("tr") || lang.equals("az") || lang.equals("lt"); /* or ch == '\u0130' */
        gint length = 0;
        gint i2 = 0;
        while (i2 < len) {
            gint ch = nextChar(value, i2);
            if (((isLocaleDependant &&
                  (ch == 0x0307 || ch == 0x0049 || ch == 0x004A || ch == 0x012E || ch == 0x00CC || ch == 0x00CD ||
                   ch == 0x0128 || ch == 0x0130 || ch == 0x0069)) || ch == 0x03a3 || ch == 0x0130)) {
                gint n = len;
                gint idx = lookupTable(value, i2, lang, true, n);
                if (idx < 0)
                    length += 1;
                else
                    length += n;
            } else
                length += 1;
            i2 += 1;
        }
        str.value = generate(length);
        str.len += length;
        gint lowerCase = 0;

        gint i = 0;
        gint j = 0;
        for (; i < len; ++i) {
            gchar ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint codePoint = Character::joinSurrogates(ch, ch2);
                    lowerCase = Character::toLowerCase(codePoint);
                    if (Character::isSupplementary(lowerCase))
                        i += 1;
                } else {
                    lowerCase = ch;
                }
            } else {
                if (((isLocaleDependant &&
                      (ch == 0x0307 || ch == 0x0049 || ch == 0x004A || ch == 0x012E || ch == 0x00CC || ch == 0x00CD ||
                       ch == 0x0128 || ch == 0x0130 || ch == 0x0069)) || ch == 0x03a3 || ch == 0x0130)) {
                    length = len;
                    gint idx = lookupTable(value, i, lang, true, length);
                    if (idx < 0) {
                        lowerCase = Character::toLowerCase(ch);
                    } else {
                        for (int k = 0; k < length; ++k) {
                            putChar(str.value, j++, entry[idx].lower[k]);
                        }
                        continue;
                    }
                } else
                    lowerCase = Character::toLowerCase(ch);
            }
            putChar(str.value, j, lowerCase);
            j += charCount(lowerCase);
        }
        if (j < str.len) {
            // reduce string length
            str.len = j;
            putChar(str.value, j, 0);
        }
        return str;
    }

    String String::toUpperCase(const Locale &locale) const {
        String str;
        str.len = len;
        String lang = locale.language();
        gbool isLocaleDependant = lang.equals("tr") || lang.equals("az") || lang.equals("lt"); /* or ch == '\u0130' */
        gint length = 0;
        gint i2 = 0;
        while (i2 < len) {
            gint ch = nextChar(value, i2);
            if (((isLocaleDependant &&
                  (ch == 0x0307 || ch == 0x0049 || ch == 0x004A || ch == 0x012E || ch == 0x00CC || ch == 0x00CD ||
                   ch == 0x0128 || ch == 0x0130 || ch == 0x0069)) || ch == 0x03a3 || ch == 0x0130)) {
                gint n = len;
                gint idx = lookupTable(value, i2, lang, false, n);
                if (idx < 0)
                    length += 1;
                else
                    length += n;
            } else
                length += 1;
            i2 += 1;
        }
        str.value = generate(length);
        str.len += length;
        gint upperCase = 0;

        gint i = 0;
        gint j = 0;
        for (; i < len; ++i) {
            gchar ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint codePoint = Character::joinSurrogates(ch, ch2);
                    upperCase = Character::toUpperCase(codePoint);
                    if (Character::isSupplementary(upperCase))
                        i += 1;
                } else {
                    upperCase = ch;
                }
            } else {
                if (((isLocaleDependant &&
                      (ch == 0x0307 || ch == 0x0049 || ch == 0x004A || ch == 0x012E || ch == 0x00CC || ch == 0x00CD ||
                       ch == 0x0128 || ch == 0x0130 || ch == 0x0069)) || ch == 0x03a3 || ch == 0x0130)) {
                    length = len;
                    gint idx = lookupTable(value, i, lang, false, length);
                    if (idx < 0) {
                        upperCase = Character::toUpperCase(ch);
                    } else {
                        for (int k = 0; k < length; ++k) {
                            putChar(str.value, j++, entry[idx].upper[k]);
                        }
                        continue;
                    }
                } else
                    upperCase = Character::toUpperCase(ch);
            }
            putChar(str.value, j, upperCase);
            j += charCount(upperCase);
        }
        if (j < str.len) {
            // reduce string length
            str.len = j;
            putChar(str.value, j, 0);
        }
        return str;
    }

    String String::toUpperCase() const {
        String str;
        str.len = len;
        str.value = generate(str.len);
        for (gint i = 0; i < len; ++i) {
            gchar ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint codePoint = Character::joinSurrogates(ch, ch2);
                    gint upperCase = Character::toUpperCase(codePoint);
                    putChar(str.value, i, upperCase);
                    if (Character::isSupplementary(upperCase))
                        i += 1;
                }
            } else {
                gchar upperCase = Character::toUpperCase(ch);
                putChar(str.value, i, upperCase);
            }
        }
        return str;
    }

    String String::toTitleCase() const {
        String str;
        str.len = len;
        str.value = generate(len);
        for (gint i = 0; i < len; ++i) {
            gchar ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint codePoint = Character::joinSurrogates(ch, ch2);
                    gint titleCase = Character::toTitleCase(codePoint);
                    putChar(str.value, i, titleCase);
                    if (Character::isSupplementary(titleCase))
                        i += 1;
                }
            } else {
                gchar titleCase = Character::toTitleCase(ch);
                putChar(str.value, i, titleCase);
            }
        }
        return str;
    }

    String String::toReverseCase() const {
        String str;
        str.len = len;
        str.value = generate(len);
        for (gint i = 0; i < len; ++i) {
            gchar ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint codePoint = Character::joinSurrogates(ch, ch2);
                    gint reversedCase = 0;
                    if (Character::isLowerCase(codePoint))
                        reversedCase = Character::toUpperCase(codePoint);
                    else if (Character::isUpperCase(codePoint))
                        reversedCase = Character::toLowerCase(codePoint);
                    else if (Character::isTitleCase(codePoint))
                        reversedCase = Character::toCaseFold(codePoint);
                    else
                        reversedCase = codePoint;
                    putChar(str.value, i, reversedCase);
                    if (Character::isSupplementary(reversedCase))
                        i += 1;
                }
            } else {
                gchar reversedCase = 0;
                if (Character::isLowerCase(ch))
                    reversedCase = Character::toUpperCase(ch);
                else if (Character::isUpperCase(ch))
                    reversedCase = Character::toLowerCase(ch);
                else if (Character::isTitleCase(ch))
                    reversedCase = Character::toCaseFold(ch);
                else
                    reversedCase = ch;
                putChar(str.value, i, reversedCase);
            }
        }
        return str;
    }

    String String::strip() const {
        gint startIndex = 0;
        for (startIndex = 0; startIndex < len; ++startIndex) {
            gchar ch = nextChar(value, startIndex);
            if (!Character::isSpace(ch))
                break;
        }
        gint endIndex = 0;
        for (endIndex = len; endIndex > startIndex; --endIndex) {
            gchar ch = nextChar(value, endIndex - 1);
            if (!Character::isSpace(ch))
                break;
        }
        String str;
        str.len = endIndex - startIndex;
        str.value = generate(str.len);
        arraycopy(value, startIndex, str.value, 0, str.len);
        return str;
    }

    String String::stripLeading() const {
        gint startIndex = 0;
        for (startIndex = 0; startIndex < len; ++startIndex) {
            gchar ch = nextChar(value, startIndex);
            if (!Character::isSpace(ch))
                break;
        }
        gint endIndex = len;
        String str;
        str.len = endIndex - startIndex;
        str.value = generate(str.len);
        arraycopy(value, startIndex, str.value, 0, str.len);
        return str;
    }

    String String::stripTrailing() const {
        gint startIndex = 0;
        gint endIndex = 0;
        for (endIndex = len; endIndex > startIndex; --endIndex) {
            gchar ch = nextChar(value, endIndex - 1);
            if (!Character::isSpace(ch))
                break;
        }
        String str;
        str.len = endIndex - startIndex;
        str.value = generate(str.len);
        arraycopy(value, startIndex, str.value, 0, str.len);
        return str;
    }

    gbool String::isBlank() const {
        for (gint i = 0; i < len; ++i) {
            gchar ch = nextChar(value, i);
            if (!Character::isSpace(ch))
                return false;
        }
        return true;
    }

    String String::translateEscape() const {
        String str;
        str.len = len;
        str.value = generate(len);
        gint j = 0;
        for (gint i = 0; i < len; ++i) {
            gchar ch = nextChar(value, i);
            if (ch == '\\') {
                ch = nextChar(value, i += 1);
                switch (ch) {
                    case 'a':
                        putChar(str.value, j++, '\a');
                        break;
                    case 'b':
                        putChar(str.value, j++, '\b');
                        break;
                    case 'f':
                        putChar(str.value, j++, '\f');
                        break;
                    case 'n':
                        putChar(str.value, j++, '\n');
                        break;
                    case 'r':
                        putChar(str.value, j++, '\r');
                        break;
                    case 'v':
                        putChar(str.value, j++, '\v');
                        break;
                    case 's':
                        putChar(str.value, j++, ' ');
                        break;
                    case 't':
                        putChar(str.value, j++, '\t');
                        break;
                    case '\'':
                        putChar(str.value, j++, '\'');
                        break;
                    case '\"':
                        putChar(str.value, j++, '\"');
                        break;
                    case '\\':
                        putChar(str.value, j++, '\\');
                        break;
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7': {
                        // octal escape
                        gint lim = Integer::min(i + (ch <= '3' ? 2 : 1), len);
                        gint octalCode = ch - '0';
                        while (i < lim) {
                            i += 1;
                            ch = nextChar(value, i);
                            if (ch < '0' || ch > '7')
                                break;
                            octalCode = octalCode * 8 + (ch - '0');
                        }
                        putChar(value, j++, octalCode);
                        break;
                    }
                    case 'u': {
                        // unicode 16 escape.
                        gint lim = i + 4;
                        gint escape = 0;
                        if (lim >= len) {
                            ArgumentException("Invalid unicode escape on input \"\\" + subString(i) + "\"").throws(
                                    __trace("core.String"));
                        }
                        while (++i < lim) {
                            ch = nextChar(value, i);
                            if (ch >= '0' && ch <= '9')
                                escape = escape * 16 + (ch - '0');
                            else if (ch >= 'a' && ch <= 'z')
                                escape = escape * 16 + (ch - 'a' + 10);
                            else if (ch >= 'A' && ch <= 'Z')
                                escape = escape * 16 + (ch - 'A' + 10);
                            else
                                break;
                        }
                        if (i < lim || escape > 0xFFFF)
                            ArgumentException("Invalid unicode escape on input \"\\u" + Integer::toHexString(escape) +
                                              "\"").throws(__trace("core.String"));
                        putChar(value, j++, escape);
                        break;
                    }
                    case 'U': {
                        // unicode 32 escape.
                        gint lim = i + 8;
                        gint escape = 0;
                        if (lim >= len) {
                            ArgumentException("Invalid unicode escape on input \"\\" + subString(i) + "\"").throws(
                                    __trace("core.String"));
                        }
                        while (++i < lim) {
                            ch = nextChar(value, i);
                            if (ch >= '0' && ch <= '9')
                                escape = escape * 16 + (ch - '0');
                            else if (ch >= 'a' && ch <= 'z')
                                escape = escape * 16 + (ch - 'a' + 10);
                            else if (ch >= 'A' && ch <= 'Z')
                                escape = escape * 16 + (ch - 'A' + 10);
                            else
                                break;
                        }
                        if (i < lim || escape > 0x10FFFF || escape < 0)
                            ArgumentException("Invalid unicode escape on input \"\\U" + Integer::toHexString(escape) +
                                              "\"").throws(__trace("core.String"));
                        putChar(value, j++, escape);
                        j += 1;
                        break;
                    }
                    case 'x':
                    case 'X': {
                        // hex escape
                        glong escape = 0;
                        if (i + 2 >= len)
                            ArgumentException("Invalid hex escape on input \"\\x" + subString(i) + "\"").throws(
                                    __trace("core.String"));
                        gint startIndex = i;
                        while (++i < len && escape <= 0x10FFFF) {
                            ch = nextChar(value, i);
                            if (ch >= '0' && ch <= '9')
                                escape = escape * 16 + (ch - '0');
                            else if (ch >= 'a' && ch <= 'z')
                                escape = escape * 16 + (ch - 'a' + 10);
                            else if (ch >= 'A' && ch <= 'Z')
                                escape = escape * 16 + (ch - 'A' + 10);
                            else
                                break;
                            i += 1;
                        }
                        if (i - startIndex < 2 || i - startIndex > 8 || escape > 0x10FFFF)
                            ArgumentException("Invalid hex escape on input \"\\x"
                                              + subString(startIndex, i)
                                              + "\"").throws(
                                    __trace("core.String"));
                        putChar(value, j++, (gint) escape);
                        if (escape > Character::MAX_VALUE)
                            j += 1;
                        break;
                    }
                    default:
                        ArgumentException("Invalid escape character \\" + String::valueOf(ch)).throws(
                                __trace("core.String"));
                }
            }
        }
        str.len = j;
        putChar(str.value, j, 0);
        return str;
    }

    String String::toString() const {
        return *this;
    }

    String String::repeat(gint count) const {
        if (count < 0)
            ArgumentException("Negative count").throws(__trace("core.String"));
        if (count == 0 || len == 0)
            return "";
        if (Integer::MAX_VALUE / count < len)
            MemoryError("Overflow: String length required exceed limit").throws(__trace("core.String"));
        String str;
        str.len = count * len;
        str.value = generate(str.len);
        for (gint i = 0; i < count; ++i)
            arraycopy(value, 0, str.value, i * len, len);
        return str;
    }

    gbool String::isASCII() const {
        for (gint i = 0; i < len; ++i) {
            gchar ch = nextChar(value, i);
            if (ch >= 0x80)
                return false;
        }
        return true;
    }

    gbool String::isLatin1() const {
        for (gint i = 0; i < len; ++i) {
            gchar ch = nextChar(value, i);
            if (ch >= 0x100)
                return false;
        }
        return true;
    }

    gbool String::isLowerCase() const {
        for (gint i = 0; i < len; ++i) {
            gchar ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint cp = Character::joinSurrogates(ch, ch2);
                    if (Character::isLetter(cp) && !Character::isLowerCase(cp))
                        return false;
                    i += 1;
                    continue;
                }
            }
            if (Character::isLetter(ch) && !Character::isLowerCase(ch))
                return false;
        }
        return true;
    }

    gbool String::isUpperCase() const {
        for (gint i = 0; i < len; ++i) {
            gchar ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint cp = Character::joinSurrogates(ch, ch2);
                    if (Character::isLetter(cp) && !Character::isUpperCase(cp))
                        return false;
                    i += 1;
                    continue;
                }
            }
            if (Character::isLetter(ch) && !Character::isUpperCase(ch))
                return false;
        }
        return true;
    }

    gbool String::isTitleCase() const {
        for (gint i = 0; i < len; ++i) {
            gchar ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint cp = Character::joinSurrogates(ch, ch2);
                    if (Character::isLetter(cp) && !Character::isTitleCase(cp))
                        return false;
                    i += 1;
                    continue;
                }
            }
            if (Character::isLetter(ch) && !Character::isTitleCase(ch))
                return false;
        }
        return true;
    }

    gint String::count(const String &str) const {
        if (str.len > len || str.isEmpty())
            return 0;
        gint cnt = 0;
        for (gint i = 0; i < len; ++i) {
            gbool isFound = true;
            for (gint j = 0; j < str.len; ++j) {
                gchar ch1 = nextChar(value, i + j);
                gchar ch2 = nextChar(str.value, j);
                if (ch1 != ch2) {
                    isFound = false;
                    break;
                }
            }
            if (isFound) {
                cnt += 1;
            }
        }
        return cnt;
    }

    Object &String::clone() const {
        return U.createInstance<String>(*this);
    }

    String operator+(const String &x, const String &y) {
        return x.concat(y);
    }

    String String::toPascalCase() const {
        String str;
        str.len = len;
        str.value = generate(str.len);
        gbool start = true;
        for (gint i = 0; i < len; ++i) {
            gchar ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint codePoint = Character::joinSurrogates(ch, ch2);
                    gint pascalCase = 0;
                    if (Character::isSpace(codePoint)) {
                        pascalCase = codePoint;
                        start = true;
                    } else if (start) {
                        pascalCase = Character::toUpperCase(codePoint);
                        start = false;
                    } else {
                        pascalCase = Character::toLowerCase(codePoint);
                    }
                    putChar(str.value, i, pascalCase);
                    if (Character::isSupplementary(pascalCase))
                        i += 1;
                }
            } else {
                gchar pascalCase = 0;
                if (Character::isSpace(ch)) {
                    pascalCase = ch;
                    start = true;
                } else if (start) {
                    pascalCase = Character::toUpperCase(ch);
                    start = false;
                } else {
                    pascalCase = Character::toLowerCase(ch);
                }
                putChar(str.value, i, pascalCase);
            }
        }
        return str;
    }

    String String::toCamelCase() const {
        String str;
        str.len = len;
        str.value = generate(str.len);
        gbool start = false;
        gbool start0 = true;
        for (gint i = 0; i < len; ++i) {
            gchar ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint codePoint = Character::joinSurrogates(ch, ch2);
                    gint camelCase = 0;
                    if (Character::isSpace(codePoint)) {
                        camelCase = codePoint;
                        start = !start0;
                    } else if (start) {
                        camelCase = Character::toUpperCase(codePoint);
                        start = false;
                    } else {
                        camelCase = Character::toLowerCase(codePoint);
                        start0 = false;
                    }
                    putChar(str.value, i, camelCase);
                    if (Character::isSupplementary(camelCase))
                        i += 1;
                }
            } else {
                gchar camelCase = 0;
                if (Character::isSpace(ch)) {
                    camelCase = ch;
                    start = !start0;
                } else if (start) {
                    camelCase = Character::toUpperCase(ch);
                    start = false;
                } else {
                    camelCase = Character::toLowerCase(ch);
                    start0 = false;
                }
                putChar(str.value, i, camelCase);
            }
        }
        return str;
    }

    String &String::operator+=(const String &str) {
        return (*this) = concat(str);
    }

    void String::chars(gint srcBegin, gint srcEnd, CharArray &dst, gint dstBegin) const {
        try {
            Preconditions::checkIndexFromRange(srcBegin, srcEnd, len);
            Preconditions::checkIndex(dstBegin, dst.length());
            Preconditions::checkIndexFromSize(dstBegin, srcEnd - srcBegin, dst.length());
            for (gint i = srcBegin; i < srcEnd; ++i) {
                dst[i - srcBegin] = nextChar(value, i);
            }
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
    }

    CharArray String::chars() const {
        CharArray array = CharArray(length());
        chars(0, length(), array, 0);
        return array;
    }

    void String::codePoints(gint srcBegin, gint srcEnd, IntArray &dst, gint dstBegin) const {
        try {
            Preconditions::checkIndexFromRange(srcBegin, srcEnd, len);
            Preconditions::checkIndex(dstBegin, dst.length());
            gint count = 0;
            for (gint i = srcBegin; i < srcEnd;) {
                gchar ch1 = nextChar(value, i);
                gchar ch2 = nextChar(value, i + 1);
                i += Character::isSurrogatePair(ch1, ch2) ? 2 : 1;
                count += 1;
            }
            Preconditions::checkIndexFromSize(dstBegin, count, dst.length());
            gint j = 0;
            for (gint i = srcBegin; i < srcEnd; ++i) {
                gchar ch1 = nextChar(value, i);
                gchar ch2 = nextChar(value, i + 1);
                if (Character::isSurrogatePair(ch1, ch2)) {
                    dst[j++] = Character::joinSurrogates(ch1, ch2);
                    i += 2;
                } else {
                    dst[j++] = ch1;
                    i += 1;
                }
            }
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
    }

    IntArray String::codePoints() const {
        IntArray array(0);
        try {
            gint count = 0;
            for (gint i = 0; i < len;) {
                gchar ch1 = nextChar(value, i);
                gchar ch2 = nextChar(value, i + 1);
                i += Character::isSurrogatePair(ch1, ch2) ? 2 : 1;
                count += 1;
            }
            array = IntArray(count);
            gint j = 0;
            for (gint i = 0; i < len; ++i) {
                gchar ch1 = nextChar(value, i);
                gchar ch2 = nextChar(value, i + 1);
                if (Character::isSurrogatePair(ch1, ch2)) {
                    array[j++] = Character::joinSurrogates(ch1, ch2);
                    i += 2;
                } else {
                    array[j++] = ch1;
                    i += 1;
                }
            }
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
        return array;
    }

    void String::bytes(gint srcBegin, gint srcEnd, ByteArray &dst, gint dstBegin) const {
        try {
            Preconditions::checkIndexFromRange(srcBegin, srcEnd, len);
            Preconditions::checkIndex(dstBegin, dst.length());
            Preconditions::checkIndexFromSize(dstBegin, srcEnd - srcBegin, dst.length());
            for (gint i = srcBegin; i < srcEnd; ++i) {
                dst[i - srcBegin] = (gbyte) (nextChar(value, i) & 0xFF);
            }
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
    }

    ByteArray String::bytes() const {
        if (length() * 2LL > Integer::MAX_VALUE)
            MemoryError("Out of memory").throws(__trace("core.String"));
        ByteArray array(0);
        try {
            array = ByteArray(len << 1);
            for (gint i = 0; i < len; i += 1) {
                array[2 * i + 0] = value[2 * i + 0];
                array[2 * i + 1] = value[2 * i + 1];
            }
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
        return array;
    }

    String String::valueOf(const Object &o) {
        return o.toString();
    }

    String String::valueOf(gbool b) {
        return b ? "true" : "false";
    }

    String String::valueOf(gchar c) {
        return Character::toString(c);
    }

    String String::valueOfCodePoint(gint c) {
        if (Character::isValidCodePoint(c))
            return Character::toString(c);
        return "?";
    }

    String String::valueOf(gint i) {
        return Integer::toString(i);
    }

    String String::valueOf(glong l) {
        return Long::toString(l);
    }

    String String::valueOfUnsigned(gint i) {
        if (i > 0)
            return String::valueOf(i);
        return Integer::toUnsignedString(i);
    }

    String String::valueOfUnsigned(glong l) {
        if (l > 0)
            return String::valueOf(l);
        return Long::toUnsignedString(l);
    }

    String String::valueOf(gfloat f) {
        return Float::toString(f);
    }

    String String::valueOf(gdouble d) {
        return Double::toString(d);
    }

    String String::valueOf(const String &str) {
        return str;
    }

    String::String(const CharArray &chars) : String(chars, 0, chars.length()) {}

    String::String(const CharArray &chars, gint offset, gint count) {
        try {
            Preconditions::checkIndexFromSize(offset, count, chars.length());
            value = generate(chars.length());
            len = chars.length();
            for (gint i = 0; i < count; ++i) {
                putChar(value, i, chars[i + offset]);
            }
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
    }

    String::String(const IntArray &codePoints) : String(codePoints, 0, codePoints.length()) {}

    String::String(const IntArray &codePoints, gint offset, gint count) {
        try {
            Preconditions::checkIndexFromSize(offset, count, codePoints.length());
            gint size = 0;
            for (gint i = 0; i < count; ++i) {
                gint ch = codePoints[i + offset];
                size += Character::isValidCodePoint(ch) && Character::isSupplementary(ch) ? 2 : 1;
            }
            value = generate(size);
            len = size;
            gint j = 0;
            for (gint i = 0; i < count; ++i) {
                gint ch = codePoints[i + offset];
                putChar(value, j, ch);
                j += Character::isValidCodePoint(ch) && Character::isSupplementary(ch) ? 2 : 1;
            }
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
    }

    String::~String() {
        if (value != null) {
            U.freeMemory((glong) value);
            value = null;
        }
        len = 0;
        isHashed = false;
        hashcode = 0;
    }

    gint String::length() const {
        return (value != null) && (len > 0) ? len : 0;
    }

} // core