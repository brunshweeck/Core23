//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#ifndef CORE23_HEAPDOUBLEBUFFER_H
#define CORE23_HEAPDOUBLEBUFFER_H

#include <core/io/DoubleBuffer.h>

namespace core {
    namespace io {

        class HeapDoubleBuffer /*private*/: public DoubleBuffer {
        private:
            static const glong ARRAY_BASE_OFFSET;
            static const glong ARRAY_INDEX_SCALE;

        public:
            HeapDoubleBuffer(gint cap, gint lim);

            HeapDoubleBuffer(DoubleArray &buf, gint off, gint len);

            HeapDoubleBuffer(DoubleArray &buf, gint mark, gint pos, gint lim, gint cap, gint off);

            DoubleBuffer &slice() const override;

            DoubleBuffer& slice(gint index, gint length) const override;

            DoubleBuffer &duplicate() const override;

            DoubleBuffer &asReadOnlyBuffer() const override;

        protected:
            gint ix(gint i) const;

        public:
            gdouble get() override;

            gdouble get(gint i) const override;

            DoubleBuffer &get(DoubleArray &dst, gint offset, gint length) override;

            DoubleBuffer &get(gint index, DoubleArray &dst, gint offset, gint length) const override;

            gbool isDirect() const override;

            gbool isReadOnly() const override;

            DoubleBuffer &put(gdouble x) override;

            DoubleBuffer &put(gint i, gdouble x) override;

            DoubleBuffer &put(DoubleArray &src, gint offset, gint length);

            DoubleBuffer &put(DoubleBuffer &src) override;

            DoubleBuffer &put(gint index, DoubleBuffer &src, gint offset, gint length) override;

            DoubleBuffer &put(gint index, const DoubleArray &src, gint offset, gint length) override;

            DoubleBuffer &compact() override;

            ByteOrder order() const override;
        };

    } // io
} // core

#endif //CORE23_HEAPDOUBLEBUFFER_H
