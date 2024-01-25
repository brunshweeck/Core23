//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "CharBuffer.h"
#include <core/private/Unsafe.h>
#include <core/IllegalArgumentException.h>
#include <core/native/CharArray.h>
#include <core/AssertionError.h>
#include <core/util/Preconditions.h>
#include <core/Math.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/private/ReadOnlyBufferException.h>
#include <core/io/private/HeapCharBuffer.h>
#include <core/io/private/StringCharBuffer.h>
#include <core/util/Optional.h>
#include <core/Enum.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        const glong CharBuffer::ARRAY_BASE_OFFSET = Unsafe::ARRAY_CHAR_BASE_OFFSET;

        CharBuffer::CharBuffer(gint mark, gint pos, gint lim, gint cap, CharArray &hb, gint offset) :
                Buffer(mark, pos, lim, cap), hb(&hb), offset(offset), isReadOnly(false) {}

        CharBuffer::CharBuffer(gint mark, gint pos, gint lim, gint cap, gint offset) :
                Buffer(mark, pos, lim, cap), hb(null), offset(offset), isReadOnly(false) {}

        CharBuffer::CharBuffer(gint mark, gint pos, gint lim, gint cap) :
                Buffer(mark, pos, lim, cap), hb(null), offset(0), isReadOnly(false) {}

        CharBuffer::CharBuffer(CharArray &hb, glong addr, gint cap) :
                Buffer(addr, cap), hb(&hb), offset(0), isReadOnly(false) {}

        Object &CharBuffer::base() const {
            if (hb == null)
                return null;
            else
                return (Object &) *hb;
        }

        CharBuffer &CharBuffer::allocate(gint capacity) {
            if (capacity < 0)
                IllegalArgumentException("capacity < 0: (" + String::valueOf(capacity) + " < 0)")
                        .throws(__trace("core.io.CharBuffer"));
            try {
                return Unsafe::allocateInstance<HeapCharBuffer>(capacity, capacity);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapCharBuffer"));
            }
        }

        CharBuffer &CharBuffer::wrap(CharArray &array, gint off, gint length) {
            try {
                return Unsafe::allocateInstance<HeapCharBuffer>(array, off, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapCharBuffer"));
            }
        }

        CharBuffer &CharBuffer::wrap(CharArray &array) {
            try {
                return wrap(array, 0, array.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        gint CharBuffer::read(CharBuffer &target) {
            // Determine the number of chars n that can be transferred
            gint const limit = this->limit();
            gint const pos = this->position();
            gint const remaining = limit - pos;
            CORE_ASSERT(remaining >= 0, "core.io.CharBuffer");
            if (remaining <= 0) // include equality condition when remaining == 0
                return -1;

            gint const targetRemaining = target.remaining();
            CORE_ASSERT(targetRemaining >= 0, "core.io.CharBuffer");
            if (targetRemaining <= 0) // include condition targetRemaining == 0
                return 0;

            gint const n = Math::min(remaining, targetRemaining);

            try {
                // Set source limit to prevent target overflow
                if (targetRemaining < remaining)
                    setLimit(pos + n);
                if (n > 0)
                    target.put(*this);
            } catch (const Exception &ignoredEx) {}
            setLimit(limit);
            return n;
        }

        CharBuffer &CharBuffer::wrap(const CharSequence &csq, gint start, gint end) {
            try {
                if (Class<Object>::hasInstance(csq)) {
                    Object &obj = Unsafe::copyInstance(CORE_DYN_CAST(const Object&, csq), true);
                    CharSequence &csq2 = CORE_DYN_CAST(CharSequence &, obj);
                    return Unsafe::allocateInstance<StringCharBuffer>(csq2, start, end);
                } else {
                    CharSequence &csq2 = (CharSequence &) csq;
                    return Unsafe::allocateInstance<StringCharBuffer>(csq2, start, end);
                }
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        CharBuffer &CharBuffer::wrap(const CharSequence &csq) {
            try {
                return wrap(csq, 0, csq.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        CharBuffer &CharBuffer::get(CharArray &dst, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(off, length, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferUnderflowException().throws(__trace("core.io.CharBuffer"));

            try {
                getArray(pos, dst, off, length);
                setPosition(pos + off);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
            return *this;
        }

        CharBuffer &CharBuffer::get(CharArray &dst) {
            try {
                return get(dst, 0, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        CharBuffer &CharBuffer::get(gint index, CharArray &dst, gint off, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, dst.length());
                getArray(index, dst, off, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
            return (CharBuffer &) *this;
        }

        CharBuffer &CharBuffer::get(gint index, CharArray &dst) const {
            try {
                return get(index, dst, 0, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        CharBuffer &CharBuffer::getArray(gint index, CharArray &dst, gint off, gint length) const {
            try {
                if (isAddressable() && ((glong) length << 1) > 6) {
                    glong const bufAddr = address + ((glong) index << 1);
                    glong const dstOffset = ARRAY_BASE_OFFSET + ((glong) off << 1);
                    glong const len = (glong) length << 1;

                    if (order() != ByteOrder::NATIVE_ENDIAN)
                        Unsafe::copySwapMemory(base(), bufAddr, dst, dstOffset, len, 2);
                    else

                        Unsafe::copyMemory(base(), bufAddr, dst, dstOffset, len);
                } else {
                    gint const end = off + length;
                    for (gint i = off, j = index; i < end; i++, j++) {
                        dst[i] = get(j);
                    }
                }
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
            return (CharBuffer &) *this;
        }

        CharBuffer &CharBuffer::put(CharBuffer &src) {
            if (this == &src)
                IllegalArgumentException("The source buffer is this buffer").throws(__trace("core.io.CharBuffer"));
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.CharBuffer"));

            gint const srcPos = src.position();
            gint const srcLim = src.limit();
            gint const srcRem = (srcPos <= srcLim ? srcLim - srcPos : 0);
            gint const pos = position();
            gint const lim = limit();
            gint const rem = (pos <= lim ? lim - pos : 0);

            if (srcRem > rem)
                BufferOverflowException().throws(__trace("core.io.CharBuffer"));

            try {
                putBuffer(pos, src, srcPos, srcRem);

                setPosition(pos + srcRem);
                src.setPosition(srcPos + srcRem);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
            return *this;
        }

        CharBuffer &CharBuffer::put(gint index, CharBuffer &src, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.CharBuffer"));

            try {
                putBuffer(index, src, off, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
            return *this;
        }

        void CharBuffer::putBuffer(gint pos, CharBuffer &src, gint srcPos, gint n) {

            Object srcBase = src.base();

            if (src.isAddressable()) {
                Object &base = this->base();
                CORE_ASSERT(isDirect(), "core.io.CharBuffer");

                glong const srcAddr = src.address + ((glong) srcPos << 1);
                glong const addr = address + ((glong) pos << 1);
                glong const len = (glong) n << 1;

                if (order() != src.order())
                    Unsafe::copySwapMemory(srcBase, srcAddr, base, addr, len, 2);
                else
                    Unsafe::copyMemory(srcBase, srcAddr, base, addr, len);

            } else { // src.isAddressable() == false
//                CORE_ASSERT(Class<StringCharBuffer>::hasInstance(src), "core.io.CharBuffer");
                gint const posMax = pos + n;
                for (gint i = pos, j = srcPos; i < posMax; i++, j++)
                    put(i, src.get(j));
            }
        }

        CharBuffer &CharBuffer::put(const CharArray &src, gint off, gint length) {
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.CharBuffer"));
            try {
                Preconditions::checkIndexFromSize(off, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws(__trace("core.io.CharBuffer"));

            try {
                putArray(pos, src, off, length);

                setPosition(pos + length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
            return *this;
        }

        CharBuffer &CharBuffer::put(const CharArray &src) {
            try {
                return put(src, 0, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        CharBuffer &CharBuffer::put(gint index, const CharArray &src, gint off, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(off, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.CharBuffer"));
            try {
                putArray(index, src, off, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
            return *this;
        }

        CharBuffer &CharBuffer::put(gint index, const CharArray &src) {
            try {
                return put(index, src, 0, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        CharBuffer &CharBuffer::putArray(gint index, const CharArray &src, gint off, gint length) {
            if (isAddressable() && ((glong) length << 1) > 6) {
                glong const bufAddr = address + ((glong) index << 1);
                glong const srcOffset = ARRAY_BASE_OFFSET + ((glong) off << 1);
                glong const len = (glong) length << 1;
                if (order() != ByteOrder::NATIVE_ENDIAN)
                    Unsafe::copySwapMemory((Object &) src, srcOffset, base(), bufAddr, len, 2);
                else
                    Unsafe::copyMemory(src, srcOffset, base(), bufAddr, len);
            } else {
                gint const end = off + length;
                for (gint i = off, j = index; i < end; i++, j++)
                    put(j, src[i]);
            }
            return *this;
        }

        CharBuffer &CharBuffer::put(const String &src, gint start, gint end) {
            try {
                Preconditions::checkIndexFromSize(start, end - start, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
            if (((Buffer &) *this).isReadOnly())
                ReadOnlyBufferException().throws(__trace("core.io.CharBuffer"));;
            if (end - start > remaining())
                BufferOverflowException().throws(__trace("core.io.CharBuffer"));;
            try {
                for (gint i = start; i < end; i++)
                    put(src.charAt(i));
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
            return *this;
        }

        CharBuffer &CharBuffer::put(const String &src) {
            try {
                return put(src, 0, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        gbool CharBuffer::hasArray() const {
            return hb != null && !isReadOnly;
        }

        CharArray &CharBuffer::array() const {
            if (hb == null)
                UnsupportedOperationException().throws(__trace("core.io.CharBuffer"));
            if (isReadOnly)
                ReadOnlyBufferException().throws(__trace("core.io.CharBuffer"));
            return *hb;
        }

        gint CharBuffer::arrayOffset() const {
            return offset;
        }

        CharBuffer &CharBuffer::setPosition(gint newPosition) {
            try {
                Buffer::setPosition(newPosition);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        CharBuffer &CharBuffer::setLimit(gint newLimit) {
            try {
                Buffer::setLimit(newLimit);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        CharBuffer &CharBuffer::mark() {
            try {
                Buffer::mark();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        CharBuffer &CharBuffer::reset() {
            try {
                Buffer::reset();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        CharBuffer &CharBuffer::clear() {
            try {
                Buffer::clear();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        CharBuffer &CharBuffer::flip() {
            try {
                Buffer::flip();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        CharBuffer &CharBuffer::rewind() {
            try {
                Buffer::rewind();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        gbool CharBuffer::isAddressable() const {
            return true;
        }

        gint CharBuffer::hash() const {
            gint h = 1;
            gint const p = position();
            for (gint i = limit() - 1; i >= p; i--)
                h = 31 * h + (gint) get(i);

            return h;
        }

        gbool CharBuffer::equals(const Object &obj) const {
            if (this == &obj)
                return true;
            if (!(Class<CharBuffer>::hasInstance(obj)))
                return false;
            CharBuffer const &that = (CharBuffer const &) obj;
            gint const thisPos = position();
            gint const thisRem = limit() - thisPos;
            gint const thatPos = that.position();
            gint const thatRem = that.limit() - thatPos;
            if (thisRem < 0 || thisRem != thatRem)
                return false;
            return mismatch(*this, thisPos, that, thatPos, thisRem) < 0;
        }

        gint CharBuffer::mismatch(const CharBuffer &a, gint aOff, const CharBuffer &b, gint bOff, gint length) {
            gint i = 0;
            // Ensure only heap or off-heap buffer instances use the
            // vectorized mismatch. If either buffer is a StringCharBuffer
            // (order is null), then the slow path is taken
            OptionalByteOrder const cro1 = a.charRegionOrder();
            OptionalByteOrder const cro2 = b.charRegionOrder();
            if ((length > 3) && cro1.isPresent() && cro2.isPresent() && (cro1 == cro2)) {
                if (a.get(aOff) != b.get(bOff))
                    return 0;
                i = ArraysSupport::vectorizedMismatch(
                        a.base(), a.address + (aOff << ArraysSupport::LOG2_ARRAY_CHAR_INDEX_SCALE),
                        b.base(), b.address + (bOff << ArraysSupport::LOG2_ARRAY_CHAR_INDEX_SCALE),
                        length,
                        ArraysSupport::LOG2_ARRAY_CHAR_INDEX_SCALE);
                if (i >= 0) return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a.get(aOff + i) != b.get(bOff + i))
                    return i;
            }
            return -1;
        }

        gint CharBuffer::compareTo(const CharBuffer &that) const {
            gint const thisPos = position();
            gint const thisRem = limit() - thisPos;
            gint const thatPos = that.position();
            gint const thatRem = that.limit() - thatPos;
            gint const length = Math::min(thisRem, thatRem);
            if (length < 0)
                return -1;
            gint const i = mismatch(*this, thisPos, that, thatPos, length);
            if (i >= 0) {
                return Character::compare(get(thisPos + i), that.get(thatPos + i));
            }
            return thisRem - thatRem;
        }

        gint CharBuffer::mismatch(const CharBuffer &that) const {
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

        String CharBuffer::toString() const {
            return toString(position(), limit());
        }

        gint CharBuffer::length() const {
            return remaining();
        }

        gbool CharBuffer::isEmpty() const {
            return remaining() == 0;
        }

        gchar CharBuffer::charAt(gint index) const {
            try {
                return get(position() + Preconditions::checkIndex(index, limit() - 1));
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        CharBuffer &CharBuffer::append(const CharSequence &csq) {
            try {
                return put(csq.toString());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        CharBuffer &CharBuffer::append(const CharSequence &csq, gint start, gint end) {
            try {
                return put(csq.subSequence(start, end).toString());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

        CharBuffer &CharBuffer::append(gchar c) {
            try {
                return put(c);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.CharBuffer"));
            }
        }

    } // core
} // io
