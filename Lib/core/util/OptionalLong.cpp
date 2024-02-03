//
// Created by T.N.Brunshweeck on 01/02/2024.
//

#include "OptionalLong.h"
#include <core/NoSuchElementException.h>
#include <core/util/Optional.h>

namespace core {
    namespace util {
        OptionalLong::OptionalLong() : value(0LL), present(false) {}

        OptionalLong::OptionalLong(glong value) : value(value), present(true) {}

        glong OptionalLong::get() const {
            if (!isPresent()) {
                NoSuchElementException().throws(__trace("core.util.LongOptional"));
            }
            return value;
        }

        gbool OptionalLong::isPresent() const {
            return present;
        }

        gbool OptionalLong::isEmpty() const {
            return !present;
        }

        void OptionalLong::ifPresent(const LongConsumer &action) const {
            if (isPresent()) {
                CORE_TRY_RETHROW(action.accept(value);, , __ctrace())
            }
        }

        void OptionalLong::ifPresentOrElse(const LongConsumer &action, const Runnable &emptyAction) const {
            if (isPresent()) {
                CORE_TRY_RETHROW(action.accept(value);, , __ctrace())
            } else {
                CORE_TRY_RETHROW(emptyAction.run();, , __ctrace())
            }
        }

        LongStream &OptionalLong::stream() const {
            //
        }

        glong OptionalLong::orElse(glong other) const {
            return isPresent() ? value : other;
        }

        glong OptionalLong::orElseGet(const LongSupplier &supplier) const {
            if (!isPresent()) {
                CORE_TRY_RETHROW(return supplier.get();, , __ctrace())
            }
            return value;
        }

        glong OptionalLong::orElseThrow() const {
            if (!isPresent()) {
                NoSuchElementException().throws(__trace("core.util.LongSupplier"));
            }
            return value;
        }

        glong OptionalLong::orElseThrow(const Supplier<Throwable> &exceptionSupplier) const {
            if (!isPresent()) {
                Optional<Throwable> th;
                CORE_TRY_RETHROW(th = exceptionSupplier.get();, , __ctrace())
                th.get().throws(__ctrace());
            }
            return value;
        }

        gbool OptionalLong::equals(const Object &obj) const {
            if (this == &obj) {
                return true;
            }
            if (!Class<OptionalLong>::hasInstance(obj)) {
                return false;
            }
            OptionalLong const &other = (OptionalLong const &) obj;
            return present && other.present ? Long::compare(value, other.value) == 0
                                            : present == other.present;
        }

        gint OptionalLong::hash() const {
            return isPresent() ? Long::hash(value) : 0;
        }

        String OptionalLong::toString() const {
            return isPresent() ? String::valueOf(value)
                               : u"Optional[]"_S;
        }

        Object &OptionalLong::clone() const {
            return isPresent() ? Unsafe::allocateInstance<OptionalLong>(value)
                               : Unsafe::allocateInstance<OptionalLong>();
        }
    } // util
} // core
