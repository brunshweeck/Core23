//
// Created by T.N.Brunshweeck on 14/01/2024.
//

#ifndef CORE23_UTF16_H
#define CORE23_UTF16_H

#include <core/charset/private/Unicode.h>

namespace core {
    namespace charset {

        class UTF16 : public Unicode {
        public:
            static UTF16 INSTANCE;

            UTF16();

            CharsetDecoder &decoder() const override;

            CharsetEncoder &encoder() const override;

            class Decoder : public Unicode::Decoder {
            public:
                CORE_EXPLICIT Decoder(const Charset& cs);

                Object &clone() const override;
            }; // class Decoder

            class Encoder : public Unicode::Encoder {
            public:
                CORE_EXPLICIT Encoder(const Charset& cs);

                Object &clone() const override;
            }; // class Encoder

            Object &clone() const override;

        }; // class UTF16

    } // charset
} // core

#endif //CORE23_UTF16_H
