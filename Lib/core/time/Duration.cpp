//
// Created by T.N.Brunshweeck on 20/01/2024.
//

#include "Duration.h"
#include <core/StringBuffer.h>

namespace core {
    namespace time {

        const Duration Duration::ZERO = Duration(0, 0);

        Duration Duration::ofDays(glong days) {
            try {
                return Duration(Math::multiplyExact(days, LocalTime::SECONDS_PER_DAY), 0);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
            return ZERO;
        }

        Duration Duration::ofHours(glong hours) {
            try {
                return Duration(Math::multiplyExact(hours, LocalTime::SECONDS_PER_HOUR), 0);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
            return ZERO;
        }

        Duration Duration::ofMinutes(glong minutes) {
            try {
                return Duration(Math::multiplyExact(minutes, LocalTime::SECONDS_PER_MINUTE), 0);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
            return ZERO;
        }

        Duration Duration::ofSeconds(glong seconds) {
            try {
                return Duration(seconds, 0);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
            return ZERO;
        }

        Duration Duration::ofSeconds(glong seconds, glong nanoAdjustment) {
            try {
                glong const secs = Math::addExact(seconds, Math::floorDiv(nanoAdjustment, LocalTime::NANOS_PER_SECOND));
                gint const ns = (gint) Math::floorMod(nanoAdjustment, LocalTime::NANOS_PER_SECOND);
                return Duration(secs, ns);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
            return ZERO;
        }

        Duration Duration::ofMillis(glong millis) {
            glong secs = millis / 1000;
            gint ms = (gint) (millis % 1000);
            if (ms < 0) {
                ms += 1000;
                secs -= 1;
            }
            return Duration(secs, ms * 1000000);
        }

        Duration Duration::ofNanos(glong nanos) {
            glong secs = nanos / LocalTime::NANOS_PER_SECOND;
            gint ns = (gint) (nanos % LocalTime::NANOS_PER_SECOND);
            if (ns < 0) {
                ns += (gint) LocalTime::NANOS_PER_SECOND;
                secs -= 1;
            }
            return Duration(secs, ns);
        }

        Duration Duration::of(glong amount, Temporal::ChronoUnit unit) {
            try {
                return ZERO.plus(amount, unit);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
            return ZERO;
        }

        Duration Duration::between(const Temporal &startInclusive, const Temporal &endExclusive) {
            try {
                return ofNanos(startInclusive.until(endExclusive, NANOS));
            } catch (Exception const &ex) {
                if (!Class<DateTimeException>::hasInstance(ex) && !Class<ArithmeticException>::hasInstance(ex))
                    ex.throws(__trace("core.time.Duration"));
                glong secs = startInclusive.until(endExclusive, SECONDS);
                glong nanos;
                try {
                    nanos = endExclusive.getLong(NANO_OF_SECOND) - startInclusive.getLong(NANO_OF_SECOND);
                    if (secs > 0 && nanos < 0) {
                        secs++;
                    } else if (secs < 0 && nanos > 0) {
                        secs--;
                    }
                } catch (DateTimeException const &ex2) {
                    nanos = 0;
                }
                return ofSeconds(secs, nanos);
            }
        }

        Duration::Duration(glong seconds, gint nanos) : secs(seconds), ns(nanos) {}

        glong Duration::get(Temporal::ChronoUnit unit) const {
            if (unit == SECONDS) {
                return secs;
            } else if (unit == NANOS) {
                return ns;
            } else {
                UnsupportedTemporalException("Unsupported unit: " + String::valueOf(unit))
                        .throws(__trace("core.time.Duration"));
            }
        }

        gbool Duration::isPositive() const {
            return (secs | ns) >= 0;
        }

        gbool Duration::isZero() const {
            return (secs | ns) == 0;
        }

        gbool Duration::isNegative() const {
            return secs < 0;
        }

        glong Duration::seconds() const {
            return secs;
        }

        gint Duration::nanos() const {
            return ns;
        }

        Duration Duration::withSeconds(glong seconds) const {
            return Duration(seconds, ns);
        }

        Duration Duration::withNanos(gint nanos) const {
            try {
                return Duration(secs, (gint) checkValue(NANO_OF_SECOND, nanos));
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::plus(const Duration &duration) const {
            try {
                return plus(duration.seconds(), duration.nanos());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::plus(glong amountToAdd, Temporal::TemporalUnit unit) const {
            if (unit > DAYS) {
                UnsupportedTemporalException("Unit must not have an estimated duration")
                        .throws(__trace("core.time.Duration"));
            }
            try {
                if (unit == DAYS) {
                    return plus(Math::multiplyExact(amountToAdd, LocalTime::SECONDS_PER_DAY), 0);
                }
                if (amountToAdd == 0) {
                    return *this;
                }
                switch (unit) {
                    case NANOS:
                        return plusNanos(amountToAdd);
                    case MICROS:
                        return plusSeconds((amountToAdd / (1000000LL * 1000)) * 1000).plusNanos(
                                (amountToAdd % (1000000LL * 1000)) * 1000);
                    case MILLIS :
                        return plusMillis(amountToAdd);
                    case SECONDS:
                        return plusSeconds(amountToAdd);
                    default:
                        return plusSeconds(Math::multiplyExact(ofUnit(unit).secs, amountToAdd));
                };
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
            return ZERO;
        }

        Duration Duration::plusDays(glong daysToAdd) const {
            try {
                return plusSeconds(Math::multiplyExact(daysToAdd, LocalTime::SECONDS_PER_DAY));
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::plusHours(glong hoursToAdd) const {
            try {
                return plusSeconds(Math::multiplyExact(hoursToAdd, LocalTime::SECONDS_PER_HOUR));
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::plusMinutes(glong minutesToAdd) const {
            try {
                return plusSeconds(Math::multiplyExact(minutesToAdd, LocalTime::SECONDS_PER_MINUTE));
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::plusSeconds(glong secondsToAdd) const {
            try {
                return plus(secondsToAdd, 0);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::plusMillis(glong millisToAdd) const {
            try {
                return plus(millisToAdd / 1000, (millisToAdd % 1000) * 1000000L);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::plusNanos(glong nanosToAdd) const {
            try {
                return plus(0, nanosToAdd);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::plus(glong secondsToAdd, glong nanosToAdd) const {
            try {
                if ((secondsToAdd | nanosToAdd) == 0) {
                    return *this;
                }
                glong epochSec = Math::addExact(secs, secondsToAdd);
                epochSec = Math::addExact(epochSec, nanosToAdd / LocalTime::NANOS_PER_SECOND);
                nanosToAdd = nanosToAdd % LocalTime::NANOS_PER_SECOND;
                glong const nanoAdjustment = ns + nanosToAdd;  // safe int+NANOS_PER_SECOND
                return ofSeconds(epochSec, nanoAdjustment);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::minus(const Duration &duration) const {
            try {
                glong const secsToSubtract = duration.seconds();
                gint const nanosToSubtract = duration.nanos();
                if (secsToSubtract == Long::MIN_VALUE) {
                    return plus(Long::MAX_VALUE, -nanosToSubtract).plus(1, 0);
                }
                return plus(-secsToSubtract, -nanosToSubtract);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::minus(glong amountToSubtract, Temporal::TemporalUnit unit) const {
            try {
                return (amountToSubtract == Long::MIN_VALUE) ? plus(Long::MAX_VALUE, unit).plus(1, unit)
                                                             : plus(-amountToSubtract, unit);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::minusDays(glong daysToSubtract) const {
            try {
                return (daysToSubtract == Long::MIN_VALUE) ? plusDays(Long::MAX_VALUE).plusDays(1)
                                                           : plusDays(-daysToSubtract);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::minusHours(glong hoursToSubtract) const {
            try {
                return (hoursToSubtract == Long::MIN_VALUE) ? plusHours(Long::MAX_VALUE).plusHours(1)
                                                            : plusHours(-hoursToSubtract);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::minusMinutes(glong minutesToSubtract) const {
            try {
                return (minutesToSubtract == Long::MIN_VALUE) ? plusMinutes(Long::MAX_VALUE).plusMinutes(1)
                                                              : plusMinutes(-minutesToSubtract);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::minusSeconds(glong secondsToSubtract) const {
            try {
                return (secondsToSubtract == Long::MIN_VALUE) ? plusSeconds(Long::MAX_VALUE).plusSeconds(1)
                                                              : plusSeconds(-secondsToSubtract);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::minusMillis(glong millisToSubtract) const {
            try {
                return (millisToSubtract == Long::MIN_VALUE) ? plusMillis(Long::MAX_VALUE).plusMillis(1)
                                                             : plusMillis(-millisToSubtract);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::minusNanos(glong nanosToSubtract) const {
            try {
                return (nanosToSubtract == Long::MIN_VALUE) ? plusNanos(Long::MAX_VALUE).plusNanos(1)
                                                            : plusNanos(-nanosToSubtract);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::multipliedBy(glong multiplicand) const {
            try {
                if (multiplicand == 0)
                    return ZERO;
                if (multiplicand == 1)
                    return *this;
                glong const seconds = Math::multiplyExact(multiplicand, secs);
                glong const nanos = Math::multiplyExact(multiplicand, ns);
                return ofSeconds(seconds).plusNanos(nanos);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::operator*(glong multiplicand) const {
            return multipliedBy(multiplicand);
        }

        Duration Duration::dividedBy(glong divisor) const {
            if (divisor == 0) {
                ArithmeticException("Cannot divide by zero").throws(__trace("core.time.Duration"));
            }
            try {
                glong const seconds = Math::floorDiv(secs, divisor);
                glong nanos = Math::floorMod(secs, divisor);
                try {
                    nanos = Math::multiplyExact(nanos, LocalTime::NANOS_PER_SECOND);
                    nanos = Math::floorDiv(nanos, divisor);
                } catch (ArithmeticException const &ex2) {
                    gdouble const d = (gdouble) nanos / (gdouble) divisor;
                    nanos = (gint) (d * LocalTime::NANOS_PER_SECOND);
                }
                nanos = Math::addExact(nanos, Math::floorDiv((glong) ns, divisor));
                return Duration(seconds, (gint) nanos);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::operator/(glong divisor) const {
            return dividedBy(divisor);
        }

        glong Duration::dividedBy(const Duration &divisor) const {
            if (divisor.isZero()) {
                ArithmeticException("Cannot divide by zero").throws(__trace("core.time.Duration"));
            }
            if (isZero())
                return 0;
            try {
                if (divisor.ns == 0)
                    return dividedBy(divisor.seconds()).seconds();
                else if (divisor.secs == 0) {
                    return multipliedBy(LocalTime::NANOS_PER_SECOND).dividedBy(divisor.nanos()).seconds();
                } else {
                    gdouble const d1 = (gdouble) secs + (gdouble) ns / 1.0E9;
                    gdouble const d2 = (gdouble) divisor.secs + (gdouble) divisor.ns / 1.0E9;
                    return (glong) Math::floor(d1 / d2);
                }
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        glong Duration::operator/(const Duration &divisor) const {
            return dividedBy(divisor);
        }

        Duration Duration::negated() const {
            try {
                return Duration(Math::multiplyExact(secs, -1), Math::multiplyExact(ns, -1));
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        Duration Duration::operator-() const {
            return negated();
        }

        Duration Duration::abs() const {
            try {
                return Duration(Math::abs(secs), Math::abs(ns));
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        glong Duration::toDays() const {
            return Math::floorDiv(secs, LocalTime::SECONDS_PER_DAY);
        }

        glong Duration::toHours() const {
            return Math::floorDiv(secs, LocalTime::SECONDS_PER_HOUR);
        }

        glong Duration::toMinutes() const {
            return Math::floorDiv(secs, LocalTime::SECONDS_PER_MINUTE);
        }

        glong Duration::toSeconds() const {
            return secs;
        }

        glong Duration::toMillis() const {
            try {
                return Math::addExact(Math::multiplyExact(secs, LocalTime::MILLIS_PER_SECOND),
                                      (glong) Math::floorDiv(ns, 1000000));
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        glong Duration::toNanos() const {
            try {
                return Math::addExact(Math::multiplyExact(secs, LocalTime::NANOS_PER_SECOND), (glong) ns);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.time.Duration"));
            }
        }

        glong Duration::toDaysPart() const {
            return toDays();
        }

        gint Duration::toHoursPart() const {
            return (gint) (toHours() % LocalTime::HOURS_PER_DAY);
        }

        gint Duration::toMinutesPart() const {
            return (gint) (toMinutes() % LocalTime::MINUTES_PER_HOUR);
        }

        gint Duration::toSecondsPart() const {
            return (gint) (secs % LocalTime::SECONDS_PER_MINUTE);
        }

        gint Duration::toMillisPart() const {
            return ns / 1000000;
        }

        gint Duration::toNanosPart() const {
            return ns;
        }

        Duration Duration::truncateTo(Temporal::TemporalUnit unit) const {
            switch (unit) {
                case NANOS:
                    return *this;
                case MICROS:
                    return ofSeconds(secs).plusNanos(ns * 1000LL);
                case MILLIS:
                    return ofSeconds(secs).plusNanos(ns * LocalTime::NANOS_PER_MILLI);
                case SECONDS:
                    return ofSeconds(secs);
                case MINUTES:
                    return ofMinutes(secs / LocalTime::SECONDS_PER_MINUTE);
                case HOURS:
                    return ofHours(secs / LocalTime::SECONDS_PER_HOUR);
                case HALF_DAYS:
                    return ofHours(secs / (LocalTime::SECONDS_PER_DAY / 2));
                case DAYS:
                    return ofDays(secs / LocalTime::SECONDS_PER_DAY);
                default:
                    UnsupportedTemporalException("Unit is too large to be used for truncation")
                            .throws(__trace("core.time.duration"));
            }
            return ZERO;
        }

        gint Duration::compareTo(const Duration &other) const {
            gint const r = Long::compare(secs, other.secs);
            if (r != 0)
                return r;
            return Integer::compare(ns, other.ns);
        }

        gbool Duration::equals(const Object &o) const {
            if (this == &o)
                return true;
            if (!Class<Duration>::hasInstance(o))
                return false;
            Duration const &other = (const Duration &) o;
            return secs == other.secs && ns == other.ns;
        }

        gint Duration::hash() const {
            return Long::hash(secs) + Integer::hash(ns);
        }

        String Duration::toString() const {
            if (*this == ZERO) {
                return "PT0S";
            }
            glong effectiveTotalSecs = secs;
            if (secs < 0 && ns > 0) {
                effectiveTotalSecs++;
            }
            glong const hours = effectiveTotalSecs / LocalTime::SECONDS_PER_HOUR;
            gint const minutes = (gint) ((effectiveTotalSecs % LocalTime::SECONDS_PER_HOUR) / LocalTime::SECONDS_PER_MINUTE);
            gint const secs = (gint) (effectiveTotalSecs % LocalTime::SECONDS_PER_MINUTE);
            StringBuffer buf = StringBuffer(24);
            buf.append("PT");
            if (hours != 0) {
                buf.append(hours).append('H');
            }
            if (minutes != 0) {
                buf.append(minutes).append('M');
            }
            if (secs == 0 && ns == 0 && buf.length() > 2) {
                return buf.toString();
            }
            if (secs <= 0 && ns > 0) {
                if (secs == 0) {
                    buf.append("-0");
                } else {
                    buf.append(secs);
                }
            } else {
                buf.append(secs);
            }
            if (ns > 0) {
                gint const pos = buf.length();
                if (seconds() < 0) {
                    buf.append(2 * LocalTime::NANOS_PER_SECOND - ns);
                } else {
                    buf.append(ns + LocalTime::NANOS_PER_SECOND);
                }
                while (buf.charAt(buf.length() - 1) == '0') {
                    buf.resize(buf.length() - 1);
                }
                buf.set(pos, '.');
            }
            buf.append('S');
            return buf.toString();
        }

        glong Duration::until(const Temporal &endExclusive, Temporal::TemporalUnit unit) const {
            if (!Class<Duration>::hasInstance(endExclusive)) {
                DateTimeException("Could not obtain duration from instance of class " + endExclusive.classname())
                        .throws(__trace("core.time.Duration"));
            }
            if (!supportUnit(unit))
                UnsupportedTemporalException("Unsupported unit").throws(__trace("core.time.Duration"));
            Duration const &end = (const Duration &) endExclusive;
            Duration const duration = end.plus(*this);
            switch (unit) {
                case NANOS:
                    return duration.toNanos();
                case MICROS:
                    return duration.toNanos() / 1000;
                case MILLIS:
                    return duration.toNanos() / 1000000;
                case SECONDS:
                    return duration.toSeconds();
                case MINUTES:
                    return duration.toMinutes();
                case HOURS:
                    return duration.toHours();
                case HALF_DAYS:
                    return duration.toHours() / 12;
                case DAYS:
                    return duration.toDays();
                default:
                    return 0;
            }
        }

        Duration Duration::ofUnit(Temporal::ChronoUnit unit) {
            switch (unit) {
                case NANOS:
                    return ofNanos(1);
                case MICROS:
                    return ofNanos(1000);
                case MILLIS:
                    return ofNanos(1000000);
                case SECONDS:
                    return ofSeconds(1);
                case MINUTES:
                    return ofMinutes(1);
                case HOURS:
                    return ofHours(1);
                case HALF_DAYS:
                    return ofHours(12);
                case DAYS:
                    return ofDays(1);
                case WEEKS:
                    return ofDays(7);
                case MONTHS:
                    return ofDays(30);
                case YEARS:
                    return ofDays(365);
                case DECADES:
                    return ofDays(3652);
                case CENTURIES:
                    return ofDays(36525);
                case MILLENNIA:
                    return ofDays(365250);
                case ERAS:
                    return ofDays(365250000LL);
                case FOREVER:
                    return ofDays(Long::MAX_VALUE);
            }
        }

        Duration Duration::operator+(const Duration &duration) const {
            return plus(duration);
        }

        Duration Duration::operator-(const Duration &duration) const {
            return minus(duration);
        }

    } // time
} // core
