//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "ByteBuffer.h"
#include <core/private/Unsafe.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/private/ReadOnlyBufferException.h>
#include <core/io/private/HeapByteBuffer.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace io {
        using namespace native;
        using namespace util;

        const glong ByteBuffer::ARRAY_BASE_OFFSET = Unsafe::ARRAY_BYTE_BASE_OFFSET;

        ByteBuffer::ByteBuffer(gint mark, gint pos, gint lim, gint cap, ByteArray &hb, gint offset)
                : Buffer(mark, pos, lim, cap), hb(&hb), offset(offset), isReadOnly(false) {}

        ByteBuffer::ByteBuffer(gint mark, gint pos, gint lim, gint cap)
                : Buffer(mark, pos, lim, cap), hb(null), offset(0), isReadOnly(false) {}

        ByteBuffer::ByteBuffer(ByteArray &hb, glong addr, gint cap) :
                Buffer(addr, cap), hb(&hb), offset(0), isReadOnly(false) {}

        Object &ByteBuffer::base() const {
            if (hb == null) {
                return null;
            } else {
                return *hb;
            }
        }

        ByteBuffer &ByteBuffer::allocateDirect(gint  /*capacity*/) {
            UnsupportedOperationException("require DirectByteBuffer class").throws(__trace("core.io.ByteBuffer"));
        }

        ByteBuffer &ByteBuffer::allocate(gint capacity) {
            if (capacity < 0)
                IllegalArgumentException("capacity < 0: (" + String::valueOf(capacity) + " < 0)")
                        .throws(__trace("core.io.ByteBuffer"));
            try{
                return Unsafe::allocateInstance<HeapByteBuffer>(capacity, capacity);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
            }
        }

        ByteBuffer &ByteBuffer::wrap(ByteArray &array, gint offset, gint length) {
            try{
                return Unsafe::allocateInstance<HeapByteBuffer>(array, offset, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapByteBuffer"));
            }
        }

        ByteBuffer &ByteBuffer::wrap(ByteArray &array) {
            try {
                return wrap(array, 0, array.length());
            } catch (const IllegalArgumentException &iaex) {
                iaex.throws(__trace("core.io.ByteBuffer"));
            }
        }

        ByteBuffer &ByteBuffer::get(ByteArray &dst, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(off, length, dst.length());
                gint const pos = position();
                if (length > limit() - pos)
                    BufferUnderflowException().throws(__trace("core.io.ByteBuffer"));

                getArray(pos, dst, off, length);

                return setPosition(pos + length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
        }

        ByteBuffer &ByteBuffer::get(ByteArray &dst) {
            try {
                return get(dst, 0, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
        }

        const ByteBuffer &ByteBuffer::get(gint index, ByteArray &dst, gint off, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, dst.length());
                getArray(index, dst, off, length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
        }

        const ByteBuffer &ByteBuffer::get(gint index, ByteArray &dst) const {
            try {
                return get(index, dst, 0, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
        }

        const ByteBuffer &ByteBuffer::getArray(gint index, ByteArray &dst, gint off, gint length) const {
            try {
                if (((glong) length << 0) > 6) {
                    glong const bufAddr = address + ((glong) index << 0);
                    glong const dstOffset = ARRAY_BASE_OFFSET + ((glong) off << 0);
                    glong const len = (glong) length << 0;
                    Unsafe::copyMemory(base(), bufAddr, dst, dstOffset, len);
                } else {
                    gint const end = off + length;
                    for (gint i = off, j = index; i < end; i++, j++) {
                        dst[i] = get(j);
                    }
                }
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
            return *this;
        }

        ByteBuffer &ByteBuffer::put(ByteBuffer &src) {
            if (this == &src)
                IllegalArgumentException("The source buffer is this buffer").throws(__trace("core.io.ByteBuffer"));
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.ByteBuffer"));
            gint const srcPos = src.position();
            gint const srcLim = src.limit();
            gint const srcRem = (srcPos <= srcLim ? srcLim - srcPos : 0);
            gint const pos = position();
            gint const lim = limit();
            gint const rem = (pos <= lim ? lim - pos : 0);

            if (srcRem > rem)
                BufferOverflowException().throws(__trace("core.io.ByteBuffer"));

            try {
                putBuffer(pos, src, srcPos, srcRem);

                setPosition(pos + srcRem);
                src.setPosition(srcPos + srcRem);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
            return *this;
        }

        ByteBuffer &ByteBuffer::put(gint index, ByteBuffer &src, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, src.limit());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.ByteBuffer"));
            try {
                putBuffer(index, src, off, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
            return *this;
        }

        void ByteBuffer::putBuffer(gint pos, ByteBuffer &src, gint srcPos, gint n) {
            const Object &srcBase = src.base();
            CORE_ASSERT(src.isDirect(), "core.io.ByteBuffer");

            Object &base = this->base();
            CORE_ASSERT(isDirect(), "core.io.ByteBuffer");

            glong const srcAddr = src.address + ((glong) srcPos << 0);
            glong const addr = address + ((glong) pos << 0);
            glong const len = (glong) n << 0;
            Unsafe::copyMemory(srcBase, srcAddr, base, addr, len);
        }

        ByteBuffer &ByteBuffer::put(const ByteArray &src, gint off, gint length) {
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.ByteBuffer"));
            try {
                Preconditions::checkIndexFromSize(off, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws(__trace("core.io.ByteBuffer"));

            try {
                putArray(pos, src, off, length);

                setPosition(pos + length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
            return *this;
        }

        ByteBuffer &ByteBuffer::put(const ByteArray &src) {
            try {
                return put(src, 0, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
        }

        ByteBuffer &ByteBuffer::put(gint index, const ByteArray &src, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.ByteBuffer"));
            try {
                putArray(index, src, off, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
            return *this;
        }

        ByteBuffer &ByteBuffer::put(gint index, const ByteArray &src) {
            try {
                return put(index, src, 0, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
        }

        ByteBuffer &ByteBuffer::putArray(gint index, const ByteArray &src, gint off, gint length) {
            if (((long) length << 0) > 6) {
                glong const bufAddr = address + ((long) index << 0);
                glong const srcOffset = ARRAY_BASE_OFFSET + ((long) off << 0);
                glong const len = (long) length << 0;

                Unsafe::copyMemory(src, srcOffset, base(), bufAddr, len);
            } else {
                gint const end = off + length;
                for (gint i = off, j = index; i < end; i++, j++)
                    put(j, src[i]);
            }
            return *this;
        }

        gbool ByteBuffer::hasArray() const {
            return hb != null && !isReadOnly;
        }

        ByteArray &ByteBuffer::array() const {
            if (hb == null)
                UnsupportedOperationException().throws(__trace("core.io.ByteBuffer"));
            if (isReadOnly)
                ReadOnlyBufferException().throws(__trace("core.io.ByteBuffer"));
            return *hb;
        }

        gint ByteBuffer::arrayOffset() const {
            if (hb == null)
                UnsupportedOperationException().throws(__trace("core.io.ByteBuffer"));
            if (isReadOnly)
                ReadOnlyBufferException().throws(__trace("core.io.ByteBuffer"));
            return offset;
        }

        ByteBuffer &ByteBuffer::setPosition(gint newPosition) {
            try {
                Buffer::setPosition(newPosition);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
            return *this;
        }

        ByteBuffer &ByteBuffer::setLimit(gint newLimit) {
            try {
                Buffer::setLimit(newLimit);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
            return *this;
        }

        ByteBuffer &ByteBuffer::mark() {
            try {
                Buffer::mark();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
            return *this;
        }

        ByteBuffer &ByteBuffer::reset() {
            try {
                Buffer::reset();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
            return *this;
        }

        ByteBuffer &ByteBuffer::clear() {
            try {
                Buffer::clear();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
            return *this;
        }

        ByteBuffer &ByteBuffer::flip() {
            try {
                Buffer::flip();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
            return *this;
        }

        ByteBuffer &ByteBuffer::rewind() {
            try {
                Buffer::rewind();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
            return *this;
        }

        String ByteBuffer::toString() const {
            return classname()
                   + "[pos=" + String::valueOf(position())
                   + " lim=" + String::valueOf(limit())
                   + " cap=" + String::valueOf(capacity())
                   + "]";
        }

        gint ByteBuffer::hash() const {
            gint h = 1;
            gint const p = position();
            for (gint i = limit() - 1; i >= p; i--)
                h = 31 * h + (gint) get(i);
            return h;
        }

        gbool ByteBuffer::equals(const Object &obj) const {
            if (this == &obj)
                return true;
            if (!Class<ByteBuffer>::hasInstance(obj))
                return false;
            ByteBuffer const &that = (ByteBuffer const &) obj;
            gint const thisPos = position();
            gint const thisRem = limit() - thisPos;
            gint const thatPos = position();
            gint const thatRem = that.limit() - thatPos;
            if (thisRem < 0 || thisRem != thatRem)
                return false;
            return mismatch(*this, thisPos, that, thatPos, thisRem) < 0;
        }

        gint ByteBuffer::mismatch(const ByteBuffer &a, gint aOff, const ByteBuffer &b, gint bOff, gint length) {
            gint i = 0;
            if (length > 7) {
                if (a.get(aOff) != b.get(bOff))
                    return 0;
                i = ArraysSupport::vectorizedMismatch(a.base(), a.address + aOff,
                                                      b.base(), b.address + bOff, length,
                                                      ArraysSupport::LOG2_ARRAY_BYTE_INDEX_SCALE);
                if (i >= 0) return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a.get(aOff + i) != b.get(bOff + i))
                    return i;
            }
            return -1;
        }

        gint ByteBuffer::compareTo(const ByteBuffer &that) const {
            gint const thisPos = position();
            gint const thisRem = limit() - thisPos;
            gint const thatPos = that.position();
            gint const thatRem = that.limit() - thatPos;
            gint const length = Math::min(thisRem, thatRem);
            if (length < 0)
                return -1;
            gint const i = mismatch(*this, thisPos, that, thatPos, length);
            if (i >= 0) {
                return Byte::compare(get(thisPos + i), that.get(thatPos + i));
            }
            return thisRem - thatRem;
        }

        gint ByteBuffer::mismatch(const ByteBuffer &that) const {
            gint const thisPos = position();
            gint const thisRem = limit() - thisPos;
            gint const thatPos = that.position();
            gint const thatRem = that.limit() - thatPos;
            gint const length = Math::min(thisRem, thatRem);
            if (length < 0)
                return -1;
            gint const r = mismatch(*this, thisPos, that, thatPos, length);
            return (r == -1 && thisRem != thatRem) ? length : r;
        }

        ByteBuffer::ByteOrder ByteBuffer::order() const {
            return bigEndian ? ByteOrder::BIG_ENDIAN : ByteOrder::LITTLE_ENDIAN;
        }

        ByteBuffer &ByteBuffer::setOrder(ByteBuffer::ByteOrder bo) {
            bigEndian = (bo == ByteOrder::BIG_ENDIAN);
            return *this;
        }

        gint ByteBuffer::alignmentOffset(gint index, gint unitSize) {
            if (index < 0)
                IllegalArgumentException("Index less than zero: " + String::valueOf(index))
                        .throws(__trace("core.io.ByteBuffer"));
            if (unitSize < 1 || (unitSize & (unitSize - 1)) != 0)
                IllegalArgumentException("Unit size not a power of two: " + String::valueOf(unitSize))
                        .throws(__trace("core.io.ByteBuffer"));
            if (unitSize > 8 && !isDirect())
                UnsupportedOperationException("Unit size unsupported for non-direct buffers: " + String::valueOf(unitSize))
                        .throws(__trace("core.io.ByteBuffer"));

            return (int) ((address + index) & (unitSize - 1));
        }

        ByteBuffer &ByteBuffer::alignedSlice(gint unitSize) {
            try{
                int const pos = position();
                int const lim = limit();

                int const pos_mod = alignmentOffset(pos, unitSize);
                int const lim_mod = alignmentOffset(lim, unitSize);

                // Round up the position to align with unit size
                int aligned_pos = (pos_mod > 0) ? pos + (unitSize - pos_mod) : pos;

                // Round down the limit to align with unit size
                int aligned_lim = lim - lim_mod;

                if (aligned_pos > lim || aligned_lim < pos) {
                    aligned_pos = aligned_lim = pos;
                }

                return slice(aligned_pos, aligned_lim - aligned_pos);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ByteBuffer"));
            }
        }

    } // core
} // io
