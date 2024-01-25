//
// Created by T.N.Brunshweeck on 17/01/2024.
//

#ifndef CORE23_GB18030_H
#define CORE23_GB18030_H

#include <core/charset/Charset.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>
#include <core/charset/private/Surrogate.h>

namespace core {
    namespace charset {

        class GB18030: public Charset {
        private:
            static CORE_FAST gint GB18030_SINGLE_BYTE = 1;
            static CORE_FAST gint GB18030_DOUBLE_BYTE = 2;
            static CORE_FAST gint GB18030_FOUR_BYTE = 3;

        public:
            static GB18030 INSTANCE;

            GB18030();

            gbool contains(const Charset &cs) const override;

            CharsetDecoder &decoder() const override;

            CharsetEncoder &encoder() const override;

            Object &clone() const override;

        private:
            static ShortArray decoderIndex1;
            static StringArray decoderIndex2;
            static ShortArray index1;
            static StringArray index2;
            static ShortArray encoderIndex1;
            static StringArray encoderIndex2;

            class Decoder : public CharsetDecoder {

            private :
                static CORE_FAST gchar REPLACE_CHAR = u'\uFFFD';

                gint currentState = GB18030_DOUBLE_BYTE;

            public:
                Decoder();

            private:
                static gchar getChar(gint offset);

            public:
                Object &clone() const override;

            protected :
                static gchar decodeDouble(gint byte1, gint byte2);

                void reset0() override;

            private :
                CoderResult decodeArrayLoop(ByteBuffer &src, CharBuffer &dst);

                CoderResult decodeBufferLoop(ByteBuffer &src, CharBuffer &dst);


            protected :
                CoderResult decodeLoop(ByteBuffer &src, CharBuffer &dst) override;
            };

            class Encoder : public CharsetEncoder {
            private :
                gint currentState = GB18030_DOUBLE_BYTE;

            public:
                CORE_EXPLICIT Encoder();

                gbool canEncode(gchar c) override;

                Object &clone() const override;

            private :
                Surrogate::Parser sgp;

                static gint getGB18030(const ShortArray &outerIndex, const StringArray &innerEncoderIndex, gchar ch);

            protected:
                void reset0() override;

            private :
                CoderResult encodeArrayLoop(CharBuffer &src, ByteBuffer &dst);

                CoderResult encodeBufferLoop(CharBuffer &src, ByteBuffer &dst);
            protected:
                CoderResult encodeLoop(CharBuffer &src, ByteBuffer &dst) override;
            };

        };

    } // charset
} // core

#endif //CORE23_GB18030_H
