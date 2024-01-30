//
// Created by T.N.Brunshweeck on 16/01/2024.
//

#ifndef CORE23_DOUBLEBYTE_H
#define CORE23_DOUBLEBYTE_H

#include <core/charset/Charset.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>
#include <core/charset/CoderResult.h>
#include <core/charset/private/Surrogate.h>
#include <core/charset/private/Unicode.h>
#include <core/charset/private/SingleByte.h>
#include <core/io/ByteBuffer.h>
#include <core/io/CharBuffer.h>
#include <core/native/CharArray2D.h>

namespace core {
    namespace charset {

        using namespace util;

        /*
         * Four types of "DoubleByte" charsets are implemented in this class
         * (1)DoubleByte
         *    The "mostly widely used" multibyte charset, a combination of
         *    a singlebyte character set (usually the ASCII charset) and a
         *    doublebyte character set. The codepoint values of singlebyte
         *    and doublebyte don't overlap. Microsoft's multibyte charsets
         *    and IBM's "DBCS_ASCII" charsets, such as IBM1381, 942, 943,
         *    948, 949 and 950 are such charsets.
         *
         * (2)DoubleByte_EBCDIC
         *    IBM EBCDIC Mix multibyte charset. Use SO and SI to shift (switch)
         *    in and out between the singlebyte character set and doublebyte
         *    character set.
         *
         * (3)DoubleByte_SIMPLE_EUC
         *    It's a "simple" form of EUC encoding scheme, only have the
         *    singlebyte character set G0 and one doublebyte character set
         *    G1 are defined, G2 (with SS2) and G3 (with SS3) are not used.
         *    So it is actually the same as the "typical" type (1) mentioned
         *    above, except it return "malformed" for the SS2 and SS3 when
         *    decoding.
         *
         * (4)DoubleByte ONLY
         *    A "pure" doublebyte only character setValue. From implementation
         *    point of view, this is the type (1) with "decodeSingle" always
         *    returns unmappable.
         *
         * For simplicity, all implementations share the same decoding and
         * encoding data structure.
         *
         * Decoding:
         *
         *    CharArray2D b2c;
         *    CharArray b2cSB;
         *    gint b2Min, b2Max
         *
         *
         *     gchar decodeSingle(gint b) {
         *         return b2cSB.[b];
         *     }
         *
         *      gchar decodeDouble(gint b1, gint b2) {
         *        if (b2 < b2Min || b2 > b2Max)
         *            return UNMAPPABLE_DECODING;
         *         return b2c[b1][b2 - b2Min];
         *      }
         *
         *    (1)b2Min, b2Max are the corresponding min and max value of the
         *       low-half of the double-gbyte.
         *    (2)The high 8-bit/b1 of the double-gbyte are used to indexed into
         *       b2c array.
         *
         * Encoding:
         *
         *    CharArray c2b;
         *    CharArray c2bIndex;
         *
         *    gint encodeChar(gchar ch) {
         *        return c2b[c2bIndex[ch >> 8] + (ch & 0xff)];
         *    }
         *
         */
        class DoubleByte : public Object {
        public:
            static CORE_FAST gchar UNMAPPABLE_DECODING = (gchar) 0xFFFD;
            static CORE_FAST gint UNMAPPABLE_ENCODING = 0xFFFD;

            static CharArray B2C_UNMAPPABLE;

            class Decoder : public CharsetDecoder {
            protected:
                CharArray2D &b2c;
                CharArray &b2cSB;
                gint b2Min;
                gint b2Max;
                gbool isASCIICompatible;

                // for SimpleEUC override
                static CoderResult crMalformedOrUnderFlow(gint b);

                virtual CoderResult crMalformedOrUnmappable(gint b1, gint b2);

            public :
                CORE_EXPLICIT Decoder(const Charset &cs, gfloat avgcpb, gfloat maxcpb, CharArray2D &b2c,
                                      CharArray &b2cSB, gint b2Min, gint b2Max, gbool isASCIICompatible);

                CORE_EXPLICIT Decoder(const Charset &cs, CharArray2D &b2c, CharArray &b2cSB, gint b2Min,
                                      gint b2Max, gbool isASCIICompatible);

                CORE_EXPLICIT Decoder(const Charset &cs, CharArray2D &b2c, CharArray &b2cSB, gint b2Min,
                                      gint b2Max);

                Object &clone() const override;

            protected :
                virtual CoderResult decodeArrayLoop(ByteBuffer &src, CharBuffer &dst);

                virtual CoderResult decodeBufferLoop(ByteBuffer &src, CharBuffer &dst);

                // Make some protected methods public for use by JISAutoDetect
            public :
                CoderResult decodeLoop(ByteBuffer &src, CharBuffer &dst) override;

                virtual gint decode(const ByteArray &src, gint sp, gint len, CharArray &dst);

                void reset0() override;

                CoderResult flush0(CharBuffer &out) override;

                // decode loops are not using decodeSingle/Double() for performance
                // reason.
                gchar decodeSingle(gint b);

                gchar decodeDouble(gint b1, gint b2);
            };

            // IBM_EBCDIC_DBCS
            class Decoder_EBCDIC : public Decoder {
            private :
                static CORE_FAST gint SBCS = 0;
                static CORE_FAST gint DBCS = 1;
                static CORE_FAST gint SO = 0x0e;
                static CORE_FAST gint SI = 0x0f;
                gint currentState = {};

            public :
                CORE_EXPLICIT Decoder_EBCDIC(const Charset &cs, CharArray2D &b2c, CharArray &b2cSB, gint b2Min,
                                             gint b2Max, gbool isASCIICompatible);

                CORE_EXPLICIT Decoder_EBCDIC(const Charset &cs, CharArray2D &b2c, CharArray &b2cSB, gint b2Min,
                                             gint b2Max);

                void reset0() override;

                // Check validity of dbcs ebcdic gbyte pair values
                //
                // First gbyte : 0x41 -- 0xFE
                // Second gbyte: 0x41 -- 0xFE
                // Doublebyte blank: 0x4040
                //
                // The validation implementation in "old" DBCS_IBM_EBCDIC and sun.io
                // as
                //            if ((b1 != 0x40 || b2 != 0x40) &&
                //                (b2 < 0x41 || b2 > 0xfe)) {...}
                // is not correct/complete (range check for b1)
                //
            private :
                static gbool isDoubleByte(gint b1, gint b2);

            protected :
                CoderResult decodeArrayLoop(ByteBuffer &src, CharBuffer &dst) override;

                CoderResult decodeBufferLoop(ByteBuffer &src, CharBuffer &dst) override;


            public :
                gint decode(const ByteArray &src, gint sp, gint len, CharArray &dst) override;
            };

            // DBCS_ONLY
            class Decoder_DBCSONLY : public Decoder {
            public:
                static CharArray b2cSB_UNMAPPABLE;

                // always returns unmappableForLenth(2) for doublebyte_only

            protected :
                CoderResult crMalformedOrUnmappable(gint b1, gint b2) override;

            public :
                CORE_EXPLICIT Decoder_DBCSONLY(const Charset &cs, CharArray2D &b2c, CharArray &b2cSB, gint b2Min,
                                               gint b2Max, gbool isASCIICompatible);

                CORE_EXPLICIT Decoder_DBCSONLY(const Charset &cs, CharArray2D &b2c, CharArray &b2cSB, gint b2Min,
                                               gint b2Max);
            };

            // EUC_SIMPLE
            // The only thing we need to "override" is to check SS2/SS3 and
            // return "malformed" if found
            class Decoder_EUC_SIM : public Decoder {
            private :
                gint SS2 = 0x8E;
                gint SS3 = 0x8F;

            public :
                CORE_EXPLICIT Decoder_EUC_SIM(const Charset &cs, CharArray2D &b2c, CharArray &b2cSB, gint b2Min,
                                              gint b2Max, gbool isASCIICompatible);

                // No support provided for G2/G3 for SimpleEUC
            protected :
                CoderResult crMalformedOrUnderFlow(gint b) const;

                CoderResult crMalformedOrUnmappable(gint b1, gint b2) override;

            public :
                gint decode(const ByteArray &src, gint sp, gint len, CharArray &dst) override;
            };

            class Encoder : public CharsetEncoder {
            protected:
                gint MAX_SINGLEBYTE = 0xff;
            private :
                CharArray &c2b;
                CharArray &c2bIndex;
            protected :
                Surrogate::Parser sgp;
                gbool isASCIICompatible;

            public :
                CORE_EXPLICIT Encoder(const Charset &cs, CharArray &c2b, CharArray &c2bIndex);

                CORE_EXPLICIT Encoder(const Charset &cs, CharArray &c2b, CharArray &c2bIndex, gbool isASCIICompatible);

                CORE_EXPLICIT Encoder(const Charset &cs, gfloat avg, gfloat max, const ByteArray &repl, CharArray &c2b,
                                      CharArray &c2bIndex, gbool isASCIICompatible);

                gbool canEncode(gchar c) override;

                Object &clone() const override;

            protected :
                virtual CoderResult encodeArrayLoop(CharBuffer &src, ByteBuffer &dst);

                virtual CoderResult encodeBufferLoop(CharBuffer &src, ByteBuffer &dst);

                CoderResult encodeLoop(CharBuffer &src, ByteBuffer &dst) override;

                ByteArray repl = replacement();

                void replaceWith0(const ByteArray &newReplacement) override;


            public :
                virtual gint encode(const CharArray &src, gint sp, gint len, ByteArray &dst);


                virtual gint encodeFromLatin1(const ByteArray &src, gint sp, gint len, ByteArray &dst);


                virtual gint encodeFromUTF16(const ByteArray &src, gint sp, gint len, ByteArray &dst);

                virtual gint encodeChar(gchar ch);

                // init the c2b and c2bIndex tables from b2c.
                static void
                initC2B(const StringArray &b2c, const String &b2cSB, const String &b2cNR, const String &c2bNR,
                        gint b2Min, gint b2Max, CharArray c2b, CharArray c2bIndex);
            };

            class Encoder_DBCSONLY : public Encoder {

            public :
                CORE_EXPLICIT Encoder_DBCSONLY(const Charset &cs, const ByteArray &repl, CharArray &c2b,
                                               CharArray &c2bIndex, gbool isASCIICompatible);

                gint encodeChar(gchar ch) override;
            };

            class Encoder_EBCDIC : public Encoder {
            public:
                static CORE_FAST gint SBCS = 0;
                static CORE_FAST gint DBCS = 1;
                static CORE_FAST gbyte SO = 0x0e;
                static CORE_FAST gbyte SI = 0x0f;

            protected :
                gint currentState = SBCS;

            public :
                CORE_EXPLICIT Encoder_EBCDIC(const Charset &cs, CharArray &c2b, CharArray &c2bIndex,
                                             gbool isASCIICompatible);

            protected :
                void reset0() override;

                CoderResult flush0(ByteBuffer &out) override;

                CoderResult encodeArrayLoop(CharBuffer &src, ByteBuffer &dst) override;

                CoderResult encodeBufferLoop(CharBuffer &src, ByteBuffer &dst) override;


            public :
                gint encode(const CharArray &src, gint sp, gint len, ByteArray &dst) override;


                gint encodeFromLatin1(const ByteArray &src, gint sp, gint len, ByteArray &dst) override;


                gint encodeFromUTF16(const ByteArray &src, gint sp, gint len, ByteArray &dst) override;
            };

            // EUC_SIMPLE
            class Encoder_EUC_SIM : public Encoder {
            public:
                CORE_EXPLICIT Encoder_EUC_SIM(const Charset &cs, CharArray &c2b, CharArray &c2bIndex,
                                              gbool isASCIICompatible) :
                        Encoder(cs, c2b, c2bIndex, isASCIICompatible) {}
            };
        };

    } // charset
} // core

#endif //CORE23_DOUBLEBYTE_H
