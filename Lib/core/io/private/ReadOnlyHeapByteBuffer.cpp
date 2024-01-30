//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#include "ReadOnlyHeapByteBuffer.h"
#include <core/private/Unsafe.h>
#include <core/util/Preconditions.h>
#include <core/io/private/ReadOnlyBufferException.h>
#include <core/io/CharBuffer.h>
#include <core/io/DoubleBuffer.h>
#include <core/io/FloatBuffer.h>
#include <core/io/IntBuffer.h>
#include <core/io/LongBuffer.h>
#include <core/io/ShortBuffer.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        ReadOnlyHeapByteBuffer::ReadOnlyHeapByteBuffer(gint cap, gint lim) : HeapByteBuffer(cap, lim) { ByteBuffer::isReadOnly = true; }

        ReadOnlyHeapByteBuffer::ReadOnlyHeapByteBuffer(ByteArray &buf, gint off, gint len) : HeapByteBuffer(buf, off, len) { ByteBuffer::isReadOnly = true; }

        ReadOnlyHeapByteBuffer::ReadOnlyHeapByteBuffer(ByteArray &buf, gint mark, gint pos, gint lim, gint cap, gint off) : HeapByteBuffer(buf, mark, pos, lim, cap, off) { ByteBuffer::isReadOnly = true; }

        ByteBuffer &ReadOnlyHeapByteBuffer::slice() const {
            try {
                gint const pos = position();
                gint const lim = limit();
                gint const rem = (pos <= lim ? lim - pos : 0);
                return Unsafe::allocateInstance<ReadOnlyHeapByteBuffer>(*hb, -1, 0, rem, rem, pos + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
            }
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return Unsafe::allocateInstance<ReadOnlyHeapByteBuffer>(*hb, -1, 0, length, length, index + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
            }
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::duplicate() const {
            try {
                return Unsafe::allocateInstance<ReadOnlyHeapByteBuffer>(*hb, markValue(), position(), limit(),
                                                                        capacity(),
                                                                        offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
            }
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
            }
        }

        gbool ReadOnlyHeapByteBuffer::isReadOnly() const {
            return true;
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::put(gbyte  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::put(gint  /*i*/, gbyte  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::put(const ByteArray & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::put(ByteBuffer & /*src*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        ByteBuffer &
        ReadOnlyHeapByteBuffer::put(gint  /*index*/, ByteBuffer & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        ByteBuffer &
        ReadOnlyHeapByteBuffer::put(gint  /*index*/, const ByteArray & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));

        }

        ByteBuffer &ReadOnlyHeapByteBuffer::compact() {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        gbyte ReadOnlyHeapByteBuffer::_get(gint i) const {
            try {
                return (*hb)[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
            }
        }

        void ReadOnlyHeapByteBuffer::_put(gint  /*i*/, gbyte  /*b*/) {
            try {
                ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
            }
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::putChar(gchar  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::putChar(gint  /*i*/, gchar  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        CharBuffer &ReadOnlyHeapByteBuffer::asCharBuffer() const {
            return HeapByteBuffer::asCharBuffer();
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::putShort(short  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::putShort(gint  /*i*/, short  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        ShortBuffer &ReadOnlyHeapByteBuffer::asShortBuffer() const {
            return HeapByteBuffer::asShortBuffer();
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::putInt(gint  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::putInt(gint  /*i*/, gint  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        IntBuffer &ReadOnlyHeapByteBuffer::asIntBuffer() const {
            return HeapByteBuffer::asIntBuffer();
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::putLong(glong  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::putLong(gint  /*i*/, glong  /*x*/) {
            try {
                ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
            }

        }

        LongBuffer &ReadOnlyHeapByteBuffer::asLongBuffer() const {
            return HeapByteBuffer::asLongBuffer();
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::putFloat(gfloat  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::putFloat(gint  /*i*/, gfloat  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        FloatBuffer &ReadOnlyHeapByteBuffer::asFloatBuffer() const {
            return HeapByteBuffer::asFloatBuffer();
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::putDouble(gdouble  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        ByteBuffer &ReadOnlyHeapByteBuffer::putDouble(gint  /*i*/, gdouble  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
        }

        DoubleBuffer &ReadOnlyHeapByteBuffer::asDoubleBuffer() const {
            try {
                return HeapByteBuffer::asDoubleBuffer();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
            }
        }
    } // io
} // core