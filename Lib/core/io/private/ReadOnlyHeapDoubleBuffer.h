//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#ifndef CORE23_READONLYHEAPDOUBLEBUFFER_H
#define CORE23_READONLYHEAPDOUBLEBUFFER_H

#include <core/io/private/HeapDoubleBuffer.h>

namespace core {
    namespace io {

        /**
         * A read-only HeapDoubleBuffer.  This class extends the corresponding
         * read/write class, overriding the mutation methods to throw a <b style="color:orange;">
         * ReadOnlyBufferException</b> and overriding the view-buffer methods to return an
         * instance of this class rather than of the superclass.
         */
        class ReadOnlyHeapDoubleBuffer CORE_FINAL /*private*/: public HeapDoubleBuffer {
        public:
            ReadOnlyHeapDoubleBuffer(gint cap, gint lim);

            ReadOnlyHeapDoubleBuffer(DoubleArray &buf, gint off, gint len);

            ReadOnlyHeapDoubleBuffer(DoubleArray &buf, gint mark, gint pos, gint lim, gint cap, gint off);

            DoubleBuffer &slice() const override;

            DoubleBuffer &slice(gint index, gint length) const override;

            DoubleBuffer &duplicate() const override;

            DoubleBuffer &asReadOnlyBuffer() const override;

            gbool isReadOnly() const override;

            DoubleBuffer &put(gdouble x) override;

            DoubleBuffer &put(gint i, gdouble x) override;

            DoubleBuffer &put(const DoubleArray &src, gint offset, gint length) override;

            DoubleBuffer &put(DoubleBuffer &src) override;

            DoubleBuffer &put(gint index, DoubleBuffer &src, gint offset, gint length) override;

            DoubleBuffer &put(gint index, const DoubleArray &src, gint offset, gint length) override;

            DoubleBuffer &compact() override;

            ByteOrder order() const override;
        };

    } // io
} // core

#endif //CORE23_READONLYHEAPDOUBLEBUFFER_H
