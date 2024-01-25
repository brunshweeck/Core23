//
// Created by T.N.Brunshweeck on 24/01/2024.
//

#ifndef CORE23_LANGUAGETAG_H
#define CORE23_LANGUAGETAG_H

#include <core/util/Locale.h>
#include <core/util/private/ParseStatus.h>
#include <core/util/private/LocaleUtils.h>

namespace core {
    namespace util {

        class Locale::LanguageTag CORE_FINAL: public Object {
        public:
            static String const SEP; // "-"
            static String const PRIVATEUSE; // "x"
            static String const UNDETERMINED; // "und"
            static String const PRIVUSE_VARIANT_PREFIX; // "lvariant"

            //
            // Language subtag fields
            //
            String language = "";      // language subtag
            String script = "";        // script subtag
            String region = "";        // region subtag
            String privateuse = "";    // private-use

            ArrayList<String> extlangs; // extlang subtags
            ArrayList<String> variants; // variant subtags
            ArrayList<String> extensions; // extensions

        private:
            // Map contains legacy language tags and its preferred mappings from
            // http://www.ietf.org/rfc/rfc5646.txt
            // Keys are lower-case strings.
            static const Map<String, StringArray> &LEGACY;

            LanguageTag() = default;

        public:

            /*
             * BNF in RFC5646
             *
             * Language-Tag  = langtag             ; normal language tags
             *               / privateuse          ; private use tag
             *               / grandfathered       ; grandfathered tags
             *
             *
             * langtag       = language
             *                 ["-" script]
             *                 ["-" region]
             *                 *("-" variant)
             *                 *("-" extension)
             *                 ["-" privateuse]
             *
             * language      = 2*3ALPHA            ; shortest ISO 639 code
             *                 ["-" extlang]       ; sometimes followed by
             *                                     ; extended language subtags
             *               / 4ALPHA              ; or reserved for future use
             *               / 5*8ALPHA            ; or registered language subtag
             *
             * extlang       = 3ALPHA              ; selected ISO 639 codes
             *                 *2("-" 3ALPHA)      ; permanently reserved
             *
             * script        = 4ALPHA              ; ISO 15924 code
             *
             * region        = 2ALPHA              ; ISO 3166-1 code
             *               / 3DIGIT              ; UN M.49 code
             *
             * variant       = 5*8alphanum         ; registered variants
             *               / (DIGIT 3alphanum)
             *
             * extension     = singleton 1*("-" (2*8alphanum))
             *
             *                                     ; Single alphanumerics
             *                                     ; "x" reserved for private use
             * singleton     = DIGIT               ; 0 - 9
             *               / %x41-57             ; A - W
             *               / %x59-5A             ; Y - Z
             *               / %x61-77             ; a - w
             *               / %x79-7A             ; y - z
             *
             * privateuse    = "x" 1*("-" (1*8alphanum))
             *
             */
            static LanguageTag parse(const String &languageTag, ParseStatus &sts);

            //
            // Language subtag parsers
            //
            gbool parseLanguage(StringTokenizer &itr, ParseStatus &sts);

            gbool parseExtlangs(StringTokenizer &itr, ParseStatus &sts);

            gbool parseScript(StringTokenizer &itr, ParseStatus &sts);

            gbool parseRegion(StringTokenizer &itr, ParseStatus &sts);

            gbool parseVariants(StringTokenizer &itr, ParseStatus &sts);

            gbool parseExtensions(StringTokenizer &itr, ParseStatus &sts);

            gbool parsePrivateuse(StringTokenizer &itr, ParseStatus &sts);

            static LanguageTag parseLocale(const BaseLocale& baseLocale, const Map<Character, String> &localeExtensions);

            //
            // Language subtag syntax checking methods
            //
            static gbool isLanguage(const String& s);

            static gbool isExtLang(const String& s);

            static gbool isScript(const String& s);

            static gbool isRegion(const String& s);

            static gbool isVariant(const String& s);

            static gbool isExtensionSingleton(const String& s);

            static gbool isExtensionSingletonChar(gchar c);

            static gbool isExtensionSubtag(const String& s);

            static gbool isPrivateUsePrefix(const String& s);

            static gbool isPrivateUsePrefixChar(gchar c);

            static gbool isPrivateUseSubtag(const String& s);

            //
            // Language subtag canonicalization methods
            //
            static String canonicalizeLanguage(const String& s) {
                return LocaleUtils::toLowerString(s);
            }

            static String canonicalizeExtlang(const String& s) {
                return LocaleUtils::toLowerString(s);
            }

            static String canonicalizeScript(const String& s) {
                return LocaleUtils::toTitleString(s);
            }

            static String canonicalizeRegion(const String& s) {
                return LocaleUtils::toUpperString(s);
            }

            static String canonicalizeVariant(const String& s) {
                return LocaleUtils::toLowerString(s);
            }

            static String canonicalizeExtension(const String& s) {
                return LocaleUtils::toLowerString(s);
            }

            static String canonicalizeExtensionSingleton(const String& s) {
                return LocaleUtils::toLowerString(s);
            }

            static String canonicalizeExtensionSubtag(const String& s) {
                return LocaleUtils::toLowerString(s);
            }

            static String canonicalizePrivateuse(const String& s) {
                return LocaleUtils::toLowerString(s);
            }

            static String canonicalizePrivateuseSubtag(const String& s) {
                return LocaleUtils::toLowerString(s);
            }

            String toString() const override;
        };

    } // util
} // core

#endif //CORE23_LANGUAGETAG_H
