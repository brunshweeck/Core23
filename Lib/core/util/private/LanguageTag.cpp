//
// Created by T.N.Brunshweeck on 24/01/2024.
//

#include "LanguageTag.h"

namespace core {
    namespace util {


        String const Locale::LanguageTag::SEP = "-"_S;
        String const Locale::LanguageTag::PRIVATEUSE = "x"_S;
        String const Locale::LanguageTag::UNDETERMINED = "und"_S;
        String const Locale::LanguageTag::PRIVUSE_VARIANT_PREFIX = "lvariant"_S;

        const Map<String, StringArray> &Locale::LanguageTag::LEGACY = []() -> Map<String, StringArray> & {

            // grandfathered = irregular           ; non-redundant tags registered
            //               / regular             ; during the RFC 3066 era
            //
            // irregular     = "en-GB-oed"         ; irregular tags do not match
            //               / "i-ami"             ; the 'langtag' production and
            //               / "i-bnn"             ; would not otherwise be
            //               / "i-default"         ; considered 'well-formed'
            //               / "i-enochian"        ; These tags are all valid,
            //               / "i-hak"             ; but most are deprecated
            //               / "i-klingon"         ; in favor of more modern
            //               / "i-lux"             ; subtags or subtag
            //               / "i-mingo"           ; combination
            //               / "i-navajo"
            //               / "i-pwn"
            //               / "i-tao"
            //               / "i-tay"
            //               / "i-tsu"
            //               / "sgn-BE-FR"
            //               / "sgn-BE-NL"
            //               / "sgn-CH-DE"
            //
            // regular       = "art-lojban"        ; these tags match the 'langtag'
            //               / "cel-gaulish"       ; production, but their subtags
            //               / "no-bok"            ; are not extended language
            //               / "no-nyn"            ; or variant subtags: their meaning
            //               / "zh-guoyu"          ; is defined by their registration
            //               / "zh-hakka"          ; and all of these are deprecated
            //               / "zh-min"            ; in favor of a more modern
            //               / "zh-min-nan"        ; subtag or sequence of subtags
            //               / "zh-xiang"
            const StringArray2D entries = StringArray2D::of(
//                    StringArray::of(u"tag"_S, "preferred"_S),
                    StringArray::of(u"art-lojban"_S, "jbo"_S),
                    StringArray::of(u"cel-gaulish"_S, "xtg-x-cel-gaulish"_S),   // fallback
                    StringArray::of(u"en-GB-oed"_S, "en-GB-x-oed"_S),         // fallback
                    StringArray::of(u"i-ami"_S, "ami"_S),
                    StringArray::of(u"i-bnn"_S, "bnn"_S),
                    StringArray::of(u"i-default"_S, "en-x-i-default"_S),      // fallback
                    StringArray::of(u"i-enochian"_S, "und-x-i-enochian"_S),    // fallback
                    StringArray::of(u"i-hak"_S, "hak"_S),
                    StringArray::of(u"i-klingon"_S, "tlh"_S),
                    StringArray::of(u"i-lux"_S, "lb"_S),
                    StringArray::of(u"i-mingo"_S, "see-x-i-mingo"_S),       // fallback
                    StringArray::of(u"i-navajo"_S, "nv"_S),
                    StringArray::of(u"i-pwn"_S, "pwn"_S),
                    StringArray::of(u"i-tao"_S, "tao"_S),
                    StringArray::of(u"i-tay"_S, "tay"_S),
                    StringArray::of(u"i-tsu"_S, "tsu"_S),
                    StringArray::of(u"no-bok"_S, "nb"_S),
                    StringArray::of(u"no-nyn"_S, "nn"_S),
                    StringArray::of(u"sgn-BE-FR"_S, "sfb"_S),
                    StringArray::of(u"sgn-BE-NL"_S, "vgt"_S),
                    StringArray::of(u"sgn-CH-DE"_S, "sgg"_S),
                    StringArray::of(u"zh-guoyu"_S, "cmn"_S),
                    StringArray::of(u"zh-hakka"_S, "hak"_S),
                    StringArray::of(u"zh-min"_S, "nan-x-zh-min"_S),        // fallback
                    StringArray::of(u"zh-min-nan"_S, "nan"_S),
                    StringArray::of(u"zh-xiang"_S, "hsn"_S)
            );
            Map < String, StringArray > &LEGACY = Unsafe::allocateInstance < HashMap < String, StringArray >> ();
            for (StringArray const &e: entries) {
                LEGACY.put(LocaleUtils::toLowerString(e[0]), e);
            }
            return LEGACY;
        }();

        Locale::LanguageTag Locale::LanguageTag::parse(const String &languageTag, ParseStatus &sts) {
            sts.reset();
            StringTokenizer itr = StringTokenizer(""_S);

            // Check if the tag is a legacy language tag
            StringArray const &gfmap = LEGACY.getOrDefault(LocaleUtils::toLowerString(languageTag), {});
            if (!gfmap.isEmpty()) {
                // use preferred mapping
                itr = StringTokenizer(gfmap[1], SEP);
            } else {
                itr = StringTokenizer(languageTag, SEP);
            }

            LanguageTag tag;

            // langtag must start with either language or privateuse
            if (tag.parseLanguage(itr, sts)) {
                tag.parseExtlangs(itr, sts);
                tag.parseScript(itr, sts);
                tag.parseRegion(itr, sts);
                tag.parseVariants(itr, sts);
                tag.parseExtensions(itr, sts);
            }
            tag.parsePrivateuse(itr, sts);

            if (itr.hasMore() && !sts.isError()) {
                String const s = itr.lastToken();
                sts.errIndex = itr.lastIndex();
                if (s.isEmpty()) {
                    sts.errMessage = u"Empty subtag"_S;
                } else {
                    sts.errMessage = u"Invalid subtag: "_S + s;
                }
            }

            return tag;
        }

        gbool Locale::LanguageTag::parseLanguage(StringTokenizer &itr, ParseStatus &sts) {
            if (!itr.hasMore() || sts.isError()) {
                return false;
            }

            gbool found = false;

            String const s = itr.nextToken();
            if (isLanguage(s)) {
                found = true;
                language = s;
                sts.length = itr.lastIndex();
                if(itr.hasMore())
                    itr.nextToken();
            }

            return found;
        }

        gbool Locale::LanguageTag::parseExtlangs(StringTokenizer &itr, ParseStatus &sts) {
            if (!itr.hasMore() || sts.isError()) {
                return false;
            }

            gbool found = false;

            while (itr.hasMore()) {
                String const s = itr.lastToken();
                if (!isExtLang(s)) {
                    break;
                }
                found = true;
                extlangs.add(s);
                sts.length = itr.lastIndex();
                itr.nextToken();

                if (extlangs.size() == 3) {
                    // Maximum 3 extlangs
                    break;
                }
            }

            return found;
        }

        gbool Locale::LanguageTag::parseScript(StringTokenizer &itr, ParseStatus &sts) {
            if (!itr.hasMore() || sts.isError()) {
                return false;
            }

            gbool found = false;

            String const s = itr.lastToken();
            if (isScript(s)) {
                found = true;
                script = s;
                sts.length = itr.lastIndex();
                itr.nextToken();
            }

            return found;
        }

        gbool Locale::LanguageTag::parseRegion(StringTokenizer &itr, ParseStatus &sts) {
            if (!itr.hasMore() || sts.isError()) {
                return false;
            }

            gbool found = false;

            String const s = itr.lastToken();
            if (isRegion(s)) {
                found = true;
                region = s;
                sts.length = itr.lastIndex();
                itr.nextToken();
            }

            return found;
        }

        gbool Locale::LanguageTag::parseVariants(StringTokenizer &itr, ParseStatus &sts) {
            if (!itr.hasMore() || sts.isError()) {
                return false;
            }

            gbool found = false;

            while (itr.hasMore()) {
                String const s = itr.lastToken();
                if (!isVariant(s)) {
                    break;
                }
                found = true;
                variants.add(s);
                sts.length = itr.lastIndex();
                itr.nextToken();
            }

            return found;
        }

        gbool Locale::LanguageTag::parseExtensions(StringTokenizer &itr, ParseStatus &sts) {
            if (!itr.hasMore() || sts.isError()) {
                return false;
            }

            gbool found = false;

            while (itr.hasMore()) {
                String s = itr.lastToken();
                if (isExtensionSingleton(s)) {
                    gint const start = itr.lastIndex() - s.length();
                    String const singleton = s;
                    StringBuffer sb = StringBuffer(singleton);

                    while (itr.hasMore()) {
                        s = itr.nextToken();
                        if (isExtensionSubtag(s)) {
                            sb.append(SEP).append(s);
                            sts.length = itr.lastIndex();
                        } else {
                            break;
                        }
                    }

                    if (sts.parseLength() <= start) {
                        sts.errIndex = start;
                        sts.errMessage = "Incomplete extension '" + singleton + "'";
                        break;
                    }

                    extensions.add(sb.toString());
                    found = true;
                } else {
                    break;
                }
            }
            return found;
        }

        gbool Locale::LanguageTag::parsePrivateuse(StringTokenizer &itr, ParseStatus &sts) {
            if (!itr.hasMore() || sts.isError()) {
                return false;
            }

            gbool found = false;

            String s = itr.lastToken();
            if (isPrivateUsePrefix(s)) {
                gint const start = itr.lastIndex() - s.length();
                StringBuffer sb = StringBuffer(s);

                while (itr.hasMore()) {
                    s = itr.nextToken();
                    if (!isPrivateUseSubtag(s)) {
                        break;
                    }
                    sb.append(SEP).append(s);
                    sts.length = itr.lastIndex();
                }

                if (sts.length <= start) {
                    // need at least 1 private subtag
                    sts.errIndex = start;
                    sts.errMessage = "Incomplete privateuse";
                } else {
                    privateuse = sb.toString();
                    found = true;
                }
            }

            return found;
        }

        Locale::LanguageTag Locale::LanguageTag::parseLocale(const Locale::BaseLocale &baseLocale,
                                                             const Map<Character, String> &localeExtensions) {
            LanguageTag tag;

            String language = baseLocale.language;
            String const script = baseLocale.script;
            String const region = baseLocale.region;
            String variant = baseLocale.variant;

            gbool hasSubtag = false;

            String privuseVar = {};   // store ill-formed variant subtags

            if (isLanguage(language)) {
                // Convert a deprecated language code to its new code
                if (language.equals("iw")) {
                    language = "he";
                } else if (language.equals("ji")) {
                    language = "yi";
                } else if (language.equals("in")) {
                    language = "id";
                }
                tag.language = Unsafe::moveInstance(language);
            }

            if (isScript(script)) {
                tag.script = canonicalizeScript(script);
                hasSubtag = true;
            }

            if (isRegion(region)) {
                tag.region = canonicalizeRegion(region);
                hasSubtag = true;
            }

            // Special handling for no_NO_NY - use nn_NO for language tag
            if (tag.language.equals("no") && tag.region.equals("NO") && variant.equals("NY")) {
                tag.language = "nn";
                variant = "";
            }

            if (!variant.isEmpty()) {
                ArrayList<String> variants = {};
                StringTokenizer varitr = StringTokenizer(variant, BaseLocale::SEP);
                while (varitr.hasMore()) {
                    String var = varitr.nextToken();
                    if (!isVariant(var)) {
                        break;
                    }
                    variants.add(var);  // Do not canonicalize!
                }
                if (variants != null) {
                    tag.variants = variants;
                    hasSubtag = true;
                }
                if (varitr.hasMore()) {
                    // ill-formed variant subtags
                    StringBuffer sb;
                    while (varitr.hasMore()) {
                        String const prvv = varitr.nextToken();
                        if (!isPrivateUseSubtag(prvv)) {
                            // cannot use private use subtag - truncated
                            break;
                        }
                        if(!sb.isEmpty())
                            sb.append(SEP);
                        sb.append(prvv);
                    }
                    if (sb.length() > 0) {
                        privuseVar = sb.toString();
                    }
                }
            }

            ArrayList<String> extensions;
            String privateuse;

            if (localeExtensions != null) {
                Set<Character> &locextKeys = localeExtensions.keySet();
                for (Character locextKey: locextKeys) {
                    String extVal = localeExtensions.get(locextKey);
                    if (isPrivateUsePrefixChar(locextKey)) {
                        privateuse = extVal;
                    } else {
                        extensions.add(locextKey.toString() + SEP + extVal);
                    }
                }
            }

            if (!extensions.isEmpty()) {
                tag.extensions = extensions;
                hasSubtag = true;
            }

            // append ill-formed variant subtags to private use
            if (!privuseVar.isEmpty()) {
                if (privateuse.isEmpty()) {
                    privateuse = PRIVUSE_VARIANT_PREFIX + SEP + privuseVar;
                } else {
                    privateuse = privateuse + SEP + PRIVUSE_VARIANT_PREFIX
                                 + SEP + privuseVar.replace(BaseLocale::SEP, SEP);
                }
            }

            if (!privateuse.isEmpty()) {
                tag.privateuse = privateuse;
            }

            if (tag.language.isEmpty() && (hasSubtag || privateuse == null)) {
                // use lang "und" when 1) no language is available AND
                // 2) any of other subtags other than private use are available or
                // no private use tag is available
                tag.language = UNDETERMINED;
            }

            return tag;
        }

        gbool Locale::LanguageTag::isLanguage(const String &s) {
            // language      = 2*3ALPHA            ; shortest ISO 639 code
            //                 ["-" extlang]       ; sometimes followed by
            //                                     ;   extended language subtags
            //               / 4ALPHA              ; or reserved for future use
            //               / 5*8ALPHA            ; or registered language subtag
            gint const len = s.length();
            return (len >= 2) && (len <= 8) && LocaleUtils::isAlphaString(s);
        }

        gbool Locale::LanguageTag::isExtLang(const String &s) {
            // extlang       = 3ALPHA              ; selected ISO 639 codes
            //                 *2("-" 3ALPHA)      ; permanently reserved
            return (s.length() == 3) && LocaleUtils::isAlphaString(s);
        }

        gbool Locale::LanguageTag::isScript(const String &s) {
            // script        = 4ALPHA              ; ISO 15924 code
            return (s.length() == 4) && LocaleUtils::isAlphaString(s);
        }

        gbool Locale::LanguageTag::isRegion(const String &s) {
            // region        = 2ALPHA              ; ISO 3166-1 code
            //               / 3DIGIT              ; UN M.49 code
            return ((s.length() == 2) && LocaleUtils::isAlphaString(s))
                   || ((s.length() == 3) && LocaleUtils::isNumericString(s));
        }

        gbool Locale::LanguageTag::isVariant(const String &s) {
            // variant       = 5*8alphanum         ; registered variants
            //               / (DIGIT 3alphanum)
            gint const len = s.length();
            if (len >= 5 && len <= 8) {
                return LocaleUtils::isAlphaNumericString(s);
            }
            if (len == 4) {
                return LocaleUtils::isNumeric(s.charAt(0))
                       && LocaleUtils::isAlphaNumeric(s.charAt(1))
                       && LocaleUtils::isAlphaNumeric(s.charAt(2))
                       && LocaleUtils::isAlphaNumeric(s.charAt(3));
            }
            return false;
        }

        gbool Locale::LanguageTag::isExtensionSingleton(const String &s) {
            // singleton     = DIGIT               ; 0 - 9
            //               / %x41-57             ; A - W
            //               / %x59-5A             ; Y - Z
            //               / %x61-77             ; a - w
            //               / %x79-7A             ; y - z

            return (s.length() == 1)
                   && LocaleUtils::isAlphaString(s)
                   && !LocaleUtils::equalsIgnoreCase(PRIVATEUSE, s);
        }

        gbool Locale::LanguageTag::isExtensionSingletonChar(gchar c) {
            return isExtensionSingleton(String::valueOf(c));
        }

        gbool Locale::LanguageTag::isExtensionSubtag(const String &s) {
            // extension     = singleton 1*("-" (2*8alphanum))
            gint const len = s.length();
            return (len >= 2) && (len <= 8) && LocaleUtils::isAlphaNumericString(s);
        }

        gbool Locale::LanguageTag::isPrivateUsePrefix(const String &s) {
            // privateuse    = "x" 1*("-" (1*8alphanum))
            return (s.length() == 1)
                   && LocaleUtils::equalsIgnoreCase(PRIVATEUSE, s);
        }

        gbool Locale::LanguageTag::isPrivateUsePrefixChar(gchar c) {
            return (LocaleUtils::equalsIgnoreCase(PRIVATEUSE, String::valueOf(c)));
        }

        gbool Locale::LanguageTag::isPrivateUseSubtag(const String &s) {
            // privateuse    = "x" 1*("-" (1*8alphanum))
            gint const len = s.length();
            return (len >= 1) && (len <= 8) && LocaleUtils::isAlphaNumericString(s);
        }

        String Locale::LanguageTag::toString() const {
            StringBuffer sb;

            if (!language.isEmpty()) {
                sb.append(language);

                for (String extlang: extlangs) {
                    sb.append(SEP).append(extlang);
                }

                if (!script.isEmpty()) {
                    sb.append(SEP).append(script);
                }

                if (!region.isEmpty()) {
                    sb.append(SEP).append(region);
                }

                for (String variant: variants) {
                    sb.append(SEP).append(variant);
                }

                for (String extension: extensions) {
                    sb.append(SEP).append(extension);
                }
            }
            if (!privateuse.isEmpty()) {
                if (sb.length() > 0) {
                    sb.append(SEP);
                }
                sb.append(privateuse);
            }

            return sb.toString();
        }

    } // util
} // core
