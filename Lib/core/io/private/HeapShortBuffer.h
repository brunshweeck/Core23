//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#ifndef CORE23_HEAPSHORTBUFFER_H
#define CORE23_HEAPSHORTBUFFER_H

#include <core/io/ShortBuffer.h>

namespace core {
    namespace io {

        class HeapShortBuffer /*private*/: public ShortBuffer {
        private:
            static const glong ARRAY_BASE_OFFSET;
            static const glong ARRAY_INDEX_SCALE;

        public:
            HeapShortBuffer(gint cap, gint lim);

            HeapShortBuffer(ShortArray &buf, gint off, gint len);

            HeapShortBuffer(ShortArray &buf, gint mark, gint pos, gint lim, gint cap, gint off);

            ShortBuffer &slice() const override;

            ShortBuffer& slice(gint index, gint length) const override;

            ShortBuffer &duplicate() const override;

            ShortBuffer &asReadOnlyBuffer() const override;

        protected:
            gint ix(gint i) const;

        public:
            gshort get() override;

            gshort get(gint i) const override;

            ShortBuffer &get(ShortArray &dst, gint offset, gint length) override;

            ShortBuffer &get(gint index, ShortArray &dst, gint offset, gint length) const override;

            gbool isDirect() const override;

            gbool isReadOnly() const override;

            ShortBuffer &put(gshort x) override;

            ShortBuffer &put(gint i, gshort x) override;

            ShortBuffer &put(const ShortArray &src, gint offset, gint length) override;

            ShortBuffer &put(ShortBuffer &src) override;

            ShortBuffer &put(gint index, ShortBuffer &src, gint offset, gint length) override;

            ShortBuffer &put(gint index, const ShortArray &src, gint offset, gint length) override;

            ShortBuffer &compact() override;

            ByteOrder order() const override;
        };

    } // io
} // core

#endif //CORE23_HEAPSHORTBUFFER_H
