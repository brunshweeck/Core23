//
// Created by T.N.Brunshweeck on 26/11/2023.
//

#include "Temporal.h"
#include "UnsupportedTemporalException.h"
#include <core/ArithmeticException.h>
#include <core/Enum.h>
#include <core/Long.h>
#include <core/time/LocalTime.h>
#include <core/time/LocalDate.h>
#include <core/private/Unsafe.h>

namespace core {
    namespace time {

        CORE_ALIAS(U, native::Unsafe);

        static String const FieldName[30] = {
                "NANO_OF_SECOND",
                "NANO_OF_DAY",
                "MICRO_OF_SECOND",
                "MICRO_OF_DAY",
                "MILLI_OF_SECOND",
                "MILLI_OF_DAY",
                "SECOND_OF_MINUTE",
                "SECOND_DAY",
                "MINUTE_OF_HOUR",
                "MINUTE_OF_DAY",
                "HOUR_OF_AMPM",
                "CLOCK_HOUR_OF_AMPM",
                "HOUR_OF_DAY",
                "CLOCK_HOUR_OF_DAY",
                "AMPM_OF_DAY",
                "DAY_OF_WEEK",
                "ALIGNED_DAY_OF_WEEK_IN_MONTH",
                "ALIGNED_DAY_OF_WEEK_IN_YEAR",
                "DAY_OF_MONTH",
                "DAY_OF_YEAR",
                "EPOCH_DAY",
                "ALIGNED_WEEK_OF_MONTH",
                "ALIGNED_WEEK_OF_YEAR",
                "MONTH_OF_YEAR",
                "PROLEPTIC_MONTH",
                "YEAR_OF_ERA",
                "YEAR",
                "ERA",
                "INSTANT_SECOND",
                "OFFSET_SECOND",
        };

        static String const UnitName[] = {
                "NANOS",
                "MICRO",
                "MILLIS",
                "SECONDS",
                "MINUTES",
                "HOURS",
                "HALF_DAYS",
                "DAYS",
                "WEEKS",
                "MONTHS",
                "YEARS",
                "DECADES",
                "CENTURIES",
                "MILLENNIA",
                "ERAS",
                "FOREVER",
        };

        namespace {
            interface FieldRange {
                glong minSmallest = {};
                glong minLargest = {};
                glong maxSmallest = {};
                glong maxLargest = {};

                CORE_EXPLICIT FieldRange(glong min, glong max) :
                        minSmallest(min),
                        minLargest(min),
                        maxSmallest(max),
                        maxLargest(max) {}

                CORE_EXPLICIT FieldRange(glong min, glong maxSmallest, glong maxLargest) :
                        minSmallest(min),
                        minLargest(min),
                        maxSmallest(maxSmallest),
                        maxLargest(maxLargest) {}

                FieldRange(glong minSmallest, glong minLargest, glong maxSmallest, glong maxLargest) :
                        minSmallest(minSmallest),
                        minLargest(minLargest),
                        maxSmallest(maxSmallest),
                        maxLargest(maxLargest) {}
            };

            FieldRange range(Temporal::Field field) {
                CORE_ALIAS(TemporalField, Temporal::Field);
                switch (field) {
                    case TemporalField::NANO_OF_SECOND:
                        return FieldRange(0, 999999999LL);
                    case TemporalField::NANO_OF_DAY:
                        return FieldRange(0, 86400LL * 1000000000LL - 1);
                    case TemporalField::MICRO_OF_SECOND:
                        return FieldRange(0, 999999LL);
                    case TemporalField::MICRO_OF_DAY:
                        return FieldRange(0, 86400L * 1000000LL - 1);
                    case TemporalField::MILLI_OF_SECOND:
                        return FieldRange(0, 999);
                    case TemporalField::MILLI_OF_DAY:
                        return FieldRange(0, 86400L * 1000L - 1);
                    case TemporalField::SECOND_OF_MINUTE:
                        return FieldRange(0, 59);
                    case TemporalField::SECOND_OF_DAY:
                        return FieldRange(0, 86400L - 1);
                    case TemporalField::MINUTE_OF_HOUR:
                        return FieldRange(0, 59);
                    case TemporalField::MINUTE_OF_DAY:
                        return FieldRange(0, (24 * 60) - 1);
                    case TemporalField::HOUR_OF_AMPM:
                        return FieldRange(0, 11);
                    case TemporalField::CLOCK_HOUR_OF_AMPM:
                        return FieldRange(1, 12);
                    case TemporalField::HOUR_OF_DAY:
                        return FieldRange(0, 23);
                    case TemporalField::CLOCK_HOUR_OF_DAY:
                        return FieldRange(1, 24);
                    case TemporalField::AMPM_OF_DAY:
                        return FieldRange(0, 1);
                    case TemporalField::DAY_OF_WEEK:
                    case TemporalField::ALIGNED_DAY_OF_WEEK_IN_MONTH:
                    case TemporalField::ALIGNED_DAY_OF_WEEK_IN_YEAR:
                        return FieldRange(1, 7);
                    case TemporalField::DAY_OF_MONTH:
                        return FieldRange(1, 28, 31);
                    case TemporalField::DAY_OF_YEAR:
                        return FieldRange(1, 365, 366);
                    case TemporalField::EPOCH_DAY:
                        return FieldRange(-365243219162L, 365241780471L);
                    case TemporalField::ALIGNED_WEEK_OF_MONTH:
                        return FieldRange(1, 4, 5);
                    case TemporalField::ALIGNED_WEEK_OF_YEAR:
                        return FieldRange(1, 53);
                    case TemporalField::MONTH_OF_YEAR:
                        return FieldRange(1, 12);
                    case TemporalField::PROLEPTIC_MONTH:
                        return FieldRange(-999999999LL * 12, +999999999LL * 12 + 11);
                    case TemporalField::YEAR_OF_ERA:
                        return FieldRange(-999999999LL, +999999999LL + 1);
                    case TemporalField::YEAR:
                        return FieldRange(-999999999LL, +999999999LL);
                    case TemporalField::ERA:
                        return FieldRange(0, 1);
                    case TemporalField::INSTANT_SECONDS:
                        return FieldRange(Long::MIN_VALUE, Long::MAX_VALUE);
                    case TemporalField::OFFSET_SECONDS:
                        return FieldRange(-18LL * 3600, 18LL * 3600);
                    default:
                        UnsupportedTemporalException("Unsupported field: " + FieldName[field])
                                .throws(__trace("core.util.Temporal"));
                }
            }
        }


        glong Temporal::checkValue(Temporal::TemporalField field, glong value) {
            FieldRange const r = range(field);
            if ((r.minSmallest <= value || r.minLargest <= value) && (r.maxSmallest >= value || r.maxLargest >= value))
                return value;
            IllegalArgumentException(
                    "value (" + String::valueOf(value) + ") out of range for field \"" + FieldName[field] + "\"")
                    .throws(__trace("core.time.Temporal"));
        }

        const Object &Temporal::query(const Query &query) const {
            if (query == ZONE_ID || query == PRECISION || query == PRECISION)
                return null;
            try {
                return query.queryFrom(*this);
            } catch (const Exception &ex) { ex.throws(__trace("core.util.Temporal")); }
        }

        Temporal::Query const &Temporal::LOCAL_TIME = []() -> const Query & {
            class LocalTimeQuery : public Query {
            public:
                Object &queryFrom(const Temporal &temporal) const override {
                    if (temporal.supportField(NANO_OF_DAY)) {
                        LocalTime const localTime = LocalTime::ofNanoOfDay(temporal.getLong(NANO_OF_DAY));
                        return Unsafe::allocateInstance<LocalTime>(localTime);
                    }
                    return null;
                }

                String toString() const override {
                    return "Query[LocalTime]";
                }

                Object &clone() const override {
                    return (Object &) *this;
                }

            };
            static const LocalTimeQuery LTQuery = {};
            return LTQuery;
        }();

        Temporal::Query const &Temporal::LOCAL_DATE = []() -> const Query & {
            class LocalDateQuery : public Query {
            public:
                Object &queryFrom(const Temporal &temporal) const override {
                    if (temporal.supportField(EPOCH_DAY)) {
//                        LocalDate const localTime = LocalDate::ofEpochDay(temporal.getLong(EPOCH_DAY));
//                        return Unsafe::allocateInstance<LocalDate>(localDate);
                    }
                    return null;
                }

                String toString() const override {
                    return "Query[LocalTime]";
                }

                Object &clone() const override {
                    return (Object &) *this;
                }

            };
            static const LocalDateQuery LDQuery = {};
            return LDQuery;
        }();

        Temporal::Query const &Temporal::CHRONOLOGY = []() -> const Query & {
            class ChronologyQuery : public Query {
            public:
                const Object &queryFrom(const Temporal &temporal) const override {
                    return temporal.query(CHRONOLOGY);
                }

                String toString() const override {
                    return "Query[Chronology]";
                }

                Object &clone() const override {
                    return (Object &) *this;
                }

            };
            static const ChronologyQuery CQuery = {};
            return CQuery;
        }();

        Temporal::Query const &Temporal::PRECISION = []() -> const Query & {
            class PrecisionQuery : public Query {
            public:
                const Object &queryFrom(const Temporal &temporal) const override {
                    return temporal.query(PRECISION);
                }

                String toString() const override {
                    return "Query[Precision]";
                }

                Object &clone() const override {
                    return (Object &) *this;
                }

            };
            static const PrecisionQuery PQuery = {};
            return PQuery;
        }();

        Temporal::Query const &Temporal::ZONE_ID = []() -> const Query & {
            class ZoneQuery : public Query {
            public:
                const Object &queryFrom(const Temporal &temporal) const override {
                    return temporal.query(ZONE_ID);
                }

                String toString() const override {
                    return "Query[ZoneID]";
                }

                Object &clone() const override {
                    return (Object &) *this;
                }

            };
            static const ZoneQuery ZQuery = {};
            return ZQuery;
        }();

        Temporal::Query const &Temporal::OFFSET = []() -> const Query & {
            class ZoneOffsetQuery : public Query {
            public:
                Object &queryFrom(const Temporal &temporal) const override {
                    if (temporal.supportField(OFFSET_SECONDS)) {
//                        ZoneOffset offset = ZoneOffset::ofTotalSeconds(temporal.get(OFFSET_SECONDS));
//                        return Unsafe::allocateInstance<ZoneOffset>(offset);
                    }
                    return null;
                }

                String toString() const override {
                    return "Query[Offset]";
                }

                Object &clone() const override {
                    return (Object &) *this;
                }

            };
            static const ZoneOffsetQuery ZOQuery = {};
            return ZOQuery;
        }();

        gbool Temporal::supportField(Temporal::Field field) const {
            return false;
        }

        gbool Temporal::supportUnit(Temporal::Unit unit) const {
            return false;
        }

        glong Temporal::getLong(Temporal::TemporalField field) const {
            CORE_IGNORE(field);
            UnsupportedOperationException().throws(__trace("core.time.Temporal"));
        }

        gint Temporal::get(Temporal::TemporalField field) const {
            glong v;
            try {
                v = getLong(field);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Temporal"));
            }
            if (v > Integer::MAX_VALUE || v < Integer::MIN_VALUE) {
                ArithmeticException("The value " + String::valueOf(v) + " exceed int range.");
            }
            return (gint) v;
        }

    } // core
} // time
