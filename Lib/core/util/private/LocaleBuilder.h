//
// Created by T.N.Brunshweeck on 24/01/2024.
//

#ifndef CORE23_LOCALEBUILDER_H
#define CORE23_LOCALEBUILDER_H

#include <core/util/Locale.h>
#include <core/util/ArrayList.h>
#include <core/util/HashMap.h>
#include <core/util/HashSet.h>
#include <core/util/private/LocaleUtils.h>
#include <core/util/private/LocaleISOData.h>
#include <core/util/private/LocaleSyntaxException.h>
#include <core/util/private/LanguageTag.h>

namespace core {
    namespace util {

        class Locale::LocaleBuilder CORE_FINAL : Object {
        public:
            static gchar PRIVATEUSE_KEY;

            String language = "";
            String script = "";
            String region = "";
            String variant = "";

            HashMap<Character, String> extensions;
            HashSet<String> uattributes;
            HashMap<String, String> ukeywords;


            LocaleBuilder() {}

            LocaleBuilder &setLanguage(const String &language);

            LocaleBuilder setScript(const String &script);

            LocaleBuilder setRegion(const String &region);

            LocaleBuilder setVariant(const String &variant);

            static gbool isSingletonChar(gchar c);

            static gbool isAttribute(const String &s);

            static gbool isKey(const String &s);

            static gbool isTypeSubtag(const String &s);

            LocaleBuilder &addUnicodeLocaleAttribute(const String &attribute);

            LocaleBuilder &removeUnicodeLocaleAttribute(const String &attribute);

            LocaleBuilder &setUnicodeLocaleKeyword(const String& key, const String& type);

            LocaleBuilder &setExtension(gchar singleton, const String &value);

            /*
             * Set extension/subtags in a single string representation
             */
            LocaleBuilder &setExtensions(String subtags);

            /*
             * Set a list of BCP47 extensions and use subtags
             * BCP47 extensions are already validated and well-formed, but may contain duplicates
             */
            LocaleBuilder &setExtensions(const List<String> &bcpExtensions, const String &privateuse);

            /*
             * Reset Builder's internal state with the given language tag
             */
            LocaleBuilder &setLanguageTag(const LanguageTag &langtag);

            LocaleBuilder &setLocale(const BaseLocale &base, const Map<Character, String> &localeExtensions);

            LocaleBuilder &clear();

            LocaleBuilder &clearExtensions();

            BaseLocale baseLocale();

            HashMap<Character, String> localeExtensions();

            /*
             * Remove special use subtag sequence identified by "lvariant"
             * and return the rest. Only used by LocaleExtensions
             */
            static String removePrivateUseVariant(String privuseVal);

            /*
             * Check if the given variant subtags separated by the given
             * separator(s) are valid
             */
            gint checkVariants(const String &variants, const String &sep) const;

            /*
             * Private methods parsing Unicode Locale Extension subtags.
             * Duplicated attributes/keywords will be ignored.
             * The input must be a valid extension subtags (excluding singleton).
             */
            void setUnicodeLocaleExtension(const String& subtags);
        };

    } // util
} // core

#endif //CORE23_LOCALEBUILDER_H
