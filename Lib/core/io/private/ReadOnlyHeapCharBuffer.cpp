//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#include "ReadOnlyHeapCharBuffer.h"
#include <core/Enum.h>
#include <core/util/Preconditions.h>
#include <core/util/Optional.h>
#include <core/private/Unsafe.h>
#include <core/io/private/ReadOnlyBufferException.h>
#include <core/io/BufferOverflowException.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        ReadOnlyHeapCharBuffer::ReadOnlyHeapCharBuffer(gint cap, gint lim) :
                HeapCharBuffer(cap, lim) { CharBuffer::isReadOnly = true; }

        ReadOnlyHeapCharBuffer::ReadOnlyHeapCharBuffer(CharArray &buf, gint off, gint len) :
                HeapCharBuffer(buf, off, len) { CharBuffer::isReadOnly = true; }

        ReadOnlyHeapCharBuffer::ReadOnlyHeapCharBuffer(CharArray &buf, gint mark, gint pos, gint lim, gint cap,
                                                       gint off) :
                HeapCharBuffer(buf, mark, pos, lim, cap, off) { CharBuffer::isReadOnly = true; }

        CharBuffer &ReadOnlyHeapCharBuffer::slice() const {
            try {
                gint const pos = position();
                gint const lim = limit();
                gint const rem = (pos <= lim ? lim - pos : 0);
                return Unsafe::allocateInstance<ReadOnlyHeapCharBuffer>((*hb), -1, 0, rem, rem, pos + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapCharBuffer"));
            }
        }

        CharBuffer &ReadOnlyHeapCharBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return Unsafe::allocateInstance<ReadOnlyHeapCharBuffer>((*hb), -1, 0, length, length, index + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapCharBuffer"));
            }
        }

        CharBuffer &ReadOnlyHeapCharBuffer::duplicate() const {
            try {
                return Unsafe::allocateInstance<ReadOnlyHeapCharBuffer>((*hb), markValue(), position(), limit(),
                                                                        capacity(), offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapCharBuffer"));
            }
        }

        CharBuffer &ReadOnlyHeapCharBuffer::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapCharBuffer"));
            }
        }

        gbool ReadOnlyHeapCharBuffer::isReadOnly() const {
            return true;
        }

        CharBuffer &ReadOnlyHeapCharBuffer::put(gchar  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapCharBuffer"));
        }

        CharBuffer &ReadOnlyHeapCharBuffer::put(gint  /*i*/, gchar  /*x*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapCharBuffer"));
        }

        CharBuffer &ReadOnlyHeapCharBuffer::put(const CharArray & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapCharBuffer"));
        }

        CharBuffer &ReadOnlyHeapCharBuffer::put(CharBuffer & /*src*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapCharBuffer"));
        }

        CharBuffer &
        ReadOnlyHeapCharBuffer::put(gint  /*index*/, CharBuffer & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapCharBuffer"));
        }

        CharBuffer &
        ReadOnlyHeapCharBuffer::put(gint  /*index*/, const CharArray & /*src*/, gint  /*offset*/, gint  /*length*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapCharBuffer"));
        }

        CharBuffer &ReadOnlyHeapCharBuffer::put(const String &  /*src*/, gint  /*start*/, gint  /*end*/) {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapCharBuffer"));
        }

        CharBuffer &ReadOnlyHeapCharBuffer::compact() {
            ReadOnlyBufferException().throws(__trace("core.io.ReadOnlyHeapCharBuffer"));
        }

        String ReadOnlyHeapCharBuffer::toString(gint start, gint end) const {
            try {
                const String str = String((*hb), start + offset, end - start);
                return str;
            } catch (const IndexException &x) {
                x.throws(__trace("core.io.ReadOnlyHeapCharBuffer"));
            }
        }

        CharBuffer &ReadOnlyHeapCharBuffer::subSequence(gint start, gint end) const {
            gint const pos = position();
            Preconditions::checkIndexFromRange(start, end, limit() - pos);
            return Unsafe::allocateInstance<ReadOnlyHeapCharBuffer>((*hb), -1, pos + start, pos + end, capacity(),
                                                                    offset);
        }

        CharBuffer::ByteOrder ReadOnlyHeapCharBuffer::order() const {
            return ByteOrder::NATIVE_ENDIAN;
        }

        CharBuffer::OptionalByteOrder ReadOnlyHeapCharBuffer::charRegionOrder() const {
            try {
                return OptionalByteOrder::of(order());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.ReadOnlyHeapCharBuffer"));
            }
        }
    } // io
} // core