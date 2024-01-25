//
// Created by T.N.Brunshweeck on 14/01/2024.
//

#ifndef CORE23_US_ASCII_H
#define CORE23_US_ASCII_H

#include <core/charset/Charset.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>
#include <core/charset/CoderResult.h>
#include <core/charset/private/Unicode.h>
#include <core/io/ByteBuffer.h>
#include <core/io/CharBuffer.h>

namespace core {
    namespace charset {

        using namespace io;

        class US_ASCII : public Charset {
        public :
            static US_ASCII INSTANCE;

            US_ASCII();

            gbool contains(const Charset &cs) const override;

            CharsetDecoder &decoder() const override;

            CharsetEncoder &encoder() const override;

            Object &clone() const override;

        private :
            class Decoder : public CharsetDecoder {
            public:
                CORE_EXPLICIT Decoder(const Charset &cs);

            private :

                static CoderResult decodeArrayLoop(ByteBuffer &src, CharBuffer &dst);

                static CoderResult decodeBufferLoop(ByteBuffer &src, CharBuffer &dst);

            protected :
                CoderResult decodeLoop(ByteBuffer &src, CharBuffer &dst) override;

            public:
                Object &clone() const override;
            };

            class Encoder : public CharsetEncoder {

            public:
                CORE_EXPLICIT Encoder(const Charset &cs);

                gbool canEncode(gchar c) override;

                gbool isLegalReplacement(const ByteArray &repl) const override;

            private :
                Surrogate::Parser sgp;

                CoderResult encodeArrayLoop(CharBuffer &src, ByteBuffer &dst);

                CoderResult encodeBufferLoop(CharBuffer &src, ByteBuffer &dst);

            protected :
                CoderResult encodeLoop(CharBuffer &src, ByteBuffer &dst) override;

            public:
                Object &clone() const override;

            }; // class Decoder
        }; // class US_ASCII

    } // charset
} // core

#endif //CORE23_US_ASCII_H
