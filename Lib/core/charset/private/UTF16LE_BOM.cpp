//
// Created by T.N.Brunshweeck on 15/01/2024.
//

#include "UTF16LE_BOM.h"

namespace core {
    namespace charset {

        using namespace native;

        UTF16LE_BOM UTF16LE_BOM::INSTANCE{};

        UTF16LE_BOM::UTF16LE_BOM() : Unicode("x-UTF-16LE-BOM"_S, StringArray::of("UnicodeLittle")) {}

        CharsetDecoder &UTF16LE_BOM::decoder() const {
            return Unsafe::allocateInstance<Decoder>(INSTANCE);
        }

        CharsetEncoder &UTF16LE_BOM::encoder() const {
            return Unsafe::allocateInstance<Encoder>(INSTANCE);
        }

        Object &UTF16LE_BOM::clone() const {
            return INSTANCE;
        }

        UTF16LE_BOM::Decoder::Decoder(const Charset &cs) : Unicode::Decoder(cs, NONE, LITTLE) {}

        Object &UTF16LE_BOM::Decoder::clone() const {
            return Unsafe::allocateInstance<Decoder>(INSTANCE);
        }

        UTF16LE_BOM::Encoder::Encoder(const Charset &cs) : Unicode::Encoder(cs, LITTLE, true) {}

        Object &UTF16LE_BOM::Encoder::clone() const {
            return Unsafe::allocateInstance<Encoder>(INSTANCE);
        }
    } // charset
} // core