//
// Created by T.N.Brunshweeck on 14/01/2024.
//

#include "UTF16BE.h"

namespace core {
    namespace charset {

        using namespace native;

        UTF16BE UTF16BE::INSTANCE{};

        UTF16BE::UTF16BE() :
                Unicode("UTF-16BE"_S,
                        StringArray::of("UTF_16BE", "ISO-10646-UCS-2", "X-UTF-16BE", "UnicodeBigUnmarked")) {}

        CharsetDecoder &UTF16BE::decoder() const {
            return Unsafe::allocateInstance<Decoder>(INSTANCE);
        }

        CharsetEncoder &UTF16BE::encoder() const {
            return Unsafe::allocateInstance<Encoder>(INSTANCE);
        }

        Object &UTF16BE::clone() const {
            return INSTANCE;
        }

        UTF16BE::Decoder::Decoder(const Charset &cs) : Unicode::Decoder(cs, BIG) {}

        Object &UTF16BE::Decoder::clone() const {
            return Unsafe::allocateInstance<Decoder>(INSTANCE);
        }

        UTF16BE::Encoder::Encoder(const Charset &cs) : Unicode::Encoder(cs, BIG, false) {}

        Object &UTF16BE::Encoder::clone() const {
            return Unsafe::allocateInstance<Encoder>(INSTANCE);
        }
    } // charset
} // core