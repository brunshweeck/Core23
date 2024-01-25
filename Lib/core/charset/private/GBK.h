//
// Created by T.N.Brunshweeck on 18/01/2024.
//

#ifndef CORE23_GBK_H
#define CORE23_GBK_H

#include <core/charset/Charset.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>

namespace core {
    namespace charset {

        class GBK : public Charset {
        public:
            static GBK INSTANCE;

            GBK();

            gbool contains(const Charset &cs) const override;

            CharsetDecoder &decoder() const override;

            CharsetEncoder &encoder() const override;

            Object &clone() const override;

        private:

            static String b2cSBStr;
            static StringArray b2cStr;
            // Decoder requirement
            static CharArray2D b2c;
            static CharArray b2cSB;
            // Encoder requirement
            static CharArray c2b;
            static CharArray c2bIndex;
        };

    } // charset
} // core

#endif //CORE23_GBK_H
