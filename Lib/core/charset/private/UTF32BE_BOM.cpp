//
// Created by T.N.Brunshweeck on 16/01/2024.
//

#include "UTF32BE_BOM.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        using namespace native;

        UTF32BE_BOM UTF32BE_BOM::INSTANCE{};

        UTF32BE_BOM::UTF32BE_BOM() : Unicode("X-UTF-32BE-BOM", StringArray::of("UTF_32BE_BOM", "UTF-32BE-BOM")) {}

        CharsetDecoder &UTF32BE_BOM::decoder() const {
            return Unsafe::allocateInstance<Decoder>();
        }

        CharsetEncoder &UTF32BE_BOM::encoder() const {
            return Unsafe::allocateInstance<Encoder>();
        }

        Object &UTF32BE_BOM::clone() const {
            return INSTANCE;
        }

        UTF32BE_BOM::Decoder::Decoder() : UTF32Coder::Decoder(INSTANCE, UTF32Coder::BIG) {}

        Object &UTF32BE_BOM::Decoder::clone() const {
            return INSTANCE.decoder();
        }

        UTF32BE_BOM::Encoder::Encoder() : UTF32Coder::Encoder(INSTANCE, UTF32Coder::BIG, true) {}

        Object &UTF32BE_BOM::Encoder::clone() const {
            return INSTANCE.encoder();
        }
    } // charset
} // core