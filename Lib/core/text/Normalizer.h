//
// Created by T.N.Brunshweeck on 26/01/2024.
//

#ifndef MYHARDLINK_NORMALIZER_H
#define MYHARDLINK_NORMALIZER_H

#include <core/String.h>

namespace core {
    namespace text {

        /**
         * This class provides the method <b> normalize</b> which transforms Unicode
         * text into an equivalent composed or decomposed form, allowing for easier
         * sorting and searching of text.
         * The <b> normalize</b> method supports the standard normalization forms
         * described in <a href="https://www.unicode.org/reports/tr15/">
         * Unicode Standard Annex #15 &mdash; Unicode Normalization Forms</a>.
         * <p>
         * Characters with accents or other adornments can be encoded in
         * several different ways in Unicode.  For example, take the character A-acute.
         * In Unicode, this can be encoded as a single character (the "composed" form):
         *
         * <pre>
         *      U+00C1    LATIN CAPITAL LETTER A WITH ACUTE</pre> <br/>
         *
         * or as two separate characters (the "decomposed" form):
         *
         * <pre>
         *      U+0041    LATIN CAPITAL LETTER A     <br/>
         *      U+0301    COMBINING ACUTE ACCENT</pre> <br/>
         *
         * To a user of your program, however, both of these sequences should be
         * treated as the same "user-level" character "A with acute accent".  When you
         * are searching or comparing text, you must ensure that these two sequences are
         * treated as equivalent.  In addition, you must handle characters with more than
         * one accent. Sometimes the order of a character's combining accents is
         * significant, while in other cases accent sequences in different orders are
         * really equivalent.
         * <p>
         * Similarly, the string "ffi" can be encoded as three separate letters:
         *
         * <pre>
         *      U+0066    LATIN SMALL LETTER F  <br/>
         *      U+0066    LATIN SMALL LETTER F  <br/>
         *      U+0069    LATIN SMALL LETTER I</pre>    <br/>
         *
         * or as the single character
         *
         * <pre>
         *      U+FB03    LATIN SMALL LIGATURE FFI</pre> <br/>
         *
         * The ffi ligature is not a distinct semantic character, and strictly speaking
         * it shouldn't be in Unicode at all, but it was included for compatibility
         * with existing character sets that already provided it.  The Unicode standard
         * identifies such characters by giving them "compatibility" decompositions
         * into the corresponding semantic characters.  When sorting and searching, you
         * will often want to use these mappings.
         * <p>
         * The <b> normalize</b> method helps solve these problems by transforming
         * text into the canonical composed and decomposed forms as shown in the first
         * example above. In addition, you can have it perform compatibility
         * decompositions so that you can treat compatibility characters the same as
         * their equivalents.
         * Finally, the <b> normalize</b> method rearranges accents into the
         * proper canonical order, so that you do not have to worry about accent
         * rearrangement on your own.
         * <p>
         * The W3C generally recommends to exchange texts in NFC.
         * Note also that most legacy character encodings use only precomposed forms and
         * often do not encode any combining marks by themselves. For conversion to such
         * character encodings the Unicode text needs to be normalized to NFC.
         * For more usage examples, see the Unicode Standard Annex.
         *
         * @author Brunshweeck Tazeussong
         */
        class Normalizer CORE_FINAL : public Object {
        private:
            Normalizer() {}

        public:

            /**
             * This enum provides constants of the four Unicode normalization forms
             * that are described in <a href="https://www.unicode.org/reports/tr15/">
             * Unicode Standard Annex #15 &mdash; Unicode Normalization Forms</a>
             * and two methods to access them.
             */
            enum Form : gbyte {

                /**
                 * Canonical decomposition.
                 */
                NFD,

                /**
                 * Canonical decomposition, followed by canonical composition.
                 */
                NFC,

                /**
                 * Compatibility decomposition.
                 */
                NFKD,

                /**
                 * Compatibility decomposition, followed by canonical composition.
                 */
                NFKC
            };

            /**
             * Normalize a sequence of char values.
             * The sequence will be normalized according to the specified normalization
             * form.
             * @param src        The sequence of char values to normalize.
             * @param form       The normalization form; one of <br/>
             *                   <b style="color: orange;"> Normalizer.Form.NFC</b>, <br/>
             *                   <b style="color: orange;"> Normalizer.Form.NFD</b>, <br/>
             *                   <b style="color: orange;"> Normalizer.Form.NFKC</b>, <br/>
             *                   <b style="color: orange;"> Normalizer.Form.NFKD</b> <br/>
             * @return The normalized String
             */
            static String normalize(const CharSequence &src, Form form);

            /**
             * Normalize a sequence of char values.
             * The sequence will be normalized according to the specified normalization
             * form.
             * @param src        The sequence of char values to normalize.
             * @param form       The normalization form; one of <br/>
             *                   <b style="color: orange;"> Normalizer.Form.NFC</b>, <br/>
             *                   <b style="color: orange;"> Normalizer.Form.NFD</b>, <br/>
             *                   <b style="color: orange;"> Normalizer.Form.NFKC</b>, <br/>
             *                   <b style="color: orange;"> Normalizer.Form.NFKD</b> <br/>
             * @return The normalized String
             */
            static String normalize(const String &src, Form form);

            /**
             * Determines if the given sequence of char values is normalized.
             * @param src        The sequence of char values to be checked.
             * @param form       The normalization form; one of <br/>
             *                   <b style="color: orange;"> Normalizer.Form.NFC</b>, <br/>
             *                   <b style="color: orange;"> Normalizer.Form.NFD</b>, <br/>
             *                   <b style="color: orange;"> Normalizer.Form.NFKC</b>, <br/>
             *                   <b style="color: orange;"> Normalizer.Form.NFKD</b> <br/>
             * @return true if the sequence of char values is normalized;
             * false otherwise.
             */
             static gbool isNormalized(const CharSequence& src, Form form);

            /**
             * Determines if the given sequence of char values is normalized.
             * @param src        The sequence of char values to be checked.
             * @param form       The normalization form; one of <br/>
             *                   <b style="color: orange;"> Normalizer.Form.NFC</b>, <br/>
             *                   <b style="color: orange;"> Normalizer.Form.NFD</b>, <br/>
             *                   <b style="color: orange;"> Normalizer.Form.NFKC</b>, <br/>
             *                   <b style="color: orange;"> Normalizer.Form.NFKD</b> <br/>
             * @return true if the sequence of char values is normalized;
             * false otherwise.
             */
             static gbool isNormalized(const String& src, Form form);
        };

    } // text
} // core

#endif //MYHARDLINK_NORMALIZER_H
