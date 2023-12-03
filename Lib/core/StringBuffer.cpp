//
// Created by Brunshweeck on 12/09/2023.
//

#include "StringBuffer.h"
#include <core/private/Unsafe.h>
#include <core/private/Preconditions.h>
#include <core/private/ArraysSupport.h>
#include <core/primitive/CharArray.h>
#include "ArgumentException.h"
#include "IndexException.h"
#include "Character.h"
#include "Integer.h"
#include "Math.h"

namespace core {

    using native::Unsafe;
    using util::Preconditions;
    using util::ArraysSupport;

    CORE_ALIAS(PBYTE, typename Class<gbyte>::Ptr);
    CORE_ALIAS(PCBYTE, typename Class<const gbyte>::Ptr);
    namespace {

        Unsafe &U = Unsafe::U;

        void putChar(PBYTE dst, gint idx, gint ch) {
            if (dst == null || idx < 0)
                return;
            glong index = idx * 2LL;
            if (!Character::isValidCodePoint(ch)) {
                putChar(dst, idx, '?');
            } else if (Character::isSupplementary(ch)) {
                putChar(dst, idx, Character::highSurrogate(ch));
                putChar(dst, idx + 1LL, Character::lowSurrogate(ch));
            } else {
                gbyte hb = Character::highByte(ch);
                gbyte lb = Character::lowByte(ch);
                if (native::Unsafe::BIG_ENDIAN) {
                    dst[index] = hb;
                    dst[index + 1LL] = lb;
                } else {
                    dst[index] = lb;
                    dst[index + 1LL] = hb;
                }
            }
        }

        gchar nextChar(PCBYTE src, glong idx) {
            if (src == null || idx < 0)
                return Character::MIN_VALUE;
            glong index = idx * 2LL;
            if (native::Unsafe::BIG_ENDIAN)
                return Character::joinBytes(src[index], src[index + 1]);
            else
                return Character::joinBytes(src[index + 1], src[index]);
        }

        PBYTE generate(gint count) {
            if (count <= 0)
                return null;
            PBYTE address = (PBYTE) U.allocateMemory(Integer::toUnsignedLong(count) * 2LL);
            if(address == null) generate(count);
            address[count * 2LL] = 0;
            return address;
        }


        gint charCount(gint ch) {
            return Character::isSupplementary(ch) && Character::isValidCodePoint(ch) ? 2 : 1;
        }

        void arraycopy(PCBYTE src, gint offset1, PBYTE dst, gint offset2, gint count) {
            if (src == null || dst == null || count == 0 || offset1 < 0 || offset2 < 0) {
                return;
            }
            glong maxCount = count * 2LL;
            glong maxOffset1 = offset1 * 2LL;
            glong maxOffset2 = offset2 * 2LL;
            if (count > 0)
                for (glong i = maxCount - 1; i >= 0; i -= 1) {
                    dst[i + maxOffset2] = src[i + maxOffset1];
                }
            else
                for (glong i = maxCount + 1; i <= 0; i += 1) {
                    dst[-i + maxOffset2] = src[-i + maxOffset1];
                }
        }

        template<class T, class U>
        void exchange(T &t, U &u) {
            T t2 = t;
            t = (T) u;
            u = (U) t2;
        }
    }

    StringBuffer::StringBuffer() : StringBuffer(DEFAULT_CAPACITY) {}

    StringBuffer::StringBuffer(gint capacity) {
        if (capacity < 0)
            ArgumentException("Negative capacity").throws(__trace("core.StringBuffer"));
        try {
            value = generate(capacity);
            len = 0;
            cap = capacity;
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
    }

    gint StringBuffer::newCapacity(gint minCapacity) const {
        gint oldLength = cap;
        gint newLength = minCapacity << 1;
        gint growth = newLength - oldLength;
        gint length = ArraysSupport::newLength(oldLength, growth, oldLength + 2);
        if (length == Integer::MAX_VALUE)
            MemoryError("Required length exceeds implementation limit").throws(__trace("core.StringBuffer"));
        return length;
    }

    void StringBuffer::shift(gint offset, gint count) {
        return arraycopy(value, offset, value, offset + count, len - offset);
    }

    StringBuffer::StringBuffer(const String &str)
            : StringBuffer(Math::max(str.length() + DEFAULT_CAPACITY, Integer::MAX_VALUE)) {
        append(str);
    }

    StringBuffer::StringBuffer(const StringBuffer &sb)
            : StringBuffer(Math::max(sb.length(), DEFAULT_CAPACITY)) {
        append(sb);
    }

    StringBuffer::StringBuffer(StringBuffer &&sb) CORE_NOTHROW {
        exchange(cap, sb.cap);
        exchange(value, sb.value);
        exchange(len, sb.len);
    }

    StringBuffer &StringBuffer::operator=(const StringBuffer &sb) {
        if (this != &sb) {
            if (cap >= sb.cap) {
                // set all trailing characters to null (U+0000)
                for (gint i = sb.length(); i < len; ++i)
                    putChar(value, i, 0);
                len = sb.length();
            } else {
                resize(sb.length());
            }
            arraycopy(sb.value, 0, value, 0, sb.length());
        }
        return *this;
    }

    StringBuffer &StringBuffer::operator=(StringBuffer &&sb) CORE_NOTHROW {
        if (this != &sb) {
            exchange(cap, sb.cap);
            exchange(value, sb.value);
            exchange(len, sb.len);
        }
        return *this;
    }

    void StringBuffer::resize(gint newLength) {
        if (newLength < 0)
            ArgumentException().throws(__trace("core.StringBuffer"));
        else if (newLength > cap) {
            try {
                gint newCapacity = StringBuffer::newCapacity(newLength);
                PBYTE newValue = generate(newCapacity);
                arraycopy(value, 0, newValue, 0, len);
                U.freeMemory((glong) value);
                cap = newLength;
                value = newValue;
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.StringBuffer"));
            }
        }
        len = newLength;
    }

    void StringBuffer::truncate() {
        if (len < cap) {
            try {
                PBYTE newValue = generate(len);
                arraycopy(value, 0, newValue, 0, len);
                U.freeMemory((glong) value);
                value = newValue;
                cap = len;
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.StringBuffer"));
            }
        }
    }

    gchar StringBuffer::charAt(gint index) const {
        try {
            Preconditions::checkIndex(index, length());
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return nextChar(value, index);
    }

    gint StringBuffer::codePointAt(gint index) const {
        try {
            Preconditions::checkIndex(index, length());
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        gchar ch1 = nextChar(value, index);
        gchar ch2 = nextChar(value, index + 1);
        return Character::isSurrogatePair(ch1, ch2) ? Character::joinSurrogates(ch1, ch2) : ch1;
    }

    void StringBuffer::set(gint index, gchar ch) {
        try {
            Preconditions::checkIndex(index, length());
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        putChar(value, index, ch);
    }

    StringBuffer &StringBuffer::append(const Object &obj) {
        if (&null == &obj) {
            gint oldLen = len;
            try {
                resize(length() + 4);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.StringBuffer"));
            }
            putChar(value, oldLen + 0, 'n');
            putChar(value, oldLen + 1, 'u');
            putChar(value, oldLen + 2, 'l');
            putChar(value, oldLen + 3, 'l');
            return *this;
        } else if (Class<String>::hasInstance(obj))
            return append((const String &) obj);
        else if (Class<StringBuffer>::hasInstance(obj))
            return append((const StringBuffer &) obj);
        else
            return append(String::valueOf(obj));
    }

    StringBuffer &StringBuffer::append(const String &str) {
        gint oldLen = len;
        try {
            resize(len + str.length());
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        arraycopy(str.value, 0, value, oldLen, str.length());
        return *this;
    }

    StringBuffer &StringBuffer::append(const StringBuffer &sb) {
        gint oldLen = len;
        try {
            resize(len + sb.length());
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        arraycopy(sb.value, 0, value, oldLen, sb.length());
        return *this;
    }

    StringBuffer &StringBuffer::append(gbool b) {
        gint oldLen = len;
        try {
            if (b) {
                resize(len + 4);
                putChar(value, oldLen + 0, 't');
                putChar(value, oldLen + 1, 'r');
                putChar(value, oldLen + 2, 'u');
                putChar(value, oldLen + 3, 'e');
            } else {
                resize(len + 5);
                putChar(value, oldLen + 0, 'f');
                putChar(value, oldLen + 1, 'a');
                putChar(value, oldLen + 2, 'l');
                putChar(value, oldLen + 3, 's');
                putChar(value, oldLen + 4, 'e');
            }
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::append(gchar ch) {
        gint oldLen = len;
        try {
            resize(len + 1);
            putChar(value, oldLen + 0, ch);
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::appendCodePoint(gint codePoint) {
        gint oldLen = len;
        try {
            resize(len + charCount(codePoint));
            putChar(value, oldLen + 0, codePoint);
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::append(gint i) {
        gint oldLen = len;
        try {
            // check number of chars necessary
            gint offset = i < 0 ? 1 : 0;
            gint a = Math::abs(i);
            while (a > 0) {
                offset++;
                a /= 10;
            }
            resize(len + offset);
            a = Math::abs(i);
            do {
                offset -= 1;
                putChar(value, oldLen + offset, (a % 10) + '0');
                a /= 10;
            } while (a > 0);
            if (i < 0)
                putChar(value, oldLen + 0, '-');
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::append(glong l) {
        gint oldLen = len;
        try {
            // check number of chars necessary
            gint offset = l < 0 ? 1 : 0;
            glong a = Math::abs(l);
            while (a > 0) {
                offset++;
                a /= 10;
            }
            resize(len + offset);
            a = Math::abs(l);
            do {
                offset -= 1;
                putChar(value, oldLen + offset, (gint) (a % 10) + '0');
                a /= 10;
            } while (a > 0);
            if (l < 0)
                putChar(value, oldLen + 0, '-');
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::appendUnsigned(gint i) {
        try {
            if (i > 0)
                return append(i);
            return append(Integer::toUnsignedLong(i));
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
    }

#define CORE_AS_UNSIGNED_LONG(a) (a & 0xFFFFFFFFFFFFFFFFLL)

    StringBuffer &StringBuffer::appendUnsigned(glong l) {
        gint oldLen = len;
        try {
            if (l > 0)
                return append(l);
            gint offset = 0;
            glong a = l;
            while (a != 0) {
                offset++;
                a = CORE_AS_UNSIGNED_LONG(a) / 10;
            }
            resize(len + offset);
            a = Math::abs(l);
            do {
                offset -= 1;
                putChar(value, oldLen + offset, (gint) (CORE_AS_UNSIGNED_LONG(a) % 10) + '0');
                a = CORE_AS_UNSIGNED_LONG(a) / 10;
            } while (a > 0);
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::append(gfloat f) {
        try {
            return append(String::valueOf(f));
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
    }

    StringBuffer &StringBuffer::append(gdouble d) {
        try {
            return append(String::valueOf(d));
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
    }

    StringBuffer &StringBuffer::append(const CharArray &chars) {
        try {
            return append(chars, 0, chars.length());
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
    }

    StringBuffer &StringBuffer::append(const CharArray &chars, gint offset, gint length) {
        gint oldLen = len;
        try {
            Preconditions::checkIndexFromRange(offset, offset + length, chars.length());
            resize(len + length);
            for (gint i = 0; i < length; ++i) {
                putChar(value, oldLen + i, chars[i + offset]);
            }
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::append(const String &str, gint startIndex, gint endIndex) {
        gint oldLen = len;
        try {
            Preconditions::checkIndexFromRange(startIndex, endIndex, str.length());
            gint length = endIndex - startIndex;
            resize(len + length);
            arraycopy(str.value, startIndex, value, oldLen, length);
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::append(const StringBuffer &sb, gint startIndex, gint endIndex) {
        gint oldLen = len;
        try {
            Preconditions::checkIndexFromRange(startIndex, endIndex, sb.length());
            gint length = endIndex - startIndex;
            resize(len + length);
            arraycopy(sb.value, 0, value, oldLen, length);
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::insert(gint offset, const Object &obj) {
        try {
            if (offset == len)
                return append(obj);
            Preconditions::checkIndex(offset, len);
            if (&null == &obj) {
                resize(len + 4);
                shift(offset, 4);
                putChar(value, offset + 0, 'n');
                putChar(value, offset + 1, 'u');
                putChar(value, offset + 2, 'l');
                putChar(value, offset + 3, 'l');
            } else if (Class<StringBuffer>::hasInstance(obj))
                return insert(offset, (const StringBuffer &) obj);
            else if (Class<String>::hasInstance(obj))
                return insert(offset, (const String &) obj);
            else
                return insert(offset, String::valueOf(obj));
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::insert(gint offset, const String &str) {
        try {
            if (offset == len)
                return append(str);
            Preconditions::checkIndex(offset, len);
            resize(len + str.length());
            shift(offset, str.length());
            arraycopy(str.value, 0, value, offset, str.length());
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::insert(gint offset, const StringBuffer &sb) {
        try {
            if (offset == len)
                return append(sb);
            Preconditions::checkIndex(offset, len);
            gint oldLen = len;
            resize(len + sb.length());
            shift(offset, sb.length());
            if (this == &sb) {
                arraycopy(sb.value, 0, value, offset, offset);
                arraycopy(sb.value, offset + oldLen, value, offset << 1, oldLen - offset);
            } else {
                arraycopy(sb.value, 0, value, offset, sb.length());
            }
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::insert(gint offset, gbool b) {
        try {
            if (offset == len)
                return append(b);
            Preconditions::checkIndex(offset, len);
            if (b) {
                resize(len + 4);
                shift(offset, 4);
                putChar(value, offset + 0, 't');
                putChar(value, offset + 1, 'r');
                putChar(value, offset + 2, 'u');
                putChar(value, offset + 3, 'e');
            } else {
                resize(len + 5);
                shift(offset, 5);
                putChar(value, offset + 0, 'f');
                putChar(value, offset + 1, 'a');
                putChar(value, offset + 2, 'l');
                putChar(value, offset + 3, 's');
                putChar(value, offset + 4, 'e');
            }
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::insert(gint offset, gchar ch) {
        try {
            if (offset == len)
                return append(ch);
            Preconditions::checkIndex(offset, len);
            resize(len + 1);
            shift(offset, 1);
            putChar(value, offset + 0, ch);
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::insertCodePoint(gint offset, gint codePoint) {
        try {
            if (offset == len)
                return appendCodePoint(codePoint);
            Preconditions::checkIndex(offset, len);
            resize(len + charCount(codePoint));
            shift(offset, charCount(codePoint));
            putChar(value, offset + 0, codePoint);
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::insert(gint offset, gint i) {
        try {
            if (offset == len)
                return append(i);
            Preconditions::checkIndex(offset, len);
            // check number of chars necessary
            gint count = i < 0 ? 1 : 0;
            gint a = Math::abs(i);
            while (a > 0) {
                count++;
                a /= 10;
            }
            resize(len + count);
            shift(offset, count);
            a = Math::abs(i);
            do {
                count -= 1;
                putChar(value, offset + count, (a % 10) + '0');
                a /= 10;
            } while (a > 0);
            if (i < 0)
                putChar(value, offset + 0, '-');
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::insert(gint offset, glong l) {
        try {
            if (offset == len)
                return append(l);
            Preconditions::checkIndex(offset, len);
            // check number of chars necessary
            gint count = l < 0 ? 1 : 0;
            glong a = Math::abs(l);
            while (a > 0) {
                count++;
                a /= 10;
            }
            resize(len + count);
            shift(offset, count);
            a = Math::abs(l);
            do {
                count -= 1;
                putChar(value, offset + count, (gint) (a % 10) + '0');
                a /= 10;
            } while (a > 0);
            if (l < 0)
                putChar(value, offset + 0, '-');
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::insertUnsigned(gint offset, gint i) {
        try {
            if (offset == len)
                return appendUnsigned(i);
            return insertUnsigned(offset, Integer::toUnsignedLong(i));
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::insertUnsigned(gint offset, glong l) {
        try {
            if (offset == len)
                return appendUnsigned(l);
            else if (l > 0)
                return insert(offset, l);
            else {
                Preconditions::checkIndex(offset, len);
                gint count = 0;
                glong a = l;
                while (a != 0) {
                    count++;
                    a = CORE_AS_UNSIGNED_LONG(a) / 10;
                }
                resize(len + count);
                a = Math::abs(l);
                do {
                    count -= 1;
                    putChar(value, offset + count, (gint) (CORE_AS_UNSIGNED_LONG(a) % 10) + '0');
                    a = CORE_AS_UNSIGNED_LONG(a) / 10;
                } while (a > 0);
            }
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::insert(gint offset, gfloat f) {
        try {
            if (offset == len)
                return append(f);
            Preconditions::checkIndex(offset, len);
            return insert(offset, String::valueOf(f));
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::insert(gint offset, gdouble d) {
        try {
            if (offset == len)
                return append(d);
            Preconditions::checkIndex(offset, len);
            return insert(offset, String::valueOf(d));
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::insert(gint offset, const CharArray &chars) {
        try {
            if (offset == len)
                return append(chars);
            Preconditions::checkIndex(offset, len);
            return insert(offset, chars, 0, chars.length());
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::insert(gint index, const CharArray &chars, gint offset, gint length) {
        try {
            if (offset == len)
                return append(chars, offset, length);
            Preconditions::checkIndex(index, len);
            Preconditions::checkIndexFromRange(offset, offset + length, chars.length());
            resize(len + length);
            shift(index, length);
            for (gint i = 0; i < length; ++i) {
                putChar(value, index + i, chars[i + offset]);
            }
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::insert(gint offset, const String &str, gint startIndex, gint endIndex) {
        try {
            if (startIndex == len)
                return append(str, startIndex, endIndex);
            Preconditions::checkIndex(offset, len);
            Preconditions::checkIndexFromRange(startIndex, endIndex, str.length());
            gint length = endIndex - startIndex;
            resize(len + length);
            shift(offset, length);
            arraycopy(str.value, startIndex, value, offset, length);
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::insert(gint offset, const StringBuffer &sb, gint startIndex, gint endIndex) {
        try {
            if (startIndex == len)
                return append(sb, startIndex, endIndex);
            Preconditions::checkIndex(offset, len);
            Preconditions::checkIndexFromRange(startIndex, endIndex, sb.length());
            gint length = endIndex - startIndex;
            resize(len + length);
            shift(offset, length);
            if (this == &sb) {
                if (startIndex == offset) {}
                else if (startIndex > offset || endIndex <= offset) {
                    arraycopy(sb.value, startIndex + length, value, offset, length);
                } else {
                    gint diff = offset - startIndex;
                    arraycopy(sb.value, startIndex, value, offset, diff);
                    arraycopy(sb.value, offset + length, value, offset + diff, length - diff);
                }
            } else {
                arraycopy(sb.value, startIndex, value, offset, length);
            }
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::replace(gint startIndex, gint endIndex, const String &str) {
        try {
            Preconditions::checkIndexFromRange(startIndex, endIndex, len);
            gint length = endIndex - startIndex;
            gint n = str.length() - length;
            resize(len + n);
            shift(endIndex, n);
            arraycopy(str.value, 0, value, startIndex, str.length());
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    void StringBuffer::chars(gint srcBegin, gint srcEnd, CharArray &dst, gint dstBegin) const {
        try {
            Preconditions::checkIndexFromRange(srcBegin, srcEnd, len);
            Preconditions::checkIndexFromSize(dstBegin, srcEnd - srcBegin, dst.length());
            gint length = srcEnd - srcBegin;
            for (gint i = 0; i < length; ++i) {
                dst[i + dstBegin] = nextChar(value, i + srcBegin);
            }
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
    }

    CharArray StringBuffer::chars() const {
        try {
            CharArray array = CharArray(len);
            chars(0, len, array, 0);
            return array;
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
    }

    IntArray StringBuffer::codePoints() const {
        gint count = 0;
        // count code points
        for (gint i = 0; i < len;) {
            gchar ch1 = nextChar(value, i);
            gchar ch2 = nextChar(value, i + 1);
            count += 1;
            i += Character::isSurrogatePair(ch1, ch2) && i + 1 < len ? 2 : 1;
        }
        try {
            IntArray array = IntArray(count);
            gint j = 0;
            for (gint i = 0; i < len;) {
                gchar ch1 = nextChar(value, i);
                gchar ch2 = nextChar(value, i + 1);
                if (Character::isSurrogatePair(ch1, ch2) && i + 1 < len) {
                    array[j++] = Character::joinSurrogates(ch1, ch2);
                    i += 2;
                } else {
                    array[j++] = ch1;
                    i += 1;
                }
            }
            return array;
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
    }

    String StringBuffer::subString(gint startIndex) const {
        try {
            return subString(startIndex, len);
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
    }

    String StringBuffer::subString(gint startIndex, gint endIndex) const {
        try {
            Preconditions::checkIndexFromRange(startIndex, endIndex, len);
            if (endIndex > len)
                endIndex = len;
            String str;
            str.value = generate(endIndex - startIndex);
            str.len = endIndex - startIndex;
            arraycopy(value, startIndex, str.value, 0, str.len);
            return str;
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
    }

    gint StringBuffer::indexOf(const String &str) const {
        return indexOf(str, 0);
    }

    gint StringBuffer::indexOf(const String &str, gint begin) const {
        if (begin < 0 || begin + str.length() >= len || str.isEmpty() || len == 0)
            return -1;
        for (gint i = begin; i < len; ++i) {
            gbool isFound = true;
            for (gint j = 0; j < str.len && i + j < len; ++j) {
                gchar ch1 = nextChar(value, i + j);
                gchar ch2 = nextChar(str.value, j);
                if (ch1 != ch2) {
                    isFound = false;
                    break;
                }
            }
            if (isFound)
                return i;
        }
        return -1;
    }

    gint StringBuffer::lastIndexOf(const String &str) const {
        return lastIndexOf(str, length() - 1);
    }

    gint StringBuffer::lastIndexOf(const String &str, gint begin) const {
        if (begin >= len)
            begin = len - 1;
        if (begin < 0 || begin + 1 < str.length() || str.isEmpty() || len == 0)
            return -1;
        for (gint i = begin; i >= 0; ++i) {
            gbool isFound = true;
            for (gint j = 0; j < str.len && i - j >= 0; ++j) {
                gchar ch1 = nextChar(value, i - j);
                gchar ch2 = nextChar(str.value, (str.len - 1) - j);
                if (ch1 != ch2) {
                    isFound = false;
                    break;
                }
            }
            if (isFound)
                return i;
        }
        return -1;
    }

    StringBuffer &StringBuffer::reverse() {
        for (gint i = 0; i <= (len >> 1); ++i) {
            exchange(value[i + 0], value[(len - 1) - (i + 0)]);
            exchange(value[i + 1], value[(len - 1) - (i + 1)]);
        }
        return *this;
    }

    gbool StringBuffer::equals(const Object &object) const {
        if (!Class<StringBuffer>::hasInstance(object))
            return false;
        const StringBuffer &sb = (const StringBuffer &) object;
        if (sb.len != len)
            return false;
        return compareTo(sb) == 0;
    }

    String StringBuffer::toString() const {
        return subString(0);
    }

    gint StringBuffer::compareTo(const StringBuffer &other) const {
        gint limit = Math::min(len, other.len);
        if (limit < 7) {
            // bitwise comparing
            for (gint i = 0; i < 14; ++i) {
                if (value[i] != other.value[i])
                    return nextChar(value, i >> 1) - nextChar(other.value, i >> 1);
            }
        } else {
            for (gint i = 0; i < limit; ++i) {
                gchar ch1 = nextChar(value, i);
                gchar ch2 = nextChar(other.value, i);
                if (ch1 != ch2)
                    return ch1 - ch2;
            }
        }
        return len - other.len;
    }

    Object &StringBuffer::clone() const {
        return U.createInstance<StringBuffer>(*this);
    }

    StringBuffer &StringBuffer::removeAt(gint index) {
        try {
            Preconditions::checkIndex(index, len);
            shift(index, -1);
            len -= 1;
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    StringBuffer &StringBuffer::remove(gint startIndex, gint endIndex) {
        try {
            if (endIndex > len)
                endIndex = len;
            Preconditions::checkIndexFromRange(startIndex, endIndex, len);
            shift(endIndex, startIndex - endIndex);
            len -= endIndex - startIndex;
        } catch (const Throwable &thr) {
            thr.throws(__trace("core.StringBuffer"));
        }
        return *this;
    }

    void StringBuffer::synchronize(gbool tryLock) {
        CORE_IGNORE(tryLock);
    }
} // core