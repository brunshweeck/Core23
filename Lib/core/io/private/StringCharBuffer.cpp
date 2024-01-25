//
// Created by T.N.Brunshweeck on 09/01/2024.
//

#include "StringCharBuffer.h"
#include <core/Enum.h>
#include <core/util/Preconditions.h>
#include <core/util/Optional.h>
#include <core/private/Unsafe.h>
#include <core/io/private/ReadOnlyBufferException.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        StringCharBuffer::StringCharBuffer(CharSequence &s, gint start, gint end) :
                CharBuffer(-1, start, end, s.length()), str(s) {
            try {
                gint const n = s.length();
                Preconditions::checkIndexFromSize(start, end, n);
                CharBuffer::isReadOnly = true;
            } catch (IllegalArgumentException const &x) {
                IndexException().throws(__trace("core.io.StringCharBuffer"));
            }
        }

        StringCharBuffer::StringCharBuffer(CharSequence &s, gint mark, gint pos, gint limit, gint cap, gint offset) :
                CharBuffer(mark, pos, limit, cap, offset), str(s) {
            CharBuffer::isReadOnly = true;
        }

        CharBuffer &StringCharBuffer::slice() const {
            try {
                gint const pos = position();
                gint const lim = limit();
                gint const rem = (pos <= lim ? lim - pos : 0);
                return Unsafe::allocateInstance<StringCharBuffer>(str, -1, 0, rem, rem, offset + pos);
            } catch (IllegalArgumentException const &x) {
                IndexException().throws(__trace("core.io.StringCharBuffer"));
            }
        }

        CharBuffer &StringCharBuffer::duplicate() const {
            try {
                return Unsafe::allocateInstance<StringCharBuffer>(str, markValue(), position(), limit(), capacity(),
                                                                  offset);
            } catch (IllegalArgumentException const &x) {
                IndexException().throws(__trace("core.io.StringCharBuffer"));
            }
        }

        CharBuffer &StringCharBuffer::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (IllegalArgumentException const &x) {
                IndexException().throws(__trace("core.io.StringCharBuffer"));
            }
        }

        gchar StringCharBuffer::get() {
            try {
                return str.charAt(nextGetIndex() + offset);
            } catch (IllegalArgumentException const &x) {
                IndexException().throws(__trace("core.io.StringCharBuffer"));
            }
        }

        gchar StringCharBuffer::get(gint index) const {
            return str.charAt(checkIndex(index) + offset);
        }

        CharBuffer &StringCharBuffer::put(gchar c) {
            CORE_IGNORE(c);
            ReadOnlyBufferException().throws(__trace("core.io.StringCharBuffer"));
        }

        CharBuffer &StringCharBuffer::put(gint index, gchar c) {
            CORE_IGNORE(index);
            CORE_IGNORE(c);
            ReadOnlyBufferException().throws(__trace("core.io.StringCharBuffer"));;
        }

        CharBuffer &StringCharBuffer::compact() {
            ReadOnlyBufferException().throws(__trace("core.io.StringCharBuffer"));;
        }

        gbool StringCharBuffer::isReadOnly() const {
            return true;
        }

        CharBuffer &StringCharBuffer::subSequence(gint start, gint end) const {
            try {
                gint const pos = position();
                return Unsafe::allocateInstance<StringCharBuffer>(str, -1, pos + checkIndex(start, pos),
                                                                  pos + checkIndex(end, pos), capacity(), offset);
            } catch (IllegalArgumentException const &x) {
                IndexException().throws(__trace("core.io.StringCharBuffer"));
            }
        }

        gbool StringCharBuffer::isDirect() const {
            return false;
        }

        CharBuffer::ByteOrder StringCharBuffer::order() const {
            return ByteOrder::NATIVE_ENDIAN;
        }

        CharBuffer::OptionalByteOrder StringCharBuffer::charRegionOrder() const {
            return OptionalByteOrder::empty();
        }

        gbool StringCharBuffer::isAddressable() const {
            return false;
        }

        gbool StringCharBuffer::equals(const Object &obj) const {
            try {
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
            } catch (IllegalArgumentException const &x) {
                IndexException().throws(__trace("core.io.StringCharBuffer"));
            }
        }

        gint StringCharBuffer::compareTo(const CharBuffer &that) const {
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

        String StringCharBuffer::toString(gint start, gint end) const {
            return str.subSequence(start + offset, end + offset).toString();
        }

        gchar StringCharBuffer::getUnchecked(gint index) const {
            return str.charAt(index + offset);
        }

        CharBuffer &StringCharBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return Unsafe::allocateInstance<StringCharBuffer>(str, -1, 0, length, length, offset + index);
            } catch (IllegalArgumentException const &x) {
                IndexException().throws(__trace("core.io.StringCharBuffer"));
            }
        }
    } // io
} // core