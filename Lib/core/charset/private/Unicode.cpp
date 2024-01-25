//
// Created by T.N.Brunshweeck on 14/01/2024.
//

#include "Unicode.h"
#include <core/private/Unsafe.h>
#include <core/charset/private/US_ASCII.h>
#include <core/charset/private/ISO8859_1.h>
#include <core/charset/private/ISO8859_15.h>
#include <core/charset/private/ISO8859_16.h>
#include <core/charset/private/Windows1252.h>
#include <core/charset/private/UTF8.h>
#include <core/charset/private/UTF16.h>
#include <core/charset/private/UTF16BE.h>
#include <core/charset/private/UTF16LE.h>
#include <core/charset/private/UTF16LE_BOM.h>

namespace core {
    namespace charset {


        using namespace native;

        Unicode::Unicode(String canonicalName, StringArray aliases) :
                Charset(Unsafe::moveInstance(canonicalName), Unsafe::moveInstance(aliases)) {}

        gbool Unicode::contains(const Charset &cs) const {
            return (
                    (Class<US_ASCII>::hasInstance(cs))
                    || (Class<ISO8859_1>::hasInstance(cs))
                    || (Class<ISO8859_15>::hasInstance(cs))
                    || (Class<ISO8859_16>::hasInstance(cs))
                    || (Class<Windows1252>::hasInstance(cs))
                    || (Class<UTF8>::hasInstance(cs))
                    || (Class<UTF16>::hasInstance(cs))
                    || (Class<UTF16BE>::hasInstance(cs))
                    || (Class<UTF16LE>::hasInstance(cs))
                    || (Class<UTF16LE_BOM>::hasInstance(cs))
                    || (cs.name().equals("GBK"))
                    || (cs.name().equals("GB18030"))
                    || (cs.name().equals("ISO-8859-2"))
                    || (cs.name().equals("ISO-8859-3"))
                    || (cs.name().equals("ISO-8859-4"))
                    || (cs.name().equals("ISO-8859-5"))
                    || (cs.name().equals("ISO-8859-6"))
                    || (cs.name().equals("ISO-8859-7"))
                    || (cs.name().equals("ISO-8859-8"))
                    || (cs.name().equals("ISO-8859-9"))
                    || (cs.name().equals("ISO-8859-13"))
                    || (cs.name().equals("JIS_X0201"))
                    || (cs.name().equals("x-JIS0208"))
                    || (cs.name().equals("JIS_X0212-1990"))
                    || (cs.name().equals("GB2312"))
                    || (cs.name().equals("EUC-KR"))
                    || (cs.name().equals("x-EUC-TW"))
                    || (cs.name().equals("EUC-JP"))
                    || (cs.name().equals("x-euc-jp-linux"))
                    || (cs.name().equals("KOI8-R"))
                    || (cs.name().equals("TIS-620"))
                    || (cs.name().equals("x-ISCII91"))
                    || (cs.name().equals("windows-1251"))
                    || (cs.name().equals("windows-1253"))
                    || (cs.name().equals("windows-1254"))
                    || (cs.name().equals("windows-1255"))
                    || (cs.name().equals("windows-1256"))
                    || (cs.name().equals("windows-1257"))
                    || (cs.name().equals("windows-1258"))
                    || (cs.name().equals("windows-932"))
                    || (cs.name().equals("x-mswin-936"))
                    || (cs.name().equals("x-windows-949"))
                    || (cs.name().equals("x-windows-950"))
                    || (cs.name().equals("windows-31j"))
                    || (cs.name().equals("Big5"))
                    || (cs.name().equals("Big5-HKSCS"))
                    || (cs.name().equals("x-MS950-HKSCS"))
                    || (cs.name().equals("ISO-2022-JP"))
                    || (cs.name().equals("ISO-2022-KR"))
                    || (cs.name().equals("x-ISO-2022-CN-CNS"))
                    || (cs.name().equals("x-ISO-2022-CN-GB"))
                    || (cs.name().equals("x-Johab"))
                    || (cs.name().equals("Shift_JIS")));
        }

        gint Unicode::countPositives(const ByteArray &ba, gint off, gint len) {
            gint const limit = off + len;
            for (gint i = off; i < limit; i++) {
                if (ba[i] < 0) {
                    return i - off;
                }
            }
            return len;
        }

        void Unicode::inflate(const ByteArray &src, gint srcOff, CharArray &dst, gint dstOff, gint len) {
            for (gint i = 0; i < len; i++) {
                dst[dstOff++] = (char) (src[srcOff++] & 0xff);
            }
        }

        gint Unicode::decodeASCII(const ByteArray &src, gint srcOff, CharArray &dst, gint dstOff, gint len) {
            gint count = countPositives(src, srcOff, len);
            while (count < len) {
                if (src[srcOff + count] < 0) {
                    break;
                }
                count++;
            }
            inflate(src, srcOff, dst, dstOff, len);
            return count;
        }

        gint Unicode::encodeASCII(const CharArray &src, gint srcOff, ByteArray &dst, gint dstOff, gint len) {
            gint i = 0;
            for (; i < len; i++) {
                gchar const c = src[srcOff++];
                if (c >= u'\u0080')
                    break;
                dst[dstOff++] = (gbyte) c;
            }
            return i;
        }

        Unicode::Decoder::Decoder(const Charset &cs, gint bo) :
                CharsetDecoder(cs, 0.5F, 1.0F), expectedByteOrder(bo), currentByteOrder(bo) {}

        Unicode::Decoder::Decoder(const Charset &cs, gint bo, gint defaultBO) : Decoder(cs, bo) {
            defaultByteOrder = defaultBO;
        }

        gchar Unicode::Decoder::decode(gint b1, gint b2) const {
            if (currentByteOrder == BIG)
                return (gchar) ((b1 << 8) | b2);
            else
                return (gchar) ((b2 << 8) | b1);
        }

        CoderResult Unicode::Decoder::decodeLoop(ByteBuffer &src, CharBuffer &dst) {
            gint mark = src.position();

            try {
                while (src.remaining() > 1) {
                    gint const b1 = src.get() & 0xff;
                    gint const b2 = src.get() & 0xff;

                    // Byte Order Mark interpretation
                    if (currentByteOrder == NONE) {
                        gchar const c = (gchar) ((b1 << 8) | b2);
                        if (c == BYTE_ORDER_MARK) {
                            currentByteOrder = BIG;
                            mark += 2;
                            continue;
                        } else if (c == REVERSED_MARK) {
                            currentByteOrder = LITTLE;
                            mark += 2;
                            continue;
                        } else {
                            currentByteOrder = defaultByteOrder;
                            // FALL THROUGH to process b1, b2 normally
                        }
                    }

                    gchar const c = decode(b1, b2);

                    // Surrogates
                    if (Character::isSurrogate(c)) {
                        if (Character::isHighSurrogate(c)) {
                            if (src.remaining() < 2)
                                return CoderResult::UNDERFLOW;
                            gchar const c2 = decode(src.get() & 0xff, src.get() & 0xff);
                            if (!Character::isLowSurrogate(c2))
                                return CoderResult::malformedForLength(4);
                            if (dst.remaining() < 2)
                                return CoderResult::OVERFLOW;
                            mark += 4;
                            dst.put(c);
                            dst.put(c2);
                            continue;
                        }
                        // Unpaired low surrogate
                        return CoderResult::malformedForLength(2);
                    }

                    if (!dst.hasRemaining())
                        return CoderResult::OVERFLOW;
                    mark += 2;
                    dst.put(c);

                }
                src.setPosition(mark);
                return CoderResult::UNDERFLOW;

            } catch (const Throwable &thr) {
                src.setPosition(mark);
                thr.throws(__trace("core.charset.Unicode.Decoder"));
            }
        }

        void Unicode::Decoder::reset0() {
            currentByteOrder = expectedByteOrder;
        }

        Unicode::Encoder::Encoder(const Charset &cs, gint bo, gbool m) :
                CharsetEncoder(cs, 2.0F, m ? 4.0F : 2.0F, ((bo == BIG) ?
                                                           ByteArray::of((gbyte) 0xff, (gbyte) 0xfd) :
                                                           ByteArray::of((gbyte) 0xfd, (gbyte) 0xff))),
                usesMark(m), needsMark(m), byteOrder(bo) {}

        void Unicode::Encoder::put(gchar c, ByteBuffer &dst) const {
            if (byteOrder == BIG) {
                dst.put((gbyte) (c >> 8));
                dst.put((gbyte) (c & 0xff));
            } else {
                dst.put((gbyte) (c & 0xff));
                dst.put((gbyte) (c >> 8));
            }
        }

        CoderResult Unicode::Encoder::encodeLoop(CharBuffer &src, ByteBuffer &dst) {
            gint mark = src.position();

            if (needsMark && src.hasRemaining()) {
                if (dst.remaining() < 2)
                    return CoderResult::OVERFLOW;
                put(BYTE_ORDER_MARK, dst);
                needsMark = false;
            }
            try {
                while (src.hasRemaining()) {
                    gchar const c = src.get();
                    if (!Character::isSurrogate(c)) {
                        if (dst.remaining() < 2)
                            return CoderResult::OVERFLOW;
                        mark++;
                        put(c, dst);
                        continue;
                    }
                    gint const d = sgp.parse(c, src);
                    if (d < 0)
                        return sgp.error();
                    if (dst.remaining() < 4)
                        return CoderResult::OVERFLOW;
                    mark += 2;
                    put(Character::highSurrogate(d), dst);
                    put(Character::lowSurrogate(d), dst);
                }
                src.setPosition(mark);
                return CoderResult::UNDERFLOW;
            } catch (const Throwable &thr) {
                src.setPosition(mark);
                thr.throws(__trace("core.charset.Unicode.Encoder"));
            }
        }

        void Unicode::Encoder::reset0() {
            needsMark = usesMark;
        }

        gbool Unicode::Encoder::canEncode(gchar c) {
            return !Character::isSurrogate(c);
        }
    } // charset
} // core