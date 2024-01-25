//
// Created by T.N.Brunshweeck on 16/01/2024.
//

#ifndef CORE23_UTF32_H
#define CORE23_UTF32_H

#include <core/charset/private/UTF32Coder.h>
#include <core/charset/private/Unicode.h>

namespace core {
    namespace charset {

        class UTF32 : public Unicode {
        public:
            static UTF32 INSTANCE;

            UTF32();

            CharsetDecoder &decoder() const override;

            CharsetEncoder &encoder() const override;

            class Decoder : public UTF32Coder::Decoder {
            public:
                Decoder();

                Object &clone() const override;
            }; // class Decoder

            class Encoder : public UTF32Coder::Encoder {
            public:
                Encoder();

                Object &clone() const override;
            }; // class Encoder

            Object &clone() const override;
        }; // class UTF32

    } // charset
} // core

#endif //CORE23_UTF32_H
