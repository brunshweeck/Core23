//
// Created by T.N.Brunshweeck on 24/01/2024.
//

#ifndef CORE23_LOCALEUTILS_H
#define CORE23_LOCALEUTILS_H

#include <core/util/Locale.h>

namespace core {
    namespace util {

        /**
         * Collection of static utility methods for Locale support. The
         * methods which manipulate characters or strings support ASCII only.
         */
        class LocaleUtils: public Object {
        private:
            LocaleUtils() = default;

        public:
            /**
             * Compares two ASCII Strings s1 and s2, ignoring case.
             */
            static gbool equalsIgnoreCase(const String &s1, const String &s2);

            static gint compareIgnoreCase(const String &s1, const String &s2);

            static gchar toUpper(gchar c);

            static gchar toLower(gchar c);

            /**
             * Converts the given ASCII String to lower-case.
             */
            static String toLowerString(const String &s);

            static String toUpperString(const String &s);

            static String toTitleString(const String &s);

            static gbool isUpper(gchar c);

            static gbool isLower(gchar c);

            static gbool isAlpha(gchar c);

            static gbool isAlphaString(const String &s);

            static gbool isNumeric(gchar c);

            static gbool isNumericString(const String &s);

            static gbool isAlphaNumeric(gchar c);

            static gbool isAlphaNumericString(const String &s);
        };

    } // util
} // core

#endif //CORE23_LOCALEUTILS_H
