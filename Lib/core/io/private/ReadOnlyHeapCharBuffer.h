//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#ifndef CORE23_READONLYHEAPCHARBUFFER_H
#define CORE23_READONLYHEAPCHARBUFFER_H

#include <core/io/private/HeapCharBuffer.h>

namespace core {
    namespace io {

        /**
         * A read-only HeapCharBuffer.  This class extends the corresponding
         * read/write class, overriding the mutation methods to throw a <b style="color:orange;">
         * ReadOnlyBufferException</b> and overriding the view-buffer methods to return an
         * instance of this class rather than of the superclass.
         */
        class ReadOnlyHeapCharBuffer CORE_FINAL /*private*/: public HeapCharBuffer {
        public:
            ReadOnlyHeapCharBuffer(gint cap, gint lim);

            ReadOnlyHeapCharBuffer(CharArray &buf, gint off, gint len);

            ReadOnlyHeapCharBuffer(CharArray &buf, gint mark, gint pos, gint lim, gint cap, gint off);

            CharBuffer &slice() const override;

            CharBuffer &slice(gint index, gint length) const override;

            CharBuffer &duplicate() const override;

            CharBuffer &asReadOnlyBuffer() const override;

            gbool isReadOnly() const override;

            CharBuffer &put(gchar x) override;

            CharBuffer &put(gint i, gchar x) override;

            CharBuffer &put(const CharArray &src, gint offset, gint length) override;

            CharBuffer &put(CharBuffer &src) override;

            CharBuffer &put(gint index, CharBuffer &src, gint offset, gint length) override;

            CharBuffer &put(gint index, const CharArray &src, gint offset, gint length) override;

            CharBuffer &put(const String &src, gint start, gint end) override;

            CharBuffer &compact() override;

            String toString(gint start, gint end) const override;

            CharBuffer &subSequence(gint start, gint end) const override;

            ByteOrder order() const override;

            OptionalByteOrder charRegionOrder() const override;
        };

    } // io
} // core

#endif //CORE23_READONLYHEAPCHARBUFFER_H
