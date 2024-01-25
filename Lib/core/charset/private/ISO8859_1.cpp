//
// Created by T.N.Brunshweeck on 17/01/2024.
//

#include "ISO8859_1.h"
#include <core/charset/private/US_ASCII.h>
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        using namespace native;

        ISO8859_1 ISO8859_1::INSTANCE{};

        ISO8859_1::ISO8859_1() :
                Charset("ISO-8859-1",
                        StringArray::of("iso-ir-100", "ISO_8859-1", "latin1", "l1", "IBM819", "cp819", "csISOLatin1",
                                        "819", "IBM-819", "ISO8859_1", "ISO_8859-1:1987", "ISO_8859_1", "8859_1",
                                        "ISO8859-1")) {}

        gbool ISO8859_1::contains(const Charset &cs) const {
            return Class<US_ASCII>::hasInstance(cs) ||
                   Class<ISO8859_1>::hasInstance(cs);
        }

        CharsetDecoder &ISO8859_1::decoder() const {
            return Unsafe::allocateInstance<Decoder>();
        }

        CharsetEncoder &ISO8859_1::encoder() const {
            return Unsafe::allocateInstance<Encoder>();
        }

        Object &ISO8859_1::clone() const {
            return INSTANCE;
        }


        ISO8859_1::Decoder::Decoder() : CharsetDecoder(INSTANCE, 1.0F, 1.0F) {}

        CoderResult ISO8859_1::Decoder::decodeArrayLoop(ByteBuffer &src, CharBuffer &dst) {
            ByteArray const &sa = src.array();
            gint const soff = src.arrayOffset();
            gint sp = soff + src.position();
            gint const sl = soff + src.limit();

            CharArray &da = dst.array();
            gint const doff = dst.arrayOffset();
            gint dp = doff + dst.position();
            gint const dl = doff + dst.limit();

            gint const decodeLen = Math::min(sl - sp, dl - dp);
            Unicode::inflate(sa, sp, da, dp, decodeLen);
            sp += decodeLen;
            dp += decodeLen;
            src.setPosition(sp - soff);
            dst.setPosition(dp - doff);
            if (sl - sp > dl - dp) {
                return CoderResult::OVERFLOW;
            }
            return CoderResult::UNDERFLOW;
        }

        CoderResult ISO8859_1::Decoder::decodeBufferLoop(ByteBuffer &src, CharBuffer &dst) {
            gint mark = src.position();
            try {
                while (src.hasRemaining()) {
                    gbyte const b = src.get();
                    if (!dst.hasRemaining())
                        return CoderResult::OVERFLOW;
                    dst.put((gchar) (b & 0xff));
                    mark++;
                }
                src.setPosition(mark);
                return CoderResult::UNDERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(mark);
                th.throws();
            }
        }

        CoderResult ISO8859_1::Decoder::decodeLoop(ByteBuffer &src, CharBuffer &dst) {
            if (src.hasArray() && dst.hasArray())
                return decodeArrayLoop(src, dst);
            else
                return decodeBufferLoop(src, dst);
        }

        Object &ISO8859_1::Decoder::clone() const {
            return INSTANCE.decoder();
        }

        gbool ISO8859_1::Encoder::canEncode(gchar c) {
            return c <= u'\u00FF';
        }

        gbool ISO8859_1::Encoder::isLegalReplacement(const ByteArray &repl) const {
            return true;  // we accept any gbyte value
        }

        gint ISO8859_1::Encoder::encodeISOArray(const CharArray &sa, gint sp, ByteArray &da, gint dp, gint len) {
            if (len <= 0) {
                return 0;
            }
            encodeISOArrayCheck(sa, sp, da, dp, len);
            return encodeISOArray0(sa, sp, da, dp, len);
        }

        gint ISO8859_1::Encoder::encodeISOArray0(const CharArray &sa, gint sp, ByteArray &da, gint dp, gint len) {
            gint i = 0;
            for (; i < len; i++) {
                gchar const c = sa[sp++];
                if (c > u'\u00FF')
                    break;
                da[dp++] = (gbyte) c;
            }
            return i;
        }

        void ISO8859_1::Encoder::encodeISOArrayCheck(const CharArray &sa, gint sp, ByteArray &da, gint dp, gint len) {
            try {
                Preconditions::checkIndex(sp, sa.length());
                Preconditions::checkIndex(dp, da.length());

                Preconditions::checkIndex(sp + len - 1, sa.length());
                Preconditions::checkIndex(dp + len - 1, da.length());
            } catch (const Throwable &th) {
                th.throws(__trace("core.charset.ISO8859_1"));
            }
        }

        CoderResult ISO8859_1::Encoder::encodeArrayLoop(CharBuffer &src, ByteBuffer &dst) {
            CharArray const &sa = src.array();
            gint const soff = src.arrayOffset();
            gint sp = soff + src.position();
            gint const sl = soff + src.limit();
            CORE_ASSERT(sp <= sl, "");
            sp = (sp <= sl ? sp : sl);
            ByteArray &da = dst.array();
            gint const doff = dst.arrayOffset();
            gint dp = doff + dst.position();
            gint const dl = doff + dst.limit();
            CORE_ASSERT(dp <= dl, "");
            dp = (dp <= dl ? dp : dl);
            gint const dlen = dl - dp;
            gint const slen = sl - sp;
            gint const len = (dlen < slen) ? dlen : slen;
            try {
                gint const ret = encodeISOArray(sa, sp, da, dp, len);
                sp = sp + ret;
                dp = dp + ret;
                if (ret != len) {
                    if (sgp.parse(sa[sp], sa, sp, sl) < 0)
                        return sgp.error();
                    return sgp.unmappableResult();
                }
                src.setPosition(sp - soff);
                dst.setPosition(dp - doff);
                if (len < slen)
                    return CoderResult::OVERFLOW;
                return CoderResult::UNDERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(sp - soff);
                dst.setPosition(dp - doff);
                th.throws();
            }
        }

        CoderResult ISO8859_1::Encoder::encodeBufferLoop(CharBuffer &src, ByteBuffer &dst) {
            gint mark = src.position();
            try {
                while (src.hasRemaining()) {
                    gchar c = src.get();
                    if (c <= u'\u00FF') {
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
                src.setPosition(mark);
                return CoderResult::UNDERFLOW;
            } catch (const Throwable &th) {
                src.setPosition(mark);
                th.throws();
            }
        }

        CoderResult ISO8859_1::Encoder::encodeLoop(CharBuffer &src, ByteBuffer &dst) {
            if (src.hasArray() && dst.hasArray())
                return encodeArrayLoop(src, dst);
            else
                return encodeBufferLoop(src, dst);
        }

        Object &ISO8859_1::Encoder::clone() const {
            return INSTANCE.encoder();
        }
    } // charset
} // core