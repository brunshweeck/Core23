//
// Created by T.N.Brunshweeck on 14/12/2023.
//

#ifndef CORE23_STRINGTOKENIZER_H
#define CORE23_STRINGTOKENIZER_H

#include <core/String.h>
#include <core/Character.h>
#include <core/native/IntArray.h>

namespace core {
    namespace util {

        /**
         * The string tokenizer class allows an application to break a
         * string into tokens. The tokenization method is much simpler than
         * the one used by the <b> StreamTokenizer</b>  class. The
         * <b> StringTokenizer</b>  methods do not distinguish among
         * identifiers, numbers, and quoted strings, nor do they recognize
         * and skip comments.
         * <p>
         * The set of delimiters (the characters that separate tokens) may
         * be specified either at creation time or on a per-token basis.
         * <p>
         * An instance of <b> StringTokenizer</b>  behaves in one of two
         * ways, depending on whether it was created with the
         * <b> returnDelims</b>  flag having the value <b> true</b> 
         * or <b> false</b> :
         * <ul>
         * <li>If the flag is <b> false</b> , delimiter characters serve to
         *     separate tokens. A token is a maximal sequence of consecutive
         *     characters that are not delimiters.
         * <li>If the flag is <b> true</b> , delimiter characters are themselves
         *     considered to be tokens. A token is thus either one delimiter
         *     character, or a maximal sequence of consecutive characters that are
         *     not delimiters.
         * </ul><p>
         * A <b> StringTokenizer</b>  object internally maintains a current
         * position within the string to be tokenized. Some operations advance this
         * current position past the characters processed.<p>
         * A token is returned by taking a substring of the string that was used to
         * create the <b> StringTokenizer</b>  object.
         * <p>
         * The following is one example of the use of the tokenizer. The code:
         * <blockquote><pre>
         * @code
         *     StringTokenizer st = new StringTokenizer("this is a test");
         *     while (st.hasMore())
         *         Console::println(st.nextToken());
         *
         * @endcode
         * </pre></blockquote>
         * <p>
         * prints the following output:
         * <blockquote><pre>
         *   <li>  this
         *   <li>  is
         *   <li>  a
         *   <li>  test
         * </pre></blockquote>
         *
         * <p>
         * <b> StringTokenizer</b>  is a legacy class that is retained for
         * compatibility reasons although its use is discouraged in new code. It is
         * recommended that anyone seeking this functionality use the <b> split</b> 
         * method of <b> String</b>  or the java.util.regex package instead.
         * <p>
         * The following example illustrates how the <b> String.split</b> 
         * method can be used to break up a string into its basic tokens:
         * <blockquote><pre>
         * @code
         *     StringArray result = "this is a test".split("\\s");
         *     for (int x=0; x&lt;result.length; x++)
         *         Console::println(result[x]);
         * @endcode
         * </pre></blockquote>
         * <p>
         * prints the following output:
         * <blockquote><pre>
         *   <li>  this
         *   <li>  is
         *   <li>  a
         *   <li>  test
         * </pre></blockquote>
         *
         * @author Brunshweeck Tazeussong
         *
         * @see     core.io.StreamTokenizer
         */
        class StringTokenizer : public Object {
        private:
            String str;
            String delimiters;
            gbool retDelims;
            gbool delimsModified;
            gint cursor;
            gint limit;
            gint mark;

            /**
             * maxDelim stores the value of the delimiter character with the
             * highest value. It is used to optimize the detection of delimiter
             * characters.
             *
             * It is unlikely to provide any optimization benefit in the
             * hasSurrogates case because most string characters will be
             * smaller than the limit, but we keep it so that the two code
             * paths remain similar.
             */
            gint maxDelim;

            /**
             * If delimiters include any surrogates (including surrogate
             * pairs), hasSurrogates is true and the tokenizer uses the
             * different code path. This is because String.indexOf(int)
             * doesn't handle unpaired surrogates as a single character.
             */
            gbool hasSurrogates = false;

            /**
             * When hasSurrogates is true, delimiters are converted to code
             * points and isDelimiter(int) is used to determine if the given
             * codepoint is a delimiter.
             */
            IntArray delims;

            /**
             * Set maxDelim to the highest char in the delimiter set.
             */
            void setMaxDelimiter();

        public:

            /**
             * Constructs a string tokenizer for the specified string. All
             * characters in the <b> delim</b>  argument are the delimiters
             * for separating tokens.
             * <p>
             * If the <b> returnDelimiter</b>  flag is <b> true</b> , then
             * the delimiter characters are also returned as tokens. Each
             * delimiter is returned as a string consisting of a single
             * <a href="../lang/Character.html#unicode">Unicode code point</a>
             * of the delimiter (which may be one or two <b> char</b> s). If the
             * flag is <b> false</b> , the delimiter characters are skipped
             * and only serve as separators between tokens.
             * <p>
             * Note that if <b> delim</b>  is <b> null</b> , this constructor does
             * not throw an exception. However, trying to invoke other methods on the
             * resulting <b> StringTokenizer</b>  may result in a
             * <b> NullPointerException</b> .
             *
             * @param   str            a string to be parsed.
             * @param   delim          the delimiters.
             * @param   returnDelimiter   flag indicating whether to return the delimiters
             *                         as tokens.
             */
            CORE_EXPLICIT StringTokenizer(const String &str, const String &delim, gbool returnDelimiter);

            /**
             * Constructs a string tokenizer for the specified string. The
             * characters in the <b> delim</b>  argument are the delimiters
             * for separating tokens. Delimiter characters themselves will not
             * be treated as tokens.
             * <p>
             * Note that if <b> delim</b>  is <b> null</b> , this constructor does
             * not throw an exception. However, trying to invoke other methods on the
             * resulting <b> StringTokenizer</b>  may result in a
             * <b> NullPointerException</b> .
             *
             * @param   str     a string to be parsed.
             * @param   delim   the delimiters.
             */
            CORE_EXPLICIT StringTokenizer(const String &str, const String &delim);

            /**
             * Constructs a string tokenizer for the specified string. The
             * tokenizer uses the default delimiter set, which is
             * <code>"&nbsp;&#92;t&#92;n&#92;r&#92;f"</code>: the space character,
             * the tab character, the newline character, the carriage-return character,
             * and the form-feed character. Delimiter characters themselves will
             * not be treated as tokens.
             *
             * @param   str   a string to be parsed.
             */
            CORE_EXPLICIT StringTokenizer(const String &str);

        private:

            /**
             * Skips delimiters starting from the specified position. If retDelims
             * is false, returns the index of the first non-delimiter character at or
             * after startPos. If retDelims is true, startPos is returned.
             */
            gint skipDelimiter(gint startPos) const;

            /**
             * Skips ahead from startPos and returns the index of the next delimiter
             * character encountered, or maxPosition if no such delimiter is found.
             */
            gint scanToken(gint startPos) const;

            gbool isDelimiter(gint ch) const;

        public:

            /**
             * Tests if there are more tokens available from this tokenizer's string.
             * If this method returns <b> true</b> , then a subsequent call to
             * <b> nextToken</b>  with no argument will successfully return a token.
             *
             * @return  <b> true</b>  if and only if there is at least one token
             *          in the string after the current position; <b> false</b> 
             *          otherwise.
             */
            virtual gbool hasMore() const;

            /**
             * Returns the next token from this string tokenizer.
             *
             * @return     the next token from this string tokenizer.
             * @throws     NoSuchItemException  if there are no more tokens in this
             *               tokenizer's string.
             */
            virtual String nextToken();

            /**
             * Returns the next token in this string tokenizer's string. First,
             * the set of characters considered to be delimiters by this
             * <b> StringTokenizer</b>  object is changed to be the characters in
             * the string <b> delim</b> . Then the next token in the string
             * after the current position is returned. The current position is
             * advanced beyond the recognized token.  The new delimiter set
             * remains the default after this call.
             *
             * @param      delim   the new delimiters.
             * @return     the next token, after switching to the new delimiter set.
             * @throws     NoSuchItemException  if there are no more tokens in this
             *               tokenizer's string.
             */
            virtual String nextToken(const String &delim);

            /**
             * Calculates the number of times that this tokenizer's
             * <b> nextToken</b>  method can be called before it generates an
             * exception. The current position is not advanced.
             *
             * @return  the number of tokens remaining in the string using the current
             *          delimiter set.
             * @see     StringTokenizer.nextToken()
             */
             virtual gint countTokens() const;
        };

    } // util
} // core

#endif //CORE23_STRINGTOKENIZER_H
