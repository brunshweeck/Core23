//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "FloatBuffer.h"
#include <core/private/Unsafe.h>
#include <core/IllegalArgumentException.h>
#include <core/native/FloatArray.h>
#include <core/AssertionError.h>
#include <core/util/Preconditions.h>
#include <core/Math.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/private/ReadOnlyBufferException.h>
#include <core/io/private/HeapFloatBuffer.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        const glong FloatBuffer::ARRAY_BASE_OFFSET = Unsafe::ARRAY_FLOAT_BASE_OFFSET;

        FloatBuffer::FloatBuffer(gint mark, gint pos, gint lim, gint cap, FloatArray &hb, gint offset) :
                Buffer(mark, pos, lim, cap), hb(&hb), offset(offset), isReadOnly(false) {}

        FloatBuffer::FloatBuffer(gint mark, gint pos, gint lim, gint cap) :
                Buffer(mark, pos, lim, cap), hb(null), offset(0), isReadOnly(false) {}

        FloatBuffer::FloatBuffer(FloatArray &hb, glong addr, gint cap) :
                Buffer(addr, cap), hb(&hb), offset(0), isReadOnly(false) {}

        Object &FloatBuffer::base() const {
            if (hb == null)
                return null;
            else
                return (Object &) *hb;
        }

        FloatBuffer &FloatBuffer::allocate(gint capacity) {
            if (capacity < 0)
                IllegalArgumentException("capacity < 0: (" + String::valueOf(capacity) + " < 0)")
                        .throws(__trace("core.io.FloatBuffer"));
            try {
                return Unsafe::allocateInstance<HeapFloatBuffer>(capacity, capacity);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &FloatBuffer::wrap(FloatArray &array, gint offset, gint length) {
            try {
                return Unsafe::allocateInstance<HeapFloatBuffer>(array, offset, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &FloatBuffer::wrap(FloatArray &array) {
            try {
                return wrap(array, 0, array.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &FloatBuffer::get(FloatArray &dst, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(off, length, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferUnderflowException().throws(__trace("core.io.FloatBuffer"));

            try {
                getArray(pos, dst, off, length);
                setPosition(pos + off);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
            return *this;
        }

        FloatBuffer &FloatBuffer::get(FloatArray &dst) {
            try {
                return get(dst, 0, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &FloatBuffer::get(gint index, FloatArray &dst, gint offset, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, dst.length());
                getArray(index, dst, offset, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
            return (FloatBuffer &) *this;
        }

        FloatBuffer &FloatBuffer::get(gint index, FloatArray &dst) const {
            try {
                return get(index, dst, 0, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &FloatBuffer::getArray(gint index, FloatArray &dst, gint off, gint length) const {
            try {
                if (((glong) length << 1) > 6) {
                    glong const bufAddr = address + ((glong) index << 2);
                    glong const dstOffset = ARRAY_BASE_OFFSET + ((glong) off << 2);
                    glong const len = (glong) length << 1;

                    if (order() != ByteOrder::NATIVE_ENDIAN)
                        Unsafe::copySwapMemory(base(), bufAddr, dst, dstOffset, len, Float::BYTES);
                    else
                        Unsafe::copyMemory(base(), bufAddr, dst, dstOffset, len);
                } else {
                    gint const end = off + length;
                    for (gint i = off, j = index; i < end; i++, j++) {
                        dst[i] = get(j);
                    }
                }
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
            return (FloatBuffer &) *this;
        }

        FloatBuffer &FloatBuffer::put(FloatBuffer &src) {
            if (this == &src)
                IllegalArgumentException("The source buffer is this buffer").throws(__trace("core.io.FloatBuffer"));
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.FloatBuffer"));

            gint const srcPos = src.position();
            gint const srcLim = src.limit();
            gint const srcRem = (srcPos <= srcLim ? srcLim - srcPos : 0);
            gint const pos = position();
            gint const lim = limit();
            gint const rem = (pos <= lim ? lim - pos : 0);

            if (srcRem > rem)
                BufferOverflowException().throws(__trace("core.io.FloatBuffer"));

            try {
                putBuffer(pos, src, srcPos, srcRem);

                setPosition(pos + srcRem);
                src.setPosition(srcPos + srcRem);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
            return *this;
        }

        FloatBuffer &FloatBuffer::put(gint index, FloatBuffer &src, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, src.limit());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.FloatBuffer"));

            try {
                putBuffer(index, src, off, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
            return *this;
        }

        void FloatBuffer::putBuffer(gint pos, FloatBuffer &src, gint srcPos, gint n) {
            Object srcBase = src.base();
            CORE_ASSERT(src.isDirect(), "core.io.FloatBuffer");
            Object &base = this->base();
            CORE_ASSERT(isDirect(), "core.io.FloatBuffer");

            glong const srcAddr = src.address + ((glong) srcPos << 2);
            glong const addr = address + ((glong) pos << 2);
            glong const len = (glong) n << 2;

            if (order() != src.order())
                Unsafe::copySwapMemory(srcBase, srcAddr, base, addr, len, Float::BYTES);
            else
                Unsafe::copyMemory(srcBase, srcAddr, base, addr, len);
        }

        FloatBuffer &FloatBuffer::put(const FloatArray &src, gint off, gint length) {
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.FloatBuffer"));
            try {
                Preconditions::checkIndexFromSize(off, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws(__trace("core.io.FloatBuffer"));

            try {
                putArray(pos, src, off, length);

                setPosition(pos + length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
            return *this;
        }

        FloatBuffer &FloatBuffer::put(const FloatArray &src) {
            try {
                return put(src, 0, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &FloatBuffer::put(gint index, const FloatArray &src, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.FloatBuffer"));
            try {
                putArray(index, src, off, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
            return *this;
        }

        FloatBuffer &FloatBuffer::put(gint index, const FloatArray &src) {
            try {
                return put(index, src, 0, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &FloatBuffer::putArray(gint index, const FloatArray &src, gint off, gint length) {
            if (((glong) length << 2) > 6) {
                glong const bufAddr = address + ((glong) index << 2);
                glong const srcOffset = ARRAY_BASE_OFFSET + ((glong) off << 2);
                glong const len = (glong) length << 2;
                if (order() != ByteOrder::NATIVE_ENDIAN)
                    Unsafe::copySwapMemory((Object &) src, srcOffset, base(), bufAddr, len, Float::BYTES);
                else
                    Unsafe::copyMemory(src, srcOffset, base(), bufAddr, len);
            } else {
                gint const end = off + length;
                for (gint i = off, j = index; i < end; i++, j++)
                    put(j, src[i]);
            }
            return *this;
        }

        gbool FloatBuffer::hasArray() const {
            return hb != null && !isReadOnly;
        }

        FloatArray &FloatBuffer::array() const {
            if (hb == null)
                UnsupportedOperationException().throws(__trace("core.io.FloatBuffer"));
            if (isReadOnly)
                ReadOnlyBufferException().throws(__trace("core.io.FloatBuffer"));
            return *hb;
        }

        gint FloatBuffer::arrayOffset() const {
            return offset;
        }

        FloatBuffer &FloatBuffer::setPosition(gint newPosition) {
            try {
                Buffer::setPosition(newPosition);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &FloatBuffer::setLimit(gint newLimit) {
            try {
                Buffer::setLimit(newLimit);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &FloatBuffer::mark() {
            try {
                Buffer::mark();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &FloatBuffer::reset() {
            try {
                Buffer::reset();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &FloatBuffer::clear() {
            try {
                Buffer::clear();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &FloatBuffer::flip() {
            try {
                Buffer::flip();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &FloatBuffer::rewind() {
            try {
                Buffer::rewind();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        gint FloatBuffer::hash() const {
            gint h = 1;
            gint const p = position();
            for (gint i = limit() - 1; i >= p; i--)
                h = 31 * h + (gint) get(i);

            return h;
        }

        gbool FloatBuffer::equals(const Object &obj) const {
            if (this == &obj)
                return true;
            if (!(Class<FloatBuffer>::hasInstance(obj)))
                return false;
            FloatBuffer const &that = (FloatBuffer const &) obj;
            gint const thisPos = position();
            gint const thisRem = limit() - thisPos;
            gint const thatPos = that.position();
            gint const thatRem = that.limit() - thatPos;
            if (thisRem < 0 || thisRem != thatRem)
                return false;
            return mismatch(*this, thisPos, that, thatPos, thisRem) < 0;
        }

        int FloatBuffer::mismatch(const FloatBuffer &a, int aOff, const FloatBuffer &b, int bOff, int length) {
            int i = 0;
            if (length > 0 && a.order() == b.order()) {
                if (Float::toIntBits(a.get(aOff)) == Float::toIntBits(b.get(bOff))) {
                    i = ArraysSupport::vectorizedMismatch(
                            a.base(), a.address + (aOff << ArraysSupport::LOG2_ARRAY_FLOAT_INDEX_SCALE),
                            b.base(), b.address + (bOff << ArraysSupport::LOG2_ARRAY_FLOAT_INDEX_SCALE),
                            length,
                            ArraysSupport::LOG2_ARRAY_FLOAT_INDEX_SCALE);
                }
                // Mismatched
                if (i >= 0) {
                    // Check if mismatch is not associated with two NaN values; and
                    // is not associated with +0 and -0
                    gfloat const av = a.get(aOff + i);
                    gfloat const bv = b.get(bOff + i);
                    if (av != bv && (!Float::isNaN(av) || !Float::isNaN(bv)))
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
                gfloat const av = a.get(aOff + i);
                gfloat const bv = b.get(bOff + i);
                if (av != bv && (!Float::isNaN(av) || !Float::isNaN(bv)))
                    return i;
            }
            return -1;
        }

        gint FloatBuffer::compareTo(const FloatBuffer &that) const {
            gint const thisPos = position();
            gint const thisRem = limit() - thisPos;
            gint const thatPos = that.position();
            gint const thatRem = that.limit() - thatPos;
            gint const length = Math::min(thisRem, thatRem);
            if (length < 0)
                return -1;
            gint const i = mismatch(*this, thisPos, that, thatPos, length);
            if (i >= 0) {
                return Float::compare(get(thisPos + i), that.get(thatPos + i));
            }
            return thisRem - thatRem;
        }

        gint FloatBuffer::mismatch(const FloatBuffer &that) const {
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

        String FloatBuffer::toString() const {
            return classname()
                   + "[pos=" + String::valueOf(position())
                   + " lim=" + String::valueOf(limit())
                   + " cap=" + String::valueOf(capacity())
                   + "]";
        }
    } // core
} // io
