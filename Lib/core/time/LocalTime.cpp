//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "LocalTime.h"
#include <core/StringBuffer.h>
#include <core/time/LocalDate.h>
#include <core/time/LocalDateTime.h>
#include <core/private/Unsafe.h>

namespace core {
    namespace time {

        using namespace native;

        namespace {
            String const FieldName[30] = {
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

            String const UnitName[] = {
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

        const LocalTime LocalTime::NOON = LocalTime(12, 0);
        const LocalTime LocalTime::MIDNIGHT = LocalTime(0, 0);
        const LocalTime LocalTime::MIN = LocalTime(0, 0);
        const LocalTime LocalTime::MAX = LocalTime(23, 59, 59, 999999999L);

        gbool LocalTime::supportField(Temporal::Field field) const {
            switch (field) {
                case NANO_OF_SECOND:
                case NANO_OF_DAY:
                case MICRO_OF_SECOND:
                case MICRO_OF_DAY:
                case MILLI_OF_SECOND:
                case MILLI_OF_DAY:
                case SECOND_OF_MINUTE:
                case SECOND_OF_DAY:
                case MINUTE_OF_HOUR:
                case MINUTE_OF_DAY:
                case HOUR_OF_AMPM:
                case CLOCK_HOUR_OF_AMPM:
                case HOUR_OF_DAY:
                case CLOCK_HOUR_OF_DAY:
                case AMPM_OF_DAY:
                    return true;
                default:
                    return false;
            }
        }

        gbool LocalTime::supportUnit(Temporal::Unit unit) const {
            switch (unit) {
                case NANOS:
                case MICROS:
                case MILLIS:
                case SECONDS:
                case MINUTES:
                case HOURS:
                    return true;
                default:
                    return false;
            }
        }

        LocalTime LocalTime::now() {
            return LocalDateTime::now().toLocalTime();
        }

        LocalTime LocalTime::now(const ZoneID &zone) {
            return LocalDateTime::now(zone).toLocalTime();
        }

        LocalTime::LocalTime(gint hour, gint minute) : LocalTime(hour, minute, 0, 0) {}

        LocalTime::LocalTime(gint hour, gint minute, gint second) : LocalTime(hour, minute, second, 0) {}

        LocalTime::LocalTime(gint hour, gint minute, gint second, gint nanoOfSecond) : hhmmssSSSSSSSSS(
                checkValue(HOUR_OF_DAY, hour) << 42 |
                checkValue(MINUTE_OF_HOUR, minute) << 36 |
                checkValue(SECOND_OF_MINUTE, second) << 30 |
                checkValue(NANO_OF_SECOND, nanoOfSecond) << 0) {}

        LocalTime LocalTime::ofSecondOfDay(glong secondOfDay) {
            try {
                checkValue(SECOND_OF_DAY, secondOfDay);
                gint const hour = (gint) (secondOfDay / SECONDS_PER_HOUR);
                secondOfDay %= SECONDS_PER_HOUR;
                gint const minute = (gint) (secondOfDay / SECONDS_PER_MINUTE);
                secondOfDay %= SECONDS_PER_MINUTE;
                gint const second = (gint) secondOfDay;
                return LocalTime(hour, minute, second);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalTime")); }
        }

        LocalTime LocalTime::ofNanoOfDay(glong nanoOfDay) {
            try {
                checkValue(NANO_OF_DAY, nanoOfDay);
                gint const hour = (gint) (nanoOfDay / NANOS_PER_HOUR);
                nanoOfDay %= NANOS_PER_HOUR;
                gint const minute = (gint) (nanoOfDay / NANOS_PER_MINUTE);
                nanoOfDay %= NANOS_PER_MINUTE;
                gint const second = (gint) (nanoOfDay / NANOS_PER_SECOND);
                nanoOfDay %= NANOS_PER_SECOND;
                gint const nano = (gint) nanoOfDay;
                return LocalTime(hour, minute, second, nano);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalTime")); }
        }

        LocalTime LocalTime::parse(const String &text) {
            CORE_IGNORE(text);
            CORE_UNREACHABLE();
        }

        LocalTime LocalTime::parse(const String &text, const DateTimeFormatter &formatter) {
            CORE_IGNORE(text);
            CORE_IGNORE(formatter);
            CORE_UNREACHABLE();
        }

        gint LocalTime::get(Temporal::TemporalField field) const {
            gint const hour = this->hour();
            gint const minute = this->minute();
            gint const second = this->second();
            gint const nano = this->nano();
            switch ((ChronoField) field) {
                case NANO_OF_SECOND :
                    return nano;
                case NANO_OF_DAY :
                    UnsupportedTemporalException("Value of field \"NANO_OF_DAY\" exceed range")
                            .throws(__trace("core.time.LocalTime"));
                case MICRO_OF_SECOND :
                    return nano / 1000;
                case MICRO_OF_DAY :
                    UnsupportedTemporalException("Value of field \"MICRO_OF_DAY\" exceed range")
                            .throws(__trace("core.time.LocalTime"));
                case MILLI_OF_SECOND :
                    return nano / 1000000;
                case MILLI_OF_DAY :
                    return (gint) (toNanoOfDay() / 1000000);
                case SECOND_OF_MINUTE :
                    return second;
                case SECOND_OF_DAY :
                    return toSecondOfDay();
                case MINUTE_OF_HOUR :
                    return minute;
                case MINUTE_OF_DAY :
                    return hour * 60 + minute;
                case HOUR_OF_AMPM :
                    return hour % 12;
                case CLOCK_HOUR_OF_AMPM :
                    return hour % 12 == 0 ? 12 : hour % 12;
                case HOUR_OF_DAY :
                    return hour;
                case CLOCK_HOUR_OF_DAY :
                    return (hour == 0 ? 24 : hour);
                case AMPM_OF_DAY :
                    return hour / 12;
                default :
                    UnsupportedTemporalException("Unsupported field: " + FieldName[field])
                            .throws(__trace("core.time.LocalTime"));
            };
        }

        glong LocalTime::getLong(Temporal::TemporalField field) const {
            if (field == NANO_OF_DAY)
                return toNanoOfDay();
            if (field == MICRO_OF_DAY)
                return toNanoOfDay() / 1000;
            try {
                return get(field);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalTime")); }
        }

        LocalTime LocalTime::with(Temporal::Field field, glong newValue) const {
            gint const hour = this->hour();
            gint const minute = this->minute();
            ChronoField const chronoField = field;
            switch (chronoField) {
                case NANO_OF_SECOND     :
                    return withNano((gint) newValue);
                case NANO_OF_DAY        :
                    return LocalTime::ofNanoOfDay(newValue);
                case MICRO_OF_SECOND    :
                    return withNano((gint) newValue * 1000);
                case MICRO_OF_DAY       :
                    return LocalTime::ofNanoOfDay(newValue * 1000);
                case MILLI_OF_SECOND    :
                    return withNano((gint) newValue * 1000000);
                case MILLI_OF_DAY       :
                    return LocalTime::ofNanoOfDay(newValue * 1000000);
                case SECOND_OF_MINUTE   :
                    return withSecond((gint) newValue);
                case SECOND_OF_DAY      :
                    return afterSeconds(newValue - toSecondOfDay());
                case MINUTE_OF_HOUR     :
                    return withMinute((gint) newValue);
                case MINUTE_OF_DAY      :
                    return afterMinutes(newValue - (hour * 60 + minute));
                case HOUR_OF_AMPM       :
                    return afterHours(newValue - (hour % 12));
                case CLOCK_HOUR_OF_AMPM :
                    return afterHours((newValue == 12 ? 0 : newValue) - (hour % 12));
                case HOUR_OF_DAY        :
                    return withHour((gint) newValue);
                case CLOCK_HOUR_OF_DAY  :
                    return withHour((gint) (newValue == 24 ? 0 : newValue));
                case AMPM_OF_DAY        :
                    return afterHours((newValue - (hour / 12)) * 12);
                default :
                    UnsupportedTemporalException("Unsupported field: " + FieldName[field])
                            .throws(__trace("core.time.LocalTime"));
            };
        }

        LocalTime LocalTime::withHour(gint hour) const {
            try {
                if (this->hour() == hour)
                    return *this;
                return LocalTime((gint) checkValue(HOUR_OF_DAY, hour), minute(), second(), nano());
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalTime")); }
        }

        LocalTime LocalTime::withMinute(gint minute) const {
            try {
                if (this->minute() == minute)
                    return *this;
                return LocalTime(hour(), (gint) checkValue(MINUTE_OF_HOUR, minute), second(), nano());
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalTime")); }
        }

        LocalTime LocalTime::withSecond(gint second) const {
            try {
                if (this->second() == second)
                    return *this;
                return LocalTime(hour(), minute(), (gint) checkValue(SECOND_OF_MINUTE, second), nano());
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalTime")); }
        }

        LocalTime LocalTime::withNano(gint nanoOfSecond) const {
            try {
                if (this->nano() == nanoOfSecond)
                    return *this;
                return LocalTime(hour(), minute(), second(), (gint) checkValue(NANO_OF_SECOND, nanoOfSecond));
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalTime")); }
        }

        LocalTime LocalTime::truncatedTo(Temporal::TemporalUnit unit) const {
            gint const hour = this->hour();
            gint const minute = this->minute();
            gint const second = this->second();
            gint const nano = this->nano();
            switch (unit) {
                case NANOS:
                    return *this;
                case MICROS:
                    return LocalTime(hour, minute, second, (nano / 1000) * 1000);
                case MILLIS:
                    return LocalTime(hour, minute, second, (nano / 1000000) * 1000);
                case SECONDS:
                    return LocalTime(hour, minute, second, 0);
                case MINUTES:
                    return LocalTime(hour, minute, 0, 0);
                case HOURS:
                    return LocalTime(hour, 0, 0, 0);
                case HALF_DAYS:
                    return LocalTime::ofNanoOfDay((toNanoOfDay() / (NANOS_PER_DAY / 2)) * (NANOS_PER_DAY / 2));
                case DAYS:
                    UnsupportedTemporalException("Unit must divide into a standard day without remainder")
                            .throws(__trace("core.time.LocalTime"));
                default:
                    UnsupportedTemporalException("Unit is too large to be used for truncation")
                            .throws(__trace("core.time.LocalTime"));
            }
        }

        LocalTime LocalTime::after(glong amountToAdd, TemporalUnit unit) const {
            switch (unit) {
                case NANOS:
                    return afterNanos(amountToAdd);
                case MICROS:
                    return afterNanos((amountToAdd % MICROS_PER_DAY) * 1000);
                case MILLIS:
                    return afterNanos((amountToAdd % MILLIS_PER_DAY) * 1000000);
                case SECONDS:
                    return afterSeconds(amountToAdd);
                case MINUTES:
                    return afterMinutes(amountToAdd);
                case HOURS:
                    return afterHours(amountToAdd);
                case HALF_DAYS:
                    return afterHours((amountToAdd % 2) * 12);
                default:
                    UnsupportedTemporalException("Unsupported unit: " + UnitName[unit])
                            .throws(__trace("core.time.Temporal"));
            }
        }

        LocalTime LocalTime::afterHours(glong hoursToAdd) const {
            if (hoursToAdd == 0)
                return *this;
            return withHour((gint) (((hoursToAdd % HOURS_PER_DAY) + this->hour() + HOURS_PER_DAY) % HOURS_PER_DAY));
        }

        LocalTime LocalTime::afterMinutes(glong minutesToAdd) const {
            if (minutesToAdd == 0)
                return *this;
            gint const hour = this->hour();
            gint const minute = this->minute();
            gint const second = this->second();
            gint const nano = this->nano();

            gint const oldMin = hour * MINUTES_PER_DAY + minute;
            gint const newMin = (gint) (((minutesToAdd % MINUTES_PER_DAY) + oldMin + MINUTES_PER_DAY) %
                                        MINUTES_PER_DAY);
            if (oldMin == newMin)
                return *this;
            gint const newHour = newMin / MINUTES_PER_HOUR;
            gint const newMinute = newMin % MINUTES_PER_HOUR;
            return LocalTime(newHour, newMinute, second, nano);
        }

        LocalTime LocalTime::afterSeconds(glong secondstoAdd) const {
            if (secondstoAdd == 0)
                return *this;
            gint const hour = this->hour();
            gint const minute = this->minute();
            gint const second = this->second();
            gint const nano = this->nano();

            gint const oldSec = hour * SECONDS_PER_HOUR + minute * SECONDS_PER_MINUTE + second;
            gint const newSec = (gint) (((secondstoAdd % SECONDS_PER_DAY) + oldSec + SECONDS_PER_DAY) %
                                        SECONDS_PER_DAY);
            if (newSec == oldSec)
                return *this;

            gint const newHour = newSec / SECONDS_PER_HOUR;
            gint const newMinute = (newSec / SECONDS_PER_MINUTE) % MINUTES_PER_HOUR;
            gint const newSecond = newSec % SECONDS_PER_MINUTE;
            return LocalTime(newHour, newMinute, newSecond, nano);
        }

        LocalTime LocalTime::afterNanos(glong nanosToAdd) const {
            if (nanosToAdd == 0)
                return *this;
            gint const hour = this->hour();
            gint const minute = this->minute();
            gint const second = this->second();
            gint const nano = this->nano();

            glong const oldN = hour * NANOS_PER_HOUR + minute * NANOS_PER_MINUTE + second * NANOS_PER_SECOND + nano;
            glong const newN = ((nanosToAdd % NANOS_PER_DAY) + oldN + NANOS_PER_DAY) % NANOS_PER_DAY;
            if (oldN == newN)
                return *this;

            gint const newHour = (gint) (newN / NANOS_PER_DAY);
            gint const newMinute = (gint) ((newN / NANOS_PER_MINUTE) % MINUTES_PER_HOUR);
            gint const newSecond = (gint) ((newN / NANOS_PER_SECOND) % SECONDS_PER_MINUTE);
            gint const newNano = (gint) (newN % NANOS_PER_SECOND);

            return LocalTime(newHour, newMinute, newSecond, newNano);
        }

        LocalTime LocalTime::before(glong amountToSubtract, TemporalUnit unit) const {
            return amountToSubtract == Long::MIN_VALUE ?
                   after(Long::MAX_VALUE, unit).after(1, unit) :
                   after(-amountToSubtract, unit);
        }

        LocalTime LocalTime::beforeHours(glong hoursToSubtract) const {
            return afterHours(-(hoursToSubtract % HOURS_PER_DAY));
        }

        LocalTime LocalTime::beforeMinutes(glong minutesToSubtract) const {
            return afterMinutes(-(minutesToSubtract % MINUTES_PER_DAY));
        }

        LocalTime LocalTime::beforeSeconds(glong secondsToSubtract) const {
            return afterSeconds(-(secondsToSubtract % SECONDS_PER_DAY));
        }

        LocalTime LocalTime::beforeNanos(glong nanosToSubtract) const {
            return afterNanos(-(nanosToSubtract % NANOS_PER_DAY));
        }

        glong LocalTime::until(const Temporal &endExclusive, TemporalUnit chronoUnit) const {
            try {
                LocalTime const end = from(endExclusive);
                glong const nanosUntil = end.toNanoOfDay() - toNanoOfDay();  // no overflow
                switch (chronoUnit) {
                    case NANOS     :
                        return nanosUntil;
                    case MICROS    :
                        return nanosUntil / 1000;
                    case MILLIS    :
                        return nanosUntil / 1000000;
                    case SECONDS   :
                        return nanosUntil / NANOS_PER_SECOND;
                    case MINUTES   :
                        return nanosUntil / NANOS_PER_MINUTE;
                    case HOURS     :
                        return nanosUntil / NANOS_PER_HOUR;
                    case HALF_DAYS :
                        return nanosUntil / (12 * NANOS_PER_HOUR);
                    default :
                        break;
                };
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalTime")); }
            UnsupportedTemporalException("Unsupported unit: " + UnitName[chronoUnit])
                    .throws(__trace("core.time.LocalTime"));
        }

        LocalTime LocalTime::from(const Temporal &temporal) {
            if(Class<LocalTime>::hasInstance(temporal))
                return (const LocalTime &) temporal;
            const Object &time = temporal.query(LOCAL_TIME);
            if (time == null) {
                DateTimeException("Unable to obtain LocalTime from Temporal: " + String::valueOf(temporal)
                                  + " of type " + temporal.classname()).throws(__trace("core.time.LocalTime"));
            }
            return (const LocalTime &) time;
        }

        String LocalTime::format(const DateTimeFormatter &formatter) const {
            CORE_IGNORE(formatter);
            return {};
        }

        LocalDateTime LocalTime::atDate(const LocalDate &date) const {
            CORE_IGNORE(date);
            CORE_UNREACHABLE();
        }

        gint LocalTime::toSecondOfDay() const {
            gint const hour = this->hour();
            gint const minute = this->minute();
            gint const second = this->second();
            gint const nano = this->nano();
            return hour * SECONDS_PER_HOUR + minute * SECONDS_PER_MINUTE + second;
        }

        glong LocalTime::toNanoOfDay() const {
            gint const hour = this->hour();
            gint const minute = this->minute();
            gint const second = this->second();
            gint const nano = this->nano();
            return hour * NANOS_PER_DAY + minute * NANOS_PER_MINUTE + second * NANOS_PER_SECOND + nano;
        }

        glong LocalTime::toEpochSecond(const LocalDate &date, const ZoneOffset &offset) const {
            return atDate(date).toEpochSecond(offset);
        }

        gint LocalTime::compareTo(const LocalTime &other) const {
            gint cmp = Integer::compare(hour(), other.hour());
            if (cmp != 0)
                return cmp;
            cmp = Integer::compare(minute(), other.minute());
            if (cmp != 0)
                return cmp;
            cmp = Integer::compare(second(), other.second());
            if (cmp != 0)
                return cmp;
            return Integer::compare(nano(), other.nano());
        }

        gbool LocalTime::isAfter(const LocalTime &other) const {
            return compareTo(other) > 0;
        }

        gbool LocalTime::isBefore(const LocalTime &other) const {
            return compareTo(other) < 0;
        }

        gbool LocalTime::equals(const Object &obj) const {
            if (this == &obj)
                return true;
            return (Class<LocalTime>::hasInstance(obj)) && compareTo((const LocalTime &) obj) == 0;
        }

        gint LocalTime::hash() const {
            return Long::hash(hhmmssSSSSSSSSS);
        }

        String LocalTime::toString() const {
            StringBuffer buf = StringBuffer(18);
            gint hourValue = hour();
            gint minuteValue = minute();
            gint secondValue = second();
            gint nanoValue = nano();
            buf.append(hourValue < 10 ? "0" : "").append(hourValue)
                    .append(minuteValue < 10 ? ":0" : ":").append(minuteValue);
            if (secondValue > 0 || nanoValue > 0) {
                buf.append(secondValue < 10 ? ":0" : ":").append(secondValue);
                if (nanoValue > 0) {
                    buf.append('.');
                    if (nanoValue % 1000000 == 0) {
                        buf.append(Integer::toString((nanoValue / 1000000) + 1000).subString(1));
                    } else if (nanoValue % 1000 == 0) {
                        buf.append(Integer::toString((nanoValue / 1000) + 1000000).subString(1));
                    } else {
                        buf.append(Integer::toString((nanoValue) + 1000000000).subString(1));
                    }
                }
            }
            return buf.toString();
        }

        gint LocalTime::hour() const { return (gint) (hhmmssSSSSSSSSS >> 42) & 0x1F; }

        gint LocalTime::minute() const { return (gint) (hhmmssSSSSSSSSS >> 36) & 0x3F; }

        gint LocalTime::second() const { return (gint) (hhmmssSSSSSSSSS >> 30) & 0x3F; }

        gint LocalTime::nano() const { return (gint) (hhmmssSSSSSSSSS & 0x3FFFFFFF); }


    } // core
} // time
