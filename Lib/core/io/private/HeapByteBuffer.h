//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#ifndef CORE23_HEAPBYTEBUFFER_H
#define CORE23_HEAPBYTEBUFFER_H

#include <core/io/ByteBuffer.h>

namespace core {
    namespace io {
        /**
         * A read/write HeapByteBuffer .
         */
        class HeapByteBuffer /* private */ : public ByteBuffer {
        private:
            static const glong ARRAY_BASE_OFFSET;
            static const glong ARRAY_INDEX_SCALE;

        public:
            CORE_EXPLICIT HeapByteBuffer(gint cap, gint lim);

            CORE_EXPLICIT HeapByteBuffer(ByteArray &buf, gint off, gint len);

            CORE_EXPLICIT HeapByteBuffer(ByteArray &buf, gint mark, gint pos, gint lim, gint cap, gint off);

        private:
            // The buffer views
            class CharBufferView;
            class DoubleBufferView;
            class FloatBufferView;
            class IntBufferView;
            class LongBufferView;
            class ShortBufferView;

        public:
            ByteBuffer &slice() const override;

            ByteBuffer &slice(gint index, gint length) const override;

            ByteBuffer &duplicate() const override;

            ByteBuffer &asReadOnlyBuffer() const override;

        protected:
            gint ix(gint i) const;

            glong byteOffset(glong i) const;

        public:
            gbyte get() override;

            gbyte get(gint i) const override;


            ByteBuffer &get(ByteArray &dst, gint offset, gint length) override;

            ByteBuffer &get(gint index, ByteArray &dst, gint offset, gint length) const override;

            gbool isDirect() const override;

            gbool isReadOnly() const override;

            ByteBuffer &put(gbyte x) override;

            ByteBuffer &put(gint i, gbyte x) override;

            ByteBuffer &put(const ByteArray &src, gint offset, gint length) override;

            ByteBuffer &put(ByteBuffer &src) override;

            ByteBuffer &put(gint index, ByteBuffer &src, gint offset, gint length) override;

            ByteBuffer &put(gint index, const ByteArray &src, gint offset, gint length) override;

            ByteBuffer & compact() override;

        protected:
            virtual gbyte _get(gint i) const;

            virtual void _put(gint i, gbyte b);

        public:

            gchar getChar() override;

            gchar getChar(gint i) const override;

            ByteBuffer &putChar(gchar x) override;

            ByteBuffer &putChar(gint i, gchar x) override;

            CharBuffer &asCharBuffer() const override;
            
            gshort getShort() override;

            gshort getShort(gint i) const override;

            ByteBuffer &putShort(gshort x) override;

            ByteBuffer &putShort(gint i, gshort x) override;

            ShortBuffer &asShortBuffer() const override;

            gint getInt() override;

            gint getInt(gint i) const override;

            ByteBuffer &putInt(gint x) override;

            ByteBuffer &putInt(gint i, gint x) override;

            IntBuffer &asIntBuffer() const override;

            glong getLong() override;

            glong getLong(gint i) const override;

            ByteBuffer &putLong(glong x) override;

            ByteBuffer &putLong(gint i, glong x) override;

            LongBuffer &asLongBuffer() const override;

            gfloat getFloat() override;

            gfloat getFloat(gint i) const override;

            ByteBuffer &putFloat(gfloat x) override;

            ByteBuffer &putFloat(gint i, gfloat x) override;

            FloatBuffer &asFloatBuffer() const override;

            gdouble getDouble() override;

            gdouble getDouble(gint i) const override;

            ByteBuffer &putDouble(gdouble x) override;

            ByteBuffer &putDouble(gint i, gdouble x) override;

            DoubleBuffer &asDoubleBuffer() const override;
        };

    } // io
} // core

#endif //CORE23_HEAPBYTEBUFFER_H
