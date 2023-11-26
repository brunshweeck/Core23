//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "Trace.h"
#include "ArgumentException.h"
#include "Character.h"
#include "Integer.h"

namespace core {
    namespace {

        static gint findTemplateClosing(const String &str, gint start, gint limit) {
            for (gint i = start + 1; i < limit; ++i) {
                gchar ch = str.charAt(i);
                if (ch == '>')
                    return i;
                else if (ch == '<') {
                    gint j = findTemplateClosing(str, i, limit);
                    if (j == limit)
                        break;
                    i = j;
                }
            }
            return limit;
        }

        static gint findParenthesesClosing(const String &str, gint start, gint limit) {
            for (gint i = start + 1; i < limit; ++i) {
                gchar ch = str.charAt(i);
                if (ch == ')')
                    return i;
                else if (ch == '(') {
                    gint j = findParenthesesClosing(str, i, limit);
                    if (j == limit)
                        break;
                    i = j;
                }
            }
            return limit;
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
            gint j = findTemplateClosing(str, i, length);
            if (j == length)
                ArgumentException("Malformed class name").throws(__trace("core.Trace"));
            str2 += str.subString(last, i);
            i = j + 1;
            last = i;
            i = str.indexOf('<', last);
        }
        str2 += str.subString(last);
        str = str2.replace(" ", "");
        length = str.length();
        gbool letter = false;
        gbool sep = false; // "::" or '.' found
        gbool par = false;
        for (gint j = 0; j < length; ++j) {
            gchar ch = str.charAt(j);
            if (Character::isLetterOrNumber(ch) || ch == '$' || ch == '_') {
                if (Character::isNumber(ch)) {
                    if (!letter)
                        ArgumentException("Malformed class name").throws(__trace("core.Trace"));
                }
                letter = true;
                sep = false;
            } else if (ch == ':') {
                if (j + 1 >= length || sep)
                    // A:::B -> error
                    ArgumentException("Malformed class name").throws(__trace("core.Trace"));
                ch = str.charAt(j + 1);
                if (ch != ':')
                    // A:B -> error
                    ArgumentException("Malformed class name").throws(__trace("core.Trace"));
                sep = true;
                j += 1;
            } else if (ch == '.') {
                if (sep)
                    // A..B -> error
                    ArgumentException("Malformed class name").throws(__trace("core.Trace"));
                sep = true;
                j += 1;
            } else if (ch == '(' || ch == ')') {
                continue;
            } else
                ArgumentException("Malformed class name").throws(__trace("core.Trace"));
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
            gint j = findTemplateClosing(str, i, length);
            if (j == length)
                ArgumentException("Malformed method name").throws(__trace("core.Trace"));
            str2 += str.subString(last, i);
            if (str.startsWith("lambda(", i + 1))
                str2 += "<lambda>";
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
            gint j = findParenthesesClosing(str, i, length);
            if (j == length)
                ArgumentException("Malformed method name").throws(__trace("core.Trace"));
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
        i = str2.indexOf(" [with ");
        if (i >= 0) {
            str2 = str2.subString(0, i);
        }
        str = str2;
        if (str.startsWith("operator")) {
            if (str.length() == 8)
                str = "<call>";
            else if (str.endsWith("[]"))
                str = "<get>";
            else if (str.endsWith("="))
                str = "<assign>";
            else if (str.endsWith("=="))
                str = "<eq>";
            else if (str.endsWith("!="))
                str = "<ne>";
            else if (str.endsWith("<"))
                str = "<lt>";
            else if (str.endsWith("<="))
                str = "<le>";
            else if (str.endsWith(">"))
                str = "<gt>";
            else if (str.endsWith(">="))
                str = "<ge>";
            else if (str.endsWith("->"))
                str = "<action>";
            else if (str.endsWith("+"))
                str = "<add>";
            else if (str.endsWith("+="))
                str = "<add-assign>";
            else if (str.endsWith("++"))
                str = "<inc>";
            else if (str.endsWith("-"))
                str = "<sub>";
            else if (str.endsWith("-="))
                str = "<sub-assign>";
            else if (str.endsWith("--"))
                str = "<dec>";
            else if (str.endsWith("&"))
                str = "<and>";
            else if (str.endsWith("&="))
                str = "<and-assign>";
            else if (str.endsWith("&&"))
                str = "<and>";
            else if (str.endsWith("|"))
                str = "<or>";
            else if (str.endsWith("|="))
                str = "<or-assign>";
            else if (str.endsWith("||"))
                str = "<or>";
            else if (str.endsWith("*"))
                str = "<mul>";
            else if (str.endsWith("*="))
                str = "<mul-assign>";
            else if (str.endsWith("/"))
                str = "<div>";
            else if (str.endsWith("/="))
                str = "<div-assign>";
            else if (str.endsWith("^"))
                str = "<xor>";
            else if (str.endsWith("^="))
                str = "<xor-assign>";
            else if (str.endsWith("<=>"))
                str = "<ord>";
            else if (str.endsWith("!"))
                str = "<not>";
            else if (str.endsWith("~"))
                str = "<compl>";
            else if (str.startsWith("\"\"", 8))
                str = "<uld>";
            else if (str.startsWith("u\"\"", 8))
                str = "<u16-uld>";
            else if (str.startsWith("U\"\"", 8))
                str = "<u32-uld>";
            else if (str.startsWith("L\"\"", 8))
                str = "<w-uld>";
            else if (str.startsWith("R\"\"", 8))
                str = "<raw-uld>";
            else if (str.startsWith("uR\"()\"", 8))
                str = "<u16raw-uld>";
            else if (str.startsWith("UR\"()\"", 8))
                str = "<u32raw-uld>";
            else if (str.startsWith("LR\"()\"", 8))
                str = "<wraw-uld>";
        }
        length = str.length();
        gbool letter = false;
        gbool sep = false;
        for (gint j = 0; j < length; ++j) {
            gchar ch = str.charAt(j);
            if (Character::isLetterOrNumber(ch) || ch == '$' || ch == '_') {
                if (Character::isNumber(ch)) {
                    if (!letter)
                        ArgumentException("Malformed method name: " + str).throws(__trace("core.Trace"));
                }
                letter = true;
                sep = false;
            } else if (ch == ':') {
                if (j + 1 >= length || sep)
                    ArgumentException("Malformed method name: " + str).throws(__trace("core.Trace"));
                ch = str.charAt(j + 1);
                if (ch != ':')
                    ArgumentException("Malformed method name: " + str).throws(__trace("core.Trace"));
                sep = true;
                j += 1;
            } else if (ch == '(' || ch == ')' || ch == '<' || ch == '>' || ch == '{' || ch == '}')
                continue;
            else
                ArgumentException("Malformed method name: " + str).throws(__trace("core.Trace"));
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
            ArgumentException("Illegal class name, for input \"" + className + "\"").throws(__trace("core.Trace"));
        }
        if (methodName.isASCII() && !methodName.isBlank()) {
            metName = resolveMethodName(methodName);
            if (metName.equals(className))
                metName = "<constructor>";
            else {
                gint i = clsName.lastIndexOf("::");
                if (i >= 0 && i + 2 < clsName.length()) {
                    String tmp = clsName.subString(i + 2);
                    if (metName.equals(tmp)) {
                        metName = "<constructor>";
                    }
                }
            }
        } else {
            ArgumentException("Illegal method name, for input \"" + methodName + "\"").throws(__trace("core.Trace"));
        }
        if (!fileName.isBlank()) {
            fName = fileName;
        }
        fLine = lineNumber > 0 ? lineNumber : 0;
    }

    Trace::Trace(const String &className, const String &methodName, const String &fileName, gint lineNumber)
            : Trace("", "", className, methodName, fileName, lineNumber) {}

    String Trace::toString() const {
        String sb;
        if (!modName.isBlank()) {
            sb += modName;
            if (!modVersion.isBlank()) {
                sb += "@";
                sb += modVersion;
            }
            sb += "/";
        }
        if (!clsName.isBlank()) {
            sb += clsName;
            sb += ".";
        }
        sb += metName.startsWith(clsName) ? "<init>" : metName;
        if (fLine == 0)
            sb += "(#)";
        else {
            if (!fName.isBlank()) {
                sb += "(";
                sb += fName;
                sb += ":";
            } else
                sb += "(#:";
            sb += Integer::toString(fLine) + ")";
        }
        return sb;
    }

    gbool Trace::equals(const Object &object) const {
        if(!Class<Trace>::hasInstance(object))
            return false;
        const Trace& trace = CORE_CAST(const Trace&, object);
        if(trace.fLine != fLine)
            return false;
        if(trace.fName != fName)
            return false;
        if(trace.modName != modName)
            return false;
        if(trace.modVersion != modVersion)
            return false;
        if(trace.clsName != clsName)
            return false;
        if(trace.metName != metName)
            return false;
        return true;
    }

    String Trace::moduleName() const {
        return modName;
    }

    String Trace::moduleVersion() const {
        return modVersion;
    }

    String Trace::fileName() const {
        return fName;
    }

    String Trace::className() const {
        return clsName;
    }

    String Trace::methodName() const {
        return metName;
    }

    gint Trace::lineNumber() const {
        return fLine;
    }

    Object &Trace::clone() const {
        return native::Unsafe::U.createInstance<Trace>(*this);
    }

    gint Trace::hash() const {
        return (((modName.hash()
                  * 7 + modVersion.hash())
                 * 7 + clsName.hash())
                * 7 + metName.hash())
               * 7 + fLine;
    }

} // core