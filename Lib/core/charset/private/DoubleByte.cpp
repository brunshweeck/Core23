//
// Created by T.N.Brunshweeck on 16/01/2024.
//

#include "DoubleByte.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        using namespace native;

        CharArray DoubleByte::B2C_UNMAPPABLE = CharArray(0x100, UNMAPPABLE_DECODING);
        CharArray DoubleByte::Decoder_DBCSONLY::b2cSB_UNMAPPABLE = CharArray(0x100, UNMAPPABLE_DECODING);

        CoderResult DoubleByte::Decoder_DBCSONLY::crMalformedOrUnmappable(gint b1, gint b2) {
            CORE_IGNORE(b1);
            CORE_IGNORE(b2);
            return CoderResult::unmappableForLength(2);
        }

        DoubleByte::Decoder_DBCSONLY::Decoder_DBCSONLY(const Charset &cs, CharArray2D &b2c, CharArray &b2cSB,
                                                       gint b2Min, gint b2Max, gbool isASCIICompatible) :
                Decoder(cs, 0.5F, 1.0F, b2c, b2cSB_UNMAPPABLE, b2Min, b2Max, isASCIICompatible) {
            CORE_IGNORE(b2cSB);
        }

        DoubleByte::Decoder_DBCSONLY::Decoder_DBCSONLY(const Charset &cs, CharArray2D &b2c, CharArray &b2cSB,
                                                       gint b2Min, gint b2Max) :
                Decoder(cs, 0.5F, 1.0F, b2c, b2cSB_UNMAPPABLE, b2Min, b2Max, false) {
            CORE_IGNORE(b2cSB);
        }

        CoderResult DoubleByte::Decoder::crMalformedOrUnderFlow(gint b) {
            CORE_IGNORE(b);
            return CoderResult::UNDERFLOW;
        }

        CoderResult DoubleByte::Decoder::crMalformedOrUnmappable(gint b1, gint b2) {
            if (b2c[b1] == B2C_UNMAPPABLE ||                // isNotLeadingByte(b1)
                b2c[b2] != B2C_UNMAPPABLE ||                // isLeadingByte(b2)
                decodeSingle(b2) != UNMAPPABLE_DECODING) {  // isSingle(b2)
                return CoderResult::malformedForLength(1);
            }
            return CoderResult::unmappableForLength(2);
        }

        DoubleByte::Decoder::Decoder(const Charset &cs, gfloat avgcpb, gfloat maxcpb, CharArray2D &b2c,
                                     CharArray &b2cSB, gint b2Min, gint b2Max, gbool isASCIICompatible) :
                CharsetDecoder(cs, avgcpb, maxcpb), isASCIICompatible(isASCIICompatible), b2Max(b2Max), b2Min(b2Min),
                b2cSB(b2cSB), b2c(b2c) {}

        DoubleByte::Decoder::Decoder(const Charset &cs, CharArray2D &b2c, CharArray &b2cSB, gint b2Min, gint b2Max,
                                     gbool isASCIICompatible) : Decoder(cs, 0.5F, 1.0F, b2c, b2cSB, b2Min, b2Max,
                                                                        isASCIICompatible) {}

        DoubleByte::Decoder::Decoder(const Charset &cs, CharArray2D &b2c, CharArray &b2cSB, gint b2Min, gint b2Max)
                : Decoder(cs, 0.5F, 1.0F, b2c, b2cSB, b2Min, b2Max, false) {}

        CoderResult DoubleByte::Decoder::decodeArrayLoop(ByteBuffer &src, CharBuffer &dst) {
            ByteArray const &sa = src.array();
            gint const soff = src.arrayOffset();
            gint sp = soff + src.position();
            gint const sl = soff + src.limit();

            CharArray &da = dst.array();
            gint const doff = dst.arrayOffset();
            gint dp = doff + dst.position();
            gint const dl = doff + dst.limit();

            try {
                if (isASCIICompatible) {
                    gint const n = Unicode::decodeASCII(sa, sp, da, dp, Math::min(dl - dp, sl - sp));
                    dp += n;
                    sp += n;
                }
                while (sp < sl && dp < dl) {
                    // inline the decodeSingle/Double() for better performance
                    gint inSize = 1;
                    gint const b1 = sa[sp] & 0xff;
                    gchar c = b2cSB[b1];
                    if (c == UNMAPPABLE_DECODING) {
                        if (sl - sp < 2) {
                            src.setPosition(sp - soff);
                            dst.setPosition(dp - doff);
                            return crMalformedOrUnderFlow(b1);
                        }
                        gint const b2 = sa[sp + 1] & 0xff;
                        if (b2 < b2Min || b2 > b2Max ||
                            (c = b2c[b1][b2 - b2Min]) == UNMAPPABLE_DECODING) {
                            src.setPosition(sp - soff);
                            dst.setPosition(dp - doff);
                            return crMalformedOrUnmappable(b1, b2);
                        }
                        inSize++;
                    }
                    da[dp++] = c;
                    sp += inSize;
                }
                src.setPosition(sp - soff);
                dst.setPosition(dp - doff);
                return (sp >= sl) ? CoderResult::UNDERFLOW : CoderResult::OVERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(sp - soff);
                dst.setPosition(dp - doff);
                th.throws();
            }
        }

        CoderResult DoubleByte::Decoder::decodeBufferLoop(ByteBuffer &src, CharBuffer &dst) {
            gint mark = src.position();
            try {

                while (src.hasRemaining() && dst.hasRemaining()) {
                    gint const b1 = src.get() & 0xff;
                    gchar c = b2cSB[b1];
                    gint inSize = 1;
                    if (c == UNMAPPABLE_DECODING) {
                        if (src.remaining() < 1)
                            return crMalformedOrUnderFlow(b1);
                        gint const b2 = src.get() & 0xff;
                        if (b2 < b2Min || b2 > b2Max ||
                            (c = b2c[b1][b2 - b2Min]) == UNMAPPABLE_DECODING)
                            return crMalformedOrUnmappable(b1, b2);
                        inSize++;
                    }
                    dst.put(c);
                    mark += inSize;
                }
                src.setPosition(mark);
                return src.hasRemaining() ? CoderResult::OVERFLOW : CoderResult::UNDERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(mark);
                th.throws();
            }
        }

        CoderResult DoubleByte::Decoder::decodeLoop(ByteBuffer &src, CharBuffer &dst) {
            if (src.hasArray() && dst.hasArray())
                return decodeArrayLoop(src, dst);
            else
                return decodeBufferLoop(src, dst);
        }

        gint DoubleByte::Decoder::decode(const ByteArray &src, gint sp, gint len, CharArray &dst) {
            gint dp = 0;
            gint const sl = sp + len;
            gchar const repl = replacement().charAt(0);
            while (sp < sl) {
                gint const b1 = src[sp++] & 0xff;
                gchar c = b2cSB[b1];
                if (c == UNMAPPABLE_DECODING) {
                    if (sp < sl) {
                        gint const b2 = src[sp++] & 0xff;
                        if (b2 < b2Min || b2 > b2Max ||
                            (c = b2c[b1][b2 - b2Min]) == UNMAPPABLE_DECODING) {
                            if (crMalformedOrUnmappable(b1, b2).length() == 1) {
                                sp--;
                            }
                        }
                    }
                    if (c == UNMAPPABLE_DECODING) {
                        c = repl;
                    }
                }
                dst[dp++] = c;
            }
            return dp;
        }

        void DoubleByte::Decoder::reset0() {
            CharsetDecoder::reset0();
        }

        CoderResult DoubleByte::Decoder::flush0(CharBuffer &out) {
            return CharsetDecoder::flush0(out);
        }

        gchar DoubleByte::Decoder::decodeSingle(gint b) {
            return b2cSB[b];
        }

        gchar DoubleByte::Decoder::decodeDouble(gint b1, gint b2) {
            if (b1 < 0 || b1 > b2c.length() || b2 < b2Min || b2 > b2Max)
                return UNMAPPABLE_DECODING;
            return b2c[b1][b2 - b2Min];
        }

        Object &DoubleByte::Decoder::clone() const {
            return charset().decoder();
        }

        void DoubleByte::Decoder_EBCDIC::reset0() {
            currentState = SBCS;
        }

        DoubleByte::Decoder_EBCDIC::Decoder_EBCDIC(const Charset &cs, CharArray2D &b2c, CharArray &b2cSB,
                                                   gint b2Min, gint b2Max, gbool isASCIICompatible) :
                Decoder(cs, b2c, b2cSB, b2Min, b2Max, isASCIICompatible) {}

        DoubleByte::Decoder_EBCDIC::Decoder_EBCDIC(const Charset &cs, CharArray2D &b2c, CharArray &b2cSB,
                                                   gint b2Min, gint b2Max) :
                Decoder(cs, b2c, b2cSB, b2Min, b2Max, false) {}

        gbool DoubleByte::Decoder_EBCDIC::isDoubleByte(gint b1, gint b2) {
            return (0x41 <= b1 && b1 <= 0xfe && 0x41 <= b2 && b2 <= 0xfe)
                   || (b1 == 0x40 && b2 == 0x40); // DBCS-HOST SPACE
        }

        CoderResult DoubleByte::Decoder_EBCDIC::decodeArrayLoop(ByteBuffer &src, CharBuffer &dst) {
            ByteArray const &sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint const sl = src.arrayOffset() + src.limit();
            CharArray &da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint const dl = dst.arrayOffset() + dst.limit();

            try {
                // don't check dp/dl together here, it's possible to
                // decdoe a SO/SI without space in output buffer.
                while (sp < sl) {
                    gint const b1 = sa[sp] & 0xff;
                    gint inSize = 1;
                    if (b1 == SO) {  // Shift out
                        if (currentState != SBCS) {
                            src.setPosition(sp - src.arrayOffset());
                            dst.setPosition(dp - dst.arrayOffset());
                            return CoderResult::malformedForLength(1);
                        } else
                            currentState = DBCS;
                    } else if (b1 == SI) {
                        if (currentState != DBCS) {
                            src.setPosition(sp - src.arrayOffset());
                            dst.setPosition(dp - dst.arrayOffset());
                            return CoderResult::malformedForLength(1);
                        } else
                            currentState = SBCS;
                    } else {
                        gchar c;
                        if (currentState == SBCS) {
                            c = b2cSB[b1];
                            if (c == UNMAPPABLE_DECODING) {
                                src.setPosition(sp - src.arrayOffset());
                                dst.setPosition(dp - dst.arrayOffset());
                                return CoderResult::unmappableForLength(1);
                            }
                        } else {
                            if (sl - sp < 2)
                                return CoderResult::UNDERFLOW;
                            gint const b2 = sa[sp + 1] & 0xff;
                            if (b2 < b2Min || b2 > b2Max || (c = b2c[b1][b2 - b2Min]) == UNMAPPABLE_DECODING) {
                                src.setPosition(sp - src.arrayOffset());
                                dst.setPosition(dp - dst.arrayOffset());
                                if (!isDoubleByte(b1, b2))
                                    return CoderResult::malformedForLength(2);
                                return CoderResult::unmappableForLength(2);
                            }
                            inSize++;
                        }
                        if (dl - dp < 1) {
                            src.setPosition(sp - src.arrayOffset());
                            dst.setPosition(dp - dst.arrayOffset());
                            return CoderResult::OVERFLOW;
                        }

                        da[dp++] = c;
                    }
                    sp += inSize;
                }
                src.setPosition(sp - src.arrayOffset());
                dst.setPosition(dp - dst.arrayOffset());
                return CoderResult::UNDERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(sp - src.arrayOffset());
                dst.setPosition(dp - dst.arrayOffset());
                th.throws();
            }
        }

        gint DoubleByte::Decoder_EBCDIC::decode(const ByteArray &src, gint sp, gint len, CharArray &dst) {
            gint dp = 0;
            gint const sl = sp + len;
            currentState = SBCS;
            gchar const repl = replacement().charAt(0);
            while (sp < sl) {
                gint const b1 = src[sp++] & 0xff;
                if (b1 == SO) {  // Shift out
                    if (currentState != SBCS)
                        dst[dp++] = repl;
                    else
                        currentState = DBCS;
                } else if (b1 == SI) {
                    if (currentState != DBCS)
                        dst[dp++] = repl;
                    else
                        currentState = SBCS;
                } else {
                    gchar c = UNMAPPABLE_DECODING;
                    if (currentState == SBCS) {
                        c = b2cSB[b1];
                        if (c == UNMAPPABLE_DECODING)
                            c = repl;
                    } else {
                        if (sl == sp) {
                            c = repl;
                        } else {
                            gint const b2 = src[sp++] & 0xff;
                            if (b2 < b2Min || b2 > b2Max ||
                                (c = b2c[b1][b2 - b2Min]) == UNMAPPABLE_DECODING) {
                                c = repl;
                            }
                        }
                    }
                    dst[dp++] = c;
                }
            }
            return dp;
        }

        CoderResult DoubleByte::Decoder_EBCDIC::decodeBufferLoop(ByteBuffer &src, CharBuffer &dst) {
            gint mark = src.position();
            try {
                while (src.hasRemaining()) {
                    gint const b1 = src.get() & 0xff;
                    gint inSize = 1;
                    if (b1 == SO) {  // Shift out
                        if (currentState != SBCS)
                            return CoderResult::malformedForLength(1);
                        else
                            currentState = DBCS;
                    } else if (b1 == SI) {
                        if (currentState != DBCS)
                            return CoderResult::malformedForLength(1);
                        else
                            currentState = SBCS;
                    } else {
                        gchar c = UNMAPPABLE_DECODING;
                        if (currentState == SBCS) {
                            c = b2cSB[b1];
                            if (c == UNMAPPABLE_DECODING)
                                return CoderResult::unmappableForLength(1);
                        } else {
                            if (src.remaining() < 1)
                                return CoderResult::UNDERFLOW;
                            gint const b2 = src.get() & 0xff;
                            if (b2 < b2Min || b2 > b2Max ||
                                (c = b2c[b1][b2 - b2Min]) == UNMAPPABLE_DECODING) {
                                if (!isDoubleByte(b1, b2))
                                    return CoderResult::malformedForLength(2);
                                return CoderResult::unmappableForLength(2);
                            }
                            inSize++;
                        }

                        if (dst.remaining() < 1)
                            return CoderResult::OVERFLOW;

                        dst.put(c);
                    }
                    mark += inSize;
                }
                src.setPosition(mark);
                return CoderResult::UNDERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(mark);
                th.throws();
            }
        }

        CoderResult DoubleByte::Decoder_EUC_SIM::crMalformedOrUnderFlow(gint b) const {
            if (b == SS2 || b == SS3)
                return CoderResult::malformedForLength(1);
            return CoderResult::UNDERFLOW;
        }

        DoubleByte::Decoder_EUC_SIM::Decoder_EUC_SIM(const Charset &cs, CharArray2D &b2c, CharArray &b2cSB,
                                                     gint b2Min, gint b2Max, gbool isASCIICompatible) :
                Decoder(cs, b2c, b2cSB, b2Min, b2Max, isASCIICompatible) {}

        CoderResult DoubleByte::Decoder_EUC_SIM::crMalformedOrUnmappable(gint b1, gint b2) {
            CORE_IGNORE(b2);
            if (b1 == SS2 || b1 == SS3)
                return CoderResult::malformedForLength(1);
            return CoderResult::unmappableForLength(2);
        }

        gint DoubleByte::Decoder_EUC_SIM::decode(const ByteArray &src, gint sp, gint len, CharArray &dst) {
            gint dp = 0;
            gint const sl = sp + len;
            gchar const repl = replacement().charAt(0);
            while (sp < sl) {
                gint const b1 = src[sp++] & 0xff;
                gchar c = b2cSB[b1];
                if (c == UNMAPPABLE_DECODING) {
                    if (sp < sl) {
                        gint const b2 = src[sp++] & 0xff;
                        if (b2 < b2Min || b2 > b2Max ||
                            (c = b2c[b1][b2 - b2Min]) == UNMAPPABLE_DECODING) {
                            if (b1 == SS2 || b1 == SS3) {
                                sp--;
                            }
                            c = repl;
                        }
                    } else {
                        c = repl;
                    }
                }
                dst[dp++] = c;
            }
            return dp;
        }

        DoubleByte::Encoder::Encoder(const Charset &cs, CharArray &c2b, CharArray &c2bIndex) :
                Encoder(cs, c2b, c2bIndex, false) {}

        DoubleByte::Encoder::Encoder(const Charset &cs, CharArray &c2b, CharArray &c2bIndex, gbool isASCIICompatible) :
                CharsetEncoder(cs, 2.0F, 2.0F), isASCIICompatible(isASCIICompatible), c2bIndex(c2bIndex), c2b(c2b) {}

        DoubleByte::Encoder::Encoder(const Charset &cs, gfloat avg, gfloat max, const ByteArray &repl, CharArray &c2b,
                                     CharArray &c2bIndex, gbool isASCIICompatible) : CharsetEncoder(cs, avg, max, repl),
                                                                                     c2b(c2b), c2bIndex(c2bIndex),
                                                                                     isASCIICompatible(
                                                                                             isASCIICompatible) {}

        gbool DoubleByte::Encoder::canEncode(gchar c) {
            return encodeChar(c) != UNMAPPABLE_ENCODING;
        }

        CoderResult DoubleByte::Encoder::encodeArrayLoop(CharBuffer &src, ByteBuffer &dst) {
            CharArray const &sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint const sl = src.arrayOffset() + src.limit();

            ByteArray &da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint const dl = dst.arrayOffset() + dst.limit();

            try {
                if (isASCIICompatible) {
                    gint const n = Unicode::encodeASCII(sa, sp, da, dp, Math::min(dl - dp, sl - sp));
                    sp += n;
                    dp += n;
                }
                while (sp < sl) {
                    gchar const c = sa[sp];
                    gint const bb = encodeChar(c);
                    if (bb == UNMAPPABLE_ENCODING) {
                        if (Character::isSurrogate(c)) {
                            if (sgp.parse(c, sa, sp, sl) < 0)
                                return sgp.error();
                            return sgp.unmappableResult();
                        }
                        return CoderResult::unmappableForLength(1);
                    }

                    if (bb > MAX_SINGLEBYTE) {    // DoubleByte
                        if (dl - dp < 2)
                            return CoderResult::OVERFLOW;
                        da[dp++] = (gbyte) (bb >> 8);
                        da[dp++] = (gbyte) bb;
                    } else {                      // SingleByte
                        if (dl - dp < 1)
                            return CoderResult::OVERFLOW;
                        da[dp++] = (gbyte) bb;
                    }

                    sp++;
                }
                src.setPosition(sp - src.arrayOffset());
                dst.setPosition(dp - dst.arrayOffset());
                return CoderResult::UNDERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(sp - src.arrayOffset());
                dst.setPosition(dp - dst.arrayOffset());
                th.throws();
            }
        }

        CoderResult DoubleByte::Encoder::encodeBufferLoop(CharBuffer &src, ByteBuffer &dst) {
            gint mark = src.position();
            try {
                while (src.hasRemaining()) {
                    gchar const c = src.get();
                    gint const bb = encodeChar(c);
                    if (bb == UNMAPPABLE_ENCODING) {
                        if (Character::isSurrogate(c)) {
                            if (sgp.parse(c, src) < 0)
                                return sgp.error();
                            return sgp.unmappableResult();
                        }
                        return CoderResult::unmappableForLength(1);
                    }
                    if (bb > MAX_SINGLEBYTE) {  // DoubleByte
                        if (dst.remaining() < 2)
                            return CoderResult::OVERFLOW;
                        dst.put((gbyte) (bb >> 8));
                        dst.put((gbyte) (bb));
                    } else {
                        if (dst.remaining() < 1)
                            return CoderResult::OVERFLOW;
                        dst.put((gbyte) bb);
                    }
                    mark++;
                }
                src.setPosition(mark);
                return CoderResult::UNDERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(mark);
                th.throws();
            }
        }

        CoderResult DoubleByte::Encoder::encodeLoop(CharBuffer &src, ByteBuffer &dst) {
            if (src.hasArray() && dst.hasArray())
                return encodeArrayLoop(src, dst);
            else
                return encodeBufferLoop(src, dst);
        }

        void DoubleByte::Encoder::replaceWith0(const ByteArray &newReplacement) {
            repl = newReplacement;
        }

        gint DoubleByte::Encoder::encode(const CharArray &src, gint sp, gint len, ByteArray &dst) {
            gint dp = 0;
            gint const sl = sp + len;
            if (isASCIICompatible) {
                gint const n = Unicode::encodeASCII(src, sp, dst, dp, len);
                sp += n;
                dp += n;
            }
            while (sp < sl) {
                gchar const c = src[sp++];
                gint const bb = encodeChar(c);
                if (bb == UNMAPPABLE_ENCODING) {
                    if (Character::isHighSurrogate(c) && sp < sl && Character::isLowSurrogate(src[sp])) {
                        sp++;
                    }
                    dst[dp++] = repl[0];
                    if (repl.length() > 1)
                        dst[dp++] = repl[1];
                    continue;
                } //else
                if (bb > MAX_SINGLEBYTE) { // DoubleByte
                    dst[dp++] = (gbyte) (bb >> 8);
                    dst[dp++] = (gbyte) bb;
                } else {                          // SingleByte
                    dst[dp++] = (gbyte) bb;
                }
            }
            return dp;
        }

        gint DoubleByte::Encoder::encodeFromLatin1(const ByteArray &src, gint sp, gint len, ByteArray &dst) {
            gint dp = 0;
            gint const sl = sp + len;
            while (sp < sl) {
                gchar const c = (gchar) (src[sp++] & 0xff);
                gint const bb = encodeChar(c);
                if (bb == UNMAPPABLE_ENCODING) {
                    // no surrogate pair in latin1 string
                    dst[dp++] = repl[0];
                    if (repl.length() > 1) {
                        dst[dp++] = repl[1];
                    }
                    continue;
                } //else
                if (bb > MAX_SINGLEBYTE) { // DoubleByte
                    dst[dp++] = (gbyte) (bb >> 8);
                    dst[dp++] = (gbyte) bb;
                } else {                   // SingleByte
                    dst[dp++] = (gbyte) bb;
                }

            }
            return dp;
        }

        gint DoubleByte::Encoder::encodeFromUTF16(const ByteArray &src, gint sp, gint len, ByteArray &dst) {
            gint dp = 0;
            gint const sl = sp + len;
            while (sp < sl) {
                gchar const c = SingleByte::Encoder::getChar(src, sp++);
                gint const bb = encodeChar(c);
                if (bb == UNMAPPABLE_ENCODING) {
                    if (Character::isHighSurrogate(c) && sp < sl &&
                        Character::isLowSurrogate(SingleByte::Encoder::getChar(src, sp))) {
                        sp++;
                    }
                    dst[dp++] = repl[0];
                    if (repl.length() > 1) {
                        dst[dp++] = repl[1];
                    }
                    continue;
                } //else
                if (bb > MAX_SINGLEBYTE) { // DoubleByte
                    dst[dp++] = (gbyte) (bb >> 8);
                    dst[dp++] = (gbyte) bb;
                } else {                   // SingleByte
                    dst[dp++] = (gbyte) bb;
                }
            }
            return dp;
        }

        gint DoubleByte::Encoder::encodeChar(gchar ch) {
            return c2b[c2bIndex[ch >> 8] + (ch & 0xff)];
        }

        void DoubleByte::Encoder::initC2B(const StringArray &b2c, const String &b2cSB, const String &b2cNR,
                                          const String &c2bNR, gint b2Min, gint b2Max, CharArray c2b,
                                          CharArray c2bIndex) {
            for (gchar &c: c2b) {
                c = (gchar) UNMAPPABLE_ENCODING;
            }
            gint off = 0x100;

            CharArray2D b2c_ca = CharArray2D(b2c.length());
            CharArray b2cSB_ca = {};
            if (!b2cSB.isEmpty())
                b2cSB_ca = b2cSB.chars();

            for (gint i = 0; i < b2c.length(); i++) {
                if (b2c[i].isEmpty())
                    continue;
                b2c_ca.set(i, b2c[i].chars());
            }

            if (!b2cNR.isEmpty()) {
                gint j = 0;
                while (j < b2cNR.length()) {
                    gchar const b = b2cNR.charAt(j++);
                    gchar const c = b2cNR.charAt(j++);
                    if (b < 0x100 && b2cSB_ca != null) {
                        if (b2cSB_ca[b] == c)
                            b2cSB_ca[b] = UNMAPPABLE_DECODING;
                    } else {
                        if (b2c_ca[b >> 8][(b & 0xff) - b2Min] == c)
                            b2c_ca[b >> 8][(b & 0xff) - b2Min] = UNMAPPABLE_DECODING;
                    }
                }
            }

            if (b2cSB_ca != null) {      // SingleByte
                for (gint b = 0; b < b2cSB_ca.length(); b++) {
                    gchar const c = b2cSB_ca[b];
                    if (c == UNMAPPABLE_DECODING)
                        continue;
                    gint index = c2bIndex[c >> 8];
                    if (index == 0) {
                        index = off;
                        off += 0x100;
                        c2bIndex[c >> 8] = (gchar) index;
                    }
                    c2b[index + (c & 0xff)] = (gchar) b;
                }
            }

            for (gint b1 = 0; b1 < b2c.length(); b1++) {  // DoubleByte
                CharArray &db = b2c_ca[b1];
                if (db.isEmpty())
                    continue;
                for (gint b2 = b2Min; b2 <= b2Max; b2++) {
                    gchar const c = db[b2 - b2Min];
                    if (c == UNMAPPABLE_DECODING)
                        continue;
                    gint index = c2bIndex[c >> 8];
                    if (index == 0) {
                        index = off;
                        off += 0x100;
                        c2bIndex[c >> 8] = (gchar) index;
                    }
                    c2b[index + (c & 0xff)] = (gchar) ((b1 << 8) | b2);
                }
            }

            if (!c2bNR.isEmpty()) {
                // add c->b only nr entries
                for (gint i = 0; i < c2bNR.length(); i += 2) {
                    gchar const b = c2bNR.charAt(i);
                    gchar const c = c2bNR.charAt(i + 1);
                    gint index = (c >> 8);
                    if (c2bIndex[index] == 0) {
                        c2bIndex[index] = (gchar) off;
                        off += 0x100;
                    }
                    index = c2bIndex[index] + (c & 0xff);
                    c2b[index] = b;
                }
            }
        }

        Object &DoubleByte::Encoder::clone() const {
            return charset().encoder();
        }

        DoubleByte::Encoder_DBCSONLY::Encoder_DBCSONLY(const Charset &cs, const ByteArray &repl, CharArray &c2b,
                                                       CharArray &c2bIndex, gbool isASCIICompatible) :
                Encoder(cs, 2.0F, 2.0F, repl, c2b, c2bIndex, isASCIICompatible) {}

        gint DoubleByte::Encoder_DBCSONLY::encodeChar(gchar ch) {
            gint const bb = Encoder::encodeChar(ch);
            if (bb <= MAX_SINGLEBYTE)
                return UNMAPPABLE_ENCODING;
            return bb;
        }

        DoubleByte::Encoder_EBCDIC::Encoder_EBCDIC(const Charset &cs, CharArray &c2b, CharArray &c2bIndex,
                                                   gbool isASCIICompatible) :
                Encoder(cs, 4.0F, 5.0F, ByteArray::of((gbyte) 0x6f), c2b, c2bIndex, isASCIICompatible) {}

        void DoubleByte::Encoder_EBCDIC::reset0() {
            currentState = SBCS;
        }

        CoderResult DoubleByte::Encoder_EBCDIC::flush0(ByteBuffer &out) {
            if (currentState == DBCS) {
                if (out.remaining() < 1)
                    return CoderResult::OVERFLOW;
                out.put(SI);
            }
            reset0();
            return CoderResult::UNDERFLOW;
        }

        CoderResult DoubleByte::Encoder_EBCDIC::encodeArrayLoop(CharBuffer &src, ByteBuffer &dst) {
            CharArray const &sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint const sl = src.arrayOffset() + src.limit();
            ByteArray &da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint const dl = dst.arrayOffset() + dst.limit();

            try {
                while (sp < sl) {
                    gchar const c = sa[sp];
                    gint const bb = encodeChar(c);
                    if (bb == UNMAPPABLE_ENCODING) {
                        if (Character::isSurrogate(c)) {
                            if (sgp.parse(c, sa, sp, sl) < 0)
                                return sgp.error();
                            return sgp.unmappableResult();
                        }
                        return CoderResult::unmappableForLength(1);
                    }
                    if (bb > MAX_SINGLEBYTE) {  // DoubleByte
                        if (currentState == SBCS) {
                            if (dl - dp < 1)
                                return CoderResult::OVERFLOW;
                            currentState = DBCS;
                            da[dp++] = SO;
                        }
                        if (dl - dp < 2)
                            return CoderResult::OVERFLOW;
                        da[dp++] = (gbyte) (bb >> 8);
                        da[dp++] = (gbyte) bb;
                    } else {                    // SingleByte
                        if (currentState == DBCS) {
                            if (dl - dp < 1)
                                return CoderResult::OVERFLOW;
                            currentState = SBCS;
                            da[dp++] = SI;
                        }
                        if (dl - dp < 1)
                            return CoderResult::OVERFLOW;
                        da[dp++] = (gbyte) bb;

                    }
                    sp++;
                }
                src.setPosition(sp - src.arrayOffset());
                dst.setPosition(dp - dst.arrayOffset());
                return CoderResult::UNDERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(sp - src.arrayOffset());
                dst.setPosition(dp - dst.arrayOffset());
                th.throws();
            }
        }

        CoderResult DoubleByte::Encoder_EBCDIC::encodeBufferLoop(CharBuffer &src, ByteBuffer &dst) {
            gint mark = src.position();
            try {
                while (src.hasRemaining()) {
                    gchar const c = src.get();
                    gint const bb = encodeChar(c);
                    if (bb == UNMAPPABLE_ENCODING) {
                        if (Character::isSurrogate(c)) {
                            if (sgp.parse(c, src) < 0)
                                return sgp.error();
                            return sgp.unmappableResult();
                        }
                        return CoderResult::unmappableForLength(1);
                    }
                    if (bb > MAX_SINGLEBYTE) {  // DoubleByte
                        if (currentState == SBCS) {
                            if (dst.remaining() < 1)
                                return CoderResult::OVERFLOW;
                            currentState = DBCS;
                            dst.put(SO);
                        }
                        if (dst.remaining() < 2)
                            return CoderResult::OVERFLOW;
                        dst.put((gbyte) (bb >> 8));
                        dst.put((gbyte) (bb));
                    } else {                  // Single-gbyte
                        if (currentState == DBCS) {
                            if (dst.remaining() < 1)
                                return CoderResult::OVERFLOW;
                            currentState = SBCS;
                            dst.put(SI);
                        }
                        if (dst.remaining() < 1)
                            return CoderResult::OVERFLOW;
                        dst.put((gbyte) bb);
                    }
                    mark++;
                }
                src.setPosition(mark);
                return CoderResult::UNDERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(mark);
                th.throws();
            }
        }

        gint DoubleByte::Encoder_EBCDIC::encode(const CharArray &src, gint sp, gint len, ByteArray &dst) {
            gint dp = 0;
            gint const sl = sp + len;
            while (sp < sl) {
                gchar const c = src[sp++];
                gint const bb = encodeChar(c);

                if (bb == UNMAPPABLE_ENCODING) {
                    if (Character::isHighSurrogate(c) && sp < sl &&
                        Character::isLowSurrogate(src[sp])) {
                        sp++;
                    }
                    dst[dp++] = repl[0];
                    if (repl.length() > 1)
                        dst[dp++] = repl[1];
                    continue;
                } //else
                if (bb > MAX_SINGLEBYTE) {           // DoubleByte
                    if (currentState == SBCS) {
                        currentState = DBCS;
                        dst[dp++] = SO;
                    }
                    dst[dp++] = (gbyte) (bb >> 8);
                    dst[dp++] = (gbyte) bb;
                } else {                             // SingleByte
                    if (currentState == DBCS) {
                        currentState = SBCS;
                        dst[dp++] = SI;
                    }
                    dst[dp++] = (gbyte) bb;
                }
            }

            if (currentState == DBCS) {
                currentState = SBCS;
                dst[dp++] = SI;
            }
            return dp;
        }

        gint DoubleByte::Encoder_EBCDIC::encodeFromLatin1(const ByteArray &src, gint sp, gint len, ByteArray &dst) {
            gint dp = 0;
            gint const sl = sp + len;
            while (sp < sl) {
                gchar const c = (gchar) (src[sp++] & 0xff);
                gint const bb = encodeChar(c);
                if (bb == UNMAPPABLE_ENCODING) {
                    // no surrogate pair in latin1 string
                    dst[dp++] = repl[0];
                    if (repl.length() > 1)
                        dst[dp++] = repl[1];
                    continue;
                } //else
                if (bb > MAX_SINGLEBYTE) {           // DoubleByte
                    if (currentState == SBCS) {
                        currentState = DBCS;
                        dst[dp++] = SO;
                    }
                    dst[dp++] = (gbyte) (bb >> 8);
                    dst[dp++] = (gbyte) bb;
                } else {                             // SingleByte
                    if (currentState == DBCS) {
                        currentState = SBCS;
                        dst[dp++] = SI;
                    }
                    dst[dp++] = (gbyte) bb;
                }
            }
            if (currentState == DBCS) {
                currentState = SBCS;
                dst[dp++] = SI;
            }
            return dp;
        }

        gint DoubleByte::Encoder_EBCDIC::encodeFromUTF16(const ByteArray &src, gint sp, gint len, ByteArray &dst) {
            gint dp = 0;
            gint const sl = sp + len;
            while (sp < sl) {
                gchar const c = SingleByte::Encoder::getChar(src, sp++);
                gint const bb = encodeChar(c);
                if (bb == UNMAPPABLE_ENCODING) {
                    if (Character::isHighSurrogate(c) && sp < sl &&
                        Character::isLowSurrogate(SingleByte::Encoder::getChar(src, sp))) {
                        sp++;
                    }
                    dst[dp++] = repl[0];
                    if (repl.length() > 1)
                        dst[dp++] = repl[1];
                    continue;
                } //else
                if (bb > MAX_SINGLEBYTE) {           // DoubleByte
                    if (currentState == SBCS) {
                        currentState = DBCS;
                        dst[dp++] = SO;
                    }
                    dst[dp++] = (gbyte) (bb >> 8);
                    dst[dp++] = (gbyte) bb;
                } else {                             // SingleByte
                    if (currentState == DBCS) {
                        currentState = SBCS;
                        dst[dp++] = SI;
                    }
                    dst[dp++] = (gbyte) bb;
                }
            }
            if (currentState == DBCS) {
                currentState = SBCS;
                dst[dp++] = SI;
            }
            return dp;
        }
    } // charset
} // core
