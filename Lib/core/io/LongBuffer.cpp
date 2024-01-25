//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "LongBuffer.h"
#include <core/private/Unsafe.h>
#include <core/IllegalArgumentException.h>
#include <core/native/LongArray.h>
#include <core/AssertionError.h>
#include <core/util/Preconditions.h>
#include <core/Math.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/private/ReadOnlyBufferException.h>
#include <core/io/private/HeapLongBuffer.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        const glong LongBuffer::ARRAY_BASE_OFFSET = Unsafe::ARRAY_LONG_BASE_OFFSET;

        LongBuffer::LongBuffer(gint mark, gint pos, gint lim, gint cap, LongArray &hb, gint offset) :
                Buffer(mark, pos, lim, cap), hb(&hb), offset(offset), isReadOnly(false) {}

        LongBuffer::LongBuffer(gint mark, gint pos, gint lim, gint cap) :
                Buffer(mark, pos, lim, cap), hb(null), offset(0), isReadOnly(false) {}

        LongBuffer::LongBuffer(LongArray &hb, glong addr, gint cap) :
                Buffer(addr, cap), hb(&hb), offset(0), isReadOnly(false) {}

        Object &LongBuffer::base() const {
            if (hb == null)
                return null;
            else
                return (Object &) *hb;
        }

        LongBuffer &LongBuffer::allocate(gint capacity) {
            if (capacity < 0)
                IllegalArgumentException("capacity < 0: (" + String::valueOf(capacity) + " < 0)")
                        .throws(__trace("core.io.LongBuffer"));
            try {
                return Unsafe::allocateInstance<HeapLongBuffer>(capacity, capacity);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &LongBuffer::wrap(LongArray &array, gint offset, gint length) {
            try {
                return Unsafe::allocateInstance<HeapLongBuffer>(array, offset, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &LongBuffer::wrap(LongArray &array) {
            try {
                return wrap(array, 0, array.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &LongBuffer::get(LongArray &dst, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(off, length, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferUnderflowException().throws(__trace("core.io.LongBuffer"));

            try {
                getArray(pos, dst, off, length);
                setPosition(pos + off);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
            return *this;
        }

        LongBuffer &LongBuffer::get(LongArray &dst) {
            try {
                return get(dst, 0, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &LongBuffer::get(gint index, LongArray &dst, gint offset, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, dst.length());
                getArray(index, dst, offset, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
            return (LongBuffer &) *this;
        }

        LongBuffer &LongBuffer::get(gint index, LongArray &dst) const {
            try {
                return get(index, dst, 0, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &LongBuffer::getArray(gint index, LongArray &dst, gint off, gint length) const {
            try {
                if (((glong) length << 3) > 6) {
                    glong const bufAddr = address + ((glong) index << 3);
                    glong const dstOffset = ARRAY_BASE_OFFSET + ((glong) off << 3);
                    glong const len = (glong) length << 3;

                    if (order() != ByteOrder::NATIVE_ENDIAN)
                        Unsafe::copySwapMemory(base(), bufAddr, dst, dstOffset, len, Long::BYTES);
                    else
                        Unsafe::copyMemory(base(), bufAddr, dst, dstOffset, len);
                } else {
                    gint const end = off + length;
                    for (gint i = off, j = index; i < end; i++, j++) {
                        dst[i] = get(j);
                    }
                }
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
            return (LongBuffer &) *this;
        }

        LongBuffer &LongBuffer::put(LongBuffer &src) {
            if (this == &src)
                IllegalArgumentException("The source buffer is this buffer").throws(__trace("core.io.LongBuffer"));
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.LongBuffer"));

            gint const srcPos = src.position();
            gint const srcLim = src.limit();
            gint const srcRem = (srcPos <= srcLim ? srcLim - srcPos : 0);
            gint const pos = position();
            gint const lim = limit();
            gint const rem = (pos <= lim ? lim - pos : 0);

            if (srcRem > rem)
                BufferOverflowException().throws(__trace("core.io.LongBuffer"));

            try {
                putBuffer(pos, src, srcPos, srcRem);

                setPosition(pos + srcRem);
                src.setPosition(srcPos + srcRem);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
            return *this;
        }

        LongBuffer &LongBuffer::put(gint index, LongBuffer &src, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, src.limit());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.LongBuffer"));

            try {
                putBuffer(index, src, off, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
            return *this;
        }

        void LongBuffer::putBuffer(gint pos, LongBuffer &src, gint srcPos, gint n) {
            Object srcBase = src.base();
            CORE_ASSERT(src.isDirect(), "core.io.LongBuffer");
            Object &base = this->base();
            CORE_ASSERT(isDirect(), "core.io.LongBuffer");

            glong const srcAddr = src.address + ((glong) srcPos << 3);
            glong const addr = address + ((glong) pos << 3);
            glong const len = (glong) n << 3;

            if (order() != src.order())
                Unsafe::copySwapMemory(srcBase, srcAddr, base, addr, len, Long::BYTES);
            else
                Unsafe::copyMemory(srcBase, srcAddr, base, addr, len);
        }

        LongBuffer &LongBuffer::put(const LongArray &src, gint off, gint length) {
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.LongBuffer"));
            try {
                Preconditions::checkIndexFromSize(off, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws(__trace("core.io.LongBuffer"));

            try {
                putArray(pos, src, off, length);

                setPosition(pos + length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
            return *this;
        }

        LongBuffer &LongBuffer::put(const LongArray &src) {
            try {
                return put(src, 0, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &LongBuffer::put(gint index, const LongArray &src, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.LongBuffer"));
            try {
                putArray(index, src, off, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
            return *this;
        }

        LongBuffer &LongBuffer::put(gint index, const LongArray &src) {
            try {
                return put(index, src, 0, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &LongBuffer::putArray(gint index, const LongArray &src, gint off, gint length) {
            if (((glong) length << 3) > 6) {
                glong const bufAddr = address + ((glong) index << 3);
                glong const srcOffset = ARRAY_BASE_OFFSET + ((glong) off << 3);
                glong const len = (glong) length << 3;
                if (order() != ByteOrder::NATIVE_ENDIAN)
                    Unsafe::copySwapMemory((Object &) src, srcOffset, base(), bufAddr, len, Long::BYTES);
                else
                    Unsafe::copyMemory(src, srcOffset, base(), bufAddr, len);
            } else {
                gint const end = off + length;
                for (gint i = off, j = index; i < end; i++, j++)
                    put(j, src[i]);
            }
            return *this;
        }

        gbool LongBuffer::hasArray() const {
            return hb != null && !isReadOnly;
        }

        LongArray &LongBuffer::array() const {
            if (hb == null)
                UnsupportedOperationException().throws(__trace("core.io.LongBuffer"));
            if (isReadOnly)
                ReadOnlyBufferException().throws(__trace("core.io.LongBuffer"));
            return *hb;
        }

        gint LongBuffer::arrayOffset() const {
            return offset;
        }

        LongBuffer &LongBuffer::setPosition(gint newPosition) {
            try {
                Buffer::setPosition(newPosition);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &LongBuffer::setLimit(gint newLimit) {
            try {
                Buffer::setLimit(newLimit);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &LongBuffer::mark() {
            try {
                Buffer::mark();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &LongBuffer::reset() {
            try {
                Buffer::reset();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &LongBuffer::clear() {
            try {
                Buffer::clear();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &LongBuffer::flip() {
            try {
                Buffer::flip();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &LongBuffer::rewind() {
            try {
                Buffer::rewind();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        gint LongBuffer::hash() const {
            gint h = 1;
            gint const p = position();
            for (gint i = limit() - 1; i >= p; i--)
                h = 31 * h + (gint) get(i);

            return h;
        }

        gbool LongBuffer::equals(const Object &obj) const {
            if (this == &obj)
                return true;
            if (!(Class<LongBuffer>::hasInstance(obj)))
                return false;
            LongBuffer const &that = (LongBuffer const &) obj;
            gint const thisPos = position();
            gint const thisRem = limit() - thisPos;
            gint const thatPos = that.position();
            gint const thatRem = that.limit() - thatPos;
            if (thisRem < 0 || thisRem != thatRem)
                return false;
            return mismatch(*this, thisPos, that, thatPos, thisRem) < 0;
        }

        int LongBuffer::mismatch(const LongBuffer &a, gint aOff, const LongBuffer &b, gint bOff, gint length) {
            int i = 0;
            if (length > 3 && a.order() == b.order()) {
                if (a.get(aOff) != b.get(bOff))
                    return 0;
                i = ArraysSupport::vectorizedMismatch(
                        a.base(), a.address + (aOff << ArraysSupport::LOG2_ARRAY_LONG_INDEX_SCALE),
                        b.base(), b.address + (bOff << ArraysSupport::LOG2_ARRAY_LONG_INDEX_SCALE),
                        length,
                        ArraysSupport::LOG2_ARRAY_LONG_INDEX_SCALE);
                if (i >= 0) return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a.get(aOff + i) != b.get(bOff + i))
                    return i;
            }
            return -1;
        }

        gint LongBuffer::compareTo(const LongBuffer &that) const {
            gint const thisPos = position();
            gint const thisRem = limit() - thisPos;
            gint const thatPos = that.position();
            gint const thatRem = that.limit() - thatPos;
            gint const length = Math::min(thisRem, thatRem);
            if (length < 0)
                return -1;
            gint const i = mismatch(*this, thisPos, that, thatPos, length);
            if (i >= 0) {
                return Long::compare(get(thisPos + i), that.get(thatPos + i));
            }
            return thisRem - thatRem;
        }

        gint LongBuffer::mismatch(const LongBuffer &that) const {
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

        String LongBuffer::toString() const {
            return classname()
                   + "[pos=" + String::valueOf(position())
                   + " lim=" + String::valueOf(limit())
                   + " cap=" + String::valueOf(capacity())
                   + "]";
        }

    } // core
} // io
