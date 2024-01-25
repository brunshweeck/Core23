//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_CHARSETDECODER_H
#define CORE23_CHARSETDECODER_H

#include <core/String.h>
#include <core/charset/Charset.h>
#include <core/charset/CoderResult.h>
#include <core/io/ByteBuffer.h>
#include <core/io/CharBuffer.h>

namespace core {
    namespace charset {

        /**
         * An engine that can transform a sequence of bytes in a specific charset into a sequence of
         * sixteen-bit Unicode characters.
         *
         * <a id="steps"></a>
         *
         * <p> The input byte sequence is provided in a byte buffer or a series
         * of such buffers.  The output character sequence is written to a character buffer
         * or a series of such buffers.  A decoder should always be used by making
         * the following sequence of method invocations, hereinafter referred to as a
         * <i>decoding operation</i>:
         *
         * <ol>
         *
         *   <li><p> Reset the decoder via the <b style="color:orange;"> reset</b> method, unless it
         *   has not been used before; </p></li>
         *
         *   <li><p> Invoke the <b style="color:orange;"> decode</b> method zero or more times, as
         *   long as additional input may be available, passing <b> false</b> for the
         *   <b> endOfInput</b> argument and filling the input buffer and flushing the
         *   output buffer between invocations; </p></li>
         *
         *   <li><p> Invoke the <b style="color:orange;"> decode</b> method one final time, passing
         *   <b> true</b> for the <b> endOfInput</b> argument; and then </p></li>
         *
         *   <li><p> Invoke the <b style="color:orange;"> flush</b> method so that the decoder can
         *   flush any internal state to the output buffer. </p></li>
         *
         * </ol>
         *
         * Each invocation of the <b style="color:orange;"> decode</b> method will decode as many
         * bytes as possible from the input buffer, writing the resulting characters
         * to the output buffer.  The <b style="color:orange;"> decode</b> method returns when more
         * input is required, when there is not enough room in the output buffer, or
         * when a decoding error has occurred.  In each case a <b style="color:orange;"> CoderResult</b>
         * object is returned to describe the reason for termination.  An invoker can
         * examine this object and fill the input buffer, flush the output buffer, or
         * attempt to recover from a decoding error, as appropriate, and try again.
         *
         * <a id="ce"></a>
         *
         * <p> There are two general types of decoding errors.  If the input byte
         * sequence is not legal for this charset then the input is considered <i>malformed</i>.  If
         * the input byte sequence is legal but cannot be mapped to a valid
         * Unicode character then an <i>unmappable character</i> has been encountered.
         *
         * <a id="cae"></a>
         *
         * <p> How a decoding error is handled depends upon the action requested for
         * that type of error, which is described by an instance of the <b style="color:orange;"> 
         * CodingErrorAction</b> class.  The possible error actions are to
         * <b style="color: green;"> ignore</b> the erroneous input, <b style="color: green;"> report</b> the error to the invoker via
         * the returned <b style="color:orange;"> CoderResult</b> object, or <b style="color: green;"> replace</b> the erroneous input with the current value of the
         * replacement string.  The replacement
         *
         * has the initial value <code>"&#92;uFFFD"</code>;
         *
         * its value may be changed via the <b style="color:orange;"> replaceWith</b> method.
         *
         * <p> The default action for malformed-input and unmappable-character errors
         * is to <b style="color: green;"> report</b> them.  The
         * malformed-input error action may be changed via the <b style="color:orange;"> onMalformedInput</b> method; the
         * unmappable-character action may be changed via the <b style="color:orange;"> onUnmappableCharacter</b> method.
         *
         * <p> This class is designed to handle many of the details of the decoding
         * process, including the implementation of error actions.  A decoder for a
         * specific charset, which is a concrete subclass of this class, need only
         * implement the abstract <b style="color:orange;"> decodeLoop</b> method, which
         * encapsulates the basic decoding loop.  A subclass that maintains internal
         * state should, additionally, override the <b style="color:orange;"> flush0 </b> and
         * <b style="color:orange;"> reset0 </b> methods.
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
         * @see CharsetEncoder
         */
        class CharsetDecoder : public Object {
        private:
            const Charset &cs;
            gfloat acpb;
            gfloat mcpb;
            String repl;

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
             * Initializes a new decoder.  The new decoder will have the given
             * chars-per-byte and replacement values.
             *
             * @param  cs
             *         The charset that created this decoder
             *
             * @param  averageCharsPerByte
             *         A positive float value indicating the expected number of
             *         characters that will be produced for each input byte
             *
             * @param  maxCharsPerByte
             *         A positive float value indicating the maximum number of
             *         characters that will be produced for each input byte
             *
             * @param  replacement
             *         The initial replacement; must not be <b> null</b>, must have
             *         non-zero length, must not be longer than maxCharsPerByte,
             *         and must be <b style="color: green;"> legal</b>
             *
             * @throws  IllegalArgumentException
             *          If the preconditions on the parameters do not hold
             */
            CORE_EXPLICIT CharsetDecoder(const Charset &cs,
                                         gfloat averageCharsPerByte,
                                         gfloat maxCharsPerByte,
                                         String replacement);

            /**
             * Initializes a new decoder.  The new decoder will have the given
             * chars-per-byte values and its replacement will be the
             * string <code>"&#92;uFFFD"</code>.
             *
             * @param  cs
             *         The charset that created this decoder
             *
             * @param  averageCharsPerByte
             *         A positive float value indicating the expected number of
             *         characters that will be produced for each input byte
             *
             * @param  maxCharsPerByte
             *         A positive float value indicating the maximum number of
             *         characters that will be produced for each input byte
             *
             * @throws  IllegalArgumentException
             *          If the preconditions on the parameters do not hold
             */
            CORE_EXPLICIT CharsetDecoder(const Charset &cs,
                                         gfloat averageCharsPerByte,
                                         gfloat maxCharsPerByte);

        public:
            /**
             * Returns the charset that created this decoder.
             *
             * @return  This decoder's charset
             */
            const Charset &charset() const;

            /**
             * Returns this decoder's replacement value.
             *
             * @return  This decoder's current replacement,
             *          which is never empty
             */
            String replacement() const;

            /**
             * Changes this decoder's replacement value.
             *
             * <p> This method invokes the <b style="color: orange;"> replaceWith0</b>
             * method, passing the new replacement, after checking that the new
             * replacement is acceptable.  </p>
             *
             * @param  newReplacement  The new replacement; must not be
             *         <b> null</b>, must have non-zero length,

             *         and must not be longer than the value returned by the
             *         <b style="color: orange;"> maxCharsPerByte</b> method
             *
             * @return  This decoder
             *
             * @throws  IllegalArgumentException
             *          If the preconditions on the parameter do not hold
             */
            CharsetDecoder &replaceWith(String newReplacement);

        protected:

            /**
             * Reports a change to this decoder's replacement value.
             *
             * <p> The default implementation of this method does nothing.  This method
             * should be overridden by decoders that require notification of changes to
             * the replacement.  </p>
             *
             * @param  newReplacement    The replacement value
             */
            virtual void replaceWith0(const String &newReplacement);

        public:

            /**
             * Returns this decoder's current action for malformed-input errors.
             *
             * @return The current malformed-input action
             */
            Charset::CodingErrorAction malformedInputAction() const;

            /**
             * Changes this decoder's action for malformed-input errors.
             *
             * <p> This method invokes the <b style="color: orange;"> onMalformedInput0</b>
             * method, passing the new action.  </p>
             *
             * @param  newAction  The new action; must not be <b> null</b>
             *
             * @return  This decoder
             *
             * @throws IllegalArgumentException
             *         If the precondition on the parameter does not hold
             */
            CharsetDecoder &onMalformedInput(Charset::CodingErrorAction newAction);

        protected:

            /**
             * Reports a change to this decoder's malformed-input action.
             *
             * <p> The default implementation of this method does nothing.  This method
             * should be overridden by decoders that require notification of changes to
             * the malformed-input action.  </p>
             *
             * @param  newAction  The new action
             */
            virtual void onMalformedInput0(Charset::CodingErrorAction newAction);

        public:

            /**
             * Returns this decoder's current action for unmappable-character errors.
             *
             * @return The current unmappable-character action
             */
            Charset::CodingErrorAction unmappableCharacterAction() const;

            /**
             * Changes this decoder's action for unmappable-character errors.
             *
             * <p> This method invokes the <b style="color: orange;"> onUnmappableCharacter0</b> method,
             * passing the new action.  </p>
             *
             * @param  newAction  The new action; must not be <b> null</b>
             *
             * @return  This decoder
             *
             * @throws IllegalArgumentException
             *         If the precondition on the parameter does not hold
             */
            CharsetDecoder &onUnmappableCharacter(Charset::CodingErrorAction newAction);

        protected:

            /**
             * Reports a change to this decoder's unmappable-character action.
             *
             * <p> The default implementation of this method does nothing.  This method
             * should be overridden by decoders that require notification of changes to
             * the unmappable-character action.  </p>
             *
             * @param  newAction  The new action
             */
            virtual void onUnmappableCharacter0(Charset::CodingErrorAction newAction);

        public:

            /**
             * Returns the average number of characters that will be produced for each
             * byte of input.  This heuristic value may be used to estimate the size
             * of the output buffer required for a given input sequence.
             *
             * @return  The average number of characters produced
             *          per byte of input
             */
            gfloat averageCharsPerByte() const;

            /**
             * Returns the maximum number of characters that will be produced for each
             * byte of input.  This value may be used to compute the worst-case size
             * of the output buffer required for a given input sequence. This value
             * accounts for any necessary content-independent prefix or suffix
             * characters.
             *
             * @return  The maximum number of characters that will be produced per
             *          byte of input
             */
            gfloat maxCharsPerByte() const;

            /**
             * Decodes as many bytes as possible from the given input buffer,
             * writing the results to the given output buffer.
             *
             * <p> The buffers are read from, and written to, starting at their current
             * positions.  At most <b style="color: orange;"> in.remaining()</b> bytes
             * will be read and at most <b style="color: orange;"> out.remaining()</b>
             * characters will be written.  The buffers' positions will be advanced to
             * reflect the bytes read and the characters written, but their marks and
             * limits will not be modified.
             *
             * <p> In addition to reading bytes from the input buffer and writing
             * characters to the output buffer, this method returns a <b style="color: orange;">CoderResult</b>
             * object to describe its reason for termination:
             *
             * <ul>
             *
             *   <li><p> <b style="color: orange;">CoderResult.UNDERFLOW</b> indicates that as much of the
             *   input buffer as possible has been decoded.  If there is no further
             *   input then the invoker can proceed to the next step of the
             *   <a href="">decoding operation</a>.  Otherwise this method
             *   should be invoked again with further input.  </p></li>
             *
             *   <li><p> <b style="color: orange;">CoderResult.OVERFLOW</b> indicates that there is
             *   insufficient space in the output buffer to decode any more bytes.
             *   This method should be invoked again with an output buffer that has
             *   more <b style="color: green;"> remaining</b> characters. This is
             *   typically done by draining any decoded characters from the output
             *   buffer.  </p></li>
             *
             *   <li><p> A <b style="color: green;"> malformed-input</b> result indicates
             *   that a malformed-input error has been detected.  The malformed bytes begin at the input
             *   buffer's (possibly incremented) position; the number of malformed
             *   bytes may be determined by invoking the result object's <b style="color: orange;"> 
             *   length</b> method.  This case applies only if the
             *   <b style="color: green;"> malformed action</b> of this decoder
             *   is <b style="color: orange;">CodingErrorAction.REPORT</b>; otherwise the malformed input
             *   will be ignored or replaced, as requested.  </p></li>
             *
             *   <li><p> An <b style="color: green;"> unmappable-character</b> result indicates that an
             *   unmappable-character error has been detected.  The bytes that
             *   decode the unmappable character begin at the input buffer's (possibly
             *   incremented) position; the number of such bytes may be determined
             *   by invoking the result object's <b style="color: orange;"> length</b>
             *   method.  This case applies only if the <b style="color: green;">
             *   unmappable action</b> of this decoder is <b style="color: orange;"> 
             *   CodingErrorAction.REPORT</b>; otherwise the unmappable character will be
             *   ignored or replaced, as requested.  </p></li>
             *
             * </ul>
             *
             * In any case, if this method is to be reinvoked in the same decoding
             * operation then care should be taken to preserve any bytes remaining
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
             * pass <b> true</b> so that any remaining undecoded input will be treated
             * as being malformed.
             *
             * <p> This method works by invoking the <b style="color: orange;"> decodeLoop</b>
             * method, interpreting its results, handling error conditions, and
             * reinvoking it as necessary.  </p>
             *
             *
             * @param  in
             *         The input byte buffer
             *
             * @param  out
             *         The output character buffer
             *
             * @param  endOfInput
             *         <b> true</b> if, and only if, the invoker can provide no
             *         additional input bytes beyond those in the given buffer
             *
             * @return  A coder-result object describing the reason for termination
             *
             * @throws  IllegalStateException
             *          If a decoding operation is already in progress and the previous
             *          step was an invocation neither of the <b style="color: orange;"> reset</b>
             *          method, nor of this method with a value of <b> false</b> for
             *          the <b> endOfInput</b> parameter, nor of this method with a
             *          value of <b> true</b> for the <b> endOfInput</b> parameter
             *          but a return value indicating an incomplete decoding operation
             *
             * @throws  CoderMalfunctionError
             *          If an invocation of the decodeLoop method threw
             *          an unexpected exception
             */
            CoderResult decode(io::ByteBuffer &in, io::CharBuffer &out, gbool endOfInput);

            /**
             * Flushes this decoder.
             *
             * <p> Some decoders maintain internal state and may need to write some
             * final characters to the output buffer once the overall input sequence has
             * been read.
             *
             * <p> Any additional output is written to the output buffer beginning at
             * its current position.  At most <b style="color: orange;">out.remaining()</b>
             * characters will be written.  The buffer's position will be advanced
             * appropriately, but its mark and limit will not be modified.
             *
             * <p> If this method completes successfully then it returns <b style="color: orange;"> 
             * CoderResult.UNDERFLOW</b>.  If there is insufficient room in the output
             * buffer then it returns <b style="color: orange;">CoderResult.OVERFLOW</b>.  If this happens
             * then this method must be invoked again, with an output buffer that has
             * more room, in order to complete the current <a href="">decoding
             * operation</a>.
             *
             * <p> If this decoder has already been flushed then invoking this method
             * has no effect.
             *
             * <p> This method invokes the <b style="color: orange;">flush0</b> method to
             * perform the actual flushing operation.  </p>
             *
             * @param  out
             *         The output character buffer
             *
             * @return  A coder-result object, either <b style="color: orange;">CoderResult.UNDERFLOW</b> or
             *          <b style="color: orange;">CoderResult.OVERFLOW</b>
             *
             * @throws  IllegalStateException
             *          If the previous step of the current decoding operation was an
             *          invocation neither of the <b style="color: orange;">flush</b> method nor of
             *          the three-argument <b style="color: orange;"> decode</b> method
             *          with a value of <b> true</b> for the <b> endOfInput</b>
             *          parameter
             */
            CoderResult flush(io::CharBuffer &out);

        protected:
            /**
             * Flushes this decoder.
             *
             * <p> The default implementation of this method does nothing, and always
             * returns <b style="color: orange;">CoderResult.UNDERFLOW</b>.  This method should be overridden
             * by decoders that may need to write final characters to the output buffer
             * once the entire input sequence has been read. </p>
             *
             * @param  out
             *         The output character buffer
             *
             * @return  A coder-result object, either <b style="color: orange;">CoderResult.UNDERFLOW</b> or
             *          <b style="color: orange;">CoderResult.OVERFLOW</b>
             */
            virtual CoderResult flush0(io::CharBuffer &out);

        public:

            /**
             * Resets this decoder, clearing any internal state.
             *
             * <p> This method resets charset-independent state and also invokes the
             * <b style="color: orange;"> reset0</b> method in order to perform any
             * charset-specific reset actions.  </p>
             *
             * @return  This decoder
             *
             */
            CharsetDecoder &reset();

        protected:

            /**
             * Resets this decoder, clearing any charset-specific internal state.
             *
             * <p> The default implementation of this method does nothing.  This method
             * should be overridden by decoders that maintain internal state.  </p>
             */
            virtual void reset0();

            /**
             * Decodes one or more bytes into one or more characters.
             *
             * <p> This method encapsulates the basic decoding loop, decoding as many
             * bytes as possible until it either runs out of input, runs out of room
             * in the output buffer, or encounters a decoding error.  This method is
             * invoked by the <b style="color: orange;">decode</b> method, which handles result
             * interpretation and error recovery.
             *
             * <p> The buffers are read from, and written to, starting at their current
             * positions.  At most <b style="color: orange;">in.remaining()</b> bytes
             * will be read, and at most <b style="color: orange;">out.remaining()</b>
             * characters will be written.  The buffers' positions will be advanced to
             * reflect the bytes read and the characters written, but their marks and
             * limits will not be modified.
             *
             * <p> This method returns a <b style="color: orange;">CoderResult</b> object to describe its
             * reason for termination, in the same manner as the <b style="color: orange;"> decode</b>
             * method.  Most implementations of this method will handle decoding errors
             * by returning an appropriate result object for interpretation by the
             * <b style="color: orange;"> decode</b> method.  An optimized implementation may instead
             * examine the relevant error action and implement that action itself.
             *
             * <p> An implementation of this method may perform arbitrary lookahead by
             * returning <b style="color: orange;">CoderResult.UNDERFLOW</b> until it receives sufficient
             * input.  </p>
             *
             * @param  in
             *         The input byte buffer
             *
             * @param  out
             *         The output character buffer
             *
             * @return  A coder-result object describing the reason for termination
             */
            virtual CoderResult decodeLoop(io::ByteBuffer &in, io::CharBuffer &out) = 0;

        public:

            /**
             * Convenience method that decodes the remaining content of a single input
             * byte buffer into a newly-allocated character buffer.
             *
             * <p> This method implements an entire <a href="">decoding
             * operation</a>; that is, it resets this decoder, then it decodes the
             * bytes in the given byte buffer, and finally it flushes this
             * decoder.  This method should therefore not be invoked if a decoding
             * operation is already in progress.  </p>
             *
             * @param  in
             *         The input byte buffer
             *
             * @return A newly-allocated character buffer containing the result of the
             *         decoding operation.  The buffer's position will be zero and its
             *         limit will follow the last character written.
             *
             * @throws  IllegalStateException
             *          If a decoding operation is already in progress
             *
             * @throws  MalformedInputException
             *          If the byte sequence starting at the input buffer's current
             *          position is not legal for this charset and the current malformed-input action
             *          is <b style="color: orange;">CodingErrorAction.REPORT</b>
             *
             * @throws  UnmappableCharacterException
             *          If the byte sequence starting at the input buffer's current
             *          position cannot be mapped to an equivalent character sequence and
             *          the current unmappable-character action is <b style="color: orange;"> 
             *          CodingErrorAction.REPORT</b>
             */
            io::CharBuffer &decode(io::ByteBuffer &in);


            /**
             * Tells whether or not this decoder implements an auto-detecting charset.
             *
             * <p> The default implementation of this method always returns
             * <b> false</b>; it should be overridden by auto-detecting decoders to
             * return <b> true</b>.  </p>
             *
             * @return  <b> true</b> if, and only if, this decoder implements an
             *          auto-detecting charset
             */
            virtual gbool isAutoDetecting() const;

            /**
             * Tells whether or not this decoder has yet detected a
             * charset&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> If this decoder implements an auto-detecting charset then at a
             * single point during a decoding operation this method may start returning
             * <b> true</b> to indicate that a specific charset has been detected in
             * the input byte sequence.  Once this occurs, the <b style="color: orange;">
             * detectedCharset</b> method may be invoked to retrieve the detected charset.
             *
             * <p> That this method returns <b> false</b> does not imply that no bytes
             * have yet been decoded.  Some auto-detecting decoders are capable of
             * decoding some, or even all, of an input byte sequence without fixing on
             * a particular charset.
             *
             * <p> The default implementation of this method always throws an <b style="color: orange;"> 
             * UnsupportedOperationException</b>; it should be overridden by
             * auto-detecting decoders to return <b> true</b> once the input charset
             * has been determined.  </p>
             *
             * @return  <b> true</b> if, and only if, this decoder has detected a
             *          specific charset
             *
             * @throws  UnsupportedOperationException
             *          If this decoder does not implement an auto-detecting charset
             */
            virtual gbool isCharsetDetected() const;

            /**
             * Retrieves the charset that was detected by this
             * decoder&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> If this decoder implements an auto-detecting charset then this
             * method returns the actual charset once it has been detected.  After that
             * point, this method returns the same value for the duration of the
             * current decoding operation.  If not enough input bytes have yet been
             * read to determine the actual charset then this method throws an <b style="color: orange;"> 
             * IllegalStateException</b>.
             *
             * <p> The default implementation of this method always throws an <b style="color: orange;"> 
             * UnsupportedOperationException</b>; it should be overridden by
             * auto-detecting decoders to return the appropriate value.  </p>
             *
             * @return  The charset detected by this auto-detecting decoder,
             *          or <b> null</b> if the charset has not yet been determined
             *
             * @throws  IllegalStateException
             *          If insufficient bytes have been read to determine a charset
             *
             * @throws  UnsupportedOperationException
             *          If this decoder does not implement an auto-detecting charset
             */
            virtual const Charset &detectedCharset() const;
        };

    } // core
} // charset

#endif //CORE23_CHARSETDECODER_H
