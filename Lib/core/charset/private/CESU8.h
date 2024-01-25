//
// Created by T.N.Brunshweeck on 17/01/2024.
//

#ifndef CORE23_CESU8_H
#define CORE23_CESU8_H

#include <core/charset/private/Unicode.h>
#include <core/util/Optional.h>

namespace core {
    namespace charset {

        using namespace util;

        /* Legal CESU-8 Byte Sequences
         *
         * #    Code Points      Bits   Bit/Byte pattern
         * 1                     7      0xxxxxxx
         *      U+0000..U+007F          00..7F
         *
         * 2                     11     110xxxxx    10xxxxxx
         *      U+0080..U+07FF          C2..DF      80..BF
         *
         * 3                     16     1110xxxx    10xxxxxx    10xxxxxx
         *      U+0800..U+0FFF          E0          A0..BF      80..BF
         *      U+1000..U+FFFF          E1..EF      80..BF      80..BF
         *
         */
        class CESU8 : public Unicode {
        public:
            static CESU8 INSTANCE;

            CESU8();

            CharsetDecoder &decoder() const override;

            CharsetEncoder &encoder() const override;

            Object &clone() const override;


        private:
            static void updatePositions(Buffer &src, gint sp, Buffer &dst, gint dp);

            class Decoder : public CharsetDecoder {

            public:
                Decoder() : CharsetDecoder(INSTANCE, 1.0F, 1.0F) {}

                Object &clone() const override;

            private :
                static gbool isNotContinuation(gint b);

                //  [E0]     [A0..BF] [80..BF]
                //  [E1..EF] [80..BF] [80..BF]
                static gbool isMalformed3(gint b1, gint b2, gint b3);

                // only used when there is only one gbyte left in src buffer
                static gbool isMalformed3_2(gint b1, gint b2);

                static CoderResult malformedN(ByteBuffer &src, gint nb);

                static CoderResult malformed(ByteBuffer &src, gint sp, CharBuffer &dst, gint dp, gint nb);

                static CoderResult malformed(ByteBuffer &src, gint mark, gint nb);

                static CoderResult
                malformedForLength(ByteBuffer &src, gint sp, CharBuffer &dst, gint dp, gint malformedNB);

                static CoderResult malformedForLength(ByteBuffer &src, gint mark, gint malformedNB);

                static CoderResult xflow(Buffer &src, gint sp, gint sl, Buffer &dst, gint dp, gint nb);

                static CoderResult xflow(Buffer &src, gint mark, gint nb);

                static CoderResult decodeArrayLoop(ByteBuffer &src, CharBuffer &dst);

                static CoderResult decodeBufferLoop(ByteBuffer &src, CharBuffer &dst);

            protected :
                CoderResult decodeLoop(ByteBuffer &src, CharBuffer &dst) override;

            private :
                static ByteBuffer &getByteBuffer(Optional<ByteBuffer> &bb, ByteArray &ba, gint sp);

                // returns -1 if there is/are malformed gbyte(s) and the
                // "action" for malformed input is not REPLACE.
            public :
                gint decode(const ByteArray &sa, gint sp, gint len, CharArray &da);
            };

            class Encoder : public CharsetEncoder {

            public:
                Encoder();

                gbool canEncode(gchar c) override;

                gbool isLegalReplacement(const ByteArray &repl) const override;

                Object &clone() const override;

            private :
                static CoderResult overflow(CharBuffer &src, gint sp, ByteBuffer &dst, gint dp);

                static CoderResult overflow(CharBuffer &src, gint mark);

                static void to3Bytes(ByteArray &da, gint dp, gchar c);

                static void to3Bytes(ByteBuffer &dst, gchar c);

                Surrogate::Parser sgp;

                CoderResult encodeArrayLoop(CharBuffer &src, ByteBuffer &dst);

                CoderResult encodeBufferLoop(CharBuffer &src, ByteBuffer &dst);

            protected:
                CoderResult encodeLoop(CharBuffer &src, ByteBuffer &dst) override;

                // returns -1 if there is malformed gchar(s) and the
                // "action" for malformed input is not REPLACE.
            public :
                gint encode(CharArray const &sa, gint sp, gint len, ByteArray &da);
            };

        };

    } // charset
} // core

#endif //CORE23_CESU8_H
