//
// Created by T.N.Brunshweeck on 15/01/2024.
//

#ifndef CORE23_IBM737_H
#define CORE23_IBM737_H

#include <core/charset/Charset.h>
#include <core/charset/CharsetEncoder.h>
#include <core/charset/CharsetDecoder.h>

namespace core {
    namespace charset {

        class IBM737: public Charset {
        public:
            static IBM737 INSTANCE;

            IBM737();

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

#endif //CORE23_IBM737_H
