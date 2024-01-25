//
// Created by T.N.Brunshweeck on 24/01/2024.
//

#include "LocaleUtils.h"

namespace core {
    namespace util {
        gbool LocaleUtils::equalsIgnoreCase(const String &s1, const String &s2) {
            if (s1 == s2) {
                return true;
            }

            gint const len = s1.length();
            if (len != s2.length()) {
                return false;
            }

            for (gint i = 0; i < len; i++) {
                gchar const c1 = s1.charAt(i);
                gchar const c2 = s2.charAt(i);
                if (c1 != c2 && toLower(c1) != toLower(c2)) {
                    return false;
                }
            }
            return true;
        }

        gint LocaleUtils::compareIgnoreCase(const String &s1, const String &s2) {
            if (s1 == s2) {
                return 0;
            }
            return toLowerString(s1).compareTo(toLowerString(s2));
        }

        gchar LocaleUtils::toUpper(gchar c) {
            return isLower(c) ? (gchar) (c - 0x20) : c;
        }

        gchar LocaleUtils::toLower(gchar c) {
            return isUpper(c) ? (gchar) (c + 0x20) : c;
        }

        String LocaleUtils::toLowerString(const String &s) {
            gint const len = s.length();
            gint idx = 0;
            for (; idx < len; idx++) {
                if (isUpper(s.charAt(idx))) {
                    break;
                }
            }
            if (idx == len) {
                return s;
            }

            CharArray buf = CharArray(len);
            for (gint i = 0; i < len; i++) {
                gchar const c = s.charAt(i);
                buf[i] = (i < idx) ? c : toLower(c);
            }
            return String(buf);
        }

        String LocaleUtils::toUpperString(const String &s) {
            gint const len = s.length();
            gint idx = 0;
            for (; idx < len; idx++) {
                if (isLower(s.charAt(idx))) {
                    break;
                }
            }
            if (idx == len) {
                return s;
            }

            CharArray buf = CharArray(len);
            for (gint i = 0; i < len; i++) {
                gchar const c = s.charAt(i);
                buf[i] = (i < idx) ? c : toUpper(c);
            }
            return String(buf);
        }

        String LocaleUtils::toTitleString(const String &s) {
            gint len;
            if ((len = s.length()) == 0) {
                return s;
            }
            gint idx = 0;
            if (!isLower(s.charAt(idx))) {
                for (idx = 1; idx < len; idx++) {
                    if (isUpper(s.charAt(idx))) {
                        break;
                    }
                }
            }
            if (idx == len) {
                return s;
            }

            CharArray buf = CharArray(len);
            for (gint i = 0; i < len; i++) {
                gchar const c = s.charAt(i);
                if (i == 0 && idx == 0) {
                    buf[i] = toUpper(c);
                } else if (i < idx) {
                    buf[i] = c;
                } else {
                    buf[i] = toLower(c);
                }
            }
            return String(buf);
        }

        gbool LocaleUtils::isUpper(gchar c) {
            return c >= 'A' && c <= 'Z';
        }

        gbool LocaleUtils::isLower(gchar c) {
            return c >= 'a' && c <= 'z';
        }

        gbool LocaleUtils::isAlpha(gchar c) {
            return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
        }

        gbool LocaleUtils::isAlphaString(const String &s) {
            gint const len = s.length();
            for (gint i = 0; i < len; i++) {
                if (!isAlpha(s.charAt(i))) {
                    return false;
                }
            }
            return true;
        }

        gbool LocaleUtils::isNumeric(gchar c) {
            return (c >= '0' && c <= '9');
        }

        gbool LocaleUtils::isNumericString(const String &s) {
            gint const len = s.length();
            for (gint i = 0; i < len; i++) {
                if (!isNumeric(s.charAt(i))) {
                    return false;
                }
            }
            return true;
        }

        gbool LocaleUtils::isAlphaNumeric(gchar c) {
            return isAlpha(c) || isNumeric(c);
        }

        gbool LocaleUtils::isAlphaNumericString(const String &s) {
            gint const len = s.length();
            for (gint i = 0; i < len; i++) {
                if (!isAlphaNumeric(s.charAt(i))) {
                    return false;
                }
            }
            return true;
        }
    } // util
} // core
