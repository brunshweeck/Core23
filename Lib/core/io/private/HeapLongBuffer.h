//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#ifndef CORE23_HEAPLONGBUFFER_H
#define CORE23_HEAPLONGBUFFER_H

#include <core/io/LongBuffer.h>

namespace core {
    namespace io {

        class HeapLongBuffer /*private*/: public LongBuffer {
        private:
            static const glong ARRAY_BASE_OFFSET;
            static const glong ARRAY_INDEX_SCALE;

        public:
            HeapLongBuffer(gint cap, gint lim);

            HeapLongBuffer(LongArray &buf, gint off, gint len);

            HeapLongBuffer(LongArray &buf, gint mark, gint pos, gint lim, gint cap, gint off);

            LongBuffer &slice() const override;

            LongBuffer& slice(gint index, gint length) const override;

            LongBuffer &duplicate() const override;

            LongBuffer &asReadOnlyBuffer() const override;

        protected:
            gint ix(gint i) const;

        public:
            glong get() override;

            glong get(gint i) const override;

            LongBuffer &get(LongArray &dst, gint offset, gint length) override;

            LongBuffer &get(gint index, LongArray &dst, gint offset, gint length) const override;

            gbool isDirect() const override;

            gbool isReadOnly() const override;

            LongBuffer &put(glong x) override;

            LongBuffer &put(gint i, glong x) override;

            LongBuffer &put(const LongArray &src, gint offset, gint length) override;

            LongBuffer &put(LongBuffer &src) override;

            LongBuffer &put(gint index, LongBuffer &src, gint offset, gint length) override;

            LongBuffer &put(gint index, const LongArray &src, gint offset, gint length) override;

            LongBuffer &compact() override;

            ByteOrder order() const override;
        };

    } // io
} // core

#endif //CORE23_HEAPLONGBUFFER_H
