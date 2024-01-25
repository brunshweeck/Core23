//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#ifndef CORE23_READONLYHEAPSHORTBUFFER_H
#define CORE23_READONLYHEAPSHORTBUFFER_H

#include <core/io/private/HeapShortBuffer.h>

namespace core {
    namespace io {

        /**
         * A read-only HeapShortBuffer.  This class extends the corresponding
         * read/write class, overriding the mutation methods to throw a <b style="color:orange;">
         * ReadOnlyBufferException</b> and overriding the view-buffer methods to return an
         * instance of this class rather than of the superclass.
         */
        class ReadOnlyHeapShortBuffer CORE_FINAL /*private*/: public HeapShortBuffer {
        public:
            ReadOnlyHeapShortBuffer(gint cap, gint lim);

            ReadOnlyHeapShortBuffer(ShortArray &buf, gint off, gint len);

            ReadOnlyHeapShortBuffer(ShortArray &buf, gint mark, gint pos, gint lim, gint cap, gint off);

            ShortBuffer &slice() const override;

            ShortBuffer &slice(gint index, gint length) const override;

            ShortBuffer &duplicate() const override;

            ShortBuffer &asReadOnlyBuffer() const override;

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

#endif //CORE23_READONLYHEAPSHORTBUFFER_H
