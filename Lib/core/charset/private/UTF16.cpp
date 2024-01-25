//
// Created by T.N.Brunshweeck on 14/01/2024.
//

#include "UTF16.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        using namespace native;

        UTF16 UTF16::INSTANCE{};

        UTF16::UTF16() : Unicode("UTF-16"_S, StringArray::of("UTF_16", "utf16", "unicode", "UnicodeBig")) {}

        CharsetDecoder &UTF16::decoder() const {
            return Unsafe::allocateInstance<Decoder>(INSTANCE);
        }

        CharsetEncoder &UTF16::encoder() const {
            return Unsafe::allocateInstance<Encoder>(INSTANCE);
        }

        Object &UTF16::clone() const {
            return INSTANCE;
        }

        UTF16::Decoder::Decoder(const Charset &cs) : Unicode::Decoder(cs, NONE) {}

        Object &UTF16::Decoder::clone() const {
            return Unsafe::allocateInstance<Decoder>(INSTANCE);
        }

        UTF16::Encoder::Encoder(const Charset &cs) : Unicode::Encoder(cs, BIG, true) {}

        Object &UTF16::Encoder::clone() const {
            return Unsafe::allocateInstance<Encoder>(INSTANCE);
        }
    } // charset
} // core