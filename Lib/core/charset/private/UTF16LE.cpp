//
// Created by T.N.Brunshweeck on 14/01/2024.
//

#include "UTF16LE.h"

namespace core {
    namespace charset {

        using namespace native;

        UTF16LE UTF16LE::INSTANCE{};

        UTF16LE::UTF16LE() :
                Unicode("UTF-16LE"_S, StringArray::of("UTF_16LE", "X-UTF-16LE", "UnicodeLittleUnmarked")) {}

        CharsetDecoder &UTF16LE::decoder() const {
            return Unsafe::allocateInstance<Decoder>(INSTANCE);
        }

        CharsetEncoder &UTF16LE::encoder() const {
            return Unsafe::allocateInstance<Encoder>(INSTANCE);
        }

        Object &UTF16LE::clone() const {
            return INSTANCE;
        }

        UTF16LE::Decoder::Decoder(const Charset &cs) : Unicode::Decoder(cs, LITTLE) {}

        Object &UTF16LE::Decoder::clone() const {
            return Unsafe::allocateInstance<Decoder>(INSTANCE);
        }

        UTF16LE::Encoder::Encoder(const Charset &cs) : Unicode::Encoder(cs, LITTLE, false) {}

        Object &UTF16LE::Encoder::clone() const {
            return Unsafe::allocateInstance<Encoder>(INSTANCE);
        }
    } // charset
} // core