//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "DoubleBuffer.h"
#include <core/private/Unsafe.h>
#include <core/IllegalArgumentException.h>
#include <core/native/DoubleArray.h>
#include <core/AssertionError.h>
#include <core/util/Preconditions.h>
#include <core/Math.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/private/ReadOnlyBufferException.h>
#include <core/io/private/HeapDoubleBuffer.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        const glong DoubleBuffer::ARRAY_BASE_OFFSET = Unsafe::ARRAY_DOUBLE_BASE_OFFSET;

        DoubleBuffer::DoubleBuffer(gint mark, gint pos, gint lim, gint cap, DoubleArray &hb, gint offset) :
                Buffer(mark, pos, lim, cap), hb(&hb), offset(offset), isReadOnly(false) {}

        DoubleBuffer::DoubleBuffer(gint mark, gint pos, gint lim, gint cap) :
                Buffer(mark, pos, lim, cap), hb(null), offset(0), isReadOnly(false) {}

        DoubleBuffer::DoubleBuffer(DoubleArray &hb, glong addr, gint cap) :
                Buffer(addr, cap), hb(&hb), offset(0), isReadOnly(false) {}

        Object &DoubleBuffer::base() const {
            if (hb == null)
                return null;
            else
                return (Object &) *hb;
        }

        DoubleBuffer &DoubleBuffer::allocate(gint capacity) {
            if (capacity < 0)
                IllegalArgumentException("capacity < 0: (" + String::valueOf(capacity) + " < 0)")
                        .throws(__trace("core.io.DoubleBuffer"));
            try {
                return Unsafe::allocateInstance<HeapDoubleBuffer>(capacity, capacity);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.DoubleBuffer"));
            }
        }

        DoubleBuffer &DoubleBuffer::wrap(DoubleArray &array, gint offset, gint length) {
            try {
                return Unsafe::allocateInstance<HeapDoubleBuffer>(array, offset, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.DoubleBuffer"));
            }
        }

        DoubleBuffer &DoubleBuffer::wrap(DoubleArray &array) {
            try {
                return wrap(array, 0, array.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.DoubleBuffer"));
            }
        }

        DoubleBuffer &DoubleBuffer::get(DoubleArray &dst, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(off, length, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferUnderflowException().throws(__ctrace());

            try {
                getArray(pos, dst, off, length);
                setPosition(pos + off);
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
            return *this;
        }

        DoubleBuffer &DoubleBuffer::get(DoubleArray &dst) {
            try {
                return get(dst, 0, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
        }

        DoubleBuffer &DoubleBuffer::get(gint index, DoubleArray &dst, gint off, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, dst.length());
                getArray(index, dst, off, length);
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
            return (DoubleBuffer &) *this;
        }

        DoubleBuffer &DoubleBuffer::get(gint index, DoubleArray &dst) const {
            try {
                return get(index, dst, 0, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
        }

        DoubleBuffer &DoubleBuffer::getArray(gint index, DoubleArray &dst, gint off, gint length) const {
            try {
                if (((glong) length << 1) > 6) {
                    glong const bufAddr = address + ((glong) index << 3);
                    glong const dstOffset = ARRAY_BASE_OFFSET + ((glong) off << 3);
                    glong const len = (glong) length << 1;

                    if (order() != ByteOrder::NATIVE_ENDIAN)
                        Unsafe::copySwapMemory(base(), bufAddr, dst, dstOffset, len, Double::BYTES);
                    else
                        Unsafe::copyMemory(base(), bufAddr, dst, dstOffset, len);
                } else {
                    gint const end = off + length;
                    for (gint i = off, j = index; i < end; i++, j++) {
                        dst[i] = get(j);
                    }
                }
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
            return (DoubleBuffer &) *this;
        }

        DoubleBuffer &DoubleBuffer::put(DoubleBuffer &src) {
            if (this == &src)
                IllegalArgumentException("The source buffer is this buffer").throws(__ctrace());
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__ctrace());

            gint const srcPos = src.position();
            gint const srcLim = src.limit();
            gint const srcRem = (srcPos <= srcLim ? srcLim - srcPos : 0);
            gint const pos = position();
            gint const lim = limit();
            gint const rem = (pos <= lim ? lim - pos : 0);

            if (srcRem > rem)
                BufferOverflowException().throws(__ctrace());

            try {
                putBuffer(pos, src, srcPos, srcRem);

                setPosition(pos + srcRem);
                src.setPosition(srcPos + srcRem);
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
            return *this;
        }

        DoubleBuffer &DoubleBuffer::put(gint index, DoubleBuffer &src, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, src.limit());
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__ctrace());

            try {
                putBuffer(index, src, off, length);
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
            return *this;
        }

        void DoubleBuffer::putBuffer(gint pos, DoubleBuffer &src, gint srcPos, gint n) {
            Object srcBase = src.base();
            CORE_ASSERT(null != srcBase || src.isDirect(), __ctrace())
            Object &base = this->base();
            CORE_ASSERT(null != base || isDirect(), __ctrace())

            glong const srcAddr = src.address + ((glong) srcPos << 3);
            glong const addr = address + ((glong) pos << 3);
            glong const len = (glong) n << 3;

            if (order() != src.order())
                Unsafe::copySwapMemory(srcBase, srcAddr, base, addr, len, Double::BYTES);
            else
                Unsafe::copyMemory(srcBase, srcAddr, base, addr, len);
        }

        DoubleBuffer &DoubleBuffer::put(const DoubleArray &src, gint off, gint length) {
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__ctrace());
            try {
                Preconditions::checkIndexFromSize(off, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws(__ctrace());

            try {
                putArray(pos, src, off, length);

                setPosition(pos + length);
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
            return *this;
        }

        DoubleBuffer &DoubleBuffer::put(const DoubleArray &src) {
            try {
                return put(src, 0, src.length());
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
        }

        DoubleBuffer &DoubleBuffer::put(gint index, const DoubleArray &src, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__ctrace());
            try {
                putArray(index, src, off, length);
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
            return *this;
        }

        DoubleBuffer &DoubleBuffer::put(gint index, const DoubleArray &src) {
            try {
                return put(index, src, 0, src.length());
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
        }

        DoubleBuffer &DoubleBuffer::putArray(gint index, const DoubleArray &src, gint off, gint length) {
            if (((glong) length << 3) > 6) {
                glong const bufAddr = address + ((glong) index << 3);
                glong const srcOffset = ARRAY_BASE_OFFSET + ((glong) off << 3);
                glong const len = (glong) length << 3;
                if (order() != ByteOrder::NATIVE_ENDIAN)
                    Unsafe::copySwapMemory((Object &) src, srcOffset, base(), bufAddr, len, Double::BYTES);
                else
                    Unsafe::copyMemory(src, srcOffset, base(), bufAddr, len);
            } else {
                gint const end = off + length;
                for (gint i = off, j = index; i < end; i++, j++)
                    put(j, src[i]);
            }
            return *this;
        }

        gbool DoubleBuffer::hasArray() const {
            return hb != null && !isReadOnly;
        }

        DoubleArray &DoubleBuffer::array() const {
            if (hb == null)
                UnsupportedOperationException().throws(__ctrace());
            if (isReadOnly)
                ReadOnlyBufferException().throws(__ctrace());
            return *hb;
        }

        gint DoubleBuffer::arrayOffset() const {
            return offset;
        }

        DoubleBuffer &DoubleBuffer::setPosition(gint newPosition) {
            try {
                Buffer::setPosition(newPosition);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
        }

        DoubleBuffer &DoubleBuffer::setLimit(gint newLimit) {
            try {
                Buffer::setLimit(newLimit);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
        }

        DoubleBuffer &DoubleBuffer::mark() {
            try {
                Buffer::mark();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
        }

        DoubleBuffer &DoubleBuffer::reset() {
            try {
                Buffer::reset();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
        }

        DoubleBuffer &DoubleBuffer::clear() {
            try {
                Buffer::clear();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
        }

        DoubleBuffer &DoubleBuffer::flip() {
            try {
                Buffer::flip();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
        }

        DoubleBuffer &DoubleBuffer::rewind() {
            try {
                Buffer::rewind();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__ctrace());
            }
        }

        gint DoubleBuffer::hash() const {
            gint h = 1;
            gint const p = position();
            for (gint i = limit() - 1; i >= p; i--)
                h = 31 * h + (gint) get(i);

            return h;
        }

        gbool DoubleBuffer::equals(const Object &obj) const {
            if (this == &obj)
                return true;
            if (!(Class<DoubleBuffer>::hasInstance(obj)))
                return false;
            DoubleBuffer const &that = (DoubleBuffer const &) obj;
            gint const thisPos = position();
            gint const thisRem = limit() - thisPos;
            gint const thatPos = that.position();
            gint const thatRem = that.limit() - thatPos;
            if (thisRem < 0 || thisRem != thatRem)
                return false;
            return mismatch(*this, thisPos, that, thatPos, thisRem) < 0;
        }

        int DoubleBuffer::mismatch(const DoubleBuffer &a, int aOff, const DoubleBuffer &b, int bOff, int length) {
            int i = 0;
            if (length > 0 && a.order() == b.order()) {
                if (Double::toLongBits(a.get(aOff)) == Double::toLongBits(b.get(bOff))) {
                    i = ArraysSupport::vectorizedMismatch(
                            a.base(), a.address + (aOff << ArraysSupport::LOG2_ARRAY_DOUBLE_INDEX_SCALE),
                            b.base(), b.address + (bOff << ArraysSupport::LOG2_ARRAY_DOUBLE_INDEX_SCALE),
                            length,
                            ArraysSupport::LOG2_ARRAY_DOUBLE_INDEX_SCALE);
                }
                // Mismatched
                if (i >= 0) {
                    // Check if mismatch is not associated with two NaN values; and
                    // is not associated with +0 and -0
                    gdouble const av = a.get(aOff + i);
                    gdouble const bv = b.get(bOff + i);
                    if (av != bv && (!Double::isNaN(av) || !Double::isNaN(bv)))
                        return i;

                    // Fall back to slow mechanism
                    // ISSUE: Consider looping over vectorizedMismatch adjusting ranges
                    // However, requires that returned value be relative to input ranges
                    i++;
                }
                    // Matched
                else {
                    return -1;
                }
            }
            for (; i < length; i++) {
                gdouble const av = a.get(aOff + i);
                gdouble const bv = b.get(bOff + i);
                if (av != bv && (!Double::isNaN(av) || !Double::isNaN(bv)))
                    return i;
            }
            return -1;
        }

        gint DoubleBuffer::compareTo(const DoubleBuffer &that) const {
            gint const thisPos = position();
            gint const thisRem = limit() - thisPos;
            gint const thatPos = that.position();
            gint const thatRem = that.limit() - thatPos;
            gint const length = Math::min(thisRem, thatRem);
            if (length < 0)
                return -1;
            gint const i = mismatch(*this, thisPos, that, thatPos, length);
            if (i >= 0) {
                return Double::compare(get(thisPos + i), that.get(thatPos + i));
            }
            return thisRem - thatRem;
        }

        gint DoubleBuffer::mismatch(const DoubleBuffer &that) const {
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

        String DoubleBuffer::toString() const {
            return classname()
                   + "[pos=" + String::valueOf(position())
                   + " lim=" + String::valueOf(limit())
                   + " cap=" + String::valueOf(capacity())
                   + "]";
        }
    } // core
} // io
