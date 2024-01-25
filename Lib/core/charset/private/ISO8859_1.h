//
// Created by T.N.Brunshweeck on 17/01/2024.
//

#ifndef CORE23_ISO8859_1_H
#define CORE23_ISO8859_1_H

#include <core/charset/Charset.h>
#include <core/charset/CharsetEncoder.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/private/Unicode.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace charset {

        using namespace util;

        class ISO8859_1 : public Charset {
        public:
            static ISO8859_1 INSTANCE;

            ISO8859_1();

            gbool contains(const Charset &cs) const override;

            CharsetDecoder &decoder() const override;

            CharsetEncoder &encoder() const override;

            Object &clone() const override;

        private :
            class Decoder : public CharsetDecoder {
            public:
                Decoder();

            private:

                static CoderResult decodeArrayLoop(ByteBuffer &src, CharBuffer &dst);

                static CoderResult decodeBufferLoop(ByteBuffer &src, CharBuffer &dst);

            protected :
                CoderResult decodeLoop(ByteBuffer &src, CharBuffer &dst) override;

            public:
                Object &clone() const override;
            };

            class Encoder : public CharsetEncoder {

            public:
                CORE_EXPLICIT Encoder() : CharsetEncoder(INSTANCE, 1.0F, 1.0F) {}

                gbool canEncode(gchar c) override;

                gbool isLegalReplacement(const ByteArray &repl) const override;

                Object &clone() const override;

            private:
                Surrogate::Parser sgp;

                // Method possible replaced with a compiler intrinsic.
                static gint encodeISOArray(const CharArray &sa, gint sp, ByteArray &da, gint dp, gint len);

                static gint encodeISOArray0(const CharArray &sa, gint sp, ByteArray &da, gint dp, gint len);

                static void encodeISOArrayCheck(const CharArray &sa, gint sp, ByteArray &da, gint dp, gint len);

                CoderResult encodeArrayLoop(CharBuffer &src, ByteBuffer &dst);

                CoderResult encodeBufferLoop(CharBuffer &src, ByteBuffer &dst);

            protected :
                CoderResult encodeLoop(CharBuffer &src, ByteBuffer &dst) override;
            };
        };

    } // charset
} // core

#endif //CORE23_ISO8859_1_H
