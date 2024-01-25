//
// Created by T.N.Brunshweeck on 16/01/2024.
//

#include "UTF32LE.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        using namespace native;

        UTF32LE UTF32LE::INSTANCE{};

        UTF32LE::UTF32LE() : Unicode("UTF-32LE", StringArray::of("UTF_32LE", "X-UTF-32LE")) {}

        CharsetDecoder &UTF32LE::decoder() const {
            return Unsafe::allocateInstance<Decoder>();
        }

        CharsetEncoder &UTF32LE::encoder() const {
            return Unsafe::allocateInstance<Encoder>();
        }

        Object &UTF32LE::clone() const {
            return INSTANCE;
        }

        UTF32LE::Decoder::Decoder() : UTF32Coder::Decoder(INSTANCE, UTF32Coder::LITTLE) {}

        Object &UTF32LE::Decoder::clone() const {
            return INSTANCE.decoder();
        }

        UTF32LE::Encoder::Encoder() : UTF32Coder::Encoder(INSTANCE, UTF32Coder::LITTLE, false) {}

        Object &UTF32LE::Encoder::clone() const {
            return INSTANCE.encoder();
        }
    } // charset
} // core