//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#ifndef CORE23_READONLYHEAPFLOATBUFFER_H
#define CORE23_READONLYHEAPFLOATBUFFER_H

#include <core/io/private/HeapFloatBuffer.h>

namespace core {
    namespace io {

        /**
         * A read-only HeapFloatBuffer.  This class extends the corresponding
         * read/write class, overriding the mutation methods to throw a <b style="color:orange;">
         * ReadOnlyBufferException</b> and overriding the view-buffer methods to return an
         * instance of this class rather than of the superclass.
         */
        class ReadOnlyHeapFloatBuffer CORE_FINAL /*private*/: public HeapFloatBuffer {
        public:
            ReadOnlyHeapFloatBuffer(gint cap, gint lim);

            ReadOnlyHeapFloatBuffer(FloatArray &buf, gint off, gint len);

            ReadOnlyHeapFloatBuffer(FloatArray &buf, gint mark, gint pos, gint lim, gint cap, gint off);

            FloatBuffer &slice() const override;

            FloatBuffer &slice(gint index, gint length) const override;

            FloatBuffer &duplicate() const override;

            FloatBuffer &asReadOnlyBuffer() const override;

            gbool isReadOnly() const override;

            FloatBuffer &put(gfloat x) override;

            FloatBuffer &put(gint i, gfloat x) override;

            FloatBuffer &put(const FloatArray &src, gint offset, gint length) override;

            FloatBuffer &put(FloatBuffer &src) override;

            FloatBuffer &put(gint index, FloatBuffer &src, gint offset, gint length) override;

            FloatBuffer &put(gint index, const FloatArray &src, gint offset, gint length) override;

            FloatBuffer &compact() override;

            ByteOrder order() const override;
        };

    } // io
} // core

#endif //CORE23_READONLYHEAPFLOATBUFFER_H
