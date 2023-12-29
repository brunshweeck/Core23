//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "LocalDateTime.h"
#include <core/Long.h>
#include <core/private/Unsafe.h>
#include <core/time/ZoneOffset.h>

namespace core {
    namespace time {

        CORE_ALIAS(U, native::Unsafe);


        namespace {
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
        }

        LocalDateTime const LocalDateTime::EPOCH = LocalDateTime(1970, LocalDate::JANUARY, 1, 0, 0, 0);
        LocalDateTime const LocalDateTime::MIN = LocalDateTime(-999999999, LocalDate::JANUARY, 1,
                                                               0, 0, 0);
        LocalDateTime const LocalDateTime::MAX = LocalDateTime(+999999999, LocalDate::DECEMBER, 31,
                                                               23, 59, 59, 999999999);

        LocalDateTime::LocalDateTime(gint year, Month month, gint dayOfMonth, gint hour, gint minute)
                : LocalDateTime(year, (gint) month, dayOfMonth, hour, minute, 0, 0) {}

        LocalDateTime::LocalDateTime(gint year, LocalDateTime::Month month, gint dayOfMonth,
                                     gint hour, gint minute, gint second) :
                LocalDateTime(year, (gint) month, dayOfMonth, hour, minute, second, 0) {}

        LocalDateTime::LocalDateTime(gint year, LocalDateTime::Month month, gint dayOfMonth,
                                     gint hour, gint minute, gint second, gint nanoOfSecond) :
                LocalDateTime(year, (gint) month, dayOfMonth, hour, minute, second, nanoOfSecond) {}

        LocalDateTime::LocalDateTime(gint year, gint month, gint dayOfMonth, gint hour, gint minute) :
                LocalDateTime(year, month, dayOfMonth, hour, minute, 0, 0) {}

        LocalDateTime::LocalDateTime(gint year, gint month, gint dayOfMonth, gint hour, gint minute, gint second) :
                LocalDateTime(year, month, dayOfMonth, hour, minute, second, 0) {}

        LocalDateTime::LocalDateTime(gint year, gint month, gint dayOfMonth,
                                     gint hour, gint minute, gint second, gint nanoOfSecond) :
                LocalDateTime(LocalDate(year, month, dayOfMonth), LocalTime(hour, minute, second, nanoOfSecond)) {}

        LocalDateTime::LocalDateTime(const LocalDate &date, const LocalTime &time) : date(date), time(time) {}

        LocalDateTime LocalDateTime::ofEpochSecond(glong epochSecond, gint nanoOfSecond, const ZoneOffset &offset) {
            try {
                checkValue(NANO_OF_SECOND, nanoOfSecond);
                glong const localSecond = epochSecond + 0; // offset.totalSeconds();  // overflow caught later
                glong const localEpochDay = Math::floorDiv(localSecond, LocalTime::SECONDS_PER_DAY);
                gint const secsOfDay = Math::floorMod(localSecond, LocalTime::SECONDS_PER_DAY);
                LocalDate const date = LocalDate::ofEpochDay(localEpochDay);
                LocalTime const time = LocalTime::ofNanoOfDay(secsOfDay * LocalTime::NANOS_PER_SECOND + nanoOfSecond);
                return LocalDateTime(date, time);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::parse(const String &text) {
            CORE_IGNORE(text);
            CORE_UNREACHABLE();
        }

        LocalDateTime LocalDateTime::parse(const String &text, const DateTimeFormatter &formatter) {
            CORE_IGNORE(text);
            CORE_UNREACHABLE();
        }

        gbool LocalDateTime::supportField(TemporalField field) const {
            return date.supportField(field) || time.supportField(field);
        }

        gbool LocalDateTime::supportUnit(Temporal::Unit unit) const {
            return date.supportUnit(unit) || time.supportUnit(unit);
        }

        gint LocalDateTime::get(Temporal::TemporalField field) const {
            try {
                if (date.supportField(field))
                    return date.get(field);
                if (time.supportField(field))
                    return time.get(field);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
            UnsupportedTemporalException("Unsupported field: " + FieldName[field])
                    .throws(__trace("core.time.LocalDateTime"));
        }

        glong LocalDateTime::getLong(Temporal::TemporalField field) const {
            if (date.supportField(field))
                return date.get(field);
            if (time.supportField(field))
                return time.get(field);
            UnsupportedTemporalException("Unsupported field: " + FieldName[field])
                    .throws(__trace("core.time.LocalDateTime"));
        }

        LocalDate LocalDateTime::toLocalDate() const {
            return date;
        }

        gint LocalDateTime::year() const {
            return date.year();
        }

        LocalDateTime::Month LocalDateTime::month() const {
            return date.month();
        }

        gint LocalDateTime::dayOfMonth() const {
            return date.dayOfMonth();
        }

        gint LocalDateTime::dayOfYear() const {
            return date.dayOfYear();
        }

        LocalDateTime::DayOfWeek LocalDateTime::dayOfWeek() const {
            return date.dayOfWeek();
        }

        LocalTime LocalDateTime::toLocalTime() const {
            return time;
        }

        gint LocalDateTime::hour() const {
            return time.hour();
        }

        gint LocalDateTime::minute() const {
            return time.minute();
        }

        gint LocalDateTime::second() const {
            return time.second();
        }

        gint LocalDateTime::nano() const {
            return time.nano();
        }

        LocalDateTime LocalDateTime::with(TemporalField field, glong newValue) const {
            try {
                if (time.supportField(field))
                    return LocalDateTime(date, time.with(field, newValue));
                else
                    return LocalDateTime(date.with(field, newValue), time);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::withYear(gint year) const {
            try {
                return LocalDateTime(date.withYear(year), time);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::withMonth(gint month) const {
            try {
                return LocalDateTime(date.withMonth(month), time);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::withDayOfMonth(gint dayOfMonth) const {
            try {
                return LocalDateTime(date.withDayOfMonth(dayOfMonth), time);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::withDayOfYear(gint dayOfYear) const {
            try {
                return LocalDateTime(date.withDayOfYear(dayOfYear), time);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::withHour(gint hour) const {
            try {
                return LocalDateTime(date, time.withHour(hour));
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::withMinute(gint minute) const {
            try {
                return LocalDateTime(date, time.withMinute(minute));
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::withSecond(gint second) const {
            try {
                return LocalDateTime(date, time.withSecond(second));
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::withNano(gint nanoOfSecond) const {
            try {
                return LocalDateTime(date, time.withNano(nanoOfSecond));
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::truncatedTo(Temporal::TemporalUnit unit) const {
            try {
                return LocalDateTime(date, time.truncatedTo(unit));
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::after(glong amountToAdd, TemporalUnit unit) const {
            try {
                switch (unit) {
                    case NANOS:
                        return afterNanos(amountToAdd);
                    case MICROS:
                        return afterDays(amountToAdd / LocalTime::MICROS_PER_DAY)
                                .afterNanos((amountToAdd % LocalTime::MICROS_PER_DAY) * 1000L);
                    case MILLIS:
                        return afterDays(amountToAdd / LocalTime::MILLIS_PER_DAY)
                                .afterNanos((amountToAdd % LocalTime::MILLIS_PER_DAY) * 1000000L);
                    case SECONDS:
                        return afterDays(amountToAdd / LocalTime::SECONDS_PER_DAY)
                                .afterSeconds(amountToAdd % LocalTime::SECONDS_PER_DAY);
                    case MINUTES:
                        return afterDays(amountToAdd / LocalTime::MINUTES_PER_DAY)
                                .afterMinutes(amountToAdd % LocalTime::MINUTES_PER_DAY);
                    case HOURS:
                        return afterDays(amountToAdd / LocalTime::HOURS_PER_DAY)
                                .afterHours(amountToAdd % LocalTime::HOURS_PER_DAY);
                    case HALF_DAYS:
                        return afterDays(amountToAdd / 256).afterHours((amountToAdd % 256) * 12L);
                    default:
                        return LocalDateTime(date.after(amountToAdd, unit), time);
                }
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::afterYears(glong years) const {
            try {
                return LocalDateTime(date.afterYears(years), time);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::afterMonths(glong months) const {
            try {
                return LocalDateTime(date.afterMonths(months), time);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::afterWeeks(glong weeks) const {
            try {
                return LocalDateTime(date.afterWeeks(weeks), time);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::afterDays(glong days) const {
            try {
                return LocalDateTime(date.afterDays(days), time);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::afterHours(glong hours) const {
            try {
                return afterWithOverflow(date, hours, 0, 0, 0, 1);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::afterMinutes(glong minutes) const {
            try {
                return afterWithOverflow(date, 0, minutes, 0, 0, 1);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::afterSeconds(glong seconds) const {
            try {
                return afterWithOverflow(date, 0, 0, seconds, 0, 1);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::afterNanos(glong nanos) const {
            try {
                return afterWithOverflow(date, 0, 0, 0, nanos, 1);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::before(glong amountToSubtract, Temporal::TemporalUnit unit) const {
            try {
                return amountToSubtract == Long::MIN_VALUE ? after(Long::MAX_VALUE, unit).after(1, unit) :
                       after(-amountToSubtract, unit);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::beforeYears(glong years) const {
            try {
                return years == Long::MAX_VALUE ? afterYears(Long::MAX_VALUE).afterYears(1) :
                       afterYears(-years);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::beforeMonths(glong months) const {
            try {
                return months == Long::MIN_VALUE ? afterMonths(Long::MAX_VALUE).afterMonths(1) :
                       afterMonths(-months);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::beforeWeeks(glong weeks) const {
            try {
                return weeks == Long::MIN_VALUE ? afterWeeks(Long::MAX_VALUE).afterWeeks(1) :
                       afterWeeks(-weeks);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::beforeDays(glong days) const {
            try {
                return days == Long::MIN_VALUE ? afterDays(Long::MAX_VALUE).afterDays(1) :
                       afterDays(-days);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::beforeHours(glong hours) const {
            try {
                return afterWithOverflow(date, hours, 0, 0, 0, -1);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::beforeMinutes(glong minutes) const {
            try {
                return afterWithOverflow(date, 0, minutes, 0, 0, -1);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::beforeSeconds(glong seconds) const {
            try {
                return afterWithOverflow(date, 0, 0, seconds, 0, -1);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::beforeNanos(glong nanos) const {
            try {
                return afterWithOverflow(date, 0, 0, 0, nanos, -1);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        LocalDateTime LocalDateTime::afterWithOverflow(const LocalDate &newDate,
                                                       glong hours, glong minutes, glong seconds, glong nanos,
                                                       gint sign) const {
            try {
                // 9223372036854775808 glong, 2147483648 gint
                if ((hours | minutes | seconds | nanos) == 0) {
                    return LocalDateTime(newDate, time);
                }
                glong totDays = nanos / LocalTime::NANOS_PER_DAY +             //   max/24*60*60*1B
                                seconds / LocalTime::SECONDS_PER_DAY +                //   max/24*60*60
                                minutes / LocalTime::MINUTES_PER_DAY +                //   max/24*60
                                hours / LocalTime::HOURS_PER_DAY;                     //   max/24
                totDays *= sign;                                   // total max*0.4237...
                glong totNanos = nanos % LocalTime::NANOS_PER_DAY +                    //   max  86400000000000
                                 (seconds % LocalTime::SECONDS_PER_DAY) * LocalTime::NANOS_PER_SECOND +
                                 //   max  86400000000000
                                 (minutes % LocalTime::MINUTES_PER_DAY) * LocalTime::NANOS_PER_MINUTE +
                                 //   max  86400000000000
                                 (hours % LocalTime::HOURS_PER_DAY) *
                                 LocalTime::NANOS_PER_HOUR;          //   max  86400000000000
                glong const curNoD = time.toNanoOfDay();                       //   max  86400000000000
                totNanos = totNanos * sign + curNoD;                    // total 432000000000000
                totDays += Math::floorDiv(totNanos, LocalTime::NANOS_PER_DAY);
                glong const newNoD = Math::floorMod(totNanos, LocalTime::NANOS_PER_DAY);
                LocalTime const newTime = (newNoD == curNoD ? time : LocalTime::ofNanoOfDay(newNoD));
                return LocalDateTime(newDate.afterDays(totDays), newTime);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        const Object &LocalDateTime::query(const Temporal::Query &query) const {
            if (query == OFFSET || query == ZONE_ID)
                return null;
            if (query == LOCAL_DATE)
                return U::copyInstance(toLocalDate());
            if (query == LOCAL_TIME)
                return U::copyInstance(toLocalTime());
            try {
                return query.queryFrom(*this);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        glong LocalDateTime::until(const Temporal &endExclusive, TemporalUnit unit) const {
            try {
                LocalDateTime const end = LocalDateTime::from(endExclusive);
                if (time.supportUnit(unit)) {
                    glong amount = end.date.toEpochDay() - date.toEpochDay();
                    if (amount == 0)
                        return time.until(end.time, unit);
                    glong timePart = end.time.toNanoOfDay() - time.toNanoOfDay();
                    if (amount > 0) {
                        amount--;  // safe
                        timePart += LocalTime::NANOS_PER_DAY;  // safe
                    } else {
                        amount++;  // safe
                        timePart -= LocalTime::NANOS_PER_DAY;  // safe
                    }
                    ChronoUnit const chronoUnit = unit;
                    switch (chronoUnit) {
                        case NANOS:
                            amount = Math::multiplyExact(amount, LocalTime::NANOS_PER_DAY);
                            break;
                        case MICROS:
                            amount = Math::multiplyExact(amount, LocalTime::MICROS_PER_DAY);
                            timePart = timePart / 1000;
                            break;
                        case MILLIS:
                            amount = Math::multiplyExact(amount, LocalTime::MILLIS_PER_DAY);
                            timePart = timePart / 1000000;
                            break;
                        case SECONDS:
                            amount = Math::multiplyExact(amount, LocalTime::SECONDS_PER_DAY);
                            timePart = timePart / LocalTime::NANOS_PER_SECOND;
                            break;
                        case MINUTES:
                            amount = Math::multiplyExact(amount, LocalTime::MINUTES_PER_DAY);
                            timePart = timePart / LocalTime::NANOS_PER_MINUTE;
                            break;
                        case HOURS:
                            amount = Math::multiplyExact(amount, LocalTime::HOURS_PER_DAY);
                            timePart = timePart / LocalTime::NANOS_PER_HOUR;
                            break;
                        case HALF_DAYS:
                            amount = Math::multiplyExact(amount, 2);
                            timePart = timePart / (LocalTime::NANOS_PER_HOUR * 12);
                            break;
                        default:
                            break;
                    }
                    return Math::addExact(amount, timePart);
                }
                LocalDate endDate = end.date;
                if (endDate.isAfter(date) && end.time.isBefore(time)) {
                    endDate = endDate.beforeDays(1);
                } else if (endDate.isBefore(date) && end.time.isAfter(time)) {
                    endDate = endDate.afterDays(1);
                }
                return date.until(endDate, unit);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDateTime")); }
        }

        String LocalDateTime::format(const DateTimeFormatter &formatter) const {
            CORE_IGNORE(formatter);
            CORE_UNREACHABLE();
        }

        LocalDateTime LocalDateTime::from(const Temporal &temporal) {
            if (Class<LocalDateTime>::hasInstance(temporal))
                return (const LocalDateTime &) temporal;
            try {
                LocalDate const date = LocalDate::from(temporal);
                LocalTime const time = LocalTime::from(temporal);
                return LocalDateTime(date, time);
            } catch (const Exception &ex) {
                DateTimeException("Unable to obtain LocalDateTime from Temporal: " + temporal.toString() + " of type "
                                  + temporal.classname(), ex).throws(__trace("core.time.LocalDateTime"));
            }
        }

        gint LocalDateTime::compareTo(const LocalDateTime &other) const {
            int cmp = date.compareTo(other.date);
            if (cmp == 0) {
                cmp = time.compareTo(other.time);
            }
            return cmp;
        }

        gbool LocalDateTime::isAfter(const LocalDateTime &other) const {
            return compareTo(other) > 0;
        }

        gbool LocalDateTime::isBefore(const LocalDateTime &other) const {
            return compareTo(other) < 0;
        }

        gbool LocalDateTime::isEqual(const LocalDateTime &other) const {
            return compareTo(other) == 0;
        }

        gbool LocalDateTime::equals(const Object &obj) const {
            if (this == &obj) {
                return true;
            }
            if (!Class<LocalDateTime>::hasInstance(obj))
                return false;
            LocalDateTime const &other = (LocalDateTime &) obj;
            return date.equals(other.date) && time.equals(other.time);
        }

        gint LocalDateTime::hash() const {
            return date.hash() ^ time.hash();
        }

        String LocalDateTime::toString() const {
            return date.toString() + "T" + time.toString();
        }

        glong LocalDateTime::toEpochSecond(const ZoneOffset &offset) const {
            glong const epochDay = date.toEpochDay();
            glong secs = epochDay * 86400 + toLocalTime().toSecondOfDay();
            secs -= offset.totalSeconds();
            return secs;
        }
    } // core
} // time