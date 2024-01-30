//
// Created by T.N.Brunshweeck on 20/01/2024.
//

#ifndef CORE23_DURATION_H
#define CORE23_DURATION_H

#include <core/time/LocalTime.h>
#include <core/ArithmeticException.h>
#include <core/Math.h>
#include <core/Long.h>
#include <core/Integer.h>

namespace core {
    namespace time {

        /**
         * A time-based amount of time, such as '34.5 seconds'.
         * <p>
         * This class models a quantity or amount of time in terms of seconds and nanoseconds.
         * It can be accessed using other duration-based units, such as minutes and hours.
         * In addition, the <b style="color: orange;"> DAYS</b> unit can be used and is treated as
         * exactly equal to 24 hours, thus ignoring daylight savings effects.
         * <p>
         * A physical duration could be of infinite length.
         * The duration uses nanosecond resolution with a maximum value of the seconds that can
         * be held in a <b> long</b>. This is greater than the current estimated age of the universe.
         * <p>
         * The range of a duration requires the storage of a number larger than a <b> long</b>.
         * To achieve this, the class stores a <b> long</b> representing seconds and an <b> int</b>
         * representing nanosecond-of-second, which will always be between 0 and 999,999,999.
         * The model is of a directed duration, meaning that the duration may be negative.
         * <p>
         * The duration is measured in "seconds", but these are not necessarily identical to
         * the scientific "SI second" definition based on atomic clocks.
         * This difference only impacts durations measured near a leap-second and should not affect
         * most applications.
         * <p>
         * This is a <a href="">value-based</a>
         * class; programmers should treat instances that are
         * <b style="color: green;"> equal</b> as interchangeable and should not
         * use instances for synchronization, or unpredictable behavior may
         * occur. For example, in a future release, synchronization may fail.
         * The <b> equals</b> method should be used for comparisons.
         *
         * @implSpec
         * This class is immutable and thread-safe.
         *
         * @author Brunshweeck Tazeussong
         */
        class Duration CORE_FINAL : public Temporal, public Comparable<Duration> {
        private:
            /**
             * The number of seconds in the duration.
             */
            glong Seconds;

            /**
             * The number of nanoseconds in the duration, expressed as a fraction of the
             * number of seconds. This is always positive, and never exceeds 999,999,999.
             */
            gint Nanos;

        public:

            /**
             * Constant for a duration of zero.
             */
            static const Duration ZERO;

            /**
             * Obtains a <b> Duration</b> representing a number of standard 24 hour days.
             * <p>
             * The seconds are calculated based on the standard definition of a day,
             * where each day is 86400 seconds which implies a 24 hour day.
             * The nanosecond in second field is setValue to zero.
             *
             * @param days  the number of days, positive or negative
             * @return a <b> Duration</b>
             * @throws ArithmeticException if the input days exceeds the capacity of <b> Duration</b>
             */
            static Duration ofDays(glong days);

            /**
             * Obtains a <b> Duration</b> representing a number of standard hours.
             * <p>
             * The seconds are calculated based on the standard definition of an hour,
             * where each hour is 3600 seconds.
             * The nanosecond in second field is setValue to zero.
             *
             * @param hours  the number of hours, positive or negative
             * @return a <b> Duration</b>
             * @throws ArithmeticException if the input hours exceeds the capacity of <b> Duration</b>
             */
            static Duration ofHours(glong hours);

            /**
             * Obtains a <b> Duration</b> representing a number of standard minutes.
             * <p>
             * The seconds are calculated based on the standard definition of a minute,
             * where each minute is 60 seconds.
             * The nanosecond in second field is setValue to zero.
             *
             * @param minutes  the number of minutes, positive or negative
             * @return a <b> Duration</b>
             * @throws ArithmeticException if the input minutes exceeds the capacity of <b> Duration</b>
             */
            static Duration ofMinutes(glong minutes);

            /**
             * Obtains a <b> Duration</b> representing a number of seconds.
             * <p>
             * The nanosecond in second field is setValue to zero.
             *
             * @param seconds  the number of seconds, positive or negative
             * @return a <b> Duration</b>
             */
            static Duration ofSeconds(glong seconds);

            /**
             * Obtains a <b> Duration</b> representing a number of seconds and an
             * adjustment in nanoseconds.
             * <p>
             * This method allows an arbitrary number of nanoseconds to be passed in.
             * The factory will alter the values of the second and nanosecond in order
             * to ensure that the stored nanosecond is in the range 0 to 999,999,999.
             * For example, the following will result in exactly the same duration:
             * <pre>
             *  Duration.ofSeconds(3, 1);
             *  Duration.ofSeconds(4, -999_999_999);
             *  Duration.ofSeconds(2, 1000_000_001);
             * </pre>
             *
             * @param seconds  the number of seconds, positive or negative
             * @param nanoAdjustment  the nanosecond adjustment to the number of seconds, positive or negative
             * @return a <b> Duration</b>
             * @throws ArithmeticException if the adjustment causes the seconds to exceed the capacity of <b> Duration</b>
             */
            static Duration ofSeconds(glong seconds, glong nanoAdjustment);

            /**
             * Obtains a <b> Duration</b> representing a number of milliseconds.
             * <p>
             * The seconds and nanoseconds are extracted from the specified milliseconds.
             *
             * @param millis  the number of milliseconds, positive or negative
             * @return a <b> Duration</b>
             */
            static Duration ofMillis(glong millis);

            /**
             * Obtains a <b> Duration</b> representing a number of nanoseconds.
             * <p>
             * The seconds and nanoseconds are extracted from the specified nanoseconds.
             *
             * @param nanos  the number of nanoseconds, positive or negative
             * @return a <b> Duration</b>
             */
            static Duration ofNanos(glong nanos);

            /**
             * Obtains a <b> Duration</b> representing an amount in the specified unit.
             * <p>
             * The parameters represent the two parts of a phrase like '6 Hours'. For example:
             * <pre>
             *  Duration.of(3, SECONDS);
             *  Duration.of(465, HOURS);
             * </pre>
             * Only a subset of units are accepted by this method.
             * The unit must either have an <b style="color: green;"> exact duration</b> or
             * be <b style="color: orange;"> ChronoUnit::DAYS</b> which is treated as 24 hours. Other units throw an exception.
             *
             * @param amount  the amount of the duration, measured in terms of the unit, positive or negative
             * @param unit  the unit that the duration is measured in, must have an exact duration
             * @return a <b> Duration</b>
             * @throws DateTimeException if the period unit has an estimated duration
             * @throws ArithmeticException if a numeric overflow occurs
             */
            static Duration of(glong amount, ChronoUnit unit);

            /**
             * Obtains a <b> Duration</b> representing the duration between two temporal objects.
             * <p>
             * This calculates the duration between two temporal objects. If the objects
             * are of different types, then the duration is calculated based on the type
             * of the first object. For example, if the first argument is a <b> LocalTime</b>
             * then the second argument is converted to a <b> LocalTime</b>.
             * <p>
             * The specified temporal objects must support the <b style="color: orange;"> ChronoUnit::SECONDS</b> unit.
             * For full accuracy, either the <b style="color: orange;"> ChronoUnit::NANOS</b> unit or the
             * <b style="color: orange;"> ChronoField::NANO_OF_SECOND</b> field should be supported.
             * <p>
             * The result of this method can be a negative period if the end is before the start.
             * To guarantee to obtain a positive duration call <b style="color: orange;"> Duration::abs()</b> on the result.
             *
             * @param startInclusive  the start instant, inclusive
             * @param endExclusive  the end instant, exclusive
             * @return a <b> Duration</b>
             * @throws DateTimeException if the seconds between the temporals cannot be obtained
             * @throws ArithmeticException if the calculation exceeds the capacity of <b> Duration</b>
             */
            static Duration between(const Temporal &startInclusive, const Temporal &endExclusive);

        private:

            /**
             * Constructs an instance of <b> Duration</b> using seconds and nanoseconds.
             *
             * @param seconds  the length of the duration in seconds, positive or negative
             * @param nanos  the nanoseconds within the second, from 0 to 999,999,999
             */
            CORE_EXPLICIT Duration(glong seconds, gint nanos);

        public:

            /**
             * Gets the value of the requested unit.
             * <p>
             * This returns a value for each of the two supported units,
             * <b style="color: orange;"> ChronoUnit::SECONDS</b> and <b style="color: orange;"> ChronoUnit::NANOS</b>.
             * All other units throw an exception.
             *
             * @param unit the <b> TemporalUnit</b> for which to return the value
             * @return the long value of the unit
             * @throws DateTimeException if the unit is not supported
             * @throws UnsupportedTemporalTypeException if the unit is not supported
             */
            glong get(ChronoUnit unit) const;

            /**
             * Checks if this duration is positive, excluding zero.
             * <p>
             * A <b> Duration</b> represents a directed distance between two points on
             * the time-line and can therefore be positive, zero or negative.
             * This method checks whether the length is greater than zero.
             *
             * @return true if this duration has a total length greater than zero
             */
            gbool isPositive() const;

            /**
             * Checks if this duration is zero length.
             * <p>
             * A <b> Duration</b> represents a directed distance between two points on
             * the time-line and can therefore be positive, zero or negative.
             * This method checks whether the length is zero.
             *
             * @return true if this duration has a total length equal to zero
             */
            gbool isZero() const;

            /**
             * Checks if this duration is negative, excluding zero.
             * <p>
             * A <b> Duration</b> represents a directed distance between two points on
             * the time-line and can therefore be positive, zero or negative.
             * This method checks whether the length is less than zero.
             *
             * @return true if this duration has a total length less than zero
             */
            gbool isNegative() const;

            /**
             * Gets the number of seconds in this duration.
             * <p>
             * The length of the duration is stored using two fields - seconds and nanoseconds.
             * The nanoseconds part is a value from 0 to 999,999,999 that is an adjustment to
             * the length in seconds.
             * The total duration is defined by calling this method and <b style="color: orange;"> Duration::nanos()</b>.
             * <p>
             * A <b> Duration</b> represents a directed distance between two points on the time-line.
             * A negative duration is expressed by the negative sign of the seconds part.
             * A duration of -1 nanosecond is stored as -1 seconds plus 999,999,999 nanoseconds.
             *
             * @return the whole seconds part of the length of the duration, positive or negative
             */
            glong seconds() const;

            /**
             * Gets the number of nanoseconds within the second in this duration.
             * <p>
             * The length of the duration is stored using two fields - seconds and nanoseconds.
             * The nanoseconds part is a value from 0 to 999,999,999 that is an adjustment to
             * the length in seconds.
             * The total duration is defined by calling this method and <b style="color: orange;"> Duration::seconds()</b>.
             * <p>
             * A <b> Duration</b> represents a directed distance between two points on the time-line.
             * A negative duration is expressed by the negative sign of the seconds part.
             * A duration of -1 nanosecond is stored as -1 seconds plus 999,999,999 nanoseconds.
             *
             * @return the nanoseconds within the second part of the length of the duration, from 0 to 999,999,999
             */
            gint nanos() const;

            /**
             * Returns a copy of this duration with the specified amount of seconds.
             * <p>
             * This returns a duration with the specified seconds, retaining the
             * nano-of-second part of this duration.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param seconds  the seconds to represent, may be negative
             * @return a <b> Duration</b> based on this period with the requested seconds
             */
            Duration withSeconds(glong seconds) const;

            /**
             * Returns a copy of this duration with the specified nano-of-second.
             * <p>
             * This returns a duration with the specified nano-of-second, retaining the
             * seconds part of this duration.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param nanoOfSecond  the nano-of-second to represent, from 0 to 999,999,999
             * @return a <b> Duration</b> based on this period with the requested nano-of-second
             * @throws DateTimeException if the nano-of-second is invalid
             */
            Duration withNanos(gint nanos) const;

            /**
             * Returns a copy of this duration with the specified duration added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param duration  the duration to add, positive or negative
             * @return a <b> Duration</b> based on this duration with the specified duration added
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plus(const Duration &duration) const;

            Duration operator+(const Duration &duration) const;

            /**
             * Returns a copy of this duration with the specified duration added.
             * <p>
             * The duration amount is measured in terms of the specified unit.
             * Only a subset of units are accepted by this method.
             * The unit must either have an <b style="color: green;"> exact duration</b> or
             * be <b style="color: orange;"> ChronoUnit::DAYS</b> which is treated as 24 hours. Other units throw an exception.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param amountToAdd  the amount to add, measured in terms of the unit, positive or negative
             * @param unit  the unit that the amount is measured in, must have an exact duration
             * @return a <b> Duration</b> based on this duration with the specified duration added
             * @throws UnsupportedTemporalTypeException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plus(glong amountToAdd, TemporalUnit unit) const;

            /**
             * Returns a copy of this duration with the specified duration in standard 24 hour days added.
             * <p>
             * The number of days is multiplied by 86400 to obtain the number of seconds to add.
             * This is based on the standard definition of a day as 24 hours.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param daysToAdd  the days to add, positive or negative
             * @return a <b> Duration</b> based on this duration with the specified days added
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plusDays(glong daysToAdd) const;

            /**
             * Returns a copy of this duration with the specified duration in hours added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param hoursToAdd  the hours to add, positive or negative
             * @return a <b> Duration</b> based on this duration with the specified hours added
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plusHours(glong hoursToAdd) const;

            /**
             * Returns a copy of this duration with the specified duration in minutes added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param minutesToAdd  the minutes to add, positive or negative
             * @return a <b> Duration</b> based on this duration with the specified minutes added
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plusMinutes(glong minutesToAdd) const;

            /**
             * Returns a copy of this duration with the specified duration in seconds added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param secondsToAdd  the seconds to add, positive or negative
             * @return a <b> Duration</b> based on this duration with the specified seconds added
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plusSeconds(glong secondsToAdd) const;

            /**
             * Returns a copy of this duration with the specified duration in milliseconds added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param millisToAdd  the milliseconds to add, positive or negative
             * @return a <b> Duration</b> based on this duration with the specified milliseconds added
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plusMillis(glong millisToAdd) const;

            /**
             * Returns a copy of this duration with the specified duration in nanoseconds added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param nanosToAdd  the nanoseconds to add, positive or negative
             * @return a <b> Duration</b> based on this duration with the specified nanoseconds added
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plusNanos(glong nanosToAdd) const;

        private:

            /**
             * Returns a copy of this duration with the specified duration added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param secondsToAdd  the seconds to add, positive or negative
             * @param nanosToAdd  the nanos to add, positive or negative
             * @return a <b> Duration</b> based on this duration with the specified seconds added
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plus(glong secondsToAdd, glong nanosToAdd) const;

        public:
            /**
             * Returns a copy of this duration with the specified duration subtracted.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param duration  the duration to subtract, positive or negative
             * @return a <b> Duration</b> based on this duration with the specified duration subtracted
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration minus(const Duration &duration) const;

            Duration operator-(const Duration &duration) const;

            /**
             * Returns a copy of this duration with the specified duration subtracted.
             * <p>
             * The duration amount is measured in terms of the specified unit.
             * Only a subset of units are accepted by this method.
             * The unit must either have an <b style="color: green;"> exact duration</b> or
             * be <b style="color: orange;"> ChronoUnit::DAYS</b> which is treated as 24 hours. Other units throw an exception.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param amountToSubtract  the amount to subtract, measured in terms of the unit, positive or negative
             * @param unit  the unit that the amount is measured in, must have an exact duration
             * @return a <b> Duration</b> based on this duration with the specified duration subtracted
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration minus(glong amountToSubtract, TemporalUnit unit) const;

            /**
             * Returns a copy of this duration with the specified duration in standard 24 hour days subtracted.
             * <p>
             * The number of days is multiplied by 86400 to obtain the number of seconds to subtract.
             * This is based on the standard definition of a day as 24 hours.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param daysToSubtract  the days to subtract, positive or negative
             * @return a <b> Duration</b> based on this duration with the specified days subtracted
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration minusDays(glong daysToSubtract) const;

            /**
             * Returns a copy of this duration with the specified duration in hours subtracted.
             * <p>
             * The number of hours is multiplied by 3600 to obtain the number of seconds to subtract.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param hoursToSubtract  the hours to subtract, positive or negative
             * @return a <b> Duration</b> based on this duration with the specified hours subtracted
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration minusHours(glong hoursToSubtract) const;

            /**
             * Returns a copy of this duration with the specified duration in minutes subtracted.
             * <p>
             * The number of hours is multiplied by 60 to obtain the number of seconds to subtract.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param minutesToSubtract  the minutes to subtract, positive or negative
             * @return a <b> Duration</b> based on this duration with the specified minutes subtracted
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration minusMinutes(glong minutesToSubtract) const;

            /**
             * Returns a copy of this duration with the specified duration in seconds subtracted.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param secondsToSubtract  the seconds to subtract, positive or negative
             * @return a <b> Duration</b> based on this duration with the specified seconds subtracted
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration minusSeconds(glong secondsToSubtract) const;

            /**
             * Returns a copy of this duration with the specified duration in milliseconds subtracted.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param millisToSubtract  the milliseconds to subtract, positive or negative
             * @return a <b> Duration</b> based on this duration with the specified milliseconds subtracted
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration minusMillis(glong millisToSubtract) const;

            /**
             * Returns a copy of this duration with the specified duration in nanoseconds subtracted.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param nanosToSubtract  the nanoseconds to subtract, positive or negative
             * @return a <b> Duration</b> based on this duration with the specified nanoseconds subtracted
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration minusNanos(glong nanosToSubtract) const;

            /**
             * Returns a copy of this duration multiplied by the scalar.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param multiplicand  the value to multiply the duration by, positive or negative
             * @return a <b> Duration</b> based on this duration multiplied by the specified scalar
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration multipliedBy(glong multiplicand) const;

            Duration operator*(glong multiplicand) const;

            /**
             * Returns a copy of this duration divided by the specified value.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param divisor  the value to divide the duration by, positive or negative, not zero
             * @return a <b> Duration</b> based on this duration divided by the specified divisor
             * @throws ArithmeticException if the divisor is zero or if numeric overflow occurs
             */
            Duration dividedBy(glong divisor) const;

            Duration operator/(glong divisor) const;

            /**
             * Returns number of whole times a specified Duration occurs within this Duration.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param divisor the value to divide the duration by, positive or negative
             * @return number of whole times, rounded toward zero, a specified
             *         <b> Duration</b> occurs within this Duration, may be negative
             * @throws ArithmeticException if the divisor is zero, or if numeric overflow occurs
             */
            glong dividedBy(const Duration &divisor) const;

            glong operator/(const Duration &divisor) const;

            /**
             * Returns a copy of this duration with the length negated.
             * <p>
             * This method swaps the sign of the total length of this duration.
             * For example, <b> PT1.3S</b> will be returned as <b> PT-1.3S</b>.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @return a <b> Duration</b> based on this duration with the amount negated
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration negated() const;

            Duration operator-() const;

            /**
             * Returns a copy of this duration with a positive length.
             * <p>
             * This method returns a positive duration by effectively removing the sign from any negative total length.
             * For example, <b> PT-1.3S</b> will be returned as <b> PT1.3S</b>.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @return a <b> Duration</b> based on this duration with an absolute length
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration abs() const;

            /**
             * Gets the number of days in this duration.
             * <p>
             * This returns the total number of days in the duration by dividing the
             * number of seconds by 86400.
             * This is based on the standard definition of a day as 24 hours.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @return the number of days in the duration, may be negative
             */
            glong toDays() const;

            /**
             * Gets the number of hours in this duration.
             * <p>
             * This returns the total number of hours in the duration by dividing the
             * number of seconds by 3600.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @return the number of hours in the duration, may be negative
             */
            glong toHours() const;

            /**
             * Gets the number of minutes in this duration.
             * <p>
             * This returns the total number of minutes in the duration by dividing the
             * number of seconds by 60.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @return the number of minutes in the duration, may be negative
             */
            glong toMinutes() const;

            /**
             * Gets the number of seconds in this duration.
             * <p>
             * This returns the total number of whole seconds in the duration.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @return the whole seconds part of the length of the duration, positive or negative
             */
            glong toSeconds() const;

            /**
             * Converts this duration to the total length in milliseconds.
             * <p>
             * If this duration is too large to fit in a <b> long</b> milliseconds, then an
             * exception is thrown.
             * <p>
             * If this duration has greater than millisecond precision, then the conversion
             * will drop any excess precision information as though the amount in nanoseconds
             * was subject to integer division by one million.
             *
             * @return the total length of the duration in milliseconds
             * @throws ArithmeticException if numeric overflow occurs
             */
            glong toMillis() const;

            /**
             * Converts this duration to the total length in nanoseconds expressed as a <b> long</b>.
             * <p>
             * If this duration is too large to fit in a <b> long</b> nanoseconds, then an
             * exception is thrown.
             *
             * @return the total length of the duration in nanoseconds
             * @throws ArithmeticException if numeric overflow occurs
             */
            glong toNanos() const;

            /**
             * Extracts the number of days in the duration.
             * <p>
             * This returns the total number of days in the duration by dividing the
             * number of seconds by 86400.
             * This is based on the standard definition of a day as 24 hours.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * @apiNote
             * This method behaves exactly the same way as <b style="color: orange;"> Duration::toDays()</b>.
             *
             * @return the number of days in the duration, may be negative
             */
            glong toDaysPart() const;

            /**
             * Extracts the number of hours part in the duration.
             * <p>
             * This returns the number of remaining hours when dividing <b style="color: orange;"> Duration::toHours</b>
             * by hours in a day.
             * This is based on the standard definition of a day as 24 hours.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @return the number of hours part in the duration, may be negative
             */
            gint toHoursPart() const;

            /**
             * Extracts the number of minutes part in the duration.
             * <p>
             * This returns the number of remaining minutes when dividing <b style="color: orange;"> Duration::toMinutes</b>
             * by minutes in an hour.
             * This is based on the standard definition of an hour as 60 minutes.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @return the number of minutes parts in the duration, may be negative
             */
            gint toMinutesPart() const;

            /**
             * Extracts the number of seconds part in the duration.
             * <p>
             * This returns the remaining seconds when dividing <b style="color: orange;"> Duration::toSeconds</b>
             * by seconds in a minute.
             * This is based on the standard definition of a minute as 60 seconds.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @return the number of seconds parts in the duration, may be negative
             */
            gint toSecondsPart() const;

            /**
             * Extracts the number of milliseconds part of the duration.
             * <p>
             * This returns the milliseconds part by dividing the number of nanoseconds by 1,000,000.
             * The length of the duration is stored using two fields - seconds and nanoseconds.
             * The nanoseconds part is a value from 0 to 999,999,999 that is an adjustment to
             * the length in seconds.
             * The total duration is defined by calling <b style="color: orange;"> Duration::nanos()</b> and <b style="color: orange;"> Duration::seconds()</b>.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @return the number of milliseconds part of the duration.
             */
            gint toMillisPart() const;

            /**
             * Get the nanoseconds part within seconds of the duration.
             * <p>
             * The length of the duration is stored using two fields - seconds and nanoseconds.
             * The nanoseconds part is a value from 0 to 999,999,999 that is an adjustment to
             * the length in seconds.
             * The total duration is defined by calling <b style="color: orange;"> Duration::nanos()</b> and <b style="color: orange;"> Duration::seconds()</b>.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @return the nanoseconds within the second part of the length of the duration, from 0 to 999,999,999
             */
            gint toNanosPart() const;

            /**
             * Returns a copy of this <b> Duration</b> truncated to the specified unit.
             * <p>
             * Truncating the duration returns a copy of the original with conceptual fields
             * smaller than the specified unit setValue to zero.
             * For example, truncating with the <b style="color: orange;"> ChronoUnit::MINUTES</b> unit will
             * round down towards zero to the nearest minute, setting the seconds and
             * nanoseconds to zero.
             * <p>
             * The unit must have a <b style="color: green;"> duration</b>
             * that divides into the length of a standard day without remainder.
             * This includes all
             * <b style="color: green;"> time-based units on <b> ChronoUnit</b></b>
             * and <b style="color: orange;"> ChronoUnit::DAYS DAYS</b>. Other ChronoUnits throw an exception.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param unit the unit to truncate to
             * @return a <b> Duration</b> based on this duration with the time truncated
             * @throws DateTimeException if the unit is invalid for truncation
             * @throws UnsupportedTemporalTypeException if the unit is not supported
             */
            Duration truncateTo(TemporalUnit unit) const;

            /**
             * Compares this duration to the specified <b> Duration</b>.
             * <p>
             * The comparison is based on the total length of the durations.
             * It is "consistent with equals", as defined by <b style="color: orange;"> Comparable</b>.
             *
             * @param otherDuration the other duration to compare to
             * @return the comparator value, negative if less, positive if greater
             */
            gint compareTo(const Duration &other) const override;

            /**
             * Checks if this duration is equal to the specified <b> Duration</b>.
             * <p>
             * The comparison is based on the total length of the durations.
             *
             * @param other the other duration, null returns false
             * @return true if the other duration is equal to this one
             */
            gbool equals(const Object &o) const override;

            /**
             * A hash code for this duration.
             *
             * @return a suitable hash code
             */
            gint hash() const override;

            /**
             * A string representation of this duration using ISO-8601 seconds
             * based representation, such as <b> PT8H6M12.345S</b>.
             * <p>
             * The format of the returned string will be <b> PTnHnMnS</b>, where n is
             * the relevant hours, minutes or seconds part of the duration.
             * Any fractional seconds are placed after a decimal point in the seconds section.
             * If a section has a zero value, it is omitted.
             * The hours, minutes and seconds will all have the same sign.
             * <p>
             * Examples:
             * <pre>
             *    "20.345 seconds"                 -- "PT20.345S
             *    "15 minutes" (15 * 60 seconds)   -- "PT15M"
             *    "10 hours" (10 * 3600 seconds)   -- "PT10H"
             *    "2 days" (2 * 86400 seconds)     -- "PT48H"
             * </pre>
             * Note that multiples of 24 hours are not output as days to avoid confusion
             * with <b> Period</b>.
             *
             * @return an ISO-8601 representation of this duration
             */
            String toString() const override;

            glong until(const Temporal &endExclusive, TemporalUnit unit) const override;

            static Duration ofUnit(ChronoUnit unit);
        };

    } // time
} // core

#endif //CORE23_DURATION_H
