//
// Created by T.N.Brunshweeck on 14/01/2024.
//

#ifndef CORE23_SURROGATE_H
#define CORE23_SURROGATE_H

#include <core/Character.h>
#include <core/charset/CoderResult.h>
#include <core/charset/MalformedInputException.h>
#include <core/charset/UnmappableCharacterException.h>
#include <core/io/CharBuffer.h>
#include <core/io/ByteBuffer.h>

namespace core {
    namespace charset {

        using namespace io;

        /**
         * Utility class for dealing with surrogates.
         *
         * @author Brunshweeck Tazeussong
         */
        class Surrogate : public Object {
        private:
            Surrogate() = default;

        public:

            // TODO: Deprecate/remove the following redundant definitions
            static CORE_FAST gchar MIN_HIGH = Character::MIN_HIGH_SURROGATE;
            static CORE_FAST gchar MAX_HIGH = Character::MAX_HIGH_SURROGATE;
            static CORE_FAST gchar MIN_LOW = Character::MIN_LOW_SURROGATE;
            static CORE_FAST gchar MAX_LOW = Character::MAX_LOW_SURROGATE;
            static CORE_FAST gchar MIN = Character::MIN_SURROGATE;
            static CORE_FAST gchar MAX = Character::MAX_SURROGATE;
            static CORE_FAST gint UCS4_MIN = Character::MIN_SUPPLEMENTARY;
            static CORE_FAST gint UCS4_MAX = Character::MAX_CODEPOINT;

            /**
             * Tells whether or not the given value is in the high surrogate range.
             * Use of <b style="color:orange;"> Character#isHighSurrogate</b>  is generally preferred.
             */
            static gbool isHigh(gint c);

            /**
             * Tells whether or not the given value is in the low surrogate range.
             * Use of <b style="color:orange;"> Character#isLowSurrogate</b>  is generally preferred.
             */
            static gbool isLow(gint c);

            /**
             * Tells whether or not the given value is in the surrogate range.
             * Use of <b style="color:orange;"> Character#isSurrogate</b>  is generally preferred.
             */
            static gbool is(gint c);

            /**
             * Tells whether or not the given UCS-4 character must be represented as a
             * surrogate pair in UTF-16.
             * Use of <b style="color:orange;"> Character#isSupplementary</b>  is generally preferred.
             */
            static gbool neededFor(gint uc);

            /**
             * Returns the high UTF-16 surrogate for the given supplementary UCS-4 character.
             * Use of <b style="color:orange;"> Character#highSurrogate</b>  is generally preferred.
             */
            static gchar high(gint uc);

            /**
             * Returns the low UTF-16 surrogate for the given supplementary UCS-4 character.
             * Use of <b style="color:orange;"> Character#lowSurrogate</b>  is generally preferred.
             */
            static gchar low(gint uc);

            /**
             * Converts the given surrogate pair into a 32-bit UCS-4 character.
             * Use of <b style="color:orange;"> Character#joinSurrogates</b>  is generally preferred.
             */
            static gint toUCS4(gchar c, gchar d);

            /**
             * Surrogate parsing support.  Charset implementations may use instances of
             * this class to handle the details of parsing UTF-16 surrogate pairs.
             */
            class Parser : public Object {

            public :
                Parser() = default;

            private :
                gint chr = -1;          // UCS-4
                CoderResult err = CoderResult::UNDERFLOW;
                gbool pair = false;

                /**
                 * Returns the UCS-4 character previously parsed.
                 */
            public :
                gint character() const;

                /**
                 * Tells whether or not the previously-parsed UCS-4 character was
                 * originally represented by a surrogate pair.
                 */
                gbool isPair() const;

                /**
                 * Returns the number of UTF-16 characters consumed by the previous
                 * parse.
                 */
                gint increment() const;

                /**
                 * If the previous parse operation detected an error, return the object
                 * describing that error.
                 */
                CoderResult error() const;

                /**
                 * Returns an unmappable-input result object, with the appropriate
                 * input length, for the previously-parsed character.
                 */
                CoderResult unmappableResult() const;

                /**
                 * Parses a UCS-4 character from the given source buffer, handling
                 * surrogates.
                 *
                 * @param  c    The first character
                 * @param  in   The source buffer, from which one more character
                 *              will be consumed if c is a high surrogate
                 *
                 * @return  Either a parsed UCS-4 character, in which case the isPair()
                 *          and increment() methods will return meaningful values, or
                 *          -1, in which case error() will return a descriptive result
                 *          object
                 */
                gint parse(gchar c, CharBuffer &in);

                /**
                 * Parses a UCS-4 character from the given source buffer, handling
                 * surrogates.
                 *
                 * @param  c    The first character
                 * @param  ia   The input array, from which one more character
                 *              will be consumed if c is a high surrogate
                 * @param  ip   The input index
                 * @param  il   The input limit
                 *
                 * @return  Either a parsed UCS-4 character, in which case the isPair()
                 *          and increment() methods will return meaningful values, or
                 *          -1, in which case error() will return a descriptive result
                 *          object
                 */
                gint parse(gchar c, const CharArray &ia, gint ip, gint il);

            }; // class Parser

            /**
             * Surrogate generation support.  Charset implementations may use instances
             * of this class to handle the details of generating UTF-16 surrogate
             * pairs.
             */
            class Generator : public Object {

            public :
                Generator() = default;

            private :
                CoderResult err = CoderResult::OVERFLOW;

                /**
                 * If the previous generation operation detected an error, return the
                 * object describing that error.
                 */
            public :
                CoderResult error() const;

                /**
                 * Generates one or two UTF-16 characters to represent the given UCS-4
                 * character.
                 *
                 * @param  uc   The UCS-4 character
                 * @param  len  The number of input bytes from which the UCS-4 value
                 *              was constructed (used when creating result objects)
                 * @param  dst  The destination buffer, to which one or two UTF-16
                 *              characters will be written
                 *
                 * @return  Either a positive count of the number of UTF-16 characters
                 *          written to the destination buffer, or -1, in which case
                 *          error() will return a descriptive result object
                 */
                gint generate(gint uc, gint len, CharBuffer &dst);

                /**
                 * Generates one or two UTF-16 characters to represent the given UCS-4
                 * character.
                 *
                 * @param  uc   The UCS-4 character
                 * @param  len  The number of input bytes from which the UCS-4 value
                 *              was constructed (used when creating result objects)
                 * @param  da   The destination array, to which one or two UTF-16
                 *              characters will be written
                 * @param  dp   The destination position
                 * @param  dl   The destination limit
                 *
                 * @return  Either a positive count of the number of UTF-16 characters
                 *          written to the destination buffer, or -1, in which case
                 *          error() will return a descriptive result object
                 */
                gint generate(gint uc, gint len, CharArray &da, gint dp, gint dl);
            }; // class Generator
        }; // class Surrogate

    } // charset
} // core

#endif //CORE23_SURROGATE_H
