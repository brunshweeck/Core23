//
// Created by T.N.Brunshweeck on 17/11/2023.
//


#include <core/String.h>
#include <core/private/Unsafe.h>
#include <core/StringBuffer.h>
#include "Locale.h"

namespace core {
    namespace util {
        Locale::Locale(String language) :
                Locale((String &&) language, "", "") {}

        Locale::Locale(String language, String country) :
                Locale((String &&) language, "", (String &&) country) {}

        Locale::Locale(String language, String script, String country) : Locale(
                BaseLocale::parseLanguage((String &&) language),
                BaseLocale::parseCountry((String &&) country),
                BaseLocale::parseScript((String &&) script)) {}

        constexpr Locale::Locale(const Locale &locale) : base(locale.base) {}

        String Locale::language() const {
            return base.languageCode();
        }

        String Locale::script() const {
            return base.scriptCode();
        }

        String Locale::country() const {
            return base.countryCode();
        }

        String Locale::toString() const {
            return name();
        }

        String Locale::name() const {
            return base.name('_');
        }

        String Locale::toLanguageTag() const {
            return base.bcp47Name();
        }

        Locale Locale::forLanguageTag(String languageTag) {
            Locale locale = BaseLocale::parseLocale((String &&) languageTag);
            locale.base = locale.base.addSubTag();
            return locale;
        }

        String Locale::iso3language() const {
            return base.languageISO3Code();
        }

        String Locale::iso3country() const {
            return base.countryISO3Code();
        }

        String Locale::displayLanguage() const {
            return base.languageName();
        }

        String Locale::nativeLanguage() const {
            return base.nativeLanguageName();
        }

        String Locale::displayScript() const {
            return base.scriptName();
        }

        String Locale::displayCountry() const {
            return base.countryName();
        }

        String Locale::nativeCountry() const {
            return base.nativeCountryName();
        }

        String Locale::displayName() const {
            String lang = displayLanguage();
            String script = displayScript();
            String country = displayCountry();
            StringBuffer sb;
            if (!script.isEmpty() && !lang.isEmpty() && !country.isEmpty())
                sb.append(lang).append(u" (").append(script).append(u", ").append(country).append(u")");
            else if (script.isEmpty() && !lang.isEmpty() && !country.isEmpty())
                sb.append(lang).append(u" (").append(country).append(u")");
            else if (!script.isEmpty() && lang.isEmpty() && !country.isEmpty())
                sb.append(script).append(u" (").append(country).append(u")");
            else if (!script.isEmpty() && !lang.isEmpty() && country.isEmpty())
                sb.append(lang).append(u" (").append(script).append(u")");
            return sb.toString();
        }

        Object &Locale::clone() const {
            return native::Unsafe::U.createInstance<Locale>(*this);
        }

        gint Locale::hash() const {
            return (gint) base.language << 20 | (gint) base.country << 10 | (gint) base.script;
        }

        gbool Locale::equals(const Object &o) const {
            if (!Class<Locale>::hasInstance(o))
                return false;
            return base.equals(CORE_CAST(const Locale&, o).base);
        }

        Locale Locale::forName(String locale) {
            return Locale(String());
        }

        Locale Locale::of(String language, String script, String country) {
            return Locale(String());
        }

        Locale Locale::of(Locale::Language language, Locale::Script script, Locale::Country country) {
            Locale locale = Locale(BaseLocale::normalize(language), BaseLocale::normalize(country),
                                   BaseLocale::normalize(script));
            return locale;
        }

        Locale Locale::systemLocale() {
            return Locale(String());
        }

        Locale Locale::defaultLocale() {
            return Locale(core::String());
        }

        void Locale::setDefaultLocale(Locale newLocale) {}

        List<String> &Locale::availableUILanguages() {
            return *(List<String> *) 0;
        }

        List<Locale> &Locale::availableLocales() {
            return *(List<Locale> *) 0;
        }


        Object &Locale::BaseLocale::clone() const {
            return native::Unsafe::U.createInstance<BaseLocale>(*this);
        }

        String Locale::BaseLocale::languageCode() const {
            return {};
        }

        String Locale::BaseLocale::languageISO3Code() const {
            return {};
        }

        String Locale::BaseLocale::languageName() const {
            return {};
        }

        String Locale::BaseLocale::countryCode() const {
            return {};
        }

        String Locale::BaseLocale::countryISO3Code() const {
            return {};
        }

        String Locale::BaseLocale::countryName() const {
            return {};
        }

        String Locale::BaseLocale::scriptCode() const {
            return {};
        }

        String Locale::BaseLocale::scriptName() const {
            return {};
        }

        gint Locale::BaseLocale::findLocaleIndex() const {
            return {};
        }

        gint Locale::BaseLocale::findLocaleIndex0() const {
            return {};
        }

        gint Locale::BaseLocale::findLocaleDataIndex() const {
            return {};
        }

        String Locale::BaseLocale::bcp47Name() const {
            return {};
        }

        String Locale::BaseLocale::name(gchar sep) const {
            return {};
        }

        String Locale::BaseLocale::nativeLanguageName() const {
            return {};
        }

        String Locale::BaseLocale::nativeCountryName() const {
            return {};
        }

        gbool Locale::BaseLocale::equals(const Object &object) const {
            if (!Class<BaseLocale>::hasInstance(object))
                return false;
            BaseLocale b = (const BaseLocale &) object;
            return language == b.language && country == b.country && script == b.script;
        }

        gbool Locale::BaseLocale::validateSC(const Locale::BaseLocale &target) const {
            return {};
            //
        }

        gbool Locale::BaseLocale::validateL(Locale::Language lang) const {
            return {};
            //
        }

        gbool Locale::BaseLocale::validate() const {
            return {};
            //
        }

        Locale::BaseLocale Locale::BaseLocale::addSubTag() const {
            return {};
            //
        }

        Locale::BaseLocale Locale::BaseLocale::deleteSubTag() const {
            return {};
            //
        }

        Locale::Language Locale::BaseLocale::normalize(Locale::Language language) {
            return {};
            //
        }

        Locale::Country Locale::BaseLocale::normalize(Locale::Country country) {
            return {};
            //
        }

        Locale::Script Locale::BaseLocale::normalize(Locale::Script script) {
            return {};
            //
        }

        Locale::Language Locale::BaseLocale::parseLanguage(String language) {
            return {};
            //
        }

        Locale::Language Locale::BaseLocale::parseLanguageName(String language) {
            return {};
            //
        }

        Locale::Language Locale::BaseLocale::parseNativeLanguageName(String language) {
            return {};
            //
        }

        Locale::Country Locale::BaseLocale::parseCountry(String country) {
            return {};
            //
        }

        Locale::Country Locale::BaseLocale::parseCountryName(String country) {
            return {};
            //
        }

        Locale::Country Locale::BaseLocale::parseNativeCountryName(String country) {
            return {};
            //
        }

        Locale::Script Locale::BaseLocale::parseScript(String script) {
            return {};
            //
        }

        Locale::Script Locale::BaseLocale::parseScriptName(String script) {
            return {};
            //
        }

        Locale Locale::BaseLocale::parseLocale(String locale) {
            return Locale(String());
            //
        }

    }
} // core