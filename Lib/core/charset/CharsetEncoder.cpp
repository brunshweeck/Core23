//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "CharsetEncoder.h"
#include <core/private/Unsafe.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CoderMalfunctionError.h>
#include <core/charset/CharacterCodingException.h>
#include <core/native/StringArray.h>

namespace core {
    namespace charset {

        using namespace native;
        using namespace io;

        CharsetEncoder::CharsetEncoder(
                const Charset &cs, gfloat averageBytesPerChar, gfloat maxBytesPerChar, ByteArray replacement) :
                cs(cs), abpc(averageBytesPerChar), mbpc(maxBytesPerChar), repl(replacement) {
            // Use !(a > 0.0F) rather than (a <= 0.0F) to exclude NaN values
            if (!(averageBytesPerChar > 0.0F))
                IllegalArgumentException("Non-positive averageBytesPerChar")
                        .throws(__trace("core.charset.CharsetEncoder"));
            // Use !(a > 0.0f) rather than (a <= 0.0f) to exclude NaN values
            if (!(maxBytesPerChar > 0.0F))
                IllegalArgumentException("Non-positive maxBytesPerChar")
                        .throws(__trace("core.charset.CharsetEncoder"));
            if (averageBytesPerChar > maxBytesPerChar)
                IllegalArgumentException("averageBytesPerChar exceeds maxBytesPerChar")
                        .throws(__trace("core.charset.CharsetEncoder"));
            replaceWith(replacement);
        }

        CharsetEncoder::CharsetEncoder(const Charset &cs, gfloat averagebytesPerChar, gfloat maxBytesPerChar) :
                CharsetEncoder(cs, averagebytesPerChar, maxBytesPerChar, ByteArray::of((gbyte) '?')) {}

        const Charset &CharsetEncoder::charset() const {
            return cs;
        }

        ByteArray CharsetEncoder::replacement() const {
            return repl;
        }

        CharsetEncoder &CharsetEncoder::replaceWith(ByteArray newReplacement) {
            gint const len = newReplacement.length();
            if (len == 0)
                IllegalArgumentException("Empty replacement").throws(__trace("core.charset.CharsetEncoder"));
            if ((gfloat) len > mbpc)
                IllegalArgumentException("Replacement too long").throws(__trace("core.charset.CharsetEncoder"));
            if (!isLegalReplacement(newReplacement))
                IllegalArgumentException("Illegal replacement").throws(__trace("core.charset.CharsetEncoder"));
            repl = Unsafe::moveInstance(newReplacement);
            replaceWith0(repl);
            return *this;
        }

        void CharsetEncoder::replaceWith0(const ByteArray &newReplacement) {}

        Charset::CodingErrorAction CharsetEncoder::malformedInputAction() const {
            return malformedAction;
        }

        CharsetEncoder &CharsetEncoder::onMalformedInput(Charset::CodingErrorAction newAction) {
            malformedAction = newAction;
            onMalformedInput0(newAction);
            return *this;
        }

        gbool CharsetEncoder::isLegalReplacement(const ByteArray &replacement) const {
            Cache cache = cachedDecoder;
            Cache dec = null;
            if ((cache == null) || ((dec = cache) == null)) {
                dec = &charset().decoder();
                dec->onMalformedInput(Charset::REPORT);
                dec->onUnmappableCharacter(Charset::REPORT);
                (Cache &) cachedDecoder = dec;
            } else {
                dec->reset();
            }
            ByteBuffer &bb = ByteBuffer::wrap((ByteArray &) replacement);
            CharBuffer &cb = CharBuffer::allocate((gint) ((gfloat) bb.remaining() * dec->maxCharsPerByte()));
            CoderResult const cr = dec->decode(bb, cb, true);
            return !cr.isError();
        }

        void CharsetEncoder::onMalformedInput0(Charset::CodingErrorAction newAction) {}

        Charset::CodingErrorAction CharsetEncoder::unmappableCharacterAction() const {
            return unmappableAction;
        }

        CharsetEncoder &CharsetEncoder::onUnmappableCharacter(Charset::CodingErrorAction newAction) {
            unmappableAction = newAction;
            onUnmappableCharacter0(newAction);
            return *this;
        }

        void CharsetEncoder::onUnmappableCharacter0(Charset::CodingErrorAction newAction) {}

        gfloat CharsetEncoder::averageBytesPerChar() const { return abpc; }

        gfloat CharsetEncoder::maxCharsPerByte() const { return mbpc; }

        namespace {
            StringArray stateNames = StringArray::of("RESET", "CODING", "CODING_END", "FLUSHED");
        }

        CoderResult CharsetEncoder::encode(CharBuffer &in, ByteBuffer &out, gbool endOfInput) {
            CoderState const newState = endOfInput ? END : CODING;
            if ((state != RESET) && (state != CODING) && (!endOfInput || (state != END)))
                IllegalStateException("Current state = " + stateNames[state] + ", new state = " + stateNames[newState])
                        .throws(__trace("core.charset.CharsetEncoder"));
            state = newState;

            for (;;) {

                CoderResult cr = CoderResult::UNDERFLOW;
                try {
                    cr = encodeLoop(in, out);
                } catch (RuntimeException const &x) {
                    CoderMalfunctionError(x).throws(__trace("core.charset.CharsetEncoder"));
                }

                if (cr.isOverflow())
                    return cr;

                if (cr.isUnderflow()) {
                    if (endOfInput && in.hasRemaining()) {
                        cr = CoderResult::malformedForLength(in.remaining());
                        // Fall through to malformed-input case
                    } else {
                        return cr;
                    }
                }

                Charset::CodingErrorAction action = Charset::REPORT;
                if (cr.isMalformed())
                    action = malformedAction;
                else if (cr.isUnmappable())
                    action = unmappableAction;
                else {
                    CORE_RAISE(false, cr.toString(), __ctrace());
                }

                if (action == Charset::REPORT)
                    return cr;

                if (action == Charset::REPLACE) {
                    if (out.remaining() < repl.length())
                        return CoderResult::OVERFLOW;
                    out.put(repl);
                }

                if ((action == Charset::IGNORE)
                    || (action == Charset::REPLACE)) {
                    // Skip erroneous input either way
                    in.setPosition(in.position() + cr.length());
                    continue;
                }

                CORE_ASSERT(false, __ctrace())
            }
        }

        CoderResult CharsetEncoder::flush(ByteBuffer &out) {
            if (state == END) {
                CoderResult cr = flush0(out);
                if (cr.isUnderflow())
                    state = FLUSHED;
                return cr;
            }

            if (state != FLUSHED)
                IllegalStateException("Current state = " + stateNames[state] + ", new state = " + stateNames[FLUSHED])
                        .throws(__trace("core.charset.CharsetEncoder"));

            return CoderResult::UNDERFLOW; // Already flushed
        }

        CoderResult CharsetEncoder::flush0(ByteBuffer & /*out*/) {
            return CoderResult::UNDERFLOW;
        }

        CharsetEncoder &CharsetEncoder::reset() {
            reset0();
            state = RESET;
            return *this;
        }

        void CharsetEncoder::reset0() {}

        io::ByteBuffer &CharsetEncoder::encode(CharBuffer &in) {
            try {
                int n = (int) ((gfloat) in.remaining() * averageBytesPerChar());
                ByteBuffer &out = ByteBuffer::allocate(n);

                if ((n == 0) && (in.remaining() == 0))
                    return out;
                reset();
                for (;;) {
                    CoderResult cr = in.hasRemaining() ? encode(in, out, true) : CoderResult::UNDERFLOW;
                    if (cr.isUnderflow())
                        cr = flush(out);

                    if (cr.isUnderflow())
                        break;
                    if (cr.isOverflow()) {
                        n = 2 * n + 1;    // Ensure progress; n might be 0!
                        ByteBuffer &o = ByteBuffer::allocate(n);
                        out.flip();
                        o.put(out);
                        out = o;
                        continue;
                    }
                    cr.throwException();
                }
                out.flip();
                return out;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.charset.CharsetEncoder"));
            }
        }

        gbool CharsetEncoder::canEncode(CharBuffer &cb) {
            if (state == FLUSHED)
                reset();
            else if (state != RESET)
                IllegalStateException("Current state = " + stateNames[state] + ", new state = " + stateNames[CODING])
                        .throws(__trace("core.charset.CharsetEncoder"));
            Charset::CodingErrorAction const ma = malformedInputAction();
            Charset::CodingErrorAction const ua = unmappableCharacterAction();
            try {
                onMalformedInput(Charset::REPORT);
                onUnmappableCharacter(Charset::REPORT);
                encode(cb);
                onMalformedInput(ma);
                onUnmappableCharacter(ua);
                reset();
            } catch (CharacterCodingException const& x) {
                onMalformedInput(ma);
                onUnmappableCharacter(ua);
                reset();
                return false;
            }
            return true;
        }

        gbool CharsetEncoder::canEncode(gchar c) {
            CharBuffer& cb = CharBuffer::allocate(1);
            cb.put(c);
            cb.flip();
            gbool const b = canEncode(cb);
            Unsafe::destroyInstance(cb);
            return b;
        }

        gbool CharsetEncoder::canEncode(const CharSequence &csq) {
            try {
                if (Class<CharBuffer>::hasInstance(csq)) {
                    CharBuffer &cb = ((const CharBuffer &) csq).duplicate();
                    gbool const b = canEncode(cb);
                    Unsafe::destroyInstance(cb);
                    return b;
                } else {
                    CharBuffer &cb = CharBuffer::wrap(csq);
                    gbool const b = canEncode(cb);
                    Unsafe::destroyInstance(cb);
                    return b;
                }
            } catch (const Exception &ex) {
                ex.throws(__trace("core.charset.CharsetEncoder"));
            }
        }

    } // core
} // charset
