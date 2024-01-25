//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#include "ReadOnlyHeapIntBuffer.h"
#include <core/util/Preconditions.h>
#include <core/private/Unsafe.h>
#include <core/io/private/ReadOnlyBufferException.h>
#include <core/io/BufferOverflowException.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        ReadOnlyHeapIntBuffer::ReadOnlyHeapIntBuffer(gint cap, gint lim) :
                HeapIntBuffer(cap, lim) { IntBuffer::isReadOnly = true; }

        ReadOnlyHeapIntBuffer::ReadOnlyHeapIntBuffer(IntArray &buf, gint off, gint len) :
                HeapIntBuffer(buf, off, len) { IntBuffer::isReadOnly = true; }

        ReadOnlyHeapIntBuffer::ReadOnlyHeapIntBuffer(IntArray &buf, gint mark, gint pos, gint lim, gint cap, gint off) :
                HeapIntBuffer(buf, mark, pos, lim, cap, off) { IntBuffer::isReadOnly = true; }

        IntBuffer &ReadOnlyHeapIntBuffer::slice() const {
            try {
                gint const pos = position();
                gint const lim = limit();
                gint const rem = (pos <= lim ? lim - pos : 0);
                return Unsafe::allocateInstance<ReadOnlyHeapIntBuffer>((*hb), -1, 0, rem, rem, pos + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapIntBuffer"));
            }
        }

        IntBuffer &ReadOnlyHeapIntBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return Unsafe::allocateInstance<ReadOnlyHeapIntBuffer>((*hb), -1, 0, length, length, index + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapIntBuffer"));
            }
        }

        IntBuffer &ReadOnlyHeapIntBuffer::duplicate() const {
            try {
                return Unsafe::allocateInstance<ReadOnlyHeapIntBuffer>((*hb), markValue(), position(), limit(),
                                                                       capacity(), offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapIntBuffer"));
            }
        }

        IntBuffer &ReadOnlyHeapIntBuffer::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapIntBuffer"));
            }
        }

        gbool ReadOnlyHeapIntBuffer::isReadOnly() const {
            return true;
        }

        IntBuffer &ReadOnlyHeapIntBuffer::put(gint  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapIntBuffer"));
        }

        IntBuffer &ReadOnlyHeapIntBuffer::put(gint  /*i*/, gint  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapIntBuffer"));
        }

        IntBuffer &ReadOnlyHeapIntBuffer::put(const IntArray & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapIntBuffer"));
        }

        IntBuffer &ReadOnlyHeapIntBuffer::put(IntBuffer & /*src*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapIntBuffer"));
        }

        IntBuffer &
        ReadOnlyHeapIntBuffer::put(gint  /*index*/, IntBuffer & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapIntBuffer"));
        }

        IntBuffer &
        ReadOnlyHeapIntBuffer::put(gint  /*index*/, const IntArray & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapIntBuffer"));
        }

        IntBuffer &ReadOnlyHeapIntBuffer::compact() {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapIntBuffer"));
        }

        IntBuffer::ByteOrder ReadOnlyHeapIntBuffer::order() const {
            return ByteOrder::NATIVE_ENDIAN;
        }
    } // io
} // core