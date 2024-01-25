//
// Created by T.N.Brunshweeck on 24/01/2024.
//

#include "LocaleBuilder.h"

namespace core {
    namespace util {
        Locale::LocaleBuilder &Locale::LocaleBuilder::setLanguage(const String &language) {
            if (language.isEmpty()) {
                this->language = ""_S;
            } else {
                if (!LanguageTag::isLanguage(language)) {
                    LocaleSyntaxException("Ill-formed language: "_S + language, 0)
                            .throws(__trace("core.util.LocaleBuilder"_S));
                }
                this->language = language;
            }
            return *this;
        }

        Locale::LocaleBuilder Locale::LocaleBuilder::setScript(const String &script) {
            if (script.isEmpty()) {
                this->script = "";
            } else {
                if (!LanguageTag::isScript(script)) {
                    LocaleSyntaxException("Ill-formed script: " + script, 0)
                            .throws(__trace("core.util.LocaleBuilder"_S));
                }
                this->script = script;
            }
            return *this;
        }

        Locale::LocaleBuilder Locale::LocaleBuilder::setRegion(const String &region) {
            if (region.isEmpty()) {
                this->region = "";
            } else {
                if (!LanguageTag::isRegion(region)) {
                    LocaleSyntaxException("Ill-formed region: " + region, 0)
                            .throws(__trace("core.util.LocaleBuilder"_S));
                }
                this->region = region;
            }
            return *this;
        }

        Locale::LocaleBuilder Locale::LocaleBuilder::setVariant(const String &variant) {
            if (variant.isEmpty()) {
                this->variant = "";
            } else {
                // normalize separators to "_"
                String const var = variant.replace(LanguageTag::SEP, BaseLocale::SEP);
                gint errIdx = checkVariants(var, BaseLocale::SEP);
                if (errIdx != -1) {
                    LocaleSyntaxException("Ill-formed variant: " + variant, errIdx)
                            .throws(__trace("core.util.LocaleBuilder"_S));
                }
                this->variant = var;
            }
            return *this;
        }

        gbool Locale::LocaleBuilder::isSingletonChar(gchar c) {
            return ('u' == LocaleUtils::toLower(c));
        }

        gbool Locale::LocaleBuilder::isAttribute(const String &s) {
            // 3*8alphanum
            gint const len = s.length();
            return (len >= 3) && (len <= 8) && LocaleUtils::isAlphaNumericString(s);
        }

        gbool Locale::LocaleBuilder::isKey(const String &s) {
            // 2alphanum
            return (s.length() == 2) && LocaleUtils::isAlphaNumericString(s);
        }

        gbool Locale::LocaleBuilder::isTypeSubtag(const String &s) {
            // 3*8alphanum
            gint const len = s.length();
            return (len >= 3) && (len <= 8) && LocaleUtils::isAlphaNumericString(s);
        }

        Locale::LocaleBuilder &Locale::LocaleBuilder::addUnicodeLocaleAttribute(const String &attribute) {
            if (!isAttribute(attribute)) {
                throw LocaleSyntaxException("Ill-formed Unicode locale attribute: " + attribute);
            }
            // Use case insensitive string to prevent duplication
            if (!uattributes.isEmpty()) {
                uattributes = HashSet<String>(4);
            }
            uattributes.add(String(attribute));
            return *this;
        }

        Locale::LocaleBuilder &Locale::LocaleBuilder::removeUnicodeLocaleAttribute(const String &attribute) {
            if (attribute == null || !isAttribute(attribute)) {
                throw LocaleSyntaxException("Ill-formed Unicode locale attribute: " + attribute);
            }
            if (uattributes.isEmpty() == false) {
                uattributes.remove(String(attribute));
            }
            return *this;
        }

        Locale::LocaleBuilder &Locale::LocaleBuilder::setUnicodeLocaleKeyword(const String &key, const String &type) {
            if (!isKey(key)) {
                throw LocaleSyntaxException("Ill-formed Unicode locale keyword key: " + key);
            }

            String const cikey = String(key);
            if (type == null) {
                if (ukeywords.isEmpty() == false) {
                    // null type is used for remove the key
                    ukeywords.remove(cikey);
                }
            } else {
                if (type.length() != 0) {
                    // normalize separator to "-"
                    String const tp = type.replace(BaseLocale::SEP, LanguageTag::SEP);
                    // validate
                    StringTokenizer itr = StringTokenizer(tp, LanguageTag::SEP);
                    while (itr.hasMore()) {
                        String const s = itr.nextToken();
                        if (!isTypeSubtag(s)) {
                            throw LocaleSyntaxException("Ill-formed Unicode locale keyword type: "
                                                        + type,
                                                        itr.lastIndex() - s.length());
                        }
                    }
                }
                if (ukeywords.isEmpty()) {
                    ukeywords = HashMap<String, String>(4);
                }
                ukeywords.put(cikey, type);
            }
            return *this;
        }

        Locale::LocaleBuilder &Locale::LocaleBuilder::setExtension(gchar singleton, const String &value) {
            // validate key
            gbool const isBcpPrivateuse = LanguageTag::isPrivateUsePrefixChar(singleton);
            if (!isBcpPrivateuse && !LanguageTag::isExtensionSingletonChar(singleton)) {
                throw LocaleSyntaxException("Ill-formed extension key: " + String::valueOf(singleton));
            }

            gbool const remove = value.isEmpty();
            gchar const key = singleton;

            if (remove) {
                if (key == LocaleUtils::toLower(key)) {
                    // clear entire Unicode locale extension
                    if (!uattributes.isEmpty()) {
                        uattributes.clear();
                    }
                    if (!ukeywords.isEmpty()) {
                        ukeywords.clear();
                    }
                } else {
                    if (!extensions.isEmpty()) {
                        extensions.remove(key);
                    }
                }
            } else {
                // validate value
                String const val = value.replace(BaseLocale::SEP, LanguageTag::SEP);
                StringTokenizer itr = StringTokenizer(val, LanguageTag::SEP);
                while (itr.hasMore()) {
                    String const s = itr.nextToken();
                    gbool validSubtag;
                    if (isBcpPrivateuse) {
                        validSubtag = LanguageTag::isPrivateUseSubtag(s);
                    } else {
                        validSubtag = LanguageTag::isExtensionSubtag(s);
                    }
                    if (!validSubtag) {
                        LocaleSyntaxException("Ill-formed extension value: " + s, itr.lastIndex() - s.length())
                                .throws(__trace("core.util.LocaleBuilder"_S));;
                    }
                }

                if (key == LocaleUtils::toLower(key)) {
                    setUnicodeLocaleExtension(val);
                } else {
                    if (extensions.isEmpty()) {
                        extensions = HashMap<Character, String>(4);
                    }
                    extensions.put(key, val);
                }
            }
            return *this;
        }

        Locale::LocaleBuilder &Locale::LocaleBuilder::setExtensions(String subtags) {
            if ((subtags.isEmpty())) {
                clearExtensions();
                return *this;
            }
            subtags = subtags.replace(BaseLocale::SEP, LanguageTag::SEP);
            StringTokenizer itr = StringTokenizer(subtags, LanguageTag::SEP);

            ArrayList<String> extensions = {};
            String privateuse;

            gint parsed = 0;
            gint start;

            // Make a list of extension subtags
            while (itr.hasMore()) {
                String s = itr.nextToken();
                if (LanguageTag::isExtensionSingleton(s)) {
                    start = itr.lastIndex() - s.length();
                    String singleton = s;
                    StringBuffer sb = StringBuffer(singleton);
                    while (itr.hasMore()) {
                        s = itr.nextToken();
                        if (LanguageTag::isExtensionSubtag(s)) {
                            sb.append(LanguageTag::SEP).append(s);
                            parsed = itr.lastIndex();
                        } else {
                            break;
                        }
                    }

                    if (parsed < start) {
                        throw LocaleSyntaxException("Incomplete extension '" + singleton + "'",
                                                    start);
                    }

                    if (extensions.isEmpty()) {
                        extensions = ArrayList<String>(4);
                    }
                    extensions.add(sb.toString());
                } else {
                    break;
                }
            }
            if (itr.hasMore()) {
                String s = itr.nextToken();
                if (LanguageTag::isPrivateUsePrefix(s)) {
                    start = itr.lastIndex() - s.length();
                    StringBuffer sb = StringBuffer(s);
                    while (itr.hasMore()) {
                        s = itr.nextToken();
                        if (!LanguageTag::isPrivateUseSubtag(s)) {
                            break;
                        }
                        sb.append(LanguageTag::SEP).append(s);
                        parsed = itr.lastIndex();
                    }
                    if (parsed <= start) {
                        throw LocaleSyntaxException("Incomplete privateuse:" + subtags.subString(start), start);
                    } else {
                        privateuse = sb.toString();
                    }
                }
            }

            if (itr.hasMore()) {
                throw LocaleSyntaxException("Ill-formed extension subtags:" + subtags.subString(itr.lastIndex()),
                                            itr.lastIndex());
            }

            return setExtensions(extensions, privateuse);
        }

        Locale::LocaleBuilder &
        Locale::LocaleBuilder::setExtensions(const List<String> &bcpExtensions, const String &privateuse) {
            clearExtensions();

            if (!bcpExtensions.isEmpty()) {
                HashSet<Character> done = HashSet<Character>(bcpExtensions.size());
                for (String bcpExt: bcpExtensions) {
                    gchar key = bcpExt.charAt(0);
                    // ignore duplicates
                    if (!done.contains(key)) {
                        // each extension string contains singleton, e.g. "a-abc-def"
                        if (isSingletonChar(key)) {
                            setUnicodeLocaleExtension(bcpExt.subString(2));
                        } else {
                            if (extensions.isEmpty()) {
                                extensions = HashMap<Character, String>(4);
                            }
                            extensions.put(key, bcpExt.subString(2));
                        }
                    }
                    done.add(key);
                }
            }
            if (!privateuse.isEmpty() && !privateuse.isEmpty()) {
                // privateuse string contains prefix, e.g. "x-abc-def"
                if (extensions.isEmpty()) {
                    extensions = HashMap<Character, String>(1);
                }
                extensions.put(privateuse.charAt(0), privateuse.subString(2));
            }

            return *this;
        }

        Locale::LocaleBuilder &Locale::LocaleBuilder::setLanguageTag(const Locale::LanguageTag &langtag) {
            clear();
            if (!langtag.extlangs.isEmpty()) {
                language = langtag.extlangs.get(0);
            } else {
                String lang = langtag.language;
                if (!lang.equals(LanguageTag::UNDETERMINED)) {
                    language = lang;
                }
            }
            script = langtag.script;
            region = langtag.region;

            ArrayList<String> bcpVariants = langtag.variants;
            if (!bcpVariants.isEmpty()) {
                StringBuffer var = StringBuffer(bcpVariants.get(0));
                gint size = bcpVariants.size();
                for (gint i = 1; i < size; i++) {
                    var.append(BaseLocale::SEP).append(bcpVariants.get(i));
                }
                variant = var.toString();
            }

            setExtensions(langtag.extensions, langtag.privateuse);

            return *this;
        }

        Locale::LocaleBuilder &
        Locale::LocaleBuilder::setLocale(const Locale::BaseLocale &base, const Map<Character, String> &localeExtensions) {
            String language = base.language;
            String script = base.script;
            String region = base.region;
            String variant = base.variant;

            // Special backward compatibility support

            // Exception 1 - ja_JP_JP
            if (language.equals("ja") && region.equals("JP") && variant.equals("JP")) {
                // When locale ja_?_JP_JP is created, ca-japanese is always there.
                // The builder ignores the variant "JP"
                // assert("japanese".equals(localeExtensions.getUnicodeLocaleType("ca")));
                variant = "";
            }
                // Exception 2 - th_TH_TH
            else if (language.equals("th") && region.equals("TH") && variant.equals("TH")) {
                // When locale th_?_TH_TH is created, nu-thai is always there.
                // The builder ignores the variant "TH"
                // assert("thai".equals(localeExtensions.getUnicodeLocaleType("nu")));
                variant = "";
            }
                // Exception 3 - no_NO_NY
            else if (language.equals("no") && region.equals("NO") && variant.equals("NY")) {
                // no_?_NO_NY is a valid locale and used by Java 6 or older versions.
                // The build ignores the variant "NY" and change the language to "nn".
                language = "nn";
                variant = "";
            }

            // Validate base locale fields before updating internal state.
            // LocaleExtensions always store validated/canonicalized values,
            // so no checks are necessary.
            if (!language.isEmpty() && !LanguageTag::isLanguage(language)) {
                throw LocaleSyntaxException("Ill-formed language: " + language);
            }

            if (!script.isEmpty() && !LanguageTag::isScript(script)) {
                throw LocaleSyntaxException("Ill-formed script: " + script);
            }

            if (!region.isEmpty() && !LanguageTag::isRegion(region)) {
                throw LocaleSyntaxException("Ill-formed region: " + region);
            }

            if (!variant.isEmpty()) {
                gint errIdx = checkVariants(variant, BaseLocale::SEP);
                if (errIdx != -1) {
                    throw LocaleSyntaxException("Ill-formed variant: " + variant, errIdx);
                }
            }

            // The input locale is validated at this point.
            // Now, updating builder's internal fields.
            this->language = language;
            this->script = script;
            this->region = region;
            this->variant = variant;
            clearExtensions();

            Set<Character> &extKeys = localeExtensions.keySet();
            if (extKeys.isEmpty() == false) {
                // map localeExtensions back to builder's internal format
                for (Character const &key: extKeys) {
                    String const &e = localeExtensions.get(key);
                    if (extensions.isEmpty()) {
                        extensions = HashMap<Character, String>(4);
                    }
                    extensions.put(key, e);
                }
            }
            return *this;
        }

        Locale::LocaleBuilder &Locale::LocaleBuilder::clear() {
            language = ""_S;
            script = ""_S;
            region = ""_S;
            variant = ""_S;
            clearExtensions();
            return *this;
        }

        Locale::LocaleBuilder &Locale::LocaleBuilder::clearExtensions() {
            if (extensions.isEmpty() == false) {
                extensions.clear();
            }
            if (uattributes.isEmpty() == false) {
                uattributes.clear();
            }
            if (ukeywords.isEmpty() == false) {
                ukeywords.clear();
            }
            return *this;
        }

        Locale::BaseLocale Locale::LocaleBuilder::baseLocale() {
            String language = this->language;
            String script = this->script;
            String region = this->region;
            String variant = this->variant;

            // Special use subtag sequence identified by "lvariant" will be
            // interpreted as Java variant.
            if (extensions.isEmpty() == false) {
                String privuse = extensions.get(PRIVATEUSE_KEY);
                if (!privuse.isEmpty()) {
                    StringTokenizer itr = StringTokenizer(privuse, LanguageTag::SEP);
                    gbool sawPrefix = false;
                    gint privVarStart = -1;
                    while (itr.hasMore()) {
                        const String s = itr.nextToken();
                        if (sawPrefix) {
                            privVarStart = itr.lastIndex() - s.length();
                            break;
                        }
                        if (LocaleUtils::equalsIgnoreCase(s, LanguageTag::PRIVUSE_VARIANT_PREFIX)) {
                            sawPrefix = true;
                        }
                    }
                    if (privVarStart != -1) {
                        StringBuffer sb = StringBuffer(variant);
                        if (sb.length() != 0) {
                            sb.append(BaseLocale::SEP);
                        }
                        sb.append(privuse.subString(privVarStart).replace(LanguageTag::SEP,
                                                                          BaseLocale::SEP));
                        variant = sb.toString();
                    }
                }
            }

            return BaseLocale(language, script, region, variant);
        }

        HashMap<Character, String> Locale::LocaleBuilder::localeExtensions() {
            if ((extensions.isEmpty()) && (uattributes.isEmpty()) && (ukeywords.isEmpty())) {
                return {};
            }
            //
            HashMap<Character, String> lext;
            return lext;
        }

        String Locale::LocaleBuilder::removePrivateUseVariant(String privuseVal) {
            StringTokenizer itr = StringTokenizer(privuseVal, LanguageTag::SEP);

            // Note: privateuse value "abc-lvariant" is unchanged
            // because no subtags after "lvariant".

            gint prefixStart = -1;
            gbool sawPrivuseVar = false;
            while (itr.hasMore()) {
                if (prefixStart != -1) {
                    // Note: privateuse value "abc-lvariant" is unchanged
                    // because no subtags after "lvariant".
                    sawPrivuseVar = true;
                    break;
                }
                const String s = itr.nextToken();
                if (LocaleUtils::equalsIgnoreCase(s, LanguageTag::PRIVUSE_VARIANT_PREFIX)) {
                    prefixStart = itr.lastIndex() - s.length();
                }
            }
            if (!sawPrivuseVar) {
                return privuseVal;
            }

            // assert(prefixStart == 0 || prefixStart > 1);
            return (prefixStart == 0) ? ""_S : privuseVal.subString(0, prefixStart - 1);
        }

        gint Locale::LocaleBuilder::checkVariants(const String &variants, const String &sep) const {
            StringTokenizer itr = StringTokenizer(variants, sep);
            while (itr.hasMore()) {
                String const s = itr.nextToken();
                if (!LanguageTag::isVariant(s)) {
                    return itr.lastIndex() - s.length();
                }
            }
            return -1;
        }

        void Locale::LocaleBuilder::setUnicodeLocaleExtension(const String &subtags) {
            // wipe out existing attributes/keywords
            if (uattributes.isEmpty() == false) {
                uattributes.clear();
            }
            if (ukeywords.isEmpty() == false) {
                ukeywords.clear();
            }

            StringTokenizer itr = StringTokenizer(subtags, LanguageTag::SEP);

            // parse attributes
            while (itr.hasMore()) {
                const String s = itr.nextToken();
                if (!isAttribute(s)) {
                    break;
                }
                if (uattributes.isEmpty()) {
                    uattributes = HashSet<String>(4);
                }
                uattributes.add(s);
            }

            // parse keywords
            String key;
            String type;
            gint typeStart = -1;
            gint typeEnd = -1;
            while (itr.hasMore()) {
                const String s = itr.nextToken();
                if (!key.isEmpty()) {
                    if (isKey(s)) {
                        // next keyword - emit previous one
                        // assert(typeStart == -1 || typeEnd != -1);
                        type = (typeStart == -1) ? ""_S : subtags.subString(typeStart, typeEnd);
                        if (ukeywords.isEmpty()) {
                            ukeywords = HashMap<String, String>(4);
                        }
                        ukeywords.put(key, type);

                        // reset keyword info
                        String const tmpKey = itr.lastToken();
                        key = ukeywords.containsKey(tmpKey) ? ""_S : tmpKey;
                        typeStart = typeEnd = -1;
                    } else {
                        if (typeStart == -1) {
                            typeStart = itr.lastIndex();
                        }
                        typeEnd = itr.lastIndex();
                    }
                } else if (isKey(s)) {
                    // 1. first keyword or
                    // 2. next keyword, but previous one was duplicate
                    key = itr.lastToken();
                    if (ukeywords.isEmpty() == false && ukeywords.containsKey(key)) {
                        // duplicate
                        key = {};
                    }
                }

                if (!itr.hasMore()) {
                    if (!key.isEmpty()) {
                        // last keyword
                        // assert(typeStart == -1 || typeEnd != -1);
                        type = (typeStart == -1) ? "" : subtags.subString(typeStart, typeEnd);
                        if (ukeywords.isEmpty()) {
                            ukeywords = HashMap<String, String>(4);
                        }
                        ukeywords.put(key, type);
                    }
                    break;
                }
            }
        }
    } // util
} // core
