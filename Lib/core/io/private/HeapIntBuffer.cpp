//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#include "HeapIntBuffer.h"
#include <core/private/Unsafe.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/private/ReadOnlyHeapIntBuffer.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        namespace {
            class System CORE_FINAL : public Object {
            public:
                static void arraycopy(const IntArray &src, gint srcBegin, IntArray &dst, gint dstBegin, gint length) {
                    try {
                        Preconditions::checkIndexFromSize(srcBegin, length, src.length());
                        Preconditions::checkIndexFromSize(dstBegin, length, src.length());
                        Unsafe::copyMemory(src, Unsafe::ARRAY_INT_BASE_OFFSET + srcBegin,
                                           dst, Unsafe::ARRAY_INT_BASE_OFFSET + dstBegin,
                                           (glong) length * Unsafe::ARRAY_INT_INDEX_SCALE);
                    } catch (const Exception &ex) {
                        ex.throws(__trace("core.io.System"));
                    }
                }
            };
        }

        const glong HeapIntBuffer::ARRAY_BASE_OFFSET = Unsafe::ARRAY_INT_BASE_OFFSET;
        const glong HeapIntBuffer::ARRAY_INDEX_SCALE = Unsafe::ARRAY_INT_INDEX_SCALE;

        HeapIntBuffer::HeapIntBuffer(gint cap, gint lim) :
                IntBuffer(-1, 0, lim, cap,
                          Unsafe::allocateInstance<IntArray>(cap), 0) { address = ARRAY_BASE_OFFSET; }

        HeapIntBuffer::HeapIntBuffer(IntArray &buf, gint off, gint len) :
                IntBuffer(-1, off, off + len, buf.length(), buf, 0) { address = ARRAY_BASE_OFFSET; }

        HeapIntBuffer::HeapIntBuffer(IntArray &buf, gint mark, gint pos, gint lim, gint cap, gint off) :
                IntBuffer(mark, pos, lim, cap, buf, off) { address = ARRAY_BASE_OFFSET + off * ARRAY_INDEX_SCALE; }

        IntBuffer &HeapIntBuffer::slice() const {
            try {
                gint const pos = position();
                gint const lim = limit();
                gint const rem = (pos <= lim ? lim - pos : 0);
                return Unsafe::allocateInstance<HeapIntBuffer>(*hb, -1, 0, rem, rem, pos + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &HeapIntBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return Unsafe::allocateInstance<HeapIntBuffer>(*hb, -1, 0, length, length, index + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &HeapIntBuffer::duplicate() const {
            try {
                return Unsafe::allocateInstance<HeapIntBuffer>(*hb, markValue(), position(), limit(), capacity(),
                                                               offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &HeapIntBuffer::asReadOnlyBuffer() const {
            try {
                return Unsafe::allocateInstance<ReadOnlyHeapIntBuffer>(*hb, markValue(), position(), limit(),
                                                                       capacity(),
                                                                       offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        gint HeapIntBuffer::get() {
            try {
                return (*hb)[ix(nextGetIndex())];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        gint HeapIntBuffer::get(gint i) const {
            try {
                return (*hb)[ix(checkIndex(i))];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &HeapIntBuffer::get(IntArray &dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferUnderflowException().throws(__trace("core.io.HeapIntBuffer"));
            try {
                System::arraycopy((*hb), ix(pos), dst, offset, length);
                setPosition(pos + length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &HeapIntBuffer::get(gint index, IntArray &dst, gint offset, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, dst.length());
                System::arraycopy((*hb), ix(index), dst, offset, length);
                return (IntBuffer &) *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        gbool HeapIntBuffer::isDirect() const {
            return false;
        }

        gbool HeapIntBuffer::isReadOnly() const {
            return false;
        }

        IntBuffer &HeapIntBuffer::put(gint x) {
            try {
                (*hb)[ix(nextPutIndex())] = x;
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &HeapIntBuffer::put(gint i, gint x) {
            try {
                (*hb)[ix(checkIndex(i))] = x;
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &HeapIntBuffer::put(IntArray &src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws(__trace("core.io.HeapIntBuffer"));
            try {
                System::arraycopy(src, offset, (*hb), ix(pos), length);
                setPosition(pos + length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &HeapIntBuffer::put(IntBuffer &src) {
            try {
                IntBuffer::put(src);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &HeapIntBuffer::put(gint index, IntBuffer &src, gint offset, gint length) {
            try {
                IntBuffer::put(index, src, offset, length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &HeapIntBuffer::put(gint index, const IntArray &src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                System::arraycopy(src, offset, (*hb), ix(index), length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer &HeapIntBuffer::compact() {
            try {
                gint const pos = position();
                gint const lim = limit();
                CORE_ASSERT(pos <= lim, "core.io.HeapIntBuffer");
                gint const rem = (pos <= lim ? lim - pos : 0);
                System::arraycopy((*hb), ix(pos), (*hb), ix(0), rem);
                setPosition(rem);
                setLimit(capacity());
                discardMark();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.IntBuffer"));
            }
        }

        IntBuffer::ByteOrder HeapIntBuffer::order() const {
            return ByteOrder::NATIVE_ENDIAN;
        }

        gint HeapIntBuffer::ix(gint i) const {
            return i + offset;
        }

    } // io
} // core