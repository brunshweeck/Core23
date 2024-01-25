//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#include "HeapLongBuffer.h"
#include <core/private/Unsafe.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/private/ReadOnlyHeapLongBuffer.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        namespace {
            class System CORE_FINAL : public Object {
            public:
                static void arraycopy(const LongArray &src, gint srcBegin, LongArray &dst, gint dstBegin, gint length) {
                    try {
                        Preconditions::checkIndexFromSize(srcBegin, length, src.length());
                        Preconditions::checkIndexFromSize(dstBegin, length, src.length());
                        Unsafe::copyMemory(src, Unsafe::ARRAY_LONG_BASE_OFFSET + srcBegin,
                                           dst, Unsafe::ARRAY_LONG_BASE_OFFSET + dstBegin,
                                           (glong) length * Unsafe::ARRAY_LONG_INDEX_SCALE);
                    } catch (const Exception &ex) {
                        ex.throws(__trace("core.io.System"));
                    }
                }
            };
        }

        const glong HeapLongBuffer::ARRAY_BASE_OFFSET = Unsafe::ARRAY_LONG_BASE_OFFSET;
        const glong HeapLongBuffer::ARRAY_INDEX_SCALE = Unsafe::ARRAY_LONG_INDEX_SCALE;

        HeapLongBuffer::HeapLongBuffer(gint cap, gint lim) :
                LongBuffer(-1, 0, lim, cap,
                           Unsafe::allocateInstance<LongArray>(cap), 0) { address = ARRAY_BASE_OFFSET; }

        HeapLongBuffer::HeapLongBuffer(LongArray &buf, gint off, gint len) :
                LongBuffer(-1, off, off + len, buf.length(), buf, 0) { address = ARRAY_BASE_OFFSET; }

        HeapLongBuffer::HeapLongBuffer(LongArray &buf, gint mark, gint pos, gint lim, gint cap, gint off) :
                LongBuffer(mark, pos, lim, cap, buf, off) { address = ARRAY_BASE_OFFSET + off * ARRAY_INDEX_SCALE; }

        LongBuffer &HeapLongBuffer::slice() const {
            try {
                gint const pos = position();
                gint const lim = limit();
                gint const rem = (pos <= lim ? lim - pos : 0);
                return Unsafe::allocateInstance<HeapLongBuffer>(*hb, -1, 0, rem, rem, pos + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &HeapLongBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return Unsafe::allocateInstance<HeapLongBuffer>(*hb, -1, 0, length, length, index + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &HeapLongBuffer::duplicate() const {
            try {
                return Unsafe::allocateInstance<HeapLongBuffer>(*hb, markValue(), position(), limit(), capacity(),
                                                                offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &HeapLongBuffer::asReadOnlyBuffer() const {
            try {
                return Unsafe::allocateInstance<ReadOnlyHeapLongBuffer>(*hb, markValue(), position(), limit(),
                                                                        capacity(),
                                                                        offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        glong HeapLongBuffer::get() {
            try {
                return (*hb)[ix(nextGetIndex())];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        glong HeapLongBuffer::get(gint i) const {
            try {
                return (*hb)[ix(checkIndex(i))];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &HeapLongBuffer::get(LongArray &dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferUnderflowException().throws(__trace("core.io.HeapLongBuffer"));
            try {
                System::arraycopy((*hb), ix(pos), dst, offset, length);
                setPosition(pos + length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &HeapLongBuffer::get(gint index, LongArray &dst, gint offset, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, dst.length());
                System::arraycopy((*hb), ix(index), dst, offset, length);
                return (LongBuffer &) *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        gbool HeapLongBuffer::isDirect() const {
            return false;
        }

        gbool HeapLongBuffer::isReadOnly() const {
            return false;
        }

        LongBuffer &HeapLongBuffer::put(glong x) {
            try {
                (*hb)[ix(nextPutIndex())] = x;
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &HeapLongBuffer::put(gint i, glong x) {
            try {
                (*hb)[ix(checkIndex(i))] = x;
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &HeapLongBuffer::put(const LongArray &src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws(__trace("core.io.HeapLongBuffer"));
            try {
                System::arraycopy(src, offset, (*hb), ix(pos), length);
                setPosition(pos + length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &HeapLongBuffer::put(LongBuffer &src) {
            try {
                LongBuffer::put(src);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &HeapLongBuffer::put(gint index, LongBuffer &src, gint offset, gint length) {
            try {
                LongBuffer::put(index, src, offset, length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &HeapLongBuffer::put(gint index, const LongArray &src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                System::arraycopy(src, offset, (*hb), ix(index), length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer &HeapLongBuffer::compact() {
            try {
                gint const pos = position();
                gint const lim = limit();
                CORE_ASSERT(pos <= lim, "core.io.HeapLongBuffer");
                gint const rem = (pos <= lim ? lim - pos : 0);
                System::arraycopy((*hb), ix(pos), (*hb), ix(0), rem);
                setPosition(rem);
                setLimit(capacity());
                discardMark();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.LongBuffer"));
            }
        }

        LongBuffer::ByteOrder HeapLongBuffer::order() const {
            return ByteOrder::NATIVE_ENDIAN;
        }

        gint HeapLongBuffer::ix(gint i) const {
            return i + offset;
        }

    } // io
} // core