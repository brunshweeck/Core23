//
// Created by T.N.Brunshweeck on 15/01/2024.
//

#ifndef CORE23_WINDOWS1256_H
#define CORE23_WINDOWS1256_H

#include <core/charset/Charset.h>
#include <core/charset/CharsetEncoder.h>
#include <core/charset/CharsetDecoder.h>

namespace core {
    namespace charset {

        class Windows1256: public Charset {
        public:
            static Windows1256 INSTANCE;

            Windows1256();

            gbool contains(const Charset &cs) const override;

            CharsetDecoder &decoder() const override;

            CharsetEncoder &encoder() const override;

            Object &clone() const override;

        private:
            static CharArray b2c;
            static CharArray c2b;
            static CharArray c2bIndex;
            static CharArray c2bNR;
        };

    } // charset
} // core

#endif //CORE23_WINDOWS1256_H
