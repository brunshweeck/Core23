//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#ifndef CORE23_HEAPCHARBUFFER_H
#define CORE23_HEAPCHARBUFFER_H

#include <core/io/CharBuffer.h>

namespace core {
    namespace io {

        class HeapCharBuffer /* private */: public CharBuffer {
        private:
            static const glong ARRAY_BASE_OFFSET;
            static const glong ARRAY_INDEX_SCALE;

        public:
            CORE_EXPLICIT HeapCharBuffer(int cap, int lim);

            CORE_EXPLICIT HeapCharBuffer(CharArray &buf, int off, int len);

            CORE_EXPLICIT HeapCharBuffer(CharArray &buf, int mark, int pos, int lim, int cap, int off);

            CharBuffer &slice() const override;

            CharBuffer &slice(int index, int length) const override;

            CharBuffer &duplicate() const override;

            CharBuffer &asReadOnlyBuffer() const override;

        protected:
            gint ix(int i) const;

        public:
            gchar get() override;

            gchar get(int i) const override;

            gchar getUnchecked(int i) const override;

            CharBuffer &get(CharArray &dst, int offset, int length) override;

            CharBuffer &get(int index, CharArray &dst, int offset, int length) const override;

            gbool isDirect() const override;

            gbool isReadOnly() const override;

            CharBuffer &put(gchar x) override;

            CharBuffer &put(int i, gchar x) override;

            CharBuffer &put(const CharArray &src, int offset, int length) override;

            CharBuffer &put(CharBuffer &src) override;

            CharBuffer &put(int index, CharBuffer &src, int offset, int length) override;

            CharBuffer &put(int index, CharArray &src, int offset, int length);

            CharBuffer &put(const String &src, int start, int end) override;

            CharBuffer &compact() override;

        protected:

            String toString(int start, int end) const override;

        public:

            CharBuffer &subSequence(int start, int end) const override;

            ByteOrder order() const override;

            OptionalByteOrder charRegionOrder() const override;
        };

    } // io
} // core

#endif //CORE23_HEAPCHARBUFFER_H
