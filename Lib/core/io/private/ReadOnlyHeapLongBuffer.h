//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#ifndef CORE23_READONLYHEAPLONGBUFFER_H
#define CORE23_READONLYHEAPLONGBUFFER_H

#include <core/io/private/HeapLongBuffer.h>

namespace core {
    namespace io {

        /**
         * A read-only HeapLongBuffer.  This class extends the corresponding
         * read/write class, overriding the mutation methods to throw a <b style="color:orange;">
         * ReadOnlyBufferException</b> and overriding the view-buffer methods to return an
         * instance of this class rather than of the superclass.
         */
        class ReadOnlyHeapLongBuffer CORE_FINAL /*private*/: public HeapLongBuffer {
        public:
            ReadOnlyHeapLongBuffer(gint cap, gint lim);

            ReadOnlyHeapLongBuffer(LongArray &buf, gint off, gint len);

            ReadOnlyHeapLongBuffer(LongArray &buf, gint mark, gint pos, gint lim, gint cap, gint off);

            LongBuffer &slice() const override;

            LongBuffer &slice(gint index, gint length) const override;

            LongBuffer &duplicate() const override;

            LongBuffer &asReadOnlyBuffer() const override;

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

#endif //CORE23_READONLYHEAPLONGBUFFER_H
