//
// Created by Brunshweeck on 12/09/2023.
//

#include "String.h"
#include <core/private/Unsafe.h>
#include <core/private/Null.h>
#include <core/util/Preconditions.h>
#include <core/IndexException.h>
#include <core/Character.h>
#include <core/Integer.h>
#include <core/Long.h>
#include <core/Float.h>
#include <core/Double.h>

namespace core {

    using namespace util;
    using namespace native;

    namespace {

        CORE_ALIAS(PBYTE, typename Class<gbyte>::Ptr);
        CORE_ALIAS(PCBYTE, typename Class<const gbyte>::Ptr);
        CORE_ALIAS(PCBYTE2, typename Class<const gchar>::Ptr);
        CORE_ALIAS(PCBYTE4, typename Class<const gint>::Ptr);

        CORE_FAST glong alignToHeapWordSize(glong bytes) {
            return bytes >= 0 ? (bytes + Unsafe::ADDRESS_SIZE - 1) & ~(Unsafe::ADDRESS_SIZE - 1) : -1;
        }

        void putChar(PBYTE dst, glong idx, gchar ch) {
            if(dst == null || idx < 0)
                return;
            glong const index = idx * 2LL;
            gbyte const hb = Character::highByte(ch);
            gbyte const lb = Character::lowByte(ch);
            if (Unsafe::BIG_ENDIAN) {
                dst[index] = hb;
                dst[index + 1LL] = lb;
            } else {
                dst[index] = lb;
                dst[index + 1LL] = hb;
            }
        }

        void putChar(PBYTE dst, glong idx, gint cp) {
            if ((dst == null) || idx < 0)
                return;
            if (cp < 0 || cp > Character::MAX_CODEPOINT) {
                putChar(dst, idx, u'?');
            } else if (cp > Character::MAX_VALUE) {
                const gchar high = Character::highSurrogate(cp);
                const gchar low = Character::lowSurrogate(cp);
                putChar(dst, idx, high);
                putChar(dst, idx + 1LL, low);
            } else {
                putChar(dst, idx, (gchar) cp);
            }
        }

        gchar nextChar(PCBYTE src, glong idx) {
            if ((src == null) || idx < 0)
                return Character::MIN_VALUE;
            glong const index = idx * 2LL;
            if (Unsafe::BIG_ENDIAN)
                return Character::joinBytes(src[index], src[index + 1]);
            else
                return Character::joinBytes(src[index + 1], src[index]);
        }

        PBYTE generate(gint count) {
            if (count <= 0) return null;
            glong const sizeInBytes = (count + 1LL) << 1;
            const glong heapSize = alignToHeapWordSize(sizeInBytes);
            PBYTE address = (PBYTE) Unsafe::allocateMemory(sizeInBytes);
            if (address != null)
                for (glong i = sizeInBytes - 2; i < heapSize; ++i)
                    address[i] = 0;
            return address;
        }

        gint decodeUTF8_UTF16(PCBYTE in, PBYTE out, glong limit) {
            glong i = 0;
            glong j = 0;
            for (i = 0; i < limit;) {
                gbyte const b1 = in[i];
                if (b1 >= 0) {
                    putChar(out, j, b1);
                    i += 1;
                    j += 1;
                } else if (b1 >> 5 == -2) {
                    if ((b1 & 0x1e) == 0) {
                        putChar(out, j, '?');
                    } else {
                        gbyte const b2 = in[i + 1];
                        if ((b2 & 0xc0) != 0x80) {
                            putChar(out, j, '?');
                        } else {
                            gchar const ch = (gchar) ((b1 << 6) ^ b2) ^ (((gbyte) 0xC0 << 6) ^ ((gbyte) 0x80 << 0));
                            putChar(out, j, ch);
                        }
                    }
                    i += 2;
                    j += 1;
                } else if (b1 >> 4 == -2) {
                    gbyte const b2 = in[i + 1];
                    gbyte const b3 = in[i + 2];
                    if ((b1 == (gbyte) 0xe0 && (b2 & 0xe0) == 0x80) || (b2 & 0xc0) != 0x80 || (b3 & 0xc0) != 0x80) {
                        putChar(out, j, '?');
                    } else {
                        gchar const ch = (gchar) (b1 << 12) ^ (b2 << 6) ^
                                         (b3 ^ (((gbyte) 0xE0 << 12) ^ ((gbyte) 0x80 << 6) ^ ((gbyte) 0x80 << 0)));
                        if (Character::isSurrogate(ch)) {
                            putChar(out, j, '?');
                        } else {
                            putChar(out, j, ch);
                        }
                    }
                    i += 3;
                    j += 1;
                } else if (b1 >> 3 == -2) {
                    gbyte const b2 = in[i + 1];
                    gbyte const b3 = in[i + 2];
                    gbyte const b4 = in[i + 3];
                    gint const ch = (b1 << 18 ^ (b2 << 12) ^ (b3 << 6) ^ (b4 ^ (
                            ((gbyte) 0xF0 << 18) ^ ((gbyte) 0x80 << 12) ^ ((gbyte) 0x80 << 6) ^ ((gbyte) 0x80 << 0))));
                    if ((b2 & 0xc0) != 0x80 || (b3 & 0xc0) != 0x80 || (b4 & 0xc0) != 0x80 || ch <= 0xFFFF) {
                        putChar(out, j, '?');
                    } else {
                        putChar(out, j, ch);
                    }
                    i += 4;
                    j += 2;
                } else {
                    putChar(out, j, '?');
                    i += 1;
                    j += 1;
                }
            }
            return (gint) Math::min(j, (glong) Integer::MAX_VALUE);
        }

        gint charCount(gint ch) {
            return Character::isSupplementary(ch) && Character::isValidCodePoint(ch) ? 2 : 1;
        }

        void arraycopy(PCBYTE src, gint offset1, PBYTE dst, gint offset2, gint count) {
            if ((src == null) || (dst == null) || count == 0 || offset1 < 0 || offset2 < 0) {
                return;
            }
            glong const maxCount = count * 2LL;
            glong const maxOffset1 = offset1 * 2LL;
            glong const maxOffset2 = offset2 * 2LL;
            if (count > 0)
                for (glong i = maxCount - 1; i >= 0; i -= 1) {
                    dst[i + maxOffset2] = src[i + maxOffset1];
                }
            else
                for (glong i = maxCount + 1; i <= 0; i += 1) {
                    dst[-i + maxOffset2] = src[-i + maxOffset1];
                }
        }
    }

    void String::wrap(glong addr, gint bpc, glong count) {
        value = null;
        len = 0;
        hashcode = 0;
        isHashed = false;
        if (addr == 0)
            IllegalArgumentException("Null pointer").throws(__trace("core.String"));
        // assert ((bpc == 1) || (bpc == 2) || (bpc == 4))
        if (count == 0) return;
        gint length = 0;
        if (count < 0) {
            count = 0;
            switch (bpc) {
                case 1: {
                    PCBYTE b = (PCBYTE) addr;
                    while (b[count] != 0)
                        count += 1;
                    length = decodeUTF8_UTF16(b, null, count);
                    value = generate(length);
                    len = length;
                    decodeUTF8_UTF16(b, value, count);
                    break;
                }
                case 2: {
                    PCBYTE2 b = (PCBYTE2) addr;
                    while (b[count] != 0)
                        count += 1;
                    length = (gint) Math::min((glong) Integer::MAX_VALUE, count);
                    value = generate(length);
                    len = length;
                    for (gint i = 0; i < length; ++i) {
                        putChar(value, i, b[i]);
                    }
                    break;
                }
                case 4: {
                    PCBYTE4 b = (PCBYTE4) addr;
                    gint count2 = 0;
                    while (b[count] != 0) {
                        count2 += charCount(b[count]);
                        count += 1;
                        if (count2 < 0) {
                            count2 = Integer::MAX_VALUE;
                            break;
                        }
                    }
                    length = Math::min(Integer::MAX_VALUE, count2);
                    value = generate(length);
                    len = length;
                    gint j = 0;
                    for (gint i = 0; i < count; ++i) {
                        putChar(value, j, b[i]);
                        j += charCount(b[i]);
                    }
                    break;
                }
                default:
                    return;
            }
        } else {
            switch (bpc) {
                case 1: {
                    PCBYTE b = (PCBYTE) addr;
                    length = decodeUTF8_UTF16(b, null, count);
                    value = generate(length);
                    len = length;
                    decodeUTF8_UTF16(b, value, count);
                    break;
                }
                case 2: {
                    PCBYTE2 b = (PCBYTE2) addr;
                    length = (gint) Long::min(Integer::MAX_VALUE, count);
                    value = generate(length);
                    len = length;
                    for (gint i = 0; i < length; ++i) {
                        putChar(value, i, b[i]);
                    }
                    break;
                }
                case 4: {
                    PCBYTE4 b = (PCBYTE4) addr;
                    gint count2 = 0;
                    count = (gint) Long::min(Integer::MAX_VALUE, count);
                    for (gint i = 0; i < count; ++i) {
                        count2 += charCount(b[i]);
                    }
                    length = (gint) Long::min(Integer::MAX_VALUE, count2);
                    value = generate(length);
                    len = length;
                    gint j = 0;
                    for (gint i = 0; i < count; ++i) {
                        putChar(value, j, b[i]);
                        j += charCount(b[i]);
                    }
                    break;
                }
                default:
                    return;
            }
        }
    }

    void String::wrap(glong addr, gint bpc, glong count, gint offset, gint limit) {
        value = null;
        len = 0;
        hashcode = 0;
        isHashed = false;
        if (addr == 0)
            IllegalArgumentException("Null pointer").throws(__trace("core.String"));
        if (limit < 0)
            IllegalArgumentException("Negative length").throws(__trace("core.String"));
        if (offset < 0)
            IllegalArgumentException("Negative offset").throws(__trace("core.String"));
        // assert ((bpc == 1) && (bpc == 2) && (bpc == 4))
        if (count == 0)
            return;
        gint length = 0;
        if (count < 0) {
            count = Math::min(Integer::MAX_VALUE, limit);
        } else {
            count = Math::min(count, (glong) limit);
        }
        addr += offset * (glong) bpc;
        switch (bpc) {
            case 1: {
                PCBYTE b = (PCBYTE) addr;
                length = decodeUTF8_UTF16(b, null, count - offset);
                value = generate(length);
                len = length;
                decodeUTF8_UTF16(b, value, count);
                break;
            }
            case 2: {
                PCBYTE2 b = (PCBYTE2) addr;
                length = (gint) (count - offset);
                value = generate(length);
                len = length;
                for (gint i = 0; i < length; ++i) {
                    gchar const ch = b[i];
                    putChar(value, i, ch);
                }
                break;
            }
            case 4: {
                PCBYTE4 b = (PCBYTE4) addr;
                gint count2 = 0;
                count = (gint) Long::min(Integer::MAX_VALUE, count - offset);
                for (gint i = 0; i < count; ++i) {
                    count2 += charCount(b[i]);
                }
                length = (gint) Long::min(Integer::MAX_VALUE, count2);
                value = generate(length);
                len = length;
                gint j = 0;
                for (gint i = 0; i < count; ++i) {
                    putChar(value, j, b[i]);
                    j += charCount(b[i]);
                }
                break;
            }
            default:
                return;
        }
    }

    String::String(const String &original) : value(generate(original.len)), len(original.len),
                                             isHashed(original.isHashed), hashcode(original.hashcode) {
        arraycopy(original.value, 0, value, 0, len);
    }

    String::String(String &&original) CORE_NOTHROW {
        Unsafe::swapValues(value, original.value);
        Unsafe::swapValues(len, original.len);
        Unsafe::swapValues(hashcode, original.hashcode);
        Unsafe::swapValues(isHashed, original.isHashed);
    }

    String &String::operator=(const String &str) {
        if (this != &str) {
            PBYTE newValue = {};
            if (len != str.len) {
                newValue = generate(str.len);
                len = 0;
                Unsafe::freeMemory((glong) value);
            } else
                newValue = value;
            arraycopy(str.value, 0, value = newValue, 0, len = str.len);
            hashcode = str.hashcode;
            isHashed = str.isHashed;
        }
        return *this;
    }

    String &String::operator=(String &&str) CORE_NOTHROW {
        if (this != &str) {
            Unsafe::swapValues(value, str.value);
            Unsafe::swapValues(len, str.len);
            Unsafe::swapValues(hashcode, str.hashcode);
            Unsafe::swapValues(isHashed, str.isHashed);
        }
        return *this;
    }

    gchar String::charAt(gint index) const {
        try {
            Preconditions::checkIndex(index, len);
            return nextChar(value, index);
        } catch (const IndexException &ie) { ie.throws(__trace("core.String")); }
    }

    gint String::codePointAt(gint index) const {
        try {
            Preconditions::checkIndex(index, len);
            gchar const ch = nextChar(value, index);
            if (Character::isSurrogate(ch)) return ch;
            gchar const ch2 = nextChar(value, index + 1);
            return Character::isSurrogatePair(ch, ch2) ? Character::joinSurrogates(ch, ch2) : ch;
        } catch (const IndexException &ie) { ie.throws(__trace("core.String")); }
    }

    gbool String::equals(const Object &object) const {
        return (this == &object) || Class<String>::hasInstance(object) && equals((const String &) object);
    }

    gbool String::equals(const String &str) const {
        return (this == &str) || (len == str.len) && (compareTo(str) == 0);
    }

    gbool String::equalsIgnoreCase(const String &str) const {
        return (this == &str) || (len == str.len) && (compareToIgnoreCase(str) == 0);
    }

    gint String::compareTo(const String &other) const {
        if (this == &other) return 0;
        gint const length = Math::min(len, other.len);
        for (gint i = 0; i < length; ++i) {
            gchar const ch1 = nextChar(value, i);
            gchar const ch2 = nextChar(other.value, i);
            if (ch1 != ch2) return ch1 - ch2;
        }
        return len == other.len ? 0 :
               length == len ? -nextChar(other.value, length) :
               nextChar(value, length);
    }

    gint String::compareToIgnoreCase(const String &other) const {
        if (this == &other) return 0;
        gint const length = Math::min(len, other.len);
        for (gint i = 0; i < length; ++i) {
            gchar ch1 = nextChar(value, i);
            gchar ch2 = nextChar(other.value, i);
            if (ch1 != ch2) {
                ch1 = Character::toLowerCase(ch1);
                ch2 = Character::toLowerCase(ch2);
                if (ch1 != ch2) return ch1 - ch2;
            }
        }
        return len == other.len ? 0 :
               length == len ? -nextChar(other.value, length) :
               nextChar(value, length);
    }

    gbool String::startsWith(const String &str, gint offset) const {
        if (offset >= 0 && str.len > 0 && str.len <= len) {
            gbool isFound = true;
            for (gint i = offset; i < len; ++i) {
                for (gint j = 0; j < str.len; ++j) {
                    gchar const ch1 = nextChar(value, i + j);
                    gchar const ch2 = nextChar(str.value, j);
                    if (ch1 != ch2) {
                        isFound = false;
                        break;
                    }
                }
                if (isFound)
                    return true;
            }
        }
        return false;
    }

    gbool String::startsWith(const String &str) const {
        return startsWith(str, 0);
    }

    gbool String::endsWith(const String &str) const {
        return startsWith(str, length() - str.length());
    }

    gint String::hash() const {
        if (!isHashed) {
            gint &hash = (gint &) hashcode;
            hash = 0;
            for (gint i = 0; i < len; ++i) {
                gchar const ch = nextChar(value, i);
                hash += ch * 31 ^ (len - (i + 1));
            }
        }
        return hashcode;
    }

    gint String::indexOf(gint ch) const {
        return indexOf(ch, 0);
    }

    gint String::indexOf(gint ch, gint startIndex) const {
        if (startIndex >= 0)
            for (gint i = startIndex; i < len; ++i) {
                gchar const ch2 = nextChar(value, i);
                if (ch2 == ch)
                    return i;
            }
        return -1;
    }

    gint String::lastIndexOf(gint ch) const {
        return lastIndexOf(ch, length() - 1);
    }

    gint String::lastIndexOf(gint ch, gint startIndex) const {
        if (startIndex >= len)
            startIndex = len - 1;
        if (startIndex >= 0)
            for (gint i = startIndex; i >= 0; --i) {
                gchar const ch2 = nextChar(value, i);
                if (ch2 == ch)
                    return i;
            }
        return -1;
    }

    gint String::indexOf(const String &str) const {
        return indexOf(str, 0);
    }

    gint String::indexOf(const String &str, gint startIndex) const {
        if (startIndex < 0 || startIndex + str.len > len || str.isEmpty())
            return -1;
        for (gint i = startIndex; i < len; ++i) {
            gbool isFound = true;
            for (gint j = 0; j < str.len; ++j) {
                gchar const ch1 = nextChar(value, i + j);
                gchar const ch2 = nextChar(str.value, j);
                if (ch1 != ch2) {
                    isFound = false;
                    break;
                }
            }
            if (isFound) {
                return i;
            }
        }
        return -1;
    }

    gint String::lastIndexOf(const String &str) const {
        return lastIndexOf(str, len - 1);
    }

    gint String::lastIndexOf(const String &str, gint startIndex) const {
        if (startIndex >= len)
            startIndex = len - 1;
        if (startIndex < 0 || startIndex + 1 < str.len || str.isEmpty())
            return -1;
        for (gint i = startIndex; i >= 0; --i) {
            gbool isFound = true;
            for (gint j = 0; j < str.len; ++j) {
                gchar const ch1 = nextChar(value, i - j);
                gchar const ch2 = nextChar(str.value, str.len - 1 - j);
                if (ch1 != ch2) {
                    isFound = false;
                    break;
                }
            }
            if (isFound) {
                return i + 1 - str.len;
            }
        }
        return -1;
    }

    String String::subString(gint startIndex) const {
        try {
            return subString(startIndex, length());
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
    }

    String String::subString(gint startIndex, gint endIndex) const {
        try {
            Preconditions::checkIndexFromRange(startIndex, endIndex, len);
            if (endIndex > len)
                endIndex = len;
            String str;
            str.len = endIndex - startIndex;
            str.value = generate(str.len);
            arraycopy(value, startIndex, str.value, 0, str.len);
            return str;
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
    }

    String String::concat(const String &str) const {
        String str2;
        str2.len = len + str.len;
        if (str2.len < 0)
            MemoryError("Overflow: String length out of range").throws(__trace("core.String"));
        str2.value = generate(str2.len);
        arraycopy(value, 0, str2.value, 0, len);
        arraycopy(str.value, 0, str2.value, len, str.len);
        return str2;
    }

    String String::replace(gchar oldChar, gchar newChar) const {
        String str;
        str.value = generate(len);
        for (gint i = 0; i < len; ++i) {
            gchar const ch = nextChar(value, i);
            putChar(str.value, i, (ch == oldChar) ? newChar : ch);
        }
        return str;
    }

    String String::replace(const String &str, const String &replacement) const {
        gint cnt = count(str);
        if (cnt == 0 || isEmpty() || str.len == 0)
            return *this;
        String str2;
        str2.len = len + (replacement.len - str.len) * cnt;
        if (str2.len < 0)
            MemoryError("Overflow: String length out of range").throws(__trace("core.String"));
        str2.value = generate(str2.len);
        gint k = 0;
        for (gint i = 0; i < len; ++i) {
            gbool isFound = false;
            if (cnt > 0) {
                isFound = true;
                for (gint j = 0; j < str.len; ++j) {
                    gchar const ch1 = nextChar(value, i + j);
                    gchar const ch2 = nextChar(str.value, j);
                    if (ch1 != ch2) {
                        isFound = false;
                        break;
                    }
                }
            }
            if (isFound) {
                cnt -= 1;
                arraycopy(replacement.value, 0, str2.value, k, replacement.len);
                k += replacement.len;
                i += str.len - 1;
            } else {
                gchar const ch = nextChar(value, i);
                putChar(str2.value, k, ch);
                k += 1;
            }
        }
        return str2;
    }

    String String::toLowerCase() const {
        String str;
        str.len = len;
        str.value = generate(len);
        for (gint i = 0; i < len; ++i) {
            gchar const ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar const ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint const codePoint = Character::joinSurrogates(ch, ch2);
                    gint const lowerCase = Character::toLowerCase(codePoint);
                    putChar(str.value, i, lowerCase);
                    if (Character::isSupplementary(lowerCase))
                        i += 1;
                }
            } else {
                gchar const lowerCase = Character::toLowerCase(ch);
                putChar(str.value, i, lowerCase);
            }
        }
        return str;
    }

    String String::toUpperCase() const {
        String str;
        str.len = len;
        str.value = generate(str.len);
        for (gint i = 0; i < len; ++i) {
            gchar const ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar const ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint const codePoint = Character::joinSurrogates(ch, ch2);
                    gint const upperCase = Character::toUpperCase(codePoint);
                    putChar(str.value, i, upperCase);
                    if (Character::isSupplementary(upperCase))
                        i += 1;
                }
            } else {
                gchar const upperCase = Character::toUpperCase(ch);
                putChar(str.value, i, upperCase);
            }
        }
        return str;
    }

    String String::toTitleCase() const {
        String str;
        str.len = len;
        str.value = generate(len);
        for (gint i = 0; i < len; ++i) {
            gchar const ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar const ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint const codePoint = Character::joinSurrogates(ch, ch2);
                    gint const titleCase = Character::toTitleCase(codePoint);
                    putChar(str.value, i, titleCase);
                    if (Character::isSupplementary(titleCase))
                        i += 1;
                }
            } else {
                gchar const titleCase = Character::toTitleCase(ch);
                putChar(str.value, i, titleCase);
            }
        }
        return str;
    }

    String String::toReverseCase() const {
        String str;
        str.len = len;
        str.value = generate(len);
        for (gint i = 0; i < len; ++i) {
            gchar const ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar const ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint const codePoint = Character::joinSurrogates(ch, ch2);
                    gint reversedCase = 0;
                    if (Character::isLowerCase(codePoint))
                        reversedCase = Character::toUpperCase(codePoint);
                    else if (Character::isUpperCase(codePoint))
                        reversedCase = Character::toLowerCase(codePoint);
                    else if (Character::isTitleCase(codePoint))
                        reversedCase = Character::toCaseFold(codePoint);
                    else
                        reversedCase = codePoint;
                    putChar(str.value, i, reversedCase);
                    if (Character::isSupplementary(reversedCase))
                        i += 1;
                }
            } else {
                gchar reversedCase = 0;
                if (Character::isLowerCase(ch))
                    reversedCase = Character::toUpperCase(ch);
                else if (Character::isUpperCase(ch))
                    reversedCase = Character::toLowerCase(ch);
                else if (Character::isTitleCase(ch))
                    reversedCase = Character::toCaseFold(ch);
                else
                    reversedCase = ch;
                putChar(str.value, i, reversedCase);
            }
        }
        return str;
    }

    String String::strip() const {
        gint startIndex = 0;
        for (startIndex = 0; startIndex < len; ++startIndex) {
            gchar const ch = nextChar(value, startIndex);
            if (!Character::isSpace(ch))
                break;
        }
        gint endIndex = 0;
        for (endIndex = len; endIndex > startIndex; --endIndex) {
            gchar const ch = nextChar(value, endIndex - 1);
            if (!Character::isSpace(ch))
                break;
        }
        String str;
        str.len = endIndex - startIndex;
        str.value = generate(str.len);
        arraycopy(value, startIndex, str.value, 0, str.len);
        return str;
    }

    String String::stripLeading() const {
        gint startIndex = 0;
        for (startIndex = 0; startIndex < len; ++startIndex) {
            gchar const ch = nextChar(value, startIndex);
            if (!Character::isSpace(ch))
                break;
        }
        gint const endIndex = len;
        String str;
        str.len = endIndex - startIndex;
        str.value = generate(str.len);
        arraycopy(value, startIndex, str.value, 0, str.len);
        return str;
    }

    String String::stripTrailing() const {
        gint const startIndex = 0;
        gint endIndex = 0;
        for (endIndex = len; endIndex > startIndex; --endIndex) {
            gchar const ch = nextChar(value, endIndex - 1);
            if (!Character::isSpace(ch))
                break;
        }
        String str;
        str.len = endIndex - startIndex;
        str.value = generate(str.len);
        arraycopy(value, startIndex, str.value, 0, str.len);
        return str;
    }

    gbool String::isBlank() const {
        for (gint i = 0; i < len; ++i) {
            gchar const ch = nextChar(value, i);
            if (!Character::isSpace(ch))
                return false;
        }
        return true;
    }

    String String::translateEscape() const {
        String str;
        str.len = len;
        str.value = generate(len);
        gint j = 0;
        for (gint i = 0; i < len; ++i) {
            gchar ch = nextChar(value, i);
            if (ch == '\\') {
                ch = nextChar(value, i += 1);
                switch (ch) {
                    case 'a':
                        putChar(str.value, j++, '\a');
                        break;
                    case 'b':
                        putChar(str.value, j++, '\b');
                        break;
                    case 'f':
                        putChar(str.value, j++, '\f');
                        break;
                    case 'n':
                        putChar(str.value, j++, '\n');
                        break;
                    case 'r':
                        putChar(str.value, j++, '\r');
                        break;
                    case 'v':
                        putChar(str.value, j++, '\v');
                        break;
                    case 's':
                        putChar(str.value, j++, ' ');
                        break;
                    case 't':
                        putChar(str.value, j++, '\t');
                        break;
                    case '\'':
                        putChar(str.value, j++, '\'');
                        break;
                    case '\"':
                        putChar(str.value, j++, '\"');
                        break;
                    case '\\':
                        putChar(str.value, j++, '\\');
                        break;
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7': {
                        // octal escape
                        gint const lim = Integer::min(i + (ch <= '3' ? 2 : 1), len);
                        gint octalCode = ch - '0';
                        while (i < lim) {
                            i += 1;
                            ch = nextChar(value, i);
                            if (ch < '0' || ch > '7')
                                break;
                            octalCode = octalCode * 8 + (ch - '0');
                        }
                        putChar(value, j++, octalCode);
                        break;
                    }
                    case 'u': {
                        // unicode 16 escape.
                        gint const lim = i + 4;
                        gint escape = 0;
                        if (lim >= len) {
                            IllegalArgumentException(
                                    "Invalid unicode escape on input \"\\" + subString(i) + "\"").throws(
                                    __trace("core.String"));
                        }
                        while (++i < lim) {
                            ch = nextChar(value, i);
                            if (ch >= '0' && ch <= '9')
                                escape = escape * 16 + (ch - '0');
                            else if (ch >= 'a' && ch <= 'z')
                                escape = escape * 16 + (ch - 'a' + 10);
                            else if (ch >= 'A' && ch <= 'Z')
                                escape = escape * 16 + (ch - 'A' + 10);
                            else
                                break;
                        }
                        if (i < lim || escape > 0xFFFF)
                            IllegalArgumentException(
                                    "Invalid unicode escape on input \"\\u" + Integer::toHexString(escape) +
                                    "\"").throws(__trace("core.String"));
                        putChar(value, j++, escape);
                        break;
                    }
                    case 'U': {
                        // unicode 32 escape.
                        gint const lim = i + 8;
                        gint escape = 0;
                        if (lim >= len) {
                            IllegalArgumentException(
                                    "Invalid unicode escape on input \"\\" + subString(i) + "\"").throws(
                                    __trace("core.String"));
                        }
                        while (++i < lim) {
                            ch = nextChar(value, i);
                            if (ch >= '0' && ch <= '9')
                                escape = escape * 16 + (ch - '0');
                            else if (ch >= 'a' && ch <= 'z')
                                escape = escape * 16 + (ch - 'a' + 10);
                            else if (ch >= 'A' && ch <= 'Z')
                                escape = escape * 16 + (ch - 'A' + 10);
                            else
                                break;
                        }
                        if (i < lim || escape > 0x10FFFF || escape < 0)
                            IllegalArgumentException(
                                    "Invalid unicode escape on input \"\\U" + Integer::toHexString(escape) +
                                    "\"").throws(__trace("core.String"));
                        putChar(value, j++, escape);
                        j += 1;
                        break;
                    }
                    case 'x':
                    case 'X': {
                        // hex escape
                        glong escape = 0;
                        if (i + 2 >= len)
                            IllegalArgumentException("Invalid hex escape on input \"\\x" + subString(i) + "\"").throws(
                                    __trace("core.String"));
                        gint const startIndex = i;
                        while (++i < len && escape <= 0x10FFFF) {
                            ch = nextChar(value, i);
                            if (ch >= '0' && ch <= '9')
                                escape = escape * 16 + (ch - '0');
                            else if (ch >= 'a' && ch <= 'z')
                                escape = escape * 16 + (ch - 'a' + 10);
                            else if (ch >= 'A' && ch <= 'Z')
                                escape = escape * 16 + (ch - 'A' + 10);
                            else
                                break;
                            i += 1;
                        }
                        if (i - startIndex < 2 || i - startIndex > 8 || escape > 0x10FFFF)
                            IllegalArgumentException(
                                    "Invalid hex escape on input \"\\x" + subString(startIndex, i) + "\"")
                                    .throws(__trace("core.String"));
                        putChar(value, j++, (gint) escape);
                        if (escape > Character::MAX_VALUE)
                            j += 1;
                        break;
                    }
                    default:
                        IllegalArgumentException("Invalid escape character \\" + String::valueOf(ch))
                                .throws(__trace("core.String"));
                }
            }
        }
        str.len = j;
        putChar(str.value, j, 0);
        return str;
    }

    String String::toString() const {
        return *this;
    }

    String String::repeat(gint count) const {
        if (count < 0)
            IllegalArgumentException("Negative count").throws(__trace("core.String"));
        if (count == 0 || len == 0)
            return "";
        if (Integer::MAX_VALUE / count < len)
            MemoryError("Overflow: String length required exceed limit").throws(__trace("core.String"));
        String str;
        str.len = count * len;
        str.value = generate(str.len);
        for (gint i = 0; i < count; ++i)
            arraycopy(value, 0, str.value, i * len, len);
        return str;
    }

    gbool String::isASCII() const {
        for (gint i = 0; i < len; ++i) {
            gchar const ch = nextChar(value, i);
            if (ch >= 0x80)
                return false;
        }
        return true;
    }

    gbool String::isLatin1() const {
        for (gint i = 0; i < len; ++i) {
            gchar const ch = nextChar(value, i);
            if (ch >= 0x100)
                return false;
        }
        return true;
    }

    gbool String::isLowerCase() const {
        for (gint i = 0; i < len; ++i) {
            gchar const ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar const ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint const cp = Character::joinSurrogates(ch, ch2);
                    if (Character::isLetter(cp) && !Character::isLowerCase(cp))
                        return false;
                    i += 1;
                    continue;
                }
            }
            if (Character::isLetter(ch) && !Character::isLowerCase(ch))
                return false;
        }
        return true;
    }

    gbool String::isUpperCase() const {
        for (gint i = 0; i < len; ++i) {
            gchar const ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar const ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint const cp = Character::joinSurrogates(ch, ch2);
                    if (Character::isLetter(cp) && !Character::isUpperCase(cp))
                        return false;
                    i += 1;
                    continue;
                }
            }
            if (Character::isLetter(ch) && !Character::isUpperCase(ch))
                return false;
        }
        return true;
    }

    gbool String::isTitleCase() const {
        for (gint i = 0; i < len; ++i) {
            gchar const ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar const ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint const cp = Character::joinSurrogates(ch, ch2);
                    if (Character::isLetter(cp) && !Character::isTitleCase(cp))
                        return false;
                    i += 1;
                    continue;
                }
            }
            if (Character::isLetter(ch) && !Character::isTitleCase(ch))
                return false;
        }
        return true;
    }

    gint String::count(const String &str) const {
        if (str.len > len || str.isEmpty())
            return 0;
        gint cnt = 0;
        for (gint i = 0; i < len; ++i) {
            gbool isFound = true;
            for (gint j = 0; j < str.len; ++j) {
                gchar const ch1 = nextChar(value, i + j);
                gchar const ch2 = nextChar(str.value, j);
                if (ch1 != ch2) {
                    isFound = false;
                    break;
                }
            }
            if (isFound) {
                cnt += 1;
            }
        }
        return cnt;
    }

    Object &String::clone() const {
        return Unsafe::allocateInstance<String>(*this);
    }

    String operator+(const String &x, const String &y) {
        return x.concat(y);
    }

    String String::toPascalCase() const {
        String str;
        str.len = len;
        str.value = generate(str.len);
        gbool start = true;
        for (gint i = 0; i < len; ++i) {
            gchar const ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar const ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint const codePoint = Character::joinSurrogates(ch, ch2);
                    gint pascalCase = 0;
                    if (Character::isSpace(codePoint)) {
                        pascalCase = codePoint;
                        start = true;
                    } else if (start) {
                        pascalCase = Character::toUpperCase(codePoint);
                        start = false;
                    } else {
                        pascalCase = Character::toLowerCase(codePoint);
                    }
                    putChar(str.value, i, pascalCase);
                    if (Character::isSupplementary(pascalCase))
                        i += 1;
                }
            } else {
                gchar pascalCase = 0;
                if (Character::isSpace(ch)) {
                    pascalCase = ch;
                    start = true;
                } else if (start) {
                    pascalCase = Character::toUpperCase(ch);
                    start = false;
                } else {
                    pascalCase = Character::toLowerCase(ch);
                }
                putChar(str.value, i, pascalCase);
            }
        }
        return str;
    }

    String String::toCamelCase() const {
        String str;
        str.len = len;
        str.value = generate(str.len);
        gbool start = false;
        gbool start0 = true;
        for (gint i = 0; i < len; ++i) {
            gchar const ch = nextChar(value, i);
            if (Character::isHighSurrogate(ch)) {
                gchar const ch2 = nextChar(value, i + 1);
                if (Character::isLowSurrogate(ch2)) {
                    gint const codePoint = Character::joinSurrogates(ch, ch2);
                    gint camelCase = 0;
                    if (Character::isSpace(codePoint)) {
                        camelCase = codePoint;
                        start = !start0;
                    } else if (start) {
                        camelCase = Character::toUpperCase(codePoint);
                        start = false;
                    } else {
                        camelCase = Character::toLowerCase(codePoint);
                        start0 = false;
                    }
                    putChar(str.value, i, camelCase);
                    if (Character::isSupplementary(camelCase))
                        i += 1;
                }
            } else {
                gchar camelCase = 0;
                if (Character::isSpace(ch)) {
                    camelCase = ch;
                    start = !start0;
                } else if (start) {
                    camelCase = Character::toUpperCase(ch);
                    start = false;
                } else {
                    camelCase = Character::toLowerCase(ch);
                    start0 = false;
                }
                putChar(str.value, i, camelCase);
            }
        }
        return str;
    }

    String &String::operator+=(const String &str) {
        return (*this) = concat(str);
    }

    void String::chars(gint srcBegin, gint srcEnd, CharArray &dst, gint dstBegin) const {
        try {
            Preconditions::checkIndexFromRange(srcBegin, srcEnd, len);
            Preconditions::checkIndex(dstBegin, dst.length());
            Preconditions::checkIndexFromSize(dstBegin, srcEnd - srcBegin, dst.length());
            for (gint i = srcBegin; i < srcEnd; ++i) {
                dst[i - srcBegin + dstBegin] = nextChar(value, i);
            }
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
    }

    CharArray String::chars() const {
        CharArray array = CharArray(length());
        chars(0, length(), array, 0);
        return array;
    }

    void String::codePoints(gint srcBegin, gint srcEnd, IntArray &dst, gint dstBegin) const {
        try {
            Preconditions::checkIndexFromRange(srcBegin, srcEnd, len);
            Preconditions::checkIndex(dstBegin, dst.length());
            gint count = 0;
            for (gint i = srcBegin; i < srcEnd;) {
                gchar const ch1 = nextChar(value, i);
                gchar const ch2 = nextChar(value, i + 1);
                i += Character::isSurrogatePair(ch1, ch2) ? 2 : 1;
                count += 1;
            }
            Preconditions::checkIndexFromSize(dstBegin, count, dst.length());
            gint j = 0;
            for (gint i = srcBegin; i < srcEnd; ++i) {
                gchar const ch1 = nextChar(value, i);
                gchar const ch2 = nextChar(value, i + 1);
                if (Character::isSurrogatePair(ch1, ch2)) {
                    dst[dstBegin - srcBegin + j] = Character::joinSurrogates(ch1, ch2);
                    i += 2;
                    j += 1;
                } else {
                    dst[dstBegin - srcBegin + j] = ch1;
                    i += 1;
                    j += 1;
                }
            }
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
    }

    IntArray String::codePoints() const {
        IntArray array(0);
        try {
            gint count = 0;
            for (gint i = 0; i < len;) {
                gchar const ch1 = nextChar(value, i);
                gchar const ch2 = nextChar(value, i + 1);
                i += Character::isSurrogatePair(ch1, ch2) ? 2 : 1;
                count += 1;
            }
            array = IntArray(count);
            gint j = 0;
            for (gint i = 0; i < len; ++i) {
                gchar const ch1 = nextChar(value, i);
                gchar const ch2 = nextChar(value, i + 1);
                if (Character::isSurrogatePair(ch1, ch2)) {
                    array[j++] = Character::joinSurrogates(ch1, ch2);
                    i += 2;
                } else {
                    array[j++] = ch1;
                    i += 1;
                }
            }
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
        return array;
    }

    void String::bytes(gint srcBegin, gint srcEnd, ByteArray &dst, gint dstBegin) const {
        try {
            Preconditions::checkIndexFromRange(srcBegin, srcEnd, len);
            Preconditions::checkIndex(dstBegin, dst.length());
            Preconditions::checkIndexFromSize(dstBegin, srcEnd - srcBegin, dst.length());
            for (gint i = srcBegin; i < srcEnd; ++i) {
                dst[i - srcBegin + dstBegin] = (gbyte) (nextChar(value, i) & 0xFF);
            }
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
    }

    ByteArray String::bytes() const {
        if (length() * 2LL > Integer::MAX_VALUE)
            MemoryError("Out of memory").throws(__trace("core.String"));
        ByteArray array(0);
        try {
            array = ByteArray(len << 1);
            for (gint i = 0; i < len; i += 1) {
                array[2 * i + 0] = value[2 * i + 0];
                array[2 * i + 1] = value[2 * i + 1];
            }
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
        return array;
    }

    String String::valueOf(const Object &o) {
        return o.toString();
    }

    String String::valueOf(gbool b) {
        return b ? "true" : "false";
    }

    String String::valueOf(gchar c) {
        return Character::toString(c);
    }

    String String::valueOfCodePoint(gint c) {
        if (Character::isValidCodePoint(c))
            return Character::toString(c);
        return "?";
    }

    String String::valueOf(gint i) {
        return Integer::toString(i);
    }

    String String::valueOf(glong l) {
        return Long::toString(l);
    }

    String String::valueOfUnsigned(gint i) {
        if (i > 0)
            return String::valueOf(i);
        return Integer::toUnsignedString(i);
    }

    String String::valueOfUnsigned(glong l) {
        if (l > 0)
            return String::valueOf(l);
        return Long::toUnsignedString(l);
    }

    String String::valueOf(gfloat f) {
        return Float::toString(f);
    }

    String String::valueOf(gdouble d) {
        return Double::toString(d);
    }

    String String::valueOf(const String &str) {
        return str;
    }

    String::String(const CharArray &chars) : String(chars, 0, chars.length()) {}

    String::String(const CharArray &chars, gint offset, gint count) {
        try {
            Preconditions::checkIndexFromSize(offset, count, chars.length());
            value = generate(count);
            len = count;
            for (gint i = 0; i < count; ++i) {
                putChar(value, i, chars[i + offset]);
            }
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
    }

    String::String(const IntArray &codePoints) : String(codePoints, 0, codePoints.length()) {}

    String::String(const IntArray &codePoints, gint offset, gint count) {
        try {
            Preconditions::checkIndexFromSize(offset, count, codePoints.length());
            gint size = 0;
            for (gint i = 0; i < count; ++i) {
                gint const ch = codePoints[i + offset];
                size += Character::isValidCodePoint(ch) && Character::isSupplementary(ch) ? 2 : 1;
            }
            value = generate(size);
            len = size;
            gint j = 0;
            for (gint i = 0; i < count; ++i) {
                gint const ch = codePoints[i + offset];
                putChar(value, j, ch);
                j += Character::isValidCodePoint(ch) && Character::isSupplementary(ch) ? 2 : 1;
            }
        } catch (const IndexException &ie) {
            ie.throws(__trace("core.String"));
        }
    }

    String::~String() {
        if (len > 0) {
            len = 0;
            Unsafe::freeMemory((glong) value);
        }
        value = null;
        isHashed = false;
        hashcode = 0;
    }

    gint String::length() const {
        return (value != null) && (len > 0) ? len : 0;
    }

    CharSequence &String::subSequence(gint start, gint end) const {
        try {
            Preconditions::checkIndexFromRange(start, end, len);
            return Unsafe::allocateInstance<String>(subString(start, end));
        } catch (IndexException const &iex) { iex.throws(__trace("core.String")); }
    }

    gbool String::isEmpty() const {
        return len <= 0 || value == null;
    }

} // core
