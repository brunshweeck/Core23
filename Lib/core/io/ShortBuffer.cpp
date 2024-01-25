//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "ShortBuffer.h"
#include <core/private/Unsafe.h>
#include <core/IllegalArgumentException.h>
#include <core/native/ShortArray.h>
#include <core/AssertionError.h>
#include <core/util/Preconditions.h>
#include <core/Math.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/private/ReadOnlyBufferException.h>
#include <core/io/private/HeapShortBuffer.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        const glong ShortBuffer::ARRAY_BASE_OFFSET = Unsafe::ARRAY_SHORT_BASE_OFFSET;

        ShortBuffer::ShortBuffer(gint mark, gint pos, gint lim, gint cap, ShortArray &hb, gint offset) :
                Buffer(mark, pos, lim, cap), hb(&hb), offset(offset), isReadOnly(false) {}

        ShortBuffer::ShortBuffer(gint mark, gint pos, gint lim, gint cap) :
                Buffer(mark, pos, lim, cap), hb(null), offset(0), isReadOnly(false) {}

        ShortBuffer::ShortBuffer(ShortArray &hb, glong addr, gint cap) :
                Buffer(addr, cap), hb(&hb), offset(0), isReadOnly(false) {}

        Object &ShortBuffer::base() const {
            if (hb == null)
                return null;
            else
                return (Object &) *hb;
        }

        ShortBuffer &ShortBuffer::allocate(gint capacity) {
            if (capacity < 0)
                IllegalArgumentException("capacity < 0: (" + String::valueOf(capacity) + " < 0)")
                        .throws(__trace("core.io.ShortBuffer"));
            try {
                return Unsafe::allocateInstance<HeapShortBuffer>(capacity, capacity);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &ShortBuffer::wrap(ShortArray &array, gint offset, gint length) {
            try {
                return Unsafe::allocateInstance<HeapShortBuffer>(array, offset, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &ShortBuffer::wrap(ShortArray &array) {
            try {
                return wrap(array, 0, array.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &ShortBuffer::get(ShortArray &dst, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(off, length, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferUnderflowException().throws(__trace("core.io.ShortBuffer"));

            try {
                getArray(pos, dst, off, length);
                setPosition(pos + off);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
            return *this;
        }

        ShortBuffer &ShortBuffer::get(ShortArray &dst) {
            try {
                return get(dst, 0, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &ShortBuffer::get(gint index, ShortArray &dst, gint off, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, dst.length());
                getArray(index, dst, off, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
            return (ShortBuffer &) *this;
        }

        ShortBuffer &ShortBuffer::get(gint index, ShortArray &dst) const {
            try {
                return get(index, dst, 0, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &ShortBuffer::getArray(gint index, ShortArray &dst, gint off, gint length) const {
            try {
                if (((glong) length << 1) > 6) {
                    glong const bufAddr = address + ((glong) index << 1);
                    glong const dstOffset = ARRAY_BASE_OFFSET + ((glong) off << 1);
                    glong const len = (glong) length << 1;

                    if (order() != ByteOrder::NATIVE_ENDIAN)
                        Unsafe::copySwapMemory(base(), bufAddr, dst, dstOffset, len, Short::BYTES);
                    else
                        Unsafe::copyMemory(base(), bufAddr, dst, dstOffset, len);
                } else {
                    gint const end = off + length;
                    for (gint i = off, j = index; i < end; i++, j++) {
                        dst[i] = get(j);
                    }
                }
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
            return (ShortBuffer &) *this;
        }

        ShortBuffer &ShortBuffer::put(ShortBuffer &src) {
            if (this == &src)
                IllegalArgumentException("The source buffer is this buffer").throws(__trace("core.io.ShortBuffer"));
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.ShortBuffer"));

            gint const srcPos = src.position();
            gint const srcLim = src.limit();
            gint const srcRem = (srcPos <= srcLim ? srcLim - srcPos : 0);
            gint const pos = position();
            gint const lim = limit();
            gint const rem = (pos <= lim ? lim - pos : 0);

            if (srcRem > rem)
                BufferOverflowException().throws(__trace("core.io.ShortBuffer"));

            try {
                putBuffer(pos, src, srcPos, srcRem);

                setPosition(pos + srcRem);
                src.setPosition(srcPos + srcRem);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
            return *this;
        }

        ShortBuffer &ShortBuffer::put(gint index, ShortBuffer &src, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, src.limit());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.ShortBuffer"));

            try {
                putBuffer(index, src, off, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
            return *this;
        }

        void ShortBuffer::putBuffer(gint pos, ShortBuffer &src, gint srcPos, gint n) {
            Object srcBase = src.base();
            CORE_ASSERT(src.isDirect(), "core.io.ShortBuffer");
            Object &base = this->base();
            CORE_ASSERT(isDirect(), "core.io.ShortBuffer");

            glong const srcAddr = src.address + ((glong) srcPos << 1);
            glong const addr = address + ((glong) pos << 1);
            glong const len = (glong) n << 1;

            if (order() != src.order())
                Unsafe::copySwapMemory(srcBase, srcAddr, base, addr, len, Short::BYTES);
            else
                Unsafe::copyMemory(srcBase, srcAddr, base, addr, len);
        }

        ShortBuffer &ShortBuffer::put(const ShortArray &src, gint off, gint length) {
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.ShortBuffer"));
            try {
                Preconditions::checkIndexFromSize(off, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws(__trace("core.io.ShortBuffer"));

            try {
                putArray(pos, src, off, length);

                setPosition(pos + length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
            return *this;
        }

        ShortBuffer &ShortBuffer::put(const ShortArray &src) {
            try {
                return put(src, 0, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &ShortBuffer::put(gint index, const ShortArray &src, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.ShortBuffer"));
            try {
                putArray(index, src, off, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
            return *this;
        }

        ShortBuffer &ShortBuffer::put(gint index, const ShortArray &src) {
            try {
                return put(index, src, 0, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &ShortBuffer::putArray(gint index, const ShortArray &src, gint off, gint length) {
            if (((glong) length << 1) > 6) {
                glong const bufAddr = address + ((glong) index << 1);
                glong const srcOffset = ARRAY_BASE_OFFSET + ((glong) off << 1);
                glong const len = (glong) length << 1;
                if (order() != ByteOrder::NATIVE_ENDIAN)
                    Unsafe::copySwapMemory((Object &) src, srcOffset, base(), bufAddr, len, Short::BYTES);
                else
                    Unsafe::copyMemory(src, srcOffset, base(), bufAddr, len);
            } else {
                gint const end = off + length;
                for (gint i = off, j = index; i < end; i++, j++)
                    put(j, src[i]);
            }
            return *this;
        }

        gbool ShortBuffer::hasArray() const {
            return hb != null && !isReadOnly;
        }

        ShortArray &ShortBuffer::array() const {
            if (hb == null)
                UnsupportedOperationException().throws(__trace("core.io.ShortBuffer"));
            if (isReadOnly)
                ReadOnlyBufferException().throws(__trace("core.io.ShortBuffer"));
            return *hb;
        }

        gint ShortBuffer::arrayOffset() const {
            return offset;
        }

        ShortBuffer &ShortBuffer::setPosition(gint newPosition) {
            try {
                Buffer::setPosition(newPosition);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &ShortBuffer::setLimit(gint newLimit) {
            try {
                Buffer::setLimit(newLimit);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &ShortBuffer::mark() {
            try {
                Buffer::mark();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &ShortBuffer::reset() {
            try {
                Buffer::reset();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &ShortBuffer::clear() {
            try {
                Buffer::clear();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &ShortBuffer::flip() {
            try {
                Buffer::flip();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &ShortBuffer::rewind() {
            try {
                Buffer::rewind();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        gint ShortBuffer::hash() const {
            gint h = 1;
            gint const p = position();
            for (gint i = limit() - 1; i >= p; i--)
                h = 31 * h + (gint) get(i);

            return h;
        }

        gbool ShortBuffer::equals(const Object &obj) const {
            if (this == &obj)
                return true;
            if (!(Class<ShortBuffer>::hasInstance(obj)))
                return false;
            ShortBuffer const &that = (ShortBuffer const &) obj;
            gint const thisPos = position();
            gint const thisRem = limit() - thisPos;
            gint const thatPos = that.position();
            gint const thatRem = that.limit() - thatPos;
            if (thisRem < 0 || thisRem != thatRem)
                return false;
            return mismatch(*this, thisPos, that, thatPos, thisRem) < 0;
        }

        int ShortBuffer::mismatch(const ShortBuffer &a, gint aOff, const ShortBuffer &b, gint bOff, gint length) {
            int i = 0;
            if (length > 3 && a.order() == b.order()) {
                if (a.get(aOff) != b.get(bOff))
                    return 0;
                i = ArraysSupport::vectorizedMismatch(
                        a.base(), a.address + (aOff << ArraysSupport::LOG2_ARRAY_SHORT_INDEX_SCALE),
                        b.base(), b.address + (bOff << ArraysSupport::LOG2_ARRAY_SHORT_INDEX_SCALE),
                        length,
                        ArraysSupport::LOG2_ARRAY_SHORT_INDEX_SCALE);
                if (i >= 0) return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a.get(aOff + i) != b.get(bOff + i))
                    return i;
            }
            return -1;
        }

        gint ShortBuffer::compareTo(const ShortBuffer &that) const {
            gint const thisPos = position();
            gint const thisRem = limit() - thisPos;
            gint const thatPos = that.position();
            gint const thatRem = that.limit() - thatPos;
            gint const length = Math::min(thisRem, thatRem);
            if (length < 0)
                return -1;
            gint const i = mismatch(*this, thisPos, that, thatPos, length);
            if (i >= 0) {
                return Short::compare(get(thisPos + i), that.get(thatPos + i));
            }
            return thisRem - thatRem;
        }

        gint ShortBuffer::mismatch(const ShortBuffer &that) const {
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

        String ShortBuffer::toString() const {
            return classname()
                   + "[pos=" + String::valueOf(position())
                   + " lim=" + String::valueOf(limit())
                   + " cap=" + String::valueOf(capacity())
                   + "]";
        }

    } // core
} // io
