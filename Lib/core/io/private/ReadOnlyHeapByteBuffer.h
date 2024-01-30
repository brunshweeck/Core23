//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#ifndef CORE23_READONLYHEAPBYTEBUFFER_H
#define CORE23_READONLYHEAPBYTEBUFFER_H

#include <core/io/private/HeapByteBuffer.h>

namespace core {
    namespace io {

        /**
         * A read-only HeapByteBuffer.  This class extends the corresponding
         * read/write class, overriding the mutation methods to throw a <b style="color:orange;">
         * ReadOnlyBufferException</b> and overriding the view-buffer methods to return an
         * instance of this class rather than of the superclass.
         */
        class ReadOnlyHeapByteBuffer CORE_FINAL /*private*/ : public HeapByteBuffer {
        public:
            ReadOnlyHeapByteBuffer(gint cap, gint lim);

            ReadOnlyHeapByteBuffer(ByteArray &buf, gint off, gint len);

            ReadOnlyHeapByteBuffer(ByteArray &buf, gint mark, gint pos, gint lim, gint cap, gint off);

            ByteBuffer &slice() const override;

            ByteBuffer &slice(gint index, gint length) const override;

            ByteBuffer &duplicate() const override;

            ByteBuffer &asReadOnlyBuffer() const override;

            gbool isReadOnly() const override;

            ByteBuffer &put(gbyte x) override;

            ByteBuffer &put(gint i, gbyte x) override;

            ByteBuffer &put(const ByteArray &src, gint offset, gint length) override;

            ByteBuffer &put(ByteBuffer &src) override;

            ByteBuffer &put(gint index, ByteBuffer &src, gint offset, gint length) override;

            ByteBuffer &put(gint index, const ByteArray &src, gint offset, gint length) override;

            ByteBuffer &compact() override;

        private:
            gbyte _get(gint i) const override;

            void _put(gint i, gbyte b) override;

        public:

            ByteBuffer &putChar(gchar x) override;

            ByteBuffer &putChar(gint i, gchar x) override;

            CharBuffer &asCharBuffer() const override;

            ByteBuffer &putShort(short x) override;

            ByteBuffer &putShort(gint i, short x) override;

            ShortBuffer &asShortBuffer() const override;

            ByteBuffer &putInt(gint x) override;

            ByteBuffer &putInt(gint i, gint x) override;

            IntBuffer &asIntBuffer() const override;

            ByteBuffer &putLong(glong x) override;

            ByteBuffer &putLong(gint i, glong x) override;

            LongBuffer &asLongBuffer() const override;

            ByteBuffer &putFloat(gfloat x) override;

            ByteBuffer &putFloat(gint i, gfloat x) override;

            FloatBuffer &asFloatBuffer() const override;

            ByteBuffer &putDouble(gdouble x) override;

            ByteBuffer &putDouble(gint i, gdouble x) override;

            DoubleBuffer &asDoubleBuffer() const override;
        };

    } // io
} // core

#endif //CORE23_READONLYHEAPBYTEBUFFER_H
