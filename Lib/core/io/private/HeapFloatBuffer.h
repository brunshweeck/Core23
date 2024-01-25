//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#ifndef CORE23_HEAPFLOATBUFFER_H
#define CORE23_HEAPFLOATBUFFER_H

#include <core/io/FloatBuffer.h>

namespace core {
    namespace io {

        class HeapFloatBuffer /*private*/: public FloatBuffer {
        private:
            static const glong ARRAY_BASE_OFFSET;
            static const glong ARRAY_INDEX_SCALE;

        public:
            HeapFloatBuffer(gint cap, gint lim);

            HeapFloatBuffer(FloatArray &buf, gint off, gint len);

            HeapFloatBuffer(FloatArray &buf, gint mark, gint pos, gint lim, gint cap, gint off);

            FloatBuffer &slice() const override;

            FloatBuffer& slice(gint index, gint length) const override;

            FloatBuffer &duplicate() const override;

            FloatBuffer &asReadOnlyBuffer() const override;

        protected:
            gint ix(gint i) const;

        public:
            gfloat get() override;

            gfloat get(gint i) const override;

            FloatBuffer &get(FloatArray &dst, gint offset, gint length) override;

            FloatBuffer &get(gint index, FloatArray &dst, gint offset, gint length) const override;

            gbool isDirect() const override;

            gbool isReadOnly() const override;

            FloatBuffer &put(gfloat x) override;

            FloatBuffer &put(gint i, gfloat x) override;

            FloatBuffer &put(FloatArray &src, gint offset, gint length);

            FloatBuffer &put(FloatBuffer &src) override;

            FloatBuffer &put(gint index, FloatBuffer &src, gint offset, gint length) override;

            FloatBuffer &put(gint index, const FloatArray &src, gint offset, gint length) override;

            FloatBuffer &compact() override;

            ByteOrder order() const override;
        };

    } // io
} // core

#endif //CORE23_HEAPFLOATBUFFER_H
