//
// Created by T.N.Brunshweeck on 01/02/2024.
//

#include "OptionalInt.h"
#include <core/NoSuchElementException.h>
#include <core/util/Optional.h>

namespace core {
    namespace util {
        OptionalInt::OptionalInt() : value(0), present(false) {}

        OptionalInt::OptionalInt(gint value) : value(value), present(true) {}

        gint OptionalInt::get() const {
            if (!isPresent()) {
                NoSuchElementException().throws(__trace("core.util.IntOptional"));
            }
            return value;
        }

        gbool OptionalInt::isPresent() const {
            return present;
        }

        gbool OptionalInt::isEmpty() const {
            return !present;
        }

        void OptionalInt::ifPresent(const IntConsumer &action) const {
            if (isPresent()) {
                CORE_TRY_RETHROW(action.accept(value);, , __ctrace())
            }
        }

        void OptionalInt::ifPresentOrElse(const IntConsumer &action, const Runnable &emptyAction) const {
            if (isPresent()) {
                CORE_TRY_RETHROW(action.accept(value);, , __ctrace())
            } else {
                CORE_TRY_RETHROW(emptyAction.run();, , __ctrace())
            }
        }

        IntStream &OptionalInt::stream() const {
            //
        }

        gint OptionalInt::orElse(gint other) const {
            return isPresent() ? value : other;
        }

        gint OptionalInt::orElseGet(const IntSupplier &supplier) const {
            if (!isPresent()) {
                CORE_TRY_RETHROW(return supplier.get();, , __ctrace())
            }
            return value;
        }

        gint OptionalInt::orElseThrow() const {
            if (!isPresent()) {
                NoSuchElementException().throws(__trace("core.util.IntSupplier"));
            }
            return value;
        }

        gint OptionalInt::orElseThrow(const Supplier<Throwable> &exceptionSupplier) const {
            if (!isPresent()) {
                Optional<Throwable> th;
                CORE_TRY_RETHROW(th = exceptionSupplier.get();, , __ctrace())
                th.get().throws(__ctrace());
            }
            return value;
        }

        gbool OptionalInt::equals(const Object &obj) const {
            if (this == &obj) {
                return true;
            }
            if (!Class<OptionalInt>::hasInstance(obj)) {
                return false;
            }
            OptionalInt const &other = (OptionalInt const &) obj;
            return present && other.present ? Integer::compare(value, other.value) == 0
                                            : present == other.present;
        }

        gint OptionalInt::hash() const {
            return isPresent() ? Integer::hash(value) : 0;
        }

        String OptionalInt::toString() const {
            return isPresent() ? String::valueOf(value)
                               : u"Optional[]"_S;
        }

        Object &OptionalInt::clone() const {
            return isPresent() ? Unsafe::allocateInstance<OptionalInt>(value)
                               : Unsafe::allocateInstance<OptionalInt>();
        }
    } // util
} // core
