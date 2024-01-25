//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#ifndef CORE23_HEAPINTBUFFER_H
#define CORE23_HEAPINTBUFFER_H

#include <core/io/IntBuffer.h>

namespace core {
    namespace io {

        class HeapIntBuffer /*private*/: public IntBuffer {
        private:
            static const glong ARRAY_BASE_OFFSET;
            static const glong ARRAY_INDEX_SCALE;

        public:
            HeapIntBuffer(gint cap, gint lim);

            HeapIntBuffer(IntArray &buf, gint off, gint len);

            HeapIntBuffer(IntArray &buf, gint mark, gint pos, gint lim, gint cap, gint off);

            IntBuffer &slice() const override;

            IntBuffer& slice(gint index, gint length) const override;

            IntBuffer &duplicate() const override;

            IntBuffer &asReadOnlyBuffer() const override;

        protected:
            gint ix(gint i) const;

        public:
            gint get() override;

            gint get(gint i) const override;

            IntBuffer &get(IntArray &dst, gint offset, gint length) override;

            IntBuffer &get(gint index, IntArray &dst, gint offset, gint length) const override;

            gbool isDirect() const override;

            gbool isReadOnly() const override;

            IntBuffer &put(gint x) override;

            IntBuffer &put(gint i, gint x) override;

            IntBuffer &put(IntArray &src, gint offset, gint length);

            IntBuffer &put(IntBuffer &src) override;

            IntBuffer &put(gint index, IntBuffer &src, gint offset, gint length) override;

            IntBuffer &put(gint index, const IntArray &src, gint offset, gint length) override;

            IntBuffer &compact() override;

            ByteOrder order() const override;
        };

    } // io
} // core

#endif //CORE23_HEAPINTBUFFER_H
