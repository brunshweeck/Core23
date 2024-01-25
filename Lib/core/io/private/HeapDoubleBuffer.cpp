//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#include "HeapDoubleBuffer.h"
#include <core/private/Unsafe.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/private/ReadOnlyHeapDoubleBuffer.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;
        
        namespace {
            class System CORE_FINAL : public Object {
            public:
                static void arraycopy(const DoubleArray &src, gint srcBegin, DoubleArray &dst, gint dstBegin, gint length) {
                    try {
                        Preconditions::checkIndexFromSize(srcBegin, length, src.length());
                        Preconditions::checkIndexFromSize(dstBegin, length, src.length());
                        Unsafe::copyMemory(src, Unsafe::ARRAY_DOUBLE_BASE_OFFSET + srcBegin,
                                           dst, Unsafe::ARRAY_DOUBLE_BASE_OFFSET + dstBegin,
                                           (glong) length * Unsafe::ARRAY_DOUBLE_INDEX_SCALE);
                    } catch (const Exception &ex) {
                        ex.throws(__trace("core.io.System"));
                    }
                }
            };
        }


        const glong HeapDoubleBuffer::ARRAY_BASE_OFFSET = Unsafe::ARRAY_DOUBLE_BASE_OFFSET;
        const glong HeapDoubleBuffer::ARRAY_INDEX_SCALE = Unsafe::ARRAY_DOUBLE_INDEX_SCALE;

        HeapDoubleBuffer::HeapDoubleBuffer(gint cap, gint lim) :
                DoubleBuffer(-1, 0, lim, cap,
                             Unsafe::allocateInstance<DoubleArray>(cap), 0) { address = ARRAY_BASE_OFFSET; }

        HeapDoubleBuffer::HeapDoubleBuffer(DoubleArray &buf, gint off, gint len) :
                DoubleBuffer(-1, off, off + len, buf.length(), buf, 0) { address = ARRAY_BASE_OFFSET; }

        HeapDoubleBuffer::HeapDoubleBuffer(DoubleArray &buf, gint mark, gint pos, gint lim, gint cap, gint off) :
                DoubleBuffer(mark, pos, lim, cap, buf, off) { address = ARRAY_BASE_OFFSET + off * ARRAY_INDEX_SCALE; }

        DoubleBuffer &HeapDoubleBuffer::slice() const {
            gint const pos = position();
            gint const lim = limit();
            gint const rem = (pos <= lim ? lim - pos : 0);
            return Unsafe::allocateInstance<HeapDoubleBuffer>(*hb, -1, 0, rem, rem, pos + offset);
        }

        DoubleBuffer &HeapDoubleBuffer::slice(gint index, gint length) const {
            Preconditions::checkIndexFromSize(index, length, limit());
            return Unsafe::allocateInstance<HeapDoubleBuffer>(*hb, -1, 0, length, length, index + offset);
        }

        DoubleBuffer &HeapDoubleBuffer::duplicate() const {
            return Unsafe::allocateInstance<HeapDoubleBuffer>(*hb, markValue(), position(), limit(), capacity(), offset);
        }

        DoubleBuffer &HeapDoubleBuffer::asReadOnlyBuffer() const {
            return Unsafe::allocateInstance<ReadOnlyHeapDoubleBuffer>(*hb, markValue(), position(), limit(), capacity(),
                                                                      offset);
        }

        gdouble HeapDoubleBuffer::get() {
            return (*hb)[ix(nextGetIndex())];
        }

        gdouble HeapDoubleBuffer::get(gint i) const {
            return (*hb)[ix(checkIndex(i))];
        }

        DoubleBuffer &HeapDoubleBuffer::get(DoubleArray &dst, gint offset, gint length) {
            Preconditions::checkIndexFromSize(offset, length, dst.length());
            gint const pos = position();
            if (length > limit() - pos)
                BufferUnderflowException().throws(__trace("core.io.HeapDoubleBuffer"));
            System::arraycopy((*hb), ix(pos), dst, offset, length);
            setPosition(pos + length);
            return *this;
        }

        DoubleBuffer &HeapDoubleBuffer::get(gint index, DoubleArray &dst, gint offset, gint length) const {
            Preconditions::checkIndexFromSize(index, length, limit());
            Preconditions::checkIndexFromSize(offset, length, dst.length());
            System::arraycopy((*hb), ix(index), dst, offset, length);
            return (DoubleBuffer &) *this;
        }

        gbool HeapDoubleBuffer::isDirect() const {
            return false;
        }

        gbool HeapDoubleBuffer::isReadOnly() const {
            return false;
        }

        DoubleBuffer &HeapDoubleBuffer::put(gdouble x) {
            (*hb)[ix(nextPutIndex())] = x;
            return *this;
        }

        DoubleBuffer &HeapDoubleBuffer::put(gint i, gdouble x) {
            (*hb)[ix(checkIndex(i))] = x;
            return *this;
        }

        DoubleBuffer &HeapDoubleBuffer::put(DoubleArray &src, gint offset, gint length) {
            Preconditions::checkIndexFromSize(offset, length, src.length());
            gint const pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws(__trace("core.io.HeapDoubleBuffer"));
            System::arraycopy(src, offset, (*hb), ix(pos), length);
            setPosition(pos + length);
            return *this;
        }

        DoubleBuffer &HeapDoubleBuffer::put(DoubleBuffer &src) {
            DoubleBuffer::put(src);
            return *this;
        }

        DoubleBuffer &HeapDoubleBuffer::put(gint index, DoubleBuffer &src, gint offset, gint length) {
            DoubleBuffer::put(index, src, offset, length);
            return *this;
        }

        DoubleBuffer &HeapDoubleBuffer::put(gint index, const DoubleArray &src, gint offset, gint length) {
            Preconditions::checkIndexFromSize(index, length, limit());
            Preconditions::checkIndexFromSize(offset, length, src.length());
            System::arraycopy(src, offset, (*hb), ix(index), length);
            return *this;
        }

        DoubleBuffer &HeapDoubleBuffer::compact() {
            gint const pos = position();
            gint const lim = limit();
            CORE_ASSERT(pos <= lim, "core.io.HeapDoubleBuffer");
            gint const rem = (pos <= lim ? lim - pos : 0);
            System::arraycopy((*hb), ix(pos), (*hb), ix(0), rem);
            setPosition(rem);
            setLimit(capacity());
            discardMark();
            return *this;
        }

        DoubleBuffer::ByteOrder HeapDoubleBuffer::order() const {
            return ByteOrder::NATIVE_ENDIAN;
        }

        gint HeapDoubleBuffer::ix(gint i) const {
            return i + offset;
        }

    } // io
} // core