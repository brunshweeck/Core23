//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "Chrono.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace time {
        Chrono::Chrono() : seconds(-1), nanos(0) {}

        Chrono::Chrono(gbool autoStart) : Chrono() {
            if (autoStart) {
                start0();
            }
        }

        glong Chrono::start() {
            if (!isStarted()) {
                start0();
                return 0;
            } else {
                glong const oldSeconds = seconds;
                gint const oldNanos = nanos;
                start0();
                try {
                    glong const secs = Math::multiplyExact(seconds - oldSeconds, LocalTime::MILLIS_PER_SECOND);
                    return Math::addExact(secs, (glong) Math::floorDiv(nanos - oldNanos, 1000000));
                } catch (const ArithmeticException &ex) {
                    return Long::MAX_VALUE;
                }
            }
        }

        gbool Chrono::isStarted() const {
            return seconds != -1;
        }

        glong Chrono::elapsedTime() const {
            return elapsedTime(ChronoUnit::MILLIS);
        }

        glong Chrono::elapsedTime(Temporal::ChronoUnit unit) const {
            if (!supportUnit(unit))
                UnsupportedTemporalException("Unsupported unit").throws(__trace("Core.time.Chrono"));
            if (!isStarted())
                return 0;
            Chrono const current = Chrono(true);
            return until(current, unit);
        }

        gbool Chrono::hasExpired(glong time) const {
            return hasExpired(time, ChronoUnit::MILLIS);
        }

        gbool Chrono::hasExpired(glong time, Temporal::ChronoUnit unit) const {
            return elapsedTime(unit) >= time;
        }

        gbool Chrono::hasExpired(const Duration &duration) const {
            if(!isStarted())
                return false;
            if(duration.isNegative())
                return true;
            Chrono const chrono = Chrono(true);
            glong secs = chrono.seconds - seconds;
            glong ns = chrono.nanos - nanos;
            if(ns < 0) {
                secs -= 1;
                ns += LocalTime::NANOS_PER_SECOND;
            }
            return secs > duration.seconds() || secs == duration.seconds() && ns >= duration.nanos();
        }

        gbool Chrono::supportUnit(Temporal::Unit unit) const {
            return unit <= WEEKS;
        }

        glong Chrono::until(const Temporal &endExclusive, Temporal::ChronoUnit unit) const {
            if (!Class<Chrono>::hasInstance(endExclusive))
                DateTimeException("Couldn't obtain Chrono with temporal").throws(__trace("Core.time.Chrono"));
            if (!supportUnit(unit))
                UnsupportedTemporalException("Unsupported unit").throws(__trace("Core.time.Chrono"));
            if (!isStarted())
                return Long::MIN_VALUE;
            if (this == &endExclusive)
                return 0LL;
            const Chrono &chrono = (Chrono &) endExclusive;
            if (!chrono.isStarted())
                return Long::MIN_VALUE;
            glong secs = chrono.seconds - seconds;
            glong ns = chrono.nanos - nanos;
            if(ns < 0) {
                ns += LocalTime::NANOS_PER_SECOND;
                secs -= 1;
            }
            try {
                switch (unit) {
                    case NANOS:
                        return Math::addExact(Math::multiplyExact(secs, LocalTime::NANOS_PER_SECOND), ns);
                    case MICROS:
                        return Math::addExact(Math::multiplyExact(secs, LocalTime::MICROS_PER_SECOND),
                                              Math::floorDiv(ns, LocalTime::NANOS_PER_MICRO));
                    case MILLIS:
                        return Math::addExact(Math::multiplyExact(secs, LocalTime::MILLIS_PER_SECOND),
                                              Math::floorDiv(ns, LocalTime::NANOS_PER_MILLI));
                    case SECONDS:
                        return secs;
                    case MINUTES:
                        return Math::floorDiv(secs, LocalTime::SECONDS_PER_MINUTE);
                    case HOURS:
                        return Math::floorDiv(secs, LocalTime::SECONDS_PER_HOUR);
                    case HALF_DAYS:
                        return Math::floorDiv(secs, LocalTime::SECONDS_PER_HOUR * 12);
                    case DAYS:
                        return Math::floorDiv(secs, LocalTime::SECONDS_PER_DAY);
                    case WEEKS:
                        return Math::floorDiv(secs, LocalTime::SECONDS_PER_DAY * 7);
                    default:
                        break;
                }
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core.time.Chrono"));
            }
            return 0L;
        }

        gint Chrono::compareTo(const Chrono &other) const {
            gint const r = Long::compare(seconds, other.seconds);
            if (r != 0) {
                return r;
            }
            return nanos - other.nanos;
        }

        gint Chrono::hash() const {
            return Long::hash(seconds) * 31 + nanos;
        }

        gbool Chrono::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (!Class<Chrono>::hasInstance(o)) {
                return false;
            }
            const Chrono &other = (Chrono &) o;
            return seconds == other.seconds && nanos == other.nanos;
        }

        String Chrono::toString() const {
            if (!isStarted()) {
                return u"Chrono[Stopped]"_S;
            } else {
                return u"Chrono[Running]_S";
            }
        }

        Object &Chrono::clone() const {
            return Unsafe::allocateInstance<Chrono>(*this);
        }

        Chrono::~Chrono() {
            seconds = -1;
            nanos = 0;
        }
    } // core
} // time
