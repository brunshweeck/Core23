//
// Created by T.N.Brunshweeck on 14/01/2024.
//

#ifndef CORE23_UTF8_H
#define CORE23_UTF8_H

#include <core/charset/private/Unicode.h>
#include <core/charset/private/Surrogate.h>
#include <core/io/Buffer.h>

namespace core {
    namespace charset {

        class UTF8 : public Unicode {
        public:
            static UTF8 INSTANCE;

            UTF8();

            CharsetDecoder &decoder() const override;

            CharsetEncoder &encoder() const override;

            static void updatePositions(Buffer &src, gint sp, Buffer &dst, gint dp);

            class Decoder : public CharsetDecoder {
            public:
                CORE_EXPLICIT Decoder(const Charset &cs);


            private :
                static gbool isNotContinuation(gint b);

                //  [E0]     [A0..BF] [80..BF]
                //  [E1..EF] [80..BF] [80..BF]
                static gbool isMalformed3(gint b1, gint b2, gint b3);

                // only used when there is only one gbyte left in src buffer
                static gbool isMalformed3_2(gint b1, gint b2);

                //  [F0]     [90..BF] [80..BF] [80..BF]
                //  [F1..F3] [80..BF] [80..BF] [80..BF]
                //  [F4]     [80..8F] [80..BF] [80..BF]
                //  only check 80-be range here, the [0xf0,0x80...] and [0xf4,0x90-...]
                //  will be checked by Character::isSupplementaryCodePoint(uc)
                static gbool isMalformed4(gint b2, gint b3, gint b4);

                // only used when there are less than 4 bytes left in src buffer.
                // both b1 and b2 should be "& 0xff" before passed in.
                static gbool isMalformed4_2(gint b1, gint b2);

                // tests if b1 and b2 are malformed as the first 2 bytes of a
                // legal`4-gbyte utf-8 gbyte sequence.
                // only used when there are less than 4 bytes left in src buffer,
                // after isMalformed4_2 has been invoked.
                static gbool isMalformed4_3(gint b3);

                static CoderResult malformedN(ByteBuffer &src, gint nb);

                static CoderResult malformed(ByteBuffer &src, gint sp, CharBuffer &dst, gint dp, gint nb);


                static CoderResult malformed(ByteBuffer &src, gint mark, gint nb);

                static CoderResult malformedForLength(ByteBuffer &src, gint sp,
                                                      CharBuffer &dst, gint dp, gint malformedNB);

                static CoderResult malformedForLength(ByteBuffer &src, gint mark, gint malformedNB);


                static CoderResult xflow(Buffer &src, gint sp, gint sl, Buffer &dst, gint dp, gint nb);

                static CoderResult xflow(Buffer &src, gint mark, gint nb);

                static CoderResult decodeArrayLoop(ByteBuffer &src, CharBuffer &dst);

                static CoderResult decodeBufferLoop(ByteBuffer &src, CharBuffer &dst);

            protected:
                CoderResult decodeLoop(ByteBuffer &src, CharBuffer &dst) override;

            };


            class Encoder : public CharsetEncoder {

            public:
                CORE_EXPLICIT Encoder(const Charset &cs);

                gbool canEncode(gchar c) override;

                gbool isLegalReplacement(const ByteArray &repl) const  override;

            private :
                static CoderResult overflow(CharBuffer &src, gint sp, ByteBuffer &dst, gint dp);

                static CoderResult overflow(CharBuffer &src, gint mark);

                Surrogate::Parser sgp;

                CoderResult encodeArrayLoop(CharBuffer &src, ByteBuffer &dst);

                CoderResult encodeArrayLoopSlow(CharBuffer &src, const CharArray &sa, gint sp, gint sl,
                                                ByteBuffer &dst, ByteArray &da, gint dp, gint dl);

                CoderResult encodeBufferLoop(CharBuffer &src, ByteBuffer &dst);

            protected:
                CoderResult encodeLoop(CharBuffer &src, ByteBuffer &dst) override {
                    if (src.hasArray() && dst.hasArray())
                        return encodeArrayLoop(src, dst);
                    else
                        return encodeBufferLoop(src, dst);
                }

            }; // class Encoder


            Object &clone() const override;
        }; //class UTF8

    } // charset
} // core

#endif //CORE23_UTF8_H
