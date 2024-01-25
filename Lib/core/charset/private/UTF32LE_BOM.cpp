//
// Created by T.N.Brunshweeck on 16/01/2024.
//

#include "UTF32LE_BOM.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace charset {

        using namespace native;

        UTF32LE_BOM UTF32LE_BOM::INSTANCE{};

        UTF32LE_BOM::UTF32LE_BOM() : Unicode("X-UTF-32LE-BOM", StringArray::of("UTF_32LE_BOM", "UTF-32LE-BOM")) {}

        CharsetDecoder &UTF32LE_BOM::decoder() const {
            return Unsafe::allocateInstance<Decoder>();
        }

        CharsetEncoder &UTF32LE_BOM::encoder() const {
            return Unsafe::allocateInstance<Encoder>();
        }

        Object &UTF32LE_BOM::clone() const {
            return INSTANCE;
        }

        UTF32LE_BOM::Decoder::Decoder() : UTF32Coder::Decoder(INSTANCE, UTF32Coder::LITTLE) {}

        Object &UTF32LE_BOM::Decoder::clone() const {
            return INSTANCE.decoder();
        }

        UTF32LE_BOM::Encoder::Encoder() : UTF32Coder::Encoder(INSTANCE, UTF32Coder::LITTLE, true) {}

        Object &UTF32LE_BOM::Encoder::clone() const {
            return INSTANCE.encoder();
        }
    } // charset
} // core