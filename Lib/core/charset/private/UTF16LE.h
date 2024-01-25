//
// Created by T.N.Brunshweeck on 14/01/2024.
//

#ifndef CORE23_UTF16LE_H
#define CORE23_UTF16LE_H

#include <core/charset/private/Unicode.h>

namespace core {
    namespace charset {

        class UTF16LE : public Unicode {
        public:
            static UTF16LE INSTANCE;

            UTF16LE();

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

        }; // class UTF16LE

    } // charset
} // core

#endif //CORE23_UTF16LE_H
