//
// Created by T.N.Brunshweeck on 17/01/2024.
//

#include "CESU8.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        using namespace native;

        CESU8 CESU8::INSTANCE{};

        CESU8::CESU8() : Unicode("CESU-8", StringArray::of("CESU8", "csCESU-8")) {}

        CharsetDecoder &CESU8::decoder() const {
            return Unsafe::allocateInstance<Decoder>();
        }

        CharsetEncoder &CESU8::encoder() const {
            return Unsafe::allocateInstance<Encoder>();
        }

        Object &CESU8::clone() const {
            return INSTANCE;
        }

        void CESU8::updatePositions(Buffer &src, gint sp, Buffer &dst, gint dp) {
            src.setPosition(sp - src.arrayOffset());
            dst.setPosition(dp - dst.arrayOffset());
        }

        gbool CESU8::Decoder::isNotContinuation(gint b) {
            return (b & 0xc0) != 0x80;
        }

        gbool CESU8::Decoder::isMalformed3(gint b1, gint b2, gint b3) {
            return (b1 == (gbyte)0xe0 && (b2 & 0xe0) == 0x80) ||
                   (b2 & 0xc0) != 0x80 || (b3 & 0xc0) != 0x80;
        }

        gbool CESU8::Decoder::isMalformed3_2(gint b1, gint b2) {
            return (b1 == (gbyte)0xe0 && (b2 & 0xe0) == 0x80) ||
                   (b2 & 0xc0) != 0x80;
        }

        CoderResult CESU8::Decoder::malformedN(ByteBuffer &src, gint nb) {
            gint b1;
            gint b2;
            switch (nb) {
                case 1:
                case 2:                    // always 1
                    return CoderResult::malformedForLength(1);
                case 3:
                    b1 = src.get();
                    b2 = src.get();    // no need to look up b3
                    return CoderResult::malformedForLength(
                            ((b1 == (gbyte)0xe0 && (b2 & 0xe0) == 0x80) ||
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
                    CORE_ASSERT(false, "");
            }
        }

        CoderResult CESU8::Decoder::malformed(ByteBuffer &src, gint sp, CharBuffer &dst, gint dp, gint nb) {
            src.setPosition(sp - src.arrayOffset());
            CoderResult cr = malformedN(src, nb);
            updatePositions(src, sp, dst, dp);
            return cr;
        }

        CoderResult CESU8::Decoder::malformed(ByteBuffer &src, gint mark, gint nb) {
            src.setPosition(mark);
            CoderResult cr = malformedN(src, nb);
            src.setPosition(mark);
            return cr;
        }

        CoderResult
        CESU8::Decoder::malformedForLength(ByteBuffer &src, gint sp, CharBuffer &dst, gint dp, gint malformedNB) {
            updatePositions(src, sp, dst, dp);
            return CoderResult::malformedForLength(malformedNB);
        }

        CoderResult CESU8::Decoder::malformedForLength(ByteBuffer &src, gint mark, gint malformedNB) {
            src.setPosition(mark);
            return CoderResult::malformedForLength(malformedNB);
        }

        CoderResult CESU8::Decoder::xflow(Buffer &src, gint sp, gint sl, Buffer &dst, gint dp, gint nb) {
            updatePositions(src, sp, dst, dp);
            return (nb == 0 || sl - sp < nb)
                   ? CoderResult::UNDERFLOW : CoderResult::OVERFLOW;
        }

        CoderResult CESU8::Decoder::xflow(Buffer &src, gint mark, gint nb) {
            src.setPosition(mark);
            return (nb == 0 || src.remaining() < nb)
                   ? CoderResult::UNDERFLOW : CoderResult::OVERFLOW;
        }

        CoderResult CESU8::Decoder::decodeArrayLoop(ByteBuffer &src, CharBuffer &dst) {
            // This method is optimized for ASCII input.
            ByteArray const &sa = src.array();
            gint const soff = src.arrayOffset();
            gint sp = soff + src.position();
            gint const sl = soff + src.limit();

            CharArray &da = dst.array();
            gint const doff = dst.arrayOffset();
            gint dp = doff + dst.position();
            gint const dl = doff + dst.limit();

            gint const n = Unicode::decodeASCII(sa, sp, da, dp, Math::min(sl - sp, dl - dp));
            sp += n;
            dp += n;

            while (sp < sl) {
                gint const b1 = sa[sp];
                if (b1 >= 0) {
                    // 1 gbyte, 7 bits: 0xxxxxxx
                    if (dp >= dl)
                        return xflow(src, sp, sl, dst, dp, 1);
                    da[dp++] = (char) b1;
                    sp++;
                } else if ((b1 >> 5) == -2 && (b1 & 0x1e) != 0) {
                    // 2 bytes, 11 bits: 110xxxxx 10xxxxxx
                    if (sl - sp < 2 || dp >= dl)
                        return xflow(src, sp, sl, dst, dp, 2);
                    gint const b2 = sa[sp + 1];
                    if (isNotContinuation(b2))
                        return malformedForLength(src, sp, dst, dp, 1);
                    da[dp++] = (char) (((b1 << 6) ^ b2)
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
                    da[dp++] = (char)
                            ((b1 << 12) ^
                             (b2 <<  6) ^
                             (b3 ^
                              (((gbyte) 0xE0 << 12) ^
                               ((gbyte) 0x80 <<  6) ^
                               ((gbyte) 0x80 <<  0))));
                    sp += 3;
                } else {
                    return malformed(src, sp, dst, dp, 1);
                }
            }
            return xflow(src, sp, sl, dst, dp, 0);
        }

        CoderResult CESU8::Decoder::decodeBufferLoop(ByteBuffer &src, CharBuffer &dst) {
            gint mark = src.position();
            gint const limit = src.limit();
            while (mark < limit) {
                gint const b1 = src.get();
                if (b1 >= 0) {
                    // 1 gbyte, 7 bits: 0xxxxxxx
                    if (dst.remaining() < 1)
                        return xflow(src, mark, 1); // overflow
                    dst.put((char) b1);
                    mark++;
                } else if ((b1 >> 5) == -2 && (b1 & 0x1e) != 0) {
                    // 2 bytes, 11 bits: 110xxxxx 10xxxxxx
                    if (limit - mark < 2|| dst.remaining() < 1)
                        return xflow(src, mark, 2);
                    gint const b2 = src.get();
                    if (isNotContinuation(b2))
                        return malformedForLength(src, mark, 1);
                    dst.put((char) (((b1 << 6) ^ b2)
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
                    dst.put((char)
                                    ((b1 << 12) ^
                                     (b2 <<  6) ^
                                     (b3 ^
                                      (((gbyte) 0xE0 << 12) ^
                                       ((gbyte) 0x80 <<  6) ^
                                       ((gbyte) 0x80 <<  0)))));
                    mark += 3;
                } else {
                    return malformed(src, mark, 1);
                }
            }
            return xflow(src, mark, 0);
        }

        CoderResult CESU8::Decoder::decodeLoop(ByteBuffer &src, CharBuffer &dst) {
            if (src.hasArray() && dst.hasArray())
                return decodeArrayLoop(src, dst);
            else
                return decodeBufferLoop(src, dst);
        }

        ByteBuffer &CESU8::Decoder::getByteBuffer(Optional<ByteBuffer>  &bb, ByteArray &ba, gint sp) {
            if (bb.isEmpty()) {
                Unsafe::destroyInstance(bb.get());
                bb = Optional<ByteBuffer>::of(ByteBuffer::wrap(ba));
            }
            bb.get().setPosition(sp);
            return bb.get();
        }

        gint CESU8::Decoder::decode(const ByteArray &sa, gint sp, gint len, CharArray &da) {
            const gint sl = sp + len;
            gint dp = 0;
            gint const dlASCII = Math::min(len, da.length());
            Optional<ByteBuffer> bb = Optional<ByteBuffer>::empty(); // only if malformed

            // ASCII only optimized loop
            while (dp < dlASCII && sa[sp] >= 0)
                da[dp++] = (char) sa[sp++];

            while (sp < sl) {
                gint const b1 = sa[sp++];
                if (b1 >= 0) {
                    // 1 gbyte, 7 bits: 0xxxxxxx
                    da[dp++] = (char) b1;
                } else if ((b1 >> 5) == -2 && (b1 & 0x1e) != 0) {
                    // 2 bytes, 11 bits: 110xxxxx 10xxxxxx
                    if (sp < sl) {
                        gint const b2 = sa[sp++];
                        if (isNotContinuation(b2)) {
                            if (malformedInputAction() != Charset::REPLACE)
                                return -1;
                            da[dp++] = replacement().charAt(0);
                            sp--;            // malformedN(bb, 2) always returns 1
                        } else {
                            da[dp++] = (char) (((b1 << 6) ^ b2)^
                                               (((gbyte) 0xC0 << 6) ^
                                                ((gbyte) 0x80 << 0)));
                        }
                        continue;
                    }
                    if (malformedInputAction() != Charset::REPLACE)
                        return -1;
                    da[dp++] = replacement().charAt(0);
                    return dp;
                } else if ((b1 >> 4) == -2) {
                    // 3 bytes, 16 bits: 1110xxxx 10xxxxxx 10xxxxxx
                    if (sp + 1 < sl) {
                        gint const b2 = sa[sp++];
                        gint const b3 = sa[sp++];
                        if (isMalformed3(b1, b2, b3)) {
                            if (malformedInputAction() != Charset::REPLACE)
                                return -1;
                            da[dp++] = replacement().charAt(0);
                            sp -=3;
                            bb = Optional<ByteBuffer>::of(getByteBuffer(bb, (ByteArray &)sa, sp));
                            sp += malformedN(bb, 3).length();
                        } else {
                            da[dp++] = (char)((b1 << 12) ^
                                              (b2 <<  6) ^
                                              (b3 ^
                                               (((gbyte) 0xE0 << 12) ^
                                                ((gbyte) 0x80 <<  6) ^
                                                ((gbyte) 0x80 <<  0))));
                        }
                        continue;
                    }
                    if (malformedInputAction() != Charset::REPLACE)
                        return -1;
                    if (sp  < sl && isMalformed3_2(b1, sa[sp])) {
                        da[dp++] = replacement().charAt(0);
                        continue;

                    }
                    da[dp++] = replacement().charAt(0);
                    return dp;
                } else {
                    if (malformedInputAction() != Charset::REPLACE)
                        return -1;
                    da[dp++] = replacement().charAt(0);
                }
            }
            return dp;
        }

        Object &CESU8::Decoder::clone() const {
            return INSTANCE.decoder();
        }

        CESU8::Encoder::Encoder() : CharsetEncoder(INSTANCE, 1.1F, 3.0F) {}

        gbool CESU8::Encoder::canEncode(gchar c) {
            return !Character::isSurrogate(c);
        }

        gbool CESU8::Encoder::isLegalReplacement(const ByteArray &repl) const {
            return ((repl.length() == 1 && repl[0] >= 0) || CharsetEncoder::isLegalReplacement(repl));
        }

        CoderResult CESU8::Encoder::overflow(CharBuffer &src, gint sp, ByteBuffer &dst, gint dp) {
            updatePositions(src, sp, dst, dp);
            return CoderResult::OVERFLOW;
        }

        CoderResult CESU8::Encoder::overflow(CharBuffer &src, gint mark) {
            src.setPosition(mark);
            return CoderResult::OVERFLOW;
        }

        void CESU8::Encoder::to3Bytes(ByteArray &da, gint dp, gchar c) {
            da[dp] = (gbyte)(0xe0 | ((c >> 12)));
            da[dp + 1] = (gbyte)(0x80 | ((c >>  6) & 0x3f));
            da[dp + 2] = (gbyte)(0x80 | (c & 0x3f));
        }

        void CESU8::Encoder::to3Bytes(ByteBuffer &dst, gchar c) {
            dst.put((gbyte)(0xe0 | ((c >> 12))));
            dst.put((gbyte)(0x80 | ((c >>  6) & 0x3f)));
            dst.put((gbyte)(0x80 | (c & 0x3f)));
        }

        CoderResult CESU8::Encoder::encodeArrayLoop(CharBuffer &src, ByteBuffer &dst) {
            CharArray const& sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint const sl = src.arrayOffset() + src.limit();

            ByteArray &da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint const dl = dst.arrayOffset() + dst.limit();

            // Handle ASCII-only prefix
            gint const n = Unicode::encodeASCII(sa, sp, da, dp, Math::min(sl - sp, dl - dp));
            sp += n;
            dp += n;

            while (sp < sl) {
                gchar const c = sa[sp];
                if (c < 0x80) {
                    // Have at most seven bits
                    if (dp >= dl)
                        return overflow(src, sp, dst, dp);
                    da[dp++] = (gbyte)c;
                } else if (c < 0x800) {
                    // 2 bytes, 11 bits
                    if (dl - dp < 2)
                        return overflow(src, sp, dst, dp);
                    da[dp++] = (gbyte)(0xc0 | (c >> 6));
                    da[dp++] = (gbyte)(0x80 | (c & 0x3f));
                } else if (Character::isSurrogate(c)) {
                    // Have a surrogate pair
                    gint const uc = sgp.parse(c, sa, sp, sl);
                    if (uc < 0) {
                        updatePositions(src, sp, dst, dp);
                        return sgp.error();
                    }
                    if (dl - dp < 6)
                        return overflow(src, sp, dst, dp);
                    to3Bytes(da, dp, Character::highSurrogate(uc));
                    dp += 3;
                    to3Bytes(da, dp, Character::lowSurrogate(uc));
                    dp += 3;
                    sp++;  // 2 chars
                } else {
                    // 3 bytes, 16 bits
                    if (dl - dp < 3)
                        return overflow(src, sp, dst, dp);
                    to3Bytes(da, dp, c);
                    dp += 3;
                }
                sp++;
            }
            updatePositions(src, sp, dst, dp);
            return CoderResult::UNDERFLOW;
        }

        CoderResult CESU8::Encoder::encodeBufferLoop(CharBuffer &src, ByteBuffer &dst) {
            gint mark = src.position();
            while (src.hasRemaining()) {
                gchar const c = src.get();
                if (c < 0x80) {
                    // Have at most seven bits
                    if (!dst.hasRemaining())
                        return overflow(src, mark);
                    dst.put((gbyte)c);
                } else if (c < 0x800) {
                    // 2 bytes, 11 bits
                    if (dst.remaining() < 2)
                        return overflow(src, mark);
                    dst.put((gbyte)(0xc0 | (c >> 6)));
                    dst.put((gbyte)(0x80 | (c & 0x3f)));
                } else if (Character::isSurrogate(c)) {
                    // Have a surrogate pair
                    gint const uc = sgp.parse(c, src);
                    if (uc < 0) {
                        src.setPosition(mark);
                        return sgp.error();
                    }
                    if (dst.remaining() < 6)
                        return overflow(src, mark);
                    to3Bytes(dst, Character::highSurrogate(uc));
                    to3Bytes(dst, Character::lowSurrogate(uc));
                    mark++;  // 2 chars
                } else {
                    // 3 bytes, 16 bits
                    if (dst.remaining() < 3)
                        return overflow(src, mark);
                    to3Bytes(dst, c);
                }
                mark++;
            }
            src.setPosition(mark);
            return CoderResult::UNDERFLOW;
        }

        CoderResult CESU8::Encoder::encodeLoop(CharBuffer &src, ByteBuffer &dst) {
            if (src.hasArray() && dst.hasArray())
                return encodeArrayLoop(src, dst);
            else
                return encodeBufferLoop(src, dst);
        }

        gint CESU8::Encoder::encode(const CharArray &sa, gint sp, gint len, ByteArray &da) {
            gint const sl = sp + len;
            gint dp = 0;

            // Handle ASCII-only prefix
            gint const n = Unicode::encodeASCII(sa, sp, da, dp, Math::min(len, da.length()));
            sp += n;
            dp += n;

            while (sp < sl) {
                gchar const c = sa[sp++];
                if (c < 0x80) {
                    // Have at most seven bits
                    da[dp++] = (gbyte)c;
                } else if (c < 0x800) {
                    // 2 bytes, 11 bits
                    da[dp++] = (gbyte)(0xc0 | (c >> 6));
                    da[dp++] = (gbyte)(0x80 | (c & 0x3f));
                } else if (Character::isSurrogate(c)) {
                    gint const uc = sgp.parse(c, sa, sp - 1, sl);
                    if (uc < 0) {
                        if (malformedInputAction() != Charset::REPLACE)
                            return -1;
                        da[dp++] = replacement()[0];
                    } else {
                        to3Bytes(da, dp, Character::highSurrogate(uc));
                        dp += 3;
                        to3Bytes(da, dp, Character::lowSurrogate(uc));
                        dp += 3;
                        sp++;  // 2 chars
                    }
                } else {
                    // 3 bytes, 16 bits
                    to3Bytes(da, dp, c);
                    dp += 3;
                }
            }
            return dp;
        }

        Object &CESU8::Encoder::clone() const {
            return INSTANCE.encoder();
        }
    } // charset
} // core