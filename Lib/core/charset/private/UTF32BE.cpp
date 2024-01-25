//
// Created by T.N.Brunshweeck on 16/01/2024.
//

#include "UTF32BE.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        using namespace native;

        UTF32BE UTF32BE::INSTANCE{};

        UTF32BE::UTF32BE() : Unicode("UTF-32BE", StringArray::of("UTF_32BE", "X-UTF-32BE")) {}

        CharsetDecoder &UTF32BE::decoder() const {
            return Unsafe::allocateInstance<Decoder>();
        }

        CharsetEncoder &UTF32BE::encoder() const {
            return Unsafe::allocateInstance<Encoder>();
        }

        Object &UTF32BE::clone() const {
            return INSTANCE;
        }

        UTF32BE::Decoder::Decoder() : UTF32Coder::Decoder(INSTANCE, UTF32Coder::BIG) {}

        Object &UTF32BE::Decoder::clone() const {
            return INSTANCE.decoder();
        }

        UTF32BE::Encoder::Encoder() : UTF32Coder::Encoder(INSTANCE, UTF32Coder::BIG, false) {}

        Object &UTF32BE::Encoder::clone() const {
            return INSTANCE.encoder();
        }
    } // charset
} // core