//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#include "HeapCharBuffer.h"
#include <core/Enum.h>
#include <core/util/Preconditions.h>
#include <core/util/Optional.h>
#include <core/private/Unsafe.h>
#include <core/native/CharArray.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/private/ReadOnlyHeapCharBuffer.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        namespace {
            class System CORE_FINAL : public Object {
            public:
                static void arraycopy(const CharArray &src, gint srcBegin, CharArray &dst, gint dstBegin, gint length) {
                    try {
                        Preconditions::checkIndexFromSize(srcBegin, length, src.length());
                        Preconditions::checkIndexFromSize(dstBegin, length, src.length());
                        Unsafe::copyMemory(src, Unsafe::ARRAY_CHAR_BASE_OFFSET + srcBegin,
                                           dst, Unsafe::ARRAY_CHAR_BASE_OFFSET + dstBegin,
                                           (glong) length * Unsafe::ARRAY_CHAR_INDEX_SCALE);
                    } catch (const Exception &ex) {
                        ex.throws(__trace("core.io.System"));
                    }
                }
            };
        }

        const glong HeapCharBuffer::ARRAY_BASE_OFFSET = Unsafe::ARRAY_CHAR_BASE_OFFSET;
        const glong HeapCharBuffer::ARRAY_INDEX_SCALE = Unsafe::ARRAY_CHAR_INDEX_SCALE;

        HeapCharBuffer::HeapCharBuffer(gint cap, gint lim) :
                CharBuffer(-1, 0, lim, cap, Unsafe::allocateInstance<CharArray>(cap), 0) { address = ARRAY_BASE_OFFSET; }

        HeapCharBuffer::HeapCharBuffer(CharArray &buf, gint off, gint len) :
                CharBuffer(-1, off, off + len, buf.length(), buf, 0) { address = ARRAY_BASE_OFFSET; }

        HeapCharBuffer::HeapCharBuffer(CharArray &buf, gint mark, gint pos, gint lim, gint cap, gint off)
                : CharBuffer(mark, pos, lim, cap, buf, off) { address = ARRAY_BASE_OFFSET + off * ARRAY_INDEX_SCALE; }

        CharBuffer &HeapCharBuffer::slice() const {
            try {
                gint const pos = position();
                gint const lim = limit();
                gint const rem = (pos <= lim ? lim - pos : 0);
                return Unsafe::allocateInstance<HeapCharBuffer>(*hb, -1, 0, rem, rem, pos + offset);
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        CharBuffer &HeapCharBuffer::slice(gint index, gint length) const {
            Preconditions::checkIndexFromSize(index, length, limit());
            return Unsafe::allocateInstance<HeapCharBuffer>(*hb, -1, 0, length, length, index + offset);
        }

        CharBuffer &HeapCharBuffer::duplicate() const {
            return Unsafe::allocateInstance<HeapCharBuffer>(*hb, markValue(), position(), limit(), capacity(), offset);
        }

        CharBuffer &HeapCharBuffer::asReadOnlyBuffer() const {
            try {
                return Unsafe::allocateInstance<ReadOnlyHeapCharBuffer>(*hb, markValue(), position(), limit(),
                                                                        capacity(),
                                                                        offset);
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        gint HeapCharBuffer::ix(gint i) const {
            return i + offset;
        }

        gchar HeapCharBuffer::get() {
            try {
                return (*hb)[ix(nextGetIndex())];
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        gchar HeapCharBuffer::get(gint i) const {
            try {
                return (*hb)[ix(checkIndex(i))];
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        gchar HeapCharBuffer::getUnchecked(gint i) const {
            try {
                return (*hb)[ix(i)];
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        CharBuffer &HeapCharBuffer::get(CharArray &dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, dst.length());
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferUnderflowException().throws(__trace("core.io.HeapCharBuffer"));
            try {
                System::arraycopy((*hb), ix(pos), dst, offset, length);
                setPosition(pos + length);
                return *this;
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        CharBuffer &HeapCharBuffer::get(gint index, CharArray &dst, gint offset, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, dst.length());
                System::arraycopy((*hb), ix(index), dst, offset, length);
                return (CharBuffer &) *this;
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        gbool HeapCharBuffer::isDirect() const {
            return false;
        }

        gbool HeapCharBuffer::isReadOnly() const {
            return false;
        }

        CharBuffer &HeapCharBuffer::put(gchar x) {
            try {
                (*hb)[ix(nextPutIndex())] = x;
                return *this;
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        CharBuffer &HeapCharBuffer::put(gint i, gchar x) {
            try {
                (*hb)[ix(checkIndex(i))] = x;
                return *this;
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        CharBuffer &HeapCharBuffer::put(const CharArray &src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, src.length());
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws(__trace("core.io.HeapCharBuffer"));
            try {
                System::arraycopy(src, offset, (*hb), ix(pos), length);
                setPosition(pos + length);
                return *this;
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        CharBuffer &HeapCharBuffer::put(CharBuffer &src) {
            try {
                CharBuffer::put(src);
                return *this;
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        CharBuffer &HeapCharBuffer::put(gint index, CharBuffer &src, gint offset, gint length) {
            try {
                CharBuffer::put(index, src, offset, length);
                return *this;
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        CharBuffer &HeapCharBuffer::put(gint index, CharArray &src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                System::arraycopy(src, offset, (*hb), ix(index), length);
                return *this;
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        CharBuffer &HeapCharBuffer::put(const String &src, gint start, gint end) {
            gint const length = end - start;
            try {
                Preconditions::checkIndexFromSize(start, length, src.length());
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
            gint const pos = position();
            gint const lim = limit();
            gint const rem = (pos <= lim) ? lim - pos : 0;
            if (length > rem)
                BufferOverflowException().throws(__trace("core.io.HeapCharBuffer"));
            try {
                src.chars(start, end, (*hb), ix(pos));
                setPosition(pos + length);
                return *this;
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        CharBuffer &HeapCharBuffer::compact() {
            try {
                gint const pos = position();
                gint const lim = limit();
                CORE_ASSERT(pos <= lim, __trace("core.io.HeapCharBuffer"));
                gint const rem = (pos <= lim ? lim - pos : 0);
                System::arraycopy((*hb), ix(pos), (*hb), ix(0), rem);
                setPosition(rem);
                setLimit(capacity());
                discardMark();
                return *this;
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        String HeapCharBuffer::toString(gint start, gint end) const {
            try {
                String const str = String((*hb), start + offset, end - start);
                return str;
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        CharBuffer &HeapCharBuffer::subSequence(gint start, gint end) const {
            try {
                gint const pos = position();
                Preconditions::checkIndexFromRange(start, end, limit() - pos);
                return Unsafe::allocateInstance<HeapCharBuffer>((*hb), -1, pos + start, pos + end, capacity(), offset);
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }

        CharBuffer::ByteOrder HeapCharBuffer::order() const {
            return ByteOrder::NATIVE_ENDIAN;
        }

        CharBuffer::OptionalByteOrder HeapCharBuffer::charRegionOrder() const {
            try {
                return OptionalByteOrder::of(ByteOrder::NATIVE_ENDIAN);
            } catch (const Exception &x) {
                x.throws(__trace("core.io.HeapCharBuffer"));
            }
        }
    } // io
} // core
