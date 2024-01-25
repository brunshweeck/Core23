//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#include "ReadOnlyHeapLongBuffer.h"
#include <core/util/Preconditions.h>
#include <core/private/Unsafe.h>
#include <core/io/private/ReadOnlyBufferException.h>
#include <core/io/BufferOverflowException.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        ReadOnlyHeapLongBuffer::ReadOnlyHeapLongBuffer(gint cap, gint lim) :
                HeapLongBuffer(cap, lim) { LongBuffer::isReadOnly = true; }

        ReadOnlyHeapLongBuffer::ReadOnlyHeapLongBuffer(LongArray &buf, gint off, gint len) :
                HeapLongBuffer(buf, off, len) { LongBuffer::isReadOnly = true; }

        ReadOnlyHeapLongBuffer::ReadOnlyHeapLongBuffer(LongArray &buf, gint mark, gint pos, gint lim, gint cap,
                                                       gint off) :
                HeapLongBuffer(buf, mark, pos, lim, cap, off) { LongBuffer::isReadOnly = true; }

        LongBuffer &ReadOnlyHeapLongBuffer::slice() const {
            try {
                gint const pos = position();
                gint const lim = limit();
                gint const rem = (pos <= lim ? lim - pos : 0);
                return Unsafe::allocateInstance<ReadOnlyHeapLongBuffer>((*hb), -1, 0, rem, rem, pos + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapLongBuffer"));
            }
        }

        LongBuffer &ReadOnlyHeapLongBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return Unsafe::allocateInstance<ReadOnlyHeapLongBuffer>((*hb), -1, 0, length, length, index + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapLongBuffer"));
            }
        }

        LongBuffer &ReadOnlyHeapLongBuffer::duplicate() const {
            try {
                return Unsafe::allocateInstance<ReadOnlyHeapLongBuffer>((*hb), markValue(), position(), limit(),
                                                                        capacity(), offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapLongBuffer"));
            }
        }

        LongBuffer &ReadOnlyHeapLongBuffer::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapLongBuffer"));
            }
        }

        gbool ReadOnlyHeapLongBuffer::isReadOnly() const {
            return true;
        }

        LongBuffer &ReadOnlyHeapLongBuffer::put(glong  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapLongBuffer"));
        }

        LongBuffer &ReadOnlyHeapLongBuffer::put(gint  /*i*/, glong  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapLongBuffer"));
        }

        LongBuffer &ReadOnlyHeapLongBuffer::put(const LongArray & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapLongBuffer"));
        }

        LongBuffer &ReadOnlyHeapLongBuffer::put(LongBuffer & /*src*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapLongBuffer"));
        }

        LongBuffer &
        ReadOnlyHeapLongBuffer::put(gint  /*index*/, LongBuffer & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapLongBuffer"));
        }

        LongBuffer &
        ReadOnlyHeapLongBuffer::put(gint  /*index*/, const LongArray & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapLongBuffer"));
        }

        LongBuffer &ReadOnlyHeapLongBuffer::compact() {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapLongBuffer"));
        }

        LongBuffer::ByteOrder ReadOnlyHeapLongBuffer::order() const {
            return ByteOrder::NATIVE_ENDIAN;
        }
    } // io
} // core