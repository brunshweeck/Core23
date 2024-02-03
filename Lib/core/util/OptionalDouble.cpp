//
// Created by T.N.Brunshweeck on 01/02/2024.
//

#include "OptionalDouble.h"
#include <core/NoSuchElementException.h>
#include <core/util/Optional.h>

namespace core {
    namespace util {
        OptionalDouble::OptionalDouble() : value(Double::NaN), present(false) {}

        OptionalDouble::OptionalDouble(gdouble value) : value(value), present(true) {}

        gdouble OptionalDouble::get() const {
            if (!isPresent()) {
                NoSuchElementException().throws(__trace("core.util.DoubleOptional"));
            }
            return value;
        }

        gbool OptionalDouble::isPresent() const {
            return present;
        }

        gbool OptionalDouble::isEmpty() const {
            return !present;
        }

        void OptionalDouble::ifPresent(const DoubleConsumer &action) const {
            if (isPresent()) {
                CORE_TRY_RETHROW(action.accept(value), , __ctrace())
            }
        }

        void OptionalDouble::ifPresentOrElse(const DoubleConsumer &action, const Runnable &emptyAction) const {
            if (isPresent()) {
                CORE_TRY_RETHROW(action.accept(value);, , __ctrace())
            } else {
                emptyAction.run();
            }
        }

        DoubleStream &OptionalDouble::stream() const {
            //
        }

        gdouble OptionalDouble::orElse(gdouble other) const {
            return isPresent() ? value : other;
        }

        gdouble OptionalDouble::orElseGet(const DoubleSupplier &supplier) const {
            if (!isPresent()) {
                CORE_TRY_RETHROW(return supplier.get();, , __ctrace())
            }
            return value;
        }

        gdouble OptionalDouble::orElseThrow() const {
            if (!isPresent()) {
                NoSuchElementException().throws(__trace("core.util.DoubleSupplier"));
            }
            return value;
        }

        gdouble OptionalDouble::orElseThrow(const Supplier<Throwable> &exceptionSupplier) const {
            if (!isPresent()) {
                Optional<Throwable> th;
                CORE_TRY_RETHROW(th = exceptionSupplier.get();, , __ctrace())
                th.get().throws(__ctrace());
            }
            return value;
        }

        gbool OptionalDouble::equals(const Object &obj) const {
            if (this == &obj) {
                return true;
            }
            if (!Class<OptionalDouble>::hasInstance(obj)) {
                return false;
            }
            OptionalDouble const &other = (OptionalDouble const &) obj;
            return present && other.present ? Double::compare(value, other.value) == 0
                                            : present == other.present;
        }

        gint OptionalDouble::hash() const {
            return isPresent() ? Double::hash(value) : 0;
        }

        String OptionalDouble::toString() const {
            return isPresent() ? String::valueOf(value)
                               : u"Optional[]"_S;
        }

        Object &OptionalDouble::clone() const {
            return isPresent() ? Unsafe::allocateInstance<OptionalDouble>(value)
                               : Unsafe::allocateInstance<OptionalDouble>();
        }
    } // util
} // core
