//
// Created by T.N.Brunshweeck on 15/01/2024.
//

#ifndef CORE23_SINGLEBYTE_H
#define CORE23_SINGLEBYTE_H

#include <core/charset/Charset.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>
#include <core/charset/CoderResult.h>
#include <core/charset/private/Unicode.h>
#include <core/io/ByteBuffer.h>
#include <core/io/CharBuffer.h>
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        using namespace native;

        class SingleByte : public Object {
        public:

            static CORE_FAST gchar UNMAPPABLE_DECODING = (gchar) 0xFFFD;
            static CORE_FAST gint UNMAPPABLE_ENCODING = 0xFFFD;

        private:
            SingleByte() = default;

            static CoderResult withResult(CoderResult cr, Buffer &src, gint sp, Buffer &dst, gint dp);

        public :
            class Decoder : public CharsetDecoder {

            private :
                const CharArray &b2c;
                const gbool isASCIICompatible;
                const gbool isLatin1Decodable;

            public:
                CORE_EXPLICIT Decoder(const Charset &cs, const CharArray &b2c);

                CORE_EXPLICIT Decoder(const Charset &cs, const CharArray &b2c, gbool isASCIICompatible);

                CORE_EXPLICIT Decoder(const Charset &cs, const CharArray &b2c, gbool isASCIICompatible,
                                      gbool isLatin1Decodable);

            private :
                CoderResult decodeArrayLoop(ByteBuffer &src, CharBuffer &dst) const;

                CoderResult decodeBufferLoop(ByteBuffer &src, CharBuffer &dst) const;

            protected :
                CoderResult decodeLoop(ByteBuffer &src, CharBuffer &dst) override;

            public:
                gchar decode(gint b) const;

            private :
                gchar repl = (gchar) 0xFFFD;
            protected :
                void replaceWith0(const String &newReplacement) override;

            public :
                gint decodeToLatin1(const ByteArray &src, gint sp, gint len, ByteArray &dst) const;

                gint decode(const ByteArray &src, gint sp, gint len, CharArray &dst) const;
            };

            class Encoder : public CharsetEncoder {
            private :
                Surrogate::Parser sgp;
                const CharArray &c2b;
                const CharArray &c2bIndex;
                const gbool isASCIICompatible;

            public :
                CORE_EXPLICIT Encoder(const Charset &cs, const CharArray &c2b, const CharArray &c2bIndex,
                                      gbool isASCIICompatible);

                gbool canEncode(gchar c) override;

                gbool isLegalReplacement(const ByteArray &repl) const override;

            private:
                CoderResult encodeArrayLoop(CharBuffer &src, ByteBuffer &dst);

                CoderResult encodeBufferLoop(CharBuffer &src, ByteBuffer &dst);

            protected :
                CoderResult encodeLoop(CharBuffer &src, ByteBuffer &dst) override;

            public:
                gint encode(gchar ch);

            private:
                gbyte repl = (gbyte) '?';
            protected :
                void replaceWith0(const ByteArray &newReplacement) override;

            public :
                gint encode(const CharArray &src, gint sp, gint len, ByteArray &dst);

                gint encodeFromLatin1(ByteArray src, gint sp, gint len, ByteArray dst);

                static CORE_FAST gint HI_BYTE_SHIFT = Unsafe::BIG_ENDIAN ? 8 : 0;
                static CORE_FAST gint LO_BYTE_SHIFT = Unsafe::BIG_ENDIAN ? 0 : 8;


                // intrinsic performs no bounds checks
                static gchar getChar(const ByteArray &val, int index);

                static int length(const ByteArray &value);

                gint encodeFromUTF16(const ByteArray& src, gint sp, gint len, ByteArray &dst);
            };

            // init the c2b and c2bIndex tables from b2c.
            static void initC2B(CharArray &b2c, CharArray &c2bNR, CharArray &c2b, CharArray &c2bIndex);
        };

    } // charset
} // core

#endif //CORE23_SINGLEBYTE_H
