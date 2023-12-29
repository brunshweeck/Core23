//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_CHARACTER_H
#define CORE23_CHARACTER_H

#include <core/Comparable.h>

namespace core {

    /**
     * The <b> Character</b> class wraps a value of the native
     * type <b> gchar</b> in an object. An object of class
     * <b> Character</b> contains a single field whose type is
     * <b> gchar</b>.
     * <p>
     * In addition, this class provides a large number of static methods for
     * determining a character's category (lowercase letter, digit, etc.)
     * and for converting characters from uppercase to lowercase and vice
     * versa.
     *
     * <h2><a id="conformance">Unicode Conformance</a></h2>
     * <p>
     * The fields and methods of class <b> Character</b> are defined in terms
     * of character information from the Unicode Standard, specifically the
     * <i>UnicodeData</i> file that is part of the Unicode Character Database.
     * This file specifies properties including name and category for every
     * assigned Unicode code point or character range. The file is available
     * from the Unicode Consortium at
     * <a href="http://www.unicode.org">http://www.unicode.org</a>.
     * <p>
     * Character information is based on the Unicode Standard, version 15.0.
     * <h2><a id="unicode">Unicode Character Representations</a></h2>
     *
     * <p>The <b> gchar</b> array type (and therefore the value that a
     * <b> Character</b> object encapsulates) are based on the
     * original Unicode specification, which defined characters as
     * fixed-width 16-bit entities. The Unicode Standard has since been
     * changed to allow for characters whose representation requires more
     * than 16 bits.  The range of legal <em>code point</em>s is now
     * U+0000 to U+10FFFF, known as <em>Unicode scalar value</em>.
     * (Refer to the <a
     * href="http://www.unicode.org/reports/tr27/#notation"><i>
     * definition</i></a> of the U+<i>n</i> notation in the Unicode
     * Standard.)
     *
     * <p><a id="BMP">The set of characters from U+0000 to U+FFFF</a> is
     * sometimes referred to as the <em>Basic Multilingual Plane (BMP)</em>.
     * <a id="supplementary">Characters</a> whose code points are greater
     * than U+FFFF are called <em>supplementary character</em>s.  The
     * platform uses the UTF-16 representation in <b> gchar</b> arrays and
     * in the <b> String</b> and <b> StringBuffer</b> classes. In
     * this representation, supplementary characters are represented as a pair
     * of <b> gchar</b> values, the first from the <em>high-surrogates</em>
     * range, (&#92;uD800-&#92;uDBFF), the second from the
     * <em>low-surrogates</em> range (&#92;uDC00-&#92;uDFFF).
     *
     * <p>A <b> gchar</b> value, therefore, represents Basic
     * Multilingual Plane (BMP) code points, including the surrogate
     * code points, or code units of the UTF-16 encoding. An
     * <b> gint</b> value represents all Unicode code points,
     * including supplementary code points. The lower (least significant)
     * 21 bits of <b> gint</b> are used to represent Unicode code
     * points and the upper (most significant) 11 bits must be zero.
     * Unless otherwise specified, the behavior with respect to
     * supplementary characters and surrogate <b> gchar</b> values is
     * as follows:
     *
     * <ul>
     * <li>The methods that only accept a <b> gchar</b> value cannot support
     * supplementary characters. They treat <b> gchar</b> values from the
     * surrogate ranges as undefined characters. For example,
     * <b> Character.isLetter('&#92;uD840')</b> returns <b> false</b>, even though
     * this specific value if followed by any low-surrogate value in a string
     * would represent a letter.
     *
     * <li>The methods that accept an <b> gint</b> value support all
     * Unicode characters, including supplementary characters. For
     * example, <b> Character.isLetter(0x2F81A)</b> returns
     * <b> true</b> because the code point value represents a letter
     * (a CJK ideograph).
     * </ul>
     *
     * <p>In the API documentation, <em>Unicode code point</em> is
     * used for character values in the range between U+0000 and U+10FFFF,
     * and <em>Unicode code unit</em> is used for 16-bit
     * <b> gchar</b> values that are code units of the <em>UTF-16</em>
     * encoding. For more information on Unicode terminology, refer to the
     * <a href="http://www.unicode.org/glossary/">Unicode Glossary</a>.
     *
     * <p>This is a <a href="">value-based</a>
     * class; programmers should treat instances that are
     * <b style="color: green"> equal</b> as interchangeable and should not
     * use instances for synchronization, or unpredictable behavior may
     * occur. For example, in a future release, synchronization may fail.
     *
     * @author  Brunshweeck Tazeussong
     */
    class Character CORE_FINAL : public Object, public Comparable<Character> {
    private:
        /**
         * The value of Character
         */
        gchar value;

    public:

        /**
         * The constant value of this field is the smallest value of type
         * <b> gchar</b>, <b> '&#92;u0000'</b>.
         */
        static CORE_FAST gchar MIN_VALUE = u'\u0000';

        /**
         * The constant value of this field is the largest value of type
         * <b> gchar</b>, <b> '&#92;uFFFF'</b>.
         */
        static CORE_FAST gchar MAX_VALUE = u'\uFFFF';

        /**
         * The minimum value of a
         * <a href="http://www.unicode.org/glossary/#high_surrogate_code_unit">
         * Unicode high-surrogate code unit</a>
         * in the UTF-16 encoding, constant <b> '&#92;uD800'</b>.
         * A high-surrogate is also known as a <i>leading-surrogate</i>.
         */
        static CORE_FAST gchar MIN_HIGH_SURROGATE = u'\xD800';

        /**
         * The maximum value of a
         * <a href="http://www.unicode.org/glossary/#high_surrogate_code_unit">
         * Unicode high-surrogate code unit</a>
         * in the UTF-16 encoding, constant <b> '&#92;uDBFF'</b>.
         * A high-surrogate is also known as a <i>leading-surrogate</i>.
         */
        static CORE_FAST gchar MAX_HIGH_SURROGATE = u'\xDBFF';

        /**
         * The minimum value of a
         * <a href="http://www.unicode.org/glossary/#low_surrogate_code_unit">
         * Unicode low-surrogate code unit</a>
         * in the UTF-16 encoding, constant <b> '&#92;uDC00'</b>.
         * A low-surrogate is also known as a <i>trailing-surrogate</i>.
         */
        static CORE_FAST gchar MIN_LOW_SURROGATE = u'\xDC00';

        /**
         * The maximum value of a
         * <a href="http://www.unicode.org/glossary/#low_surrogate_code_unit">
         * Unicode low-surrogate code unit</a>
         * in the UTF-16 encoding, constant <b> '&#92;uDFFF'</b>.
         * A low-surrogate is also known as a <i>trailing-surrogate</i>.
         */
        static CORE_FAST gchar MAX_LOW_SURROGATE = u'\xDFFF';

        /**
         * The minimum value of a Unicode surrogate code unit in the
         * UTF-16 encoding, constant <b> '&#92;uD800'</b>.
         */
        static CORE_FAST gchar MIN_SURROGATE = MIN_HIGH_SURROGATE;

        /**
         * The maximum value of a Unicode surrogate code unit in the
         * UTF-16 encoding, constant <b> '&#92;uDFFF'</b>.
         */
        static CORE_FAST gchar MAX_SURROGATE = MAX_LOW_SURROGATE;

        /**
         * The minimum value of a
         * <a href="http://www.unicode.org/glossary/#supplementary_code_point">
         * Unicode supplementary code point</a>, constant <b> U+10000</b>.
         */
        static CORE_FAST gint MIN_SUPPLEMENTARY = 0x010000;

        /**
         * The minimum value of a
         * <a href="http://www.unicode.org/glossary/#code_point">
         * Unicode code point</a>, constant <b> U+0000</b>.
         */
        static CORE_FAST gint MIN_CODEPOINT = 0x000000;

        /**
         * The maximum value of a
         * <a href="http://www.unicode.org/glossary/#code_point">
         * Unicode code point</a>, constant <b> U+10FFFF</b>.
         */
        static CORE_FAST gint MAX_CODEPOINT = 0x10FFFF;

        /**
         *  General character categories
         */
        enum class Category: gbyte {
            /**
            * General category "Cn" in the Unicode specification.
            *
            */
            UNASSIGNED = 13,

            /**
            * General category "Lu" in the Unicode specification.
            *
            */
            UPPERCASE_LETTER = 14,

            /**
            * General category "Ll" in the Unicode specification.
            *
            */
            LOWERCASE_LETTER = 15,

            /**
            * General category "Lt" in the Unicode specification.
            *
            */
            TITLECASE_LETTER = 16,

            /**
            * General category "Lm" in the Unicode specification.
            *
            */
            MODIFIER_LETTER = 17,

            /**
            * General category "Lo" in the Unicode specification.
            *
            */
            OTHER_LETTER = 18,

            /**
            * General category "Mn" in the Unicode specification.
            *
            */
            NON_SPACING_MARK = 0,

            /**
            * General category "Me" in the Unicode specification.
            *
            */
            ENCLOSING_MARK = 2,

            /**
            * General category "Mc" in the Unicode specification.
            *
            */
            COMBINING_SPACING_MARK = 1,

            /**
            * General category "Nd" in the Unicode specification.
            *
            */
            DECIMAL_DIGIT_NUMBER = 3,

            /**
            * General category "Nl" in the Unicode specification.
            *
            */
            LETTER_NUMBER = 4,

            /**
            * General category "No" in the Unicode specification.
            *
            */
            OTHER_NUMBER = 5,

            /**
            * General category "Zs" in the Unicode specification.
            *
            */
            SPACE_SEPARATOR = 6,

            /**
            * General category "Zl" in the Unicode specification.
            *
            */
            LINE_SEPARATOR = 7,

            /**
            * General category "Zp" in the Unicode specification.
            *
            */
            PARAGRAPH_SEPARATOR = 8,

            /**
            * General category "Cc" in the Unicode specification.
            *
            */
            CONTROL = 9,

            /**
            * General category "Cf" in the Unicode specification.
            *
            */
            FORMAT = 10,

            /**
            * General category "Co" in the Unicode specification.
            *
            */
            PRIVATE_USE = 12,

            /**
            * General category "Cs" in the Unicode specification.
            *
            */
            SURROGATE = 11,

            /**
            * General category "Pd" in the Unicode specification.
            *
            */
            DASH_PUNCTUATION = 20,

            /**
            * General category "Ps" in the Unicode specification.
            *
            */
            START_PUNCTUATION = 21,

            /**
            * General category "Pe" in the Unicode specification.
            *
            */
            END_PUNCTUATION = 22,

            /**
            * General category "Pc" in the Unicode specification.
            *
            */
            CONNECTOR_PUNCTUATION = 19,

            /**
            * General category "Po" in the Unicode specification.
            *
            */
            OTHER_PUNCTUATION = 25,

            /**
            * General category "Sm" in the Unicode specification.
            *
            */
            MATH_SYMBOL = 26,

            /**
            * General category "Sc" in the Unicode specification.
            *
            */
            CURRENCY_SYMBOL = 27,

            /**
            * General category "Sk" in the Unicode specification.
            *
            */
            MODIFIER_SYMBOL = 28,

            /**
            * General category "So" in the Unicode specification.
            *
            */
            OTHER_SYMBOL = 29,

            /**
            * General category "Pi" in the Unicode specification.
            *
            */
            INITIAL_QUOTE_PUNCTUATION = 23,

            /**
            * General category "Pf" in the Unicode specification.
            *
            */
            FINAL_QUOTE_PUNCTUATION = 24
        };


        /**
         *  Directionality of character
         */
        enum class Directionality: gbyte {
            /**
            * Undefined Directionality character type. Undefined character
            * values have undefined directionality in the Unicode specification.
            */
            UNASSIGNED = 0,

            /**
            * Strong Directionality character type "L" in the Unicode specification.
            */
            LEFT_TO_RIGHT,

            /**
            * Strong Directionality character type "T" in the Unicode specification.
            */
            RIGHT_TO_LEFT,

            /**
            * Strong Directionality character type "AL" in the Unicode specification.
            */
            RIGHT_TO_LEFT_ARABIC,

            /**
            * Weak Directionality character type "EN" in the Unicode specification.
            */
            EUROPEAN_NUMBER,

            /**
            * Weak Directionality character type "ES" in the Unicode specification.
            */
            EUROPEAN_NUMBER_SEPARATOR,

            /**
            * Weak Directionality character type "ET" in the Unicode specification.
            */
            EUROPEAN_NUMBER_TERMINATOR,

            /**
            * Weak Directionality character type "AN" in the Unicode specification.
            */
            ARABIC_NUMBER,

            /**
            * Weak Directionality character type "CS" in the Unicode specification.
            */
            COMMON_NUMBER_SEPARATOR,

            /**
            * Weak Directionality character type "NSM" in the Unicode specification.
            */
            NON_SPACING_MARK,

            /**
            * Weak Directionality character type "BN" in the Unicode specification.
            */
            BOUNDARY_NEUTRAL,

            /**
            * Neutral Directionality character type "B" in the Unicode specification.
            */
            PARAGRAPH_SEPARATOR,

            /**
            * Neutral Directionality character type "S" in the Unicode specification.
            */
            SEGMENT_SEPARATOR,

            /**
            * Neutral Directionality character type "WS" in the Unicode specification.
            */
            WHITESPACE,

            /**
            * Neutral Directionality character type "ON" in the Unicode specification.
            */
            OTHER_NEUTRALS,

            /**
            * Strong Directionality character type "LRE" in the Unicode specification.
            */
            LEFT_TO_RIGHT_EMBEDDING,

            /**
            * Strong Directionality character type "LRO" in the Unicode specification.
            */
            LEFT_TO_RIGHT_OVERRIDE,

            /**
            * Strong Directionality character type "RLE" in the Unicode specification.
            */
            RIGHT_TO_LEFT_EMBEDDING,

            /**
            * Strong Directionality character type "RLO" in the Unicode specification.
            */
            RIGHT_TO_LEFT_OVERRIDE,

            /**
            * Weak Directionality character type "PDF" in the Unicode specification.
            */
            POP_DIRECTIONAL_FORMAT,

            /**
            * Weak Directionality character type "LRI" in the Unicode specification.
            */
            LEFT_TO_RIGHT_ISOLATE,

            /**
            * Weak Directionality character type "RLI" in the Unicode specification.
            */
            RIGHT_TO_LEFT_ISOLATE,

            /**
            * Weak Directionality character type "FSI" in the Unicode specification.
            */
            FIRST_STRONG_ISOLATE,

            /**
            * Weak Directionality character type "PDI" in the Unicode specification.
            */
            POP_DIRECTIONAL_ISOLATE,
        };


        /**
         * A family of character subsets representing the character scripts
         * defined in the <a href="http://www.unicode.org/reports/tr24/">
         * <i>Unicode Standard Annex #24: Script Names</i></a>. Every Unicode
         * character is assigned to a single Unicode script, either a specific
         * script, such as <b style="color: orange"> Latin</b>, or
         * one of the following three special values,
         * <b style="color: orange"> Inherited</b>,
         * <b style="color: orange"> Common</b> or
         * <b style="color: orange"> Unknown</b>.
         */
        enum class UnicodeScript: native::GENERIC_UINT8 {

            /**
             * Unicode script "Common".
             */
            COMMON,

            /**
             * Unicode script "Latin".
             */
            LATIN,

            /**
             * Unicode script "Greek".
             */
            GREEK,

            /**
             * Unicode script "Cyrillic".
             */
            CYRILLIC,

            /**
             * Unicode script "Armenian".
             */
            ARMENIAN,

            /**
             * Unicode script "Hebrew".
             */
            HEBREW,

            /**
             * Unicode script "Arabic".
             */
            ARABIC,

            /**
             * Unicode script "Syriac".
             */
            SYRIAC,

            /**
             * Unicode script "Thaana".
             */
            THAANA,

            /**
             * Unicode script "Devanagari".
             */
            DEVANAGARI,

            /**
             * Unicode script "Bengali".
             */
            BENGALI,

            /**
             * Unicode script "Gurmukhi".
             */
            GURMUKHI,

            /**
             * Unicode script "Gujarati".
             */
            GUJARATI,

            /**
             * Unicode script "Oriya".
             */
            ORIYA,

            /**
             * Unicode script "Tamil".
             */
            TAMIL,

            /**
             * Unicode script "Telugu".
             */
            TELUGU,

            /**
             * Unicode script "Kannada".
             */
            KANNADA,

            /**
             * Unicode script "Malayalam".
             */
            MALAYALAM,

            /**
             * Unicode script "Sinhala".
             */
            SINHALA,

            /**
             * Unicode script "Thai".
             */
            THAI,

            /**
             * Unicode script "Lao".
             */
            LAO,

            /**
             * Unicode script "Tibetan".
             */
            TIBETAN,

            /**
             * Unicode script "Myanmar".
             */
            MYANMAR,

            /**
             * Unicode script "Georgian".
             */
            GEORGIAN,

            /**
             * Unicode script "Hangul".
             */
            HANGUL,

            /**
             * Unicode script "Ethiopic".
             */
            ETHIOPIC,

            /**
             * Unicode script "Cherokee".
             */
            CHEROKEE,

            /**
             * Unicode script "Canadian_Aboriginal".
             */
            CANADIAN_ABORIGINAL,

            /**
             * Unicode script "Ogham".
             */
            OGHAM,

            /**
             * Unicode script "Runic".
             */
            RUNIC,

            /**
             * Unicode script "Khmer".
             */
            KHMER,

            /**
             * Unicode script "Mongolian".
             */
            MONGOLIAN,

            /**
             * Unicode script "Hiragana".
             */
            HIRAGANA,

            /**
             * Unicode script "Katakana".
             */
            KATAKANA,

            /**
             * Unicode script "Bopomofo".
             */
            BOPOMOFO,

            /**
             * Unicode script "Han".
             */
            HAN,

            /**
             * Unicode script "Yi".
             */
            YI,

            /**
             * Unicode script "Old_Italic".
             */
            OLD_ITALIC,

            /**
             * Unicode script "Gothic".
             */
            GOTHIC,

            /**
             * Unicode script "Deseret".
             */
            DESERET,

            /**
             * Unicode script "Inherited".
             */
            INHERITED,

            /**
             * Unicode script "Tagalog".
             */
            TAGALOG,

            /**
             * Unicode script "Hanunoo".
             */
            HANUNOO,

            /**
             * Unicode script "Buhid".
             */
            BUHID,

            /**
             * Unicode script "Tagbanwa".
             */
            TAGBANWA,

            /**
             * Unicode script "Limbu".
             */
            LIMBU,

            /**
             * Unicode script "Tai_Le".
             */
            TAI_LE,

            /**
             * Unicode script "Linear_B".
             */
            LINEAR_B,

            /**
             * Unicode script "Ugaritic".
             */
            UGARITIC,

            /**
             * Unicode script "Shavian".
             */
            SHAVIAN,

            /**
             * Unicode script "Osmanya".
             */
            OSMANYA,

            /**
             * Unicode script "Cypriot".
             */
            CYPRIOT,

            /**
             * Unicode script "Braille".
             */
            BRAILLE,

            /**
             * Unicode script "Buginese".
             */
            BUGINESE,

            /**
             * Unicode script "Coptic".
             */
            COPTIC,

            /**
             * Unicode script "New_Tai_Lue".
             */
            NEW_TAI_LUE,

            /**
             * Unicode script "Glagolitic".
             */
            GLAGOLITIC,

            /**
             * Unicode script "Tifinagh".
             */
            TIFINAGH,

            /**
             * Unicode script "Syloti_Nagri".
             */
            SYLOTI_NAGRI,

            /**
             * Unicode script "Old_Persian".
             */
            OLD_PERSIAN,

            /**
             * Unicode script "Kharoshthi".
             */
            KHAROSHTHI,

            /**
             * Unicode script "Balinese".
             */
            BALINESE,

            /**
             * Unicode script "Cuneiform".
             */
            CUNEIFORM,

            /**
             * Unicode script "Phoenician".
             */
            PHOENICIAN,

            /**
             * Unicode script "Phags_Pa".
             */
            PHAGS_PA,

            /**
             * Unicode script "Nko".
             */
            NKO,

            /**
             * Unicode script "Sundanese".
             */
            SUNDANESE,

            /**
             * Unicode script "Batak".
             */
            BATAK,

            /**
             * Unicode script "Lepcha".
             */
            LEPCHA,

            /**
             * Unicode script "Ol_Chiki".
             */
            OL_CHIKI,

            /**
             * Unicode script "Vai".
             */
            VAI,

            /**
             * Unicode script "Saurashtra".
             */
            SAURASHTRA,

            /**
             * Unicode script "Kayah_Li".
             */
            KAYAH_LI,

            /**
             * Unicode script "Rejang".
             */
            REJANG,

            /**
             * Unicode script "Lycian".
             */
            LYCIAN,

            /**
             * Unicode script "Carian".
             */
            CARIAN,

            /**
             * Unicode script "Lydian".
             */
            LYDIAN,

            /**
             * Unicode script "Cham".
             */
            CHAM,

            /**
             * Unicode script "Tai_Tham".
             */
            TAI_THAM,

            /**
             * Unicode script "Tai_Viet".
             */
            TAI_VIET,

            /**
             * Unicode script "Avestan".
             */
            AVESTAN,

            /**
             * Unicode script "Egyptian_Hieroglyphs".
             */
            EGYPTIAN_HIEROGLYPHS,

            /**
             * Unicode script "Samaritan".
             */
            SAMARITAN,

            /**
             * Unicode script "Mandaic".
             */
            MANDAIC,

            /**
             * Unicode script "Lisu".
             */
            LISU,

            /**
             * Unicode script "Bamum".
             */
            BAMUM,

            /**
             * Unicode script "Javanese".
             */
            JAVANESE,

            /**
             * Unicode script "Meetei_Mayek".
             */
            MEETEI_MAYEK,

            /**
             * Unicode script "Imperial_Aramaic".
             */
            IMPERIAL_ARAMAIC,

            /**
             * Unicode script "Old_South_Arabian".
             */
            OLD_SOUTH_ARABIAN,

            /**
             * Unicode script "Inscriptional_Parthian".
             */
            INSCRIPTIONAL_PARTHIAN,

            /**
             * Unicode script "Inscriptional_Pahlavi".
             */
            INSCRIPTIONAL_PAHLAVI,

            /**
             * Unicode script "Old_Turkic".
             */
            OLD_TURKIC,

            /**
             * Unicode script "Brahmi".
             */
            BRAHMI,

            /**
             * Unicode script "Kaithi".
             */
            KAITHI,

            /**
             * Unicode script "Meroitic Hieroglyphs".
             *
             */
            MEROITIC_HIEROGLYPHS,

            /**
             * Unicode script "Meroitic Cursive".
             *
             */
            MEROITIC_CURSIVE,

            /**
             * Unicode script "Sora Sompeng".
             *
             */
            SORA_SOMPENG,

            /**
             * Unicode script "Chakma".
             *
             */
            CHAKMA,

            /**
             * Unicode script "Sharada".
             *
             */
            SHARADA,

            /**
             * Unicode script "Takri".
             *
             */
            TAKRI,

            /**
             * Unicode script "Miao".
             *
             */
            MIAO,

            /**
             * Unicode script "Caucasian Albanian".
             *
             */
            CAUCASIAN_ALBANIAN,

            /**
             * Unicode script "Bassa Vah".
             *
             */
            BASSA_VAH,

            /**
             * Unicode script "Duployan".
             *
             */
            DUPLOYAN,

            /**
             * Unicode script "Elbasan".
             *
             */
            ELBASAN,

            /**
             * Unicode script "Grantha".
             *
             */
            GRANTHA,

            /**
             * Unicode script "Pahawh Hmong".
             *
             */
            PAHAWH_HMONG,

            /**
             * Unicode script "Khojki".
             *
             */
            KHOJKI,

            /**
             * Unicode script "Linear A".
             *
             */
            LINEAR_A,

            /**
             * Unicode script "Mahajani".
             *
             */
            MAHAJANI,

            /**
             * Unicode script "Manichaean".
             *
             */
            MANICHAEAN,

            /**
             * Unicode script "Mende Kikakui".
             *
             */
            MENDE_KIKAKUI,

            /**
             * Unicode script "Modi".
             *
             */
            MODI,

            /**
             * Unicode script "Mro".
             *
             */
            MRO,

            /**
             * Unicode script "Old North Arabian".
             *
             */
            OLD_NORTH_ARABIAN,

            /**
             * Unicode script "Nabataean".
             *
             */
            NABATAEAN,

            /**
             * Unicode script "Palmyrene".
             *
             */
            PALMYRENE,

            /**
             * Unicode script "Pau Cin Hau".
             *
             */
            PAU_CIN_HAU,

            /**
             * Unicode script "Old Permic".
             *
             */
            OLD_PERMIC,

            /**
             * Unicode script "Psalter Pahlavi".
             *
             */
            PSALTER_PAHLAVI,

            /**
             * Unicode script "Siddham".
             *
             */
            SIDDHAM,

            /**
             * Unicode script "Khudawadi".
             *
             */
            KHUDAWADI,

            /**
             * Unicode script "Tirhuta".
             *
             */
            TIRHUTA,

            /**
             * Unicode script "Warang Citi".
             *
             */
            WARANG_CITI,

            /**
             * Unicode script "Ahom".
             *
             */
            AHOM,

            /**
             * Unicode script "Anatolian Hieroglyphs".
             *
             */
            ANATOLIAN_HIEROGLYPHS,

            /**
             * Unicode script "Hatran".
             *
             */
            HATRAN,

            /**
             * Unicode script "Multani".
             *
             */
            MULTANI,

            /**
             * Unicode script "Old Hungarian".
             *
             */
            OLD_HUNGARIAN,

            /**
             * Unicode script "SignWriting".
             *
             */
            SIGNWRITING,

            /**
             * Unicode script "Adlam".
             *
             */
            ADLAM,

            /**
             * Unicode script "Bhaiksuki".
             *
             */
            BHAIKSUKI,

            /**
             * Unicode script "Marchen".
             *
             */
            MARCHEN,

            /**
             * Unicode script "Newa".
             *
             */
            NEWA,

            /**
             * Unicode script "Osage".
             *
             */
            OSAGE,

            /**
             * Unicode script "Tangut".
             *
             */
            TANGUT,

            /**
             * Unicode script "Masaram Gondi".
             *
             */
            MASARAM_GONDI,

            /**
             * Unicode script "Nushu".
             *
             */
            NUSHU,

            /**
             * Unicode script "Soyombo".
             *
             */
            SOYOMBO,

            /**
             * Unicode script "Zanabazar Square".
             *
             */
            ZANABAZAR_SQUARE,

            /**
             * Unicode script "Hanifi Rohingya".
             *
             */
            HANIFI_ROHINGYA,

            /**
             * Unicode script "Old Sogdian".
             *
             */
            OLD_SOGDIAN,

            /**
             * Unicode script "Sogdian".
             *
             */
            SOGDIAN,

            /**
             * Unicode script "Dogra".
             *
             */
            DOGRA,

            /**
             * Unicode script "Gunjala Gondi".
             *
             */
            GUNJALA_GONDI,

            /**
             * Unicode script "Makasar".
             *
             */
            MAKASAR,

            /**
             * Unicode script "Medefaidrin".
             *
             */
            MEDEFAIDRIN,

            /**
             * Unicode script "Elymaic".
             *
             */
            ELYMAIC,

            /**
             * Unicode script "Nandinagari".
             *
             */
            NANDINAGARI,

            /**
             * Unicode script "Nyiakeng Puachue Hmong".
             *
             */
            NYIAKENG_PUACHUE_HMONG,

            /**
             * Unicode script "Wancho".
             *
             */
            WANCHO,

            /**
             * Unicode script "Yezidi".
             *
             */
            YEZIDI,

            /**
             * Unicode script "Chorasmian".
             *
             */
            CHORASMIAN,

            /**
             * Unicode script "Dives Akuru".
             *
             */
            DIVES_AKURU,

            /**
             * Unicode script "Khitan Small Script".
             *
             */
            KHITAN_SMALL_SCRIPT,

            /**
             * Unicode script "Vithkuqi".
             *
             */
            VITHKUQI,

            /**
             * Unicode script "Old Uyghur".
             *
             */
            OLD_UYGHUR,

            /**
             * Unicode script "Cypro Minoan".
             *
             */
            CYPRO_MINOAN,

            /**
             * Unicode script "Tangsa".
             *
             */
            TANGSA,

            /**
             * Unicode script "Toto".
             *
             */
            TOTO,

            /**
             * Unicode script "Kawi".
             *
             */
            KAWI,

            /**
             * Unicode script "Nag Mundari".
             *
             */
            NAG_MUNDARI,

            /**
             * Unicode script "Unknown".
             */
            UNKNOWN

        };

        // See http://www.unicode.org/Public/UNIDATA/Blocks.txt
        // for the latest specification of Unicode Blocks.

        /**
         * A family of character subsets representing the character blocks in the
         * Unicode specification. Character blocks generally define characters
         * used for a specific script or purpose. A character is contained by
         * at most one Unicode block.
         */
        enum class UnicodeBlock: gshort {

            /**
             * Constant for the "Basic Latin" Unicode character block.
             */
            BASIC_LATIN,

            /**
             * Constant for the "Latin-1 Supplement" Unicode character block.
             */
            LATIN_1_SUPPLEMENT,

            /**
             * Constant for the "Latin Extended-A" Unicode character block.
             */
            LATIN_EXTENDED_A,

            /**
             * Constant for the "Latin Extended-B" Unicode character block.
             */
            LATIN_EXTENDED_B,

            /**
             * Constant for the "IPA Extensions" Unicode character block.
             */
            IPA_EXTENSIONS,

            /**
             * Constant for the "Spacing Modifier Letters" Unicode character block.
             */
            SPACING_MODIFIER_LETTERS,

            /**
             * Constant for the "Combining Diacritical Marks" Unicode character block.
             */
            COMBINING_DIACRITICAL_MARKS,

            /**
             * Constant for the "Greek and Coptic" Unicode character block.
             * <p>
             * This block was previously known as the "Greek" block.
             */
            GREEK,

            /**
             * Constant for the "Cyrillic" Unicode character block.
             */
            CYRILLIC,

            /**
             * Constant for the "Armenian" Unicode character block.
             */
            ARMENIAN,

            /**
             * Constant for the "Hebrew" Unicode character block.
             */
            HEBREW,

            /**
             * Constant for the "Arabic" Unicode character block.
             */
            ARABIC,

            /**
             * Constant for the "Devanagari" Unicode character block.
             */
            DEVANAGARI,

            /**
             * Constant for the "Bengali" Unicode character block.
             */
            BENGALI,

            /**
             * Constant for the "Gurmukhi" Unicode character block.
             */
            GURMUKHI,

            /**
             * Constant for the "Gujarati" Unicode character block.
             */
            GUJARATI,

            /**
             * Constant for the "Oriya" Unicode character block.
             */
            ORIYA,

            /**
             * Constant for the "Tamil" Unicode character block.
             */
            TAMIL,

            /**
             * Constant for the "Telugu" Unicode character block.
             */
            TELUGU,

            /**
             * Constant for the "Kannada" Unicode character block.
             */
            KANNADA,

            /**
             * Constant for the "Malayalam" Unicode character block.
             */
            MALAYALAM,

            /**
             * Constant for the "Thai" Unicode character block.
             */
            THAI,

            /**
             * Constant for the "Lao" Unicode character block.
             */
            LAO,

            /**
             * Constant for the "Tibetan" Unicode character block.
             */
            TIBETAN,

            /**
             * Constant for the "Georgian" Unicode character block.
             */
            GEORGIAN,

            /**
             * Constant for the "Hangul Jamo" Unicode character block.
             */
            HANGUL_JAMO,

            /**
             * Constant for the "Latin Extended Additional" Unicode character block.
             */
            LATIN_EXTENDED_ADDITIONAL,

            /**
             * Constant for the "Greek Extended" Unicode character block.
             */
            GREEK_EXTENDED,

            /**
             * Constant for the "General Punctuation" Unicode character block.
             */
            GENERAL_PUNCTUATION,

            /**
             * Constant for the "Superscripts and Subscripts" Unicode character
             * block.
             */
            SUPERSCRIPTS_AND_SUBSCRIPTS,

            /**
             * Constant for the "Currency Symbols" Unicode character block.
             */
            CURRENCY_SYMBOLS,

            /**
             * Constant for the "Combining Diacritical Marks for Symbols" Unicode
             * character block.
             * <p>
             * This block was previously known as "Combining Marks for Symbols".
             */
            COMBINING_MARKS_FOR_SYMBOLS,

            /**
             * Constant for the "Letterlike Symbols" Unicode character block.
             */
            LETTERLIKE_SYMBOLS,

            /**
             * Constant for the "Number Forms" Unicode character block.
             */
            NUMBER_FORMS,

            /**
             * Constant for the "Arrows" Unicode character block.
             */
            ARROWS,

            /**
             * Constant for the "Mathematical Operators" Unicode character block.
             */
            MATHEMATICAL_OPERATORS,

            /**
             * Constant for the "Miscellaneous Technical" Unicode character block.
             */
            MISCELLANEOUS_TECHNICAL,

            /**
             * Constant for the "Control Pictures" Unicode character block.
             */
            CONTROL_PICTURES,

            /**
             * Constant for the "Optical Character Recognition" Unicode character block.
             */
            OPTICAL_CHARACTER_RECOGNITION,

            /**
             * Constant for the "Enclosed Alphanumerics" Unicode character block.
             */
            ENCLOSED_ALPHANUMERICS,

            /**
             * Constant for the "Box Drawing" Unicode character block.
             */
            BOX_DRAWING,

            /**
             * Constant for the "Block Elements" Unicode character block.
             */
            BLOCK_ELEMENTS,

            /**
             * Constant for the "Geometric Shapes" Unicode character block.
             */
            GEOMETRIC_SHAPES,

            /**
             * Constant for the "Miscellaneous Symbols" Unicode character block.
             */
            MISCELLANEOUS_SYMBOLS,

            /**
             * Constant for the "Dingbats" Unicode character block.
             */
            DINGBATS,

            /**
             * Constant for the "CJK Symbols and Punctuation" Unicode character block.
             */
            CJK_SYMBOLS_AND_PUNCTUATION,

            /**
             * Constant for the "Hiragana" Unicode character block.
             */
            HIRAGANA,

            /**
             * Constant for the "Katakana" Unicode character block.
             */
            KATAKANA,

            /**
             * Constant for the "Bopomofo" Unicode character block.
             */
            BOPOMOFO,

            /**
             * Constant for the "Hangul Compatibility Jamo" Unicode character block.
             */
            HANGUL_COMPATIBILITY_JAMO,

            /**
             * Constant for the "Kanbun" Unicode character block.
             */
            KANBUN,

            /**
             * Constant for the "Enclosed CJK Letters and Months" Unicode character block.
             */
            ENCLOSED_CJK_LETTERS_AND_MONTHS,

            /**
             * Constant for the "CJK Compatibility" Unicode character block.
             */
            CJK_COMPATIBILITY,

            /**
             * Constant for the "CJK Unified Ideographs" Unicode character block.
             */
            CJK_UNIFIED_IDEOGRAPHS,

            /**
             * Constant for the "Hangul Syllables" Unicode character block.
             */
            HANGUL_SYLLABLES,

            /**
             * Constant for the "private Use Area" Unicode character block.
             */
            PRIVATE_USE_AREA,

            /**
             * Constant for the "CJK Compatibility Ideographs" Unicode character
             * block.
             */
            CJK_COMPATIBILITY_IDEOGRAPHS,

            /**
             * Constant for the "Alphabetic Presentation Forms" Unicode character block.
             */
            ALPHABETIC_PRESENTATION_FORMS,

            /**
             * Constant for the "Arabic Presentation Forms-A" Unicode character
             * block.
             */
            ARABIC_PRESENTATION_FORMS_A,

            /**
             * Constant for the "Combining Half Marks" Unicode character block.
             */
            COMBINING_HALF_MARKS,

            /**
             * Constant for the "CJK Compatibility Forms" Unicode character block.
             */
            CJK_COMPATIBILITY_FORMS,

            /**
             * Constant for the "Small Form Variants" Unicode character block.
             */
            SMALL_FORM_VARIANTS,

            /**
             * Constant for the "Arabic Presentation Forms-B" Unicode character block.
             */
            ARABIC_PRESENTATION_FORMS_B,

            /**
             * Constant for the "Halfwidth and Fullwidth Forms" Unicode character
             * block.
             */
            HALFWIDTH_AND_FULLWIDTH_FORMS,

            /**
             * Constant for the "Specials" Unicode character block.
             */
            SPECIALS,

            /**
             * \deprecated
             * Instead of URROGATES_AREA, use HIGH_SURROGATES,
             * HIGH_PRIVATE_USE_SURROGATES, and LOW_SURROGATES.
             * These constants match the block definitions of the Unicode Standard.
             */
            SURROGATES_AREA,

            /**
             * Constant for the "Syriac" Unicode character block.
             */
            SYRIAC,

            /**
             * Constant for the "Thaana" Unicode character block.
             */
            THAANA,

            /**
             * Constant for the "Sinhala" Unicode character block.
             */
            SINHALA,

            /**
             * Constant for the "Myanmar" Unicode character block.
             */
            MYANMAR,

            /**
             * Constant for the "Ethiopic" Unicode character block.
             */
            ETHIOPIC,

            /**
             * Constant for the "Cherokee" Unicode character block.
             */
            CHEROKEE,

            /**
             * Constant for the "Unified Canadian Aboriginal Syllabics" Unicode character block.
             */
            UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS,

            /**
             * Constant for the "Ogham" Unicode character block.
             */
            OGHAM,

            /**
             * Constant for the "Runic" Unicode character block.
             */
            RUNIC,

            /**
             * Constant for the "Khmer" Unicode character block.
             */
            KHMER,

            /**
             * Constant for the "Mongolian" Unicode character block.
             */
            MONGOLIAN,

            /**
             * Constant for the "Braille Patterns" Unicode character block.
             */
            BRAILLE_PATTERNS,

            /**
             * Constant for the "CJK Radicals Supplement" Unicode character block.
             */
            CJK_RADICALS_SUPPLEMENT,

            /**
             * Constant for the "Kangxi Radicals" Unicode character block.
             */
            KANGXI_RADICALS,

            /**
             * Constant for the "Ideographic Description Characters" Unicode character block.
             */
            IDEOGRAPHIC_DESCRIPTION_CHARACTERS,

            /**
             * Constant for the "Bopomofo Extended" Unicode character block.
             */
            BOPOMOFO_EXTENDED,

            /**
             * Constant for the "CJK Unified Ideographs Extension A" Unicode character block.
             */
            CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A,

            /**
             * Constant for the "Yi Syllables" Unicode character block.
             */
            YI_SYLLABLES,

            /**
             * Constant for the "Yi Radicals" Unicode character block.
             */
            YI_RADICALS,

            /**
             * Constant for the "Cyrillic Supplement" Unicode character block.
             * This block was previously known as the "Cyrillic Supplementary" block.
             *
             */
            CYRILLIC_SUPPLEMENTARY,

            /**
             * Constant for the "Tagalog" Unicode character block.
             */
            TAGALOG,

            /**
             * Constant for the "Hanunoo" Unicode character block.
             */
            HANUNOO,

            /**
             * Constant for the "Buhid" Unicode character block.
             */
            BUHID,

            /**
             * Constant for the "Tagbanwa" Unicode character block.
             */
            TAGBANWA,

            /**
             * Constant for the "Limbu" Unicode character block.
             */
            LIMBU,

            /**
             * Constant for the "Tai Le" Unicode character block.
             */
            TAI_LE,

            /**
             * Constant for the "Khmer Symbols" Unicode character block.
             */
            KHMER_SYMBOLS,

            /**
             * Constant for the "Phonetic Extensions" Unicode character block.
             */
            PHONETIC_EXTENSIONS,

            /**
             * Constant for the "Miscellaneous Mathematical Symbols-A" Unicode character block.
             */
            MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A,

            /**
             * Constant for the "Supplemental Arrows-A" Unicode character block.
             */
            SUPPLEMENTAL_ARROWS_A,

            /**
             * Constant for the "Supplemental Arrows-B" Unicode character block.
             */
            SUPPLEMENTAL_ARROWS_B,

            /**
             * Constant for the "Miscellaneous Mathematical Symbols-B" Unicode
             * character block.
             */
            MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B,

            /**
             * Constant for the "Supplemental Mathematical Operators" Unicode
             * character block.
             */
            SUPPLEMENTAL_MATHEMATICAL_OPERATORS,

            /**
             * Constant for the "Miscellaneous Symbols and Arrows" Unicode character
             * block.
             */
            MISCELLANEOUS_SYMBOLS_AND_ARROWS,

            /**
             * Constant for the "Katakana Phonetic Extensions" Unicode character
             * block.
             */
            KATAKANA_PHONETIC_EXTENSIONS,

            /**
             * Constant for the "Yijing Hexagram Symbols" Unicode character block.
             */
            YIJING_HEXAGRAM_SYMBOLS,

            /**
             * Constant for the "Variation Selectors" Unicode character block.
             */
            VARIATION_SELECTORS,

            /**
             * Constant for the "Linear B Syllabary" Unicode character block.
             */
            LINEAR_B_SYLLABARY,

            /**
             * Constant for the "Linear B Ideograms" Unicode character block.
             */
            LINEAR_B_IDEOGRAMS,

            /**
             * Constant for the "Aegean Numbers" Unicode character block.
             */
            AEGEAN_NUMBERS,

            /**
             * Constant for the "Old Italic" Unicode character block.
             */
            OLD_ITALIC,

            /**
             * Constant for the "Gothic" Unicode character block.
             */
            GOTHIC,

            /**
             * Constant for the "Ugaritic" Unicode character block.
             */
            UGARITIC,

            /**
             * Constant for the "Deseret" Unicode character block.
             */
            DESERET,

            /**
             * Constant for the "Shavian" Unicode character block.
             */
            SHAVIAN,

            /**
             * Constant for the "Osmanya" Unicode character block.
             */
            OSMANYA,

            /**
             * Constant for the "Cypriot Syllabary" Unicode character block.
             */
            CYPRIOT_SYLLABARY,

            /**
             * Constant for the "Byzantine Musical Symbols" Unicode character block.
             */
            BYZANTINE_MUSICAL_SYMBOLS,

            /**
             * Constant for the "Musical Symbols" Unicode character block.
             */
            MUSICAL_SYMBOLS,

            /**
             * Constant for the "Tai Xuan Jing Symbols" Unicode character block.
             */
            TAI_XUAN_JING_SYMBOLS,

            /**
             * Constant for the "Mathematical Alphanumeric Symbols" Unicode
             * character block.
             */
            MATHEMATICAL_ALPHANUMERIC_SYMBOLS,

            /**
             * Constant for the "CJK Unified Ideographs Extension B" Unicode
             * character block.
             */
            CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B,

            /**
             * Constant for the "CJK Compatibility Ideographs Supplement" Unicode character block.
             */
            CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT,

            /**
             * Constant for the "Tags" Unicode character block.
             */
            TAGS,

            /**
             * Constant for the "Variation Selectors Supplement" Unicode character
             * block.
             */
            VARIATION_SELECTORS_SUPPLEMENT,

            /**
             * Constant for the "Supplementary private Use Area-A" Unicode character
             * block.
             */
            SUPPLEMENTARY_PRIVATE_USE_AREA_A,

            /**
             * Constant for the "Supplementary private Use Area-B" Unicode character
             * block.
             */
            SUPPLEMENTARY_PRIVATE_USE_AREA_B,

            /**
             * Constant for the "High Surrogates" Unicode character block.
             * This block represents codepoint values in the high surrogate
             * range: U+D800 through U+DB7F
             *
             *
             */
            HIGH_SURROGATES,

            /**
             * Constant for the "High private Use Surrogates" Unicode character
             * block.
             * This block represents codepoint values in the private use high
             * surrogate range: U+DB80 through U+DBFF
             *
             *
             */
            HIGH_PRIVATE_USE_SURROGATES,

            /**
             * Constant for the "Low Surrogates" Unicode character block.
             * This block represents codepoint values in the low surrogate
             * range: U+DC00 through U+DFFF
             *
             *
             */
            LOW_SURROGATES,

            /**
             * Constant for the "Arabic Supplement" Unicode character block.
             */
            ARABIC_SUPPLEMENT,

            /**
             * Constant for the "NKo" Unicode character block.
             */
            NKO,

            /**
             * Constant for the "Samaritan" Unicode character block.
             */
            SAMARITAN,

            /**
             * Constant for the "Mandaic" Unicode character block.
             */
            MANDAIC,

            /**
             * Constant for the "Ethiopic Supplement" Unicode character block.
             */
            ETHIOPIC_SUPPLEMENT,

            /**
             * Constant for the "Unified Canadian Aboriginal Syllabics Extended"
             * Unicode character block.
             */
            UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED,

            /**
             * Constant for the "New Tai Lue" Unicode character block.
             */
            NEW_TAI_LUE,

            /**
             * Constant for the "Buginese" Unicode character block.
             */
            BUGINESE,

            /**
             * Constant for the "Tai Tham" Unicode character block.
             */
            TAI_THAM,

            /**
             * Constant for the "Balinese" Unicode character block.
             */
            BALINESE,

            /**
             * Constant for the "Sundanese" Unicode character block.
             */
            SUNDANESE,

            /**
             * Constant for the "Batak" Unicode character block.
             */
            BATAK,

            /**
             * Constant for the "Lepcha" Unicode character block.
             */
            LEPCHA,

            /**
             * Constant for the "Ol Chiki" Unicode character block.
             */
            OL_CHIKI,

            /**
             * Constant for the "Vedic Extensions" Unicode character block.
             */
            VEDIC_EXTENSIONS,

            /**
             * Constant for the "Phonetic Extensions Supplement" Unicode character
             * block.
             */
            PHONETIC_EXTENSIONS_SUPPLEMENT,

            /**
             * Constant for the "Combining Diacritical Marks Supplement" Unicode
             * character block.
             */
            COMBINING_DIACRITICAL_MARKS_SUPPLEMENT,

            /**
             * Constant for the "Glagolitic" Unicode character block.
             */
            GLAGOLITIC,

            /**
             * Constant for the "Latin Extended-C" Unicode character block.
             */
            LATIN_EXTENDED_C,

            /**
             * Constant for the "Coptic" Unicode character block.
             */
            COPTIC,

            /**
             * Constant for the "Georgian Supplement" Unicode character block.
             */
            GEORGIAN_SUPPLEMENT,

            /**
             * Constant for the "Tifinagh" Unicode character block.
             */
            TIFINAGH,

            /**
             * Constant for the "Ethiopic Extended" Unicode character block.
             */
            ETHIOPIC_EXTENDED,

            /**
             * Constant for the "Cyrillic Extended-A" Unicode character block.
             */
            CYRILLIC_EXTENDED_A,

            /**
             * Constant for the "Supplemental Punctuation" Unicode character block.
             */
            SUPPLEMENTAL_PUNCTUATION,

            /**
             * Constant for the "CJK Strokes" Unicode character block.
             */
            CJK_STROKES,

            /**
             * Constant for the "Lisu" Unicode character block.
             */
            LISU,

            /**
             * Constant for the "Vai" Unicode character block.
             */
            VAI,

            /**
             * Constant for the "Cyrillic Extended-B" Unicode character block.
             */
            CYRILLIC_EXTENDED_B,

            /**
             * Constant for the "Bamum" Unicode character block.
             */
            BAMUM,

            /**
             * Constant for the "Modifier Tone Letters" Unicode character block.
             */
            MODIFIER_TONE_LETTERS,

            /**
             * Constant for the "Latin Extended-D" Unicode character block.
             */
            LATIN_EXTENDED_D,

            /**
             * Constant for the "Syloti Nagri" Unicode character block.
             */
            SYLOTI_NAGRI,

            /**
             * Constant for the "Common Indic Number Forms" Unicode character block.
             */
            COMMON_INDIC_NUMBER_FORMS,

            /**
             * Constant for the "Phags-pa" Unicode character block.
             */
            PHAGS_PA,

            /**
             * Constant for the "Saurashtra" Unicode character block.
             */
            SAURASHTRA,

            /**
             * Constant for the "Devanagari Extended" Unicode character block.
             */
            DEVANAGARI_EXTENDED,

            /**
             * Constant for the "Kayah Li" Unicode character block.
             */
            KAYAH_LI,

            /**
             * Constant for the "Rejang" Unicode character block.
             */
            REJANG,

            /**
             * Constant for the "Hangul Jamo Extended-A" Unicode character block.
             */
            HANGUL_JAMO_EXTENDED_A,

            /**
             * Constant for the "Javanese" Unicode character block.
             */
            JAVANESE,

            /**
             * Constant for the "Cham" Unicode character block.
             */
            CHAM,

            /**
             * Constant for the "Myanmar Extended-A" Unicode character block.
             */
            MYANMAR_EXTENDED_A,

            /**
             * Constant for the "Tai Viet" Unicode character block.
             */
            TAI_VIET,

            /**
             * Constant for the "Ethiopic Extended-A" Unicode character block.
             */
            ETHIOPIC_EXTENDED_A,

            /**
             * Constant for the "Meetei Mayek" Unicode character block.
             */
            MEETEI_MAYEK,

            /**
             * Constant for the "Hangul Jamo Extended-B" Unicode character block.
             */
            HANGUL_JAMO_EXTENDED_B,

            /**
             * Constant for the "Vertical Forms" Unicode character block.
             */
            VERTICAL_FORMS,

            /**
             * Constant for the "Ancient Greek Numbers" Unicode character block.
             */
            ANCIENT_GREEK_NUMBERS,

            /**
             * Constant for the "Ancient Symbols" Unicode character block.
             */
            ANCIENT_SYMBOLS,

            /**
             * Constant for the "Phaistos Disc" Unicode character block.
             */
            PHAISTOS_DISC,

            /**
             * Constant for the "Lycian" Unicode character block.
             */
            LYCIAN,

            /**
             * Constant for the "Carian" Unicode character block.
             */
            CARIAN,

            /**
             * Constant for the "Old Persian" Unicode character block.
             */
            OLD_PERSIAN,

            /**
             * Constant for the "Imperial Aramaic" Unicode character block.
             */
            IMPERIAL_ARAMAIC,

            /**
             * Constant for the "Phoenician" Unicode character block.
             */
            PHOENICIAN,

            /**
             * Constant for the "Lydian" Unicode character block.
             */
            LYDIAN,

            /**
             * Constant for the "Kharoshthi" Unicode character block.
             */
            KHAROSHTHI,

            /**
             * Constant for the "Old South Arabian" Unicode character block.
             */
            OLD_SOUTH_ARABIAN,

            /**
             * Constant for the "Avestan" Unicode character block.
             */
            AVESTAN,

            /**
             * Constant for the "Inscriptional Parthian" Unicode character block.
             */
            INSCRIPTIONAL_PARTHIAN,

            /**
             * Constant for the "Inscriptional Pahlavi" Unicode character block.
             */
            INSCRIPTIONAL_PAHLAVI,

            /**
             * Constant for the "Old Turkic" Unicode character block.
             */
            OLD_TURKIC,

            /**
             * Constant for the "Rumi Numeral Symbols" Unicode character block.
             */
            RUMI_NUMERAL_SYMBOLS,

            /**
             * Constant for the "Brahmi" Unicode character block.
             */
            BRAHMI,

            /**
             * Constant for the "Kaithi" Unicode character block.
             */
            KAITHI,

            /**
             * Constant for the "Cuneiform" Unicode character block.
             */
            CUNEIFORM,

            /**
             * Constant for the "Cuneiform Numbers and Punctuation" Unicode
             * character block.
             */
            CUNEIFORM_NUMBERS_AND_PUNCTUATION,

            /**
             * Constant for the "Egyptian Hieroglyphs" Unicode character block.
             */
            EGYPTIAN_HIEROGLYPHS,

            /**
             * Constant for the "Bamum Supplement" Unicode character block.
             */
            BAMUM_SUPPLEMENT,

            /**
             * Constant for the "Kana Supplement" Unicode character block.
             */
            KANA_SUPPLEMENT,

            /**
             * Constant for the "Ancient Greek Musical Notation" Unicode character
             * block.
             */
            ANCIENT_GREEK_MUSICAL_NOTATION,

            /**
             * Constant for the "Counting Rod Numerals" Unicode character block.
             */
            COUNTING_ROD_NUMERALS,

            /**
             * Constant for the "Mahjong Tiles" Unicode character block.
             */
            MAHJONG_TILES,

            /**
             * Constant for the "Domino Tiles" Unicode character block.
             */
            DOMINO_TILES,

            /**
             * Constant for the "Playing Cards" Unicode character block.
             */
            PLAYING_CARDS,

            /**
             * Constant for the "Enclosed Alphanumeric Supplement" Unicode character
             * block.
             */
            ENCLOSED_ALPHANUMERIC_SUPPLEMENT,

            /**
             * Constant for the "Enclosed Ideographic Supplement" Unicode character
             * block.
             */
            ENCLOSED_IDEOGRAPHIC_SUPPLEMENT,

            /**
             * Constant for the "Miscellaneous Symbols And Pictographs" Unicode
             * character block.
             */
            MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS,

            /**
             * Constant for the "Emoticons" Unicode character block.
             */
            EMOTICONS,

            /**
             * Constant for the "Transport And Map Symbols" Unicode character block.
             */
            TRANSPORT_AND_MAP_SYMBOLS,

            /**
             * Constant for the "Alchemical Symbols" Unicode character block.
             */
            ALCHEMICAL_SYMBOLS,

            /**
             * Constant for the "CJK Unified Ideographs Extension C" Unicode
             * character block.
             */
            CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C,

            /**
             * Constant for the "CJK Unified Ideographs Extension D" Unicode
             * character block.
             */
            CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D,

            /**
             * Constant for the "Arabic Extended-A" Unicode character block.
             */
            ARABIC_EXTENDED_A,

            /**
             * Constant for the "Sundanese Supplement" Unicode character block.
             */
            SUNDANESE_SUPPLEMENT,

            /**
             * Constant for the "Meetei Mayek Extensions" Unicode character block.
             */
            MEETEI_MAYEK_EXTENSIONS,

            /**
             * Constant for the "Meroitic Hieroglyphs" Unicode character block.
             */
            MEROITIC_HIEROGLYPHS,

            /**
             * Constant for the "Meroitic Cursive" Unicode character block.
             */
            MEROITIC_CURSIVE,

            /**
             * Constant for the "Sora Sompeng" Unicode character block.
             */
            SORA_SOMPENG,

            /**
             * Constant for the "Chakma" Unicode character block.
             */
            CHAKMA,

            /**
             * Constant for the "Sharada" Unicode character block.
             */
            SHARADA,

            /**
             * Constant for the "Takri" Unicode character block.
             */
            TAKRI,

            /**
             * Constant for the "Miao" Unicode character block.
             */
            MIAO,

            /**
             * Constant for the "Arabic Mathematical Alphabetic Symbols" Unicode
             * character block.
             */
            ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS,

            /**
             * Constant for the "Combining Diacritical Marks Extended" Unicode
             * character block.
             */
            COMBINING_DIACRITICAL_MARKS_EXTENDED,

            /**
             * Constant for the "Myanmar Extended-B" Unicode character block.
             */
            MYANMAR_EXTENDED_B,

            /**
             * Constant for the "Latin Extended-E" Unicode character block.
             */
            LATIN_EXTENDED_E,

            /**
             * Constant for the "Coptic Epact Numbers" Unicode character block.
             */
            COPTIC_EPACT_NUMBERS,

            /**
             * Constant for the "Old Permic" Unicode character block.
             */
            OLD_PERMIC,

            /**
             * Constant for the "Elbasan" Unicode character block.
             */
            ELBASAN,

            /**
             * Constant for the "Caucasian Albanian" Unicode character block.
             */
            CAUCASIAN_ALBANIAN,

            /**
             * Constant for the "Linear A" Unicode character block.
             */
            LINEAR_A,

            /**
             * Constant for the "Palmyrene" Unicode character block.
             */
            PALMYRENE,

            /**
             * Constant for the "Nabataean" Unicode character block.
             */
            NABATAEAN,

            /**
             * Constant for the "Old North Arabian" Unicode character block.
             */
            OLD_NORTH_ARABIAN,

            /**
             * Constant for the "Manichaean" Unicode character block.
             */
            MANICHAEAN,

            /**
             * Constant for the "Psalter Pahlavi" Unicode character block.
             */
            PSALTER_PAHLAVI,

            /**
             * Constant for the "Mahajani" Unicode character block.
             */
            MAHAJANI,

            /**
             * Constant for the "Sinhala Archaic Numbers" Unicode character block.
             */
            SINHALA_ARCHAIC_NUMBERS,

            /**
             * Constant for the "Khojki" Unicode character block.
             */
            KHOJKI,

            /**
             * Constant for the "Khudawadi" Unicode character block.
             */
            KHUDAWADI,

            /**
             * Constant for the "Grantha" Unicode character block.
             */
            GRANTHA,

            /**
             * Constant for the "Tirhuta" Unicode character block.
             */
            TIRHUTA,

            /**
             * Constant for the "Siddham" Unicode character block.
             */
            SIDDHAM,

            /**
             * Constant for the "Modi" Unicode character block.
             */
            MODI,

            /**
             * Constant for the "Warang Citi" Unicode character block.
             */
            WARANG_CITI,

            /**
             * Constant for the "Pau Cin Hau" Unicode character block.
             */
            PAU_CIN_HAU,

            /**
             * Constant for the "Mro" Unicode character block.
             */
            MRO,

            /**
             * Constant for the "Bassa Vah" Unicode character block.
             */
            BASSA_VAH,

            /**
             * Constant for the "Pahawh Hmong" Unicode character block.
             */
            PAHAWH_HMONG,

            /**
             * Constant for the "Duployan" Unicode character block.
             */
            DUPLOYAN,

            /**
             * Constant for the "Shorthand Format Controls" Unicode character block.
             */
            SHORTHAND_FORMAT_CONTROLS,

            /**
             * Constant for the "Mende Kikakui" Unicode character block.
             */
            MENDE_KIKAKUI,

            /**
             * Constant for the "Ornamental Dingbats" Unicode character block.
             */
            ORNAMENTAL_DINGBATS,

            /**
             * Constant for the "Geometric Shapes Extended" Unicode character block.
             */
            GEOMETRIC_SHAPES_EXTENDED,

            /**
             * Constant for the "Supplemental Arrows-C" Unicode character block.
             */
            SUPPLEMENTAL_ARROWS_C,

            /**
             * Constant for the "Cherokee Supplement" Unicode character block.
             */
            CHEROKEE_SUPPLEMENT,

            /**
             * Constant for the "Hatran" Unicode character block.
             */
            HATRAN,

            /**
             * Constant for the "Old Hungarian" Unicode character block.
             */
            OLD_HUNGARIAN,

            /**
             * Constant for the "Multani" Unicode character block.
             */
            MULTANI,

            /**
             * Constant for the "Ahom" Unicode character block.
             */
            AHOM,

            /**
             * Constant for the "Early Dynastic Cuneiform" Unicode character block.
             */
            EARLY_DYNASTIC_CUNEIFORM,

            /**
             * Constant for the "Anatolian Hieroglyphs" Unicode character block.
             */
            ANATOLIAN_HIEROGLYPHS,

            /**
             * Constant for the "Sutton SignWriting" Unicode character block.
             */
            SUTTON_SIGNWRITING,

            /**
             * Constant for the "Supplemental Symbols and Pictographs" Unicode
             * character block.
             */
            SUPPLEMENTAL_SYMBOLS_AND_PICTOGRAPHS,

            /**
             * Constant for the "CJK Unified Ideographs Extension E" Unicode
             * character block.
             */
            CJK_UNIFIED_IDEOGRAPHS_EXTENSION_E,

            /**
             * Constant for the "Syriac Supplement" Unicode
             * character block.
             */
            SYRIAC_SUPPLEMENT,

            /**
             * Constant for the "Cyrillic Extended-C" Unicode
             * character block.
             */
            CYRILLIC_EXTENDED_C,

            /**
             * Constant for the "Osage" Unicode
             * character block.
             */
            OSAGE,

            /**
             * Constant for the "Newa" Unicode
             * character block.
             */
            NEWA,

            /**
             * Constant for the "Mongolian Supplement" Unicode
             * character block.
             */
            MONGOLIAN_SUPPLEMENT,

            /**
             * Constant for the "Marchen" Unicode
             * character block.
             */
            MARCHEN,

            /**
             * Constant for the "Ideographic Symbols and Punctuation" Unicode
             * character block.
             */
            IDEOGRAPHIC_SYMBOLS_AND_PUNCTUATION,

            /**
             * Constant for the "Tangut" Unicode
             * character block.
             */
            TANGUT,

            /**
             * Constant for the "Tangut Components" Unicode
             * character block.
             */
            TANGUT_COMPONENTS,

            /**
             * Constant for the "Kana Extended-A" Unicode
             * character block.
             */
            KANA_EXTENDED_A,
            /**
             * Constant for the "Glagolitic Supplement" Unicode
             * character block.
             */
            GLAGOLITIC_SUPPLEMENT,
            /**
             * Constant for the "Adlam" Unicode
             * character block.
             */
            ADLAM,

            /**
             * Constant for the "Masaram Gondi" Unicode
             * character block.
             */
            MASARAM_GONDI,

            /**
             * Constant for the "Zanabazar Square" Unicode
             * character block.
             */
            ZANABAZAR_SQUARE,

            /**
             * Constant for the "Nushu" Unicode
             * character block.
             */
            NUSHU,

            /**
             * Constant for the "Soyombo" Unicode
             * character block.
             */
            SOYOMBO,

            /**
             * Constant for the "Bhaiksuki" Unicode
             * character block.
             */
            BHAIKSUKI,

            /**
             * Constant for the "CJK Unified Ideographs Extension F" Unicode
             * character block.
             */
            CJK_UNIFIED_IDEOGRAPHS_EXTENSION_F,
            /**
             * Constant for the "Georgian Extended" Unicode
             * character block.
             */
            GEORGIAN_EXTENDED,

            /**
             * Constant for the "Hanifi Rohingya" Unicode
             * character block.
             */
            HANIFI_ROHINGYA,

            /**
             * Constant for the "Old Sogdian" Unicode
             * character block.
             */
            OLD_SOGDIAN,

            /**
             * Constant for the "Sogdian" Unicode
             * character block.
             */
            SOGDIAN,

            /**
             * Constant for the "Dogra" Unicode
             * character block.
             */
            DOGRA,

            /**
             * Constant for the "Gunjala Gondi" Unicode
             * character block.
             */
            GUNJALA_GONDI,

            /**
             * Constant for the "Makasar" Unicode
             * character block.
             */
            MAKASAR,

            /**
             * Constant for the "Medefaidrin" Unicode
             * character block.
             */
            MEDEFAIDRIN,

            /**
             * Constant for the "Mayan Numerals" Unicode
             * character block.
             */
            MAYAN_NUMERALS,

            /**
             * Constant for the "Indic Siyaq Numbers" Unicode
             * character block.
             */
            INDIC_SIYAQ_NUMBERS,

            /**
             * Constant for the "Chess Symbols" Unicode
             * character block.
             */
            CHESS_SYMBOLS,

            /**
             * Constant for the "Elymaic" Unicode
             * character block.
             */
            ELYMAIC,

            /**
             * Constant for the "Nandinagari" Unicode
             * character block.
             */
            NANDINAGARI,

            /**
             * Constant for the "Tamil Supplement" Unicode
             * character block.
             */
            TAMIL_SUPPLEMENT,

            /**
             * Constant for the "Egyptian Hieroglyph Format Controls" Unicode
             * character block.
             */
            EGYPTIAN_HIEROGLYPH_FORMAT_CONTROLS,

            /**
             * Constant for the "Small Kana Extension" Unicode
             * character block.
             */
            SMALL_KANA_EXTENSION,

            /**
             * Constant for the "Nyiakeng Puachue Hmong" Unicode
             * character block.
             */
            NYIAKENG_PUACHUE_HMONG,

            /**
             * Constant for the "Wancho" Unicode
             * character block.
             */
            WANCHO,

            /**
             * Constant for the "Ottoman Siyaq Numbers" Unicode
             * character block.
             */
            OTTOMAN_SIYAQ_NUMBERS,

            /**
             * Constant for the "Symbols and Pictographs Extended-A" Unicode
             * character block.
             */
            SYMBOLS_AND_PICTOGRAPHS_EXTENDED_A,

            /**
             * Constant for the "Yezidi" Unicode
             * character block.
             */
            YEZIDI,

            /**
             * Constant for the "Chorasmian" Unicode
             * character block.
             */
            CHORASMIAN,

            /**
             * Constant for the "Dives Akuru" Unicode
             * character block.
             */
            DIVES_AKURU,

            /**
             * Constant for the "Lisu Supplement" Unicode
             * character block.
             */
            LISU_SUPPLEMENT,

            /**
             * Constant for the "Khitan Small Script" Unicode
             * character block.
             */
            KHITAN_SMALL_SCRIPT,

            /**
             * Constant for the "Tangut Supplement" Unicode
             * character block.
             */
            TANGUT_SUPPLEMENT,

            /**
             * Constant for the "Symbols for Legacy Computing" Unicode
             * character block.
             */
            SYMBOLS_FOR_LEGACY_COMPUTING,

            /**
             * Constant for the "CJK Unified Ideographs Extension G" Unicode
             * character block.
             */
            CJK_UNIFIED_IDEOGRAPHS_EXTENSION_G,

            /**
             * Constant for the "Arabic Extended-B" Unicode
             * character block.
             */
            ARABIC_EXTENDED_B,

            /**
             * Constant for the "Vithkuqi" Unicode
             * character block.
             */
            VITHKUQI,

            /**
             * Constant for the "Latin Extended-F" Unicode
             * character block.
             */
            LATIN_EXTENDED_F,

            /**
             * Constant for the "Old Uyghur" Unicode
             * character block.
             */
            OLD_UYGHUR,

            /**
             * Constant for the "Unified Canadian Aboriginal Syllabics Extended-A" Unicode
             * character block.
             */
            UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED_A,

            /**
             * Constant for the "Cypro-Minoan" Unicode
             * character block.
             */
            CYPRO_MINOAN,

            /**
             * Constant for the "Tangsa" Unicode
             * character block.
             */
            TANGSA,

            /**
             * Constant for the "Kana Extended-B" Unicode
             * character block.
             */
            KANA_EXTENDED_B,

            /**
             * Constant for the "Znamenny Musical Notation" Unicode
             * character block.
             */
            ZNAMENNY_MUSICAL_NOTATION,

            /**
             * Constant for the "Latin Extended-G" Unicode
             * character block.
             */
            LATIN_EXTENDED_G,

            /**
             * Constant for the "Toto" Unicode
             * character block.
             */
            TOTO,

            /**
             * Constant for the "Ethiopic Extended-B" Unicode
             * character block.
             */
            ETHIOPIC_EXTENDED_B,

            /**
             * Constant for the "Arabic Extended-C" Unicode
             * character block.
             */
            ARABIC_EXTENDED_C,

            /**
             * Constant for the "Devanagari Extended-A" Unicode
             * character block.
             */
            DEVANAGARI_EXTENDED_A,

            /**
             * Constant for the "Kawi" Unicode
             * character block.
             */
            KAWI,

            /**
             * Constant for the "Kaktovik Numerals" Unicode
             * character block.
             */
            KAKTOVIK_NUMERALS,

            /**
             * Constant for the "Cyrillic Extended-D" Unicode
             * character block.
             */
            CYRILLIC_EXTENDED_D,

            /**
             * Constant for the "Nag Mundari" Unicode
             * character block.
             */
            NAG_MUNDARI,

            /**
             * Constant for the "CJK Unified Ideographs Extension H" Unicode
             * character block.
             */
            CJK_UNIFIED_IDEOGRAPHS_EXTENSION_H,

            /**
             * Constant for the UNASSIGNED
             */
            UNASSIGNED
        };

        /**
         *  Character decomposition style
         */
        enum class Decomposition: gbyte {
            /**
             * Undefined specify that character is not decomposable or iterator is invalid character
             */
            UNDEFINED = -1,

            CANONICAL,
            FONT,
            NO_BREAK,
            INITIAL,
            MEDIAL,
            FINAL,
            ISOLATED,

            /**
             * Circle is used for character into circle
             */
            CIRCLE,

            /**
             * Super is used form the exponent character (<sup>2</sup>(u'&#92;u00b2') => "2")
             */
            SUPER,

            /**
             * Sub is used from the sub exponent character (u'&#92;u00bc' => "1/4")
             */
            SUB,
            VERTICAL,
            WIDE,
            NARROW,
            SMALL,

            /**
             * Circle is used for character into square
             */
            SQUARE,
            COMPAT,

            /**
             * Fraction is used to decompose fraction character
             */
            FRACTION
        };

        /**
         * Character combining class
         */
        enum class CombiningClass: native::GENERIC_UINT8 {
            UNDEFINED = 0,

            BELOW_LEFT_ATTACHED = 200,
            BELOW_ATTACHED = 202,
            BELOW_RIGHT_ATTACHED = 204,
            LEFT_ATTACHED = 208,
            RIGHT_ATTACHED = 210,
            ABOVE_LEFT_ATTACHED = 212,
            ABOVE_ATTACHED = 214,
            ABOVE_RIGHT_ATTACHED = 216,

            BELOW_LEFT = 218,
            BELOW = 220,
            BELOW_RIGHT = 222,
            LEFT = 224,
            RIGHT = 226,
            ABOVE_LEFT = 228,
            ABOVE = 230,
            ABOVE_RIGHT = 232,

            DOUBLE_BELOW = 233,
            DOUBLE_ABOVE = 234,
            IOTA_SUBSCRIPT = 240
        };

        /**
         * Construct new Character object representing the specified
         * gchar value.
         *
         * @param value
         *          The value of Character
         */
        CORE_FAST Character(gchar value) : value(value) {}

        /**
         * Returns a <b> Character</b> instance representing the specified
         * <b> gchar</b> value.
         *
         * This method will always cache values in the range <b>
         * '&#92;u0000'</b> to <b> '&#92;u007F'</b>, inclusive, and may
         * cache other values outside of this range.
         *
         * @param  c a gchar value.
         * @return a <b> Character</b> instance representing <b> c</b>.
         */
        static Character valueOf(gchar c);

        /**
         * Returns the value of this <b> Character</b> object.
         * @return  the native <b> gchar</b> value represented by
         *          this object.
         */
        CORE_FAST gchar charValue() const { return value; }

        /**
         * Returns a hash code for this <b> Character</b>; equal to the result
         * of invoking <b> charValue()</b>.
         *
         * @return a hash code value for this <b> Character</b>
         */
        gint hash() const override;

        /**
         * Returns a hash code for a <b> gchar</b> value; compatible with
         * <b> Character.hash</b>.
         *
         * @param value The <b> gchar</b> for which to return a hash code.
         * @return a hash code value for a <b> gchar</b> value.
         */
        static CORE_FAST gint hash(gchar c) { return (gint) c; }

        /**
         * Compares two <b> Character</b> objects numerically.
         *
         * @param   anotherCharacter   the <b> Character</b> to be compared.
         * @return  the value <b> 0</b> if the argument <b> Character</b>
         *          is equal to this <b> Character</b>; a value less than
         *          <b> 0</b> if this <b> Character</b> is numerically less
         *          than the <b> Character</b> argument; and a value greater than
         *          <b> 0</b> if this <b> Character</b> is numerically greater
         *          than the <b> Character</b> argument (unsigned comparison).
         *          Note that this is strictly a numerical comparison; it is not
         *          locale-dependent.
         */
        gint compareTo(const Character& other) const override;

        /**
         * Compares two <b> gchar</b> values numerically.
         * The value returned is identical to what would be returned by:
         * <pre>
         *    Character.valueOf(x).compareTo(Character.valueOf(y))
         * </pre>
         *
         * @param  x the first <b> gchar</b> to compare
         * @param  y the second <b> gchar</b> to compare
         * @return the value <b> 0</b> if <b> x == y</b>;
         *         a value less than <b> 0</b> if <b> x < y</b>; and
         *         a value greater than <b> 0</b> if <b> x > y</b>
         */
        static CORE_FAST gint compare(gchar x, gchar y) { return x - y; }

        /**
         * Compares this object against the specified object.
         * The result is <b> true</b> if and only if the argument is not
         * <b> null</b> and is a <b> Character</b> object that
         * represents the same <b> gchar</b> value as this object.
         *
         * @param   obj   the object to compare with.
         * @return  <b> true</b> if the objects are the same;
         *          <b> false</b> otherwise.
         */
        gbool equals(const Object &object) const override;

        /**
         * Returns a <b> String</b> object representing this
         * <b> Character</b>'s value.  The result is a string of
         * length 1 whose sole component is the native
         * <b> gchar</b> value represented by this
         * <b> Character</b> object.
         *
         * @return  a string representation of this object.
         */
        String toString() const override;

        /**
         * Returns a <b> String</b> object representing the
         * specified <b> gchar</b>.  The result is a string of length
         * 1 consisting solely of the specified <b> gchar</b>.
         *
         * @apiNote This method cannot handle <a
         * href="#supplementary"> supplementary characters</a>. To support
         * all Unicode characters, including supplementary characters, use
         * the <b style="color: orange"> #toString(gint)</b> method.
         *
         * @param c the <b> gchar</b> to be converted
         * @return the string representation of the specified <b> gchar</b>
         */
        static String toString(gchar ch);

        /**
         * Returns a <b> String</b> object representing the
         * specified character (Unicode code point).  The result is a string of
         * length 1 or 2, consisting solely of the specified <b> codePoint</b>.
         *
         * @param codePoint the <b> codePoint</b> to be converted
         * @return the string representation of the specified <b> codePoint</b>
         * @throws ArgumentException if the specified
         *      <b> codePoint</b> is not a <b style="color: green"> isValidCodePoint
         *      valid Unicode code point</b>.
         */
        static String toString(gint codePoint);

        /**
         * Determines whether the specified code point is a valid
         * <a href="http://www.unicode.org/glossary/#code_point">
         * Unicode code point value</a>.
         *
         * @param  codePoint the Unicode code point to be tested
         * @return <b> true</b> if the specified code point value is between
         *         <b style="color: orange"> #MIN_CODE_POINT</b> and
         *         <b style="color: orange"> #MAX_CODE_POINT</b> inclusive;
         *         <b> false</b> otherwise.
         */
        static CORE_FAST gbool isValidCodePoint(gint codePoint) {
            // Optimized form of codePoint >= MIN_CODEPOINT && codePoint <= MAX_CODEPOINT
            return (codePoint >> 16) < ((MAX_CODEPOINT + 1) >> 16);
        }

        /**
         * Determines whether the specified character (Unicode code point)
         * is in the <a href="#BMP">Basic Multilingual Plane (BMP)</a>.
         * Such code points can be represented using a single <b> gchar</b>.
         *
         * @param  codePoint the character (Unicode code point) to be tested
         * @return <b> true</b> if the specified code point is between
         *         <b style="color: orange"> #MIN_VALUE</b> and <b style="color: orange"> #MAX_VALUE</b> inclusive;
         *         <b> false</b> otherwise.
         */
        static CORE_FAST gbool isBMP(gint codePoint) {
            // Optimized form of codePoint >= MIN_VALUE && codePoint <= MAX_VALUE
            return (codePoint >> 16) == 0;
        }

        /**
         * Determines whether the specified character (Unicode code point)
         * is in the <a href="#supplementary">supplementary character</a> range.
         *
         * @param  codePoint the character (Unicode code point) to be tested
         * @return <b> true</b> if the specified code point is between
         *         <b style="color: orange"> #MIN_SUPPLEMENTARY_CODE_POINT</b> and
         *         <b style="color: orange"> #MAX_CODE_POINT</b> inclusive;
         *         <b> false</b> otherwise.
         */
        static CORE_FAST gbool isSupplementary(gint codePoint) {
            return MIN_SUPPLEMENTARY <= codePoint && codePoint < MAX_CODEPOINT + 1;
        }

        /**
         * Determines if the given <b> gchar</b> value is a
         * <a href="http://www.unicode.org/glossary/#high_surrogate_code_unit">
         * Unicode high-surrogate code unit</a>
         * (also known as <i>leading-surrogate code unit</i>).
         *
         * <p>Such values do not represent characters by themselves,
         * but are used in the representation of
         * <a href="#supplementary">supplementary characters</a>
         * in the UTF-16 encoding.
         *
         * @param  ch the <b> gchar</b> value to be tested.
         * @return <b> true</b> if the <b> gchar</b> value is between
         *         <b style="color: orange"> #MIN_HIGH_SURROGATE</b> and
         *         <b style="color: orange"> #MAX_HIGH_SURROGATE</b> inclusive;
         *         <b> false</b> otherwise.
         * @see    Character.isLowSurrogate(gchar)
         * @see    Character.UnicodeBlock#of(gint)
         */
        static CORE_FAST gbool isHighSurrogate(gchar ch) {
            return MIN_HIGH_SURROGATE <= ch && ch < MAX_HIGH_SURROGATE + 1;
        }

        /**
         * Determines if the given <b> gchar</b> value is a
         * <a href="http://www.unicode.org/glossary/#low_surrogate_code_unit">
         * Unicode low-surrogate code unit</a>
         * (also known as <i>trailing-surrogate code unit</i>).
         *
         * <p>Such values do not represent characters by themselves,
         * but are used in the representation of
         * <a href="#supplementary">supplementary characters</a>
         * in the UTF-16 encoding.
         *
         * @param  ch the <b> gchar</b> value to be tested.
         * @return <b> true</b> if the <b> gchar</b> value is between
         *         <b style="color: orange"> #MIN_LOW_SURROGATE</b> and
         *         <b style="color: orange"> #MAX_LOW_SURROGATE</b> inclusive;
         *         <b> false</b> otherwise.
         * @see    Character.isHighSurrogate(gchar)
         */
        static CORE_FAST gbool isLowSurrogate(gchar ch) {
            return MIN_LOW_SURROGATE <= ch && ch < MAX_LOW_SURROGATE + 1;
        }

        /**
         * Determines if the given <b> gchar</b> value is a Unicode
         * <i>surrogate code unit</i>.
         *
         * <p>Such values do not represent characters by themselves,
         * but are used in the representation of
         * <a href="#supplementary">supplementary characters</a>
         * in the UTF-16 encoding.
         *
         * <p>A gchar value is a surrogate code unit if and only if it is either
         * a <b style="color: green"> low-surrogate code unit</b> or
         * a <b style="color: green"> high-surrogate code unit</b>.
         *
         * @param  ch the <b> gchar</b> value to be tested.
         * @return <b> true</b> if the <b> gchar</b> value is between
         *         <b style="color: orange"> #MIN_SURROGATE</b> and
         *         <b style="color: orange"> #MAX_SURROGATE</b> inclusive;
         *         <b> false</b> otherwise.
         */
        static CORE_FAST gbool isSurrogate(gchar ch) {
            return MIN_SURROGATE <= ch && ch < MAX_SURROGATE + 1;
        }

        /**
         * Determines whether the specified pair of <b> gchar</b>
         * values is a valid
         * <a href="http://www.unicode.org/glossary/#surrogate_pair">
         * Unicode surrogate pair</a>.
         *
         * <p>This method is equivalent to the expression:
         * <blockquote><pre><b>
         * isHighSurrogate(high) && isLowSurrogate(low)
         * </b></pre></blockquote>
         *
         * @param  high the high-surrogate code value to be tested
         * @param  low the low-surrogate code value to be tested
         * @return <b> true</b> if the specified high and
         * low-surrogate code values represent a valid surrogate pair;
         * <b> false</b> otherwise.
         */
        static CORE_FAST gbool isSurrogatePair(gchar high, gchar low) {
            return isHighSurrogate(high) && isLowSurrogate(low);
        }

        /**
         * Determines the number of <b> gchar</b> values needed to
         * represent the specified character (Unicode code point). If the
         * specified character is equal to or greater than 0x10000, then
         * the method returns 2. Otherwise, the method returns 1.
         *
         * <p>This method doesn't validate the specified character to be a
         * valid Unicode code point. The caller must validate the
         * character value using <b style="color: orange"> isValidCodePoint</b>
         * if necessary.
         *
         * @param   codePoint the character (Unicode code point) to be tested.
         * @return  2 if the character is a valid supplementary character; 1 otherwise.
         * @see     Character.isSupplementaryCodePoint(gint)
         */
        static CORE_FAST gint charCount(gint codePoint) {
            return codePoint < MIN_SUPPLEMENTARY ? 1 : 2;
        }

        /**
         * Converts the specified surrogate pair to its supplementary code
         * point value. This method does not validate the specified
         * surrogate pair. The caller must validate it using <b style="color: orange">
         * isSurrogatePair</b> if necessary.
         *
         * @param  high the high-surrogate code unit
         * @param  low the low-surrogate code unit
         * @return the supplementary code point composed from the
         *         specified surrogate pair.
         */
        static CORE_FAST gint joinSurrogates(gchar high, gint low) {
            // Optimized form of ((high - MIN_HIGH_SURROGATE) << 10) + (low - MIN_LOW_SURROGATE) + MIN_SUPPLEMENTARY;
            return ((high << 10) + low) + (MIN_SUPPLEMENTARY - (MIN_HIGH_SURROGATE << 10) - MIN_LOW_SURROGATE);
        }

        /**
         * Returns the leading surrogate (a
         * <a href="http://www.unicode.org/glossary/#high_surrogate_code_unit">
         * high surrogate code unit</a>) of the
         * <a href="http://www.unicode.org/glossary/#surrogate_pair">
         * surrogate pair</a>
         * representing the specified supplementary character (Unicode
         * code point) in the UTF-16 encoding.  If the specified character
         * is not a
         * <a href="Character.html#supplementary">supplementary character</a>,
         * an unspecified <b> gchar</b> is returned.
         *
         * <p>If
         * <b style="color: orange"> isSupplementary(x)</b>
         * is <b> true</b>, then
         * <b style="color: orange"> isHighSurrogate</b><b> (highSurrogate(x))</b> and
         * <b style="color: orange"> joinSurrogate</b><b> (highSurrogate(x), </b><b style="color: orange"> #lowSurrogate lowSurrogate</b><b> (x)) == x</b>
         * are also always <b> true</b>.
         *
         * @param   codePoint a supplementary character (Unicode code point)
         * @return  the leading surrogate code unit used to represent the
         *          character in the UTF-16 encoding
         */
        static CORE_FAST gchar highSurrogate(gint codePoint) {
            return (gchar) ((codePoint >> 10) + (MIN_HIGH_SURROGATE - (MIN_SUPPLEMENTARY >> 10)));
        }

        /**
         * Returns the trailing surrogate (a
         * <a href="http://www.unicode.org/glossary/#low_surrogate_code_unit">
         * low surrogate code unit</a>) of the
         * <a href="http://www.unicode.org/glossary/#surrogate_pair">
         * surrogate pair</a>
         * representing the specified supplementary character (Unicode
         * code point) in the UTF-16 encoding.  If the specified character
         * is not a
         * <a href="Character.html#supplementary">supplementary character</a>,
         * an unspecified <b> gchar</b> is returned.
         *
         * <p>If
         * <b style="color: orange"> isSupplementary(x)</b>
         * is <b> true</b>, then
         * <b style="color: orange"> isLowSurrogate</b><b> (lowSurrogate(x))</b> and
         * <b style="color: orange"> #toCodePoint toCodePoint</b><b> (</b><b style="color: orange"> highSurrogate highSurrogate</b><b> (x), lowSurrogate(x)) == x</b>
         * are also always <b> true</b>.
         *
         * @param   codePoint a supplementary character (Unicode code point)
         * @return  the trailing surrogate code unit used to represent the
         *          character in the UTF-16 encoding
         */
        static CORE_FAST gchar lowSurrogate(gint codePoint) {
            return (gchar) ((codePoint & 0x3ff) + MIN_LOW_SURROGATE);
        }

        /**
         * Determines if the specified character is a lowercase character.
         * <p>
         * A character is lowercase if its general category type, provided
         * by <b> Character.category(ch)</b>, is
         * <b> LOWERCASE_LETTER</b>, or it has contributory property
         * Other_Lowercase as defined by the Unicode Standard.
         * <p>
         * The following are examples of lowercase characters:
         * <blockquote><pre>
         * a b c d e f g h i j k l m n o p q r s t u v w x y z
         * '&#92;u00DF' '&#92;u00E0' '&#92;u00E1' '&#92;u00E2' '&#92;u00E3' '&#92;u00E4' '&#92;u00E5' '&#92;u00E6'
         * '&#92;u00E7' '&#92;u00E8' '&#92;u00E9' '&#92;u00EA' '&#92;u00EB' '&#92;u00EC' '&#92;u00ED' '&#92;u00EE'
         * '&#92;u00EF' '&#92;u00F0' '&#92;u00F1' '&#92;u00F2' '&#92;u00F3' '&#92;u00F4' '&#92;u00F5' '&#92;u00F6'
         * '&#92;u00F8' '&#92;u00F9' '&#92;u00FA' '&#92;u00FB' '&#92;u00FC' '&#92;u00FD' '&#92;u00FE' '&#92;u00FF'
         * </pre></blockquote>
         * <p> Many other Unicode characters are lowercase too.
         *
         * <p><b>Note:</b> This method cannot handle <a
         * href="#supplementary"> supplementary characters</a>. To support
         * all Unicode characters, including supplementary characters, use
         * the <b style="color: orange"> isLowerCase(gint)</b> method.
         *
         * @param   ch   the character to be tested.
         * @return  <b> true</b> if the character is lowercase;
         *          <b> false</b> otherwise.
         * @see     Character.isLowerCase(gchar)
         * @see     Character.isTitleCase(gchar)
         * @see     Character.toLowerCase(gchar)
         * @see     Character.category(gchar)
         */
        static gbool isLowerCase(gchar ch);

        /**
         * Determines if the specified character (Unicode code point) is a
         * lowercase character.
         * <p>
         * A character is lowercase if its general category type, provided
         * by <b style="color: orange"> Character.category(codePoint)</b>, is
         * <b> LOWERCASE_LETTER</b>, or it has contributory property
         * Other_Lowercase as defined by the Unicode Standard.
         * <p>
         * The following are examples of lowercase characters:
         * <blockquote><pre>
         * a b c d e f g h i j k l m n o p q r s t u v w x y z
         * '&#92;u00DF' '&#92;u00E0' '&#92;u00E1' '&#92;u00E2' '&#92;u00E3' '&#92;u00E4' '&#92;u00E5' '&#92;u00E6'
         * '&#92;u00E7' '&#92;u00E8' '&#92;u00E9' '&#92;u00EA' '&#92;u00EB' '&#92;u00EC' '&#92;u00ED' '&#92;u00EE'
         * '&#92;u00EF' '&#92;u00F0' '&#92;u00F1' '&#92;u00F2' '&#92;u00F3' '&#92;u00F4' '&#92;u00F5' '&#92;u00F6'
         * '&#92;u00F8' '&#92;u00F9' '&#92;u00FA' '&#92;u00FB' '&#92;u00FC' '&#92;u00FD' '&#92;u00FE' '&#92;u00FF'
         * </pre></blockquote>
         * <p> Many other Unicode characters are lowercase too.
         *
         * @param   codePoint the character (Unicode code point) to be tested.
         * @return  <b> true</b> if the character is lowercase;
         *          <b> false</b> otherwise.
         * @see     Character.isLowerCase(gint)
         * @see     Character.isTitleCase(gint)
         * @see     Character.toLowerCase(gint)
         * @see     Character.category(gint)
         */
        static gbool isLowerCase(gint codePoint);

        /**
         * Determines if the specified character is an uppercase character.
         * <p>
         * A character is uppercase if its general category type, provided by
         * <b> Character.category(ch)</b>, is <b> UPPERCASE_LETTER</b>.
         * or it has contributory property Other_Uppercase as defined by the Unicode Standard.
         * <p>
         * The following are examples of uppercase characters:
         * <blockquote><pre>
         * A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
         * '&#92;u00C0' '&#92;u00C1' '&#92;u00C2' '&#92;u00C3' '&#92;u00C4' '&#92;u00C5' '&#92;u00C6' '&#92;u00C7'
         * '&#92;u00C8' '&#92;u00C9' '&#92;u00CA' '&#92;u00CB' '&#92;u00CC' '&#92;u00CD' '&#92;u00CE' '&#92;u00CF'
         * '&#92;u00D0' '&#92;u00D1' '&#92;u00D2' '&#92;u00D3' '&#92;u00D4' '&#92;u00D5' '&#92;u00D6' '&#92;u00D8'
         * '&#92;u00D9' '&#92;u00DA' '&#92;u00DB' '&#92;u00DC' '&#92;u00DD' '&#92;u00DE'
         * </pre></blockquote>
         * <p> Many other Unicode characters are uppercase too.
         *
         * <p><b>Note:</b> This method cannot handle <a
         * href="#supplementary"> supplementary characters</a>. To support
         * all Unicode characters, including supplementary characters, use
         * the <b style="color: orange"> isUpperCase(gint)</b> method.
         *
         * @param   ch   the character to be tested.
         * @return  <b> true</b> if the character is uppercase;
         *          <b> false</b> otherwise.
         * @see     Character.isLowerCase(gchar)
         * @see     Character.isTitleCase(gchar)
         * @see     Character.toUpperCase(gchar)
         * @see     Character.category(gchar)
         */
        static gbool isUpperCase(gchar ch);

        /**
         * Determines if the specified character (Unicode code point) is an uppercase character.
         * <p>
         * A character is uppercase if its general category type, provided by
         * <b style="color: orange"> Character.category(codePoint)</b>, is <b> UPPERCASE_LETTER</b>,
         * or it has contributory property Other_Uppercase as defined by the Unicode Standard.
         * <p>
         * The following are examples of uppercase characters:
         * <blockquote><pre>
         * A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
         * '&#92;u00C0' '&#92;u00C1' '&#92;u00C2' '&#92;u00C3' '&#92;u00C4' '&#92;u00C5' '&#92;u00C6' '&#92;u00C7'
         * '&#92;u00C8' '&#92;u00C9' '&#92;u00CA' '&#92;u00CB' '&#92;u00CC' '&#92;u00CD' '&#92;u00CE' '&#92;u00CF'
         * '&#92;u00D0' '&#92;u00D1' '&#92;u00D2' '&#92;u00D3' '&#92;u00D4' '&#92;u00D5' '&#92;u00D6' '&#92;u00D8'
         * '&#92;u00D9' '&#92;u00DA' '&#92;u00DB' '&#92;u00DC' '&#92;u00DD' '&#92;u00DE'
         * </pre></blockquote>
         * <p> Many other Unicode characters are uppercase too.
         *
         * @param   codePoint the character (Unicode code point) to be tested.
         * @return  <b> true</b> if the character is uppercase;
         *          <b> false</b> otherwise.
         * @see     Character.isLowerCase(gint)
         * @see     Character.isTitleCase(gint)
         * @see     Character.toUpperCase(gint)
         * @see     Character.category(gint)
         */
        static gbool isUpperCase(gint codePoint);

        /**
         * Determines if the specified character is a titlecase character.
         * <p>
         * A character is a titlecase character if its general
         * category type, provided by <b> Character.category(ch)</b>,
         * is <b> TITLECASE_LETTER</b>.
         * <p>
         * Some characters look like pairs of Latin letters. For example, there
         * is an uppercase letter that looks like "LJ" and has a corresponding
         * lowercase letter that looks like "lj". A third form, which looks like "Lj",
         * is the appropriate form to use when rendering a word in lowercase
         * with initial capitals, as for a book title.
         * <p>
         * These are some of the Unicode characters for which this method returns
         * <b> true</b>:
         * <ul>
         * <li><b> LATIN CAPITAL LETTER D WITH SMALL LETTER Z WITH CARON</b>
         * <li><b> LATIN CAPITAL LETTER L WITH SMALL LETTER J</b>
         * <li><b> LATIN CAPITAL LETTER N WITH SMALL LETTER J</b>
         * <li><b> LATIN CAPITAL LETTER D WITH SMALL LETTER Z</b>
         * </ul>
         * <p> Many other Unicode characters are titlecase too.
         *
         * <p><b>Note:</b> This method cannot handle <a
         * href="#supplementary"> supplementary characters</a>. To support
         * all Unicode characters, including supplementary characters, use
         * the <b style="color: orange"> isTitleCase(gint)</b> method.
         *
         * @param   ch   the character to be tested.
         * @return  <b> true</b> if the character is titlecase;
         *          <b> false</b> otherwise.
         * @see     Character.isLowerCase(gchar)
         * @see     Character.isUpperCase(gchar)
         * @see     Character.toTitleCase(gchar)
         * @see     Character.category(gchar)
         */
        static gbool isTitleCase(gchar ch);

        /**
         * Determines if the specified character (Unicode code point) is a titlecase character.
         * <p>
         * A character is a titlecase character if its general
         * category type, provided by <b style="color: orange"> Character.category(codePoint)</b>,
         * is <b> TITLECASE_LETTER</b>.
         * <p>
         * Some characters look like pairs of Latin letters. For example, there
         * is an uppercase letter that looks like "LJ" and has a corresponding
         * lowercase letter that looks like "lj". A third form, which looks like "Lj",
         * is the appropriate form to use when rendering a word in lowercase
         * with initial capitals, as for a book title.
         * <p>
         * These are some of the Unicode characters for which this method returns
         * <b> true</b>:
         * <ul>
         * <li><b> LATIN CAPITAL LETTER D WITH SMALL LETTER Z WITH CARON</b>
         * <li><b> LATIN CAPITAL LETTER L WITH SMALL LETTER J</b>
         * <li><b> LATIN CAPITAL LETTER N WITH SMALL LETTER J</b>
         * <li><b> LATIN CAPITAL LETTER D WITH SMALL LETTER Z</b>
         * </ul>
         * <p> Many other Unicode characters are titlecase too.
         *
         * @param   codePoint the character (Unicode code point) to be tested.
         * @return  <b> true</b> if the character is titlecase;
         *          <b> false</b> otherwise.
         * @see     Character.isLowerCase(gint)
         * @see     Character.isUpperCase(gint)
         * @see     Character.toTitleCase(gint)
         * @see     Character.category(gint)
         */
        static gbool isTitleCase(gint codePoint);

        /**
         * Determines if the specified character is a digit.
         * <p>
         * A character is a digit if its general category type, provided
         * by <b> Character.category(ch)</b>, is
         * <b> DECIMAL_DIGIT_NUMBER</b>.
         * <p>
         * Some Unicode character ranges that contain digits:
         * <ul>
         * <li><b> '&#92;u0030'</b> through <b> '&#92;u0039'</b>,
         *     ISO-LATIN-1 digits (<b> '0'</b> through <b> '9'</b>)
         * <li><b> '&#92;u0660'</b> through <b> '&#92;u0669'</b>,
         *     Arabic-Indic digits
         * <li><b> '&#92;u06F0'</b> through <b> '&#92;u06F9'</b>,
         *     Extended Arabic-Indic digits
         * <li><b> '&#92;u0966'</b> through <b> '&#92;u096F'</b>,
         *     Devanagari digits
         * <li><b> '&#92;uFF10'</b> through <b> '&#92;uFF19'</b>,
         *     Fullwidth digits
         * </ul>
         *
         * Many other character ranges contain digits as well.
         *
         * <p><b>Note:</b> This method cannot handle <a
         * href="#supplementary"> supplementary characters</a>. To support
         * all Unicode characters, including supplementary characters, use
         * the <b style="color: orange"> isDigit(gint)</b> method.
         *
         * @param   ch   the character to be tested.
         * @return  <b> true</b> if the character is a digit;
         *          <b> false</b> otherwise.
         * @see     Character.digit(gchar, gint)
         * @see     Character.forDigit(gint, gint)
         * @see     Character.category(gchar)
         */
        static gbool isDigit(gchar ch);

        /**
         * Determines if the specified character (Unicode code point) is a digit.
         * <p>
         * A character is a digit if its general category type, provided
         * by <b style="color: orange"> Character.category(codePoint)</b>, is
         * <b> DECIMAL_DIGIT_NUMBER</b>.
         * <p>
         * Some Unicode character ranges that contain digits:
         * <ul>
         * <li><b> '&#92;u0030'</b> through <b> '&#92;u0039'</b>,
         *     ISO-LATIN-1 digits (<b> '0'</b> through <b> '9'</b>)
         * <li><b> '&#92;u0660'</b> through <b> '&#92;u0669'</b>,
         *     Arabic-Indic digits
         * <li><b> '&#92;u06F0'</b> through <b> '&#92;u06F9'</b>,
         *     Extended Arabic-Indic digits
         * <li><b> '&#92;u0966'</b> through <b> '&#92;u096F'</b>,
         *     Devanagari digits
         * <li><b> '&#92;uFF10'</b> through <b> '&#92;uFF19'</b>,
         *     Fullwidth digits
         * </ul>
         *
         * Many other character ranges contain digits as well.
         *
         * @param   codePoint the character (Unicode code point) to be tested.
         * @return  <b> true</b> if the character is a digit;
         *          <b> false</b> otherwise.
         * @see     Character.forDigit(gint, gint)
         * @see     Character.category(gint)
         */
        static gbool isDigit(gint codePoint);

        /**
         * Determines if the specified character is a letter.
         * <p>
         * A character is considered to be a letter if its general
         * category type, provided by <b> Character.category(ch)</b>,
         * is any of the following:
         * <ul>
         * <li> <b> UPPERCASE_LETTER</b>
         * <li> <b> LOWERCASE_LETTER</b>
         * <li> <b> TITLECASE_LETTER</b>
         * <li> <b> MODIFIER_LETTER</b>
         * <li> <b> OTHER_LETTER</b>
         * </ul>
         *
         * Not all letters have case. Many characters are
         * letters but are neither uppercase nor lowercase nor titlecase.
         *
         * <p><b>Note:</b> This method cannot handle <a
         * href="#supplementary"> supplementary characters</a>. To support
         * all Unicode characters, including supplementary characters, use
         * the <b style="color: orange"> isLetter(gint)</b> method.
         *
         * @param   ch   the character to be tested.
         * @return  <b> true</b> if the character is a letter;
         *          <b> false</b> otherwise.
         * @see     Character.isDigit(gchar)
         * @see     Character.isLetterOrDigit(gchar)
         * @see     Character.isLowerCase(gchar)
         * @see     Character.isTitleCase(gchar)
         * @see     Character.isUnicodeIdentifierStart(gchar)
         * @see     Character.isUpperCase(gchar)
         */
        static gbool isLetter(gchar ch);

        /**
         * Determines if the specified character (Unicode code point) is a letter.
         * <p>
         * A character is considered to be a letter if its general
         * category type, provided by <b style="color: orange"> Character.category(codePoint)</b>,
         * is any of the following:
         * <ul>
         * <li> <b> UPPERCASE_LETTER</b>
         * <li> <b> LOWERCASE_LETTER</b>
         * <li> <b> TITLECASE_LETTER</b>
         * <li> <b> MODIFIER_LETTER</b>
         * <li> <b> OTHER_LETTER</b>
         * </ul>
         *
         * Not all letters have case. Many characters are
         * letters but are neither uppercase nor lowercase nor titlecase.
         *
         * @param   codePoint the character (Unicode code point) to be tested.
         * @return  <b> true</b> if the character is a letter;
         *          <b> false</b> otherwise.
         * @see     Character.isDigit(gint)
         * @see     Character.isLetterOrDigit(gint)
         * @see     Character.isLowerCase(gint)
         * @see     Character.isTitleCase(gint)
         * @see     Character.isUnicodeIdentifierStart(gint)
         * @see     Character.isUpperCase(gint)
         */
        static gbool isLetter(gint codePoint);

        static gbool isNumber(gchar ch);

        static gbool isNumber(gint codePoint);

        /**
         * Determines if the specified character is a letter or digit.
         * <p>
         * A character is considered to be a letter or digit if either
         * <b> Character.isLetter(gchar ch)</b> or
         * <b> Character.isDigit(gchar ch)</b> returns
         * <b> true</b> for the character.
         *
         * <p><b>Note:</b> This method cannot handle <a
         * href="#supplementary"> supplementary characters</a>. To support
         * all Unicode characters, including supplementary characters, use
         * the <b style="color: orange"> isLetterOrDigit(gint)</b> method.
         *
         * @param   ch   the character to be tested.
         * @return  <b> true</b> if the character is a letter or digit;
         *          <b> false</b> otherwise.
         * @see     Character.isDigit(gchar)
         * @see     Character.isLetter(gchar)
         * @see     Character.isUnicodeIdentifierPart(gchar)
         */
        static gbool isLetterOrNumber(gchar ch);

        /**
         * Determines if the specified character (Unicode code point) is a letter or digit.
         * <p>
         * A character is considered to be a letter or digit if either
         * <b style="color: orange"> isLetter(gint) isLetter(codePoint)</b> or
         * <b style="color: orange"> isDigit(gint) isDigit(codePoint)</b> returns
         * <b> true</b> for the character.
         *
         * @param   codePoint the character (Unicode code point) to be tested.
         * @return  <b> true</b> if the character is a letter or digit;
         *          <b> false</b> otherwise.
         * @see     Character.isDigit(gint)
         * @see     Character.isLetter(gint)
         * @see     Character.isUnicodeIdentifierPart(gint)
         */
        static gbool isLetterOrNumber(gint codePoint);

        /**
         * Determines if the specified character (Unicode code point) is alphabetic.
         * <p>
         * A character is considered to be alphabetic if its general category type,
         * provided by <b style="color: orange"> Character.category(codePoint)</b>, is any of
         * the following:
         * <ul>
         * <li> <b> UPPERCASE_LETTER</b>
         * <li> <b> LOWERCASE_LETTER</b>
         * <li> <b> TITLECASE_LETTER</b>
         * <li> <b> MODIFIER_LETTER</b>
         * <li> <b> OTHER_LETTER</b>
         * <li> <b> LETTER_NUMBER</b>
         * </ul>
         * or it has contributory property Other_Alphabetic as defined by the
         * Unicode Standard.
         *
         * @param   codePoint the character (Unicode code point) to be tested.
         * @return  <b> true</b> if the character is a Unicode alphabet
         *          character, <b> false</b> otherwise.
         */
         static gbool isAlphabetic(gint codePoint);

        /**
         * Determines if the specified character (Unicode code point) is a CJKV
         * (Chinese, Japanese, Korean and Vietnamese) ideograph, as defined by
         * the Unicode Standard.
         *
         * @param   codePoint the character (Unicode code point) to be tested.
         * @return  <b> true</b> if the character is a Unicode ideograph
         *          character, <b> false</b> otherwise.
         */
         static gbool isIdeographic(gint codePoint);

        /**
         * Determines if the specified character is a mark.
         * <p>
         * A character is considered to be a mark if its general
         * category type, provided by category(ch), is any of the following:
         * <ul>
         * <li> COMBINING_SPACING_MARK
         * <li> ENCLOSING_MARK
         * <li> NON_SPACING_MARK
         * </ul>
         *
         * @param ch
         *          The character to be tested.
         */
        static gbool isMark(gchar ch);

        /**
         * Determines if the specified character (Unicode code point) is a mark.
         * <p>
         * A character is considered to be a mark if its general
         * category type, provided by category(codePoint), is any of the following:
         * <ul>
         * <li> COMBINING_SPACING_MARK
         * <li> ENCLOSING_MARK
         * <li> NON_SPACING_MARK
         * </ul>
         *
         * @param ch
         *         The character (Unicode code point) to be tested.
         */
        static gbool isMark(gint codePoint);

        /**
         * Determines if the specified character is a punctuation.
         * <p>
         * A character is considered to be a punctuation if its general
         * category type, provided by category(ch), is any of the following:
         * <ul>
         * <li> CONNECTOR_PUNCTUATION
         * <li> DASH_PUNCTUATION
         * <li> START_PUNCTUATION
         * <li> END_PUNCTUATION
         * <li> INITIAL_QUOTE_PUNCTUATION
         * <li> FINAL_QUOTE_PUNCTUATION
         * <li> OTHER_PUNCTUATION
         * </ul>
         *
         * @param ch
         *          The character to be tested.
         */
        static gbool isPunctuation(gchar ch);

        /**
         * Determines if the specified character (Unicode code point) is a punctuation.
         * <p>
         * A character is considered to be a punctuation if its general
         * category type, provided by category(codePoint), is any of the following:
         * <ul>
         * <li> CONNECTOR_PUNCTUATION
         * <li> DASH_PUNCTUATION
         * <li> START_PUNCTUATION
         * <li> END_PUNCTUATION
         * <li> INITIAL_QUOTE_PUNCTUATION
         * <li> FINAL_QUOTE_PUNCTUATION
         * <li> OTHER_PUNCTUATION
         * </ul>
         *
         * @param ch
         *         The character (Unicode code point) to be tested.
         */
        static gbool isPunctuation(gint codePoint);

        /**
         * Determines if the specified character is printable.
         * <p>
         * A character is considered to be a printable if its general
         * category type, provided by category(ch), is any of the following:
         * <ul>
         * <li> FORMAT
         * <li> CONTROL
         * <li> SURROGATE
         * <li> PRIVATE_USE
         * <li> UNASSIGNED
         * </ul>
         *
         * @param ch
         *          The character to be tested.
         */
        static gbool isPrintable(gchar ch);

        /**
         * Determines if the specified character (Unicode code point) is printable.
         * <p>
         * A character is considered to be a printable if its general
         * category type, provided by category(codePoint), is not any of the following:
         * <ul>
         * <li> FORMAT
         * <li> CONTROL
         * <li> SURROGATE
         * <li> PRIVATE_USE
         * <li> UNASSIGNED
         * </ul>
         *
         * @param ch
         *         The character (Unicode code point) to be tested.
         */
        static gbool isPrintable(gint codePoint);

        /**
         * Determines if the specified character is white space.
         * A character is a whitespace character if and only if it satisfies
         * one of the following criteria:
         * <ul>
         * <li> It is a Unicode space character (<b> SPACE_SEPARATOR</b>,
         *      <b> LINE_SEPARATOR</b>, or <b> PARAGRAPH_SEPARATOR</b>)
         *      but is not also a non-breaking space (<b> '&#92;u00A0'</b>,
         *      <b> '&#92;u2007'</b>, <b> '&#92;u202F'</b>).
         * <li> It is <b> '&#92;t'</b>, U+0009 HORIZONTAL TABULATION.
         * <li> It is <b> '&#92;n'</b>, U+000A LINE FEED.
         * <li> It is <b> '&#92;u000B'</b>, U+000B VERTICAL TABULATION.
         * <li> It is <b> '&#92;f'</b>, U+000C FORM FEED.
         * <li> It is <b> '&#92;r'</b>, U+000D CARRIAGE RETURN.
         * <li> It is <b> '&#92;u001C'</b>, U+001C FILE SEPARATOR.
         * <li> It is <b> '&#92;u001D'</b>, U+001D GROUP SEPARATOR.
         * <li> It is <b> '&#92;u001E'</b>, U+001E RECORD SEPARATOR.
         * <li> It is <b> '&#92;u001F'</b>, U+001F UNIT SEPARATOR.
         * </ul>
         *
         * <p><b>Note:</b> This method cannot handle <a
         * href="#supplementary"> supplementary characters</a>. To support
         * all Unicode characters, including supplementary characters, use
         * the <b style="color: orange"> isSpace(gint)</b> method.
         *
         * @param   ch the character to be tested.
         * @return  <b> true</b> if the character is a whitespace
         *          character; <b> false</b> otherwise.
         * @see     Character.isSpaceChar(gchar)
         */
        static gbool isSpace(gchar ch);

        /**
         * Determines if the specified character (Unicode code point) is
         * white space.  A character is a
         * whitespace character if and only if it satisfies one of the
         * following criteria:
         * <ul>
         * <li> It is a Unicode space character (<b style="color: orange"> #SPACE_SEPARATOR</b>,
         *      <b style="color: orange"> #LINE_SEPARATOR</b>, or <b style="color: orange"> #PARAGRAPH_SEPARATOR</b>)
         *      but is not also a non-breaking space (<b> '&#92;u00A0'</b>,
         *      <b> '&#92;u2007'</b>, <b> '&#92;u202F'</b>).
         * <li> It is <b> '&#92;t'</b>, U+0009 HORIZONTAL TABULATION.
         * <li> It is <b> '&#92;n'</b>, U+000A LINE FEED.
         * <li> It is <b> '&#92;u000B'</b>, U+000B VERTICAL TABULATION.
         * <li> It is <b> '&#92;f'</b>, U+000C FORM FEED.
         * <li> It is <b> '&#92;r'</b>, U+000D CARRIAGE RETURN.
         * <li> It is <b> '&#92;u001C'</b>, U+001C FILE SEPARATOR.
         * <li> It is <b> '&#92;u001D'</b>, U+001D GROUP SEPARATOR.
         * <li> It is <b> '&#92;u001E'</b>, U+001E RECORD SEPARATOR.
         * <li> It is <b> '&#92;u001F'</b>, U+001F UNIT SEPARATOR.
         * </ul>
         *
         * @param   codePoint the character (Unicode code point) to be tested.
         * @return  <b> true</b> if the character is a whitespace
         *          character; <b> false</b> otherwise.
         * @see     Character.isSpaceChar(gint)
         */
        static gbool isSpace(gint codePoint);

        /**
         * Determines if the referenced character (Unicode code point) is an ISO control
         * character.  A character is considered to be an ISO control
         * character if its code is in the range <b> '&#92;u0000'</b>
         * through <b> '&#92;u001F'</b> or in the range
         * <b> '&#92;u007F'</b> through <b> '&#92;u009F'</b>.
         *
         * @param   codePoint the character (Unicode code point) to be tested.
         * @return  <b> true</b> if the character is an ISO control character;
         *          <b> false</b> otherwise.
         * @see     Character.isSpaceChar(gint)
         * @see     Character.isWhitespace(gint)
         */
         static gbool isControl(gint codePoint);

        /**
         * Determines if the specified character is a symbol.
         * <p>
         * A character is considered to be a symbol if its general
         * category type, provided by category(ch), is any of the following:
         * <ul>
         * <li> CURRENCY_SYMBOL
         * <li> MATH_SYMBOL
         * <li> MODIFIER_SYMBOL
         * <li> OTHER_SYMBOL
         * </ul>
         *
         * @param ch
         *          The character to be tested.
         */
        static gbool isSymbol(gchar ch);

        /**
         * Determines if the specified character (Unicode code point) is a symbol.
         * <p>
         * A character is considered to be a symbol if its general
         * category type, provided by category(codePoint), is any of the following:
         * <ul>
         * <li> CURRENCY_SYMBOL
         * <li> MATH_SYMBOL
         * <li> MODIFIER_SYMBOL
         * <li> OTHER_SYMBOL
         * </ul>
         *
         * @param ch
         *         The character (Unicode code point) to be tested.
         */
        static gbool isSymbol(gint codePoint);

        /**
         * Return the sharable copy of this Character
         */
        Object &clone() const override;

        /**
         * Converts the character argument to lowercase using case
         * mapping information from the UnicodeData file.
         * <p>
         * Note that
         * <b> Character.isLowerCase(Character.toLowerCase(ch))</b>
         * does not always return <b> true</b> for some ranges of
         * characters, particularly those that are symbols or ideographs.
         *
         * <p>In general, <b style="color: orange"> String#toLowerCase()</b> should be used to map
         * characters to lowercase. <b> String</b> case mapping methods
         * have several benefits over <b> Character</b> case mapping methods.
         * <b> String</b> case mapping methods can perform locale-sensitive
         * mappings, context-sensitive mappings, and 1:M character mappings, whereas
         * the <b> Character</b> case mapping methods cannot.
         *
         * <p><b>Note:</b> This method cannot handle <a
         * href="#supplementary"> supplementary characters</a>. To support
         * all Unicode characters, including supplementary characters, use
         * the <b style="color: orange"> #toLowerCase(gint)</b> method.
         *
         * @param   ch   the character to be converted.
         * @return  the lowercase equivalent of the character, if any;
         *          otherwise, the character itself.
         * @see     Character.isLowerCase(gchar)
         * @see     String#toLowerCase()
         */
        static gchar toLowerCase(gchar ch);

        /**
         * Converts the character (Unicode code point) argument to
         * lowercase using case mapping information from the UnicodeData
         * file.
         *
         * <p> Note that
         * <b> Character.isLowerCase(Character.toLowerCase(codePoint))</b>
         * does not always return <b> true</b> for some ranges of
         * characters, particularly those that are symbols or ideographs.
         *
         * <p>In general, <b style="color: orange"> String#toLowerCase()</b> should be used to map
         * characters to lowercase. <b> String</b> case mapping methods
         * have several benefits over <b> Character</b> case mapping methods.
         * <b> String</b> case mapping methods can perform locale-sensitive
         * mappings, context-sensitive mappings, and 1:M character mappings, whereas
         * the <b> Character</b> case mapping methods cannot.
         *
         * @param   codePoint   the character (Unicode code point) to be converted.
         * @return  the lowercase equivalent of the character (Unicode code
         *          point), if any; otherwise, the character itself.
         * @see     Character.isLowerCase(gint)
         * @see     String#toLowerCase()
         */
        static gint toLowerCase(gint codePoint);

        /**
         * Converts the character argument to uppercase using case mapping
         * information from the UnicodeData file.
         * <p>
         * Note that
         * <b> Character.isUpperCase(Character.toUpperCase(ch))</b>
         * does not always return <b> true</b> for some ranges of
         * characters, particularly those that are symbols or ideographs.
         *
         * <p>In general, <b style="color: orange"> String#toUpperCase()</b> should be used to map
         * characters to uppercase. <b> String</b> case mapping methods
         * have several benefits over <b> Character</b> case mapping methods.
         * <b> String</b> case mapping methods can perform locale-sensitive
         * mappings, context-sensitive mappings, and 1:M character mappings, whereas
         * the <b> Character</b> case mapping methods cannot.
         *
         * <p><b>Note:</b> This method cannot handle <a
         * href="#supplementary"> supplementary characters</a>. To support
         * all Unicode characters, including supplementary characters, use
         * the <b style="color: orange"> #toUpperCase(gint)</b> method.
         *
         * @param   ch   the character to be converted.
         * @return  the uppercase equivalent of the character, if any;
         *          otherwise, the character itself.
         * @see     Character.isUpperCase(gchar)
         * @see     String#toUpperCase()
         */
        static gchar toUpperCase(gchar ch);

        /**
         * Converts the character (Unicode code point) argument to
         * uppercase using case mapping information from the UnicodeData
         * file.
         *
         * <p>Note that
         * <b> Character.isUpperCase(Character.toUpperCase(codePoint))</b>
         * does not always return <b> true</b> for some ranges of
         * characters, particularly those that are symbols or ideographs.
         *
         * <p>In general, <b style="color: orange"> String#toUpperCase()</b> should be used to map
         * characters to uppercase. <b> String</b> case mapping methods
         * have several benefits over <b> Character</b> case mapping methods.
         * <b> String</b> case mapping methods can perform locale-sensitive
         * mappings, context-sensitive mappings, and 1:M character mappings, whereas
         * the <b> Character</b> case mapping methods cannot.
         *
         * @param   codePoint   the character (Unicode code point) to be converted.
         * @return  the uppercase equivalent of the character, if any;
         *          otherwise, the character itself.
         * @see     Character.isUpperCase(gint)
         * @see     String#toUpperCase()
         */
        static gint toUpperCase(gint codePoint);

        /**
         * Converts the character argument to titlecase using case mapping
         * information from the UnicodeData file. If a character has no
         * explicit titlecase mapping and is not itself a titlecase gchar
         * according to UnicodeData, then the uppercase mapping is
         * returned as an equivalent titlecase mapping. If the
         * <b> gchar</b> argument is already a titlecase
         * <b> gchar</b>, the same <b> gchar</b> value will be
         * returned.
         * <p>
         * Note that
         * <b> Character.isTitleCase(Character.toTitleCase(ch))</b>
         * does not always return <b> true</b> for some ranges of
         * characters.
         *
         * <p><b>Note:</b> This method cannot handle <a
         * href="#supplementary"> supplementary characters</a>. To support
         * all Unicode characters, including supplementary characters, use
         * the <b style="color: orange"> #toTitleCase(gint)</b> method.
         *
         * @param   ch   the character to be converted.
         * @return  the titlecase equivalent of the character, if any;
         *          otherwise, the character itself.
         * @see     Character.isTitleCase(gchar)
         * @see     Character.toLowerCase(gchar)
         * @see     Character.toUpperCase(gchar)
         */
        static gchar toTitleCase(gchar ch);

        /**
         * Converts the character (Unicode code point) argument to titlecase using case mapping
         * information from the UnicodeData file. If a character has no
         * explicit titlecase mapping and is not itself a titlecase gchar
         * according to UnicodeData, then the uppercase mapping is
         * returned as an equivalent titlecase mapping. If the
         * character argument is already a titlecase
         * character, the same character value will be
         * returned.
         *
         * <p>Note that
         * <b> Character.isTitleCase(Character.toTitleCase(codePoint))</b>
         * does not always return <b> true</b> for some ranges of
         * characters.
         *
         * @param   codePoint   the character (Unicode code point) to be converted.
         * @return  the titlecase equivalent of the character, if any;
         *          otherwise, the character itself.
         * @see     Character.isTitleCase(gint)
         * @see     Character.toLowerCase(gint)
         * @see     Character.toUpperCase(gint)
         */
        static gint toTitleCase(gint codePoint);

        static gchar toCaseFold(gchar ch);

        static gint toCaseFold(gint codePoint);

        /**
         * Returns the <b> gint</b> value that the specified Unicode
         * character represents. For example, the character
         * <b> '&#92;u216C'</b> (the roman numeral fifty) will return
         * an gint with a value of 50.
         * <p>
         * The letters A-Z in their uppercase (<b> '&#92;u0041'</b> through
         * <b> '&#92;u005A'</b>), lowercase
         * (<b> '&#92;u0061'</b> through <b> '&#92;u007A'</b>), and
         * full width variant (<b> '&#92;uFF21'</b> through
         * <b> '&#92;uFF3A'</b> and <b> '&#92;uFF41'</b> through
         * <b> '&#92;uFF5A'</b>) forms have numeric values from 10
         * through 35. This is independent of the Unicode specification,
         * which does not assign numeric values to these <b> gchar</b>
         * values.
         * <p>
         * If the character does not have a numeric value, then -1 is returned.
         * If the character has a numeric value that cannot be represented as a
         * nonnegative integer (for example, a fractional value), then -2
         * is returned.
         *
         * <p><b>Note:</b> This method cannot handle <a
         * href="#supplementary"> supplementary characters</a>. To support
         * all Unicode characters, including supplementary characters, use
         * the <b style="color: orange"> numericValue(gint)</b> method.
         *
         * @param   ch      the character to be converted.
         * @return  the numeric value of the character, as a nonnegative <b> gint</b>
         *          value; -2 if the character has a numeric value but the value
         *          can not be represented as a nonnegative <b> gint</b> value;
         *          -1 if the character has no numeric value.
         * @see     Character.forDigit(gint, gint)
         * @see     Character.isDigit(gchar)
         */
        static gint numericValue(gchar ch);

        /**
         * Returns the numeric value of the character <b> ch</b> in the
         * specified base.
         * <p>
         * If the base is not in the range <b> MIN_BASE</b> &le;
         * <b> base</b> &le; <b> MAX_BASE</b> or if the
         * value of <b> ch</b> is not a valid digit in the specified
         * base, <b> -1</b> is returned. A character is a valid digit
         * if at least one of the following is true:
         * <ul>
         * <li>The method <b> isDigit</b> is <b> true</b> of the character
         *     and the Unicode decimal digit value of the character (or its
         *     single-character decomposition) is less than the specified base.
         *     In this case the decimal digit value is returned.
         * <li>The character is one of the uppercase Latin letters
         *     <b> 'A'</b> through <b> 'Z'</b> and its code is less than
         *     <b> base + 'A' - 10</b>.
         *     In this case, <b> ch - 'A' + 10</b>
         *     is returned.
         * <li>The character is one of the lowercase Latin letters
         *     <b> 'a'</b> through <b> 'z'</b> and its code is less than
         *     <b> base + 'a' - 10</b>.
         *     In this case, <b> ch - 'a' + 10</b>
         *     is returned.
         * <li>The character is one of the fullwidth uppercase Latin letters A
         *     (<b> '&#92;uFF21'</b>) through Z (<b> '&#92;uFF3A'</b>)
         *     and its code is less than
         *     <b> base + '&#92;uFF21' - 10</b>.
         *     In this case, <b> ch - '&#92;uFF21' + 10</b>
         *     is returned.
         * <li>The character is one of the fullwidth lowercase Latin letters a
         *     (<b> '&#92;uFF41'</b>) through z (<b> '&#92;uFF5A'</b>)
         *     and its code is less than
         *     <b> base + '&#92;uFF41' - 10</b>.
         *     In this case, <b> ch - '&#92;uFF41' + 10</b>
         *     is returned.
         * </ul>
         *
         * <p><b>Note:</b> This method cannot handle <a
         * href="#supplementary"> supplementary characters</a>. To support
         * all Unicode characters, including supplementary characters, use
         * the <b style="color: orange"> #digit(gint, gint)</b> method.
         *
         * @param   ch      the character to be converted.
         * @param   base   the base.
         * @return  the numeric value represented by the character in the
         *          specified base.
         * @see     Character.forDigit(gint, gint)
         * @see     Character.isDigit(gchar)
         */
        static gint numericValue(gchar ch, gint base);

        /**
         * Returns the <b> gint</b> value that the specified
         * character (Unicode code point) represents. For example, the character
         * <b> '&#92;u216C'</b> (the Roman numeral fifty) will return
         * an <b> gint</b> with a value of 50.
         * <p>
         * The letters A-Z in their uppercase (<b> '&#92;u0041'</b> through
         * <b> '&#92;u005A'</b>), lowercase
         * (<b> '&#92;u0061'</b> through <b> '&#92;u007A'</b>), and
         * full width variant (<b> '&#92;uFF21'</b> through
         * <b> '&#92;uFF3A'</b> and <b> '&#92;uFF41'</b> through
         * <b> '&#92;uFF5A'</b>) forms have numeric values from 10
         * through 35. This is independent of the Unicode specification,
         * which does not assign numeric values to these <b> gchar</b>
         * values.
         * <p>
         * If the character does not have a numeric value, then -1 is returned.
         * If the character has a numeric value that cannot be represented as a
         * nonnegative integer (for example, a fractional value), then -2
         * is returned.
         *
         * @param   codePoint the character (Unicode code point) to be converted.
         * @return  the numeric value of the character, as a nonnegative <b> gint</b>
         *          value; -2 if the character has a numeric value but the value
         *          can not be represented as a nonnegative <b> gint</b> value;
         *          -1 if the character has no numeric value.
         * @see     Character.forDigit(gint, gint)
         * @see     Character.isDigit(gint)
         */
        static gint numericValue(gint codePoint);

        /**
         * Returns the numeric value of the specified character (Unicode
         * code point) in the specified base.
         *
         * <p>If the base is not in the range <b> MIN_BASE</b> &le;
         * <b> base</b> &le; <b> MAX_BASE</b> or if the
         * character is not a valid digit in the specified
         * base, <b> -1</b> is returned. A character is a valid digit
         * if at least one of the following is true:
         * <ul>
         * <li>The method <b style="color: orange"> isDigit(codePoint)</b> is <b> true</b> of the character
         *     and the Unicode decimal digit value of the character (or its
         *     single-character decomposition) is less than the specified base.
         *     In this case the decimal digit value is returned.
         * <li>The character is one of the uppercase Latin letters
         *     <b> 'A'</b> through <b> 'Z'</b> and its code is less than
         *     <b> base + 'A' - 10</b>.
         *     In this case, <b> codePoint - 'A' + 10</b>
         *     is returned.
         * <li>The character is one of the lowercase Latin letters
         *     <b> 'a'</b> through <b> 'z'</b> and its code is less than
         *     <b> base + 'a' - 10</b>.
         *     In this case, <b> codePoint - 'a' + 10</b>
         *     is returned.
         * <li>The character is one of the fullwidth uppercase Latin letters A
         *     (<b> '&#92;uFF21'</b>) through Z (<b> '&#92;uFF3A'</b>)
         *     and its code is less than
         *     <b> base + '&#92;uFF21' - 10</b>.
         *     In this case,
         *     <b> codePoint - '&#92;uFF21' + 10</b>
         *     is returned.
         * <li>The character is one of the fullwidth lowercase Latin letters a
         *     (<b> '&#92;uFF41'</b>) through z (<b> '&#92;uFF5A'</b>)
         *     and its code is less than
         *     <b> base + '&#92;uFF41'- 10</b>.
         *     In this case,
         *     <b> codePoint - '&#92;uFF41' + 10</b>
         *     is returned.
         * </ul>
         *
         * @param   codePoint the character (Unicode code point) to be converted.
         * @param   base   the base.
         * @return  the numeric value represented by the character in the
         *          specified base.
         * @see     Character.forDigit(gint, gint)
         * @see     Character.isDigit(gint)
         */
        static gint numericValue(gint codePoint, gint base);

        /**
         * Returns a value indicating a character's general category.
         *
         * <p><b>Note:</b> This method cannot handle <a
         * href="#supplementary"> supplementary characters</a>. To support
         * all Unicode characters, including supplementary characters, use
         * the <b style="color: orange"> category(gint)</b> method.
         *
         * @param   ch      the character to be tested.
         * @return  a value of type <b> gint</b> representing the
         *          character's general category.
         * @see     Character::Category::COMBINING_SPACING_MARK
         * @see     Character::Category::CONNECTOR_PUNCTUATION
         * @see     Character::Category::CONTROL
         * @see     Character::Category::CURRENCY_SYMBOL
         * @see     Character::Category::DASH_PUNCTUATION
         * @see     Character::Category::DECIMAL_DIGIT_NUMBER
         * @see     Character::Category::ENCLOSING_MARK
         * @see     Character::Category::END_PUNCTUATION
         * @see     Character::Category::FINAL_QUOTE_PUNCTUATION
         * @see     Character::Category::FORMAT
         * @see     Character::Category::INITIAL_QUOTE_PUNCTUATION
         * @see     Character::Category::LETTER_NUMBER
         * @see     Character::Category::LINE_SEPARATOR
         * @see     Character::Category::LOWERCASE_LETTER
         * @see     Character::Category::MATH_SYMBOL
         * @see     Character::Category::MODIFIER_LETTER
         * @see     Character::Category::MODIFIER_SYMBOL
         * @see     Character::Category::NON_SPACING_MARK
         * @see     Character::Category::OTHER_LETTER
         * @see     Character::Category::OTHER_NUMBER
         * @see     Character::Category::OTHER_PUNCTUATION
         * @see     Character::Category::OTHER_SYMBOL
         * @see     Character::Category::PARAGRAPH_SEPARATOR
         * @see     Character::Category::PRIVATE_USE
         * @see     Character::Category::SPACE_SEPARATOR
         * @see     Character::Category::START_PUNCTUATION
         * @see     Character::Category::SURROGATE
         * @see     Character::Category::TITLECASE_LETTER
         * @see     Character::Category::UNASSIGNED
         * @see     Character::Category::UPPERCASE_LETTER
         */
        static Category category(gchar ch);

        /**
         * Returns a value indicating a character's general category.
         *
         * @param   codePoint the character (Unicode code point) to be tested.
         * @return  a value of type <b> gint</b> representing the
         *          character's general category.
         * @see     Character::Category::COMBINING_SPACING_MARK
         * @see     Character::Category::CONNECTOR_PUNCTUATION
         * @see     Character::Category::CONTROL
         * @see     Character::Category::CURRENCY_SYMBOL
         * @see     Character::Category::DASH_PUNCTUATION
         * @see     Character::Category::DECIMAL_DIGIT_NUMBER
         * @see     Character::Category::ENCLOSING_MARK
         * @see     Character::Category::END_PUNCTUATION
         * @see     Character::Category::FINAL_QUOTE_PUNCTUATION
         * @see     Character::Category::FORMAT FORMAT
         * @see     Character::Category::INITIAL_QUOTE_PUNCTUATION
         * @see     Character::Category::LETTER_NUMBER
         * @see     Character::Category::LINE_SEPARATOR
         * @see     Character::Category::LOWERCASE_LETTER
         * @see     Character::Category::MATH_SYMBOL
         * @see     Character::Category::MODIFIER_LETTER
         * @see     Character::Category::MODIFIER_SYMBOL
         * @see     Character::Category::NON_SPACING_MARK
         * @see     Character::Category::OTHER_LETTER
         * @see     Character::Category::OTHER_NUMBER
         * @see     Character::Category::OTHER_PUNCTUATION
         * @see     Character::Category::OTHER_SYMBOL
         * @see     Character::Category::PARAGRAPH_SEPARATOR
         * @see     Character::Category::PRIVATE_USE
         * @see     Character::Category::SPACE_SEPARATOR
         * @see     Character::Category::START_PUNCTUATION
         * @see     Character::Category::SURROGATE
         * @see     Character::Category::TITLECASE_LETTER
         * @see     Character::Category::UNASSIGNED
         * @see     Character::Category::UPPERCASE_LETTER
         */
        static Category category(gint codePoint);

        /**
         * Determines the character representation for a specific digit in
         * the specified base. If the value of <b> base</b> is not a
         * valid base, or the value of <b> digit</b> is not a valid
         * digit in the specified base, the null character
         * (<b> '&#92;u0000'</b>) is returned.
         * <p>
         * The <b> base</b> argument is valid if it is greater than or
         * equal to <b> MIN_BASE</b> and less than or equal to
         * <b> MAX_BASE</b>. The <b> digit</b> argument is valid if
         * <b> 0 <= digit < base</b>.
         * <p>
         * If the digit is less than 10, then
         * <b> '0' + digit</b> is returned. Otherwise, the value
         * <b> 'a' + digit - 10</b> is returned.
         *
         * @param   digit   the number to convert to a character.
         * @param   base   the base.
         * @return  the <b> gchar</b> representation of the specified digit
         *          in the specified base.
         * @see     Character.MIN_BASE
         * @see     Character.MAX_BASE
         * @see     Character.digit(gchar, gint)
         */
         static gchar forDigit(gint digit, gint base) {
            if ((digit >= base) || (digit < 0)) {
                return '\0';
            }
            if ((base < 2) || (base > 36)) {
                return '\0';
            }
            if (digit < 10) {
                return (gchar)('0' + digit);
            }
            return (gchar)('a' - 10 + digit);
         }

        /**
         * Returns the Unicode directionality property for the given
         * character.  Character directionality is used to calculate the
         * visual ordering of text. The directionality value of undefined
         * <b> gchar</b> values is <b> Directionality::UNDEFINED</b>.
         *
         * <p><b>Note:</b> This method cannot handle <a
         * href="#supplementary"> supplementary characters</a>. To support
         * all Unicode characters, including supplementary characters, use
         * the <b style="color: orange"> directionality(gint)</b> method.
         *
         * @param  ch <b> gchar</b> for which the directionality property
         *            is requested.
         * @return the directionality property of the <b> gchar</b> value.
         *
         * @see Character::Directionality::UNDEFINED
         * @see Character::Directionality::LEFT_TO_RIGHT
         * @see Character::Directionality::RIGHT_TO_LEFT
         * @see Character::Directionality::RIGHT_TO_LEFT_ARABIC
         * @see Character::Directionality::EUROPEAN_NUMBER
         * @see Character::Directionality::EUROPEAN_NUMBER_SEPARATOR
         * @see Character::Directionality::EUROPEAN_NUMBER_TERMINATOR
         * @see Character::Directionality::ARABIC_NUMBER
         * @see Character::Directionality::COMMON_NUMBER_SEPARATOR
         * @see Character::Directionality::NONSPACING_MARK
         * @see Character::Directionality::BOUNDARY_NEUTRAL
         * @see Character::Directionality::PARAGRAPH_SEPARATOR
         * @see Character::Directionality::SEGMENT_SEPARATOR
         * @see Character::Directionality::WHITESPACE
         * @see Character::Directionality::OTHER_NEUTRALS
         * @see Character::Directionality::LEFT_TO_RIGHT_EMBEDDING
         * @see Character::Directionality::LEFT_TO_RIGHT_OVERRIDE
         * @see Character::Directionality::RIGHT_TO_LEFT_EMBEDDING
         * @see Character::Directionality::RIGHT_TO_LEFT_OVERRIDE
         * @see Character::Directionality::POP_DIRECTIONAL_FORMAT
         * @see Character::Directionality::LEFT_TO_RIGHT_ISOLATE
         * @see Character::Directionality::RIGHT_TO_LEFT_ISOLATE
         * @see Character::Directionality::FIRST_STRONG_ISOLATE
         * @see Character::Directionality::POP_DIRECTIONAL_ISOLATE
         */
        static Directionality directionality(gchar ch);

        /**
         * Returns the Unicode directionality property for the given
         * character (Unicode code point).  Character directionality is
         * used to calculate the visual ordering of text. The
         * directionality value of undefined character is <b style="color: orange">
         * Directionality::UNDEFINED</b>.
         *
         * @param   codePoint the character (Unicode code point) for which
         *          the directionality property is requested.
         * @return the directionality property of the character.
         *
         * @see Character::Directionality::UNDEFINED
         * @see Character::Directionality::LEFT_TO_RIGHT
         * @see Character::Directionality::RIGHT_TO_LEFT
         * @see Character::Directionality::RIGHT_TO_LEFT_ARABIC
         * @see Character::Directionality::EUROPEAN_NUMBER
         * @see Character::Directionality::EUROPEAN_NUMBER_SEPARATOR
         * @see Character::Directionality::EUROPEAN_NUMBER_TERMINATOR
         * @see Character::Directionality::ARABIC_NUMBER
         * @see Character::Directionality::COMMON_NUMBER_SEPARATOR
         * @see Character::Directionality::NONSPACING_MARK
         * @see Character::Directionality::BOUNDARY_NEUTRAL
         * @see Character::Directionality::PARAGRAPH_SEPARATOR
         * @see Character::Directionality::SEGMENT_SEPARATOR
         * @see Character::Directionality::WHITESPACE
         * @see Character::Directionality::OTHER_NEUTRALS
         * @see Character::Directionality::LEFT_TO_RIGHT_EMBEDDING
         * @see Character::Directionality::LEFT_TO_RIGHT_OVERRIDE
         * @see Character::Directionality::RIGHT_TO_LEFT_EMBEDDING
         * @see Character::Directionality::RIGHT_TO_LEFT_OVERRIDE
         * @see Character::Directionality::POP_DIRECTIONAL_FORMAT
         * @see Character::Directionality::LEFT_TO_RIGHT_ISOLATE
         * @see Character::Directionality::RIGHT_TO_LEFT_ISOLATE
         * @see Character::Directionality::FIRST_STRONG_ISOLATE
         * @see Character::Directionality::POP_DIRECTIONAL_ISOLATE
         */
        static Directionality directionality(gint codePoint);

        /**
         * Returns the combining class of the given character
         * @param ch character to retrieve combining class of
         */
        static CombiningClass combiningClass(gint codePoint);

        /**
         * Determines whether the character is mirrored according to the
         * Unicode specification.  Mirrored characters should have their
         * glyphs horizontally mirrored when displayed in text that is
         * right-to-left.  For example, <b> '&#92;u0028'</b> LEFT
         * PARENTHESIS is semantically defined to be an <i>opening
         * parenthesis</i>.  This will appear as a "(" in text that is
         * left-to-right but as a ")" in text that is right-to-left.
         *
         * <p><b>Note:</b> This method cannot handle <a
         * href="#supplementary"> supplementary characters</a>. To support
         * all Unicode characters, including supplementary characters, use
         * the <b style="color: orange"> isMirrored(gint)</b> method.
         *
         * @param  ch <b> gchar</b> for which the mirrored property is requested
         * @return <b> true</b> if the gchar is mirrored, <b> false</b>
         *         if the <b> gchar</b> is not mirrored or is not defined.
         */
        static gbool isMirrored(gchar ch);

        /**
         * Determines whether the specified character (Unicode code point)
         * is mirrored according to the Unicode specification.  Mirrored
         * characters should have their glyphs horizontally mirrored when
         * displayed in text that is right-to-left.  For example,
         * <b> '&#92;u0028'</b> LEFT PARENTHESIS is semantically
         * defined to be an <i>opening parenthesis</i>.  This will appear
         * as a "(" in text that is left-to-right but as a ")" in text
         * that is right-to-left.
         *
         * @param   codePoint the character (Unicode code point) to be tested.
         * @return  <b> true</b> if the character is mirrored, <b> false</b>
         *          if the character is not mirrored or is not defined.
         */
        static gbool isMirrored(gint codePoint);

        /**
         * Return the complement of specified character if isMirrored(x) is true
         * for all x or return itself
         *
         * @param ch
         *          The character for which the mirrored property is requested
         * @see isMirrored
         */
        static gchar complement(gchar ch);

        /**
         * Return the complement of specified character if isMirrored(x) is true
         * for all x or return itself
         *
         * @param codePoint
         *          The character (Unicode code point) for which the mirrored property
         *          is requested
         *
         * @see isMirrored
         */
        static gchar complement(gint codePoint);

        /**
         * Return true if specified character (unicode code point) is mirrored
         * @param ch
         *          The gchar to be tested
         */
        static CORE_FAST gbool isNonCharacter(gchar ch) {
            return isNonCharacter((gint) ch);
        }

        /**
         * Return true if specified character (unicode code point) is mirrored
         * @param ch
         *          The character (unicode code point) to be tested
         */
        static CORE_FAST gbool isNonCharacter(gint codePoint) {
            return ((0xFDD0 <= codePoint) && (codePoint <= 0xFDEF)) ||
                   (0xFFFE <= codePoint && ((codePoint & 0xFFFFFFFE) == 0xFFFE));
        }

        /**
         * Return unicode block of specified character (unicode code point)
         *
         * @param ch
         *          The gchar value
         */
        static UnicodeBlock block(gchar ch);

        /**
         * Return unicode block of specified character (unicode code point)
         *
         * @param codePoint
         *          The character (unicode code point)
         */
        static UnicodeBlock block(gint codePoint);

        /**
         * Return unicode script of specified character (unicode code point)
         *
         * @param ch
         *          The gchar value
         */
        static UnicodeScript script(gchar ch) {
            return script((gint) ch);
        }

        /**
         * Return unicode script of specified character (unicode code point)
         *
         * @param codePoint
         *          The character (unicode code point)
         */
        static UnicodeScript script(gint codePoint);

        /**
         * Return high part of specified gchar value.
         *
         * @param ch
         *         The given character
         */
        static CORE_FAST gbyte highByte(gchar c) {
            return (gbyte) ((c & 0xFF00) >> 8);
        }

        /**
         * Return low part of specified gchar value.
         *
         * @param ch
         *         The given character
         */
        static CORE_FAST gbyte lowByte(gchar c) {
            return (gbyte) (c & 0x00FF);
        }

        /**
         * Return gchar value formed by specified bytes.
         *
         * @param high
         *          The high part
         * @param low
         *          The low part
         */
        static CORE_FAST gchar joinBytes(gbyte high, gbyte low) {
            return (gchar) (((high & 0xFF) << 8) | (low & 0xFF));
        }

        /**
         * Returns the value obtained by reversing the order of the bytes in the
         * specified <b> gchar</b> value.
         *
         * @param ch The <b> gchar</b> of which to reverse the byte order.
         * @return the value obtained by reversing (or, equivalently, swapping)
         *     the bytes in the specified <b> gchar</b> value.
         */
        static CORE_FAST gchar reverseBytes(gchar ch) {
            return (gchar) (((ch & 0xFF00) >> 8) | (ch << 8));
        }

        /**
         * Returns the name of the specified character
         * <b> codePoint</b>, or null if the code point is
         * <b style="color: orange"> #UNASSIGNED unassigned</b>.
         * <p>
         * If the specified character is not assigned a name by
         * the <i>UnicodeData</i> file (part of the Unicode Character
         * Database maintained by the Unicode Consortium), the returned
         * name is the same as the result of the expression:
         *
         * <blockquote><b>
         *     Character.UnicodeBlock.of(codePoint).toString().replace('_', ' ')
         *     + " "
         *     + Integer.toHexString(codePoint).toUpperCase(Locale.ROOT);
         *
         * </b></blockquote>
         *
         * For the <b> codePoint</b>s in the <i>UnicodeData</i> file, the name
         * returned by this method follows the naming scheme in the
         * "Unicode Name Property" section of the Unicode Standard. For other
         * code points, such as Hangul/Ideographs, The name generation rule above
         * differs from the one defined in the Unicode Standard.
         *
         * @param  codePoint the character (Unicode code point)
         *
         * @return the name of the specified character, or null if
         *         the code point is unassigned.
         *
         * @throws ArgumentException if the specified
         *            <b> codePoint</b> is not a valid Unicode
         *            code point.
         */
         static String unicodeName(gint codePoint);

        /**
         * Return decomposition mode of specified gchar
         *
         * @param ch
         *          The gchar value
         */
        static Decomposition decomposition(gchar ch);

        /**
         * Return decomposition mode of specified character
         * (Unicode code point)
         *
         * @param codePoint
         *          The character
         */
        static Decomposition decomposition(gint codePoint);

        /**
         * Split specified gchar to gchar that compose iterator in one string.
         *
         * @param ch
         *          The composed gchar value
         */
        static String decompose(gchar ch);

        /**
         * Split specified character (unicode code point) to characters that compose iterator in one string.
         * Note: If the given character (code point) is not decomposable (Character.decomposition(ch) == UNDEFINED)
         * this method return the empty string.
         *
         * @param codePoint
         *          The composed character (Unicode code point)
         */
        static String decompose(gint codePoint);
    };

} // core

#endif //CORE23_CHARACTER_H
