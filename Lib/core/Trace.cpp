//
// Created by Brunshweeck on 12/09/2023.
//

#include "Trace.h"
#include <core/StringBuffer.h>
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    namespace {

        gint findTemplateClosing(const String &str, gint start, gint limit) {
            for (gint i = start + 1; i < limit; ++i) {
                gchar const ch = str.charAt(i);
                if (ch == '>')
                    return i;
                else if (ch == '<') {
                    gint const j = findTemplateClosing(str, i, limit);
                    if (j == limit)
                        break;
                    i = j;
                }
            }
            return limit;
        }

        gint findParenthesesClosing(const String &str, gint start, gint limit) {
            for (gint i = start + 1; i < limit; ++i) {
                gchar const ch = str.charAt(i);
                if (ch == ')')
                    return i;
                else if (ch == '(') {
                    gint const j = findParenthesesClosing(str, i, limit);
                    if (j == limit)
                        break;
                    i = j;
                }
            }
            return limit;
        }

        IllegalArgumentException error(const String &message, gint index) {
            return IllegalArgumentException(message + u": at index "_S + String::valueOf(index));
        }
    }

    String Trace::resolveClassName(const String &className) {
        String str = className.strip();
        gint length = str.length();
        String str2;
        gint last = 0;
        // remove all template specification (<.*>)
        gint i = str.indexOf('<');
        while (i >= 0) {
            gint const j = findTemplateClosing(str, i, length);
            if (j == length) {
                IllegalArgumentException(u"Malformed classname: at index "_S + String::valueOf(i))
                        .throws(__trace(u"core.Trace"_S));
            }
            str2 += str.subString(last, i);
            i = j + 1;
            last = i;
            i = str.indexOf('<', last);
        }
        str2 += str.subString(last);
        str = str2;
        length = str.length();
        gbool letter = false; // a-z or A-Z or $ or _ found
        gbool separator = false; // "::" or '.' found
        gbool const par = false;
        for (gint j = 0; j < length; ++j) {
            gchar ch = str.charAt(j);
            if (Character::isLetterOrNumber(ch) || ch == '$' || ch == '_') {
                if (Character::isNumber(ch) && !letter) {
                    error(u"Malformed classname"_S, j).throws(__trace(u"core.Trace"_S));
                }
                letter = true;
                separator = false;
            } else if (ch == ':') {
                if (j + 1 >= length || separator) {
                    // A:::B or A.:B -> error
                    error(u"Malformed classname"_S, j).throws(__trace(u"core.Trace"_S));
                }
                ch = str.charAt(j + 1);
                if (ch != ':') {
                    // A:B -> error
                    error(u"Malformed classname"_S, j + 1).throws(__trace(u"core.Trace"_S));
                }
                separator = true;
                j += 1;
            } else if (ch == '.') {
                if (separator) {
                    // A..B or A::.B -> error
                    error(u"Malformed classname"_S, j).throws(__trace(u"core.Trace"_S));
                }
                separator = true;
                j += 1;
            } else if (ch == '(' || ch == ')') {
                continue;
            } else
                error(u"Malformed classname"_S, j).throws(__trace(u"core.Trace"_S));
        }
        return str;
    }

    String Trace::resolveMethodName(const String &methodName) {
        String str = methodName.strip();
        gint length = str.length();
        String str2;
        gint last = 0;
        // remove all template specification (<.*>)
        gint i = str.indexOf('<');
        while (i >= 0) {
            gint const j = findTemplateClosing(str, i, length);
            if (j == length)
                error(u"Malformed method name"_S, i).throws(__trace(u"core.Trace"_S));
            str2 += str.subString(last, i);
            if (str.startsWith(u"lambda("_S, i + 1)) {
                gint const k = findParenthesesClosing(str, 6, length);
                if (k == length) {
                    error(u"Malformed classname"_S, 6).throws(__trace(u"core.Trace"_S));
                }
                str2 += u"<lambda>"_S;
            }
            i = j + 1;
            last = i;
            i = str.indexOf('<', last);
        }
        str2 += str.subString(last);
        if (str2.endsWith("::"))
            str2 = str2.subString(0, str.length() - 2);
        str = (String &&) str2;
        str2 = {};
        last = 0;
        i = str.indexOf('(');
        while (i >= 0) {
            gint const j = findParenthesesClosing(str, i, length);
            if (j == length)
                IllegalArgumentException("Malformed method name").throws(__trace("core.Trace"));
            str2 += str.subString(last, i);
            i = j + 1;
            last = i;
            i = str.indexOf('(', last);
        }
        str2 += str.subString(last);
        last = 0;
        // remove return type
        i = str2.indexOf(' ');
        if (i >= 0) {
            if (!str2.startsWith("[with ", i + 1))
                str2 = str2.subString(i + 1);
        }
        // return definition of template types
        i = str2.indexOf(" [with ");
        if (i >= 0) {
            str2 = str2.subString(0, i);
        }
        str = str2;
        if (str.startsWith(u"operator"_S)) {
            if (str.length() == 8)
                str = u"<CALL>"_S;
            else if (str.endsWith(u"[]"_S))
                str = u"<GET>"_S;
            else if (str.endsWith(u"="_S))
                str = u"<SET>"_S;
            else if (str.endsWith(u"=="_S))
                str = u"<EQ>"_S;
            else if (str.endsWith(u"!="_S))
                str = u"<NE>"_S;
            else if (str.endsWith(u"<"_S))
                str = u"<LT>"_S;
            else if (str.endsWith(u"<="_S))
                str = u"<LE>"_S;
            else if (str.endsWith(u">"_S))
                str = u"<GT>"_S;
            else if (str.endsWith(u">="_S))
                str = u"<GE>"_S;
            else if (str.endsWith(u"->"_S))
                str = u"<PTR>"_S;
            else if (str.endsWith(u"+"_S))
                str = u"<ADD>"_S;
            else if (str.endsWith(u"+="_S))
                str = u"<INC_SET>"_S;
            else if (str.endsWith(u"++"_S))
                str = u"<INC>"_S;
            else if (str.endsWith(u"-"_S))
                str = u"<SUB>"_S;
            else if (str.endsWith(u"-="_S))
                str = u"<DEC_SET>"_S;
            else if (str.endsWith(u"--"_S))
                str = u"<DEC>"_S;
            else if (str.endsWith(u"&"_S))
                str = u"<AND>"_S;
            else if (str.endsWith(u"&="_S))
                str = u"<AND_SET>"_S;
            else if (str.endsWith(u"&&"_S))
                str = u"<AND>"_S;
            else if (str.endsWith(u"|"_S))
                str = u"<OR>"_S;
            else if (str.endsWith(u"|="_S))
                str = u"<OR_SET>"_S;
            else if (str.endsWith(u"||"_S))
                str = u"<OR>"_S;
            else if (str.endsWith(u"*"_S))
                str = u"<MUL>"_S;
            else if (str.endsWith(u"*="_S))
                str = u"<MUL_SET>"_S;
            else if (str.endsWith(u"/"_S))
                str = u"<DIV>"_S;
            else if (str.endsWith(u"/="_S))
                str = u"<DIV_SET>"_S;
            else if (str.endsWith(u"^"_S))
                str = u"<XOR>"_S;
            else if (str.endsWith(u"^="_S))
                str = u"<XOR_SET>"_S;
            else if (str.endsWith(u"<=>"_S))
                str = u"<ORD>"_S;
            else if (str.endsWith(u"!"_S))
                str = u"<NOT>"_S;
            else if (str.endsWith(u"~"_S))
                str = u"<NOT>"_S;
            else if (str.startsWith(uR"("")"_S, 8))
                str = u"<UDL>"_S;
        }
        length = str.length();
        gbool letter = false;
        gbool sep = false;
        for (gint j = 0; j < length; ++j) {
            gchar ch = str.charAt(j);
            if (Character::isLetterOrNumber(ch) || ch == '$' || ch == '_') {
                if (Character::isNumber(ch)) {
                    if (!letter)
                        error(u"Malformed method name"_S, j).throws(__trace(u"core.Trace"_S));
                }
                letter = true;
                sep = false;
            } else if (ch == ':') {
                if (j + 1 >= length || sep)
                    error(u"Malformed method name"_S, j).throws(__trace(u"core.Trace"_S));
                ch = str.charAt(j + 1);
                if (ch != ':')
                    error(u"Malformed method name"_S, j).throws(__trace(u"core.Trace"_S));
                sep = true;
                j += 1;
            } else if (ch == '(' || ch == ')' || ch == '<' || ch == '>' || ch == '{' || ch == '}')
                continue;
            else
                error(u"Malformed method name"_S, j).throws(__trace(u"core.Trace"_S));
        }
        // remove redundant classname and namespace
        i = str.lastIndexOf("::");
        if (i > 0)
            str = str.subString(i + 2);
        return str;
    }

    Trace::Trace(const String &moduleName, const String &moduleVersion, const String &className,
                 const String &methodName, const String &fileName, gint lineNumber) {
        if (!moduleName.isEmpty()) {
            modName = moduleName;
            modVersion = moduleVersion;
        } else {
            // ignored
        }
        if (className.isASCII()) {
            clsName = resolveClassName(className);
        } else {
            for (int i = 0; i < className.length(); i += 1) {
                gchar const c = className.charAt(i);
                if ('0' <= c && c <= '9' && i == 0) {
                    error(u"Malformed method name"_S, i).throws(__trace(u"core.Trace"_S));
                } else if (c > 0x7F) {
                    error(u"Invalid classname"_S, i).throws(__trace(u"core.Trace"_S));
                }
            }
        }
        if (methodName.isASCII() && !methodName.isBlank()) {
            metName = resolveMethodName(methodName);
            if (metName.equals(className))
                metName = "<constructor>";
            else {
                gint const i = clsName.lastIndexOf("::");
                if (i >= 0 && i + 2 < clsName.length()) {
                    String const tmp = clsName.subString(i + 2);
                    if (metName.equals(tmp)) {
                        metName = "<init>";
                    }
                }
            }
        } else {
            IllegalArgumentException("Illegal method name, for input \"" + methodName + "\"").throws(
                    __trace("core.Trace"));
        }
        if (!fileName.isBlank()) {
            fName = fileName;
        }
        fLine = lineNumber > 0 ? lineNumber : 0;
    }

    Trace::Trace(const String &className, const String &methodName, const String &fileName, gint lineNumber)
            : Trace("", "", className, methodName, fileName, lineNumber) {}

    String Trace::toString() const {
        StringBuffer sb;
        if (!modName.isBlank()) {
            sb.append(modName);
            if (!modVersion.isBlank()) {
                sb.append(u'@').append(modVersion);
            }
            sb.append(u'/');
        }
        if (!clsName.isBlank()) {
            sb.append(clsName.replace(u'.'_S, "::")).append(u'.');
        }
        sb.append(metName);
        if (fLine == 0)
            sb.append(u'(').append(u'#');
        else {
            if (!fName.isBlank()) {
                sb.append(u'(').append(fName).append(u':');
            } else {
                sb.append(u'(').append(u'#').append(u':');
            }
            sb.append(fLine);
        }
        return sb.append(u')').toString();
    }

    gbool Trace::equals(const Object &object) const {
        if (!Class<Trace>::hasInstance(object))
            return false;
        const Trace &trace = (const Trace &) object;
        if (trace.fLine != fLine)
            return false;
        if (trace.fName != fName)
            return false;
        if (trace.modName != modName)
            return false;
        if (trace.modVersion != modVersion)
            return false;
        if (trace.clsName != clsName)
            return false;
        if (trace.metName != metName)
            return false;
        return true;
    }

    String Trace::moduleName() const { return modName; }

    String Trace::moduleVersion() const { return modVersion; }

    String Trace::fileName() const { return fName; }

    String Trace::className() const { return clsName; }

    String Trace::methodName() const { return metName; }

    gint Trace::lineNumber() const { return fLine; }

    Object &Trace::clone() const { return Unsafe::allocateInstance<Trace>(*this); }

    gint Trace::hash() const {
        return (((modName.hash()
                  * 7 + modVersion.hash())
                 * 7 + clsName.hash())
                * 7 + metName.hash())
               * 7 + fLine;
    }

    gint Trace::compareTo(const Trace &other) const {
        gint r = {};
        return (r = modName.compareTo(other.modName)) != 0 ||
               (r = modVersion.compareTo(other.modVersion)) != 0 ||
               (r = clsName.compareTo(other.clsName)) != 0 ||
               (r = metName.compareTo(other.metName)) != 0 ? r :
               fLine - other.fLine;
    }

} // core
