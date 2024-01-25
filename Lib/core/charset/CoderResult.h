//
// Created by T.N.Brunshweeck on 06/01/2024.
//

#ifndef CORE23_CODERRESULT_H
#define CORE23_CODERRESULT_H

#include <core/Object.h>

namespace core {
    namespace charset {

        /**
         * A description of the result state of a coder.
         *
         * <p> A charset coder, that is, either a decoder or an encoder, consumes bytes
         * (or characters) from an input buffer, translates them, and writes the
         * resulting characters (or bytes) to an output buffer.  A coding process
         * terminates for one of four categories of reasons, which are described by
         * instances of this class:
         *
         * <ul>
         *
         *   <li><p> <i>Underflow</i> is reported when there is no more input to be
         *   processed, or there is insufficient input and additional input is
         *   required.  This condition is represented by the unique result object
         *   <b style="color:orange;"> UNDERFLOW</b>, whose <b style="color:orange;"> isUnderflow</b> method
         *   returns <b> true</b>.  </p></li>
         *
         *   <li><p> <i>Overflow</i> is reported when there is insufficient room
         *   remaining in the output buffer.  This condition is represented by the
         *   unique result object <b style="color:orange;"> OVERFLOW</b>, whose <b style="color:orange;">
         *   isOverflow</b> method returns <b> true</b>.  </p></li>
         *
         *   <li><p> A <i>malformed-input error</i> is reported when a sequence of
         *   input units is not well-formed.  Such errors are described by instances of
         *   this class whose <b style="color:orange;"> isMalformed</b> method returns
         *   <b> true</b> and whose <b style="color:orange;"> length</b> method returns the length
         *   of the malformed sequence.  There is one unique instance of this class for
         *   all malformed-input errors of a given length.  </p></li>
         *
         *   <li><p> An <i>unmappable-character error</i> is reported when a sequence
         *   of input units denotes a character that cannot be represented in the
         *   output charset.  Such errors are described by instances of this class
         *   whose <b style="color:orange;"> isUnmappable</b> method returns <b> true</b> and
         *   whose <b style="color:orange;"> length</b> method returns the length of the input
         *   sequence denoting the unmappable character.  There is one unique instance
         *   of this class for all unmappable-character errors of a given length.
         *   </p></li>
         *
         * </ul>
         *
         * <p> For convenience, the <b style="color:orange;"> isError</b> method returns <b> true</b>
         * for result objects that describe malformed-input and unmappable-character
         * errors but <b> false</b> for those that describe underflow or overflow
         * conditions.  </p>
         *
         *
         * @author Brunshweeck Tazeussong
         */
        class CoderResult : public Object {
        private:
            static CORE_FAST gint UNDERFLOW_CODE = 0;
            static CORE_FAST gint OVERFLOW_CODE = 1;
            static CORE_FAST gint MALFORMED_CODE = 2;
            static CORE_FAST gint UNMAPPABLE_CODE = 3;

            gint t;
            gint len;

            CORE_FAST CoderResult(gint type, gint length) : t(type), len(length) {}

        public:

            /**
             * Returns a string describing this coder result.
             *
             * @return  A descriptive string
             */
            String toString() const override;

            /**
             * Tells whether or not this object describes an underflow condition.
             *
             * @return  <b> true</b> if, and only if, this object denotes underflow
             */
            gbool isUnderflow() const;

            /**
             * Tells whether or not this object describes an overflow condition.
             *
             * @return  <b> true</b> if, and only if, this object denotes overflow
             */
            gbool isOverflow() const;

            /**
             * Tells whether or not this object describes an error condition.
             *
             * @return  <b> true</b> if, and only if, this object denotes either a
             *          malformed-input error or an unmappable-character error
             */
            gbool isError() const;

            /**
             * Tells whether or not this object describes a malformed-input error.
             *
             * @return  <b> true</b> if, and only if, this object denotes a
             *          malformed-input error
             */
            gbool isMalformed() const;

            /**
             * Tells whether or not this object describes an unmappable-character
             * error.
             *
             * @return  <b> true</b> if, and only if, this object denotes an
             *          unmappable-character error
             */
            gbool isUnmappable() const;

            /**
             * Returns the length of the erroneous input described by this
             * object&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * @return  The length of the erroneous input, a positive integer
             *
             * @throws  UnsupportedOperationException
             *          If this object does not describe an error condition, that is,
             *          if the <b style="color:orange;"> isError</b> does not return <b> true</b>
             */
            gint length() const;

            /**
             * Result object indicating underflow, meaning that either the input buffer
             * has been completely consumed or, if the input buffer is not yet empty,
             * that additional input is required.
             */
            static const CoderResult UNDERFLOW;

            /**
             * Result object indicating overflow, meaning that there is insufficient
             * room in the output buffer.
             */
            static const CoderResult OVERFLOW;

        private:

            static const CoderResult malformed[4];
            static const CoderResult unmappable[4];

        public:

            /**
             * Static factory method that returns the unique object describing a
             * malformed-input error of the given length.
             *
             * @param   length
             *          The given length
             *
             * @return  The requested coder-result object
             */
            static const CoderResult &malformedForLength(gint length);

            /**
             * Static factory method that returns the unique result object describing
             * an unmappable-character error of the given length.
             *
             * @param   length
             *          The given length
             *
             * @return  The requested coder-result object
             */
            static const CoderResult &unmappableForLength(gint length);

            gbool equals(const Object &o) const override;

            Object &clone() const override;

            gint hash() const override;

            void throwException() const;
        };

    } // charset
} // core

#endif //CORE23_CODERRESULT_H
