//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_CHARSETENCODER_H
#define CORE23_CHARSETENCODER_H

#include <core/charset/Charset.h>
#include <core/charset/CoderResult.h>
#include <core/native/ByteArray.h>

namespace core {
    namespace charset {

        /**
         * An engine that can transform a sequence of sixteen-bit Unicode characters into a sequence of
         * bytes in a specific charset.
         *
         * <a id="steps"></a>
         *
         * <p> The input character sequence is provided in a character buffer or a series
         * of such buffers.  The output byte sequence is written to a byte buffer
         * or a series of such buffers.  An encoder should always be used by making
         * the following sequence of method invocations, hereinafter referred to as an
         * <i>encoding operation</i>:
         *
         * <ol>
         *
         *   <li><p> Reset the encoder via the <b style="color:orange;"> reset</b> method, unless it
         *   has not been used before; </p></li>
         *
         *   <li><p> Invoke the <b style="color:orange;"> encode</b> method zero or more times, as
         *   long as additional input may be available, passing <b> false</b> for the
         *   <b> endOfInput</b> argument and filling the input buffer and flushing the
         *   output buffer between invocations; </p></li>
         *
         *   <li><p> Invoke the <b style="color:orange;"> encode</b> method one final time, passing
         *   <b> true</b> for the <b> endOfInput</b> argument; and then </p></li>
         *
         *   <li><p> Invoke the <b style="color:orange;"> flush</b> method so that the encoder can
         *   flush any internal state to the output buffer. </p></li>
         *
         * </ol>
         *
         * Each invocation of the <b style="color:orange;"> encode</b> method will encode as many
         * characters as possible from the input buffer, writing the resulting bytes
         * to the output buffer.  The <b style="color:orange;"> encode</b> method returns when more
         * input is required, when there is not enough room in the output buffer, or
         * when an encoding error has occurred.  In each case a <b style="color:orange;"> CoderResult</b>
         * object is returned to describe the reason for termination.  An invoker can
         * examine this object and fill the input buffer, flush the output buffer, or
         * attempt to recover from an encoding error, as appropriate, and try again.
         *
         * <a id="ce"></a>
         *
         * <p> There are two general types of encoding errors.  If the input character
         * sequence is not a legal sixteen-bit Unicode sequence then the input is considered <i>malformed</i>.  If
         * the input character sequence is legal but cannot be mapped to a valid
         * byte sequence in the given charset then an <i>unmappable character</i> has been encountered.
         *
         * <a id="cae"></a>
         *
         * <p> How an encoding error is handled depends upon the action requested for
         * that type of error, which is described by an instance of the <b style="color:orange;"> 
         * CodingErrorAction</b> class.  The possible error actions are to <b style="color: green;"> 
         * ignore</b> the erroneous input, <b style="color: green;">
         * report</b> the error to the invoker via
         * the returned <b style="color:orange;"> CoderResult</b> object, or <b style="color: green;"> replace</b> the erroneous input with the current value of the
         * replacement byte array.  The replacement
         *
         * is initially setValue to the encoder's default replacement, which often
         * (but not always) has the initial value&nbsp;<code>{</code>&nbsp;<code>(gbyte)'?'</code>&nbsp;<code>}</code>;
         *
         * its value may be changed via the <b style="color:orange;"> replaceWith</b> method.
         *
         * <p> The default action for malformed-input and unmappable-character errors
         * is to <b style="color: green;"> report</b> them.  The
         * malformed-input error action may be changed via the <b style="color:orange;"> 
         * onMalformedInput</b> method; the
         * unmappable-character action may be changed via the <b style="color:orange;"> 
         * onUnmappableCharacter</b> method.
         *
         * <p> This class is designed to handle many of the details of the encoding
         * process, including the implementation of error actions.  An encoder for a
         * specific charset, which is a concrete subclass of this class, need only
         * implement the abstract <b style="color:orange;"> encodeLoop</b> method, which
         * encapsulates the basic encoding loop.  A subclass that maintains internal
         * state should, additionally, override the <b style="color:orange;"> flush</b> and
         * <b style="color:orange;"> reset</b> methods.
         *
         * <p> Instances of this class are not safe for use by multiple concurrent
         * threads.  </p>
         *
         *
         * @author Brunshweeck Tazeussong
         *
         * @see ByteBuffer
         * @see CharBuffer
         * @see Charset
         * @see CharsetDecoder
         */
        class CharsetEncoder: public Object {
        private:
            const Charset &cs;
            gfloat abpc;
            gfloat mbpc;
            ByteArray repl;

            Charset::CodingErrorAction malformedAction = Charset::REPORT;
            Charset::CodingErrorAction unmappableAction = Charset::REPORT;

            enum CoderState : gbyte {
                RESET,
                CODING,
                END,
                FLUSHED
            };

            CoderState state = RESET;

        protected:


            /**
             * Initializes a new encoder.  The new encoder will have the given
             * bytes-per-char and replacement values.
             *
             * @param  cs
             *         The charset that created this encoder
             *
             * @param  averageBytesPerChar
             *         A positive float value indicating the expected number of
             *         bytes that will be produced for each input character
             *
             * @param  maxBytesPerChar
             *         A positive float value indicating the maximum number of
             *         bytes that will be produced for each input character
             *
             * @param  replacement
             *         The initial replacement; must have
             *         non-zero length, must not be longer than maxBytesPerChar,
             *         and must be <b style="color: green;"> legal</b>
             *
             * @throws  IllegalArgumentException
             *          If the preconditions on the parameters do not hold
             */
            CORE_EXPLICIT CharsetEncoder(const Charset &cs,
                                         gfloat averageBytesPerChar,
                                         gfloat maxBytesPerChar,
                                         ByteArray replacement);

            /**
             * Initializes a new encoder.  The new encoder will have the given
             * bytes-per-char values and its replacement will be the
             * byte array <code>{</code>&nbsp;<code>(byte)'?'</code>&nbsp;<code>}</code>.
             *
             * @param  cs
             *         The charset that created this encoder
             *
             * @param  averageBytesPerChar
             *         A positive float value indicating the expected number of
             *         bytes that will be produced for each input character
             *
             * @param  maxBytesPerChar
             *         A positive float value indicating the maximum number of
             *         bytes that will be produced for each input character
             *
             * @throws  IllegalArgumentException
             *          If the preconditions on the parameters do not hold
             */
            CORE_EXPLICIT CharsetEncoder(const Charset &cs,
                                         gfloat averageBytesPerChar,
                                         gfloat maxBytesPerChar);

        public:
            /**
             * Returns the charset that created this encoder.
             *
             * @return  This encoder's charset
             */
            const Charset &charset() const;

            /**
             * Returns this encoder's replacement value.
             *
             * @return  This encoder's current replacement,
             *          which is never empty
             */
            ByteArray replacement() const;

            /**
             * Changes this encoder's replacement value.
             *
             * <p> This method invokes the <b style="color: orange;"> replaceWith0</b>
             * method, passing the new replacement, after checking that the new
             * replacement is acceptable.  </p>
             *
             * @param  newReplacement  The new replacement; must have non-zero length,

             *         and must not be longer than the value returned by the
             *         <b style="color: orange;"> maxCharsPerByte</b> method
             *
             * @return  This encoder
             *
             * @throws  IllegalArgumentException
             *          If the preconditions on the parameter do not hold
             */
            CharsetEncoder &replaceWith(ByteArray newReplacement);

        protected:

            /**
             * Reports a change to this encoder's replacement value.
             *
             * <p> The default implementation of this method does nothing.  This method
             * should be overridden by encoders that require notification of changes to
             * the replacement.  </p>
             *
             * @param  newReplacement    The replacement value
             */
            virtual void replaceWith0(const ByteArray &newReplacement);

        private:
            CORE_ALIAS(Cache, typename Class<CharsetDecoder>::Ptr);
            Cache cachedDecoder = null;

        public:

            /**
             * Tells whether or not the given byte array is a legal replacement value
             * for this encoder.
             *
             * <p> A replacement is legal if, and only if, it is a legal sequence of
             * bytes in this encoder's charset; that is, it must be possible to decode
             * the replacement into one or more sixteen-bit Unicode characters.
             *
             * <p> The default implementation of this method is not very efficient; it
             * should generally be overridden to improve performance.  </p>
             *
             * @param  replacement  The byte array to be tested
             *
             * @return  <b> true</b> if, and only if, the given byte array
             *          is a legal replacement value for this encoder
             */
             virtual gbool isLegalReplacement(const ByteArray& replacement) const;

            /**
             * Returns this encoder's current action for malformed-input errors.
             *
             * @return The current malformed-input action
             */
            Charset::CodingErrorAction malformedInputAction() const;

            /**
             * Changes this encoder's action for malformed-input errors.
             *
             * <p> This method invokes the <b style="color: orange;"> onMalformedInput0</b>
             * method, passing the new action.  </p>
             *
             * @param  newAction  The new action; must not be <b> null</b>
             *
             * @return  This encoder
             *
             * @throws IllegalArgumentException
             *         If the precondition on the parameter does not hold
             */
            CharsetEncoder &onMalformedInput(Charset::CodingErrorAction newAction);

        protected:

            /**
             * Reports a change to this encoder's malformed-input action.
             *
             * <p> The default implementation of this method does nothing.  This method
             * should be overridden by encoders that require notification of changes to
             * the malformed-input action.  </p>
             *
             * @param  newAction  The new action
             */
            virtual void onMalformedInput0(Charset::CodingErrorAction newAction);

        public:

            /**
             * Returns this encoder's current action for unmappable-character errors.
             *
             * @return The current unmappable-character action
             */
            Charset::CodingErrorAction unmappableCharacterAction() const;

            /**
             * Changes this encoder's action for unmappable-character errors.
             *
             * <p> This method invokes the <b style="color: orange;"> onUnmappableCharacter0</b> method,
             * passing the new action.  </p>
             *
             * @param  newAction  The new action; must not be <b> null</b>
             *
             * @return  This encoder
             *
             * @throws IllegalArgumentException
             *         If the precondition on the parameter does not hold
             */
            CharsetEncoder &onUnmappableCharacter(Charset::CodingErrorAction newAction);

        protected:

            /**
             * Reports a change to this encoder's unmappable-character action.
             *
             * <p> The default implementation of this method does nothing.  This method
             * should be overridden by encoders that require notification of changes to
             * the unmappable-character action.  </p>
             *
             * @param  newAction  The new action
             */
            virtual void onUnmappableCharacter0(Charset::CodingErrorAction newAction);

        public:

            /**
             * Returns the average number of bytes that will be produced for each
             * character of input.  This heuristic value may be used to estimate the size
             * of the output buffer required for a given input sequence.
             *
             * @return  The average number of bytes produced
             *          per character of input
             */
            gfloat averageBytesPerChar() const;

            /**
             * Returns the maximum number of bytes that will be produced for each
             * character of input.  This value may be used to compute the worst-case size
             * of the output buffer required for a given input sequence. This value
             * accounts for any necessary content-independent prefix or suffix
             * bytes, such as byte-order marks.
             *
             * @return  The maximum number of bytes that will be produced per
             *          character of input
             */
            gfloat maxCharsPerByte() const;

            /**
             * Encodes as many characters as possible from the given input buffer,
             * writing the results to the given output buffer.
             *
             * <p> The buffers are read from, and written to, starting at their current
             * positions.  At most <b style="color: orange;"> in.remaining()</b> characters
             * will be read and at most <b style="color: orange;"> out.remaining()</b>
             * bytes will be written.  The buffers' positions will be advanced to
             * reflect the characters read and the bytes written, but their marks and
             * limits will not be modified.
             *
             * <p> In addition to reading characters from the input buffer and writing
             * bytes to the output buffer, this method returns a <b style="color: orange;"> CoderResult</b>
             * object to describe its reason for termination:
             *
             * <ul>
             *
             *   <li><p> <b style="color: orange;"> CoderResult.UNDERFLOW</b> indicates that as much of the
             *   input buffer as possible has been encoded.  If there is no further
             *   input then the invoker can proceed to the next step of the
             *   <a href="">encoding operation</a>.  Otherwise this method
             *   should be invoked again with further input.  </p></li>
             *
             *   <li><p> <b style="color: orange;"> CoderResult.OVERFLOW</b> indicates that there is
             *   insufficient space in the output buffer to encode any more characters.
             *   This method should be invoked again with an output buffer that has
             *   more <b style="color: green;"> remaining</b> bytes. This is
             *   typically done by draining any encoded bytes from the output
             *   buffer.  </p></li>
             *
             *   <li><p> A <b style="color: green;"> malformed-input</b> result indicates that a malformed-input
             *   error has been detected.  The malformed characters begin at the input
             *   buffer's (possibly incremented) position; the number of malformed
             *   characters may be determined by invoking the result object's
             *   <b style="color: orange;"> length</b> method.  This case applies only if the
             *   <b style="color: green;"> malformed action</b> of this encoder
             *   is <b style="color: orange;"> CodingErrorAction.REPORT</b>; otherwise the malformed input
             *   will be ignored or replaced, as requested.  </p></li>
             *
             *   <li><p> An <b style="color: green;"> unmappable-character</b> result indicates that an
             *   unmappable-character error has been detected.  The characters that
             *   encode the unmappable character begin at the input buffer's (possibly
             *   incremented) position; the number of such characters may be determined
             *   by invoking the result object's <b style="color: orange;"> CoderResult.length() length</b>
             *   method.  This case applies only if the <b style="color: green;"> unmappable action</b> of this encoder
             *   is <b style="color: orange;"> CodingErrorAction.REPORT</b>; otherwise the unmappable character will be
             *   ignored or replaced, as requested.  </p></li>
             *
             * </ul>
             *
             * In any case, if this method is to be reinvoked in the same encoding
             * operation then care should be taken to preserve any characters remaining
             * in the input buffer so that they are available to the next invocation.
             *
             * <p> The <b> endOfInput</b> parameter advises this method as to whether
             * the invoker can provide further input beyond that contained in the given
             * input buffer.  If there is a possibility of providing additional input
             * then the invoker should pass <b> false</b> for this parameter; if there
             * is no possibility of providing further input then the invoker should
             * pass <b> true</b>.  It is not erroneous, and in fact it is quite
             * common, to pass <b> false</b> in one invocation and later discover that
             * no further input was actually available.  It is critical, however, that
             * the final invocation of this method in a sequence of invocations always
             * pass <b> true</b> so that any remaining unencoded input will be treated
             * as being malformed.
             *
             * <p> This method works by invoking the <b style="color: orange;"> encodeLoop</b>
             * method, interpreting its results, handling error conditions, and
             * reinvoking it as necessary.  </p>
             *
             *
             * @param  in
             *         The input character buffer
             *
             * @param  out
             *         The output byte buffer
             *
             * @param  endOfInput
             *         <b> true</b> if, and only if, the invoker can provide no
             *         additional input characters beyond those in the given buffer
             *
             * @return  A coder-result object describing the reason for termination
             *
             * @throws  IllegalStateException
             *          If an encoding operation is already in progress and the previous
             *          step was an invocation neither of the <b style="color: orange;"> reset</b>
             *          method, nor of this method with a value of <b> false</b> for
             *          the <b> endOfInput</b> parameter, nor of this method with a
             *          value of <b> true</b> for the <b> endOfInput</b> parameter
             *          but a return value indicating an incomplete encoding operation
             *
             * @throws  CoderMalfunctionError
             *          If an invocation of the encodeLoop method threw
             *          an unexpected exception
             */
            CoderResult encode(io::CharBuffer &in, io::ByteBuffer& out, gbool endOfInput);

            /**
             * Flushes this encoder.
             *
             * <p> Some encoders maintain internal state and may need to write some
             * final bytes to the output buffer once the overall input sequence has
             * been read.
             *
             * <p> Any additional output is written to the output buffer beginning at
             * its current position.  At most <b style="color: orange;"> out.remaining()</b>
             * bytes will be written.  The buffer's position will be advanced
             * appropriately, but its mark and limit will not be modified.
             *
             * <p> If this method completes successfully then it returns
             * <b style="color: orange;"> CoderResult.UNDERFLOW</b>.  If there is insufficient room in the output
             * buffer then it returns <b style="color: orange;"> CoderResult.OVERFLOW</b>.  If this happens
             * then this method must be invoked again, with an output buffer that has
             * more room, in order to complete the current <a href="">encoding
             * operation</a>.
             *
             * <p> If this encoder has already been flushed then invoking this method
             * has no effect.
             *
             * <p> This method invokes the <b style="color: orange;"> flush0</b> method to
             * perform the actual flushing operation.  </p>
             *
             * @param  out
             *         The output byte buffer
             *
             * @return  A coder-result object, either <b style="color: orange;"> CoderResult.UNDERFLOW</b> or
             *          <b style="color: orange;"> CoderResult.OVERFLOW</b>
             *
             * @throws  IllegalStateException
             *          If the previous step of the current encoding operation was an
             *          invocation neither of the <b style="color: orange;"> flush</b> method nor of
             *          the three-argument <b style="color: orange;"> encode</b> method
             *          with a value of <b> true</b> for the <b> endOfInput</b>
             *          parameter
             */
            CoderResult flush(io::ByteBuffer& out);

        protected:
            /**
             * Flushes this encoder.
             *
             * <p> The default implementation of this method does nothing, and always
             * returns <b style="color: orange;">CoderResult.UNDERFLOW</b>.  This method should be overridden
             * by encoders that may need to write final characters to the output buffer
             * once the entire input sequence has been read. </p>
             *
             * @param  out
             *         The output character buffer
             *
             * @return  A coder-result object, either <b style="color: orange;">CoderResult.UNDERFLOW</b> or
             *          <b style="color: orange;">CoderResult.OVERFLOW</b>
             */
            virtual CoderResult flush0(io::ByteBuffer& out);

        public:

            /**
             * Resets this encoder, clearing any internal state.
             *
             * <p> This method resets charset-independent state and also invokes the
             * <b style="color: orange;"> reset0</b> method in order to perform any
             * charset-specific reset actions.  </p>
             *
             * @return  This encoder
             *
             */
            CharsetEncoder &reset();
        protected:

            /**
             * Resets this encoder, clearing any charset-specific internal state.
             *
             * <p> The default implementation of this method does nothing.  This method
             * should be overridden by encoders that maintain internal state.  </p>
             */
            virtual void reset0();

            /**
             * Encodes one or more characters into one or more bytes.
             *
             * <p> This method encapsulates the basic encoding loop, encoding as many
             * characters as possible until it either runs out of input, runs out of room
             * in the output buffer, or encounters an encoding error.  This method is
             * invoked by the <b style="color: orange;"> encode</b> method, which handles result
             * interpretation and error recovery.
             *
             * <p> The buffers are read from, and written to, starting at their current
             * positions.  At most <b style="color: orange;"> in.remaining()</b> characters
             * will be read, and at most <b style="color: orange;"> out.remaining()</b>
             * bytes will be written.  The buffers' positions will be advanced to
             * reflect the characters read and the bytes written, but their marks and
             * limits will not be modified.
             *
             * <p> This method returns a <b style="color: orange;"> CoderResult</b> object to describe its
             * reason for termination, in the same manner as the <b style="color: orange;"> encode</b>
             * method.  Most implementations of this method will handle encoding errors
             * by returning an appropriate result object for interpretation by the
             * <b style="color: orange;"> encode</b> method.  An optimized implementation may instead
             * examine the relevant error action and implement that action itself.
             *
             * <p> An implementation of this method may perform arbitrary lookahead by
             * returning <b style="color: orange;"> CoderResult.UNDERFLOW</b> until it receives sufficient
             * input.  </p>
             *
             * @param  in
             *         The input character buffer
             *
             * @param  out
             *         The output byte buffer
             *
             * @return  A coder-result object describing the reason for termination
             */
            virtual CoderResult encodeLoop(io::CharBuffer &in, io::ByteBuffer& out) = 0;

        public:

            /**
             * Convenience method that encodes the remaining content of a single input
             * character buffer into a newly-allocated byte buffer.
             *
             * <p> This method implements an entire <a href="">encoding
             * operation</a>; that is, it resets this encoder, then it encodes the
             * characters in the given character buffer, and finally it flushes this
             * encoder.  This method should therefore not be invoked if an encoding
             * operation is already in progress.  </p>
             *
             * @param  in
             *         The input character buffer
             *
             * @return A newly-allocated byte buffer containing the result of the
             *         encoding operation.  The buffer's position will be zero and its
             *         limit will follow the last byte written.
             *
             * @throws  IllegalStateException
             *          If an encoding operation is already in progress
             *
             * @throws  MalformedInputException
             *          If the character sequence starting at the input buffer's current
             *          position is not a legal sixteen-bit Unicode sequence and the current malformed-input action
             *          is <b style="color: orange;"> CodingErrorAction.REPORT</b>
             *
             * @throws  UnmappableCharacterException
             *          If the character sequence starting at the input buffer's current
             *          position cannot be mapped to an equivalent byte sequence and
             *          the current unmappable-character action is <b style="color: orange;"> CodingErrorAction.REPORT</b>
             */
            io::ByteBuffer& encode(io::CharBuffer& in);

        private:

            gbool canEncode(io::CharBuffer& cb);

        public:

            /**
             * Tells whether or not this encoder can encode the given character.
             *
             * <p> This method returns <b> false</b> if the given character is a
             * surrogate character; such characters can be interpreted only when they
             * are members of a pair consisting of a high surrogate followed by a low
             * surrogate.  The <b style="color: orange;"> canEncode(CharSequence)</b> method may be used to
             * test whether or not a character sequence can be encoded.
             *
             * <p> This method may modify this encoder's state; it should therefore not
             * be invoked if an <a href="">encoding operation</a> is already in
             * progress.
             *
             * <p> The default implementation of this method is not very efficient; it
             * should generally be overridden to improve performance.  </p>
             *
             * @param   c
             *          The given character
             *
             * @return  <b> true</b> if, and only if, this encoder can encode
             *          the given character
             *
             * @throws  IllegalStateException
             *          If an encoding operation is already in progress
             */
             virtual gbool canEncode(gchar c);

            /**
             * Tells whether or not this encoder can encode the given character
             * sequence.
             *
             * <p> If this method returns <b> false</b> for a particular character
             * sequence then more information about why the sequence cannot be encoded
             * may be obtained by performing a full <a href="">encoding
             * operation</a>.
             *
             * <p> This method may modify this encoder's state; it should therefore not
             * be invoked if an encoding operation is already in progress.
             *
             * <p> The default implementation of this method is not very efficient; it
             * should generally be overridden to improve performance.  </p>
             *
             * @param   cs
             *          The given character sequence
             *
             * @return  <b> true</b> if, and only if, this encoder can encode
             *          the given character without throwing any exceptions and without
             *          performing any replacements
             *
             * @throws  IllegalStateException
             *          If an encoding operation is already in progress
             */
             gbool canEncode(const CharSequence& cs);
        };

    } // core
} // charset

#endif //CORE23_CHARSETENCODER_H
