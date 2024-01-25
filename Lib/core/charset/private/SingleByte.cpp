//
// Created by T.N.Brunshweeck on 15/01/2024.
//

#include "SingleByte.h"

namespace core {
    namespace charset {
        CoderResult SingleByte::withResult(CoderResult cr, Buffer &src, gint sp, Buffer &dst, gint dp) {
            src.setPosition(sp - src.arrayOffset());
            dst.setPosition(dp - dst.arrayOffset());
            return cr;
        }

        void SingleByte::initC2B(CharArray &b2c, CharArray &c2bNR, CharArray &c2b, CharArray &c2bIndex) {
            for (gint i = 0; i < c2bIndex.length(); i++)
                c2bIndex[i] = UNMAPPABLE_ENCODING;
            for (gint i = 0; i < c2b.length(); i++)
                c2b[i] = UNMAPPABLE_ENCODING;
            gint off = 0;
            for (gint i = 0; i < b2c.length(); i++) {
                gchar const c = b2c[i];
                if (c == UNMAPPABLE_DECODING)
                    continue;
                gint index = (c >> 8);
                if (c2bIndex[index] == UNMAPPABLE_ENCODING) {
                    c2bIndex[index] = (gchar) off;
                    off += 0x100;
                }
                index = c2bIndex[index] + (c & 0xff);
                c2b[index] = (gchar) ((i >= 0x80) ? (i - 0x80) : (i + 0x80));
            }
            if (!c2bNR.isEmpty()) {
                // c→b nr entries
                gint i = 0;
                while (i < c2bNR.length()) {
                    gchar const b = c2bNR[i++];
                    gchar const c = c2bNR[i++];
                    gint index = (c >> 8);
                    if (c2bIndex[index] == UNMAPPABLE_ENCODING) {
                        c2bIndex[index] = (gchar) off;
                        off += 0x100;
                    }
                    index = c2bIndex[index] + (c & 0xff);
                    c2b[index] = b;
                }
            }
        }

        SingleByte::Decoder::Decoder(const Charset &cs, const CharArray &b2c) :
                CharsetDecoder(cs, 1.0F, 1.0F), b2c(b2c), isASCIICompatible(false), isLatin1Decodable(false) {}

        SingleByte::Decoder::Decoder(const Charset &cs, const CharArray &b2c, gbool isASCIICompatible) :
                CharsetDecoder(cs, 1.0F, 1.0F), b2c(b2c), isASCIICompatible(isASCIICompatible),
                isLatin1Decodable(false) {}

        SingleByte::Decoder::Decoder(const Charset &cs, const CharArray &b2c, gbool isASCIICompatible,
                                     gbool isLatin1Decodable) :
                CharsetDecoder(cs, 1.0F, 1.0F), b2c(b2c), isASCIICompatible(isASCIICompatible),
                isLatin1Decodable(isLatin1Decodable) {}

        CoderResult SingleByte::Decoder::decodeArrayLoop(ByteBuffer &src, CharBuffer &dst) const {
            ByteArray sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint sl = src.arrayOffset() + src.limit();

            CharArray da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint const dl = dst.arrayOffset() + dst.limit();

            CoderResult cr = CoderResult::UNDERFLOW;
            if ((dl - dp) < (sl - sp)) {
                sl = sp + (dl - dp);
                cr = CoderResult::OVERFLOW;
            }

            if (isASCIICompatible) {
                gint const n = Unicode::decodeASCII(sa, sp, da, dp, Math::min(dl - dp, sl - sp));
                sp += n;
                dp += n;
            }
            while (sp < sl) {
                gchar const c = decode(sa[sp]);
                if (c == UNMAPPABLE_DECODING) {
                    return withResult(CoderResult::unmappableForLength(1), src, sp, dst, dp);
                }
                da[dp++] = c;
                sp++;
            }
            return withResult(cr, src, sp, dst, dp);
        }

        CoderResult SingleByte::Decoder::decodeBufferLoop(ByteBuffer &src, CharBuffer &dst) const {
            gint mark = src.position();
            try {
                while (src.hasRemaining()) {
                    gchar const c = decode(src.get());
                    if (c == UNMAPPABLE_DECODING)
                        return CoderResult::unmappableForLength(1);
                    if (!dst.hasRemaining())
                        return CoderResult::OVERFLOW;
                    dst.put(c);
                    mark++;
                }
                src.setPosition(mark);
                return CoderResult::UNDERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(mark);
                th.throws(__trace("core.charset.SingleByte.Decoder"));
            }
        }

        CoderResult SingleByte::Decoder::decodeLoop(ByteBuffer &src, CharBuffer &dst) {
            if (src.hasArray() && dst.hasArray())
                return decodeArrayLoop(src, dst);
            else
                return decodeBufferLoop(src, dst);
        }

        gchar SingleByte::Decoder::decode(gint b) const {
            return b2c[b + 128];
        }

        void SingleByte::Decoder::replaceWith0(const String &newReplacement) {
            repl = newReplacement.charAt(0);
        }

        gint SingleByte::Decoder::decodeToLatin1(const ByteArray &src, gint sp, gint len, ByteArray &dst) const {
            if (len > dst.length())
                len = dst.length();

            gint dp = 0;
            while (dp < len) {
                dst[dp++] = (gbyte) decode(src[sp++]);
            }
            return dp;
        }

        gint SingleByte::Decoder::decode(const ByteArray &src, gint sp, gint len, CharArray &dst) const {
            if (len > dst.length())
                len = dst.length();
            gint dp = 0;
            while (dp < len) {
                dst[dp] = decode(src[sp++]);
                if (dst[dp] == UNMAPPABLE_DECODING) {
                    dst[dp] = repl;
                }
                dp++;
            }
            return dp;
        }

        CoderResult SingleByte::Encoder::encodeArrayLoop(CharBuffer &src, ByteBuffer &dst) {
            CharArray sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint const sl = src.arrayOffset() + src.limit();

            ByteArray da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint const dl = dst.arrayOffset() + dst.limit();
            gint len = Math::min(dl - dp, sl - sp);

            if (isASCIICompatible) {
                gint const n = Unicode::encodeASCII(sa, sp, da, dp, len);
                sp += n;
                dp += n;
                len -= n;
            }
            while (len-- > 0) {
                gchar const c = sa[sp];
                gint const b = encode(c);
                if (b == UNMAPPABLE_ENCODING) {
                    if (Character::isSurrogate(c)) {
                        if (sgp.parse(c, sa, sp, sl) < 0) {
                            return withResult(sgp.error(), src, sp, dst, dp);
                        }
                        return withResult(sgp.unmappableResult(), src, sp, dst, dp);
                    }
                    return withResult(CoderResult::unmappableForLength(1),
                                      src, sp, dst, dp);
                }
                da[dp++] = (gbyte) b;
                sp++;
            }
            return withResult(sp < sl ? CoderResult::OVERFLOW : CoderResult::UNDERFLOW,
                              src, sp, dst, dp);
        }

        SingleByte::Encoder::Encoder(const Charset &cs, const CharArray &c2b, const CharArray &c2bIndex,
                                     gbool isASCIICompatible) :
                CharsetEncoder(cs, 1.0F, 1.0F), c2b(c2b), c2bIndex(c2bIndex), isASCIICompatible(isASCIICompatible) {}

        gbool SingleByte::Encoder::canEncode(gchar c) {
            return encode(c) != UNMAPPABLE_ENCODING;
        }

        gbool SingleByte::Encoder::isLegalReplacement(const ByteArray &repl) const {
            return ((repl.length() == 1 && repl[0] == (gbyte) '?') || CharsetEncoder::isLegalReplacement(repl));
        }

        CoderResult SingleByte::Encoder::encodeBufferLoop(CharBuffer &src, ByteBuffer &dst) {
            gint mark = src.position();
            try {
                while (src.hasRemaining()) {
                    gchar const c = src.get();
                    gint const b = encode(c);
                    if (b == UNMAPPABLE_ENCODING) {
                        if (Character::isSurrogate(c)) {
                            if (sgp.parse(c, src) < 0)
                                return sgp.error();
                            return sgp.unmappableResult();
                        }
                        return CoderResult::unmappableForLength(1);
                    }
                    if (!dst.hasRemaining())
                        return CoderResult::OVERFLOW;
                    dst.put((gbyte) b);
                    mark++;
                }
                src.setPosition(mark);
                return CoderResult::UNDERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(mark);
                th.throws(__trace("core.charset.SingleByte.Encoder"));
            }
        }

        CoderResult SingleByte::Encoder::encodeLoop(CharBuffer &src, ByteBuffer &dst) {
            if (src.hasArray() && dst.hasArray())
                return encodeArrayLoop(src, dst);
            else
                return encodeBufferLoop(src, dst);
        }

        gint SingleByte::Encoder::encode(gchar ch) {
            gchar const index = c2bIndex[ch >> 8];
            if (index == UNMAPPABLE_ENCODING)
                return UNMAPPABLE_ENCODING;
            return c2b[index + (ch & 0xff)];
        }

        void SingleByte::Encoder::replaceWith0(const ByteArray &newReplacement) {
            repl = newReplacement[0];
        }

        gint SingleByte::Encoder::encode(const CharArray &src, gint sp, gint len, ByteArray &dst) {
            gint dp = 0;
            gint sl = sp + Math::min(len, dst.length());
            while (sp < sl) {
                gchar const c = src[sp++];
                gint const b = encode(c);
                if (b != UNMAPPABLE_ENCODING) {
                    dst[dp++] = (gbyte) b;
                    continue;
                }
                if (Character::isHighSurrogate(c) && sp < sl &&
                    Character::isLowSurrogate(src[sp])) {
                    if (len > dst.length()) {
                        sl++;
                        len--;
                    }
                    sp++;
                }
                dst[dp++] = repl;
            }
            return dp;
        }

        gint SingleByte::Encoder::encodeFromLatin1(ByteArray src, gint sp, gint len, ByteArray dst) {
            gint dp = 0;
            gint const sl = sp + Math::min(len, dst.length());
            while (sp < sl) {
                gchar const c = (gchar) (src[sp++] & 0xff);
                gint const b = encode(c);
                if (b == UNMAPPABLE_ENCODING) {
                    dst[dp++] = repl;
                } else {
                    dst[dp++] = (gbyte) b;
                }
            }
            return dp;
        }

        gint SingleByte::Encoder::encodeFromUTF16(const ByteArray &src, gint sp, gint len, ByteArray &dst) {
            gint dp = 0;
            gint sl = sp + Math::min(len, dst.length());
            while (sp < sl) {
                gchar const c = getChar(src, sp++);
                gint const b = encode(c);
                if (b != UNMAPPABLE_ENCODING) {
                    dst[dp++] = (gbyte) b;
                    continue;
                }
                if (Character::isHighSurrogate(c) && sp < sl &&
                    Character::isLowSurrogate(getChar(src, sp))) {
                    if (len > dst.length()) {
                        sl++;
                        len--;
                    }
                    sp++;
                }
                dst[dp++] = repl;
            }
            return dp;
        }

        gchar SingleByte::Encoder::getChar(const ByteArray &val, int index) {
            CORE_ASSERT_IF(index >= 0 && index < length(val), "Trusted caller missed bounds check",
                           "core.charset.SingleByte.Encoder");
            index <<= 1;
            return (gchar) (((val[index] & 0xff) << HI_BYTE_SHIFT) |
                            ((val[index + 1] & 0xff) << LO_BYTE_SHIFT));
        }

        int SingleByte::Encoder::length(const ByteArray &value) {
            return value.length() >> 1;
        }
    } // charset
} // core