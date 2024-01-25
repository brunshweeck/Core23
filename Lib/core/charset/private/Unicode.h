//
// Created by T.N.Brunshweeck on 14/01/2024.
//

#ifndef CORE23_UNICODE_H
#define CORE23_UNICODE_H

#include <core/charset/Charset.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>
#include <core/charset/private/Surrogate.h>
#include <core/io/Buffer.h>

namespace core {
    namespace charset {

        using namespace io;

        class Unicode : public Charset {
        public:
            CORE_EXPLICIT Unicode(String canonicalName, StringArray aliases);

            gbool contains(const Charset &cs) const override;

        private:
            static gint countPositives(const ByteArray &ba, gint off, gint len);

        public:
            static void inflate(const ByteArray &src, gint srcOff, CharArray &dst, gint dstOff, gint len);

            static gint decodeASCII(const ByteArray &src, gint srcOff, CharArray &dst, gint dstOff, gint len);

            static gint encodeASCII(const CharArray &src, gint srcOff, ByteArray &dst, gint dstOff, gint len);

        protected:
            class Decoder : public CharsetDecoder {


            protected :
                static CORE_FAST gchar BYTE_ORDER_MARK = (gchar) 0xfeff;
                static CORE_FAST gchar REVERSED_MARK = (gchar) 0xfffe;

                static CORE_FAST gint NONE = 0;
                static CORE_FAST gint BIG = 1;
                static CORE_FAST gint LITTLE = 2;

            private :
                const gint expectedByteOrder;
                gint currentByteOrder;
                gint defaultByteOrder = BIG;

            public :
                CORE_EXPLICIT Decoder(const Charset &cs, gint bo);

                CORE_EXPLICIT Decoder(const Charset &cs, gint bo, gint defaultBO);

            private :
                gchar decode(gint b1, gint b2) const;

            protected :
                CoderResult decodeLoop(ByteBuffer &src, CharBuffer &dst) override;

                void reset0() override;
            };

            class Encoder : public CharsetEncoder {
            protected:
                static CORE_FAST gchar BYTE_ORDER_MARK = u'\uFEFF';
                static CORE_FAST gchar REVERSED_MARK = u'\uFFFE';

                static CORE_FAST gint BIG = 0;
                static CORE_FAST gint LITTLE = 1;

            private :
                gint byteOrder;      /* Byte order in use */
                gbool usesMark;   /* Write an initial BOM */
                gbool needsMark;

            protected :
                CORE_EXPLICIT Encoder(const Charset &cs, gint bo, gbool m);

            private :
                void put(gchar c, ByteBuffer &dst) const;

                Surrogate::Parser sgp;

            protected :
                CoderResult encodeLoop(CharBuffer &src, ByteBuffer &dst) override;

                void reset0() override;

            public :
                gbool canEncode(gchar c) override;
            };

        };

    } // charset
} // core

#endif //CORE23_UNICODE_H
