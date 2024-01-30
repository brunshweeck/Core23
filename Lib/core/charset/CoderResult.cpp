//
// Created by T.N.Brunshweeck on 06/01/2024.
//

#include "CoderResult.h"
#include <core/native/StringArray.h>
#include <core/IllegalArgumentException.h>
#include <core/util/ArrayList.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/BufferOverflowException.h>
#include <core/charset/MalformedInputException.h>
#include <core/charset/UnmappableCharacterException.h>

namespace core {
    namespace charset {

        using namespace native;
        using namespace io;

        String CoderResult::toString() const {
            static StringArray const names = StringArray::of(
                    "UNDERFLOW",
                    "OVERFLOW",
                    "MALFORMED",
                    "UNMAPPABLE"
            );
            return isError() ? names[t] : (names[t] + "[" + String::valueOf(length()) + "]");
        }

        gbool CoderResult::isUnderflow() const {
            return t == UNDERFLOW_CODE;
        }

        gbool CoderResult::isOverflow() const {
            return t == OVERFLOW_CODE;
        }

        gbool CoderResult::isError() const {
            return t > OVERFLOW_CODE;
        }

        gbool CoderResult::isMalformed() const {
            return t == MALFORMED_CODE;
        }

        gbool CoderResult::isUnmappable() const {
            return t == UNMAPPABLE_CODE;
        }

        gint CoderResult::length() const {
            return len;
        }

        const CoderResult CoderResult::UNDERFLOW{UNDERFLOW_CODE, 0};

        const CoderResult CoderResult::OVERFLOW{OVERFLOW_CODE, 0};

        const CoderResult CoderResult::malformed[4] = {
                CoderResult(MALFORMED_CODE, 1),
                CoderResult(MALFORMED_CODE, 2),
                CoderResult(MALFORMED_CODE, 3),
                CoderResult(MALFORMED_CODE, 4),
        };
        const CoderResult CoderResult::unmappable[4] = {
                CoderResult(UNMAPPABLE_CODE, 1),
                CoderResult(UNMAPPABLE_CODE, 2),
                CoderResult(UNMAPPABLE_CODE, 3),
                CoderResult(UNMAPPABLE_CODE, 4),
        };

        namespace {
            using namespace util;
            using namespace native;

            ArrayList<CoderResult> Cache1 = {};
            ArrayList<CoderResult> Cache2 = {};
        }

        const CoderResult &CoderResult::malformedForLength(gint length) {
            if (length <= 0)
                IllegalArgumentException("Non-positive length").throws(__trace("core.charset.CodeResult"));
            if (length <= 4)
                return malformed[length - 1];
            const CoderResult &cr = CoderResult(MALFORMED_CODE, length);
            gint i = Cache1.indexOf(cr);
            if (i < 0) {
                Cache1.add(cr);
                i = Cache1.size() - 1;
            }
            return Cache1[i];
        }

        const CoderResult &CoderResult::unmappableForLength(gint length) {
            if (length <= 0)
                IllegalArgumentException("Non-positive length").throws(__trace("core.charset.CodeResult"));
            if (length <= 4)
                return malformed[length - 1];
            const CoderResult &cr = CoderResult(UNMAPPABLE_CODE, length);
            gint i = Cache1.indexOf(cr);
            if (i < 0) {
                Cache1.add(cr);
                i = Cache1.size() - 1;
            }
            return Cache1[i];
        }

        gbool CoderResult::equals(const Object &o) const {
            if (this == &o)
                return true;
            if (!Class<CoderResult>::hasInstance(o))
                return false;
            CoderResult const &cr = (const CoderResult &) o;
            return cr.t == t && cr.len == len;
        }

        Object &CoderResult::clone() const {
            if (t == UNDERFLOW_CODE)
                return (Object &) UNDERFLOW;
            if (t == OVERFLOW_CODE)
                return (Object &) OVERFLOW;
            if (t == MALFORMED_CODE) {
                if (len <= 4)
                    return (Object &) malformed[len - 1];
                if (Cache1.indexOf(*this) < 0)
                    return Unsafe::allocateInstance<CoderResult>(CoderResult(MALFORMED_CODE, len));
                return (Object &) *this;
            }
            if (len <= 4)
                return (Object &) malformed[len - 1];
            if (Cache1.indexOf(*this) < 0)
                return Unsafe::allocateInstance<CoderResult>(CoderResult(UNMAPPABLE_CODE, len));
            return (Object &) *this;
        }

        gint CoderResult::hash() const {
            return ((len << 2) | t);
        }

        void CoderResult::throwException() const {
            switch (t) {
                case UNDERFLOW_CODE:
                    BufferUnderflowException().throws(__trace("core.charset.CoderResult"));
                case OVERFLOW_CODE:
                    BufferOverflowException().throws(__trace("core.charset.CoderResult"));
                case MALFORMED_CODE:
                    MalformedInputException(len).throws(__trace("core.charset.CoderResult"));
                case UNMAPPABLE_CODE:
                    UnmappableCharacterException(len).throws(__trace("core.charset.CoderResult"));
                default:
                    CORE_ASSERT(false, "core.charset.CoderResult");
            }
        }
    } // charset
} // core
