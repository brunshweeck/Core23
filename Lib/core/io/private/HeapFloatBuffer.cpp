//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#include "HeapFloatBuffer.h"
#include <core/private/Unsafe.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/private/ReadOnlyHeapFloatBuffer.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        namespace {
            class System CORE_FINAL : public Object {
            public:
                static void
                arraycopy(const FloatArray &src, gint srcBegin, FloatArray &dst, gint dstBegin, gint length) {
                    try {
                        Preconditions::checkIndexFromSize(srcBegin, length, src.length());
                        Preconditions::checkIndexFromSize(dstBegin, length, src.length());
                        Unsafe::copyMemory(src, Unsafe::ARRAY_FLOAT_BASE_OFFSET + srcBegin,
                                           dst, Unsafe::ARRAY_FLOAT_BASE_OFFSET + dstBegin,
                                           (glong) length * Unsafe::ARRAY_FLOAT_INDEX_SCALE);
                    } catch (const Exception &ex) {
                        ex.throws(__trace("core.io.System"));
                    }
                }
            };
        }

        const glong HeapFloatBuffer::ARRAY_BASE_OFFSET = Unsafe::ARRAY_FLOAT_BASE_OFFSET;
        const glong HeapFloatBuffer::ARRAY_INDEX_SCALE = Unsafe::ARRAY_FLOAT_INDEX_SCALE;

        HeapFloatBuffer::HeapFloatBuffer(gint cap, gint lim) :
                FloatBuffer(-1, 0, lim, cap,
                            Unsafe::allocateInstance<FloatArray>(cap), 0) { address = ARRAY_BASE_OFFSET; }

        HeapFloatBuffer::HeapFloatBuffer(FloatArray &buf, gint off, gint len) :
                FloatBuffer(-1, off, off + len, buf.length(), buf, 0) { address = ARRAY_BASE_OFFSET; }

        HeapFloatBuffer::HeapFloatBuffer(FloatArray &buf, gint mark, gint pos, gint lim, gint cap, gint off) :
                FloatBuffer(mark, pos, lim, cap, buf, off) { address = ARRAY_BASE_OFFSET + off * ARRAY_INDEX_SCALE; }

        FloatBuffer &HeapFloatBuffer::slice() const {
            gint const pos = position();
            gint const lim = limit();
            gint const rem = (pos <= lim ? lim - pos : 0);
            return Unsafe::allocateInstance<HeapFloatBuffer>(*hb, -1, 0, rem, rem, pos + offset);
        }

        FloatBuffer &HeapFloatBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return Unsafe::allocateInstance<HeapFloatBuffer>(*hb, -1, 0, length, length, index + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &HeapFloatBuffer::duplicate() const {
            try {
                return Unsafe::allocateInstance<HeapFloatBuffer>(*hb, markValue(), position(), limit(), capacity(),
                                                                 offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &HeapFloatBuffer::asReadOnlyBuffer() const {
            try {
                return Unsafe::allocateInstance<ReadOnlyHeapFloatBuffer>(*hb, markValue(), position(), limit(),
                                                                         capacity(), offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        gfloat HeapFloatBuffer::get() {
            try {
                return (*hb)[ix(nextGetIndex())];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        gfloat HeapFloatBuffer::get(gint i) const {
            try {
                return (*hb)[ix(checkIndex(i))];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &HeapFloatBuffer::get(FloatArray &dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, dst.length());
                gint const pos = position();
                if (length > limit() - pos)
                    BufferUnderflowException().throws(__trace("core.io.HeapFloatBuffer"));
                System::arraycopy((*hb), ix(pos), dst, offset, length);
                setPosition(pos + length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &HeapFloatBuffer::get(gint index, FloatArray &dst, gint offset, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, dst.length());
                System::arraycopy((*hb), ix(index), dst, offset, length);
                return (FloatBuffer &) *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        gbool HeapFloatBuffer::isDirect() const {
            return false;
        }

        gbool HeapFloatBuffer::isReadOnly() const {
            return false;
        }

        FloatBuffer &HeapFloatBuffer::put(gfloat x) {
            (*hb)[ix(nextPutIndex())] = x;
            return *this;
        }

        FloatBuffer &HeapFloatBuffer::put(gint i, gfloat x) {
            try {
                (*hb)[ix(checkIndex(i))] = x;
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &HeapFloatBuffer::put(FloatArray &src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws(__trace("core.io.HeapFloatBuffer"));
            try {
                System::arraycopy(src, offset, (*hb), ix(pos), length);
                setPosition(pos + length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &HeapFloatBuffer::put(FloatBuffer &src) {
            try {
                FloatBuffer::put(src);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &HeapFloatBuffer::put(gint index, FloatBuffer &src, gint offset, gint length) {
            try {
                FloatBuffer::put(index, src, offset, length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &HeapFloatBuffer::put(gint index, const FloatArray &src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                System::arraycopy(src, offset, (*hb), ix(index), length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer &HeapFloatBuffer::compact() {
            try {
                gint const pos = position();
                gint const lim = limit();
                CORE_ASSERT(pos <= lim, "core.io.HeapFloatBuffer");
                gint const rem = (pos <= lim ? lim - pos : 0);
                System::arraycopy((*hb), ix(pos), (*hb), ix(0), rem);
                setPosition(rem);
                setLimit(capacity());
                discardMark();
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.FloatBuffer"));
            }
        }

        FloatBuffer::ByteOrder HeapFloatBuffer::order() const {
            return ByteOrder::NATIVE_ENDIAN;
        }

        gint HeapFloatBuffer::ix(gint i) const {
            return i + offset;
        }

    } // io
} // core