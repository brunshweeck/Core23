//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#include "ReadOnlyHeapDoubleBuffer.h"
#include <core/util/Preconditions.h>
#include <core/private/Unsafe.h>
#include <core/io/private/ReadOnlyBufferException.h>
#include <core/io/BufferOverflowException.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        ReadOnlyHeapDoubleBuffer::ReadOnlyHeapDoubleBuffer(gint cap, gint lim) :
                HeapDoubleBuffer(cap, lim) { DoubleBuffer::isReadOnly = true; }

        ReadOnlyHeapDoubleBuffer::ReadOnlyHeapDoubleBuffer(DoubleArray &buf, gint off, gint len) :
                HeapDoubleBuffer(buf, off, len) { DoubleBuffer::isReadOnly = true; }

        ReadOnlyHeapDoubleBuffer::ReadOnlyHeapDoubleBuffer(DoubleArray &buf, gint mark, gint pos, gint lim, gint cap,
                                                           gint off) :
                HeapDoubleBuffer(buf, mark, pos, lim, cap, off) { DoubleBuffer::isReadOnly = true; }

        DoubleBuffer &ReadOnlyHeapDoubleBuffer::slice() const {
            try {
                gint const pos = position();
                gint const lim = limit();
                gint const rem = (pos <= lim ? lim - pos : 0);
                return Unsafe::allocateInstance<ReadOnlyHeapDoubleBuffer>((*hb), -1, 0, rem, rem, pos + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapDoubleBuffer"));
            }
        }

        DoubleBuffer &ReadOnlyHeapDoubleBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return Unsafe::allocateInstance<ReadOnlyHeapDoubleBuffer>((*hb), -1, 0, length, length, index + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapDoubleBuffer"));
            }
        }

        DoubleBuffer &ReadOnlyHeapDoubleBuffer::duplicate() const {
            try {
                return Unsafe::allocateInstance<ReadOnlyHeapDoubleBuffer>((*hb), markValue(), position(), limit(),
                                                                          capacity(), offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapDoubleBuffer"));
            }
        }

        DoubleBuffer &ReadOnlyHeapDoubleBuffer::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapDoubleBuffer"));
            }
        }

        gbool ReadOnlyHeapDoubleBuffer::isReadOnly() const {
            return true;
        }

        DoubleBuffer &ReadOnlyHeapDoubleBuffer::put(gdouble  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapDoubleBuffer"));
        }

        DoubleBuffer &ReadOnlyHeapDoubleBuffer::put(gint  /*i*/, gdouble  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapDoubleBuffer"));
        }

        DoubleBuffer &ReadOnlyHeapDoubleBuffer::put(const DoubleArray & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapDoubleBuffer"));
        }

        DoubleBuffer &ReadOnlyHeapDoubleBuffer::put(DoubleBuffer & /*src*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapDoubleBuffer"));
        }

        DoubleBuffer &
        ReadOnlyHeapDoubleBuffer::put(gint  /*index*/, DoubleBuffer & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapDoubleBuffer"));
        }

        DoubleBuffer &ReadOnlyHeapDoubleBuffer::put(gint  /*index*/, const DoubleArray & /*src*/, gint  /*offset*/,
                                                    gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapDoubleBuffer"));
        }

        DoubleBuffer &ReadOnlyHeapDoubleBuffer::compact() {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapDoubleBuffer"));
        }

        DoubleBuffer::ByteOrder ReadOnlyHeapDoubleBuffer::order() const {
            return ByteOrder::NATIVE_ENDIAN;
        }
    } // io
} // core