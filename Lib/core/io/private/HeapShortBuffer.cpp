//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#include "HeapShortBuffer.h"
#include <core/private/Unsafe.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/private/ReadOnlyHeapShortBuffer.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        namespace {
            class System CORE_FINAL : public Object {
            public:
                static void
                arraycopy(const ShortArray &src, gint srcBegin, ShortArray &dst, gint dstBegin, gint length) {
                    try {
                        Preconditions::checkIndexFromSize(srcBegin, length, src.length());
                        Preconditions::checkIndexFromSize(dstBegin, length, src.length());
                        Unsafe::copyMemory(src, Unsafe::ARRAY_SHORT_BASE_OFFSET + srcBegin,
                                           dst, Unsafe::ARRAY_SHORT_BASE_OFFSET + dstBegin,
                                           (glong) length * Unsafe::ARRAY_SHORT_INDEX_SCALE);
                    } catch (const Exception &ex) {
                        ex.throws(__trace("core.io.System"));
                    }
                }
            };
        }

        const glong HeapShortBuffer::ARRAY_BASE_OFFSET = Unsafe::ARRAY_SHORT_BASE_OFFSET;
        const glong HeapShortBuffer::ARRAY_INDEX_SCALE = Unsafe::ARRAY_SHORT_INDEX_SCALE;

        HeapShortBuffer::HeapShortBuffer(gint cap, gint lim) :
                ShortBuffer(-1, 0, lim, cap,
                            Unsafe::allocateInstance<ShortArray>(cap), 0) { address = ARRAY_BASE_OFFSET; }

        HeapShortBuffer::HeapShortBuffer(ShortArray &buf, gint off, gint len) :
                ShortBuffer(-1, off, off + len, buf.length(), buf, 0) { address = ARRAY_BASE_OFFSET; }

        HeapShortBuffer::HeapShortBuffer(ShortArray &buf, gint mark, gint pos, gint lim, gint cap, gint off) :
                ShortBuffer(mark, pos, lim, cap, buf, off) { address = ARRAY_BASE_OFFSET + off * ARRAY_INDEX_SCALE; }

        ShortBuffer &HeapShortBuffer::slice() const {
            try {
                gint const pos = position();
                gint const lim = limit();
                gint const rem = (pos <= lim ? lim - pos : 0);
                return Unsafe::allocateInstance<HeapShortBuffer>(*hb, -1, 0, rem, rem, pos + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &HeapShortBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return Unsafe::allocateInstance<HeapShortBuffer>(*hb, -1, 0, length, length, index + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &HeapShortBuffer::duplicate() const {
            try {
                return Unsafe::allocateInstance<HeapShortBuffer>(*hb, markValue(), position(), limit(), capacity(),
                                                                 offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &HeapShortBuffer::asReadOnlyBuffer() const {
            try {
                return Unsafe::allocateInstance<ReadOnlyHeapShortBuffer>(*hb, markValue(), position(), limit(),
                                                                         capacity(), offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        gshort HeapShortBuffer::get() {
            try {
                return (*hb)[ix(nextGetIndex())];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        gshort HeapShortBuffer::get(gint i) const {
            try {
                return (*hb)[ix(checkIndex(i))];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &HeapShortBuffer::get(ShortArray &dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferUnderflowException().throws(__trace("core.io.HeapShortBuffer"));
            try {
                System::arraycopy((*hb), ix(pos), dst, offset, length);
                setPosition(pos + length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &HeapShortBuffer::get(gint index, ShortArray &dst, gint offset, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, dst.length());
                System::arraycopy((*hb), ix(index), dst, offset, length);
                return (ShortBuffer &) *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        gbool HeapShortBuffer::isDirect() const {
            return false;
        }

        gbool HeapShortBuffer::isReadOnly() const {
            return false;
        }

        ShortBuffer &HeapShortBuffer::put(gshort x) {
            try {
                (*hb)[ix(nextPutIndex())] = x;
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &HeapShortBuffer::put(gint i, gshort x) {
            try {
                (*hb)[ix(checkIndex(i))] = x;
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &HeapShortBuffer::put(const ShortArray &src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws(__trace("core.io.HeapShortBuffer"));
            try {
                System::arraycopy(src, offset, (*hb), ix(pos), length);
                setPosition(pos + length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &HeapShortBuffer::put(ShortBuffer &src) {
            try {
                ShortBuffer::put(src);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &HeapShortBuffer::put(gint index, ShortBuffer &src, gint offset, gint length) {
            try {
                ShortBuffer::put(index, src, offset, length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &HeapShortBuffer::put(gint index, const ShortArray &src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                System::arraycopy(src, offset, (*hb), ix(index), length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer &HeapShortBuffer::compact() {
            try {
                gint const pos = position();
                gint const lim = limit();
                CORE_ASSERT(pos <= lim, "core.io.HeapShortBuffer");
                gint const rem = (pos <= lim ? lim - pos : 0);
                System::arraycopy((*hb), ix(pos), (*hb), ix(0), rem);
                setPosition(rem);
                setLimit(capacity());
                discardMark();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ShortBuffer"));
            }
        }

        ShortBuffer::ByteOrder HeapShortBuffer::order() const {
            return ByteOrder::NATIVE_ENDIAN;
        }

        gint HeapShortBuffer::ix(gint i) const {
            return i + offset;
        }

    } // io
} // core