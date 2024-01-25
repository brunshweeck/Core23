//
// Created by T.N.Brunshweeck on 24/01/2024.
//

#include <core/util/Locale.h>
#include <core/util/private/LocaleUtils.h>
#include <core/io/private/NativeBuffer.h>
#include <Windows.h>

namespace core {
    namespace util {

        using namespace io;

        Locale Locale::initDefaultLocale() {
            // check windows locale
            LCID const locale = GetUserDefaultLCID();
            NativeBuffer const buf = NativeBuffer(64);

            // language
            DWORD r = GetLocaleInfoW(locale, LOCALE_SISO639LANGNAME, (LPWSTR) buf.address(), buf.size() / 2);
            if (r != 2 && r != 3) {
                r = GetLocaleInfoW(locale, LOCALE_SISO639LANGNAME2, (LPWSTR) buf.address(), buf.size() / 2);
            }
            if (r == 0) {
                r = GetLocaleInfoW(locale, LOCALE_ILANGUAGE, (LPWSTR) buf.address(), buf.size() / 2);
            }
            String language = String((LPCWSTR) buf.address(), 0, r);
            if (LocaleUtils::equalsIgnoreCase(language, "0814"_S)) {
                language = "nn"_S;
            }

            // script
            r = GetLocaleInfoW(locale, LOCALE_SSCRIPTS, (LPWSTR) buf.address(), buf.size() / 2);
            String script = String((LPCWSTR) buf.address(), 0, r);
            gint i = script.indexOf(';');
            gint j = 0;
            while(i == j) {
                j += 1;
                i = script.indexOf(';', j);
            }
            if (i > 0) {
                script = script.subString(j, i);
            }

            // region
            r = GetLocaleInfoW(locale, LOCALE_SISO3166CTRYNAME, (LPWSTR) buf.address(), buf.size() / 2);
            if (r != 2 && r != 3) {
                r = GetLocaleInfoW(locale, LOCALE_SISO3166CTRYNAME2, (LPWSTR) buf.address(), buf.size() / 2);
            }
            if (r == 0) {
                r = GetLocaleInfoW(locale, LOCALE_ICOUNTRY, (LPWSTR) buf.address(), buf.size() / 2);
            }
            String const region = String((LPCWSTR) buf.address(), 0, r);

            return createLocale(Unsafe::moveInstance(language), Unsafe::moveInstance(script),
                                Unsafe::moveInstance(region), "WIN"_S);
        }

        Locale Locale::initDefaultLocale(Locale::Category category) {
            CORE_IGNORE(category);
            return initDefaultLocale();
        }

        Locale Locale::system(Locale::Category category) {
            CORE_IGNORE(category);
            // check windows locale
            LCID const locale = GetSystemDefaultLCID();
            NativeBuffer const buf = NativeBuffer(64);

            // language
            DWORD r = GetLocaleInfoW(locale, LOCALE_SISO639LANGNAME, (LPWSTR) buf.address(), buf.size() / 2);
            if (r != 2 && r != 3) {
                r = GetLocaleInfoW(locale, LOCALE_SISO639LANGNAME2, (LPWSTR) buf.address(), buf.size() / 2);
            }
            if (r == 0) {
                r = GetLocaleInfoW(locale, LOCALE_ILANGUAGE, (LPWSTR) buf.address(), buf.size() / 2);
            }
            String language = String((LPCWSTR) buf.address(), 0, r);
            if (LocaleUtils::equalsIgnoreCase(language, "0814"_S)) {
                language = "nn"_S;
            }

            // script
            r = GetLocaleInfoW(locale, LOCALE_SSCRIPTS, (LPWSTR) buf.address(), buf.size() / 2);
            String script = String((LPCWSTR) buf.address(), 0, r);
            gint i = script.indexOf(';');
            gint j = 0;
            while(i == j) {
                j += 1;
                i = script.indexOf(';', j);
            }
            if (i > 0) {
                script = script.subString(j, i);
            }

            // region
            r = GetLocaleInfoW(locale, LOCALE_SISO3166CTRYNAME, (LPWSTR) buf.address(), buf.size() / 2);
            if (r != 2 && r != 3) {
                r = GetLocaleInfoW(locale, LOCALE_SISO3166CTRYNAME2, (LPWSTR) buf.address(), buf.size() / 2);
            }
            if (r == 0) {
                r = GetLocaleInfoW(locale, LOCALE_ICOUNTRY, (LPWSTR) buf.address(), buf.size() / 2);
            }
            String const region = String((LPCWSTR) buf.address(), 0, r);

            return createLocale(Unsafe::moveInstance(language), Unsafe::moveInstance(script),
                                Unsafe::moveInstance(region), "WIN"_S);
        }

    }
}
