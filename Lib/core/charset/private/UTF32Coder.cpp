//
// Created by T.N.Brunshweeck on 16/01/2024.
//

#include "UTF32Coder.h"

namespace core {
    namespace charset {
        UTF32Coder::Decoder::Decoder(const Charset &cs, gint bo) :
                CharsetDecoder(cs, 0.25F, 1.0F), currentBO(NONE), expectedBO(bo) {}

        gint UTF32Coder::Decoder::getCP(ByteBuffer &src) const {
            return (currentBO == BIG)
                   ? (((src.get() & 0xff) << 24) |
                      ((src.get() & 0xff) << 16) |
                      ((src.get() & 0xff) << 8) |
                      (src.get() & 0xff))
                   : ((src.get() & 0xff) |
                      ((src.get() & 0xff) << 8) |
                      ((src.get() & 0xff) << 16) |
                      ((src.get() & 0xff) << 24));
        }

        CoderResult UTF32Coder::Decoder::decodeLoop(ByteBuffer &src, CharBuffer &dst) {
            if (src.remaining() < 4)
                return CoderResult::UNDERFLOW;
            gint mark = src.position();
            gint cp;
            try {
                if (currentBO == NONE) {
                    cp = ((src.get() & 0xff) << 24) |
                         ((src.get() & 0xff) << 16) |
                         ((src.get() & 0xff) << 8) |
                         (src.get() & 0xff);
                    if (cp == BOM_BIG && expectedBO != LITTLE) {
                        currentBO = BIG;
                        mark += 4;
                    } else if (cp == BOM_LITTLE && expectedBO != BIG) {
                        currentBO = LITTLE;
                        mark += 4;
                    } else {
                        if (expectedBO == NONE)
                            currentBO = BIG;
                        else
                            currentBO = expectedBO;
                        src.setPosition(mark);
                    }
                }
                while (src.remaining() >= 4) {
                    cp = getCP(src);
                    if (Character::isBMP(cp)) {
                        if (!dst.hasRemaining())
                            return CoderResult::OVERFLOW;
                        mark += 4;
                        dst.put((gchar) cp);
                    } else if (Character::isValidCodePoint(cp)) {
                        if (dst.remaining() < 2)
                            return CoderResult::OVERFLOW;
                        mark += 4;
                        dst.put(Character::highSurrogate(cp));
                        dst.put(Character::lowSurrogate(cp));
                    } else {
                        return CoderResult::malformedForLength(4);
                    }
                }
                src.setPosition(mark);
                return CoderResult::UNDERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(mark);
                th.throws();
            }
        }

        void UTF32Coder::Decoder::reset0() {
            currentBO = NONE;
        }

        Object &UTF32Coder::Decoder::clone() const {
            return charset().decoder();
        }

        void UTF32Coder::Encoder::put(gint cp, ByteBuffer &dst) const {
            if (byteOrder == BIG) {
                dst.put((gbyte) (cp >> 24));
                dst.put((gbyte) (cp >> 16));
                dst.put((gbyte) (cp >> 8));
                dst.put((gbyte) cp);
            } else {
                dst.put((gbyte) cp);
                dst.put((gbyte) (cp >> 8));
                dst.put((gbyte) (cp >> 16));
                dst.put((gbyte) (cp >> 24));
            }
        }

        CoderResult UTF32Coder::Encoder::encodeLoop(CharBuffer &src, ByteBuffer &dst) {
            gint mark = src.position();
            if (!doneBOM && src.hasRemaining()) {
                if (dst.remaining() < 4)
                    return CoderResult::OVERFLOW;
                put(BOM_BIG, dst);
                doneBOM = true;
            }
            try {
                while (src.hasRemaining()) {
                    gchar c = src.get();
                    if (!Character::isSurrogate(c)) {
                        if (dst.remaining() < 4)
                            return CoderResult::OVERFLOW;
                        mark++;
                        put(c, dst);
                    } else if (Character::isHighSurrogate(c)) {
                        if (!src.hasRemaining())
                            return CoderResult::UNDERFLOW;
                        gchar low = src.get();
                        if (Character::isLowSurrogate(low)) {
                            if (dst.remaining() < 4)
                                return CoderResult::OVERFLOW;
                            mark += 2;
                            put(Character::joinSurrogates(c, low), dst);
                        } else {
                            return CoderResult::malformedForLength(1);
                        }
                    } else {
                        // assert Character::isLowSurrogate(c);
                        return CoderResult::malformedForLength(1);
                    }
                }
                src.setPosition(mark);
                return CoderResult::UNDERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(mark);
                th.throws();
            }
        }

        void UTF32Coder::Encoder::reset0() {
            doneBOM = !doBOM;
        }

        Object &UTF32Coder::Encoder::clone() const {
            return charset().encoder();
        }
    } // charset
} // core