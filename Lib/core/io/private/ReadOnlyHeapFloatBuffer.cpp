//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#include "ReadOnlyHeapFloatBuffer.h"
#include <core/util/Preconditions.h>
#include <core/private/Unsafe.h>
#include <core/io/private/ReadOnlyBufferException.h>
#include <core/io/BufferOverflowException.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        ReadOnlyHeapFloatBuffer::ReadOnlyHeapFloatBuffer(gint cap, gint lim) :
                HeapFloatBuffer(cap, lim) { FloatBuffer::isReadOnly = true; }

        ReadOnlyHeapFloatBuffer::ReadOnlyHeapFloatBuffer(FloatArray &buf, gint off, gint len) :
                HeapFloatBuffer(buf, off, len) { FloatBuffer::isReadOnly = true; }

        ReadOnlyHeapFloatBuffer::ReadOnlyHeapFloatBuffer(FloatArray &buf, gint mark, gint pos, gint lim, gint cap,
                                                         gint off) :
                HeapFloatBuffer(buf, mark, pos, lim, cap, off) { FloatBuffer::isReadOnly = true; }

        FloatBuffer &ReadOnlyHeapFloatBuffer::slice() const {
            try {
                gint const pos = position();
                gint const lim = limit();
                gint const rem = (pos <= lim ? lim - pos : 0);
                return Unsafe::allocateInstance<ReadOnlyHeapFloatBuffer>((*hb), -1, 0, rem, rem, pos + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapFloatBuffer"));
            }
        }

        FloatBuffer &ReadOnlyHeapFloatBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return Unsafe::allocateInstance<ReadOnlyHeapFloatBuffer>((*hb), -1, 0, length, length, index + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapFloatBuffer"));
            }
        }

        FloatBuffer &ReadOnlyHeapFloatBuffer::duplicate() const {
            try {
                return Unsafe::allocateInstance<ReadOnlyHeapFloatBuffer>((*hb), markValue(), position(), limit(),
                                                                         capacity(), offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapFloatBuffer"));
            }
        }

        FloatBuffer &ReadOnlyHeapFloatBuffer::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapFloatBuffer"));
            }
        }

        gbool ReadOnlyHeapFloatBuffer::isReadOnly() const {
            return true;
        }

        FloatBuffer &ReadOnlyHeapFloatBuffer::put(gfloat  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapFloatBuffer"));
        }

        FloatBuffer &ReadOnlyHeapFloatBuffer::put(gint  /*i*/, gfloat  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapFloatBuffer"));
        }

        FloatBuffer &ReadOnlyHeapFloatBuffer::put(const FloatArray & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapFloatBuffer"));
        }

        FloatBuffer &ReadOnlyHeapFloatBuffer::put(FloatBuffer & /*src*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapFloatBuffer"));
        }

        FloatBuffer &
        ReadOnlyHeapFloatBuffer::put(gint  /*index*/, FloatBuffer & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapFloatBuffer"));
        }

        FloatBuffer &
        ReadOnlyHeapFloatBuffer::put(gint  /*index*/, const FloatArray & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapFloatBuffer"));
        }

        FloatBuffer &ReadOnlyHeapFloatBuffer::compact() {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapFloatBuffer"));
        }

        FloatBuffer::ByteOrder ReadOnlyHeapFloatBuffer::order() const {
            return ByteOrder::NATIVE_ENDIAN;
        }
    } // io
} // core