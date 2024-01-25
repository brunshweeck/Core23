//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "LocalDate.h"
#include <core/time/LocalTime.h>
#include <core/time/LocalDateTime.h>
#include <core/Long.h>
#include <core/Integer.h>
#include <core/StringBuffer.h>
#include <core/private/Unsafe.h>
#include <core/private/Null.h>

namespace core {
    namespace time {

        CORE_ALIAS(U, native::Unsafe);

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

        namespace {
            glong signBit(gint val) { return val >= 0 ? 0 : 1; }

            gbool isISOLeapYear(gint prolepticYear) {
                return ((prolepticYear & 3) == 0) && ((prolepticYear % 100) != 0 || (prolepticYear % 400) == 0);
            }

            gint firstDayOfYear(gint month, gbool leapYear) {
                gint const leap = leapYear ? 1 : 0;
                switch ((LocalDate::Month) month) {
                    case LocalDate::JANUARY   :
                        return 1;
                    case LocalDate::FEBRUARY  :
                        return 32;
                    case LocalDate::MARCH     :
                        return 60 + leap;
                    case LocalDate::APRIL     :
                        return 91 + leap;
                    case LocalDate::MAY       :
                        return 121 + leap;
                    case LocalDate::JUNE      :
                        return 152 + leap;
                    case LocalDate::JULY      :
                        return 182 + leap;
                    case LocalDate::AUGUST    :
                        return 213 + leap;
                    case LocalDate::SEPTEMBER :
                        return 244 + leap;
                    case LocalDate::OCTOBER   :
                        return 274 + leap;
                    case LocalDate::NOVEMBER  :
                        return 305 + leap;
                    case LocalDate::DECEMBER:
                    default :
                        return 335 + leap;
                };
            }

            gint numberOfDaysPerMonth(gint month, gbool leapYear) {
                switch ((LocalDate::Month) month) {
                    case LocalDate::JANUARY:
                    case LocalDate::MARCH:
                    case LocalDate::MAY:
                    case LocalDate::JULY:
                    case LocalDate::AUGUST:
                    case LocalDate::OCTOBER:
                    case LocalDate::DECEMBER:
                        return 31;
                    case LocalDate::FEBRUARY:
                        return leapYear ? 29 : 28;
                    case LocalDate::APRIL:
                    case LocalDate::JUNE:
                    case LocalDate::SEPTEMBER:
                    case LocalDate::NOVEMBER:
                    default:
                        return 30;
                }
            }

            String MonthName[13] = {
                    "",
                    "JANUARY",
                    "FEBRUARY",
                    "MARCH",
                    "APRIL",
                    "MAY",
                    "JUNE",
                    "JULY",
                    "AUGUST",
                    "SEPTEMBER",
                    "OCTOBER",
                    "NOVEMBER",
                    "DECEMBER",
            };

            glong daysUntil(const LocalDate &start, const LocalDate &end) {
                return end.toEpochDay() - start.toEpochDay();
            }

            glong monthsUntil(const LocalDate &start, const LocalDate &end) {
                glong const m1 = start.getLong(Temporal::PROLEPTIC_MONTH) * 32 + start.dayOfMonth();
                glong const m2 = end.getLong(Temporal::PROLEPTIC_MONTH) * 32 + end.dayOfMonth();
                return m2 - m1;
            }
        }

        LocalDate const LocalDate::EPOCH = LocalDate(1970, JANUARY, 1);
        LocalDate const LocalDate::MIN = LocalDate(-999999999, JANUARY, 1);
        LocalDate const LocalDate::MAX = LocalDate(+999999999, DECEMBER, 31);

        LocalDate LocalDate::now() {
            return LocalDateTime::now().toLocalDate();
        }

        LocalDate LocalDate::now(const ZoneID &zone) {
            return LocalDateTime::now(zone).toLocalDate();
        }

        LocalDate::LocalDate(gint year, Month month, gint dayOfMonth) :
                LocalDate(year, (gint) month, dayOfMonth) {}

        LocalDate::LocalDate(gint year, gint month, gint dayOfMonth) : yyyyMMdd(
                signBit(year) << 39 |
                checkValue(YEAR, year) << 9 |
                checkValue(MONTH_OF_YEAR, month) << 5 |
                checkValue(DAY_OF_MONTH, dayOfMonth)) {
            gbool const isLeap = isISOLeapYear(year);
            gint const daysPerMonth = numberOfDaysPerMonth(month, isLeap);
            if (dayOfMonth <= daysPerMonth)
                return;
            if (dayOfMonth == 29)
                DateTimeException("Invalid date 'February 29' as '" + String::valueOf(year) + "' is not a leap year")
                        .throws(__trace("core.time.LocalDate"));
            else
                DateTimeException("Invalid date '" + MonthName[month] + " " + String::valueOf(dayOfMonth) + "'")
                        .throws(__trace("core.time.LocalDate"));
        }

        LocalDate::LocalDate(gint year, gint dayOfYear) : yyyyMMdd(signBit(year) << 41 | checkValue(YEAR, year) << 11) {
            checkValue(DAY_OF_YEAR, dayOfYear);
            gbool isLeap = isISOLeapYear(year);
            if (dayOfYear == 366 && !isLeap) {
                DateTimeException("Invalid date 'DayOfYear 366' as '" + String::valueOf(year) + "' is not a leap year")
                        .throws(__trace("core.time.LocalDate"));
            }
            gint monthOfYear = (dayOfYear - 1) / 31 + 1;
            gint const endOfMonth = firstDayOfYear(monthOfYear, isLeap) + numberOfDaysPerMonth(monthOfYear, isLeap) - 1;
            if (dayOfYear > endOfMonth)
                monthOfYear += 1;
            gint const dayOfMonth = dayOfYear - firstDayOfYear(monthOfYear, isLeap) + 1;
            yyyyMMdd |= monthOfYear << 5 | dayOfMonth;
        }

        LocalDate LocalDate::ofEpochDay(glong epochDay) {
            try {
                checkValue(EPOCH_DAY, epochDay);
                glong zeroDay = epochDay + DAYS_0000_TO_1970;
                // find the march-based year
                zeroDay -= 60;  // adjust to 0000-03-01 so leap day is at end of four year cycle
                glong adjust = 0;
                if (zeroDay < 0) {
                    // adjust negative years to positive for calculation
                    glong const adjustCycles = (zeroDay + 1) / DAYS_PER_CYCLE - 1;
                    adjust = adjustCycles * 400;
                    zeroDay += -adjustCycles * DAYS_PER_CYCLE;
                }
                glong yearEst = (400 * zeroDay + 591) / DAYS_PER_CYCLE;
                glong doyEst = zeroDay - (365 * yearEst + yearEst / 4 - yearEst / 100 + yearEst / 400);
                if (doyEst < 0) {
                    // fix estimate
                    yearEst--;
                    doyEst = zeroDay - (365 * yearEst + yearEst / 4 - yearEst / 100 + yearEst / 400);
                }
                yearEst += adjust;  // reset any negative year
                gint const marchDoy0 = (gint) doyEst;

                // convert march-based values back to january-based
                gint const marchMonth0 = (marchDoy0 * 5 + 2) / 153;
                gint month = marchMonth0 + 3;
                if (month > 12) {
                    month -= 12;
                }
                gint const dom = marchDoy0 - (marchMonth0 * 306 + 5) / 10 + 1;
                if (marchDoy0 >= 306) {
                    yearEst++;
                }

                return LocalDate((gint) yearEst, month, dom);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
        }

        LocalDate LocalDate::parse(const String &text) {
            CORE_IGNORE(text);
            CORE_UNREACHABLE();
        }

        LocalDate LocalDate::parse(const String &text, const DateTimeFormatter &formatter) {
            CORE_IGNORE(text);
            CORE_IGNORE(formatter);
            CORE_UNREACHABLE();
        }

        gbool LocalDate::supportField(Temporal::TemporalField field) const {
            return field >= DAY_OF_WEEK && field <= ERA;
        }

        gbool LocalDate::supportUnit(Temporal::TemporalUnit unit) const {
            return unit >= DAYS && unit != FOREVER;
        }

        gint LocalDate::get(TemporalField field) const {
            switch ((ChronoField) field) {
                case DAY_OF_WEEK :
                    return dayOfWeek();
                case ALIGNED_DAY_OF_WEEK_IN_MONTH :
                    return ((dayOfMonth() - 1) % 7) + 1;
                case ALIGNED_DAY_OF_WEEK_IN_YEAR :
                    return ((dayOfYear() - 1) % 7) + 1;
                case DAY_OF_MONTH :
                    return dayOfMonth();
                case DAY_OF_YEAR :
                    return dayOfYear();
                case EPOCH_DAY :
                    UnsupportedTemporalException("Value of field 'EPOCH_DAY' exceed range")
                            .throws(__trace("core.time.LocalDate"));
                case ALIGNED_WEEK_OF_MONTH :
                    return ((dayOfMonth() - 1) / 7) + 1;
                case ALIGNED_WEEK_OF_YEAR :
                    return ((dayOfYear() - 1) / 7) + 1;
                case MONTH_OF_YEAR :
                    return month();
                case PROLEPTIC_MONTH :
                    UnsupportedTemporalException("Value of field 'ProlepticMonth' exceed range")
                            .throws(__trace("core.time.LocalDate"));
                case YEAR_OF_ERA :
                    return (year() >= 1 ? year() : 1 - year());
                case YEAR :
                    return year();
                case ERA :
                    return (year() >= 1 ? 1 : 0);
                default :
                    UnsupportedTemporalException("Unsupported field: " + FieldName[field])
                            .throws(__trace("core.time.LocalDate"));
            };
        }

        glong LocalDate::getLong(TemporalField field) const {
            if (field == PROLEPTIC_MONTH)
                return year() * 12 + month() - 1;
            if (field == EPOCH_DAY)
                return toEpochDay();
            try {
                return get(field);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
        }

        gint LocalDate::dayOfYear() const { return firstDayOfYear(month(), isLeapYear()) + dayOfMonth() - 1; }

        LocalDate::DayOfWeek LocalDate::dayOfWeek() const {
            return (DayOfWeek) (Math::floorDiv(toEpochDay() + 3, 7) + 1);
        }

        gbool LocalDate::isLeapYear() const {
            return isISOLeapYear(year());
        }

        gint LocalDate::daysPerMonth() const {
            return numberOfDaysPerMonth(month(), isLeapYear());
        }

        gint LocalDate::daysPerYear() const {
            return isLeapYear() ? 366 : 365;
        }

        LocalDate LocalDate::with(Temporal::TemporalField field, glong newValue) const {
            try {
                checkValue(field, newValue);
                switch (field) {
                    case DAY_OF_WEEK :
                        return afterDays(newValue - dayOfWeek());
                    case ALIGNED_DAY_OF_WEEK_IN_MONTH :
                        return afterDays(newValue - getLong(ALIGNED_DAY_OF_WEEK_IN_MONTH));
                    case ALIGNED_DAY_OF_WEEK_IN_YEAR :
                        return afterDays(newValue - getLong(ALIGNED_DAY_OF_WEEK_IN_YEAR));
                    case DAY_OF_MONTH :
                        return withDayOfMonth((gint) newValue);
                    case DAY_OF_YEAR :
                        return withDayOfYear((gint) newValue);
                    case EPOCH_DAY :
                        return LocalDate::ofEpochDay(newValue);
                    case ALIGNED_WEEK_OF_MONTH :
                        return afterWeeks(newValue - getLong(ALIGNED_WEEK_OF_MONTH));
                    case ALIGNED_WEEK_OF_YEAR :
                        return afterWeeks(newValue - getLong(ALIGNED_WEEK_OF_YEAR));
                    case MONTH_OF_YEAR :
                        return withMonth((gint) newValue);
                    case PROLEPTIC_MONTH :
                        return afterMonths(newValue - getLong(PROLEPTIC_MONTH));
                    case YEAR_OF_ERA :
                        return withYear((gint) (year() >= 1 ? newValue : 1 - newValue));
                    case YEAR :
                        return withYear((gint) newValue);
                    case ERA :
                        return (getLong(ERA) == newValue ? *this : withYear(1 - year()));
                    default :
                        break;
                };
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
            UnsupportedTemporalException("Unsupported field: " + FieldName[field])
                    .throws(__trace("core.time.LocalDate"));
        }

        LocalDate LocalDate::withYear(gint year) const {
            try {
                return LocalDate((gint) checkValue(YEAR, year), month(), dayOfMonth());
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
        }

        LocalDate LocalDate::withMonth(gint month) const {
            try {
                return LocalDate(year(), (gint) checkValue(MONTH_OF_YEAR, month), dayOfMonth());
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
        }

        LocalDate LocalDate::withDayOfMonth(gint dayOfMonth) const {
            try {
                return LocalDate(year(), month(), checkValue(DAY_OF_MONTH, dayOfMonth));
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
        }

        LocalDate LocalDate::withDayOfYear(gint dayOfYear) const {
            try {
                return LocalDate(year(), (gint) checkValue(DAY_OF_YEAR, dayOfYear));
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
        }

        LocalDate LocalDate::after(glong amountToAdd, Temporal::TemporalUnit unit) const {
            try {
                ChronoUnit const chronoUnit = unit;
                switch (chronoUnit) {
                    case DAYS      :
                        return afterDays(amountToAdd);
                    case WEEKS     :
                        return afterWeeks(amountToAdd);
                    case MONTHS    :
                        return afterMonths(amountToAdd);
                    case YEARS     :
                        return afterYears(amountToAdd);
                    case DECADES   :
                        return afterYears(Math::multiplyExact(amountToAdd, 10));
                    case CENTURIES :
                        return afterYears(Math::multiplyExact(amountToAdd, 100));
                    case MILLENNIA :
                        return afterYears(Math::multiplyExact(amountToAdd, 1000));
                    case ERAS      :
                        return with(ERA, Math::addExact(getLong(ERA), amountToAdd));
                    default :
                        break;
                };
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
            UnsupportedTemporalException("Unsupported unit: " + UnitName[unit])
                    .throws(__trace("core.time.LocalDate"));
        }

        LocalDate LocalDate::afterYears(glong yearsToAdd) const {
            if (yearsToAdd == 0)
                return *this;
            try {
                gint const newYear = (gint) checkValue(YEAR, yearsToAdd + year());
                return LocalDate(newYear, month(), dayOfMonth());
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
        }

        LocalDate LocalDate::afterMonths(glong monthsToAdd) const {
            if (monthsToAdd == 0)
                return *this;
            try {
                glong const oldMonths = year() * 12 + month();
                glong const calcMonths = oldMonths + monthsToAdd;
                gint const newYear = (gint) checkValue(YEAR, Math::floorDiv(calcMonths, 12));
                gint const newMonth = (gint) checkValue(MONTH_OF_YEAR, Math::floorMod(calcMonths, 12));
                return LocalDate(newYear, newMonth, dayOfMonth());
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
        }

        LocalDate LocalDate::afterWeeks(glong weeksToAdd) const {
            if (weeksToAdd == 0)
                return *this;
            try {
                return afterDays(Math::multiplyExact(weeksToAdd, 7));
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
        }

        LocalDate LocalDate::afterDays(glong daysToAdd) const {
            if (daysToAdd == 0)
                return *this;
            try {
                glong const dom = dayOfMonth() + daysToAdd;
                if (dom > 0) {
                    if (dom <= 28) {
                        return LocalDate(year(), month(), (gint) dom);
                    } else if (dom <= 59) { // 59th Jan is 28th Feb, 59th Feb is 31st Mar
                        glong const monthLen = daysPerMonth();
                        if (dom <= monthLen) {
                            return LocalDate(year(), month(), (gint) dom);
                        } else if (month() < 12) {
                            return LocalDate(year(), month() + 1, (gint) (dom - monthLen));
                        } else {
                            checkValue(YEAR, year() + 1);
                            return LocalDate(year() + 1, 1, (gint) (dom - monthLen));
                        }
                    }
                }

                glong const mjDay = Math::addExact(toEpochDay(), daysToAdd);
                return LocalDate::ofEpochDay(mjDay);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
        }

        LocalDate LocalDate::before(glong amountToSubtract, Temporal::TemporalUnit unit) const {
            try {
                return (amountToSubtract == Long::MIN_VALUE ?
                        after(Long::MAX_VALUE, unit).after(1, unit) : after(-amountToSubtract, unit));
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
        }

        LocalDate LocalDate::beforeYears(glong yearsToSubtract) const {
            try {
                return (yearsToSubtract == Long::MIN_VALUE ?
                        afterYears(Long::MAX_VALUE).afterYears(1) : afterYears(-yearsToSubtract));
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
        }

        LocalDate LocalDate::beforeMonths(glong monthsToSubtract) const {
            try {
                return (monthsToSubtract == Long::MIN_VALUE ?
                        afterMonths(Long::MAX_VALUE).afterMonths(1) : afterMonths(-monthsToSubtract));
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
        }

        LocalDate LocalDate::beforeWeeks(glong weeksToSubtract) const {
            try {
                return (weeksToSubtract == Long::MIN_VALUE ?
                        afterWeeks(Long::MAX_VALUE).afterWeeks(1) : afterWeeks(-weeksToSubtract));
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
        }

        LocalDate LocalDate::beforeDays(glong daysToSubtract) const {
            try {
                return (daysToSubtract == Long::MIN_VALUE ?
                        afterDays(Long::MAX_VALUE).afterDays(1) : afterDays(-daysToSubtract));
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
        }

        const Object &LocalDate::query(const Temporal::Query &query) const {
            if (query == ZONE_ID || query == OFFSET)
                return null;
            if (query == LOCAL_DATE)
                return Unsafe::copyInstance(*this, true);
            return query.queryFrom(*this);
        }

        glong LocalDate::until(const Temporal &endExclusive, Temporal::TemporalUnit unit) const {
            Object const &result = endExclusive.query(LOCAL_DATE);
            if (result == null)
                DateTimeException("Unable to obtain LocalDate from TemporalAccessor: " + endExclusive.toString()
                                  + " of type " + endExclusive.classname()).throws(__trace("core.time.LocalDate"));
            ChronoUnit const chronoUnit = unit;
            LocalDate const &end = (LocalDate &) result;
            try {
                switch (chronoUnit) {
                    case DAYS      :
                        return daysUntil(*this, end);
                    case WEEKS     :
                        return daysUntil(*this, end) / 7;
                    case MONTHS    :
                        return monthsUntil(*this, end);
                    case YEARS     :
                        return monthsUntil(*this, end) / 12;
                    case DECADES   :
                        return monthsUntil(*this, end) / 120;
                    case CENTURIES :
                        return monthsUntil(*this, end) / 1200;
                    case MILLENNIA :
                        return monthsUntil(*this, end) / 12000;
                    case ERAS      :
                        return end.getLong(ERA) - getLong(ERA);
                    default :
                        break;
                };
            } catch (const Exception &ex) { ex.throws(__trace("core.time.LocalDate")); }
            UnsupportedTemporalException("Unsupported unit: " + UnitName[unit])
                    .throws(__trace("core.time.LocalDate"));
        }

        String LocalDate::format(const DateTimeFormatter &formatter) {
            CORE_IGNORE(formatter);
            CORE_UNREACHABLE();
        }

        glong LocalDate::toEpochDay() const {
            glong const y = year();
            glong const m = month();
            glong total = 0;
            total += 365 * y;
            if (y >= 0) {
                total += (y + 3) / 4 - (y + 99) / 100 + (y + 399) / 400;
            } else {
                total -= y / -4 - y / -100 + y / -400;
            }
            total += ((367 * m - 362) / 12);
            total += dayOfMonth() - 1;
            if (m > 2) {
                total--;
                if (!isLeapYear()) {
                    total--;
                }
            }
            return total - DAYS_0000_TO_1970;
        }

        gint LocalDate::compareTo(const LocalDate &other) const {
            gint cmp = (year() - other.year());
            if (cmp == 0) {
                cmp = (month() - other.month());
                if (cmp == 0) {
                    cmp = (dayOfMonth() - other.dayOfMonth());
                }
            }
            return cmp;
        }

        gbool LocalDate::isAfter(const LocalDate &other) const {
            return compareTo(other) > 0;
        }

        gbool LocalDate::isBefore(const LocalDate &other) const {
            return compareTo(other) < 0;
        }

        gbool LocalDate::equals(const Object &obj) const {
            if (this == &obj)
                return true;
            return (Class<LocalDate>::hasInstance(obj)) && compareTo((LocalDate &) obj) == 0;
        }

        gint LocalDate::hash() const {
            return Long::hash(yyyyMMdd);
        }

        String LocalDate::toString() const {
            gint yearValue = year();
            gint const monthValue = month();
            gint const dayValue = dayOfMonth();
            gint absYear = Math::abs(yearValue);
            StringBuffer buf = StringBuffer(10);
            if (absYear < 1000) {
                if (yearValue < 0) {
                    buf.append(yearValue - 10000).removeAt(1);
                } else {
                    buf.append(yearValue + 10000).removeAt(0);
                }
            } else {
                if (yearValue > 9999) {
                    buf.append('+');
                }
                buf.append(yearValue);
            }
            return buf.append(monthValue < 10 ? "-0" : "-")
                    .append(monthValue)
                    .append(dayValue < 10 ? "-0" : "-")
                    .append(dayValue)
                    .toString();
        }

        LocalDate LocalDate::from(const Temporal &temporal) {
            if (Class<LocalDate>::hasInstance(temporal))
                return (const LocalDate &) temporal;
            Object const &obj = temporal.query(LOCAL_DATE);
            if (obj == null)
                DateTimeException("Unable to obtain LocalDate from TemporalAccessor: " + temporal.toString()
                                  + " of type " + temporal.classname()).throws(__trace("core.time.LocalDate"));
            return (const LocalDate &) obj;
        }

        gint LocalDate::year() const {
            return (gint) (((yyyyMMdd >> 39) == 1 ? -1 : 1) * ((yyyyMMdd >> 9) & 0x3FFFFFFF));
        }

        LocalDate::Month LocalDate::month() const { return (Month) ((yyyyMMdd >> 5) & 0xF); }

        gint LocalDate::dayOfMonth() const { return (gint) (yyyyMMdd & 0x1F); }

    } // core
} // time
