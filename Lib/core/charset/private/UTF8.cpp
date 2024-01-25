//
// Created by T.N.Brunshweeck on 14/01/2024.
//

#include "UTF8.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        using namespace native;

        UTF8 UTF8::INSTANCE{};

        UTF8::UTF8() : Unicode("UTF-8"_S, StringArray::of("UTF8", "unicode-1-1-utf-8")) {}

        CharsetDecoder &UTF8::decoder() const {
            return Unsafe::allocateInstance<Decoder>(*this);
        }

        CharsetEncoder &UTF8::encoder() const {
            return Unsafe::allocateInstance<Encoder>(*this);
        }

        void UTF8::updatePositions(Buffer &src, gint sp, Buffer &dst, gint dp) {
            src.setPosition(sp - src.arrayOffset());
            dst.setPosition(dp - dst.arrayOffset());
        }

        Object &UTF8::clone() const {
            return INSTANCE;
        }

        UTF8::Decoder::Decoder(const Charset &cs) : CharsetDecoder(cs, 1.0F, 1.0F) {}

        gbool UTF8::Decoder::isNotContinuation(gint b) {
            return (b & 0xc0) != 0x80;
        }

        gbool UTF8::Decoder::isMalformed3(gint b1, gint b2, gint b3) {
            return (b1 == (gbyte) 0xe0 && (b2 & 0xe0) == 0x80) ||
                   (b2 & 0xc0) != 0x80 || (b3 & 0xc0) != 0x80;
        }

        gbool UTF8::Decoder::isMalformed3_2(gint b1, gint b2) {
            return (b1 == (gbyte) 0xe0 && (b2 & 0xe0) == 0x80) ||
                   (b2 & 0xc0) != 0x80;
        }

        gbool UTF8::Decoder::isMalformed4(gint b2, gint b3, gint b4) {
            return (b2 & 0xc0) != 0x80 || (b3 & 0xc0) != 0x80 ||
                   (b4 & 0xc0) != 0x80;
        }

        gbool UTF8::Decoder::isMalformed4_2(gint b1, gint b2) {
            return (b1 == 0xf0 && (b2 < 0x90 || b2 > 0xbf)) ||
                   (b1 == 0xf4 && (b2 & 0xf0) != 0x80) ||
                   (b2 & 0xc0) != 0x80;
        }

        gbool UTF8::Decoder::isMalformed4_3(gint b3) {
            return (b3 & 0xc0) != 0x80;
        }

        CoderResult UTF8::Decoder::malformedN(ByteBuffer &src, gint nb) {
            gint b1;
            gint b2;
            switch (nb) {
                case 1:
                case 2:                    // always 1
                    return CoderResult::malformedForLength(1);
                case 3:
                    b1 = src.get();
                    b2 = src.get();    // no need to look up b3
                    return CoderResult::malformedForLength(((b1 == (gbyte) 0xe0 && (b2 & 0xe0) == 0x80) ||
                                                            isNotContinuation(b2)) ? 1 : 2);
                case 4:  // we don't care the speed here
                    b1 = src.get() & 0xff;
                    b2 = src.get() & 0xff;
                    if (b1 > 0xf4 ||
                        (b1 == 0xf0 && (b2 < 0x90 || b2 > 0xbf)) ||
                        (b1 == 0xf4 && (b2 & 0xf0) != 0x80) ||
                        isNotContinuation(b2))
                        return CoderResult::malformedForLength(1);
                    if (isNotContinuation(src.get()))
                        return CoderResult::malformedForLength(2);
                    return CoderResult::malformedForLength(3);
                default:
                    CORE_ASSERT(false, "core.charset.UTF8");
            }
        }

        CoderResult UTF8::Decoder::malformed(ByteBuffer &src, gint sp, CharBuffer &dst, gint dp, gint nb) {
            src.setPosition(sp - src.arrayOffset());
            CoderResult cr = malformedN(src, nb);
            updatePositions(src, sp, dst, dp);
            return cr;
        }

        CoderResult UTF8::Decoder::malformed(ByteBuffer &src, gint mark, gint nb) {
            src.setPosition(mark);
            CoderResult cr = malformedN(src, nb);
            src.setPosition(mark);
            return cr;
        }

        CoderResult
        UTF8::Decoder::malformedForLength(ByteBuffer &src, gint sp, CharBuffer &dst, gint dp, gint malformedNB) {
            updatePositions(src, sp, dst, dp);
            return CoderResult::malformedForLength(malformedNB);
        }

        CoderResult UTF8::Decoder::malformedForLength(ByteBuffer &src, gint mark, gint malformedNB) {
            src.setPosition(mark);
            return CoderResult::malformedForLength(malformedNB);
        }

        CoderResult UTF8::Decoder::xflow(Buffer &src, gint sp, gint sl, Buffer &dst, gint dp, gint nb) {
            updatePositions(src, sp, dst, dp);
            return (nb == 0 || sl - sp < nb)
                   ? CoderResult::UNDERFLOW : CoderResult::OVERFLOW;
        }

        CoderResult UTF8::Decoder::xflow(Buffer &src, gint mark, gint nb) {
            src.setPosition(mark);
            return (nb == 0 || src.remaining() < nb)
                   ? CoderResult::UNDERFLOW : CoderResult::OVERFLOW;
        }

        CoderResult UTF8::Decoder::decodeArrayLoop(ByteBuffer &src, CharBuffer &dst) {
            // This method is optimized for ASCII input.
            ByteArray sa = src.array();
            gint const soff = src.arrayOffset();
            gint sp = soff + src.position();
            gint const sl = soff + src.limit();

            CharArray da = dst.array();
            gint const doff = dst.arrayOffset();
            gint dp = doff + dst.position();
            gint const dl = doff + dst.limit();

            gint const n = decodeASCII(sa, sp, da, dp, Math::min(sl - sp, dl - dp));
            sp += n;
            dp += n;

            while (sp < sl) {
                gint b1 = sa[sp];
                if (b1 >= 0) {
                    // 1 gbyte, 7 bits: 0xxxxxxx
                    if (dp >= dl)
                        return xflow(src, sp, sl, dst, dp, 1);
                    da[dp++] = (gchar) b1;
                    sp++;
                } else if ((b1 >> 5) == -2 && (b1 & 0x1e) != 0) {
                    // 2 bytes, 11 bits: 110xxxxx 10xxxxxx
                    //                   [C2..DF] [80..BF]
                    if (sl - sp < 2 || dp >= dl)
                        return xflow(src, sp, sl, dst, dp, 2);
                    gint const b2 = sa[sp + 1];
                    // Now we check the first gbyte of 2-gbyte sequence as
                    //     if ((b1 >> 5) == -2 && (b1 & 0x1e) != 0)
                    // no longer need to check b1 against c1 & c0 for
                    // malformed as we did in the previous version
                    //   (b1 & 0x1e) == 0x0 || (b2 & 0xc0) != 0x80;
                    // only need to check the second gbyte b2.
                    if (isNotContinuation(b2))
                        return malformedForLength(src, sp, dst, dp, 1);
                    da[dp++] = (gchar) (((b1 << 6) ^ b2)
                                        ^
                                        (((gbyte) 0xC0 << 6) ^
                                         ((gbyte) 0x80 << 0)));
                    sp += 2;
                } else if ((b1 >> 4) == -2) {
                    // 3 bytes, 16 bits: 1110xxxx 10xxxxxx 10xxxxxx
                    gint const srcRemaining = sl - sp;
                    if (srcRemaining < 3 || dp >= dl) {
                        if (srcRemaining > 1 && isMalformed3_2(b1, sa[sp + 1]))
                            return malformedForLength(src, sp, dst, dp, 1);
                        return xflow(src, sp, sl, dst, dp, 3);
                    }
                    gint const b2 = sa[sp + 1];
                    gint const b3 = sa[sp + 2];
                    if (isMalformed3(b1, b2, b3))
                        return malformed(src, sp, dst, dp, 3);
                    gchar const c = (gchar)
                            ((b1 << 12) ^
                             (b2 << 6) ^
                             (b3 ^
                              (((gbyte) 0xE0 << 12) ^
                               ((gbyte) 0x80 << 6) ^
                               ((gbyte) 0x80 << 0))));
                    if (Character::isSurrogate(c))
                        return malformedForLength(src, sp, dst, dp, 3);
                    da[dp++] = c;
                    sp += 3;
                } else if ((b1 >> 3) == -2) {
                    // 4 bytes, 21 bits: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                    gint const srcRemaining = sl - sp;
                    if (srcRemaining < 4 || dl - dp < 2) {
                        b1 &= 0xff;
                        if (b1 > 0xf4 ||
                            srcRemaining > 1 && isMalformed4_2(b1, sa[sp + 1] & 0xff))
                            return malformedForLength(src, sp, dst, dp, 1);
                        if (srcRemaining > 2 && isMalformed4_3(sa[sp + 2]))
                            return malformedForLength(src, sp, dst, dp, 2);
                        return xflow(src, sp, sl, dst, dp, 4);
                    }
                    gint const b2 = sa[sp + 1];
                    gint const b3 = sa[sp + 2];
                    gint const b4 = sa[sp + 3];
                    gint const uc = ((b1 << 18) ^
                              (b2 << 12) ^
                              (b3 << 6) ^
                              (b4 ^
                               (((gbyte) 0xF0 << 18) ^
                                ((gbyte) 0x80 << 12) ^
                                ((gbyte) 0x80 << 6) ^
                                ((gbyte) 0x80 << 0))));
                    if (isMalformed4(b2, b3, b4) ||
                        // shortest form check
                        !Character::isSupplementary(uc)) {
                        return malformed(src, sp, dst, dp, 4);
                    }
                    da[dp++] = Character::highSurrogate(uc);
                    da[dp++] = Character::lowSurrogate(uc);
                    sp += 4;
                } else
                    return malformed(src, sp, dst, dp, 1);
            }
            return xflow(src, sp, sl, dst, dp, 0);
        }

        CoderResult UTF8::Decoder::decodeBufferLoop(ByteBuffer &src, CharBuffer &dst) {
            gint mark = src.position();
            gint const limit = src.limit();
            while (mark < limit) {
                gint b1 = src.get();
                if (b1 >= 0) {
                    // 1 gbyte, 7 bits: 0xxxxxxx
                    if (dst.remaining() < 1)
                        return xflow(src, mark, 1); // overflow
                    dst.put((gchar) b1);
                    mark++;
                } else if ((b1 >> 5) == -2 && (b1 & 0x1e) != 0) {
                    // 2 bytes, 11 bits: 110xxxxx 10xxxxxx
                    if (limit - mark < 2 || dst.remaining() < 1)
                        return xflow(src, mark, 2);
                    gint const b2 = src.get();
                    if (isNotContinuation(b2))
                        return malformedForLength(src, mark, 1);
                    dst.put((gchar) (((b1 << 6) ^ b2)
                                     ^
                                     (((gbyte) 0xC0 << 6) ^
                                      ((gbyte) 0x80 << 0))));
                    mark += 2;
                } else if ((b1 >> 4) == -2) {
                    // 3 bytes, 16 bits: 1110xxxx 10xxxxxx 10xxxxxx
                    gint const srcRemaining = limit - mark;
                    if (srcRemaining < 3 || dst.remaining() < 1) {
                        if (srcRemaining > 1 && isMalformed3_2(b1, src.get()))
                            return malformedForLength(src, mark, 1);
                        return xflow(src, mark, 3);
                    }
                    gint const b2 = src.get();
                    gint const b3 = src.get();
                    if (isMalformed3(b1, b2, b3))
                        return malformed(src, mark, 3);
                    gchar const c = (gchar)
                            ((b1 << 12) ^
                             (b2 << 6) ^
                             (b3 ^
                              (((gbyte) 0xE0 << 12) ^
                               ((gbyte) 0x80 << 6) ^
                               ((gbyte) 0x80 << 0))));
                    if (Character::isSurrogate(c))
                        return malformedForLength(src, mark, 3);
                    dst.put(c);
                    mark += 3;
                } else if ((b1 >> 3) == -2) {
                    // 4 bytes, 21 bits: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                    gint const srcRemaining = limit - mark;
                    if (srcRemaining < 4 || dst.remaining() < 2) {
                        b1 &= 0xff;
                        if (b1 > 0xf4 ||
                            srcRemaining > 1 && isMalformed4_2(b1, src.get() & 0xff))
                            return malformedForLength(src, mark, 1);
                        if (srcRemaining > 2 && isMalformed4_3(src.get()))
                            return malformedForLength(src, mark, 2);
                        return xflow(src, mark, 4);
                    }
                    gint const b2 = src.get();
                    gint const b3 = src.get();
                    gint const b4 = src.get();
                    gint const uc = ((b1 << 18) ^
                               (b2 << 12) ^
                               (b3 << 6) ^
                               (b4 ^
                                (((gbyte) 0xF0 << 18) ^
                                 ((gbyte) 0x80 << 12) ^
                                 ((gbyte) 0x80 << 6) ^
                                 ((gbyte) 0x80 << 0))));
                    if (isMalformed4(b2, b3, b4) ||
                        // shortest form check
                        !Character::isSupplementary(uc)) {
                        return malformed(src, mark, 4);
                    }
                    dst.put(Character::highSurrogate(uc));
                    dst.put(Character::lowSurrogate(uc));
                    mark += 4;
                } else {
                    return malformed(src, mark, 1);
                }
            }
            return xflow(src, mark, 0);
        }

        CoderResult UTF8::Decoder::decodeLoop(ByteBuffer &src, CharBuffer &dst) {
            if (src.hasArray() && dst.hasArray())
                return decodeArrayLoop(src, dst);
            else
                return decodeBufferLoop(src, dst);
        }

        UTF8::Encoder::Encoder(const Charset &cs) : CharsetEncoder(cs, 1.1F, 3.0F) {}

        CoderResult UTF8::Encoder::overflow(CharBuffer &src, gint sp, ByteBuffer &dst, gint dp) {
            updatePositions(src, sp, dst, dp);
            return CoderResult::OVERFLOW;
        }

        CoderResult UTF8::Encoder::overflow(CharBuffer &src, gint mark) {
            src.setPosition(mark);
            return CoderResult::OVERFLOW;
        }

        CoderResult UTF8::Encoder::encodeBufferLoop(CharBuffer &src, ByteBuffer &dst) {
            gint mark = src.position();
            while (src.hasRemaining()) {
                gchar const c = src.get();
                if (c < 0x80) {
                    // Have at most seven bits
                    if (!dst.hasRemaining())
                        return overflow(src, mark);
                    dst.put((gbyte) c);
                } else if (c < 0x800) {
                    // 2 bytes, 11 bits
                    if (dst.remaining() < 2)
                        return overflow(src, mark);
                    dst.put((gbyte) (0xc0 | (c >> 6)));
                    dst.put((gbyte) (0x80 | (c & 0x3f)));
                } else if (Character::isSurrogate(c)) {
                    // Have a surrogate pair
                    sgp = {};
                    gint const uc = sgp.parse(c, src);
                    if (uc < 0) {
                        src.setPosition(mark);
                        return sgp.error();
                    }
                    if (dst.remaining() < 4)
                        return overflow(src, mark);
                    dst.put((gbyte) (0xf0 | ((uc >> 18))));
                    dst.put((gbyte) (0x80 | ((uc >> 12) & 0x3f)));
                    dst.put((gbyte) (0x80 | ((uc >> 6) & 0x3f)));
                    dst.put((gbyte) (0x80 | (uc & 0x3f)));
                    mark++;  // 2 chars
                } else {
                    // 3 bytes, 16 bits
                    if (dst.remaining() < 3)
                        return overflow(src, mark);
                    dst.put((gbyte) (0xe0 | ((c >> 12))));
                    dst.put((gbyte) (0x80 | ((c >> 6) & 0x3f)));
                    dst.put((gbyte) (0x80 | (c & 0x3f)));
                }
                mark++;
            }
            src.setPosition(mark);
            return CoderResult::UNDERFLOW;
        }

        CoderResult UTF8::Encoder::encodeArrayLoopSlow(CharBuffer &src, const CharArray &sa,
                                                       gint sp, gint sl, ByteBuffer &dst, ByteArray &da, gint dp,
                                                       gint dl) {
            while (sp < sl) {
                gchar const c = sa[sp];
                if (c < 0x80) {
                    // Have at most seven bits
                    if (dp >= dl)
                        return overflow(src, sp, dst, dp);
                    da[dp++] = (gbyte) c;
                } else if (c < 0x800) {
                    // 2 bytes, 11 bits
                    if (dl - dp < 2)
                        return overflow(src, sp, dst, dp);
                    da[dp++] = (gbyte) (0xc0 | (c >> 6));
                    da[dp++] = (gbyte) (0x80 | (c & 0x3f));
                } else if (Character::isSurrogate(c)) {
                    // Have a surrogate pair
                    gint const uc = sgp.parse(c, sa, sp, sl);
                    if (uc < 0) {
                        updatePositions(src, sp, dst, dp);
                        return sgp.error();
                    }
                    if (dl - dp < 4)
                        return overflow(src, sp, dst, dp);
                    da[dp++] = (gbyte) (0xf0 | ((uc >> 18)));
                    da[dp++] = (gbyte) (0x80 | ((uc >> 12) & 0x3f));
                    da[dp++] = (gbyte) (0x80 | ((uc >> 6) & 0x3f));
                    da[dp++] = (gbyte) (0x80 | (uc & 0x3f));
                    sp++;  // 2 chars
                } else {
                    // 3 bytes, 16 bits
                    if (dl - dp < 3)
                        return overflow(src, sp, dst, dp);
                    da[dp++] = (gbyte) (0xe0 | ((c >> 12)));
                    da[dp++] = (gbyte) (0x80 | ((c >> 6) & 0x3f));
                    da[dp++] = (gbyte) (0x80 | (c & 0x3f));
                }
                sp++;
            }
            updatePositions(src, sp, dst, dp);
            return CoderResult::UNDERFLOW;
        }

        CoderResult UTF8::Encoder::encodeArrayLoop(CharBuffer &src, ByteBuffer &dst) {
            CharArray const sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint const sl = src.arrayOffset() + src.limit();

            ByteArray &da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint const dl = dst.arrayOffset() + dst.limit();

            // Handle ASCII-only prefix
            gint const n = encodeASCII(sa, sp, da, dp, Math::min(sl - sp, dl - dp));
            sp += n;
            dp += n;

            if (sp < sl) {
                return encodeArrayLoopSlow(src, sa, sp, sl, dst, da, dp, dl);
            } else {
                updatePositions(src, sp, dst, dp);
                return CoderResult::UNDERFLOW;
            }
        }

        gbool UTF8::Encoder::canEncode(gchar c) {
            return !Character::isSurrogate(c);
        }

        gbool UTF8::Encoder::isLegalReplacement(const ByteArray &repl) const {
            return ((repl.length() == 1 && repl[0] >= 0) ||
                    CharsetEncoder::isLegalReplacement(repl));
        }
    } // charset
} // core