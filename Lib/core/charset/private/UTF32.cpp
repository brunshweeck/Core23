//
// Created by T.N.Brunshweeck on 16/01/2024.
//

#include "UTF32.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        using namespace native;

        UTF32 UTF32::INSTANCE{};

        UTF32::UTF32() : Unicode("UTF-32", StringArray::of("UTF_32", "UTF32")) {}

        CharsetDecoder &UTF32::decoder() const {
            return Unsafe::allocateInstance<Decoder>();
        }

        CharsetEncoder &UTF32::encoder() const {
            return Unsafe::allocateInstance<Encoder>();
        }

        Object &UTF32::clone() const {
            return INSTANCE;
        }

        UTF32::Decoder::Decoder() : UTF32Coder::Decoder(INSTANCE, UTF32Coder::NONE) {}

        Object &UTF32::Decoder::clone() const {
            return INSTANCE.decoder();
        }

        UTF32::Encoder::Encoder() : UTF32Coder::Encoder(INSTANCE, UTF32Coder::BIG, true) {}

        Object &UTF32::Encoder::clone() const {
            return INSTANCE.encoder();
        }
    } // charset
} // core