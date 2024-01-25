//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_STRING_H
#define CORE23_STRING_H

#include <core/Comparable.h>
#include <core/CharSequence.h>

namespace core {
    
    /**
     * The <b> String</b>  class represents character strings. All
     * string literals in programs, such as <b> "abc"</b> , are
     * implemented as instances of this class.
     * <p>
     * Strings are constant; their values cannot be changed after they
     * are created. String buffers support mutable strings.
     * Because String objects are immutable they can be shared. For example:
     * <blockquote><pre>
     *     String str = "abc";
     * </pre></blockquote><p>
     * is equivalent to:
     * <blockquote><pre>
     *     char root[] = {'a', 'b', 'c'} ;
     *     String str = {root};
     * </pre></blockquote><p>
     * Here are some more examples of how strings can be used:
     * <blockquote><pre>
     *     String cde = "cde";
     *     "abc" + cde;
     *     String c = "abc"_S.substring(2, 3);
     *     String d = cde.substring(1, 2);
     * </pre></blockquote>
     * <p>
     * The class <b> String</b>  includes methods for examining
     * individual characters of the sequence, for comparing strings, for
     * searching strings, for extracting substrings, and for creating a
     * copy of a string with all characters translated to uppercase or to
     * lowercase. Case mapping is based on the Unicode Standard version
     * specified by the <b style="color:orange;"> Character</b>  class.
     * <p>
     * The language provides special support for the string
     * concatenation operator (&nbsp;+&nbsp;), and for conversion of
     * other objects to strings. For additional information on string
     * concatenation and conversion, see <i>The Language Specification</i>.
     *
     * <p>A <b> String</b>  represents a string in the UTF-16 format
     * in which <em>supplementary characters</em> are represented by <em>surrogate
     * pairs</em> (see the section <a href="Character.html#unicode">Unicode
     * Character Representations</a> in the <b> Character</b>  class for
     * more information).
     * Index values refer to <b> char</b>  code units, so a supplementary
     * character uses two positions in a <b> String</b> .
     * <p>The <b> String</b>  class provides methods for dealing with
     * Unicode code points (i.e., characters), in addition to those for
     * dealing with Unicode code units (i.e., <b> char</b>  values).
     *
     * @implNote The implementation of the string concatenation operator is left to
     * the discretion of a compiler, as glong as the compiler ultimately conforms
     * to <i>The Language Specification</i>. The
     * implementation of string conversion is typically through the method <b> toString</b> ,
     * defined by <b> Object</b>  and inherited by all sub-object classes.
     *
     * @author  Brunshweeck Tazeussong
     * @see     core.Object.toString()
     * @see     core.StringBuffer
     * @see     core.io.Charset
     */
    class String CORE_FINAL : public Object, public Comparable<String>, public CharSequence {
    private:
        /**
         * Storage represent a primitive byte root used to store string value
         */
        CORE_ALIAS(STORAGE, typename Class<gbyte>::Ptr);

        /**
         * The value used to store character.
         */
        STORAGE value = {};

        /**
         * The character count of this String
         */
        gint len = {};

        /**
         * The cache for hash code of this String
         */
        gint hashcode = {};

        /**
         * The cache for hash, tell if hashcode has been calculated
         */
        gbool isHashed = false;

        /**
         * This method is called only by String constructor
         */
        void wrap(glong address, gint bytesPerChar, glong nbChars);

        /**
         * This method is called only by String constructor
         */
        void wrap(glong address, gint bytesPerChar, glong nbChars, gint offset, gint limit);

        friend class StringBuffer;

    public:

        /**
         * Initializes a newly created String object so that it represents
         * an empty character sequence.
         */
        CORE_FAST String() = default;

        /**
         * Initializes a newly created String object so that it represents
         * the same sequence of characters as the argument; in other words, the
         * newly created string is a copy of the argument string. Unless an
         * explicit copy of original is needed.
         *
         * @param original
         *         A String
         */
        String(const String &original);

        /**
         * Initializes a newly created String object so that it represents
         * the same sequence of characters as the argument; in other words, the
         * newly created string is a copy of the argument string. Unless an
         * explicit copy of original is needed.
         *
         * @param original
         *         A String
         */
        String(String &&original) CORE_NOTHROW;

        /**
         * Allocates a new String so that it represents the sequence of
         * characters currently contained in the character root argument. The
         * contents of the character root are copied; subsequent modification of
         * the character root does not affect the newly created string.
         *
         * <p>
         * Example:
         * <blockquote> <pre>
         *  CharArray abc = CharArray.of('a', 'b', 'c'); <br>
         *  String str = String(abc); // produce "abc"_S <br>
         * </pre></blockquote>
         *
         * @param  chars
         *         Array that is the source of characters
         */
        CORE_EXPLICIT String(const CharArray &chars);

        /**
         * Allocates a new String that contains characters from a subarray
         * of the character root argument. The offset argument is the index
         * of the first character of the subarray and the count argument specifies
         * the length of the subarray. The contents of the subarray are copied;
         * subsequent modification of the character root does not affect the newly
         * created string.
         *
         * @param  chars
         *         Array that is the source of characters
         *
         * @param  offset
         *         The initial offset
         *
         * @param  count
         *         The length
         *
         * @throws  IndexException
         *          If offset is negative, count is negative, or
         *          offset is greater than (value length) - count
         */
        CORE_EXPLICIT String(const CharArray &chars, gint offset, gint count);

        /**
         * Allocates a new String so that it represents the sequence of
         * unicode code points currently contained in the character root argument. The
         * contents of the code point root are copied; subsequent modification of
         * the character root does not affect the newly created string.
         *
         * <p>
         * Example:
         * <blockquote> <pre>
         *  IntArray abc = IntArray.of(97, 98, 99); <br>
         *  String str = String(abc); // produce "abc"_S <br>
         * </pre></blockquote>
         *
         * @param  codePoints
         *         Array that is the source of code points
         */
        CORE_EXPLICIT String(const IntArray &codePoints);

        /**
         * Allocates a new String that contains characters from a subarray
         * of the unicode code points root argument. The offset argument is the index
         * of the first code point of the subarray and the count argument specifies
         * the length of the subarray. The contents of the subarray are copied;
         * subsequent modification of the character root does not affect the newly
         * created string.
         *
         * @param  codePoints
         *         Array that is the source of code points
         *
         * @param  offset
         *         The initial offset
         *
         * @param  count
         *         The length
         *
         * @throws  IndexException
         *          If offset is negative, count is negative, or
         *          offset is greater than (value length) - count
         */
        CORE_EXPLICIT String(const IntArray &codePoints, gint offset, gint count);

        /**
         * Allocates a new String so that it represents the sequence of
         * characters currently contained in the character root argument. The
         * contents of the character root are copied; subsequent modification of
         * the character root does not affect the newly created string.
         *
         * <p>
         * Note: The last trailing Null character ('\\u0000') are ignored.
         * Example:
         * <blockquote> <pre>
         *  gchar abc1[] = u"abc"; <br>
         *  gchar abc2[] = {'a', 'b', 'c'</b> ; <br>
         *  gchar abc2[] = {'a', 'b', 'c', '\\0'</b> ; <br>
         *  gchar abc3[] = "abc\0"; <br>
         *  String str1 = abc1; // produce "abc"_S <br>
         *  String str2 = abc2; // produce "abc"_S <br>
         *  String str3 = abc3; // produce "abc"_S <br>
         *  String str4 = abc4; // produce "abc\0"_S <br>
         * </pre></blockquote>
         *
         * @param  value
         *         The initial value of the string
         * @throws IllegalArgumentException
         *          If the specified value is null
         */
        template<class Str, Class<gbool>::OnlyIf<Class<Str>::isString()> = true>
        CORE_IMPLICIT String(Str &&value) {
            gint bpc = {}; // used to determine char type (support values 1, 2, 4)
            glong nbChars = {}; // number of chars in given root (-1 if root is pointer)
            glong address = {}; // memory address of given root
            if((address = (glong) value) == 0) {
                // the null pointer is used as empty String
                return;
            }
            if (Class<Str>::isArray()) {
                // private and static char root
                CORE_ALIAS(CharT, typename Class<Str>::NoArray);
                if((nbChars = sizeof(Str)/(bpc = sizeof(CharT))) > 0 && value[nbChars - 1] == 0) {
                    // remove last NULL char
                    nbChars -= 1;
                }
                if(nbChars == 0) {
                    // empty root
                    return;
                }
            } else {
                CORE_ALIAS(CharT, typename Class<Str>::NoPointer);
                // private and unsizable (dynamic) chars root
                // The start of this string is char at index <offset>
                // The end of this string is char at index <offset + count>
                bpc = sizeof(CharT);
                nbChars = -1;
            }
            wrap(address, bpc, nbChars);
        }

        /**
         * Allocates a new String that contains characters from a subarray
         * of the character root argument. The offset argument is the index
         * of the first character of the subarray and the count argument specifies
         * the length of the subarray. The contents of the subarray are copied;
         * subsequent modification of the character root does not affect the newly
         * created string.
         *
         * @param  value
         *         Array that is the source of characters
         *
         * @param  offset
         *         The initial offset
         *
         * @param  count
         *         The length
         *
         * @throws  IndexException
         *          If offset is negative, count is negative, or
         *          offset is greater than (value length) - count
         */
        template<class Str, Class<gbool>::OnlyIf<Class<Str>::isString()> = true>
        CORE_IMPLICIT String(Str &&value, gint offset, gint count) {
            gint bpc = {}; // used to determine char type (support values 1, 2, 4)
            glong nbChars = {}; // number of chars in given root (-1 if root is pointer)
            glong address = {}; // memory address of given root
            if((address = (glong) value) == 0) {
                // the null pointer is used as empty String
                return;
            }
            if (Class<Str>::isArray()) {
                // private and static char root
                CORE_ALIAS(CharT, typename Class<Str>::NoArray);
                if((nbChars = sizeof(Str)/(bpc = sizeof(CharT))) > 0 && value[nbChars - 1] == 0) {
                    // remove last NULL char (1 char = bpc)
                    nbChars -= 1;
                }
                if(nbChars == 0) {
                    // empty root
                    return;
                }
            } else {
                CORE_ALIAS(CharT, typename Class<Str>::NoPointer);
                // private and unsizable (dynamic) chars root
                // The start of this string is char at index <offset>
                // The end of this string is char at index <offset + count>
                bpc = sizeof(CharT);
                nbChars = -1;
            }
            wrap(address, bpc, nbChars, offset, count);
        }

        /**
         * Set with specified String object, this String content
         *
         * @param str
         *          A String
         */
        String &operator=(const String &str);

        /**
         * Exchange with specified String object, this String content
         *
         * @param str
         *          A String
         */
        String &operator=(String &&str) CORE_NOTHROW;

        /**
         * Returns the length of this string.
         * The length is equal to the number of Unicode code units in the string.
         */
        gint length() const override;

        /**
         * Returns true if, and only if, length() is 0.
         */
        gbool isEmpty() const override;

        /**
         * Returns the char value at the specified index. An index ranges from 0 to length() - 1.
         * The first char value of the sequence is at index 0, the next at index 1,
         * and so on, as for root indexing.
         *
         * <p>
         * If the char value specified by the index is a surrogate, the surrogate value is returned.
         *
         * @param index
         *          The index of the char value.
         * @throws IndexException
         *          If the index argument is negative or not less than the length of this
         *             string.
         */
        gchar charAt(gint index) const;

        /**
         * Returns the character (Unicode code point) at the specified index.
         * The index refers to char values (Unicode code units) and ranges from 0
         * to length() - 1.
         *
         * <p>
         * If the char value specified at the given index is in the high-surrogate range,
         * the following index is less than the length of this String, and the char value
         * at the following index is in the low-surrogate range, then the supplementary code
         * point corresponding to this surrogate pair is returned. Otherwise,
         * the char value at the given index is returned.
         *
         * @param index
         *          The index to the char values
         * @throws IndexException
         *          if the index argument is negative or not less than the length of this string.
         */
        gint codePointAt(gint index) const;

        /**
         * Compares this string to the specified object. The result is true if and only if the argument
         * is a  String object that represents the same sequence of characters as this object.
         *
         * <p>
         * For finer-grained String comparison, refer to core::Collator.
         *
         * @param  object
         *         The object to compare this String against
         *
         * @see  compareTo(String)
         * @see  equalsIgnoreCase(String)
         */
        gbool equals(const Object &object) const override;
        gbool equals(const String &str) const;

        /**
         * Compares this String to another String, ignoring case considerations.
         * Two strings are considered equal ignoring case if they are of the same
         * length and corresponding Unicode code points in the two strings are equal
         * ignoring case.
         *
         * <p>
         * Two Unicode code points are considered the same ignoring case if at least one
         * of the following is true:
         * <ul>
         *   <li> The two Unicode code points are the same (as compared by the == operator)
         *   <li> Calling Character::toLowerCase(Character::toUpperCase(gint))
         *        on each Unicode code point produces the same result
         * </ul>
         *
         * <p>
         * Note that this method does not take locale into account, and
         * will result in unsatisfactory results for certain locales.  The
         * core::Collator class provides locale-sensitive comparison.
         *
         * @param  str
         *         The String to compare this String against
         *
         * @see  equals(Object)
         * @see  codePoints()
         */
        gbool equalsIgnoreCase(const String &str) const;

        /**
         * Compares two strings lexicographically. The comparison is based on the Unicode value
         * of each character in the strings. The character sequence represented by this String
         * object is compared lexicographically to the character sequence represented by the argument
         * string. The result is a negative integer if this String object lexicographically precedes
         * the argument string. The result is a positive integer if this String object lexicographically
         * follows the argument string. The result is zero if the strings are equal; compareTo returns 0
         * exactly when the equals(Object) method would return true.
         *
         * <p>
         * This is the definition of lexicographic ordering. If two strings are different, then either they
         * have different characters at some index that is a valid index for both strings, or their lengths
         * are different, or both. If they have different characters at one or more index positions, let k
         * be the smallest such index; then the string whose character at position k has the smaller value,
         * as determined by using the &lt; operator, lexicographically precedes the other string. In this case,
         * compareTo returns the difference of the two character values at position k in the two string -- that is,
         * the value:
         * <blockquote><pre>
         * this.charAt(k) - str.charAt(k)
         * </pre></blockquote>
         * If there is no index position at which they differ, then the shorter string lexicographically precedes the
         * longer string. In this case, compareTo returns the difference of the lengths of the strings -- that is,
         * the value:
         * <blockquote><pre>
         * this.length() - str.length()
         * </pre></blockquote>
         *
         * <p>For finer-grained String comparison, refer to core::Collator.
         *
         * @param str
         *          The String to be compared.
         */
        gint compareTo(const String& other) const override;

        /**
         * Compares two strings lexicographically, ignoring case differences. This method returns
         * an integer whose sign is that of calling compareTo with case folded versions of the strings
         * where case differences have been eliminated by calling Character.toLowerCase(Character.toUpperCase(gint))
         * on each Unicode code point.
         *
         * <p>
         * Note that this method does <em>not</em> take locale into account, and will result in an unsatisfactory
         * ordering for certain locales. The core::Collator class provides locale-sensitive comparison.
         *
         * @param str
         *         The String to be compared.
         * @see     core::Collator
         * @see     codePoints()
         */
        gint compareToIgnoreCase(const String &str) const;

        /**
         * Tests if the substring of this string beginning at the specified index starts with the specified prefix.
         *
         * @param str
         *          The prefix String.
         * @param offset
         *          Where to begin looking in this string.
         */
        gbool startsWith(const String &str, gint offset) const;

        /**
         * Tests if this string starts with the specified prefix.
         *
         * @param str
         *          The prefix String.
         */
        gbool startsWith(const String &str) const;

        /**
         * Tests if this string ends with the specified suffix.
         *
         * @param str
         *          The suffix.
         */
        gbool endsWith(const String &str) const;

        /**
         * Returns a hash code for this string. The hash code for a String object is computed as
         * <blockquote><pre>
         * s[0]*31^(n-1) + s[1]*31^(n-2) + ... + s[n-1]
         * </pre></blockquote>
         * using gint arithmetic, where s[i] is the <i>i</i>th character of the string, n is the length of
         * the string, and ^ indicates exponentiation. (The hash value of the empty string is zero.)
         */
        gint hash() const override;

        /**
         * Returns the index within this string of the first occurrence of the specified character.
         * If a character with value ch occurs in the character sequence represented by this String
         * object, then the index (in Unicode code units) of the first such occurrence is returned.
         * For values of ch in the range from 0 to 0xFFFF (inclusive), this is the smallest value k
         * such that:
         * <blockquote><pre>
         * this.charAt(k) == ch
         * </pre></blockquote>
         * is true. For other values of ch, it is the smallest value k such that:
         * <blockquote><pre>
         * this.codePointAt(k) == ch
         * </pre></blockquote>
         * is true. In either case, if no such character occurs in this string, then -1 is returned.
         *
         * @param ch
         *         A character (Unicode code point).
         */
        gint indexOf(gint ch) const;

        /**
         * Returns the index within this string of the first occurrence of the specified character,
         * starting the search at the specified index.
         *
         * <p>
         * If a character with value ch occurs in the character sequence represented by this String
         * object at an index no smaller than startIndex, then the index of the first such occurrence is
         * returned. For values of ch in the range from 0 to 0xFFFF (inclusive), this is the smallest value k
         * such that:
         * <blockquote><pre>
         * (this.charAt(k) == ch) && (k &gt;= startIndex)
         * </pre></blockquote>
         * is true. For other values of ch, it is the smallest value k such that:
         * <blockquote><pre>
         * (this.codePointAt(k) == ch) && (k &gt;= startIndex)
         * </pre></blockquote>
         * is true. In either case, if no such character occurs in this string at or after position startIndex,
         * then -1 is returned.
         *
         * <p>
         * There is no restriction on the value of startIndex. If it is negative, it has the same effect as if it
         * were zero: this entire string may be searched. If it is greater than the length of this string, it has
         * the same effect as if it were equal to the length of this string: -1 is returned.
         *
         * <p>
         * All indices are specified in char values (Unicode code units).
         *
         * @param   ch
         *            A character (Unicode code point).
         * @param   startIndex
         *             The index to start the search from.
         */
        gint indexOf(gint ch, gint startIndex) const;

        /**
         * Returns the index within this string of the last occurrence of the specified character.
         * For values of ch in the range from 0 to 0xFFFF (inclusive), the index (in Unicode code
         * units) returned is the largest value k such that:
         * <blockquote><pre>
         * this.charAt(k) == ch
         * </pre></blockquote>
         * is true. For other values of ch, it is the largest value k such that:
         * <blockquote><pre>
         * this.codePointAt(k) == ch
         * </pre></blockquote>
         * is true.  In either case, if no such character occurs in this string, then -1 is returned.  The
         * String is searched backwards starting at the last character.
         *
         * @param ch
         *          A character (Unicode code point).
         */
        gint lastIndexOf(gint ch) const;

        /**
         * Returns the index within this string of the last occurrence of the specified character,
         * searching backward starting at the specified index. For values of ch in the range
         * from 0 to 0xFFFF (inclusive), the index returned is the largest value k such that:
         * <blockquote><pre>
         * (this.charAt(k) == ch) && (k &lt;= startIndex)
         * </pre></blockquote>
         * is true. For other values of ch, it is the largest value k such that:
         * <blockquote><pre>
         * (this.codePointAt(k) == ch) && (k &lt;= startIndex)
         * </pre></blockquote>
         * is true. In either case, if no such character occurs in this string at or before position
         * startIndex, then -1 is returned.
         *
         * <p>
         * All indices are specified in char values (Unicode code units).
         *
         * @param ch
         *          a character (Unicode code point).
         * @param startIndex
         *          the index to start the search from. There is no
         *          restriction on the value of startIndex. If it is
         *          greater than or equal to the length of this string, it has
         *          the same effect as if it were equal to one less than the
         *          length of this string: this entire string may be searched.
         *          If it is negative, it has the same effect as if it were -1:
         *          -1 is returned.
         */
        gint lastIndexOf(gint ch, gint startIndex) const;

        /**
         * Returns the index within this string of the first occurrence of the
         * specified substring.
         *
         * <p>The returned index is the smallest value k for which:
         * <pre>
         * this.startsWith(str, k)
         * </pre>
         * If no such value of k exists, then -1 is returned.
         *
         * @param   str   the substring to search for.
         */
        gint indexOf(const String &str) const;

        /**
         * Returns the index within this string of the first occurrence of the
         * specified substring, starting at the specified index.
         *
         * <p>The returned index is the smallest value k for which:
         * <pre>
         *     k >= Math.min(fromIndex, this.length()) &&
         *                   this.startsWith(str, k)
         * </pre>
         * If no such value of k exists, then -1 is returned.
         *
         * @param   str         the substring to search for.
         * @param   fromIndex   the index from which to start the search.
         */
        gint indexOf(const String &str, gint startIndex) const;

        /**
         * Returns the index within this string of the last occurrence of the
         * specified substring.  The last occurrence of the empty string ""
         * is considered to occur at the index value this.length().
         *
         * <p>
         * The returned index is the largest value k for which:
         * <pre>
         * this.startsWith(str, k)
         * </pre>
         * If no such value of k exists, then -1 is returned.
         *
         * @param   str
         *            The substring to search for.
         */
        gint lastIndexOf(const String &str) const;

        /**
         * Returns the index within this string of the last occurrence of the
         * specified substring, searching backward starting at the specified index.
         *
         * <p>The returned index is the largest value k for which:
         * <pre>
         *     k <= Math.min(fromIndex, this.length()) &&
         *                   this.startsWith(str, k)
         * </pre>
         * If no such value of k exists, then -1 is returned.
         *
         * @param   str         the substring to search for.
         * @param   fromIndex   the index to start the search from.
         */
        gint lastIndexOf(const String &str, gint startIndex) const;

        /**
         * Returns a string that is a substring of this string. The
         * substring begins with the character at the specified index and
         * extends to the end of this string. <p>
         * Examples:
         * <blockquote><pre>
         * "unhappy".substring(2) returns "happy"
         * "Harbison".substring(3) returns "bison"
         * "emptiness".substring(9) returns "" (an empty string)
         * </pre></blockquote>
         *
         * @param      startIndex   the beginning index, inclusive.
         *
         * @throws     IndexException  if
         *             startIndex is negative or larger than the
         *             length of this String object.
         */
        String subString(gint startIndex) const;

        /**
         * Returns a string that is a substring of this string. The
         * substring begins at the specified startIndex and
         * extends to the character at index endIndex - 1.
         * Thus the length of the substring is endIndex-startIndex.
         *
         * <p>
         * Examples:
         * <blockquote><pre>
         * "hamburger"_S.substring(4, 8) returns "urge"
         * "smiles"_S.substring(1, 5) returns "mile"
         * </pre></blockquote>
         *
         * @param      startIndex   the beginning index, inclusive.
         * @param      endIndex     the ending index, exclusive.
         *
         * @throws     IndexException  if the
         *             startIndex is negative, or
         *             endIndex is larger than the length of
         *             this String object, or
         *             startIndex is larger than
         *             endIndex.
         */
        String subString(gint startIndex, gint endIndex) const;

        /**
         * Returns a character sequence that is a subsequence of this sequence.
         *
         * <p> An invocation of this method of the form
         *
         * <blockquote><pre>
         * str.subSequence(begin,&nbsp;end)</pre></blockquote>
         *
         * behaves in exactly the same way as the invocation
         *
         * <blockquote><pre>
         * str.substring(begin,&nbsp;end)</pre></blockquote>
         *
         * @apiNote
         * This method is defined so that the <b> String</b>  class can implement
         * the <b style="color:orange;"> CharSequence</b>  interface.
         *
         * @param   beginIndex   the begin index, inclusive.
         * @param   endIndex     the end index, exclusive.
         * @return  the specified subsequence.
         *
         * @throws  IndexException
         *          if <b> beginIndex</b>  or <b> endIndex</b>  is negative,
         *          if <b> endIndex</b>  is greater than <b> length()</b> ,
         *          or if <b> beginIndex</b>  is greater than <b> endIndex</b>
         */
        CharSequence &subSequence(gint start, gint end) const override;

        /**
         * Concatenates the specified string to the end of this string.
         * <p>
         * If the length of the argument string is 0, then this
         * String object is returned. Otherwise, a
         * String object is returned that represents a character
         * sequence that is the concatenation of the character sequence
         * represented by this String object and the character
         * sequence represented by the argument string.<p>
         * Examples:
         * <blockquote><pre>
         * "cares".concat("s") returns "caress"
         * "to".concat("get").concat("her") returns "together"
         * </pre></blockquote>
         *
         * @param   str   the String that is concatenated to the end
         *                of this String.
         */
        String concat(const String &str) const;

        /**
         * Returns a string resulting from replacing all occurrences of
         * oldChar in this string with newChar.
         * <p>
         * If the character oldChar does not occur in the
         * character sequence represented by this String object,
         * then a reference to this String object is returned.
         * Otherwise, a String object is returned that
         * represents a character sequence identical to the character sequence
         * represented by this String object, except that every
         * occurrence of oldChar is replaced by an occurrence
         * of newChar.
         * <p>
         * Examples:
         * <blockquote><pre>
         * @code
         * "mesquite in your cellar".replace('e', 'o')
         *         returns "mosquito in your collar"
         * "the war of baronets".replace('r', 'y')
         *         returns "the way of bayonets"
         * "sparring with a purple porpoise".replace('p', 't')
         *         returns "starring with a turtle tortoise"
         * "JonL".replace('q', 'x') returns "JonL" (no change)
         * @endcode </pre></blockquote>
         *
         * @param   oldChar   the old character.
         * @param   newChar   the new character.
         *
         */
        String replace(gchar oldChar, gchar newChar) const;

        /**
         * Replaces each substring of this string that matches the given <a
         * href="../util/str/Pattern.html#sum">regular expression</a> with the
         * given replacement.
         *
         * <p> An invocation of this method of the form
         * <i>str</i>.replaceAll(<i>str</i>, <i>repl</i>)
         * yields exactly the same result as the expression
         *
         * <blockquote>
         * <code>
         *  str.pattern.
         * str.pattern.compile(String) compile(<i>str</i>).
         * str.pattern.matcher(CharSequence) matcher(<i>str</i>).
         * str.Matcher#replaceAll(String) replaceAll(<i>repl</i>)
         * </code>
         * </blockquote>
         *
         *<p>
         * Note that backslashes (\) and dollar signs ($) in the
         * replacement string may cause the results to be different than if it were
         * being treated as a literal replacement string; see
         *  str.Matcher#replaceAll Matcher.replaceAll.
         * Use  str.Matcher#quoteReplacement to suppress the special
         * meaning of these characters, if desired.
         *
         * @param   str
         *          the regular expression to which this string is to be matched
         * @param   replacement
         *          the string to be substituted for each match
         */
        String replace(const String &str, const String &replacement) const;

        /**
         * Converts all of the characters in this String to lower
         * case using the rules of the default locale. This method is equivalent to
         * toLowerCase(Locale.getDefault()).
         * <p>
         * <b>Note:</b> This method is locale sensitive, and may produce unexpected
         * results if used for strings that are intended to be interpreted locale
         * independently.
         * Examples are programming language identifiers, protocol keys, and HTML
         * tags.
         * For INSTANCE, "TITLE".toLowerCase() in a Turkish locale
         * returns "t\\u0131tle", where '\\u0131' is the
         * LATIN SMALL LETTER DOTLESS I character.
         * To obtain correct results for locale insensitive strings, use
         * toLowerCase(Locale.ROOT).
         */
        String toLowerCase() const;

        /**
         * Converts all of the characters in this String to upper
         * case using the rules of the default locale. This method is equivalent to
         * toUpperCase(Locale.getDefault()).
         * <p>
         * <b>Note:</b> This method is locale sensitive, and may produce unexpected
         * results if used for strings that are intended to be interpreted locale
         * independently.
         * Examples are programming language identifiers, protocol keys, and HTML
         * tags.
         * For INSTANCE, "title".toUpperCase() in a Turkish locale
         * returns "T\\u0130TLE", where '\\u0130' is the
         * LATIN CAPITAL LETTER I WITH DOT ABOVE character.
         * To obtain correct results for locale insensitive strings, use
         * toUpperCase(Locale.ROOT).
         */
        String toUpperCase() const;

        String toTitleCase() const;

        String toReverseCase() const;

        String toPascalCase() const;

        String toCamelCase() const;

        /**
         * Returns a string whose value is this string, with all leading
         * and trailing plain Character#isWhitespace(gint) white diskSpace
         * removed.
         * <p>
         * If this String object represents an empty string,
         * or if all code points in this string are
         * plain Character#isWhitespace(gint) white diskSpace, then an empty string
         * is returned.
         * <p>
         * Otherwise, returns a substring of this string beginning with the first
         * code point that is not a plain Character#isWhitespace(gint) white diskSpace
         * up to and including the last code point that is not a
         * plain Character#isWhitespace(gint) white diskSpace.
         * <p>
         * This method may be used to strip
         * plain Character#isWhitespace(gint) white diskSpace from
         * the beginning and end of a string.
         */
        String strip() const;

        /**
         * Returns a string whose value is this string, with all leading
         * plain Character#isWhitespace(gint) white diskSpace removed.
         * <p>
         * If this String object represents an empty string,
         * or if all code points in this string are
         * plain Character#isWhitespace(gint) white diskSpace, then an empty string
         * is returned.
         * <p>
         * Otherwise, returns a substring of this string beginning with the first
         * code point that is not a plain Character#isWhitespace(gint) white diskSpace
         * up to and including the last code point of this string.
         * <p>
         * This method may be used to trim
         * plain Character#isWhitespace(gint) white diskSpace from
         * the beginning of a string.
         */
        String stripLeading() const;

        /**
         * Returns a string whose value is this string, with all trailing
         * plain Character#isWhitespace(gint) white diskSpace removed.
         * <p>
         * If this String object represents an empty string,
         * or if all characters in this string are
         * plain Character#isWhitespace(gint) white diskSpace, then an empty string
         * is returned.
         * <p>
         * Otherwise, returns a substring of this string beginning with the first
         * code point of this string up to and including the last code point
         * that is not a plain Character#isWhitespace(gint) white diskSpace.
         * <p>
         * This method may be used to trim
         * plain Character#isWhitespace(gint) white diskSpace from
         * the end of a string.
         */
        String stripTrailing() const;

        /**
         * Returns true if the string is empty or contains only
         * plain Character#isWhitespace(gint) white diskSpace codepoints,
         * otherwise false.
         */
        gbool isBlank() const;

        /**
         * Returns a string whose value is this string, with escape sequences
         * translated as if in a string literal.
         * <p>
         * Escape sequences are translated as follows;
         * <table class="striped">
         *   <caption style="display:none">Translation</caption>
         *   <thead>
         *   <tr>
         *     <th scope="col">Escape</th>
         *     <th scope="col">Name</th>
         *     <th scope="col">Translation</th>
         *   </tr>
         *   </thead>
         *   <tbody>
         *   <tr>
         *     <th scope="row">\\b</th>
         *     <td>backspace</td>
         *     <td>U+0008</td>
         *   </tr>
         *   <tr>
         *     <th scope="row">\\t</th>
         *     <td>horizontal tab</td>
         *     <td>U+0009</td>
         *   </tr>
         *   <tr>
         *     <th scope="row">\\n</th>
         *     <td>line feed</td>
         *     <td>U+000A</td>
         *   </tr>
         *   <tr>
         *     <th scope="row">\\f</th>
         *     <td>form feed</td>
         *     <td>U+000C</td>
         *   </tr>
         *   <tr>
         *     <th scope="row">\\r</th>
         *     <td>carriage return</td>
         *     <td>U+000D</td>
         *   </tr>
         *   <tr>
         *     <th scope="row">\\s</th>
         *     <td>diskSpace</td>
         *     <td>U+0020</td>
         *   </tr>
         *   <tr>
         *     <th scope="row">\\"</th>
         *     <td>double quote</td>
         *     <td>U+0022</td>
         *   </tr>
         *   <tr>
         *     <th scope="row">\\'</th>
         *     <td>single quote</td>
         *     <td>U+0027</td>
         *   </tr>
         *   <tr>
         *     <th scope="row">\\\\</th>
         *     <td>backslash</td>
         *     <td>U+005C</td>
         *   </tr>
         *   <tr>
         *     <th scope="row">\\0 - \\377</th>
         *     <td>octal escape</td>
         *     <td>code point equivalents</td>
         *   </tr>
         *   <tr>
         *     <th scope="row">\\<line-terminator></th>
         *     <td>continuation</td>
         *     <td>discard</td>
         *   </tr>
         *   </tbody>
         * </table>
         *
         * @implNote
         * This method does <em>not</em> translate Unicode escapes such as "\\u2022".
         *
         * @throws IllegalArgumentException when an escape sequence is malformed.
         */
        String translateEscape() const;

        /**
         * This object (which is already a string!) is itself returned.
         */
        String toString() const override;

        /**
         * Returns a string whose value is the concatenation of this
         * string repeated count times.
         * <p>
         * If this string is empty or count is zero then the empty
         * string is returned.
         *
         * @param   count number of times to repeat
         *
         * @throws IllegalArgumentException if the count is
         *          negative.
         */
        String repeat(gint count) const;

        /**
         * Tell if this character sequence contains only
         * ASCII characters
         */
        gbool isASCII() const;

        /**
         * Tell if this character sequence contains only
         * Basic Latin characters
         */
        gbool isLatin1() const;

        /**
         * Tell if this character sequence contains only
         * characters in lowercase
         */
        gbool isLowerCase() const;

        /**
         * Tell if this character sequence contains only
         * characters in uppercase
         */
        gbool isUpperCase() const;

        /**
         * Tell if this character sequence contains only
         * characters in titlecase
         */
        gbool isTitleCase() const;

        /**
         * Return the number of occurrence of specified String
         * on this String object.
         *
         * @param str
         *         The substring to search
         */
        gint count(const String &str) const;

        /**
         * Return sharable copy of this String object
         *
         * @throws MemoryError
         *          If memory allocation failed
         */
        Object &clone() const override;

        /**
         * Copies characters from this string into the destination character
         * root.
         * <p>
         * The first character to be copied is at index srcBegin;
         * the last character to be copied is at index srcEnd-1
         * (thus the total number of characters to be copied is
         * srcEnd-srcBegin). The characters are copied into the
         * subarray of dst starting at index dstBegin
         * and ending at index: dstBegin + (srcEnd-srcBegin) - 1
         *
         * @param  srcBegin   index of the first character in the string
         *                        to copy.
         * @param  srcEnd     index after the last character in the string
         *                        to copy.
         * @param  dst        the destination root.
         * @param  dstBegin   the start offset in the destination root.
         * @throws IndexException If any of the following is true:
         *                         <ul>
         *                          <li>srcBegin is negative.
         *                          <li>srcBegin is greater than srcEnd
         *                          <li>srcEnd is greater than the length of this string
         *                          <li>dstBegin is negative
         *                          <li>dstBegin+(srcEnd-srcBegin) is larger than dst.length
         *                         </ul>
         */
        void chars(gint srcBegin, gint srcEnd, CharArray &dst, gint dstBegin) const;

        /**
         * Return in a new root, list of all character of this String
         */
        CharArray chars() const;

        /**
         * Copies code points from this string into the destination gint
         * root.
         * <p>
         * The first code point to be copied is at index srcBegin;
         * the last character to be copied is at index srcEnd-1
         * (thus the total number of characters to be copied is
         * srcEnd-srcBegin). The characters are copied into the
         * subarray of dst starting at index dstBegin
         * and ending at index: dstBegin + (srcEnd-srcBegin) - 1
         *
         * @param  srcBegin   index of the first character in the string
         *                        to copy.
         * @param  srcEnd     index after the last character in the string
         *                        to copy.
         * @param  dst        the destination root.
         * @param  dstBegin   the start offset in the destination root.
         * @throws IndexException If any of the following is true:
         *                         <ul>
         *                          <li>srcBegin is negative.
         *                          <li>srcBegin is greater than srcEnd
         *                          <li>srcEnd is greater than the length of this string
         *                          <li>dstBegin is negative
         *                          <li>dstBegin+(srcEnd-srcBegin) is larger than dst.length
         *                         </ul>
         */
        void codePoints(gint srcBegin, gint srcEnd, IntArray &dst, gint dstBegin) const;

        /**
         * Return in a new root, list of all code points of this String
         */
        IntArray codePoints() const;

        /**
         * Copies characters from this string into the destination byte root. Each
         * byte receives the 8 low-order bits of the corresponding character. The
         * eight high-order bits of each character are not copied and do not
         * participate in the transfer in any way.
         *
         * <p>
         * The first character to be copied is at index srcBegin; the
         * last character to be copied is at index srcEnd-1.  The total
         * number of characters to be copied is srcEnd-srcBegin. The
         * characters, converted to bytes, are copied into the subarray of dst
         * starting at index dstBegin and ending at index: dstBegin + (srcEnd-srcBegin) - 1.
         *
         * \deprecated  This method does not properly convert characters into
         * bytes.  As of the preferred way to do this is via the bytes() method,
         * which uses the default charset (utf-16 BE).
         *
         * @param  srcBegin
         *         Index of the first character in the string to copy
         *
         * @param  srcEnd
         *         Index after the last character in the string to copy
         *
         * @param  dst
         *         The destination root
         *
         * @param  dstBegin
         *         The start offset in the destination root
         *
         * @throws  IndexException
         *          If any of the following is true:
         *          <ul>
         *            <li> srcBegin is negative
         *            <li> srcBegin is greater than srcEnd
         *            <li> srcEnd is greater than the length of this String
         *            <li> dstBegin is negative
         *            <li> dstBegin+(srcEnd-srcBegin) is larger than dst.length
         *          </ul>
         */
        void bytes(gint srcBegin, gint srcEnd, ByteArray &dst, gint dstBegin) const;

        /**
         * Encodes this String into a sequence of bytes using the default charset, storing the result
         * into a new byte root.
         *
         * <p> The behavior of this method when this string cannot be encoded in
         * the default charset is unspecified.  The Charset class should be used when more control
         * over the encoding process is required.
         */
        ByteArray bytes() const;

        /**
         * Return String representation of specified object.
         * @param obj the object
         * @see Object::toString
         */
        static String valueOf(const Object &obj);

        /**
         * Return String representation of specified boolean
         * @param b the boolean value
         */
        static String valueOf(gbool b);

        /**
         * Return String representation of specified character
         * @param c the char value
         */
        static String valueOf(gchar c);

        /**
         * Return String representation of specified character (Unicode code point).
         * <p>
         * Note: If specified code point are invalid this method return "?"
         *
         * @param c the char value
         */
        static String valueOfCodePoint(gint c);

        /**
         * Return String representation of specified integer value
         * @param i the integer value
         * @see Integer.toString
         */
        static String valueOf(gint i);

        /**
         * Return String representation of specified integer value
         * @param l the integer value
         * @see Long.toString
         */
        static String valueOf(glong l);

        /**
         * Return unsigned String representation of specified integer value
         * @param i the integer value
         * @see Integer.toUnsignedString
         */
        static String valueOfUnsigned(gint i);

        /**
         * Return unsigned String representation of specified integer value
         * @param l the integer value
         * @see Long.toUnsignedString
         */
        static String valueOfUnsigned(glong l);

        /**
         * Return String representation of specified float value
         * @param f the float value
         * @see Float.toString
         */
        static String valueOf(gfloat f);

        /**
         * Return String representation of specified float value
         * @param d the float value
         * @see Double.toString
         */
        static String valueOf(gdouble d);

        /**
         * Return itself
         * @see String.toString
         */
        static String valueOf(const String &str);

        friend String operator+(const String &x, const String &y);

        String &operator+=(const String &str);

        ~String() override;
    };

} // core

#endif //CORE23_STRING_H
