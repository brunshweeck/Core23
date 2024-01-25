//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#ifndef CORE23_READONLYHEAPINTBUFFER_H
#define CORE23_READONLYHEAPINTBUFFER_H

#include <core/io/private/HeapIntBuffer.h>

namespace core {
    namespace io {

        /**
         * A read-only HeapIntBuffer.  This class extends the corresponding
         * read/write class, overriding the mutation methods to throw a <b style="color:orange;">
         * ReadOnlyBufferException</b> and overriding the view-buffer methods to return an
         * instance of this class rather than of the superclass.
         */
        class ReadOnlyHeapIntBuffer CORE_FINAL /*private*/: public HeapIntBuffer {
        public:
            ReadOnlyHeapIntBuffer(gint cap, gint lim);

            ReadOnlyHeapIntBuffer(IntArray &buf, gint off, gint len);

            ReadOnlyHeapIntBuffer(IntArray &buf, gint mark, gint pos, gint lim, gint cap, gint off);

            IntBuffer &slice() const override;

            IntBuffer &slice(gint index, gint length) const override;

            IntBuffer &duplicate() const override;

            IntBuffer &asReadOnlyBuffer() const override;

            gbool isReadOnly() const override;

            IntBuffer &put(gint x) override;

            IntBuffer &put(gint i, gint x) override;

            IntBuffer &put(const IntArray &src, gint offset, gint length) override;

            IntBuffer &put(IntBuffer &src) override;

            IntBuffer &put(gint index, IntBuffer &src, gint offset, gint length) override;

            IntBuffer &put(gint index, const IntArray &src, gint offset, gint length) override;

            IntBuffer &compact() override;

            ByteOrder order() const override;
        };

    } // io
} // core

#endif //CORE23_READONLYHEAPINTBUFFER_H
