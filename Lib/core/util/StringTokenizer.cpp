//
// Created by T.N.Brunshweeck on 14/12/2023.
//

#include <core/private/Unsafe.h>
#include <core/NoSuchItemException.h>
#include <core/native/IntArray.h>
#include "StringTokenizer.h"

namespace core {
    namespace util {

        CORE_ALIAS(U, native::Unsafe);

        void StringTokenizer::setMaxDelimiter() {
            gint const dSize = delimiters.length();
            if (dSize == 0) {
                maxDelim = 0;
                return;
            }
            gint m = 0;
            gint c = {};
            gint count = 0;
            for (gint i = 0; i < dSize; i += Character::charCount(c)) {
                c = delimiters.charAt(i);
                if (c >= Character::MIN_HIGH_SURROGATE && c <= Character::MAX_LOW_SURROGATE) {
                    c = delimiters.codePointAt(i);
                    hasSurrogates = true;
                }
                if (m < c)
                    m = c;
                count++;
            }
            maxDelim = m;


            if (hasSurrogates) {
                delims = IntArray(count);
                for (gint i = 0, j = 0; i < count; i++, j += Character::charCount(c)) {
                    c = delimiters.codePointAt(j);
                    delims[i] = c;
                }
            }
        }

        StringTokenizer::StringTokenizer(const String &str, const String &delim, gbool returnDelimiter) :
                str(str), delimiters(delim), retDelims(returnDelimiter),
                delimsModified(false), cursor(0), limit(str.length()), mark(-1), maxDelim(0) { setMaxDelimiter(); }

        StringTokenizer::StringTokenizer(const String &str, const String &delimiters) :
                StringTokenizer(str, delimiters, false) {}

        StringTokenizer::StringTokenizer(const String &str) : StringTokenizer(str, " \t\n\r\f", false) {}

        gint StringTokenizer::skipDelimiter(gint startPos) const {
            gint position = startPos;
            while (!retDelims && position < limit) {
                if (!hasSurrogates) {
                    gchar const c = str.charAt(position);
                    if ((c > maxDelim) || (delimiters.indexOf(c) < 0))
                        break;
                    position++;
                } else {
                    gint const c = str.codePointAt(position);
                    if ((c > maxDelim) || !isDelimiter(c)) {
                        break;
                    }
                    position += Character::charCount(c);
                }
            }
            return position;
        }

        gint StringTokenizer::scanToken(gint startPos) const {
            gint position = startPos;
            while (position < limit) {
                if (!hasSurrogates) {
                    gchar const c = str.charAt(position);
                    if ((c <= maxDelim) && (delimiters.indexOf(c) >= 0))
                        break;
                    position++;
                } else {
                    gint const c = str.codePointAt(position);
                    if ((c <= maxDelim) && isDelimiter(c))
                        break;
                    position += Character::charCount(c);
                }
            }
            if (retDelims && (startPos == position)) {
                if (!hasSurrogates) {
                    gchar const c = str.charAt(position);
                    if ((c <= maxDelim) && (delimiters.indexOf(c) >= 0))
                        position++;
                } else {
                    gint const c = str.codePointAt(position);
                    if ((c <= maxDelim) && isDelimiter(c))
                        position += Character::charCount(c);
                }
            }
            return position;
        }

        gbool StringTokenizer::isDelimiter(gint ch) const {
            for (gint const delimiterCodePoint: delims) {
                if (delimiterCodePoint == ch) {
                    return true;
                }
            }
            return false;
        }

        gbool StringTokenizer::hasMore() const {
            /*
             * Temporarily store this position and use it in the following
             * nextToken() method only if the delimiters haven't been changed in
             * that nextToken() invocation.
             */
            (gint &) mark = skipDelimiter(cursor);
            return (mark < limit);
        }

        String StringTokenizer::nextToken() {
            /*
             * If next position already computed in hasMoreElements() and
             * delimiters have changed between the computation and this invocation,
             * then use the computed value.
             */

            cursor = (mark >= 0 && !delimsModified) ?
                              mark : skipDelimiter(cursor);

            /* Reset these anyway */
            delimsModified = false;
            mark = -1;

            if (cursor >= limit)
                NoSuchItemException().throws(__trace("core.util.StringTokenizer"));
            gint const start = cursor;
            cursor = scanToken(cursor);
            return str.subString(start, cursor);
        }

        String StringTokenizer::nextToken(const String &delim) {
            delimiters = delim;

            /* delimiter string specified, so set the appropriate flag. */
            delimsModified = true;

            setMaxDelimiter();
            return nextToken();
        }

        gint StringTokenizer::countTokens() const {
            gint count = 0;
            gint currentPos = cursor;
            while (currentPos < limit) {
                currentPos = skipDelimiter(currentPos);
                if (currentPos >= limit)
                    break;
                currentPos = scanToken(currentPos);
                count++;
            }
            return count;
        }

    } // util
} // core