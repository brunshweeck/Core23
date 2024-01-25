//
// Created by T.N.Brunshweeck on 05/01/2024.
//

#ifndef CORE23_CHARSEQUENCE_H
#define CORE23_CHARSEQUENCE_H

#include <core/Comparable.h>

namespace core {

    /**
     * A <b> CharSequence</b>  is a readable sequence of <b> char</b>  values. This
     * interface provides uniform, read-only access to many different kinds of
     * <b> char</b>  sequences.
     * A <b> char</b>  value represents a character in the <i>Basic
     * Multilingual Plane (BMP)</i> or a surrogate. Refer to <a
     * href="">Unicode Character Representation</a> for details.
     *
     * <p> This interface does not refine the general contracts of the <b style="color:orange;">
     * equals</b>  and <b style="color:orange;"> hash</b>  methods. The result of testing two objects
     * that implement <b> CharSequence</b>  for equality is therefore, in general, undefined.
     * Each object may be implemented by a different class, and there
     * is no guarantee that each class will be capable of testing its instances
     * for equality with those of the other.  It is therefore inappropriate to use
     * arbitrary <b> CharSequence</b>  instances as elements in a set or as keys in
     * a map. </p>
     *
     * @author Brunshweeck Tazeussong
     */
    interface CharSequence {

        /**
         * Returns the length of this character sequence.  The length is the number
         * of 16-bit <b> char</b> s in the sequence.
         *
         * @return  the number of <b> char</b> s in this sequence
         */
        virtual gint length() const = 0;

        /**
         * Returns the <b> char</b>  value at the specified index.  An index ranges from zero
         * to <b> length() - 1</b> .  The first <b> char</b>  value of the sequence is at
         * index zero, the next at index one, and so on, as for array
         * indexing.
         *
         * <p>If the <b> char</b>  value specified by the index is a
         * <a href="{@docRoot</b> /java.base/java/lang/Character.html#unicode">surrogate</a>, the surrogate
         * value is returned.
         *
         * @param   index   the index of the <b> char</b>  value to be returned
         *
         * @return  the specified <b> char</b>  value
         *
         * @throws  IndexException
         *          if the <b> index</b>  argument is negative or not less than
         *          <b> length()</b> 
         */
        virtual gchar charAt(gint index) const = 0;

        /**
         * Returns <b> true</b>  if this character sequence is empty.
         *
         * @implSpec
         * The default implementation returns the result of calling <b> length() == 0</b> .
         *
         * @return <b> true</b>  if <b style="color:orange;"> length()</b>  is <b> 0</b> , otherwise
         * <b> false</b> 
         */
        virtual gbool isEmpty() const;

        /**
         * Returns a <b> CharSequence</b>  that is a subsequence of this sequence.
         * The subsequence starts with the <b> char</b>  value at the specified index and
         * ends with the <b> char</b>  value at index <b> end - 1</b> .  The length
         * (in <b> char</b> s) of the
         * returned sequence is <b> end - start</b> , so if <b> start == end</b> 
         * then an empty sequence is returned.
         *
         * @param   start   the start index, inclusive
         * @param   end     the end index, exclusive
         *
         * @return  the specified subsequence
         *
         * @throws  IndexException
         *          if <b> start</b>  or <b> end</b>  are negative,
         *          if <b> end</b>  is greater than <b> length()</b> ,
         *          or if <b> start</b>  is greater than <b> end</b> 
         */
        virtual CharSequence &subSequence(gint start, gint end) const =0;

        /**
         * Returns a string containing the characters in this sequence in the same
         * order as this sequence.  The length of the string will be the length of
         * this sequence.
         *
         * @return  a string consisting of exactly this sequence of characters
         */
        virtual String toString() const = 0;

        /**
         * Returns a stream of <b> gint</b>  zero-extending the <b> char</b>  values
         * from this sequence.  Any char which maps to a <a href="">surrogate code
         * point</a> is passed through uninterpreted.
         *
         * <p>The stream binds to this sequence when the terminal stream operation
         * commences (specifically, for mutable sequences the spliterator for the
         * stream is <a href=""><em>late-binding</em></a>).
         * If the sequence is modified during that operation then the result is
         * undefined.
         *
         * @return an IntStream of char values from this sequence
         */
        // virtual IntStream charsIterator() const;

        /**
         * Returns a stream of code point values from this sequence.  Any surrogate
         * pairs encountered in the sequence are combined as if by {@linkplain
         * Character#toCodePoint Character.toCodePoint</b>  and the result is passed
         * to the stream. Any other code units, including ordinary BMP characters,
         * unpaired surrogates, and undefined code units, are zero-extended to
         * <b> gint</b>  values which are then passed to the stream.
         *
         * <p>The stream binds to this sequence when the terminal stream operation
         * commences (specifically, for mutable sequences the spliterator for the
         * stream is <a href=""><em>late-binding</em></a>).
         * If the sequence is modified during that operation then the result is
         * undefined.
         *
         * @return an IntStream of Unicode code points from this sequence
         */
        // IntStream codePointsIterator() const;

        /**
         * Compares two <b> CharSequence</b>  instances lexicographically. Returns a
         * negative value, zero, or a positive value if the first sequence is lexicographically
         * less than, equal to, or greater than the second, respectively.
         *
         * <p>
         * The lexicographical ordering of <b> CharSequence</b>  is defined as follows.
         * Consider a <b> CharSequence</b>  <i>cs</i> of length <i>len</i> to be a
         * sequence of char values, <i>cs[0]</i> to <i>cs[len-1]</i>. Suppose <i>k</i>
         * is the lowest index at which the corresponding char values from each sequence
         * differ. The lexicographic ordering of the sequences is determined by a numeric
         * comparison of the char values <i>cs1[k]</i> with <i>cs2[k]</i>. If there is
         * no such index <i>k</i>, the shorter sequence is considered lexicographically
         * less than the other. If the sequences have the same length, the sequences are
         * considered lexicographically equal.
         *
         *
         * @param cs1 the first <b> CharSequence</b> 
         * @param cs2 the second <b> CharSequence</b> 
         *
         * @return  the value <b> 0</b>  if the two <b> CharSequence</b>  are equal;
         *          a negative integer if the first <b> CharSequence</b> 
         *          is lexicographically less than the second; or a
         *          positive integer if the first <b> CharSequence</b>  is
         *          lexicographically greater than the second.
         */
        static gint compare(const CharSequence &cs1, const CharSequence &cs2);

        virtual ~CharSequence() = default;
    };

} // core

#endif //CORE23_CHARSEQUENCE_H
