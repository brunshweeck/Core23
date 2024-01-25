//
// Created by T.N.Brunshweeck on 14/01/2024.
//

#include "US_ASCII.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        using namespace native;

        US_ASCII US_ASCII::INSTANCE{};

        US_ASCII::US_ASCII() :
                Charset("US-ASCII",
                        StringArray::of("iso-ir-6", "ANSI_X3.4-1986", "ISO_646.irv:1991", "ASCII", "ISO646-US", "us",
                                        "IBM367", "cp367", "csASCII", "646", "iso_646.irv:1983", "ANSI_X3.4-1968",
                                        "ascii7")) {}

        gbool US_ASCII::contains(const Charset &cs) const {
            return Class<US_ASCII>::hasInstance(cs);
        }

        CharsetDecoder &US_ASCII::decoder() const {
            return Unsafe::allocateInstance<Decoder>(*this);
        }

        CharsetEncoder &US_ASCII::encoder() const {
            return Unsafe::allocateInstance<Encoder>(*this);
        }

        Object &US_ASCII::clone() const {
            return INSTANCE;
        }

        US_ASCII::Decoder::Decoder(const Charset &cs) : CharsetDecoder(cs, 1.0f, 1.0f) {}

        CoderResult US_ASCII::Decoder::decodeArrayLoop(ByteBuffer &src, CharBuffer &dst) {
            ByteArray sa = src.array();
            gint soff = src.arrayOffset();
            gint sp = soff + src.position();
            gint sl = soff + src.limit();

            CharArray da = dst.array();
            gint doff = dst.arrayOffset();
            gint dp = doff + dst.position();
            gint dl = doff + dst.limit();

            // ASCII only loop
            gint n = Unicode::decodeASCII(sa, sp, da, dp, Math::min(sl - sp, dl - dp));
            sp += n;
            dp += n;
            src.setPosition(sp - soff);
            dst.setPosition(dp - doff);
            if (sp < sl) {
                if (dp >= dl) {
                    return CoderResult::OVERFLOW;
                }
                return CoderResult::malformedForLength(1);
            }
            return CoderResult::UNDERFLOW;
        }

        CoderResult US_ASCII::Decoder::decodeBufferLoop(ByteBuffer &src, CharBuffer &dst) {
            gint mark = src.position();
            try {
                while (src.hasRemaining()) {
                    gbyte b = src.get();
                    if (b >= 0) {
                        if (!dst.hasRemaining())
                            return CoderResult::OVERFLOW;
                        dst.put((gchar) b);
                        mark++;
                        continue;
                    }
                    return CoderResult::malformedForLength(1);
                }
                src.setPosition(mark);
                return CoderResult::UNDERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(mark);
                th.throws(__trace("core.charset.US_ASCII"));
            }
        }

        CoderResult US_ASCII::Decoder::decodeLoop(ByteBuffer &src, CharBuffer &dst) {
            if (src.hasArray() && dst.hasArray())
                return decodeArrayLoop(src, dst);
            else
                return decodeBufferLoop(src, dst);
        }

        Object &US_ASCII::Decoder::clone() const {
            return INSTANCE.decoder();
        }

        US_ASCII::Encoder::Encoder(const Charset &cs) : CharsetEncoder(cs, 1.0f, 1.0f) {}

        gbool US_ASCII::Encoder::canEncode(gchar c) {
            return c < 0x80;
        }

        gbool US_ASCII::Encoder::isLegalReplacement(const ByteArray &repl) const {
            return (repl.length() == 1 && repl[0] >= 0) || CharsetEncoder::isLegalReplacement(repl);
        }

        CoderResult US_ASCII::Encoder::encodeArrayLoop(CharBuffer &src, ByteBuffer &dst) {
            CharArray sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint sl = src.arrayOffset() + src.limit();
            CORE_ASSERT(sp <= sl, "core.charset.US_ASCII");
            sp = (sp <= sl ? sp : sl);
            ByteArray da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint dl = dst.arrayOffset() + dst.limit();
            CORE_ASSERT(dp <= dl, "core.charset.US_ASCII");
            dp = (dp <= dl ? dp : dl);

            gint n = Unicode::encodeASCII(sa, sp, da, dp, Math::min(sl - sp, dl - dp));
            sp += n;
            dp += n;

            try {
                while (sp < sl) {
                    gchar c = sa[sp];
                    if (c < 0x80) {
                        if (dp >= dl)
                            return CoderResult::OVERFLOW;
                        da[dp] = (gbyte) c;
                        sp++;
                        dp++;
                        continue;
                    }
                    if (sgp.parse(c, sa, sp, sl) < 0) {
                        return sgp.error();
                    }
                    return sgp.unmappableResult();
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

        CoderResult US_ASCII::Encoder::encodeBufferLoop(CharBuffer &src, ByteBuffer &dst) {
            gint mark = src.position();
            try {
                while (src.hasRemaining()) {
                    gchar c = src.get();
                    if (c < 0x80) {
                        if (!dst.hasRemaining())
                            return CoderResult::OVERFLOW;
                        dst.put((gbyte) c);
                        mark++;
                        continue;
                    }
                    if (sgp.parse(c, src) < 0)
                        return sgp.error();
                    return sgp.unmappableResult();
                }
                return CoderResult::UNDERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(mark);
                th.throws(__trace("core.charset.US_ASCII"));
            }
        }

        CoderResult US_ASCII::Encoder::encodeLoop(CharBuffer &src, ByteBuffer &dst) {
            if (src.hasArray() && dst.hasArray())
                return encodeArrayLoop(src, dst);
            else
                return encodeBufferLoop(src, dst);
        }

        Object &US_ASCII::Encoder::clone() const {
            return INSTANCE.encoder();
        }
    } // charset
} // core