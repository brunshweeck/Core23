//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "IntBuffer.h"
#include <core/private/Unsafe.h>
#include <core/IllegalArgumentException.h>
#include <core/native/IntArray.h>
#include <core/AssertionError.h>
#include <core/util/Preconditions.h>
#include <core/Math.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/private/ReadOnlyBufferException.h>
#include <core/io/private/HeapIntBuffer.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        const glong IntBuffer::ARRAY_BASE_OFFSET = Unsafe::ARRAY_INT_BASE_OFFSET;

        IntBuffer::IntBuffer(gint mark, gint pos, gint lim, gint cap, IntArray &hb, gint offset) :
                Buffer(mark, pos, lim, cap), hb(&hb), offset(offset), isReadOnly(false) {}

        IntBuffer::IntBuffer(gint mark, gint pos, gint lim, gint cap) :
                Buffer(mark, pos, lim, cap), hb(null), offset(0), isReadOnly(false) {}

        IntBuffer::IntBuffer(IntArray &hb, glong addr, gint cap) :
                Buffer(addr, cap), hb(&hb), offset(0), isReadOnly(false) {}

        Object &IntBuffer::base() const {
            if (hb == null)
                return null;
            else
                return (Object &) *hb;
        }

        IntBuffer &IntBuffer::allocate(gint capacity) {
            if (capacity < 0)
                IllegalArgumentException("capacity < 0: (" + String::valueOf(capacity) + " < 0)")
                        .throws(__trace("core.io.IntBuffer"));
            try {
                return Unsafe::allocateInstance<HeapIntBuffer>(capacity, capacity);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &IntBuffer::wrap(IntArray &array, gint offset, gint length) {
            try {
                return Unsafe::allocateInstance<HeapIntBuffer>(array, offset, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &IntBuffer::wrap(IntArray &array) {
            try {
                return wrap(array, 0, array.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &IntBuffer::get(IntArray &dst, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(off, length, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferUnderflowException().throws(__trace("core.io.IntBuffer"));

            try {
                getArray(pos, dst, off, length);
                setPosition(pos + off);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
            return *this;
        }

        IntBuffer &IntBuffer::get(IntArray &dst) {
            try {
                return get(dst, 0, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &IntBuffer::get(gint index, IntArray &dst, gint offset, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, dst.length());
                getArray(index, dst, offset, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
            return (IntBuffer &) *this;
        }

        IntBuffer &IntBuffer::get(gint index, IntArray &dst) const {
            try {
                return get(index, dst, 0, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &IntBuffer::getArray(gint index, IntArray &dst, gint off, gint length) const {
            try {
                if (((glong) length << 2) > 6) {
                    glong const bufAddr = address + ((glong) index << 2);
                    glong const dstOffset = ARRAY_BASE_OFFSET + ((glong) off << 2);
                    glong const len = (glong) length << 2;

                    if (order() != ByteOrder::NATIVE_ENDIAN)
                        Unsafe::copySwapMemory(base(), bufAddr, dst, dstOffset, len, Integer::BYTES);
                    else
                        Unsafe::copyMemory(base(), bufAddr, dst, dstOffset, len);
                } else {
                    gint const end = off + length;
                    for (gint i = off, j = index; i < end; i++, j++) {
                        dst[i] = get(j);
                    }
                }
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
            return (IntBuffer &) *this;
        }

        IntBuffer &IntBuffer::put(IntBuffer &src) {
            if (this == &src)
                IllegalArgumentException("The source buffer is this buffer").throws(__trace("core.io.IntBuffer"));
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.IntBuffer"));

            gint const srcPos = src.position();
            gint const srcLim = src.limit();
            gint const srcRem = (srcPos <= srcLim ? srcLim - srcPos : 0);
            gint const pos = position();
            gint const lim = limit();
            gint const rem = (pos <= lim ? lim - pos : 0);

            if (srcRem > rem)
                BufferOverflowException().throws(__trace("core.io.IntBuffer"));

            try {
                putBuffer(pos, src, srcPos, srcRem);

                setPosition(pos + srcRem);
                src.setPosition(srcPos + srcRem);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
            return *this;
        }

        IntBuffer &IntBuffer::put(gint index, IntBuffer &src, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, src.limit());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.IntBuffer"));

            try {
                putBuffer(index, src, off, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
            return *this;
        }

        void IntBuffer::putBuffer(gint pos, IntBuffer &src, gint srcPos, gint n) {
            Object srcBase = src.base();
            CORE_ASSERT(src.isDirect(), "core.io.IntBuffer");
            Object &base = this->base();
            CORE_ASSERT(isDirect(), "core.io.IntBuffer");

            glong const srcAddr = src.address + ((glong) srcPos << 2);
            glong const addr = address + ((glong) pos << 2);
            glong const len = (glong) n << 2;

            if (order() != src.order())
                Unsafe::copySwapMemory(srcBase, srcAddr, base, addr, len, Integer::BYTES);
            else
                Unsafe::copyMemory(srcBase, srcAddr, base, addr, len);
        }

        IntBuffer &IntBuffer::put(const IntArray &src, gint off, gint length) {
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.IntBuffer"));
            try {
                Preconditions::checkIndexFromSize(off, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws(__trace("core.io.IntBuffer"));

            try {
                putArray(pos, src, off, length);

                setPosition(pos + length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
            return *this;
        }

        IntBuffer &IntBuffer::put(const IntArray &src) {
            try {
                return put(src, 0, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &IntBuffer::put(gint index, const IntArray &src, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.IntBuffer"));
            try {
                putArray(index, src, off, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
            return *this;
        }

        IntBuffer &IntBuffer::put(gint index, const IntArray &src) {
            try {
                return put(index, src, 0, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &IntBuffer::putArray(gint index, const IntArray &src, gint offset, gint length) {
            if (((glong) length << 2) > 6) {
                glong const bufAddr = address + ((glong) index << 2);
                glong const srcOffset = ARRAY_BASE_OFFSET + ((glong) offset << 2);
                glong const len = (glong) length << 2;
                if (order() != ByteOrder::NATIVE_ENDIAN)
                    Unsafe::copySwapMemory((Object &) src, srcOffset, base(), bufAddr, len, Integer::BYTES);
                else
                    Unsafe::copyMemory(src, srcOffset, base(), bufAddr, len);
            } else {
                gint const end = offset + length;
                for (gint i = offset, j = index; i < end; i++, j++)
                    put(j, src[i]);
            }
            return *this;
        }

        gbool IntBuffer::hasArray() const {
            return hb != null && !isReadOnly;
        }

        IntArray &IntBuffer::array() const {
            if (hb == null)
                UnsupportedOperationException().throws(__trace("core.io.IntBuffer"));
            if (isReadOnly)
                ReadOnlyBufferException().throws(__trace("core.io.IntBuffer"));
            return *hb;
        }

        gint IntBuffer::arrayOffset() const {
            return offset;
        }

        IntBuffer &IntBuffer::setPosition(gint newPosition) {
            try {
                Buffer::setPosition(newPosition);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &IntBuffer::setLimit(gint newLimit) {
            try {
                Buffer::setLimit(newLimit);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &IntBuffer::mark() {
            try {
                Buffer::mark();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &IntBuffer::reset() {
            try {
                Buffer::reset();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &IntBuffer::clear() {
            try {
                Buffer::clear();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &IntBuffer::flip() {
            try {
                Buffer::flip();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &IntBuffer::rewind() {
            try {
                Buffer::rewind();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        gint IntBuffer::hash() const {
            gint h = 1;
            gint const p = position();
            for (gint i = limit() - 1; i >= p; i--)
                h = 31 * h + (gint) get(i);

            return h;
        }

        gbool IntBuffer::equals(const Object &obj) const {
            if (this == &obj)
                return true;
            if (!(Class<IntBuffer>::hasInstance(obj)))
                return false;
            IntBuffer const &that = (IntBuffer const &) obj;
            gint const thisPos = position();
            gint const thisRem = limit() - thisPos;
            gint const thatPos = that.position();
            gint const thatRem = that.limit() - thatPos;
            if (thisRem < 0 || thisRem != thatRem)
                return false;
            return mismatch(*this, thisPos, that, thatPos, thisRem) < 0;
        }

        int IntBuffer::mismatch(const IntBuffer &a, gint aOff, const IntBuffer &b, gint bOff, gint length) {
            int i = 0;
            if (length > 3 && a.order() == b.order()) {
                if (a.get(aOff) != b.get(bOff))
                    return 0;
                i = ArraysSupport::vectorizedMismatch(
                        a.base(), a.address + (aOff << ArraysSupport::LOG2_ARRAY_INT_INDEX_SCALE),
                        b.base(), b.address + (bOff << ArraysSupport::LOG2_ARRAY_INT_INDEX_SCALE),
                        length,
                        ArraysSupport::LOG2_ARRAY_INT_INDEX_SCALE);
                if (i >= 0) return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a.get(aOff + i) != b.get(bOff + i))
                    return i;
            }
            return -1;
        }

        gint IntBuffer::compareTo(const IntBuffer &that) const {
            gint const thisPos = position();
            gint const thisRem = limit() - thisPos;
            gint const thatPos = that.position();
            gint const thatRem = that.limit() - thatPos;
            gint const length = Math::min(thisRem, thatRem);
            if (length < 0)
                return -1;
            gint const i = mismatch(*this, thisPos, that, thatPos, length);
            if (i >= 0) {
                return Integer::compare(get(thisPos + i), that.get(thatPos + i));
            }
            return thisRem - thatRem;
        }

        gint IntBuffer::mismatch(const IntBuffer &that) const {
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

        String IntBuffer::toString() const {
            return classname()
                   + "[pos=" + String::valueOf(position())
                   + " lim=" + String::valueOf(limit())
                   + " cap=" + String::valueOf(capacity())
                   + "]";
        }

    } // core
} // io
