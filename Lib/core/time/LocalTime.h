//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_LOCALTIME_H
#define CORE23_LOCALTIME_H

#include <core/time/Temporal.h>
#include <core/Comparable.h>
#include "UnsupportedTemporalException.h"

namespace core {
    namespace time {

        /**
         * A time without a time-zone in the ISO-8601 calendar system,
         * such as <b> 10:15:30</b> .
         * <p>
         * <b> LocalTime</b>  is an immutable date-time object that represents a time,
         * often viewed as hour-minute-second.
         * Time is represented to nanosecond precision.
         * For example, the value "13:45.30.123456789" can be stored in a <b> LocalTime</b> .
         * <p>
         * This class does not store or represent a date or time-zone.
         * Instead, it is a description of the local time as seen on a wall clock.
         * It cannot represent an instant on the time-line without additional information
         * such as an offset or time-zone.
         * <p>
         * The ISO-8601 calendar system is the modern civil calendar system used today
         * in most of the world. This API assumes that all calendar systems use the same
         * representation, this class, for time-of-day.
         * <p>
         * This is a <a href="">value-based</a>
         * class; programmers should treat instances that are
         * <b style="color:green;"> equal</b>  as interchangeable and should not
         * use instances for synchronization, or unpredictable behavior may
         * occur. For example, in a future release, synchronization may fail.
         * The <b> equals</b>  method should be used for comparisons.
         *
         * @implSpec
         * This class is immutable and thread-safe.
         *
         */
        class LocalTime CORE_FINAL : public Temporal, public Comparable<LocalTime> {

        private:
            /**
             * LocalTime fields
             * mask = 0x1FLL &lt;&lt; 42 | 0x3FLL &lt;&lt; 36 | 0x3FLL &lt;&lt; 30 | 0x3FFFFFFF
             * hhmmssSSSSSSSSS = hh &lt;&lt; 42 | mm &lt;&lt; 36 | ss &lt;&lt; 30 | SSSSSSSSS
             */
            glong hhmmssSSSSSSSSS = {};

        public:

            /**
             * The minimum supported <b> LocalTime</b> , '00:00'.
             * This is the time of midnight at the start of the day.
             */
            static const LocalTime MIN;

            /**
             * The maximum supported <b> LocalTime</b> , '23:59:59.999999999'.
             * This is the time just before midnight at the end of the day.
             */
            static const LocalTime MAX;

            /**
             * The time of midnight at the start of the day, '00:00'.
             */
            static const LocalTime MIDNIGHT;

            /**
             * The time of noon in the middle of the day, '12:00'.
             */
            static const LocalTime NOON;

            /**
             * Hours per day.
             */
            static CORE_FAST gint HOURS_PER_DAY = 24;
            /**
             * Minutes per hour.
             */
            static CORE_FAST gint MINUTES_PER_HOUR = 60;
            /**
             * Minutes per day.
             */
            static CORE_FAST gint MINUTES_PER_DAY = MINUTES_PER_HOUR * HOURS_PER_DAY;
            /**
             * Seconds per minute.
             */
            static CORE_FAST gint SECONDS_PER_MINUTE = 60;
            /**
             * Seconds per hour.
             */
            static CORE_FAST gint SECONDS_PER_HOUR = SECONDS_PER_MINUTE * MINUTES_PER_HOUR;
            /**
             * Seconds per day.
             */
            static CORE_FAST gint SECONDS_PER_DAY = SECONDS_PER_HOUR * HOURS_PER_DAY;
            /**
             * Milliseconds per second.
             */
            static CORE_FAST glong MILLIS_PER_SECOND = 1000L;
            /**
             * Milliseconds per day.
             */
            static CORE_FAST glong MILLIS_PER_DAY = MILLIS_PER_SECOND * SECONDS_PER_DAY;
            /**
             * Microseconds per millisecond.
             */
            static CORE_FAST glong MICROS_PER_MILLI = 1000L;
            /**
             * Microseconds per second.
             */
            static CORE_FAST glong MICROS_PER_SECOND = 1000000L;
            /**
             * Microseconds per day.
             */
            static CORE_FAST glong MICROS_PER_DAY = MICROS_PER_SECOND * SECONDS_PER_DAY;
            /**
             * Nanos per microsecond.
             */
            static CORE_FAST glong NANOS_PER_MICRO = 1000L;
            /**
             * Nanos per millisecond.
             */
            static CORE_FAST glong NANOS_PER_MILLI = 1000000L;
            /**
             * Nanos per second.
             */
            static CORE_FAST glong NANOS_PER_SECOND = 1000000000L;
            /**
             * Nanos per minute.
             */
            static CORE_FAST glong NANOS_PER_MINUTE = NANOS_PER_SECOND * SECONDS_PER_MINUTE;
            /**
             * Nanos per hour.
             */
            static CORE_FAST glong NANOS_PER_HOUR = NANOS_PER_MINUTE * MINUTES_PER_HOUR;
            /**
             * Nanos per day.
             */
            static CORE_FAST glong NANOS_PER_DAY = NANOS_PER_HOUR * HOURS_PER_DAY;

            //-----------------------------------------------------------------------
            /**
             * Obtains the current time from the system clock in the default time-zone.
             * <p>
             * This will query the <b style="color:orange;"> system clock</b>  in the default
             * time-zone to obtain the current time.
             * <p>
             * Using this method will prevent the ability to use an alternate clock for testing
             * because the clock is hard-coded.
             *
             * @return the current time using the system clock and default time-zone
             */
            static LocalTime now();

            /**
             * Obtains the current time from the system clock in the specified time-zone.
             * <p>
             * This will query the <b style="color:orange;"> system clock</b>  to obtain the current time.
             * Specifying the time-zone avoids dependence on the default time-zone.
             * <p>
             * Using this method will prevent the ability to use an alternate clock for testing
             * because the clock is hard-coded.
             *
             * @param zone  the zone ID to use
             * @return the current time using the system clock
             */
            static LocalTime now(const ZoneID &zone);

            //-----------------------------------------------------------------------
            /**
             * Obtains an INSTANCE of <b> LocalTime</b>  from an hour and minute.
             * <p>
             * This returns a <b> LocalTime</b>  with the specified hour and minute.
             * The second and nanosecond fields will be setValue to zero.
             *
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @return the local time
             * @throws DateTimeException if the value of any field is out of range
             */
            CORE_EXPLICIT LocalTime(gint hour, gint minute);

            /**
             * Obtains an INSTANCE of <b> LocalTime</b>  from an hour, minute and second.
             * <p>
             * This returns a <b> LocalTime</b>  with the specified hour, minute and second.
             * The nanosecond field will be setValue to zero.
             *
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @param second  the second-of-minute to represent, from 0 to 59
             * @return the local time
             * @throws DateTimeException if the value of any field is out of range
             */
            CORE_EXPLICIT LocalTime(gint hour, gint minute, gint second);

            /**
             * Obtains an INSTANCE of <b> LocalTime</b>  from an hour, minute, second and nanosecond.
             * <p>
             * This returns a <b> LocalTime</b>  with the specified hour, minute, second and nanosecond.
             *
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @param second  the second-of-minute to represent, from 0 to 59
             * @param nanoOfSecond  the nano-of-second to represent, from 0 to 999,999,999
             * @return the local time
             * @throws DateTimeException if the value of any field is out of range
             */
            CORE_EXPLICIT LocalTime(gint hour, gint minute, gint second, gint nanoOfSecond);

            //-----------------------------------------------------------------------
            /**
             * Obtains an INSTANCE of <b> LocalTime</b>  from a second-of-day value.
             * <p>
             * This returns a <b> LocalTime</b>  with the specified second-of-day.
             * The nanosecond field will be setValue to zero.
             *
             * @param secondOfDay  the second-of-day, from <b> 0</b>  to <b> 24 * 60 * 60 - 1</b> 
             * @return the local time
             * @throws DateTimeException if the second-of-day value is invalid
             */
            static LocalTime ofSecondOfDay(glong secondOfDay);

            /**
             * Obtains an INSTANCE of <b> LocalTime</b>  from a nanos-of-day value.
             * <p>
             * This returns a <b> LocalTime</b>  with the specified nanosecond-of-day.
             *
             * @param nanoOfDay  the nano of day, from <b> 0</b>  to <b> 24 * 60 * 60 * 1,000,000,000 - 1</b> 
             * @return the local time
             * @throws DateTimeException if the nanos of day value is invalid
             */
            static LocalTime ofNanoOfDay(glong nanoOfDay);

            //-----------------------------------------------------------------------

            /**
             * Obtains an INSTANCE of <b> LocalTime</b> from a temporal object.
             * <p>
             * This obtains a local time based on the specified temporal.
             * A <b> Temporal</b> represents an arbitrary setValue of date and time information,
             * which this factory converts to an INSTANCE of <b> LocalTime</b>.
             * <p>
             * The conversion uses the <b style="color:orange;"> Temporal::LOCAL_TIME</b> query, which relies
             * on extracting the <b style="color:orange;"> ChronoField#NANO_OF_DAY NANO_OF_DAY</b> field.
             * <p>
             * This method matches the signature of the functional interface <b style="color:orange;"> TemporalQuery</b>
             * allowing it to be used as a query via method reference, <b> LocalTime::from</b>.
             *
             * @param temporal  the temporal object to convert
             * @return the local time
             * @throws DateTimeException if unable to convert to a <b> LocalTime</b>
             */
            static LocalTime from(const Temporal& temporal);

            /**
             * Obtains an INSTANCE of <b> LocalTime</b>  from a text string such as <b> 10:15</b> .
             * <p>
             * The string must represent a valid time and is parsed using
             * <b style="color:orange;"> DateTimeFormatter.ISO_LOCAL_TIME</b> .
             *
             * @param text  the text to parse such as "10:15:30"
             * @return the parsed local time
             * @throws DateTimeParseException if the text cannot be parsed
             */
            static LocalTime parse(const String &text);

            /**
             * Obtains an INSTANCE of <b> LocalTime</b>  from a text string using a specific formatter.
             * <p>
             * The text is parsed using the formatter, returning a time.
             *
             * @param text  the text to parse
             * @param formatter  the formatter to use
             * @return the parsed local time
             * @throws DateTimeParseException if the text cannot be parsed
             */
            static LocalTime parse(const String &text, const DateTimeFormatter &formatter);

            /**
             * Checks if the specified field is supported.
             * <p>
             * This checks if this time can be queried for the specified field.
             * If false, then calling the <b style="color:orange;"> range</b> ,
             * <b style="color:orange;"> get</b>  and <b style="color:orange;"> with(Field, glong)</b>
             * methods will throw an exception.
             * <p>
             * If the field is a <b style="color:orange;"> Field</b>  then the query is implemented here.
             * The supported fields are:
             * <ul>
             * <li><b> NANO_OF_SECOND</b> 
             * <li><b> NANO_OF_DAY</b> 
             * <li><b> MICRO_OF_SECOND</b> 
             * <li><b> MICRO_OF_DAY</b> 
             * <li><b> MILLI_OF_SECOND</b> 
             * <li><b> MILLI_OF_DAY</b> 
             * <li><b> SECOND_OF_MINUTE</b> 
             * <li><b> SECOND_OF_DAY</b> 
             * <li><b> MINUTE_OF_HOUR</b> 
             * <li><b> MINUTE_OF_DAY</b> 
             * <li><b> HOUR_OF_AMPM</b> 
             * <li><b> CLOCK_HOUR_OF_AMPM</b> 
             * <li><b> HOUR_OF_DAY</b> 
             * <li><b> CLOCK_HOUR_OF_DAY</b> 
             * <li><b> AMPM_OF_DAY</b> 
             * </ul>
             * All other <b> Field</b>  instances will return false.
             *
             * @param field  the field to check, null returns false
             * @return true if the field is supported on this time, false if not
             */

            gbool supportField(TemporalField field) const override;

            /**
             * Checks if the specified unit is supported.
             * <p>
             * This checks if the specified unit can be added to, or subtracted from, this time.
             * If false, then calling the <b style="color:orange;"> after(glong, Unit)</b>  and
             * <b style="color:orange;"> before(glong, Unit) before</b>  methods will throw an exception.
             * <p>
             * If the unit is a <b style="color:orange;"> Unit</b>  then the query is implemented here.
             * The supported units are:
             * <ul>
             * <li><b> NANOS</b> 
             * <li><b> MICROS</b> 
             * <li><b> MILLIS</b> 
             * <li><b> SECONDS</b> 
             * <li><b> MINUTES</b> 
             * <li><b> HOURS</b> 
             * <li><b> HALF_DAYS</b> 
             * </ul>
             * All other <b> Unit</b>  instances will return false.
             *
             * @param unit  the unit to check, null returns false
             * @return true if the unit can be added/subtracted, false if not
             */
            gbool supportUnit(TemporalUnit unit) const override;

            //-----------------------------------------------------------------------

            ;

            /**
             * Gets the value of the specified field from this time as an <b> gint</b> .
             * <p>
             * This queries this time for the value of the specified field.
             * The returned value will always be within the valid range of values for the field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoField</b>  then the query is implemented here.
             * The <b style="color:orange;"> supported fields</b>  will return valid
             * values based on this time, except <b> NANO_OF_DAY</b>  and <b> MICRO_OF_DAY</b> 
             * which are too large to fit in an <b> gint</b>  and throw an <b> UnsupportedTemporalException</b> .
             * All other <b> ChronoField</b>  instances will throw an <b> UnsupportedTemporalException</b> .
             * <p>
             * If the field is not a <b> ChronoField</b> , then the result of this method
             * is obtained by invoking <b> TemporalField.getFrom(Temporal)</b>
             * passing <b> this</b>  as the argument. Whether the value can be obtained,
             * and what the value represents, is determined by the field.
             *
             * @param field  the field to get
             * @return the value for the field
             * @throws DateTimeException if a value for the field cannot be obtained or
             *         the value is outside the range of valid values for the field
             * @throws UnsupportedTemporalException if the field is not supported or
             *         the range of values exceeds an <b> gint</b>
             * @throws ArithmeticException if numeric overflow occurs
             */
            gint get(TemporalField field) const override;


            /**
             * Gets the value of the specified field from this time as a <b> glong</b> .
             * <p>
             * This queries this time for the value of the specified field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * <p>
             * If the field is a <b style="color:orange;"> Field</b>  then the query is implemented here.
             * The <b style="color:orange;"> supported fields</b>  will return valid
             * values based on this time.
             * All other <b> Field</b>  instances will throw an <b> UnsupportedTemporalException</b> .
             *
             * @param field  the field to get
             * @return the value for the field
             * @throws DateTimeException if a value for the field cannot be obtained
             * @throws UnsupportedTemporalException if the field is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */

            glong getLong(TemporalField field) const override;

            //-----------------------------------------------------------------------
            /**
             * Gets the hour-of-day field.
             *
             * @return the hour-of-day, from 0 to 23
             */
            gint hour() const;

            /**
             * Gets the minute-of-hour field.
             *
             * @return the minute-of-hour, from 0 to 59
             */
            gint minute() const;

            /**
             * Gets the second-of-minute field.
             *
             * @return the second-of-minute, from 0 to 59
             */
            gint second() const;

            /**
             * Gets the nano-of-second field.
             *
             * @return the nano-of-second, from 0 to 999,999,999
             */
            gint nano() const;

            /**
             * Returns a copy of this time with the specified field setValue to a new value.
             * <p>
             * This returns a <b> LocalTime</b> , based on this one, with the value
             * for the specified field changed.
             * This can be used to change any supported field, such as the hour, minute or second.
             * If it is not possible to setValue the value, because the field is not supported or for
             * some other reason, an exception is thrown.
             * <p>
             * If the field is a <b style="color:orange;"> Field</b>  then the adjustment is implemented here.
             * The supported fields behave as follows:
             * <ul>
             * <li><b> NANO_OF_SECOND</b>  -
             *  Returns a <b> LocalTime</b>  with the specified nano-of-second.
             *  The hour, minute and second will be unchanged.
             * <li><b> NANO_OF_DAY</b>  -
             *  Returns a <b> LocalTime</b>  with the specified nano-of-day.
             *  This completely replaces the time and is equivalent to <b style="color:orange;"> ofNanoOfDay(glong)</b> .
             * <li><b> MICRO_OF_SECOND</b>  -
             *  Returns a <b> LocalTime</b>  with the nano-of-second replaced by the specified
             *  micro-of-second multiplied by 1,000.
             *  The hour, minute and second will be unchanged.
             * <li><b> MICRO_OF_DAY</b>  -
             *  Returns a <b> LocalTime</b>  with the specified micro-of-day.
             *  This completely replaces the time and is equivalent to using <b style="color:orange;"> ofNanoOfDay(glong)</b>
             *  with the micro-of-day multiplied by 1,000.
             * <li><b> MILLI_OF_SECOND</b>  -
             *  Returns a <b> LocalTime</b>  with the nano-of-second replaced by the specified
             *  milli-of-second multiplied by 1,000,000.
             *  The hour, minute and second will be unchanged.
             * <li><b> MILLI_OF_DAY</b>  -
             *  Returns a <b> LocalTime</b>  with the specified milli-of-day.
             *  This completely replaces the time and is equivalent to using <b style="color:orange;"> ofNanoOfDay(glong)</b>
             *  with the milli-of-day multiplied by 1,000,000.
             * <li><b> SECOND_OF_MINUTE</b>  -
             *  Returns a <b> LocalTime</b>  with the specified second-of-minute.
             *  The hour, minute and nano-of-second will be unchanged.
             * <li><b> SECOND_OF_DAY</b>  -
             *  Returns a <b> LocalTime</b>  with the specified second-of-day.
             *  The nano-of-second will be unchanged.
             * <li><b> MINUTE_OF_HOUR</b>  -
             *  Returns a <b> LocalTime</b>  with the specified minute-of-hour.
             *  The hour, second-of-minute and nano-of-second will be unchanged.
             * <li><b> MINUTE_OF_DAY</b>  -
             *  Returns a <b> LocalTime</b>  with the specified minute-of-day.
             *  The second-of-minute and nano-of-second will be unchanged.
             * <li><b> HOUR_OF_AMPM</b>  -
             *  Returns a <b> LocalTime</b>  with the specified hour-of-am-pm.
             *  The AM/PM, minute-of-hour, second-of-minute and nano-of-second will be unchanged.
             * <li><b> CLOCK_HOUR_OF_AMPM</b>  -
             *  Returns a <b> LocalTime</b>  with the specified clock-hour-of-am-pm.
             *  The AM/PM, minute-of-hour, second-of-minute and nano-of-second will be unchanged.
             * <li><b> HOUR_OF_DAY</b>  -
             *  Returns a <b> LocalTime</b>  with the specified hour-of-day.
             *  The minute-of-hour, second-of-minute and nano-of-second will be unchanged.
             * <li><b> CLOCK_HOUR_OF_DAY</b>  -
             *  Returns a <b> LocalTime</b>  with the specified clock-hour-of-day.
             *  The minute-of-hour, second-of-minute and nano-of-second will be unchanged.
             * <li><b> AMPM_OF_DAY</b>  -
             *  Returns a <b> LocalTime</b>  with the specified AM/PM.
             *  The hour-of-am-pm, minute-of-hour, second-of-minute and nano-of-second will be unchanged.
             * </ul>
             * <p>
             * In all cases, if the new value is outside the valid range of values for the field
             * then a <b> DateTimeException</b>  will be thrown.
             * <p>
             * All other <b> Field</b>  instances will throw an <b> UnsupportedTemporalException</b> .
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param field  the field to setValue in the result
             * @param newValue  the new value of the field in the result
             * @return a <b> LocalTime</b>  based on <b> this</b>  with the specified field setValue
             * @throws DateTimeException if the field cannot be setValue
             * @throws UnsupportedTemporalException if the field is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */

            LocalTime with(Field field, glong newValue) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this <b> LocalTime</b>  with the hour-of-day altered.
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param hour  the hour-of-day to setValue in the result, from 0 to 23
             * @return a <b> LocalTime</b>  based on this time with the requested hour
             * @throws DateTimeException if the hour value is invalid
             */
            LocalTime withHour(gint hour) const;

            /**
             * Returns a copy of this <b> LocalTime</b>  with the minute-of-hour altered.
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param minute  the minute-of-hour to setValue in the result, from 0 to 59
             * @return a <b> LocalTime</b>  based on this time with the requested minute
             * @throws DateTimeException if the minute value is invalid
             */
            LocalTime withMinute(gint minute) const;

            /**
             * Returns a copy of this <b> LocalTime</b>  with the second-of-minute altered.
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param second  the second-of-minute to setValue in the result, from 0 to 59
             * @return a <b> LocalTime</b>  based on this time with the requested second
             * @throws DateTimeException if the second value is invalid
             */
            LocalTime withSecond(gint second) const;

            /**
             * Returns a copy of this <b> LocalTime</b>  with the nano-of-second altered.
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param nanoOfSecond  the nano-of-second to setValue in the result, from 0 to 999,999,999
             * @return a <b> LocalTime</b>  based on this time with the requested nanosecond
             * @throws DateTimeException if the nanos value is invalid
             */
            LocalTime withNano(gint nanoOfSecond) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this <b> LocalTime</b>  with the time truncated.
             * <p>
             * Truncation returns a copy of the original time with fields
             * smaller than the specified unit setValue to zero.
             * For example, truncating with the <b style="color:orange;"> minutes</b>  unit
             * will setValue the second-of-minute and nano-of-second field to zero.
             * <p>
             * The unit must have a <b style="color:green;"> duration</b>
             * that divides into the length of a standard day without remainder.
             * This includes all supplied time units on <b style="color:orange;"> Unit</b>  and
             * <b style="color:orange;"> DAYS</b> . Other units throw an exception.
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param unit  the unit to truncate to
             * @return a <b> LocalTime</b>  based on this time with the time truncated
             * @throws DateTimeException if unable to truncate
             * @throws UnsupportedTemporalException if the unit is not supported
             */
            LocalTime truncatedTo(TemporalUnit unit) const;

            /**
             * Returns a copy of this time with the specified amount added.
             * <p>
             * This returns a <b> LocalTime</b> , based on this one, with the amount
             * in terms of the unit added. If it is not possible to add the amount, because the
             * unit is not supported or for some other reason, an exception is thrown.
             * <p>
             * If the field is a <b style="color:orange;"> Unit</b>  then the addition is implemented here.
             * The supported fields behave as follows:
             * <ul>
             * <li><b> NANOS</b>  -
             *  Returns a <b> LocalTime</b>  with the specified number of nanoseconds added.
             *  This is equivalent to <b style="color:orange;"> afterNanos(glong)</b> .
             * <li><b> MICROS</b>  -
             *  Returns a <b> LocalTime</b>  with the specified number of microseconds added.
             *  This is equivalent to <b style="color:orange;"> afterNanos(glong)</b>  with the amount
             *  multiplied by 1,000.
             * <li><b> MILLIS</b>  -
             *  Returns a <b> LocalTime</b>  with the specified number of milliseconds added.
             *  This is equivalent to <b style="color:orange;"> afterNanos(glong)</b>  with the amount
             *  multiplied by 1,000,000.
             * <li><b> SECONDS</b>  -
             *  Returns a <b> LocalTime</b>  with the specified number of seconds added.
             *  This is equivalent to <b style="color:orange;"> afterSeconds(glong)</b> .
             * <li><b> MINUTES</b>  -
             *  Returns a <b> LocalTime</b>  with the specified number of minutes added.
             *  This is equivalent to <b style="color:orange;"> afterMinutes(glong)</b> .
             * <li><b> HOURS</b>  -
             *  Returns a <b> LocalTime</b>  with the specified number of hours added.
             *  This is equivalent to <b style="color:orange;"> afterHours(glong)</b> .
             * <li><b> HALF_DAYS</b>  -
             *  Returns a <b> LocalTime</b>  with the specified number of half-days added.
             *  This is equivalent to <b style="color:orange;"> afterHours(glong)</b>  with the amount
             *  multiplied by 12.
             * </ul>
             * <p>
             * All other <b> Unit</b>  instances will throw an <b> UnsupportedTemporalException</b> .
             *
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param amountToAdd  the amount of the unit to add to the result, may be negative
             * @param unit  the unit of the amount to add
             * @return a <b> LocalTime</b>  based on this time with the specified amount added
             * @throws DateTimeException if the addition cannot be made
             * @throws UnsupportedTemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */

            LocalTime after(glong amountToAdd, TemporalUnit unit) const;

            /**
             * Returns a copy of this <b> LocalTime</b>  with the specified number of hours added.
             * <p>
             * This adds the specified number of hours to this time, returning a new time.
             * The calculation wraps around midnight.
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param hoursToAdd  the hours to add, may be negative
             * @return a <b> LocalTime</b>  based on this time with the hours added
             */
            LocalTime afterHours(glong hoursToAdd) const;

            /**
             * Returns a copy of this <b> LocalTime</b>  with the specified number of minutes added.
             * <p>
             * This adds the specified number of minutes to this time, returning a new time.
             * The calculation wraps around midnight.
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param minutesToAdd  the minutes to add, may be negative
             * @return a <b> LocalTime</b>  based on this time with the minutes added
             */
            LocalTime afterMinutes(glong minutesToAdd) const;

            /**
             * Returns a copy of this <b> LocalTime</b>  with the specified number of seconds added.
             * <p>
             * This adds the specified number of seconds to this time, returning a new time.
             * The calculation wraps around midnight.
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param secondstoAdd  the seconds to add, may be negative
             * @return a <b> LocalTime</b>  based on this time with the seconds added
             */
            LocalTime afterSeconds(glong secondstoAdd) const;

            /**
             * Returns a copy of this <b> LocalTime</b>  with the specified number of nanoseconds added.
             * <p>
             * This adds the specified number of nanoseconds to this time, returning a new time.
             * The calculation wraps around midnight.
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param nanosToAdd  the nanos to add, may be negative
             * @return a <b> LocalTime</b>  based on this time with the nanoseconds added
             */
            LocalTime afterNanos(glong nanosToAdd) const;

            /**
             * Returns a copy of this time with the specified amount subtracted.
             * <p>
             * This returns a <b> LocalTime</b> , based on this one, with the amount
             * in terms of the unit subtracted. If it is not possible to subtract the amount,
             * because the unit is not supported or for some other reason, an exception is thrown.
             * <p>
             * This method is equivalent to <b style="color:orange;"> after(glong, Unit)</b>  with the amount negated.
             * See that method for a full description of how addition, and thus subtraction, works.
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param amountToSubtract  the amount of the unit to subtract from the result, may be negative
             * @param unit  the unit of the amount to subtract
             * @return a <b> LocalTime</b>  based on this time with the specified amount subtracted
             * @throws DateTimeException if the subtraction cannot be made
             * @throws UnsupportedTemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */

            LocalTime before(glong amountToSubtract, TemporalUnit unit) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this <b> LocalTime</b>  with the specified number of hours subtracted.
             * <p>
             * This subtracts the specified number of hours from this time, returning a new time.
             * The calculation wraps around midnight.
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param hoursToSubtract  the hours to subtract, may be negative
             * @return a <b> LocalTime</b>  based on this time with the hours subtracted
             */
            LocalTime beforeHours(glong hoursToSubtract) const;

            /**
             * Returns a copy of this <b> LocalTime</b>  with the specified number of minutes subtracted.
             * <p>
             * This subtracts the specified number of minutes from this time, returning a new time.
             * The calculation wraps around midnight.
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param minutesToSubtract  the minutes to subtract, may be negative
             * @return a <b> LocalTime</b>  based on this time with the minutes subtracted
             */
            LocalTime beforeMinutes(glong minutesToSubtract) const;

            /**
             * Returns a copy of this <b> LocalTime</b>  with the specified number of seconds subtracted.
             * <p>
             * This subtracts the specified number of seconds from this time, returning a new time.
             * The calculation wraps around midnight.
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param secondsToSubtract  the seconds to subtract, may be negative
             * @return a <b> LocalTime</b>  based on this time with the seconds subtracted
             */
            LocalTime beforeSeconds(glong secondsToSubtract) const;

            /**
             * Returns a copy of this <b> LocalTime</b>  with the specified number of nanoseconds subtracted.
             * <p>
             * This subtracts the specified number of nanoseconds from this time, returning a new time.
             * The calculation wraps around midnight.
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param nanosToSubtract  the nanos to subtract, may be negative
             * @return a <b> LocalTime</b>  based on this time with the nanoseconds subtracted
             */
            LocalTime beforeNanos(glong nanosToSubtract) const;

            /**
             * Calculates the amount of time until another time in terms of the specified unit.
             * <p>
             * This calculates the amount of time between two <b> LocalTime</b> 
             * objects in terms of a single <b> Unit</b> .
             * The start and end points are <b> this</b>  and the specified time.
             * The result will be negative if the end is before the start.
             * The <b> Temporal</b>  passed to this method is converted to a
             * <b> LocalTime</b>  using <b style="color:orange;"> from(Temporal)</b> .
             * For example, the amount in hours between two times can be calculated
             * using <b> startTime.until(endTime, HOURS)</b> .
             * <p>
             * The calculation returns a whole number, representing the number of
             * complete units between the two times.
             * For example, the amount in hours between 11:30 and 13:29 will only
             * be one hour as it is one minute short of two hours.
             *
             * <p>
             * The calculation is implemented in this method for <b style="color:orange;"> Unit</b> .
             * The units <b> NANOS</b> , <b> MICROS</b> , <b> MILLIS</b> , <b> SECONDS</b> ,
             * <b> MINUTES</b> , <b> HOURS</b>  and <b> HALF_DAYS</b>  are supported.
             * Other <b> Unit</b>  values will throw an exception.
             * <p>
             * This INSTANCE is immutable and unaffected by this method call.
             *
             * @param endExclusive  the end time, exclusive, which is converted to a <b> LocalTime</b>
             * @param unit  the unit to measure the amount in
             * @return the amount of time between this time and the end time
             * @throws DateTimeException if the amount cannot be calculated, or the end
             *  temporal cannot be converted to a <b> LocalTime</b> 
             * @throws UnsupportedTemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */

            glong until(const Temporal &endExclusive, TemporalUnit unit) const override;

            /**
             * Formats this time using the specified formatter.
             * <p>
             * This time will be passed to the formatter to produce a string.
             *
             * @param formatter  the formatter to use
             * @return the formatted time string
             * @throws DateTimeException if an error occurs during printing
             */
            String format(const DateTimeFormatter &formatter) const;

            //-----------------------------------------------------------------------
            /**
             * Combines this time with a date to create a <b> LocalDateTime</b> .
             * <p>
             * This returns a <b> LocalDateTime</b>  formed from this time at the specified date.
             * All possible combinations of date and time are valid.
             *
             * @param date  the date to combine with
             * @return the local date-time formed from this time and the specified date
             */
            LocalDateTime atDate(const LocalDate &date) const;

            /**
             * Extracts the time as seconds of day,
             * from <b> 0</b>  to <b> 24 * 60 * 60 - 1</b> .
             *
             * @return the second-of-day equivalent to this time
             */
            gint toSecondOfDay() const;

            /**
             * Extracts the time as nanos of day,
             * from <b> 0</b>  to <b> 24 * 60 * 60 * 1,000,000,000 - 1</b> .
             *
             * @return the nano of day equivalent to this time
             */
            glong toNanoOfDay() const;

            /**
             * Converts this <b> LocalTime</b>  to the number of seconds since the epoch
             * of 1970-01-01T00:00:00Z.
             * <p>
             * This combines this local time with the specified date and
             * offset to calculate the epoch-second value, which is the
             * number of elapsed seconds from 1970-01-01T00:00:00Z.
             * Instants on the time-line after the epoch are positive, earlier
             * are negative.
             *
             * @param date the local date
             * @param offset the zone offset
             * @return the number of seconds since the epoch of 1970-01-01T00:00:00Z, may be negative
             */
            glong toEpochSecond(const LocalDate &date, const ZoneOffset &offset) const;

            //-----------------------------------------------------------------------
            /**
             * Compares this time to another time.
             * <p>
             * The comparison is based on the time-line position of the local times within a day.
             * It is "consistent with equals", as defined by <b style="color:orange;"> Comparable</b> .
             *
             * @param other  the other time to compare to
             * @return the comparator value, negative if less, positive if greater
             */

            gint compareTo(const LocalTime &other) const override;

            /**
             * Checks if this time is after the specified time.
             * <p>
             * The comparison is based on the time-line position of the time within a day.
             *
             * @param other  the other time to compare to
             * @return true if this is after the specified time
             */
            gbool isAfter(const LocalTime &other) const;

            /**
             * Checks if this time is before the specified time.
             * <p>
             * The comparison is based on the time-line position of the time within a day.
             *
             * @param other  the other time to compare to
             * @return true if this point is before the specified time
             */
            gbool isBefore(const LocalTime &other) const;

            //-----------------------------------------------------------------------
            /**
             * Checks if this time is equal to another time.
             * <p>
             * The comparison is based on the time-line position of the time within a day.
             * <p>
             * Only objects of type <b> LocalTime</b>  are compared, other types return false.
             * To compare the date of two <b> Temporal</b>  instances, use
             * <b style="color:orange;"> Field.NANO_OF_DAY</b>  as a comparator.
             *
             * @param obj  the object to check, null returns false
             * @return true if this is equal to the other time
             */

            gbool equals(const Object &obj) const override;

            /**
             * A hash code for this time.
             *
             * @return a suitable hash code
             */

            gint hash() const override;

            //-----------------------------------------------------------------------
            /**
             * Outputs this time as a <b> String</b> , such as <b> 10:15</b> .
             * <p>
             * The output will be one of the following ISO-8601 formats:
             * <ul>
             * <li><b> HH:mm</b> </li>
             * <li><b> HH:mm:ss</b> </li>
             * <li><b> HH:mm:ss.SSS</b> </li>
             * <li><b> HH:mm:ss.SSSSSS</b> </li>
             * <li><b> HH:mm:ss.SSSSSSSSS</b> </li>
             * </ul>
             * The format used will be the shortest that outputs the full value of
             * the time where the omitted parts are implied to be zero.
             *
             * @return a string representation of this time
             */

            String toString() const override;
        };

    } // core
} // time

#endif //CORE23_LOCALTIME_H
