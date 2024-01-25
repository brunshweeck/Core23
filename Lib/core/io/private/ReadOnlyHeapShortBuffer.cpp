//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#include "ReadOnlyHeapShortBuffer.h"
#include <core/util/Preconditions.h>
#include <core/private/Unsafe.h>
#include <core/io/private/ReadOnlyBufferException.h>
#include <core/io/BufferOverflowException.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        ReadOnlyHeapShortBuffer::ReadOnlyHeapShortBuffer(gint cap, gint lim) :
                HeapShortBuffer(cap, lim) { ShortBuffer::isReadOnly = true; }

        ReadOnlyHeapShortBuffer::ReadOnlyHeapShortBuffer(ShortArray &buf, gint off, gint len) :
                HeapShortBuffer(buf, off, len) { ShortBuffer::isReadOnly = true; }

        ReadOnlyHeapShortBuffer::ReadOnlyHeapShortBuffer(ShortArray &buf, gint mark, gint pos, gint lim, gint cap,
                                                         gint off) :
                HeapShortBuffer(buf, mark, pos, lim, cap, off) { ShortBuffer::isReadOnly = true; }

        ShortBuffer &ReadOnlyHeapShortBuffer::slice() const {
            try {
                gint const pos = position();
                gint const lim = limit();
                gint const rem = (pos <= lim ? lim - pos : 0);
                return Unsafe::allocateInstance<ReadOnlyHeapShortBuffer>((*hb), -1, 0, rem, rem, pos + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapShortBuffer"));
            }
        }

        ShortBuffer &ReadOnlyHeapShortBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return Unsafe::allocateInstance<ReadOnlyHeapShortBuffer>((*hb), -1, 0, length, length, index + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapShortBuffer"));
            }
        }

        ShortBuffer &ReadOnlyHeapShortBuffer::duplicate() const {
            try {
                return Unsafe::allocateInstance<ReadOnlyHeapShortBuffer>((*hb), markValue(), position(), limit(),
                                                                         capacity(),
                                                                         offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapShortBuffer"));
            }
        }

        ShortBuffer &ReadOnlyHeapShortBuffer::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapShortBuffer"));
            }
        }

        gbool ReadOnlyHeapShortBuffer::isReadOnly() const {
            return true;
        }

        ShortBuffer &ReadOnlyHeapShortBuffer::put(gshort  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapShortBuffer"));
        }

        ShortBuffer &ReadOnlyHeapShortBuffer::put(gint  /*i*/, gshort  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapShortBuffer"));
        }

        ShortBuffer &ReadOnlyHeapShortBuffer::put(const ShortArray & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapShortBuffer"));
        }

        ShortBuffer &ReadOnlyHeapShortBuffer::put(ShortBuffer & /*src*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapShortBuffer"));
        }

        ShortBuffer &
        ReadOnlyHeapShortBuffer::put(gint  /*index*/, ShortBuffer & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapShortBuffer"));
        }

        ShortBuffer &
        ReadOnlyHeapShortBuffer::put(gint  /*index*/, const ShortArray & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapShortBuffer"));
        }

        ShortBuffer &ReadOnlyHeapShortBuffer::compact() {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapShortBuffer"));
        }

        ShortBuffer::ByteOrder ReadOnlyHeapShortBuffer::order() const {
            return ByteOrder::NATIVE_ENDIAN;
        }
    } // io
} // core