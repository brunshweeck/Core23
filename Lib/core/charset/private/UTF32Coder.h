//
// Created by T.N.Brunshweeck on 16/01/2024.
//

#ifndef CORE23_UTF32CODER_H
#define CORE23_UTF32CODER_H

#include <core/charset/Charset.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>
#include <core/charset/CoderResult.h>
#include <core/io/ByteBuffer.h>
#include <core/io/CharBuffer.h>

namespace core {
    namespace charset {

        using namespace io;

        class UTF32Coder : public Object {
        public :
            static CORE_FAST gint BOM_BIG = 0x0000FEFF;
            static CORE_FAST gint BOM_LITTLE = (gint) 0xFFFE0000;
            static CORE_FAST gint NONE = 0;
            static CORE_FAST gint BIG = 1;
            static CORE_FAST gint LITTLE = 2;

            class Decoder : public CharsetDecoder {
            private :
                gint currentBO;
                gint expectedBO;

            protected :
                CORE_EXPLICIT Decoder(const Charset &cs, gint bo);

            private :
                gint getCP(ByteBuffer &src) const;

            protected :
                CoderResult decodeLoop(ByteBuffer &src, CharBuffer &dst) override;

                void reset0() override;

            public:
                Object &clone() const override;
            };

            class Encoder : public CharsetEncoder {
            private :
                gbool doBOM;
                gbool doneBOM;
                gint byteOrder;

            protected :
                void put(gint cp, ByteBuffer &dst) const;

                CORE_EXPLICIT Encoder(const Charset &cs, gint byteOrder, gbool doBOM) :
                        CharsetEncoder(cs, 4.0F, doBOM ? 8.0F : 4.0F,
                                       (byteOrder == BIG) ?
                                       ByteArray::of((gbyte) 0, (gbyte) 0, (gbyte) 0xff, (gbyte) 0xfd) :
                                       ByteArray::of((gbyte) 0xfd, (gbyte) 0xff, (gbyte) 0, (gbyte) 0)),
                                       byteOrder(byteOrder), doBOM(doBOM), doneBOM(!doBOM) {}

                CoderResult encodeLoop(CharBuffer &src, ByteBuffer &dst) override;

                void reset0() override;

            public:
                Object &clone() const override;

            };
        };

    } // gcharset
} // core

#endif //CORE23_UTF32CODER_H
