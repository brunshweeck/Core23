//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_STRINGBUFFER_H
#define CORE23_STRINGBUFFER_H

#include <core/String.h>

namespace core {

    /**
     * A thread-safe, mutable sequence of characters.
     * A string buffer is like a <b style="color:green;">String</b>, but can be modified. At any
     * point in time it contains some particular sequence of characters, but
     * the length and content of the sequence can be changed through certain
     * method calls.
     * <p>
     * String buffers are safe for use by multiple threads. The methods
     * are synchronized where necessary so that all the operations on any
     * particular INSTANCE behave as if they occur in some serial order
     * that is consistent with the order of the method calls made by each of
     * the individual threads involved.
     * <p>
     * The principal operations on a <b>StringBuffer</b> are the
     * <b>append</b> and <b>insert</b> methods, which are
     * overloaded so as to accept root of any type. Each effectively
     * converts a given datum to a string and then appends or inserts the
     * characters of that string to the string buffer. The
     * <b>append</b> method always adds these characters at the end
     * of the buffer; the <b>insert</b> method adds the characters at
     * a specified point.
     * <p>
     * For example, if <b>z</b> refers to a string buffer object
     * whose current contents are <b>"start"</b>, then
     * the method call <b>z.append("le")</b> would cause the string
     * buffer to contain <b>"startle"</b>, whereas
     * <b>z.insert(4, "le")</b> would alter the string buffer to
     * contain <b>"starlet"</b>.
     * <p>
     * In general, if sb refers to an INSTANCE of a <b>StringBuffer</b>,
     * then <b>sb.append(x)</b> has the same effect as
     * <b>sb.insert(sb.length(), x)</b>.
     * <p>
     * Whenever an operation occurs involving a source sequence (such as
     * appending or inserting from a source sequence), this class synchronizes
     * only on the string buffer performing the operation, not on the source.
     * Note that while <b>StringBuffer</b> is designed to be safe to use
     * concurrently from multiple threads, if the constructor or the
     * <b>append</b> or <b>insert</b> operation is passed a source sequence
     * that is shared across threads, the calling code must ensure
     * that the operation has a consistent and unchanging view of the source
     * sequence for the duration of the operation.
     * This could be satisfied by the caller holding a lock during the
     * operation's call, by using an immutable source sequence, or by not
     * sharing the source sequence across threads.
     * <p>
     * Every string buffer has a capacity. As glong as the length of the
     * character sequence contained in the string buffer does not exceed
     * the capacity, it is not necessary to allocate a new internal
     * buffer root. If the internal buffer overflows, it is
     * automatically made larger.
     *
     * @author   Brunshweeck Tazeussong
     * @see     core.String
     */
    class StringBuffer CORE_FINAL : public Object, public Comparable<StringBuffer>, public CharSequence {
    private:
        /**
         * gbyte[*]
         */
        CORE_ALIAS(STORAGE, typename Class<gbyte>::Ptr);

        /**
         * The value used for character storage.
         */
        STORAGE value = {};

        /**
         * The character count of this String
         */
        gint len = {};

        /**
         * The capacity of this Buffer.
         */
        gint cap = {};

        /**
         * The default initial capacity
         */
        static CORE_FAST gint DEFAULT_CAPACITY = 1 << 4;

        /**
         * Returns a capacity at least as large as the given minimum capacity.
         * Returns the current capacity increased by the current length + 2 if
         * that suffices.
         * Will not return a capacity greater than SOFT_MAX_ARRAY_LENGTH
         * unless the given minimum capacity is greater than that.
         *
         * @param  minCapacity the desired minimum capacity
         * @throws MemoryError if minCapacity is less than zero or
         *         greater than SOFT_MAX_ARRAY_LENGTH
         */
        gint newCapacity(gint minCapacity) const;

        /**
         * Shift position of all character after index offset from count positions
         */
        void shift(gint offset, gint count);

    public:
        /**
         * Construct new StringBuffer with no characters in it and an initial
         * capacity of 16 characters.
         */
        CORE_IMPLICIT StringBuffer() : StringBuffer(DEFAULT_CAPACITY) {}

        /**
         * Construct new StringBuffer with no characters in it and the\
         * specified initial capacity.
         *
         * @param capacity
         *          The initial capacity
         * @throws IllegalArgumentException
         *          If the initial capacity is negative
         */
        CORE_EXPLICIT StringBuffer(gint capacity);

        /**
         * Initialize newly created StringBuffer with content of specified
         * String. The initial capacity is length of String argument plus
         * default initial capacity (16).
         *
         * @param str
         *          The initial content of StringBuffer.
         */
        CORE_EXPLICIT StringBuffer(const String &str);

        /**
         * Initialize newly created StringBuffer with content of specified
         * StringBuffer. The initial capacity is length of StringBuffer argument.
         *
         * @param sb
         *         The initial content of StringBuffer.
         */
        StringBuffer(const StringBuffer &sb);

        /**
         * Transfers to the newly created StringBuffer the content and all properties
         * of StringBuffer argument.
         *
         * <p>
         * After this calling the StringBuffer argument will be empty, with
         * capacity zero (0)
         *
         * @param sb
         *         The initial content of StringBuffer.
         */
        StringBuffer(StringBuffer &&sb) CORE_NOTHROW;

        /**
         * Set content of this StringBuffer with content of specified
         * StringBuffer. After this calling:
         * <ul>
         *  <li> If capacity of this StringBuffer great than or equals to length of
         *      StringBuffer argument, The capacity not change.
         *  <li> If capacity of this StringBuffer less than length of StringBuffer
         *      argument, the capacity will be updated to length of StringBuffer
         *      argument.
         * </ul>
         *
         * @param sb
         *         The new content of StringBuffer.
         */
        StringBuffer &operator=(const StringBuffer &sb);

        /**
         * Exchange content and all properties of this StringBuffer with
         * StringBuffer argument.
         *
         * @param sb
         *         The new content of StringBuffer.
         */
        StringBuffer &operator=(StringBuffer &&sb) CORE_NOTHROW;

        /**
         * Return the current number of character in this StringBuffer.
         */
        gint length() const;

        /**
         * Return the current capacity of this StringBuffer.
         */
        gint capacity() const;

        /**
         * Set the length of this character sequence. The sequence is changed
         * to the new character sequence whose the current length is a specified
         * argument.
         *
         * <p>
         * For every non-negative index k less than new length, the character at index k
         * in the new sequence is same to the character at index k in
         * the old sequence iff k less than the length of old sequence. Otherwise it is
         * null character (U+0000)
         *
         * @param newLength
         *         The new length
         * @throws IllegalArgumentException
         *             If the new length is negative
         */
        void resize(gint newLength);

        /**
         * Reduce storage used for the character sequence. If the buffer is large than necessary
         * to hold its current sequence of characters , then it may be resized to become more diskSpace efficient.
         * Calling this method may, but is not required to, affect the value returned by a subsequent call to
         * the capacity method.
         */
        void truncate();

        /**
         * Return the char value in the sequence at specified index.
         * The first char value is at index 0, the next at index 1, and so on,
         * as in root indexing.
         * <p>
         * The index argument must be greater than or equal to 0, and less than
         * the length of this sequence.
         *
         * <p>
         * If the gchar value specified by the index is a surrogate, the surrogate
         * value is returned.
         *
         * @param index
         *             The index of desired char value
         * @throws IndexException
         *             If index is negative or greater than or equal to current length
         */
        gchar charAt(gint index) const;

        /**
         * Return the character (unicode code point) in the sequence at specified index.
         * The index refers to char values (Unicode code units) and ranges from 0 to length - 1.
         *
         * <p>
         * If the gchar value specified at the given index
         * is in the high-surrogate range, the following index is less
         * than the length of this sequence, and the gchar value at the
         * following index is in the low-surrogate range, then the supplementary
         * code point corresponding to this surrogate pair is returned. Otherwise,
         * the gchar value at the given index is returned.
         *
         * @param index
         *             The index of desired character (unicode code point).
         * @throws IndexException
         *             If index is negative or greater than or equal to current length
         */
        gint codePointAt(gint index) const;

        /**
         * Set the character at specified index to specified char value. This
         * sequence is altered to represent a new character sequence that is
         * identical to the old character sequence, except that it contains the
         * character ch at position index.
         * <p>
         * The index argument must be greater than or equal to 0, and less than
         * the length of this sequence.
         *
         * @param index
         *          The index of the character to modify.
         * @param ch
         *         The new character
         * @throws IndexException
         *          If index is negative or greater than or equal to current length
         */
        void set(gint index, gchar ch);

        /**
         * Append String representation of the Object argument
         *
         * @param obj
         *         The Object to append
         */
        StringBuffer &append(const Object &obj);

        /**
         * Append String argument to this character sequence.
         *
         * <p>
         * The characters of the String argument are appended, in
         * order, increasing the length of this sequence by the length of the
         * argument.
         *
         * <p>
         * Let n be the length of this character sequence just prior to
         * execution of the append method. Then the character at index k in the new character
         * sequence is equal to the character at index k in the old character sequence, if k is less
         * than n; otherwise, it is equal to the character at index k-n in the argument str.
         *
         * @param str
         *         The String to append
         */
        StringBuffer &append(const String &str);

        template<class Str, Class<gbool>::Iff<Class<Str>::isString()> = true>
        StringBuffer &append(Str &&str) {
            return append((String) str);
        }

        /**
         * Append StringBuffer argument to this character sequence.
         *
         * <p>
         * The characters of the StringBuffer argument are appended, in
         * order, increasing the length of this sequence by the length of the
         * argument.
         *
         * <p>
         * Let n be the length of this character sequence just prior to
         * execution of the append method. Then the character at index k in the new character
         * sequence is equal to the character at index k in the old character sequence, if k is less
         * than n; otherwise, it is equal to the character at index k-n in the argument sb.
         *
         * @param sb
         *         The StringBuffer to append
         */
        StringBuffer &append(const StringBuffer &sb);

        /**
         * Append String representation of boolean argument to this character sequence.
         *
         * @param b
         *         The boolean to append
         */
        StringBuffer &append(gbool b);

        /**
         * Append String representation of char value argument to this character sequence.
         *
         * @param ch
         *         The char value to append
         */
        StringBuffer &append(gchar ch);

        template<class Chr, Class<gbool>::Iff<Class<Chr>::isCharacter()> = true>
        StringBuffer &append(Chr &&ch) {
            return append((gchar) ch);
        }

        /**
         * Append String representation of character (unicode code point) argument to this character
         * sequence.
         *
         * @param codePoint
         *         The character (unicode code point) to append
         */
        StringBuffer &appendCodePoint(gint codePoint);

        /**
         * Append String representation of gint value argument to this character sequence.
         *
         * @param i
         *         The gint value to append
         */
        StringBuffer &append(gint i);

        /**
         * Append String representation of glong value argument to this character sequence.
         *
         * @param l
         *         The glong value to append
         */
        StringBuffer &append(glong l);

        /**
         * Append unsigned String representation of gint value argument to this character sequence.
         *
         * @param i
         *         The gint value to append
         */
        StringBuffer &appendUnsigned(gint i);

        /**
         * Append unsigned String representation of glong argument to this character sequence.
         *
         * @param l
         *         The glong value to append
         */
        StringBuffer &appendUnsigned(glong l);

        /**
         * Append String representation of float value argument to this character sequence.
         *
         * @param f
         *         The float value to append
         */
        StringBuffer &append(gfloat f);

        /**
         * Append String representation of double value argument to this character sequence.
         *
         * @param d
         *         The double value to append
         */
        StringBuffer &append(gdouble d);

        /**
         * Append String representation of the root argument to this sequence.
         *
         * <p>
         * The characters of the root argument are appended, in order, to
         * the contents of this sequence. The length of this sequence
         * increases by the length of the argument.
         *
         * @param chars
         *          The characters to be appended
         */
        StringBuffer &append(const CharArray &chars);

        /**
         * Append String representation of the subarray argument to this sequence.
         *
         * <p>
         * Characters of the char root , starting at index offset, are appended,
         * in order, to the contents of this sequence. The length of this sequence increases
         * by the value of length.
         *
         * @param chars
         *          The characters to be appended
         * @param offset
         *          The index of the first char to append.
         * @param length
         *          the number of characters to append.
         */
        StringBuffer &append(const CharArray &chars, gint offset, gint length);

        /**
         * Appends a subsequence of the specified String to this sequence.
         *
         * <p>
         * Characters of the argument str, starting at index startIndex, are appended,
         * in order, to the contents of this sequence up to the (exclusive) index endIndex.
         * The length of this sequence is increased by the value of endIndex - startIndex.
         *
         * @param str the sequence to append.
         * @param startIndex   the starting index of the subsequence to be appended.
         * @param endIndex     the end index of the subsequence to be appended.
         * @throws IndexException if
         *             startIndex is negative, or
         *             startIndex is greater than endIndex or
         *             endIndex is greater than str.length
         */
        StringBuffer &append(const String &str, gint startIndex, gint endIndex);

        /**
         * Appends a subsequence of the specified StringBuffer to this sequence.
         *
         * <p>
         * Characters of the argument sb, starting at index startIndex, are appended,
         * in order, to the contents of this sequence up to the (exclusive) index endIndex.
         * The length of this sequence is increased by the value of endIndex - startIndex.
         *
         * @param sb the sequence to append.
         * @param startIndex   the starting index of the subsequence to be appended.
         * @param endIndex     the end index of the subsequence to be appended.
         * @throws IndexException if
         *             startIndex is negative, or
         *             startIndex is greater than endIndex or
         *             endIndex is greater than str.length
         */
        StringBuffer &append(const StringBuffer &sb, gint startIndex, gint endIndex);

        /**
         * Inserts the string representation of the Object
         * argument into this character sequence.
         *
         * <p>
         * The offset argument must be greater than or equal to
         * 0, and less than or equal to the length of this sequence.
         *
         * @param offset   the offset.
         * @param obj      an Object.
         * @throws IndexException  if the offset is invalid.
         */
        StringBuffer &insert(gint offset, const Object &obj);

        /**
         * Inserts the string into this character sequence.
         *
         * <p>
         * The characters of the String argument are inserted, in
         * order, into this sequence at the indicated offset, moving up any
         * characters originally above that position and increasing the length
         * of this sequence by the length of the argument.
         *
         * <p>
         * The character at index k in the new character sequence is
         * equal to:
         * <ul>
         * <li>the character at index k in the old character sequence,
         *      if k is less than offset
         * <li>the character at index k-offset in the argument str,
         *      if k is not less than offset but is less than offset+str.length
         * <li>the character at index k-str.length in the old character sequence,
         *      if k is not less than offset+str.length
         * </ul>
         *
         * <p>
         * The offset argument must be greater than or equal to
         * 0, and less than or equal to the length of this sequence.
         *
         * @param offset   the offset.
         * @param str      a string.
         * @throws IndexException  if the offset is invalid.
         */
        StringBuffer &insert(gint offset, const String &str);

        template<class Str, Class<gbool>::Iff<Class<Str>::isString()> = true>
        StringBuffer &insert(gint offset, Str &&str) {
            return insert(offset, (String) str);
        }

        /**
         * Inserts the specified StringBuffer into this sequence.
         *
         * <p>
         * The characters of the StringBuffer argument are inserted,
         * in order, into this sequence at the indicated offset, moving up
         * any characters originally above that position and increasing the length
         * of this sequence by the length of the argument s.
         *
         * @param offset   the offset.
         * @param sb the sequence to be inserted
         * @throws IndexException  if the offset is invalid.
         */
        StringBuffer &insert(gint offset, const StringBuffer &sb);

        /**
         * Inserts the string representation of the boolean
         * argument into this sequence.
         *
         * <p>
         * The offset argument must be greater than or equal to
         * 0, and less than or equal to the length
         * of this sequence.
         *
         * @param offset   the offset.
         * @param b        a boolean.
         * @throws IndexException  if the offset is invalid.
         */
        StringBuffer &insert(gint offset, gbool b);

        /**
         * Inserts the string representation of the char
         * argument into this sequence.
         *
         * <p>
         * The offset argument must be greater than or equal to
         * 0, and less than or equal to the length
         * of this sequence.
         *
         * @param offset   the offset.
         * @param ch        a char.
         * @throws IndexException  if the offset is invalid.
         */
        StringBuffer &insert(gint offset, gchar ch);

        template<class Chr, Class<gbool>::Iff<Class<Chr>::isCharacter()> = true>
        StringBuffer &insert(gint offset, Chr &&ch) {
            return insert(offset, (gchar) ch);
        }

        /**
         * Insert String representation of character (unicode code point) argument into this character
         * sequence.
         *
         * @param offset   the offset.
         * @param codePoint
         *         The character (unicode code point) to insert
         */
        StringBuffer &insertCodePoint(gint offset, gint codePoint);

        /**
         * Inserts the string representation of the second gint
         * argument into this sequence.
         *
         * <p>
         * The offset argument must be greater than or equal to
         * 0, and less than or equal to the length
         * of this sequence.
         *
         * @param offset   the offset.
         * @param i        an gint.
         * @throws IndexException  if the offset is invalid.
         */
        StringBuffer &insert(gint offset, gint i);

        /**
         * Inserts the string representation of the glong
         * argument into this sequence.
         *
         * <p>
         * The offset argument must be greater than or equal to
         * 0, and less than or equal to the length
         * of this sequence.
         *
         * @param offset   the offset.
         * @param l        a glong.
         * @throws IndexException  if the offset is invalid.
         */
        StringBuffer &insert(gint offset, glong l);

        /**
         * Insert unsigned String representation of gint value argument into this character sequence.
         *
         * @param offset   the offset.
         * @param i
         *         The gint value to insert
         */
        StringBuffer &insertUnsigned(gint offset, gint i);

        /**
         * Insert unsigned String representation of glong argument to this character sequence.
         *
         * @param offset   the offset.
         * @param l
         *         The glong value to insert
         */
        StringBuffer &insertUnsigned(gint offset, glong l);

        /**
         * Inserts the string representation of the float
         * argument into this sequence.
         *
         * <p>
         * The offset argument must be greater than or equal to
         * 0, and less than or equal to the length
         * of this sequence.
         *
         * @param offset   the offset.
         * @param f        a float.
         * @throws IndexException  if the offset is invalid.
         */
        StringBuffer &insert(gint offset, gfloat f);

        /**
         * Inserts the string representation of the double
         * argument into this sequence.
         *
         * <p>
         * The offset argument must be greater than or equal to
         * 0, and less than or equal to the length
         * of this sequence.
         *
         * @param offset   the offset.
         * @param d        a double.
         * @throws IndexException  if the offset is invalid.
         */
        StringBuffer &insert(gint offset, gdouble d);

        /**
         * Inserts the string representation of the char root
         * argument into this sequence.
         *
         * <p>
         * The characters of the root argument are inserted into the
         * contents of this sequence at the position indicated by
         * offset. The length of this sequence increases by
         * the length of the argument.
         *
         * <p>
         * The offset argument must be greater than or equal to
         * 0, and less than or equal to the length
         * of this sequence.
         *
         * @param offset   the offset.
         * @param str      a character root.
         * @throws IndexException  if the offset is invalid.
         */
        StringBuffer &insert(gint offset, const CharArray &chars);

        /**
         * Inserts the string representation of a subarray of the chars
         * root argument into this sequence. The subarray begins at the
         * specified offset and extends length chars.
         * The characters of the subarray are inserted into this sequence at
         * the position indicated by index. The length of this
         * sequence increases by length chars.
         *
         * @param      index    position at which to insert subarray.
         * @param      str       A char root.
         * @param      offset   the index of the first char in subarray to
         *             be inserted.
         * @param      length      the number of chars in the subarray to
         *             be inserted.
         * @throws     IndexException  if index
         *             is negative or greater than length, or
         *             offset or length are negative, or
         *             (offset+len) is greater than
         *             str.length.
         */
        StringBuffer &insert(gint index, const CharArray &chars, gint offset, gint length);

        /**
         * Inserts a subsequence of the specified CharSequence into
         * this sequence.
         *
         * <p>
         * The subsequence of the argument str specified by
         * startIndex and endIndex are inserted,
         * in order, into this sequence at the specified destination startIndex, moving
         * up any characters originally above that position. The length of this
         * sequence is increased by endIndex - startIndex.
         *
         * <p>
         * The character at index k in this sequence becomes equal to:
         * <ul>
         * <li>the character at index k in this sequence, if
         * k is less than offset
         * <li>the character at index k+startIndex-offset in
         * the argument str, if k is greater than or equal to
         * offset but is less than offset+endIndex-startIndex
         * <li>the character at index k-(endIndex-startIndex) in this
         * sequence, if k is greater than or equal to
         * offset+endIndex-startIndex
         * </ul>
         *
         * <p>
         * The offset argument must be greater than or equal to
         * 0, and less than or equal to the length
         * of this sequence.
         *
         * <p>
         * The startIndex argument must be non-negative, and not greater than
         * endIndex.
         *
         * <p>
         * The endIndex argument must be greater than or equal to
         * startIndex, and less than or equal to the length of str.
         *
         * @param offset   the startIndex in this sequence.
         * @param str       the sequence to be inserted.
         * @param startIndex   the starting index of the subsequence to be inserted.
         * @param endIndex     the end index of the subsequence to be inserted.
         * @throws IndexException  if offset
         *             is negative or greater than this.length, or
         *              startIndex or endIndex are negative, or
         *              startIndex is greater than endIndex or
         *              endIndex is greater than str.length
         */
        StringBuffer &insert(gint offset, const String &str, gint startIndex, gint endIndex);

        /**
         * Inserts a subsequence of the specified CharSequence into
         * this sequence.
         *
         * <p>
         * The subsequence of the argument sb specified by
         * startIndex and endIndex are inserted,
         * in order, into this sequence at the specified destination offset, moving
         * up any characters originally above that position. The length of this
         * sequence is increased by endIndex - startIndex.
         *
         * <p>
         * The character at index k in this sequence becomes equal to:
         * <ul>
         * <li>the character at index k in this sequence, if
         * k is less than dstOffset
         * <li>the character at index k+startIndex-dstOffset in
         * the argument sb, if k is greater than or equal to
         * dstOffset but is less than dstOffset+endIndex-startIndex
         * <li>the character at index k-(endIndex-startIndex) in this
         * sequence, if k is greater than or equal to
         * dstOffset+endIndex-startIndex
         * </ul>
         *
         * <p>
         * The dstOffset argument must be greater than or equal to
         * 0, and less than or equal to the length
         * of this sequence.
         * <p>
         * The startIndex argument must be non-negative, and not greater than
         * endIndex.
         *
         * <p>
         * The endIndex argument must be greater than or equal to
         * startIndex, and less than or equal to the length of sb.
         *
         * @param dstOffset   the offset in this sequence.
         * @param sb       the sequence to be inserted.
         * @param startIndex   the starting index of the subsequence to be inserted.
         * @param endIndex     the end index of the subsequence to be inserted.
         * @throws IndexException  if dstOffset
         *             is negative or greater than this.length, or
         *              startIndex or endIndex are negative, or
         *              startIndex is greater than endIndex or
         *              endIndex is greater than sb.length
         */
        StringBuffer &insert(gint offset, const StringBuffer &sb, gint startIndex, gint endIndex);

        /**
         * Replaces the characters in a substring of this sequence with characters in the specified String.
         * The substring begins at the specified startIndex and extends to the character
         * at index endIndex - 1 or to the endIndex of the sequence if no such character exists.
         * First the characters in the substring are removed and then the specified
         * String is inserted at startIndex. (This sequence will be lengthened to accommodate the
         * specified String if necessary.)
         *
         * @param startIndex The beginning index, inclusive.
         * @param endIndex   The ending index, exclusive.
         * @param str String that will replace previous contents.
         * @throws IndexException  if startIndex
         *             is negative, greater than length, or
         *             greater than endIndex.
         */
        StringBuffer &replace(gint startIndex, gint endIndex, const String &str);

        /**
         * Copy characters from this sequence into destination character root. The first character to
         * be copied is at index srcBegin; the last character to be copied is at index srcEnd-1.
         * The total number of characters to be copied is srcEnd-srcBegin. The characters are copied into
         * the subarray of dst starting at index dstBegin and ending at index: dstbegin + (srcEnd-srcBegin) - 1
         *
         * @param srcBegin   start copying at this offset.
         * @param srcEnd     stop copying at this offset.
         * @param dst        the root to copy the root into.
         * @param dstBegin   offset into dst.
         * @throws IndexException  if any of the following is true:
         *             <ul>
         *             <li> srcBegin is negative
         *             <li> dstBegin is negative
         *             <li> the srcBegin argument is greater than the srcEnd argument.
         *             <li> srcEnd is greater than this.length.
         *             <li> dstBegin+srcEnd-srcBegin is greater than dst.length
         *             </ul>
         */
        void chars(gint srcBegin, gint srcEnd, CharArray &dst, gint dstBegin) const;

        /**
         * Returns an root of char values from this sequence.  Any char which maps to a
         * surrogate code point</a> is passed through uninterpreted.
         */
        CharArray chars() const;

        /**
         * Returns an root of  of code point values from this sequence.  Any surrogate
         * pairs encountered in the sequence are combined as if and the result is passed
         * to the stream. Any other code units, including ordinary BMP characters,
         * unpaired surrogates, and undefined code units, are zero-extended to
         * gint values which are then passed to the root.
         */
        IntArray codePoints() const;

        /**
         * Returns a new String that contains a subsequence of characters currently contained
         * in this character sequence. The substring begins at the specified index and extends
         * to the end of this sequence.
         *
         * @param startIndex The beginning index, inclusive.
         * @throws IndexException  if startIndex is less than zero, or greater than the length of this object.
         */
        String subString(gint startIndex) const;

        /**
         * Returns a new character sequence that is a subsequence of this sequence.
         *
         * @param startIndex the start index, inclusive.
         * @param endIndex the end index, exclusive.
         * @throws IndexException
         *          if startIndex or endIndex are negative,
         *          if endIndex is greater than length(),
         *          or if startIndex is greater than endIndex
         */
        String subString(gint startIndex, gint endIndex) const;

        CharSequence &subSequence(gint start, gint end) const override;

        /**
         * Returns the index within this string of the first occurrence of the
         * specified substring.
         *
         * <p>The returned index is the smallest value k for which:
         * <pre>
         * this.toString().startsWith(str, k)
         * </pre>
         * If no such value of k exists, then -1 is returned.
         *
         * @param str   the substring to search for.
         * @return  the index of the first occurrence of the specified substring,
         *          or -1 if there is no such occurrence.
         */
        gint indexOf(const String &str) const;

        /**
         * Returns the index within this string of the first occurrence of the
         * specified substring, starting at the specified index.
         *
         * <p>The returned index is the smallest value k for which:
         * <pre>
         *     k >= Math.min(fromIndex, this.length) &&
         *                   this.toString().startsWith(str, k)
         * </pre>
         * If no such value of k exists, then -1 is returned.
         *
         * @param str         the substring to search for.
         * @param fromIndex   the index from which to start the search.
         * @return  the index of the first occurrence of the specified substring,
         *          starting at the specified index,
         *          or -1 if there is no such occurrence.
         */
        gint indexOf(const String &str, gint begin) const;

        /**
         * Returns the index within this string of the last occurrence of the
         * specified substring.  The last occurrence of the empty string "" is
         * considered to occur at the index value this.length.
         *
         * <p>The returned index is the largest value k for which:
         * <pre>
         * this.toString().startsWith(str, k)
         * </pre>
         * If no such value of k exists, then -1 is returned.
         *
         * @param str   the substring to search for.
         * @return  the index of the last occurrence of the specified substring,
         *          or -1 if there is no such occurrence.
         */
        gint lastIndexOf(const String &str) const;

        /**
         * Returns the index within this string of the last occurrence of the
         * specified substring, searching backward starting at the specified index.
         *
         * <p>The returned index is the largest value k for which:
         * <pre>
         *     k <= Math.min(fromIndex, this.length) &&
         *                   this.toString().startsWith(str, k)
         * </pre>
         * If no such value of k exists, then -1 is returned.
         *
         * @param str         the substring to search for.
         * @param fromIndex   the index to start the search from.
         * @return  the index of the last occurrence of the specified substring,
         *          searching backward from the specified index,
         *          or -1 if there is no such occurrence.
         */
        gint lastIndexOf(const String &str, gint begin) const;

        /**
         * Causes this character sequence to be replaced by the reverse of
         * the sequence. If there are any surrogate pairs included in the
         * sequence, these are treated as single characters for the
         * reverse operation. Thus, the order of the high-low surrogates
         * is never reversed.
         *
         * Let n be the character length of this character sequence
         * (not the length in char values) just prior to
         * execution of the reverse method. Then the
         * character at index k in the new character sequence is
         * equal to the character at index n-k-1 in the old
         * character sequence.
         *
         * <p>Note that the reverse operation may result in producing
         * surrogate pairs that were unpaired low-surrogates and
         * high-surrogates before the operation. For example, reversing
         * "\\uDC00\\uD800" produces "\\uD800\\uDC00" which is
         * a valid surrogate pair.
         *
         * @return  a reference to this object.
         */
        StringBuffer &reverse();

        /**
         * Return true if specified object is StringBuffer INSTANCE and has
         * same character sequence as this.
         *
         * @param object The object to be compared
         */
        gbool equals(const Object &object) const override;

        /**
         * Return sharable copy of this Object
         */
        Object &clone() const override;

        /**
         * Returns a string representing the root in this sequence.
         */
        String toString() const override;

        /**
         * Compares two StringBuffer instances lexicographically.
         *
         * @param other The object to be compared
         */
        gint compareTo(const StringBuffer &other) const override;

        /**
         * Removes the char at the specified position in this
         * sequence. This sequence is shortened by one char.
         *
         * <p>
         * Note: If the character at the given index is a supplementary
         * character, this method does not remove the entire character.
         *
         * @param       index  Index of char to remove
         * @throws      IndexException  if the index
         *              is negative or greater than or equal to
         *              length.
         */
        StringBuffer &removeAt(gint index);

        /**
         * Removes the characters in a substring of this sequence.
         * The substring begins at the specified startIndex and extends to
         * the character at index endIndex - 1 or to the end of the
         * sequence if no such character exists. If start is equal to end, no changes are made.
         *
         * @param      startIndex  The beginning index, inclusive.
         * @param      endIndex    The ending index, exclusive.
         * @throws     IndexException  if startIndex
         *             is negative, greater than length, or
         *             greater than endIndex.
         */
        StringBuffer &remove(gint startIndex, gint endIndex);

        ~StringBuffer() override;
    };

} // core

#endif //CORE23_STRINGBUFFER_H
