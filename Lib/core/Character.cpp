//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include <core/foreign/UnicodeTable.h>
#include "Character.h"
#include "String.h"

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    using foreign::UnicodeTable;

    gint Character::hash() const {
        return hash(value);
    }

    gint Character::compareTo(const Character &other) const {
        return compare(value, other.value);
    }

    gbool Character::equals(const Object &object) const {
        if (Class<Character>::hasInstance(object)) {
            return value == CORE_CAST(Character &, object).value;
        }
        return false;
    }

    String Character::toString() const {
        return toString(value);
    }

    String Character::toString(gchar ch) {
        return toString((gint) ch);
    }

    String Character::toString(gint codePoint) {
        gchar c[2] = {0, 0};
        gint charCount = 1;
        if (isSupplementary(codePoint)) {
            c[0] = Character::highSurrogate(codePoint);
            c[1] = Character::lowSurrogate(codePoint);
            charCount = 2;
        } else
            c[0] = (gchar) codePoint;
        return String(c, 0, charCount);
    }

    gbool Character::isLowerCase(gchar ch) {
        return isLowerCase((gint) ch);
    }

    gbool Character::isLowerCase(gint codePoint) {
        return category(codePoint) == Category::LOWERCASE_LETTER;
    }

    gbool Character::isUpperCase(gchar ch) {
        return isUpperCase((gint) ch);
    }

    gbool Character::isUpperCase(gint codePoint) {
        return category(codePoint) == Category::UPPERCASE_LETTER;
    }

    gbool Character::isTitleCase(gchar ch) {
        return isTitleCase((gint) ch);
    }

    gbool Character::isTitleCase(gint codePoint) {
        return category(codePoint) == Category::TITLECASE_LETTER;
    }

    gbool Character::isDigit(gchar ch) {
        return isDigit((gint) ch);
    }

    gbool Character::isDigit(gint codePoint) {
        return category(codePoint) == Category::DECIMAL_DIGIT_NUMBER;
    }

    gbool Character::isLetter(gchar ch) {
        return isLetter((gint) ch);
    }

    gbool Character::isLetter(gint codePoint) {
        if (codePoint <= 0x7f)
            return ('A' <= codePoint && codePoint <= 'Z') || ('a' <= codePoint && codePoint <= 'z');
        gint mask = (1 << (gint) Category::LOWERCASE_LETTER) |
                    (1 << (gint) Category::UPPERCASE_LETTER) |
                    (1 << (gint) Category::TITLECASE_LETTER) |
                    (1 << (gint) Category::MODIFIER_LETTER) |
                    (1 << (gint) Category::OTHER_LETTER);
        return ((mask >> (gint) category(codePoint)) & 1) != 0;
    }

    gbool Character::isNumber(gchar ch) {
        return isNumber((gint) ch);
    }

    gbool Character::isNumber(gint codePoint) {
        if (codePoint <= 0x7f)
            return '0' <= codePoint && codePoint <= '9';
        gint mask = (1 << (gint) Category::DECIMAL_DIGIT_NUMBER) |
                    (1 << (gint) Category::LETTER_NUMBER) |
                    (1 << (gint) Category::OTHER_NUMBER);
        return ((mask >> (gint) category(codePoint)) & 1) != 0;
    }

    gbool Character::isLetterOrNumber(gchar ch) {
        return isLetterOrNumber((gint) ch);
    }

    gbool Character::isLetterOrNumber(gint codePoint) {
        if (codePoint <= 0x7f)
            return ('0' <= codePoint && codePoint <= '9') || ('A' <= codePoint && codePoint <= 'Z') ||
                   ('a' <= codePoint && codePoint <= 'z');
        gint mask = (1 << (gint) Category::LOWERCASE_LETTER) |
                    (1 << (gint) Category::UPPERCASE_LETTER) |
                    (1 << (gint) Category::TITLECASE_LETTER) |
                    (1 << (gint) Category::MODIFIER_LETTER) |
                    (1 << (gint) Category::OTHER_LETTER) |
                    (1 << (gint) Category::DECIMAL_DIGIT_NUMBER) |
                    (1 << (gint) Category::LETTER_NUMBER) |
                    (1 << (gint) Category::OTHER_NUMBER);
        return ((mask >> (gint) category(codePoint)) & 1) != 0;
    }

    gbool Character::isMark(gchar ch) {
        return isMark((gint) ch);
    }

    gbool Character::isMark(gint codePoint) {
        gint mask = (1 << (gint) Category::COMBINING_SPACING_MARK) |
                    (1 << (gint) Category::ENCLOSING_MARK) |
                    (1 << (gint) Category::NON_SPACING_MARK);
        return ((mask >> (gint) category(codePoint)) & 1) != 0;
    }

    gbool Character::isPunctuation(gchar ch) {
        return isPunctuation((gint) ch);
    }

    gbool Character::isPunctuation(gint codePoint) {
        gint mask = (1 << (gint) Category::CONNECTOR_PUNCTUATION) |
                    (1 << (gint) Category::DASH_PUNCTUATION) |
                    (1 << (gint) Category::START_PUNCTUATION) |
                    (1 << (gint) Category::END_PUNCTUATION) |
                    (1 << (gint) Category::INITIAL_QUOTE_PUNCTUATION) |
                    (1 << (gint) Category::FINAL_QUOTE_PUNCTUATION) |
                    (1 << (gint) Category::OTHER_PUNCTUATION);
        return ((mask >> (gint) category(codePoint)) & 1) != 0;
    }

    gbool Character::isPrintable(gchar ch) {
        return isPrintable((gint) ch);
    }

    gbool Character::isPrintable(gint codePoint) {
        gint mask = (1 << (gint) Category::FORMAT) |
                    (1 << (gint) Category::CONTROL) |
                    (1 << (gint) Category::SURROGATE) |
                    (1 << (gint) Category::PRIVATE_USE) |
                    (1 << (gint) Category::UNASSIGNED);
        return ((mask >> (gint) category(codePoint)) & 1) == 0;
    }

    gbool Character::isSpace(gchar ch) {
        return isSpace((gint) ch);
    }

    gbool Character::isSpace(gint codePoint) {
        if (codePoint <= 0x7f)
            return (0x9 <= codePoint && codePoint <= 0xd) || (codePoint == 0x20);
        if ((codePoint == 0x85) || (codePoint == 0xa0))
            return true;
        gint mask = (1 << (gint) Category::SPACE_SEPARATOR) |
                    (1 << (gint) Category::PARAGRAPH_SEPARATOR) |
                    (1 << (gint) Category::LINE_SEPARATOR);
        return ((mask >> (gint) category(codePoint)) & 1) != 0;

    }

    gbool Character::isSymbol(gchar ch) {
        return isSymbol((gint) ch);
    }

    gbool Character::isSymbol(gint codePoint) {
        gint mask = (1 << (gint) Category::CURRENCY_SYMBOL) |
                    (1 << (gint) Category::MATH_SYMBOL) |
                    (1 << (gint) Category::MODIFIER_SYMBOL) |
                    (1 << (gint) Category::OTHER_SYMBOL);
        return ((mask >> (gint) category(codePoint)) & 1) != 0;
    }

    gbool Character::isAlphabetic(gint codePoint) {
        if (codePoint <= 0x7f)
            return ('A' <= codePoint && codePoint <= 'Z') || ('a' <= codePoint && codePoint <= 'z');
        gint mask = (1 << (gint) Category::LOWERCASE_LETTER) |
                    (1 << (gint) Category::UPPERCASE_LETTER) |
                    (1 << (gint) Category::TITLECASE_LETTER) |
                    (1 << (gint) Category::MODIFIER_LETTER) |
                    (1 << (gint) Category::OTHER_LETTER) |
                    (1 << (gint) Category::LETTER_NUMBER);
        return ((mask >> (gint) category(codePoint)) & 1) != 0;
    }

    gbool Character::isIdeographic(gint codePoint) {
        UnicodeBlock blk = block(codePoint);
        return blk == UnicodeBlock::CJK_COMPATIBILITY ||
               blk == UnicodeBlock::CJK_COMPATIBILITY_FORMS ||
               blk == UnicodeBlock::CJK_COMPATIBILITY_IDEOGRAPHS ||
               blk == UnicodeBlock::CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT ||
               blk == UnicodeBlock::CJK_RADICALS_SUPPLEMENT ||
               blk == UnicodeBlock::CJK_STROKES ||
               blk == UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS ||
               blk == UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A ||
               blk == UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B ||
               blk == UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C ||
               blk == UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D ||
               blk == UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_E ||
               blk == UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_F ||
               blk == UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_G ||
               blk == UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS_EXTENSION_H ||
               blk == UnicodeBlock::ENCLOSED_CJK_LETTERS_AND_MONTHS;
    }

    gbool Character::isControl(gint codePoint) {
        if (codePoint >= 0 && codePoint < 0x1F || codePoint >= 0x7F && codePoint <= 0x9F)
            return true;
        return category(codePoint) == Category::CONTROL;
    }

    Object &Character::clone() const {
        return U::createInstance<Character>(*this);
    }

    gchar Character::toLowerCase(gchar ch) {
        return toLowerCase((gint) ch);
    }

    static UnicodeTable &table = UnicodeTable::instance;

    gint Character::toLowerCase(gint codePoint) {
        if (!isValidCodePoint(codePoint))
            return codePoint;
        if ('A' <= codePoint && codePoint <= 'Z')
            return codePoint + 32;
        if ('a' <= codePoint && codePoint <= 'z')
            return codePoint;
        gint offset = table.query(codePoint, UnicodeTable::Column::LOWERCASE_OFFSET);
        return codePoint + offset;
    }

    gchar Character::toUpperCase(gchar ch) {
        return toUpperCase((gint) ch);
    }

    gint Character::toUpperCase(gint codePoint) {
        if (!isValidCodePoint(codePoint))
            return codePoint;
        if ('a' <= codePoint && codePoint <= 'z')
            return codePoint - 32;
        if ('A' <= codePoint && codePoint <= 'Z')
            return codePoint;
        if (codePoint <= 0x7F)
            return codePoint;
        gint offset = table.query(codePoint, UnicodeTable::Column::UPPERCASE_OFFSET);
        return codePoint + offset;
    }

    gchar Character::toTitleCase(gchar ch) {
        return toTitleCase((gint) ch);
    }

    gint Character::toTitleCase(gint codePoint) {
        if (!isValidCodePoint(codePoint))
            return codePoint;
        if (codePoint <= 0x7F)
            return codePoint;
        gint offset = table.query(codePoint, UnicodeTable::Column::TITLECASE_OFFSET);
        return codePoint + offset;
    }

    gchar Character::toCaseFold(gchar ch) {
        return toCaseFold((gint) ch);
    }

    gint Character::toCaseFold(gint codePoint) {
        if (!isValidCodePoint(codePoint))
            return codePoint;
        gint offset = table.query(codePoint, UnicodeTable::Column::CASE_FOLD_OFFSET);
        return codePoint + offset;
    }

    gint Character::numericValue(gchar ch) {
        return numericValue((gint) ch);
    }

    gint Character::numericValue(gint codePoint) {
        if (!isValidCodePoint(codePoint))
            return codePoint;
        if ('0' <= codePoint && codePoint <= '9')
            return codePoint - 48;
        if (codePoint <= 0x7F)
            return -1;
        gint num = table.query(codePoint, UnicodeTable::Column::NUMERIC_VALUE);
        return num;
    }

    Character::Category Character::category(gchar ch) {
        return category((gint) ch);
    }

    Character::Category Character::category(gint codePoint) {
        if (!isValidCodePoint(codePoint))
            return Category::UNASSIGNED;
        gint type = table.query(codePoint, UnicodeTable::Column::CATEGORY);
        return (Category) type;
    }

    Character::Directionality Character::directionality(gchar ch) {
        return directionality((gint) ch);
    }

    Character::Directionality Character::directionality(gint codePoint) {
        if (!isValidCodePoint(codePoint))
            return Directionality::UNASSIGNED;
        gint dir = table.query(codePoint, UnicodeTable::Column::DIRECTION);
        return (Directionality) dir;
    }

    Character::CombiningClass Character::combiningClass(gint codePoint) {
        if (!isValidCodePoint(codePoint))
            return CombiningClass::UNDEFINED;
        gint cc = table.query(codePoint, UnicodeTable::Column::COMBINING_CLASS);
        return (CombiningClass) cc;
    }

    gbool Character::isMirrored(gchar ch) {
        return isMirrored((gint) ch);
    }

    gbool Character::isMirrored(gint codePoint) {
        if (!isValidCodePoint(codePoint))
            return false;
        gint offset = table.query(codePoint, UnicodeTable::Column::MIRROR_OFFSET);
        return offset != 0;
    }

    gchar Character::complement(gchar ch) {
        return (gchar) complement((gint) ch);
    }

    gchar Character::complement(gint codePoint) {
        if (!isValidCodePoint(codePoint))
            return codePoint;
        gint offset = table.query(codePoint, UnicodeTable::Column::MIRROR_OFFSET);
        return codePoint + offset;
    }

    Character::UnicodeBlock Character::block(gchar ch) {
        return block((gint) ch);
    }

    Character::UnicodeBlock Character::block(gint codePoint) {
        if (!isValidCodePoint(codePoint))
            return UnicodeBlock::UNASSIGNED;
        gint blk = table.query(codePoint, UnicodeTable::Column::UNICODE_BLOCK);
        return (UnicodeBlock) blk;
    }

    Character::UnicodeScript Character::script(gint codePoint) {
        if (!isValidCodePoint(codePoint))
            return UnicodeScript::UNKNOWN;
        gint script = table.query(codePoint, UnicodeTable::Column::UNICODE_SCRIPT);
        return (UnicodeScript) script;
    }

    gint Character::numericValue(gchar ch, gint base) {
        return numericValue((gint) ch, base);
    }

    gint Character::numericValue(gint codePoint, gint base) {
        if (base < 2 || base > 36)
            return -1;
        gint ch = codePoint;
        gint i = numericValue(ch);
        if (i < 0)
            return i;
        if (i < base && base <= 10)
            return i;
        if (ch >= 'A' && ch <= 'Z') {
            gint d = ch - 'A' + 10;
            if (base > d)
                return d;
            return -1;
        }
        if (ch >= 'a' && ch <= 'z') {
            gint d = ch - 'a' + 10;
            if (base > d)
                return d;
            return -1;
        }
        if (ch >= u'\uff21' && ch <= u'\uff3a') {
            gint d = ch - u'\uff21' + 10;
            if (base > d)
                return d;
            return -1;
        }
        if (ch >= u'\uff41' && ch <= u'\uff5a') {
            gint d = ch - u'\uff41' + 10;
            if (base > d)
                return d;
            return -1;
        }
        return -1;
    }

    String Character::unicodeName(gint codePoint) { return {}; }


    static CORE_FAST gint HANGUL_BASE_S = 0xac00;
    static CORE_FAST gint HANGUL_BASE_L = 0x1100;
    static CORE_FAST gint HANGUL_BASE_V = 0x1161;
    static CORE_FAST gint HANGUL_BASE_T = 0x11a7;
    static CORE_FAST gint HANGUL_COUNT_L = 19;
    static CORE_FAST gint HANGUL_COUNT_V = 21;
    static CORE_FAST gint HANGUL_COUNT_T = 28;
    static CORE_FAST gint HANGUL_COUNT_N = HANGUL_COUNT_V * HANGUL_COUNT_T;
    static CORE_FAST gint HANGUL_COUNT_S = HANGUL_COUNT_L * HANGUL_COUNT_N;

    Character::Decomposition Character::decomposition(gchar ch) {
        return decomposition((gint) ch);
    }

    Character::Decomposition Character::decomposition(gint codePoint) {
        if (codePoint >= HANGUL_BASE_S && codePoint <= HANGUL_BASE_S + HANGUL_COUNT_S)
            return Decomposition::CANONICAL;
        glong dec = table.query(codePoint, UnicodeTable::Column::DECOMPOSITION);
        if (dec == 0)
            return Decomposition::UNDEFINED;
        return (Decomposition) U::getChar(dec);
    }

    String Character::decompose(gchar ch) {
        return decompose((gint) ch);
    }

    String Character::decompose(gint codePoint) {
        if (codePoint >= HANGUL_BASE_S && codePoint <= HANGUL_BASE_S + HANGUL_COUNT_S) {
            gint offset = codePoint - HANGUL_BASE_S;
            gchar ch[3] = {
                    (gchar) (HANGUL_BASE_L + offset / HANGUL_COUNT_N), // L
                    (gchar) (HANGUL_BASE_V + (offset % HANGUL_COUNT_N) / HANGUL_COUNT_T), // V
                    (gchar) (HANGUL_BASE_T + offset % HANGUL_COUNT_T), // T
            };
            return ch;
        }
        glong dec = table.query(codePoint, UnicodeTable::Column::DECOMPOSITION);
        if (dec == 0)
            return {};
        gint length = U::getChar(dec);
        switch (length) {
            case 0:
                return {};
            case 1: {
                gchar ch[1] = {
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE),
                };
                return ch;
            }
            case 2: {
                gchar ch[2] = {
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 2L),
                };
                return ch;
            }
            case 3: {
                gchar ch[3] = {
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 1L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 2L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 3L),
                };
                return ch;
            }
            case 4: {
                gchar ch[4] = {
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 1L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 2L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 3L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 4L),
                };
                return ch;
            }
            case 5: {
                gchar ch[5] = {
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 1L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 2L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 3L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 4L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 5L),
                };
                return ch;
            }
            case 6: {
                gchar ch[6] = {
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 1L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 2L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 3L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 4L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 5L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 6L),
                };
                return ch;
            }
            case 7: {
                gchar ch[7] = {
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 1L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 2L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 3L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 4L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 5L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 6L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 7L),
                };
                return ch;
            }
            case 8: {
                gchar ch[8] = {
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 1L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 2L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 3L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 4L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 5L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 6L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 7L),
                        U::getChar(dec + U::ARRAY_CHAR_INDEX_SCALE * 8L),
                };
                return ch;
            }
            default:
                return String((gchar *) dec, 1, length);
        }
    }

    Character Character::valueOf(gchar c) { return c;}
} // core