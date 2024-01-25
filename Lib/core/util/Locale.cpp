//
// Created by T.N.Brunshweeck on 17/11/2023.
//


#include "Locale.h"
#include <core/String.h>
#include <core/StringBuffer.h>
#include <core/util/private/LocaleUtils.h>
#include <core/util/private/LocaleISOData.h>
#include <core/util/private/LocaleBuilder.h>
#include <core/util/private/LanguageTag.h>
#include <core/util/MissingResourceException.h>

namespace core {
    namespace util {

        namespace {
            HashMap<Character, String> const EMPTY = {};
        }

        Locale const Locale::ENGLISH = createLocale(u"en"_S, ""_S);
        Locale const Locale::FRENCH = createLocale(u"fr"_S, ""_S);
        Locale const Locale::GERMAN = createLocale(u"de"_S, ""_S);
        Locale const Locale::ITALIAN = createLocale(u"it"_S, ""_S);
        Locale const Locale::JAPANESE = createLocale(u"ja"_S, ""_S);
        Locale const Locale::KOREAN = createLocale(u"ko"_S, ""_S);
        Locale const Locale::CHINESE = createLocale(u"zh"_S, ""_S);
        Locale const Locale::SIMPLIFIED_CHINESE = createLocale(u"zh"_S, "CN"_S);
        Locale const Locale::TRADITIONAL_CHINESE = createLocale(u"zh"_S, "TW"_S);
        Locale const Locale::FRANCE = createLocale(u"fr"_S, "FR"_S);
        Locale const Locale::GERMANY = createLocale(u"de"_S, "DE"_S);
        Locale const Locale::ITALY = createLocale(u"it"_S, "IT"_S);
        Locale const Locale::JAPAN = createLocale(u"ja"_S, "JP"_S);
        Locale const Locale::KOREA = createLocale(u"ko"_S, "KR"_S);
        Locale const Locale::UK = createLocale(u"en"_S, "GB"_S);
        Locale const Locale::US = createLocale(u"en"_S, "US"_S);
        Locale const Locale::CANADA = createLocale(u"en"_S, "CA"_S);
        Locale const Locale::CANADA_FRENCH = createLocale(u"fr"_S, "CA"_S);
        Locale const Locale::ROOT = createLocale(u""_S, ""_S);
        Locale const Locale::CHINA = SIMPLIFIED_CHINESE;
        Locale const Locale::PRC = SIMPLIFIED_CHINESE;
        Locale const Locale::TAIWAN = TRADITIONAL_CHINESE;

        Locale Locale::DEFAULT_LOCALE = initDefaultLocale();
        Locale Locale::DEFAULT_DISPLAY_LOCALE = initDefaultLocale(Category::DISPLAY);
        Locale Locale::DEFAULT_FORMAT_LOCALE = initDefaultLocale(Category::FORMAT);

        Locale::Locale(String language) :
                Locale(Unsafe::moveInstance(language), ""_S, ""_S) {}

        Locale::Locale(String language, String country) :
                Locale(Unsafe::moveInstance(language), ""_S, Unsafe::moveInstance(country)) {}

        Locale::Locale(String language, String script, String country) :
                baseLocale(Unsafe::moveInstance(language), Unsafe::moveInstance(script),
                           Unsafe::moveInstance(country), ""_S) {}

        Locale::Locale(Locale::BaseLocale baseLocale, const Map<Character, String> &extensions) :
                baseLocale(Unsafe::moveInstance(baseLocale)), localeExtensions(extensions) {}

        Locale Locale::createLocale(const String &language, const String &script, const String &country,
                                    const String &variant) {
            return createLocale(language, script, country, variant, EMPTY);
        }

        Locale Locale::createLocale(const String &language, const String &script, const String &country,
                                    const String &variant, const Map<Character, String> &extensions) {
            BaseLocale const baseLocale = BaseLocale(language, script, country, variant);
            return createLocale(baseLocale, extensions);
        }

        Locale Locale::createLocale(const String &language, const String &country) {
            return createLocale(language, ""_S, country, ""_S, EMPTY);
        }

        Locale Locale::system() {
            return system((Category) ((gint) Category::DISPLAY | (gint) Category::FORMAT));
        }

        Locale Locale::defaultLocale() {
            return DEFAULT_LOCALE;
        }

        Locale Locale::defaultLocale(Locale::Category category) {
            switch (category) {
                case Category::DISPLAY:
                    return DEFAULT_DISPLAY_LOCALE;
                case Category::FORMAT:
                    return DEFAULT_FORMAT_LOCALE;
            }
            return DEFAULT_LOCALE;
        }

        void Locale::setDefaultLocale(const Locale &newLocale) {
            setDefaultLocale(Category::DISPLAY, newLocale);
            setDefaultLocale(Category::FORMAT, newLocale);
            DEFAULT_LOCALE = Optional<Locale>::of(Unsafe::copyInstance(newLocale));
        }

        void Locale::setDefaultLocale(Locale::Category category, const Locale &newLocale) {
            switch (category) {
                case Category::DISPLAY: {
                    if (DEFAULT_DISPLAY_LOCALE.equals(newLocale)) {
                        return;
                    }
                    DEFAULT_DISPLAY_LOCALE = Unsafe::copyInstance(newLocale);
                    break;
                }
                case Category::FORMAT: {
                    if (DEFAULT_FORMAT_LOCALE.equals(newLocale)) {
                        return;
                    }
                    DEFAULT_FORMAT_LOCALE = Unsafe::copyInstance(newLocale);
                    break;
                }
                default:
                    CORE_ASSERT_IF(false, "Unknown category"_S, "core.util.Locale"_S);
            }
        }

        StringArray Locale::isoCountries() {
            static StringArray ISO2Countries;
            if (ISO2Countries.isEmpty()) {
                ISO2Countries = iso2Table(LocaleISOData::ISO_COUNTRIES_TABLE);
            }
            return ISO2Countries;
        }

        StringArray Locale::isoCountries(Locale::ISOCountryCode type) {
            switch (type) {
                case ISOCountryCode::PART1_ALPHA2:
                    return isoCountries();
                case ISOCountryCode::PART1_ALPHA3: {
                    static StringArray ISO3Countries;
                    if (ISO3Countries.isEmpty()) {
                        ISO3Countries = iso3Table(LocaleISOData::ISO_COUNTRIES_TABLE);
                    }
                    return ISO3Countries;
                }
                case ISOCountryCode::PART3:
                    return LocaleISOData::ISO3166_3;
            }
            return {};
        }

        StringArray Locale::isoLanguages() {
            static StringArray ISO2Languages;
            if (ISO2Languages.isEmpty()) {
                ISO2Languages = iso2Table(LocaleISOData::ISO_LANGUAGES_TABLE);
            }
            return ISO2Languages;
        }

        Set<Locale> &Locale::availableLocales() {
            Set<Locale> &AvailableLocales = Unsafe::allocateInstance<HashSet<Locale>>();
            return Set<Locale>::unmodifiableSet(AvailableLocales);
        }

        String Locale::language() const {
            return baseLocale.language;
        }

        String Locale::script() const {
            return baseLocale.script;
        }

        String Locale::country() const {
            return baseLocale.region;
        }

        String Locale::variant() const {
            return baseLocale.variant;
        }

        gbool Locale::hasExtensions() const {
            return !localeExtensions.isEmpty();
        }

        Locale Locale::stripExtensions() const {
            return createLocale(baseLocale, EMPTY);
        }

        Locale
        Locale::createLocale(const Locale::BaseLocale &baseLocale, const Map<Character, String> &localeExtensions) {
            //
        }

        String Locale::extension(gchar key) const {
            if (!(LanguageTag::isExtensionSingletonChar(key) || LanguageTag::isPrivateUsePrefixChar(key))) {
                IllegalArgumentException("Ill-formed extension key: "_S + String::valueOf(key))
                        .throws(__trace("core.util.Locale"));
            }
            return hasExtensions() ? localeExtensions.get(key) : "";
        }

        String Locale::toString() const {
            gbool const l = !baseLocale.language.isEmpty();
            gbool const s = !baseLocale.script.isEmpty();
            gbool const r = !baseLocale.region.isEmpty();
            gbool const v = !baseLocale.variant.isEmpty();
            gbool const e = !localeExtensions.isEmpty();

            StringBuffer result = StringBuffer(baseLocale.language);

            if (s && (l || r)) {
                result.append("_#"_S)
                        .append(baseLocale.script);
            }

            if (r || (l && (v || s || e))) {
                result.append('_')
                        .append(baseLocale.region); // This may just append '_'
            }
            if (v && (l || r)) {
                result.append("_#"_S)
                        .append(baseLocale.variant);
            }

            if (e && (l || r)) {
                result.append('_');
                if (!s) {
                    result.append('#');
                }
                String id;
                for (const Map<Character, String>::Entry &extension: localeExtensions.entrySet()) {
                    id += extension.key().toString() + "-"_S + extension.value();
                }
                result.append(id);
            }

            return result.toString();
        }

        String Locale::toLanguageTag() const {
            String lTag = BCP47_TAG;
            if (!lTag.isEmpty()) {
                return lTag;
            }

            LanguageTag tag = LanguageTag::parseLocale(baseLocale, localeExtensions);
            StringBuffer buf;

            String subtag = tag.language;
            if (!subtag.isEmpty()) {
                buf.append(LanguageTag::canonicalizeLanguage(subtag));
            }

            subtag = tag.script;
            if (!subtag.isEmpty()) {
                buf.append(LanguageTag::SEP);
                buf.append(LanguageTag::canonicalizeScript(subtag));
            }

            subtag = tag.region;
            if (!subtag.isEmpty()) {
                buf.append(LanguageTag::SEP);
                buf.append(LanguageTag::canonicalizeRegion(subtag));
            }

            ArrayList<String> subtags = tag.variants;
            for (String s: subtags) {
                buf.append(LanguageTag::SEP);
                // preserve casing
                buf.append(s);
            }

            subtags = tag.extensions;
            for (String s: subtags) {
                buf.append(LanguageTag::SEP);
                buf.append(LanguageTag::canonicalizeExtension(s));
            }

            subtag = tag.privateuse;
            if (!subtag.isEmpty()) {
                if (buf.length() > 0) {
                    buf.append(LanguageTag::SEP);
                }
                buf.append(LanguageTag::PRIVATEUSE).append(LanguageTag::SEP);
                // preserve casing
                buf.append(subtag);
            }

            String langTag = buf.toString();
            // synchronized
            {
                if (BCP47_TAG.isEmpty()) {
                    (String &) BCP47_TAG = langTag;
                }
            }
            return langTag;
        }

        Locale Locale::forLanguageTag(const String &languageTag) {
            ParseStatus status;
            LanguageTag const tag = LanguageTag::parse(languageTag, status);
            LocaleBuilder bldr = {};
            bldr.setLanguageTag(tag);
            BaseLocale base = bldr.baseLocale();
            HashMap<Character, String> exts = bldr.localeExtensions();
            if (exts.isEmpty() && !base.variant.isEmpty()) {
                if (LocaleUtils::equalsIgnoreCase(base.language, "ja"_S) && base.script.isEmpty() &&
                    LocaleUtils::equalsIgnoreCase(base.region, "JP"_S) &&
                    LocaleUtils::equalsIgnoreCase(base.variant, "JP"_S)) {
                    exts.put('u', "ca-japanese");
                } else if (LocaleUtils::equalsIgnoreCase(base.language, "th"_S) && base.script.isEmpty() &&
                           LocaleUtils::equalsIgnoreCase(base.region, "TH"_S) &&
                           LocaleUtils::equalsIgnoreCase(base.variant, "TH"_S)) {
                    exts.put('u', "nu-thai");
                }
            }
            return createLocale(base, exts);
        }

        String Locale::iso3language() const {
            String lang = baseLocale.language;
            if (lang.length() == 3) {
                return lang;
            }
            if (lang.isEmpty()) {
                return ""_S;
            }

            String language3 = iso3Code(lang, LocaleISOData::ISO_LANGUAGES_TABLE);
            if (language3.isEmpty()) {
                MissingResourceException("Couldn't find 3-letter language code for " + lang, "FormatData_" + toString(),
                                         "ShortLanguage")
                        .throws(__trace("core.util.Locale"));
            }
            return language3;
        }

        String Locale::iso3country() const {
            if (baseLocale.region.isEmpty()) {
                return ""_S;
            }
            String country3 = iso3Code(baseLocale.region, LocaleISOData::ISO_COUNTRIES_TABLE);
            if (country3.isEmpty()) {
                MissingResourceException("Couldn't find 3-letter country code for " + baseLocale.region,
                                         "FormatData_" + toString(), "ShortCountry")
                        .throws(__trace("core.util.Locale"));
            }
            return country3;
        }

        String Locale::iso3Code(const String &iso2Code, const String &table) {
            gint const codeLength = iso2Code.length();
            if (codeLength == 0) {
                return "";
            }

            gint const tableLength = table.length();
            gint index = tableLength;
            if (codeLength == 2) {
                gchar const c1 = iso2Code.charAt(0);
                gchar const c2 = iso2Code.charAt(1);
                for (index = 0; index < tableLength; index += 5) {
                    if (table.charAt(index) == c1
                        && table.charAt(index + 1) == c2) {
                        break;
                    }
                }
            }
            return index < tableLength ? table.subString(index + 2, index + 5) : "";
        }

        String Locale::displayLanguage() const {
            return displayLanguage(defaultLocale(Category::DISPLAY));
        }

        String Locale::displayScript() const {
            return displayScript(defaultLocale(Category::DISPLAY));
        }

        String Locale::displayLanguage(const Locale &inLocale) const {
            return displayString(baseLocale.language, ""_S, inLocale, DISPLAY_LANGUAGE);
        }

        String Locale::displayScript(const Locale &inLocale) const {
            return displayString(baseLocale.script, ""_S, inLocale, DISPLAY_SCRIPT);
        }

        String Locale::displayCountry() const {
            return displayCountry(defaultLocale(Category::DISPLAY));
        }

        String Locale::displayCountry(const Locale &inLocale) const {
            return displayString(baseLocale.region, ""_S, inLocale, DISPLAY_COUNTRY);
        }

        String Locale::displayVariant() const {
            return displayVariant(defaultLocale(Category::DISPLAY));
        }

        String Locale::displayVariant(const Locale &inLocale) const {
            return displayString(baseLocale.variant, ""_S, inLocale, DISPLAY_VARIANT);
        }

        String Locale::displayName() const {
            return displayName(defaultLocale(Category::DISPLAY));
        }

        String Locale::displayName(const Locale &inLocale) const {
            //
        }

        Object &Locale::clone() const {
            return Unsafe::allocateInstance<Locale>(*this);
        }

        gint Locale::hash() const {
            return baseLocale.hash() * 31 + localeExtensions.hash();
        }

        gbool Locale::equals(const Object &obj) const {
            if (this == &obj) {                      // quick check
                return true;
            }
            if (!Class<Locale>::hasInstance(obj)) {
                return false;
            }
            BaseLocale const &otherBase = ((Locale const &) obj).baseLocale;
            return baseLocale.equals(otherBase) &&
                   localeExtensions.equals(((Locale const &) obj).localeExtensions);
        }

        Locale Locale::of(const String &language, const String &country) {
            return of(language, ""_S, country, ""_S);
        }

        Locale Locale::of(const String &language, const String &script, const String &country) {
            return of(language, script, country, ""_S);
        }

        StringArray Locale::iso2Table(const String &table) {
            gint const len = table.length() / 5;
            StringArray isoTable = StringArray(len);
            for (gint i = 0, j = 0; i < len; i++, j += 5) {
                isoTable[i] = table.subString(i, i + 2);
            }
            return isoTable;
        }

        StringArray Locale::iso3Table(const String &table) {
            gint const len = table.length() / 5;
            StringArray isoTable = StringArray(len);
            for (gint i = 0, j = 0; i < len; i++, j += 5) {
                isoTable[i] = table.subString(i + 2, i + 5);
            }
            return isoTable;
        }

        Locale Locale::of(const String &language, const String &script, const String &country, const String &variant) {
            LocaleBuilder lb;
            lb.setLanguage(language);
            lb.setScript(script);
            lb.setRegion(country);
            lb.setVariant(variant);
            return createLocale(lb.baseLocale(), EMPTY);
        }

        String const Locale::BaseLocale::SEP = "_"_S;

        Locale::BaseLocale::BaseLocale(String language, String script, String region, String variant) :
                language(Unsafe::moveInstance(language)),
                script(Unsafe::moveInstance(script)),
                region(Unsafe::moveInstance(region)),
                variant(Unsafe::moveInstance(variant)) {}

        String Locale::BaseLocale::convertOldISOCode(const String &language) {
            if (language.length() == 2) {
                if (language == u"iw"_S)
                    return u"he"_S;
                else if (language == u"in"_S)
                    return u"id"_S;
                else if (language == u"ji"_S)
                    return u"yi"_S;
            }
            return language;
        }

        gbool Locale::BaseLocale::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (!Class<BaseLocale>::hasInstance(o)) {
                return false;
            }
            BaseLocale const &other = (const BaseLocale &) o;
            return language == other.language
                   && script == other.script
                   && region == other.region
                   && variant == other.variant;
        }

        String Locale::BaseLocale::toString() const {
            StringBuffer sb = {};
            if (!language.isEmpty()) {
                sb.append("language=" + language);
            }
            if (!script.isEmpty()) {
                if (!sb.isEmpty())
                    sb.append(", "_S);
                sb.append("script=" + script);
            }
            if (!region.isEmpty()) {
                if (!sb.isEmpty())
                    sb.append(", "_S);
                sb.append("region=" + region);
            }
            if (!variant.isEmpty()) {
                if (!sb.isEmpty())
                    sb.append(", "_S);
                sb.append("variant="_S + variant);
            }
            return sb.toString();
        }

        gint Locale::BaseLocale::hash() const {
            gint h = hashcode;
            if (h == 0) {
                // Generating a hash value from language, script, region
                h = language.hash();
                h = 31 * h + script.hash();
                h = 31 * h + region.hash();
                if (h != 0) {
                    (gint &)
                            hashcode = h;
                }
            }
            return h;
        }

        Object &Locale::BaseLocale::clone() const {
            return Unsafe::allocateInstance<BaseLocale>(*this);
        }

        Locale::BaseLocale Locale::BaseLocale::of(const String &language) {
            return BaseLocale(Unsafe::moveInstance(language), ""_S, ""_S, ""_S);
        }

        Locale::BaseLocale Locale::BaseLocale::of(const String &language, const String &region) {
            return BaseLocale(Unsafe::moveInstance(language), ""_S, Unsafe::moveInstance(region), ""_S);
        }

    }
} // core
