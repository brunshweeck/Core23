//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "CharsetDecoder.h"
#include <core/private/Unsafe.h>
#include <core/native/StringArray.h>
#include <core/charset/CoderMalfunctionError.h>
#include <core/io/BufferUnderflowException.h>


namespace core {
    namespace charset {

        using namespace native;
        using namespace io;

        CORE_ALIAS(CodingErrorAction, typename Charset::CodingErrorAction);

        CharsetDecoder::CharsetDecoder(const Charset &cs, gfloat averageCharsPerByte, gfloat maxCharsPerByte,
                                       String replacement) : cs(cs), acpb(averageCharsPerByte),
                                                             mcpb(maxCharsPerByte),
                                                             repl(Unsafe::moveInstance(replacement)) {
            // Use !(a > 0.0f) rather than (a <= 0.0f) to exclude NaN values
            if (!(averageCharsPerByte > 0.0F))
                IllegalArgumentException("Non-positive averageCharsPerByte")
                        .throws(__trace("core.charset.CharsetDecoder"));
            // Use !(a > 0.0f) rather than (a <= 0.0f) to exclude NaN values
            if (!(maxCharsPerByte > 0.0F))
                IllegalArgumentException("Non-positive maxCharsPerByte")
                        .throws(__trace("core.charset.CharsetDecoder"));
            if (averageCharsPerByte > maxCharsPerByte)
                IllegalArgumentException("averageCharsPerByte exceeds maxCharsPerByte")
                        .throws(__trace("core.charset.CharsetDecoder"));
        }

        CharsetDecoder::CharsetDecoder(const Charset &cs, gfloat avarageCharsPerByte, gfloat maxCharsPerByte) :
                CharsetDecoder(cs, avarageCharsPerByte, maxCharsPerByte, String::valueOf((gchar)0xFFFD)) {}

        const Charset &CharsetDecoder::charset() const {
            return cs;
        }

        String CharsetDecoder::replacement() const {
            return repl;
        }

        CharsetDecoder &CharsetDecoder::replaceWith(String newReplacement) {
            gint const len = newReplacement.length();
            if (len == 0)
                IllegalArgumentException("Empty replacement").throws(__trace("core.charset.CharsetDecoder"));;
            if ((gfloat) len > mcpb)
                IllegalArgumentException("Replacement too long").throws(__trace("core.charset.CharsetDecoder"));;

            replaceWith0(newReplacement);
            repl = Unsafe::moveInstance(newReplacement);
            return *this;
        }

        Charset::CodingErrorAction CharsetDecoder::malformedInputAction() const {
            return malformedAction;
        }

        CharsetDecoder &CharsetDecoder::onMalformedInput(Charset::CodingErrorAction newAction) {
            malformedAction = newAction;
            onMalformedInput0(newAction);
            return *this;
        }

        Charset::CodingErrorAction CharsetDecoder::unmappableCharacterAction() const {
            return unmappableAction;
        }

        CharsetDecoder &CharsetDecoder::onUnmappableCharacter(Charset::CodingErrorAction newAction) {
            unmappableAction = newAction;
            onUnmappableCharacter0(newAction);
            return *this;
        }

        gfloat CharsetDecoder::averageCharsPerByte() const { return acpb; }

        gfloat CharsetDecoder::maxCharsPerByte() const { return mcpb; }

        CharsetDecoder &CharsetDecoder::reset() {
            reset0();
            state = RESET;
            return *this;
        }

        namespace {
            const StringArray stateNames = StringArray::of("RESET", "CODING", "CODING_END", "FLUSHED");
        }

        CoderResult CharsetDecoder::decode(io::ByteBuffer &in, io::CharBuffer &out, gbool endOfInput) {
            CoderState const newState = endOfInput ? END : CODING;
            if ((state != RESET) && (state != CODING) && (!endOfInput || (state != END)))
                IllegalStateException("Current state = " + stateNames[state] + ", new state = " + stateNames[newState])
                        .throws(__trace("core.charset.CharsetDecoder"));
            state = newState;

            for (;;) {

                CoderResult cr = CoderResult::UNDERFLOW;
                try {
                    cr = decodeLoop(in, out);
                } catch (RuntimeException const &x) {
                    CoderMalfunctionError(x).throws(__trace("core.charset.CharsetDecoder"));
                }

                if (cr.isOverflow())
                    return cr;

                if (cr.isUnderflow()) {
                    if (endOfInput && in.hasRemaining()) {
                        cr = CoderResult::malformedForLength(in.remaining());
                        // Fall through to a malformed-input case
                    } else {
                        return cr;
                    }
                }

                CodingErrorAction action = Charset::REPORT;
                if (cr.isMalformed())
                    action = malformedAction;
                else if (cr.isUnmappable())
                    action = unmappableAction;
                else {
                    CORE_ASSERT_IF(false, cr.toString(), "core.charset.CharsetDecoder")
                };

                if (action == Charset::REPORT)
                    return cr;

                if (action == Charset::REPLACE) {
                    if (out.remaining() < repl.length())
                        return CoderResult::OVERFLOW;
                    out.put(repl);
                }

                if ((action == CodingErrorAction::IGNORE) || (action == CodingErrorAction::REPLACE)) {
                    // Skip erroneous input either way
                    in.setPosition(in.position() + cr.length());
                    continue;
                }

                CORE_ASSERT(false, "core.charset.CharsetDecoder");
            }
        }

        CoderResult CharsetDecoder::flush(io::CharBuffer &out) {
            if (state == END) {
                CoderResult cr = flush0(out);
                if (cr.isUnderflow())
                    state = FLUSHED;
                return cr;
            }

            if (state != FLUSHED)
                IllegalStateException(
                        "Current state = " + stateNames[state] + ", new state = " + stateNames[FLUSHED])
                        .throws(__trace("core.charset.CharsetDecoder"));

            return CoderResult::UNDERFLOW; // Already flushed
        }

        CoderResult CharsetDecoder::flush0(io::CharBuffer &out) {
            CORE_IGNORE(out);
            return CoderResult::UNDERFLOW;
        }

        void CharsetDecoder::reset0() {}

        io::CharBuffer &CharsetDecoder::decode(io::ByteBuffer &in) {
            gint n = (gint) ((gfloat) in.remaining() * averageCharsPerByte());
            CharBuffer &out = CharBuffer::allocate(n);

            if ((n == 0) && (in.remaining() == 0))
                return out;
            reset();
            for (;;) {
                CoderResult cr = CoderResult::UNDERFLOW;
                if (in.hasRemaining()) {
                    try {
                        cr = decode(in, out, true);
                    } catch (const Exception &ex) {
                        ex.throws(__trace("core.charset.CharsetDecoder"));
                    }
                }
                if (cr.isUnderflow())
                    cr = flush(out);

                if (cr.isUnderflow())
                    break;
                try{
                    if (cr.isOverflow()) {
                        n = 2 * n + 1;    // Ensure progress; n might be 0!
                        CharBuffer &o = CharBuffer::allocate(n);
                        out.flip();
                        o.put(out);
                        out = o;
                        continue;
                    }
                    cr.throwException();
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.charset.CharsetDecoder"));
                }
            }
            out.flip();
            return out;
        }

        gbool CharsetDecoder::isAutoDetecting() const {
            return false;
        }

        gbool CharsetDecoder::isCharsetDetected() const {
            UnsupportedOperationException().throws(__trace("core.charset.CharsetDecoder"));
        }

        const Charset &CharsetDecoder::detectedCharset() const {
            UnsupportedOperationException().throws(__trace("core.charset.CharsetDecoder"));
        }

        void CharsetDecoder::onUnmappableCharacter0(Charset::CodingErrorAction newAction) {}

        void CharsetDecoder::onMalformedInput0(Charset::CodingErrorAction newAction) {}

        void CharsetDecoder::replaceWith0(const String &newReplacement) {}
    } // core
} // charset
