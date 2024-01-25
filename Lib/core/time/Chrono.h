//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_CHRONO_H
#define CORE23_CHRONO_H

#include <core/time/Duration.h>

namespace core {
    namespace time {

        /**
         * The Chrono class represent the chronometer, it is usually used to quickly calculate how much
         * time has elapsed between two events.
         *
         * However, unlike LocalTime, Chrono tries to use monotonic clocks if
         * possible. This means it's not possible to convert Chrono objects
         * to a human-readable time.
         *
         * The typical use-case for the class is to determine how much time was
         * spent in a slow operation. The simplest example of such a case is for
         * debugging purposes.
         *
         * In this class, the timer is started by a call to <b style="color: orange;"> start</b> method and the
         * elapsed time is calculated by the <b style="color: orange;"> elapsedTime</b> and <b style="color: orange;"> duration</b> methods.
         *
         * The time elapsed can also be used to recalculate the time available for
         * another operation, after the first one is complete. This is useful when
         * the execution must complete within a certain time period, but several
         * steps are needed.

         * Another use-case is to execute a certain operation for a specific
         * timeslice. For this, Chrono provides the <b style="color: orange;"> hasExpired</b> convenience
         * method, which can be used to determine if a certain number of
         * milliseconds has already elapsed.
         *
         * <p>
         * <h2> Reference Clocks </h2>
         *
         * Chrono will use the platform's monotonic reference clock in all
         * platforms that support it. This has the added benefit that Chrono
         * is immune to time adjustments, such as the user correcting the time.
         * Also unlike <b style="color: orange;"> LocalTime</b>, Chrono is immune to changes in the timezone settings,
         * such as daylight-saving periods.
         *
         * <p>
         * On the other hand, this means Chrono values can only be compared
         * with other values that use the same reference. This is especially true if
         * the time since the reference is extracted from the <b> Chrono</b> object
         * and serialised. These values should never be exchanged across the network or
         * saved to disk, since there's no telling whether the computer node receiving
         * the data is the same as the one originating it or if it has rebooted since.
         *
         * It is, however, possible to exchange the value with other processes
         * running on the same machine, provided that they also use the same
         * reference clock. Chrono will always use the same clock, so it's
         * safe to compare with the value coming from another process in the same
         * machine. If comparing to values produced by other APIs, you should check
         * that the clock used is the same as Chrono (see Chrono.clock method).
         *
         * <p>
         * <h2> 32 bits overflows </h2>
         *
         * Some of the clocks used by Chrono have a limited range and may
         * overflow after hitting the upper limit (usually 32 bits). Chrono
         * deals with this overflow issue and presents a consistent timing. However,
         * when extracting the time since reference from Chrono, two
         * different processes in the same machine may have different understanding
         * of how much time has actually elapsed.
         *
         * <p style="color: red;">
         * Note: It not recommended to use this class as key map in the Map or element in the set.
         *
         * @author Brunshweeck Tazeussong
         *
         * @see LocalTime
         * @see Duration
         * @see Temporal::ChronoUnit
         */
        class Chrono CORE_FINAL : public Temporal, public Comparable<Chrono> {
        private:
            /**
             * count of seconds since the start instant
             */
            glong seconds;

            /**
             * count of nanos of seconds since the start instant
             */
            gint nanos;

        public:
            /**
             * Create new Chrono. A Chrono time becomes valid once it has been started.
             */
            Chrono();

            /**
             * Create and start new Chrono.
             */
            CORE_EXPLICIT Chrono(gbool autoStart);

            /**
             * Start or restart this chrono timer.
             *
             * @return the previous elapsed timeline in milliseconds or 0 if has not started previously.
             */
            glong start();

            /**
             * Return true if chrono timer has started, false otherwise.
             * This calling are equivalent to <b> return this.elapsed() >= 0; </b>
             */
            gbool isStarted() const;

            /**
             * Return the current elapsed time (timeout) in milliseconds or
             * -1 if this chrono has not started.
             * This calling is equivalent to <b> return this.elapsed(Time::MILLI_SECONDS); </b>
             */
            glong elapsedTime() const;

            /**
             * Return the current elapsed time (timeout) in specified unit or
             * -1 if this chrono has not started.
             *
             * @param unit The output unit
             * @throws ArithmeticException if value representation exceed 63 bits
             */
            glong elapsedTime(ChronoUnit unit) const;

            /**
             * Return the current elapsed time or
             * -1 if this chrono has not started.
             * This calling is equivalent to <b> return this.elapsed(Time::MILLI_SECONDS); </b>
             */
            Duration duration() const;

            /**
             * Return the current elapsed time in specified unit or
             * -1 if this chrono has not started.
             *
             * @param unit The output unit
             * @throws ArithmeticException if value representation exceed 63 bits
             */
            Duration duration(ChronoUnit unit) const;

            /**
             * Check if specified timeout has expired. In other words,
             * return true if the current elapsed time great or equals to specified timeout.
             *
             * @param time The except timeout in milliseconds
             */
            gbool hasExpired(glong time) const;

            /**
             * Check if specified duration has expired. In other words,
             * return true if the current elapsed time great or equals to specified timeout.
             *
             * @param time The except timeout in milliseconds
             */
            gbool hasExpired(const Duration &duration) const;

            /**
             * Check if specified timeout has expired. In other words,
             * return true if the current elapsed time great or equals to
             * specified timeout.
             *
             * @param timeout The except timeout
             * @param unit The timeout unit
             */
            gbool hasExpired(glong time, ChronoUnit unit) const;

            /**
             * Return true if value returned by elapsed() and until() methods is convertible to
             * specified unit.
             * <li> NANO_SECONDS
             * <li> MICRO_SECONDS
             * <li> MILLI_SECONDS
             * <li> SECONDS
             * <li> MINUTES
             * <li> HOURS
             * <li> HALF_DAYS
             * <li> DAYS
             */
            gbool supportUnit(Unit unit) const override;

            /**
             * Return offset time between this time and specified temporal or
             * Long.MIN_VALUE if any timer not started.
             *
             * @param temporal The given temporal
             * @param unit The output unit
             *
             * @throws TemporalException if specified unit is not supported or
             *                           specified temporal is not instance of Chrono
             * @throws ArithmeticException If value overflow long range
             */
            glong until(const Temporal &endExclusive, ChronoUnit unit) const override;

            /**
             * Get field value into elapsed time.
             * this override method is not supported
             * @throws UnsupportedOperationException always throws
             */
            gint get(ChronoField field) const override;

            /**
             * Get field value into elapsed time as long.
             * this override method is not supported
             * @throws UnsupportedOperationException always throws
             */
            glong getLong(ChronoField field) const override;

            /**
             * Compare specified Chrono with this timeline numerically
             *
             * @param other The object to be compared
             */
            gint compareTo(const Chrono &other) const override;

            /**
             * Return hash code of this object
             */
            gint hash() const override;

            /**
             * Return true if and only if specified object are Chrono instance
             * and have same instant value with this Chrono
             *
             * @param o object to be compared
             */
            gbool equals(const Object &o) const override;

            /**
             * Return String representation of this Chrono.
             * Note: The value return by this function is not same
             * in any times.
             */
            String toString() const override;

            /**
             * Return shadow copy of this object
             */
            Object &clone() const override;
        };

    } // core
} // time

#endif //CORE23_CHRONO_H
