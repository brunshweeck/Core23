//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_LOCALDATETIME_H
#define CORE23_LOCALDATETIME_H

#include <core/time/Temporal.h>
#include <core/time/LocalTime.h>
#include <core/time/LocalDate.h>

namespace core {
    namespace time {

        /**
         * A date-time without a time-zone in the ISO-8601 calendar system,
         * such as <b> 2007-12-03T10:15:30</b> .
         * <p>
         * <b> LocalDateTime</b>  is an immutable date-time object that represents a date-time,
         * often viewed as year-m-day-hour-minute-second. Other date and time fields,
         * such as day-of-year, day-of-week and week-of-year, can also be accessed.
         * Time is represented to nanosecond precision.
         * For example, the value "2nd October 2007 at 13:45.30.123456789" can be
         * stored in a <b> LocalDateTime</b> .
         * <p>
         * This class does not store or represent a time-zone.
         * Instead, it is a description of the date, as used for birthdays, combined with
         * the local time as seen on a wall clock.
         * It cannot represent an instant on the time-line without additional information
         * such as an offset or time-zone.
         * <p>
         * The ISO-8601 calendar system is the modern civil calendar system used today
         * in most of the world. It is equivalent to the proleptic Gregorian calendar
         * system, in which today's rules for leap years are applied for all time.
         * For most applications written today, the ISO-8601 rules are entirely suitable.
         * However, any application that makes use of historical dates, and requires them
         * to be accurate will find the ISO-8601 approach unsuitable.
         * <p>
         * This is a <a href="">value-based</a> class; programmers should treat instances that are
         * <b style="color:green;">equal</b>  as interchangeable and should not
         * use instances for synchronization, or unpredictable behavior may
         * occur. For example, in a future release, synchronization may fail.
         * The <b> equals</b>  method should be used for comparisons.
         *
         * @implSpec
         * This class is immutable and thread-safe.
         *
         */
        class LocalDateTime CORE_FINAL : public Temporal, public Comparable<LocalDateTime> {
        public:
            /**
             * The minimum supported <b> LocalDateTime</b> , '-999999999-01-01T00:00:00'.
             * This is the local date-time of midnight at the start of the minimum date.
             * This combines <b style="color:orange;"> LocalDate#MIN</b>  and <b style="color:orange;"> LocalTime#MIN</b> .
             * This could be used by an application as a "far past" date-time.
             */
            static const LocalDateTime MIN;

            /**
             * The maximum supported <b> LocalDateTime</b> , '+999999999-12-31T23:59:59.999999999'.
             * This is the local date-time just before midnight at the end of the maximum date.
             * This combines <b style="color:orange;"> LocalDate#MAX</b>  and <b style="color:orange;"> LocalTime#MAX</b> .
             * This could be used by an application as a "far future" date-time.
             */
            static const LocalDateTime MAX;

            /**
             * The epoch year <b> LocalDateTime</b>, '1970-01-01T00:00:00'.
             *
             */
            static const LocalDateTime EPOCH;

            CORE_ALIAS(Month, LocalDate::Month);
            CORE_ALIAS(DayOfWeek, LocalDate::DayOfWeek);

        private:

            /**
             * The date part.
             */
            LocalDate date;

            /**
             * The time part.
             */
            LocalTime time;

        public:

            //-----------------------------------------------------------------------
            /**
             * Obtains the current date-time from the system clock in the default time-zone.
             * <p>
             * This will query the <b style="color:orange;"> Clock#systemDefaultZone() system clock</b>  in the default
             * time-zone to obtain the current date-time.
             * <p>
             * Using this method will prevent the ability to use an alternate clock for testing
             * because the clock is hard-coded.
             *
             * @return the current date-time using the system clock and default time-zone
             */
             static LocalDateTime now();

            /**
             * Obtains the current date-time from the system clock in the specified time-zone.
             * <p>
             * This will query the <b style="color:orange;"> Clock#system(ZoneId) system clock</b>  to obtain the current date-time.
             * Specifying the time-zone avoids dependence on the default time-zone.
             * <p>
             * Using this method will prevent the ability to use an alternate clock for testing
             * because the clock is hard-coded.
             *
             * @param zone  the zone ID to use
             * @return the current date-time using the system clock
             */
             static LocalDateTime now(const ZoneID& zone);

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> LocalDateTime</b>  from year, m,
             * day, hour and minute, setting the second and nanosecond to zero.
             * <p>
             * This returns a <b> LocalDateTime</b>  with the specified year, m,
             * day-of-m, hour and minute.
             * The day must be valid for the year and m, otherwise an exception will be thrown.
             * The second and nanosecond fields will be set to zero.
             *
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the m-of-year to represent
             * @param dayOfMonth  the day-of-m to represent, from 1 to 31
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @return the local date-time
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-m is invalid for the m-year
             */
             CORE_EXPLICIT LocalDateTime(gint year, Month month, gint dayOfMonth, gint hour, gint minute);

            /**
             * Obtains an instance of <b> LocalDateTime</b>  from year, m,
             * day, hour, minute and second, setting the nanosecond to zero.
             * <p>
             * This returns a <b> LocalDateTime</b>  with the specified year, m,
             * day-of-m, hour, minute and second.
             * The day must be valid for the year and m, otherwise an exception will be thrown.
             * The nanosecond field will be set to zero.
             *
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the m-of-year to represent
             * @param dayOfMonth  the day-of-m to represent, from 1 to 31
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @param second  the second-of-minute to represent, from 0 to 59
             * @return the local date-time
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-m is invalid for the m-year
             */
             CORE_EXPLICIT LocalDateTime(gint year, Month month, gint dayOfMonth, gint hour, gint minute, gint second);

            /**
             * Obtains an instance of <b> LocalDateTime</b>  from year, m,
             * day, hour, minute, second and nanosecond.
             * <p>
             * This returns a <b> LocalDateTime</b>  with the specified year, m,
             * day-of-m, hour, minute, second and nanosecond.
             * The day must be valid for the year and m, otherwise an exception will be thrown.
             *
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the m-of-year to represent
             * @param dayOfMonth  the day-of-m to represent, from 1 to 31
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @param second  the second-of-minute to represent, from 0 to 59
             * @param nanoOfSecond  the nano-of-second to represent, from 0 to 999,999,999
             * @return the local date-time
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-m is invalid for the m-year
             */
             CORE_EXPLICIT LocalDateTime(gint year, Month month, gint dayOfMonth, gint hour, gint minute, gint second, gint nanoOfSecond);

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> LocalDateTime</b>  from year, m,
             * day, hour and minute, setting the second and nanosecond to zero.
             * <p>
             * This returns a <b> LocalDateTime</b>  with the specified year, m,
             * day-of-m, hour and minute.
             * The day must be valid for the year and m, otherwise an exception will be thrown.
             * The second and nanosecond fields will be set to zero.
             *
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the m-of-year to represent, from 1 (January) to 12 (December)
             * @param dayOfMonth  the day-of-m to represent, from 1 to 31
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @return the local date-time
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-m is invalid for the m-year
             */
             CORE_EXPLICIT LocalDateTime(gint year, gint month, gint dayOfMonth, gint hour, gint minute);

            /**
             * Obtains an instance of <b> LocalDateTime</b>  from year, m,
             * day, hour, minute and second, setting the nanosecond to zero.
             * <p>
             * This returns a <b> LocalDateTime</b>  with the specified year, m,
             * day-of-m, hour, minute and second.
             * The day must be valid for the year and m, otherwise an exception will be thrown.
             * The nanosecond field will be set to zero.
             *
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the m-of-year to represent, from 1 (January) to 12 (December)
             * @param dayOfMonth  the day-of-m to represent, from 1 to 31
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @param second  the second-of-minute to represent, from 0 to 59
             * @return the local date-time
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-m is invalid for the m-year
             */
             CORE_EXPLICIT LocalDateTime(gint year, gint month, gint dayOfMonth, gint hour, gint minute, gint second);

            /**
             * Obtains an instance of <b> LocalDateTime</b>  from year, m,
             * day, hour, minute, second and nanosecond.
             * <p>
             * This returns a <b> LocalDateTime</b>  with the specified year, m,
             * day-of-m, hour, minute, second and nanosecond.
             * The day must be valid for the year and m, otherwise an exception will be thrown.
             *
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the m-of-year to represent, from 1 (January) to 12 (December)
             * @param dayOfMonth  the day-of-m to represent, from 1 to 31
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @param second  the second-of-minute to represent, from 0 to 59
             * @param nanoOfSecond  the nano-of-second to represent, from 0 to 999,999,999
             * @return the local date-time
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-m is invalid for the m-year
             */
             CORE_EXPLICIT LocalDateTime(gint year, gint month, gint dayOfMonth, gint hour, gint minute, gint second, gint nanoOfSecond);

            /**
             * Obtains an instance of <b> LocalDateTime</b>  from a date and time.
             *
             * @param date  the local date
             * @param time  the local time
             * @return the local date-time
             */
             CORE_EXPLICIT LocalDateTime(const LocalDate &date, const LocalTime &time);

            /**
             * Obtains an instance of <b> LocalDateTime</b>  using seconds from the
             * epoch of 1970-01-01T00:00:00Z.
             * <p>
             * This allows the <b style="color:orange;"> epoch-second</b>  field
             * to be converted to a local date-time. This is primarily intended for
             * low-level conversions rather than general application usage.
             *
             * @param epochSecond  the number of seconds from the epoch of 1970-01-01T00:00:00Z
             * @param nanoOfSecond  the nanosecond within the second, from 0 to 999,999,999
             * @param offset  the zone offset
             * @return the local date-time
             * @throws DateTimeException if the result exceeds the supported range,
             *  or if the nano-of-second is invalid
             */
             static LocalDateTime ofEpochSecond(glong epochSecond, gint nanoOfSecond, const ZoneOffset &offset);

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> LocalDateTime</b> from a temporal object.
             * <p>
             * This obtains a local date-time based on the specified temporal.
             * A <b> Temporal</b> represents an arbitrary set of date and time information,
             * which this factory converts to an instance of <b> LocalDateTime</b>.
             * <p>
             * The conversion extracts and combines the <b> LocalDate</b> and the
             * <b> LocalTime</b> from the temporal object.
             * Implementations are permitted to perform optimizations such as accessing
             * those fields that are equivalent to the relevant objects.
             * <p>
             * This method matches the signature of the functional interface <b style="color:orange;"> TemporalQuery</b>
             * allowing it to be used as a query via method reference, <b> LocalDateTime::from</b>.
             *
             * @param temporal  the temporal object to convert
             * @return the local date-time
             * @throws DateTimeException if unable to convert to a <b> LocalDateTime</b>
             */
            static LocalDateTime from(const Temporal &temporal);

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> LocalDateTime</b>  from a text string such as <b> 2007-12-03T10:15:30</b> .
             * <p>
             * The string must represent a valid date-time and is parsed using
             * <b style="color:orange;"> java.time.format.DateTimeFormatter#ISO_LOCAL_DATE_TIME</b> .
             *
             * @param text  the text to parse such as "2007-12-03T10:15:30"
             * @return the parsed local date-time
             * @throws DateTimeParseException if the text cannot be parsed
             */
             static LocalDateTime parse(const String& text);

            /**
             * Obtains an instance of <b> LocalDateTime</b>  from a text string using a specific formatter.
             * <p>
             * The text is parsed using the formatter, returning a date-time.
             *
             * @param text  the text to parse
             * @param formatter  the formatter to use
             * @return the parsed local date-time
             * @throws DateTimeParseException if the text cannot be parsed
             */
             static LocalDateTime parse(const String& text, const DateTimeFormatter& formatter);

            //-----------------------------------------------------------------------
            /**
             * Checks if the specified field is supported.
             * <p>
             * This checks if this date-time can be queried for the specified field.
             * If false, then calling the <b style="color:orange;"> #range(TemporalField) range</b> ,
             * <b style="color:orange;"> get(TemporalField) get</b>  and <b style="color:orange;"> #with(TemporalField, glong)</b>
             * methods will throw an exception.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoField</b>  then the query is implemented here.
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
             * <li><b> DAY_OF_WEEK</b> 
             * <li><b> ALIGNED_DAY_OF_WEEK_IN_MONTH</b> 
             * <li><b> ALIGNED_DAY_OF_WEEK_IN_YEAR</b> 
             * <li><b> DAY_OF_MONTH</b> 
             * <li><b> DAY_OF_YEAR</b> 
             * <li><b> EPOCH_DAY</b> 
             * <li><b> ALIGNED_WEEK_OF_MONTH</b> 
             * <li><b> ALIGNED_WEEK_OF_YEAR</b> 
             * <li><b> MONTH_OF_YEAR</b> 
             * <li><b> PROLEPTIC_MONTH</b> 
             * <li><b> YEAR_OF_ERA</b> 
             * <li><b> YEAR</b> 
             * <li><b> ERA</b> 
             * </ul>
             * All other <b> ChronoField</b>  instances will return false.
             *
             * @param field  the field to check, null returns false
             * @return true if the field is supported on this date-time, false if not
             */
            gbool supportField(Field field) const override;

            /**
             * Checks if the specified unit is supported.
             * <p>
             * This checks if the specified unit can be added to, or subtracted from, this date-time.
             * If false, then calling the <b style="color:orange;"> after(glong, TemporalUnit)</b>  and
             * <b style="color:orange;"> minus(glong, TemporalUnit) minus</b>  methods will throw an exception.
             * <p>
             * If the unit is a <b style="color:orange;"> ChronoUnit</b>  then the query is implemented here.
             * The supported units are:
             * <ul>
             * <li><b> NANOS</b> 
             * <li><b> MICROS</b> 
             * <li><b> MILLIS</b> 
             * <li><b> SECONDS</b> 
             * <li><b> MINUTES</b> 
             * <li><b> HOURS</b> 
             * <li><b> HALF_DAYS</b> 
             * <li><b> DAYS</b> 
             * <li><b> WEEKS</b> 
             * <li><b> MONTHS</b> 
             * <li><b> YEARS</b> 
             * <li><b> DECADES</b> 
             * <li><b> CENTURIES</b> 
             * <li><b> MILLENNIA</b> 
             * <li><b> ERAS</b> 
             * </ul>
             * All other <b> ChronoUnit</b>  instances will return false.
             * <p>
             * If the unit is not a <b> ChronoUnit</b> , then the result of this method
             * is obtained by invoking <b> TemporalUnit.isSupportedBy(Temporal)</b> 
             * passing <b> this</b>  as the argument.
             * Whether the unit is supported is determined by the unit.
             *
             * @param unit  the unit to check, null returns false
             * @return true if the unit can be added/subtracted, false if not
             */
            gbool supportUnit(Unit unit) const override;

            /**
             * Gets the value of the specified field from this date-time as an <b> gint</b> .
             * <p>
             * This queries this date-time for the value of the specified field.
             * The returned value will always be within the valid range of values for the field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoField</b>  then the query is implemented here.
             * The <b style="color:orange;"> #isSupported(TemporalField) supported fields</b>  will return valid
             * values based on this date-time, except <b> NANO_OF_DAY</b> , <b> MICRO_OF_DAY</b> ,
             * <b> EPOCH_DAY</b>  and <b> PROLEPTIC_MONTH</b>  which are too large to fit in
             * an <b> gint</b>  and throw an <b> UnsupportedTemporalException</b> .
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
             * Gets the value of the specified field from this date-time as a <b> glong</b> .
             * <p>
             * This queries this date-time for the value of the specified field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoField</b>  then the query is implemented here.
             * The <b style="color:orange;"> #isSupported(TemporalField) supported fields</b>  will return valid
             * values based on this date-time.
             * All other <b> ChronoField</b>  instances will throw an <b> UnsupportedTemporalException</b> .
             * <p>
             * If the field is not a <b> ChronoField</b> , then the result of this method
             * is obtained by invoking <b> TemporalField.getFrom(Temporal)</b>
             * passing <b> this</b>  as the argument. Whether the value can be obtained,
             * and what the value represents, is determined by the field.
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
             * Gets the <b> LocalDate</b>  part of this date-time.
             * <p>
             * This returns a <b> LocalDate</b>  with the same year, m and day
             * as this date-time.
             *
             * @return the date part of this date-time
             */
             LocalDate toLocalDate() const;

            /**
             * Gets the year field.
             * <p>
             * This method returns the primitive <b> gint</b>  value for the year.
             * <p>
             * The year returned by this method is proleptic as per <b> get(YEAR)</b> .
             * To obtain the year-of-era, use <b> get(YEAR_OF_ERA)</b> .
             *
             * @return the year, from MIN_YEAR to MAX_YEAR
             */
             gint year() const;

            /**
             * Gets the m-of-year field using the <b> Month</b>  enum.
             * <p>
             * This method returns the enum <b style="color:orange;"> Month</b>  for the m.
             * This avoids confusion as to what <b> gint</b>  values mean.
             * If you need access to the primitive <b> gint</b>  value then the enum
             * provides the <b style="color:orange;"> gint value</b> .
             *
             * @return the m-of-year
             *
             */
             Month month() const;

            /**
             * Gets the day-of-m field.
             * <p>
             * This method returns the primitive <b> gint</b>  value for the day-of-m.
             *
             * @return the day-of-m, from 1 to 31
             */
             gint dayOfMonth() const;

            /**
             * Gets the day-of-year field.
             * <p>
             * This method returns the primitive <b> gint</b>  value for the day-of-year.
             *
             * @return the day-of-year, from 1 to 365, or 366 in a leap year
             */
             gint dayOfYear() const;

            /**
             * Gets the day-of-week field, which is an enum <b> DayOfWeek</b> .
             * <p>
             * This method returns the enum <b style="color:orange;"> DayOfWeek</b>  for the day-of-week.
             * This avoids confusion as to what <b> gint</b>  values mean.
             * If you need access to the primitive <b> gint</b>  value then the enum
             * provides the <b style="color:orange;"> gint value</b> .
             * <p>
             * Additional information can be obtained from the <b> DayOfWeek</b> .
             * This includes textual names of the values.
             *
             * @return the day-of-week
             */
             DayOfWeek dayOfWeek() const;

            //-----------------------------------------------------------------------
            /**
             * Gets the <b> LocalTime</b>  part of this date-time.
             * <p>
             * This returns a <b> LocalTime</b>  with the same hour, minute, second and
             * nanosecond as this date-time.
             *
             * @return the time part of this date-time
             */
             LocalTime toLocalTime() const;

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

            //-----------------------------------------------------------------------

            /**
             * Returns a copy of this date-time with the specified field set to a new value.
             * <p>
             * This returns a <b> LocalDateTime</b> , based on this one, with the value
             * for the specified field changed.
             * This can be used to change any supported field, such as the year, m or day-of-m.
             * If it is not possible to set the value, because the field is not supported or for
             * some other reason, an exception is thrown.
             * <p>
             * In some cases, changing the specified field can cause the resulting date-time to become invalid,
             * such as changing the m from 31st January to February would make the day-of-m invalid.
             * In cases like this, the field is responsible for resolving the date. Typically it will choose
             * the previous valid date, which would be the last valid day of February in this example.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoField</b>  then the adjustment is implemented here.
             * The <b style="color:orange;"> #isSupported(TemporalField) supported fields</b>  will behave as per
             * the matching method on <b style="color:orange;"> LocalDate#with(TemporalField, glong) LocalDate</b>
             * or <b style="color:orange;"> LocalTime#with(TemporalField, glong) LocalTime</b> .
             * All other <b> ChronoField</b>  instances will throw an <b> UnsupportedTemporalException</b> .
             * <p>
             * If the field is not a <b> ChronoField</b> , then the result of this method
             * is obtained by invoking <b> TemporalField.adjustInto(Temporal, glong)</b>
             * passing <b> this</b>  as the argument. In this case, the field determines
             * whether and how to adjust the instant.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param field  the field to set in the result
             * @param newValue  the new value of the field in the result
             * @return a <b> LocalDateTime</b>  based on <b> this</b>  with the specified field set
             * @throws DateTimeException if the field cannot be set
             * @throws UnsupportedTemporalException if the field is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
             LocalDateTime with(TemporalField field, glong newValue) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the year altered.
             * <p>
             * The time does not affect the calculation and will be the same in the result.
             * If the day-of-m is invalid for the year, it will be changed to the last valid day of the m.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param year  the year to set in the result, from MIN_YEAR to MAX_YEAR
             * @return a <b> LocalDateTime</b>  based on this date-time with the requested year
             * @throws DateTimeException if the year value is invalid
             */
             LocalDateTime withYear(gint year) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the m-of-year altered.
             * <p>
             * The time does not affect the calculation and will be the same in the result.
             * If the day-of-m is invalid for the year, it will be changed to the last valid day of the m.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param month  the m-of-year to set in the result, from 1 (January) to 12 (December)
             * @return a <b> LocalDateTime</b>  based on this date-time with the requested m
             * @throws DateTimeException if the m-of-year value is invalid
             */
             LocalDateTime withMonth(gint month) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the day-of-m altered.
             * <p>
             * If the resulting date-time is invalid, an exception is thrown.
             * The time does not affect the calculation and will be the same in the result.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param dayOfMonth  the day-of-m to set in the result, from 1 to 28-31
             * @return a <b> LocalDateTime</b>  based on this date-time with the requested day
             * @throws DateTimeException if the day-of-m value is invalid,
             *  or if the day-of-m is invalid for the m-year
             */
            LocalDateTime withDayOfMonth(gint dayOfMonth) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the day-of-year altered.
             * <p>
             * If the resulting date-time is invalid, an exception is thrown.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param dayOfYear  the day-of-year to set in the result, from 1 to 365-366
             * @return a <b> LocalDateTime</b>  based on this date with the requested day
             * @throws DateTimeException if the day-of-year value is invalid,
             *  or if the day-of-year is invalid for the year
             */
            LocalDateTime withDayOfYear(gint dayOfYear) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the hour-of-day altered.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param hour  the hour-of-day to set in the result, from 0 to 23
             * @return a <b> LocalDateTime</b>  based on this date-time with the requested hour
             * @throws DateTimeException if the hour value is invalid
             */
            LocalDateTime withHour(gint hour) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the minute-of-hour altered.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param minute  the minute-of-hour to set in the result, from 0 to 59
             * @return a <b> LocalDateTime</b>  based on this date-time with the requested minute
             * @throws DateTimeException if the minute value is invalid
             */
            LocalDateTime withMinute(gint minute) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the second-of-minute altered.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param second  the second-of-minute to set in the result, from 0 to 59
             * @return a <b> LocalDateTime</b>  based on this date-time with the requested second
             * @throws DateTimeException if the second value is invalid
             */
            LocalDateTime withSecond(gint second) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the nano-of-second altered.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param nanoOfSecond  the nano-of-second to set in the result, from 0 to 999,999,999
             * @return a <b> LocalDateTime</b>  based on this date-time with the requested nanosecond
             * @throws DateTimeException if the nano value is invalid
             */
            LocalDateTime withNano(gint nanoOfSecond) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the time truncated.
             * <p>
             * Truncation returns a copy of the original date-time with fields
             * smaller than the specified unit set to zero.
             * For example, truncating with the <b style="color:orange;"> minutes</b>  unit
             * will set the second-of-minute and nano-of-second field to zero.
             * <p>
             * The unit must have a <b style="color:green;">duration</b> 
             * that divides into the length of a standard day without remainder.
             * This includes all supplied time units on <b style="color:orange;"> ChronoUnit</b>  and
             * <b style="color:orange;"> ChronoUnit::DAYS DAYS</b> . Other units throw an exception.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param unit  the unit to truncate to
             * @return a <b> LocalDateTime</b>  based on this date-time with the time truncated
             * @throws DateTimeException if unable to truncate
             * @throws UnsupportedTemporalException if the unit is not supported
             */
            LocalDateTime truncatedTo(TemporalUnit unit) const;

            //-----------------------------------------------------------------------

            /**
             * Returns a copy of this date-time with the specified amount added.
             * <p>
             * This returns a <b> LocalDateTime</b> , based on this one, with the amount
             * in terms of the unit added. If it is not possible to add the amount, because the
             * unit is not supported or for some other reason, an exception is thrown.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoUnit</b>  then the addition is implemented here.
             * Date units are added as per <b style="color:orange;"> LocalDate::after(glong, TemporalUnit)</b> .
             * Time units are added as per <b style="color:orange;"> LocalTime::after(glong, TemporalUnit)</b>  with
             * any overflow in days added equivalent to using <b style="color:orange;"> afterDays(glong)</b> .
             * <p>
             * If the field is not a <b> ChronoUnit</b> , then the result of this method
             * is obtained by invoking <b> TemporalUnit.addTo(Temporal, glong)</b>
             * passing <b> this</b>  as the argument. In this case, the unit determines
             * whether and how to perform the addition.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param amountToAdd  the amount of the unit to add to the result, may be negative
             * @param unit  the unit of the amount to add
             * @return a <b> LocalDateTime</b>  based on this date-time with the specified amount added
             * @throws DateTimeException if the addition cannot be made
             * @throws UnsupportedTemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
             LocalDateTime after(glong amountToAdd, TemporalUnit unit) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified number of years added.
             * <p>
             * This method adds the specified amount to the years field in three steps:
             * <ol>
             * <li>Add the input years to the year field</li>
             * <li>Check if the resulting date would be invalid</li>
             * <li>Adjust the day-of-m to the last valid day if necessary</li>
             * </ol>
             * <p>
             * For example, 2008-02-29 (leap year) after one year would result in the
             * invalid date 2009-02-29 (standard year). Instead of returning an invalid
             * result, the last valid day of the m, 2009-02-28, is selected instead.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param years  the years to add, may be negative
             * @return a <b> LocalDateTime</b>  based on this date-time with the years added
             * @throws DateTimeException if the result exceeds the supported date range
             */
             LocalDateTime afterYears(glong years) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified number of months added.
             * <p>
             * This method adds the specified amount to the months field in three steps:
             * <ol>
             * <li>Add the input months to the m-of-year field</li>
             * <li>Check if the resulting date would be invalid</li>
             * <li>Adjust the day-of-m to the last valid day if necessary</li>
             * </ol>
             * <p>
             * For example, 2007-03-31 after one m would result in the invalid date
             * 2007-04-31. Instead of returning an invalid result, the last valid day
             * of the m, 2007-04-30, is selected instead.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param months  the months to add, may be negative
             * @return a <b> LocalDateTime</b>  based on this date-time with the months added
             * @throws DateTimeException if the result exceeds the supported date range
             */
             LocalDateTime afterMonths(glong months) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified number of weeks added.
             * <p>
             * This method adds the specified amount in weeks to the days field incrementing
             * the m and year fields as necessary to ensure the result remains valid.
             * The result is only invalid if the maximum/minimum year is exceeded.
             * <p>
             * For example, 2008-12-31 after one week would result in 2009-01-07.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param weeks  the weeks to add, may be negative
             * @return a <b> LocalDateTime</b>  based on this date-time with the weeks added
             * @throws DateTimeException if the result exceeds the supported date range
             */
             LocalDateTime afterWeeks(glong weeks) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified number of days added.
             * <p>
             * This method adds the specified amount to the days field incrementing the
             * m and year fields as necessary to ensure the result remains valid.
             * The result is only invalid if the maximum/minimum year is exceeded.
             * <p>
             * For example, 2008-12-31 after one day would result in 2009-01-01.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param days  the days to add, may be negative
             * @return a <b> LocalDateTime</b>  based on this date-time with the days added
             * @throws DateTimeException if the result exceeds the supported date range
             */
             LocalDateTime afterDays(glong days) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified number of hours added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param hours  the hours to add, may be negative
             * @return a <b> LocalDateTime</b>  based on this date-time with the hours added
             * @throws DateTimeException if the result exceeds the supported date range
             */
             LocalDateTime afterHours(glong hours) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified number of minutes added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param minutes  the minutes to add, may be negative
             * @return a <b> LocalDateTime</b>  based on this date-time with the minutes added
             * @throws DateTimeException if the result exceeds the supported date range
             */
             LocalDateTime afterMinutes(glong minutes) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified number of seconds added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param seconds  the seconds to add, may be negative
             * @return a <b> LocalDateTime</b>  based on this date-time with the seconds added
             * @throws DateTimeException if the result exceeds the supported date range
             */
             LocalDateTime afterSeconds(glong seconds) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified number of nanoseconds added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param nanos  the nanos to add, may be negative
             * @return a <b> LocalDateTime</b>  based on this date-time with the nanoseconds added
             * @throws DateTimeException if the result exceeds the supported date range
             */
             LocalDateTime afterNanos(glong nanos) const;

            /**
             * Returns a copy of this date-time with the specified amount subtracted.
             * <p>
             * This returns a <b> LocalDateTime</b> , based on this one, with the amount
             * in terms of the unit subtracted. If it is not possible to subtract the amount,
             * because the unit is not supported or for some other reason, an exception is thrown.
             * <p>
             * This method is equivalent to <b style="color:orange;"> after(glong, TemporalUnit)</b>  with the amount negated.
             * See that method for a full description of how addition, and thus subtraction, works.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param amountToSubtract  the amount of the unit to subtract from the result, may be negative
             * @param unit  the unit of the amount to subtract
             * @return a <b> LocalDateTime</b>  based on this date-time with the specified amount subtracted
             * @throws DateTimeException if the subtraction cannot be made
             * @throws UnsupportedTemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
             LocalDateTime before(glong amountToSubtract, TemporalUnit unit) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified number of years subtracted.
             * <p>
             * This method subtracts the specified amount from the years field in three steps:
             * <ol>
             * <li>Subtract the input years from the year field</li>
             * <li>Check if the resulting date would be invalid</li>
             * <li>Adjust the day-of-m to the last valid day if necessary</li>
             * </ol>
             * <p>
             * For example, 2008-02-29 (leap year) minus one year would result in the
             * invalid date 2007-02-29 (standard year). Instead of returning an invalid
             * result, the last valid day of the m, 2007-02-28, is selected instead.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param years  the years to subtract, may be negative
             * @return a <b> LocalDateTime</b>  based on this date-time with the years subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
             LocalDateTime beforeYears(glong years) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified number of months subtracted.
             * <p>
             * This method subtracts the specified amount from the months field in three steps:
             * <ol>
             * <li>Subtract the input months from the m-of-year field</li>
             * <li>Check if the resulting date would be invalid</li>
             * <li>Adjust the day-of-m to the last valid day if necessary</li>
             * </ol>
             * <p>
             * For example, 2007-03-31 minus one m would result in the invalid date
             * 2007-02-31. Instead of returning an invalid result, the last valid day
             * of the m, 2007-02-28, is selected instead.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param months  the months to subtract, may be negative
             * @return a <b> LocalDateTime</b>  based on this date-time with the months subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
             LocalDateTime beforeMonths(glong months) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified number of weeks subtracted.
             * <p>
             * This method subtracts the specified amount in weeks from the days field decrementing
             * the m and year fields as necessary to ensure the result remains valid.
             * The result is only invalid if the maximum/minimum year is exceeded.
             * <p>
             * For example, 2009-01-07 minus one week would result in 2008-12-31.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param weeks  the weeks to subtract, may be negative
             * @return a <b> LocalDateTime</b>  based on this date-time with the weeks subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
             LocalDateTime beforeWeeks(glong weeks) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified number of days subtracted.
             * <p>
             * This method subtracts the specified amount from the days field decrementing the
             * m and year fields as necessary to ensure the result remains valid.
             * The result is only invalid if the maximum/minimum year is exceeded.
             * <p>
             * For example, 2009-01-01 minus one day would result in 2008-12-31.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param days  the days to subtract, may be negative
             * @return a <b> LocalDateTime</b>  based on this date-time with the days subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
             LocalDateTime beforeDays(glong days) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified number of hours subtracted.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param hours  the hours to subtract, may be negative
             * @return a <b> LocalDateTime</b>  based on this date-time with the hours subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
             LocalDateTime beforeHours(glong hours) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified number of minutes subtracted.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param minutes  the minutes to subtract, may be negative
             * @return a <b> LocalDateTime</b>  based on this date-time with the minutes subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
             LocalDateTime beforeMinutes(glong minutes) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified number of seconds subtracted.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param seconds  the seconds to subtract, may be negative
             * @return a <b> LocalDateTime</b>  based on this date-time with the seconds subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
             LocalDateTime beforeSeconds(glong seconds) const;

            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified number of nanoseconds subtracted.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param nanos  the nanos to subtract, may be negative
             * @return a <b> LocalDateTime</b>  based on this date-time with the nanoseconds subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
             LocalDateTime beforeNanos(glong nanos) const;

        private:

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this <b> LocalDateTime</b>  with the specified period added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param newDate  the new date to base the calculation on
             * @param hours  the hours to add, may be negative
             * @param minutes the minutes to add, may be negative
             * @param seconds the seconds to add, may be negative
             * @param nanos the nanos to add, may be negative
             * @param sign  the sign to determine add or subtract
             * @return the combined result
             */
            LocalDateTime afterWithOverflow(const LocalDate& newDate, glong hours, glong minutes, glong seconds, glong nanos, gint sign) const;

        public:
            //-----------------------------------------------------------------------
            /**
             * Queries this date-time using the specified query.
             * <p>
             * This queries this date-time using the specified query strategy object.
             * The <b> Query</b>  object defines the logic to be used to
             * obtain the result. Read the documentation of the query to understand
             * what the result of this method will be.
             * <p>
             * The result of this method is obtained by invoking the
             * <b style="color:orange;"> Query::queryFrom(Temporal)</b>  method on the
             * specified query passing <b> this</b>  as the argument.
             *
             * @param <R> the type of the result
             * @param query  the query to invoke
             * @return the query result, null may be returned (defined by the query)
             * @throws DateTimeException if unable to query (defined by the query)
             * @throws ArithmeticException if numeric overflow occurs (defined by the query)
             */
            const Object &query(const Query &query) const override;

            /**
             * Calculates the amount of time until another date-time in terms of the specified unit.
             * <p>
             * This calculates the amount of time between two <b> LocalDateTime</b> 
             * objects in terms of a single <b> TemporalUnit</b> .
             * The start and end points are <b> this</b>  and the specified date-time.
             * The result will be negative if the end is before the start.
             * The <b> Temporal</b>  passed to this method is converted to a
             * <b> LocalDateTime</b>  using <b style="color:orange;"> from(Temporal)</b> .
             * For example, the amount in days between two date-times can be calculated
             * using <b> startDateTime.until(endDateTime, DAYS)</b> .
             * <p>
             * The calculation returns a whole number, representing the number of
             * complete units between the two date-times.
             * For example, the amount in months between 2012-06-15T00:00 and 2012-08-14T23:59
             * will only be one m as it is one minute short of two months.
             * <p>
             * The calculation is implemented in this method for <b style="color:orange;"> ChronoUnit</b> .
             * The units <b> NANOS</b> , <b> MICROS</b> , <b> MILLIS</b> , <b> SECONDS</b> ,
             * <b> MINUTES</b> , <b> HOURS</b>  and <b> HALF_DAYS</b> , <b> DAYS</b> ,
             * <b> WEEKS</b> , <b> MONTHS</b> , <b> YEARS</b> , <b> DECADES</b> ,
             * <b> CENTURIES</b> , <b> MILLENNIA</b>  and <b> ERAS</b>  are supported.
             * Other <b> ChronoUnit</b>  values will throw an exception.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param endExclusive  the end date, exclusive, which is converted to a <b> LocalDateTime</b>
             * @param unit  the unit to measure the amount in
             * @return the amount of time between this date-time and the end date-time
             * @throws DateTimeException if the amount cannot be calculated, or the end
             *  temporal cannot be converted to a <b> LocalDateTime</b> 
             * @throws UnsupportedTemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            glong until(const Temporal &endExclusive, TemporalUnit unit) const override;

            /**
             * Formats this date-time using the specified formatter.
             * <p>
             * This date-time will be passed to the formatter to produce a string.
             *
             * @param formatter  the formatter to use
             * @return the formatted date-time string
             * @throws DateTimeException if an error occurs during printing
             */
             String format(const DateTimeFormatter& formatter) const;

            /**
             * Converts this date-time to the number of seconds from the epoch
             * of 1970-01-01T00:00:00Z.
             * <p>
             * This combines this local date-time and the specified offset to calculate the
             * epoch-second value, which is the number of elapsed seconds from 1970-01-01T00:00:00Z.
             * Instants on the time-line after the epoch are positive, earlier are negative.
             * <p>
             * This default implementation calculates from the epoch-day of the date and the
             * second-of-day of the time.
             *
             * @param offset  the offset to use for the conversion, not null
             * @return the number of seconds from the epoch of 1970-01-01T00:00:00Z
             */
             glong toEpochSecond(const ZoneOffset& offset) const;

            //-----------------------------------------------------------------------
            /**
             * Compares this date-time to another date-time.
             * <p>
             * The comparison is primarily based on the date-time, from earliest to latest.
             * It is "consistent with equals", as defined by <b style="color:orange;"> Comparable</b> .
             * <p>
             * If all the date-times being compared are instances of <b> LocalDateTime</b> ,
             * then the comparison will be entirely based on the date-time.
             *
             * @param other  the other date-time to compare to
             * @return the comparator value, negative if less, positive if greater
             */
            gint compareTo(const LocalDateTime &other) const override;

            /**
             * Checks if this date-time is after the specified date-time.
             * <p>
             * This checks to see if this date-time represents a point on the
             * local time-line after the other date-time.
             * <pre>
             * @code
             *   LocalDate a = LocalDateTime.of(2012, 6, 30, 12, 00);
             *   LocalDate b = LocalDateTime.of(2012, 7, 1, 12, 00);
             *   a.isAfter(b) == false
             *   a.isAfter(a) == false
             *   b.isAfter(a) == true
             * @endcode
             * </pre>
             *
             * @param other  the other date-time to compare to
             * @return true if this date-time is after the specified date-time
             */
             gbool isAfter(const LocalDateTime& other) const;

            /**
             * Checks if this date-time is before the specified date-time.
             * <p>
             * This checks to see if this date-time represents a point on the
             * local time-line before the other date-time.
             * <pre>
             * @code
             *   LocalDate a = LocalDateTime.of(2012, 6, 30, 12, 00);
             *   LocalDate b = LocalDateTime.of(2012, 7, 1, 12, 00);
             *   a.isBefore(b) == true
             *   a.isBefore(a) == false
             *   b.isBefore(a) == false
             * @endcode
             * </pre>
             *
             * @param other  the other date-time to compare to
             * @return true if this date-time is before the specified date-time
             */
             gbool isBefore(const LocalDateTime& other) const;

            /**
             * Checks if this date-time is equal to the specified date-time.
             * <p>
             * This checks to see if this date-time represents the same point on the
             * local time-line as the other date-time.
             * <pre>
             * @code
             *   LocalDate a = LocalDateTime.of(2012, 6, 30, 12, 00);
             *   LocalDate b = LocalDateTime.of(2012, 7, 1, 12, 00);
             *   a.isEqual(b) == false
             *   a.isEqual(a) == true
             *   b.isEqual(a) == false
             * @endcode
             * </pre>
             *
             * @param other  the other date-time to compare to
             * @return true if this date-time is equal to the specified date-time
             */
             gbool isEqual(const LocalDateTime& other) const;

            //-----------------------------------------------------------------------
            /**
             * Checks if this date-time is equal to another date-time.
             * <p>
             * Compares this <b> LocalDateTime</b>  with another ensuring that the date-time is the same.
             * Only objects of type <b> LocalDateTime</b>  are compared, other types return false.
             *
             * @param obj  the object to check, null returns false
             * @return true if this is equal to the other date-time
             */
            gbool equals(const Object &o) const override;

            /**
             * A hash code for this date-time.
             *
             * @return a suitable hash code
             */
            gint hash() const override;

            //-----------------------------------------------------------------------
            /**
             * Outputs this date-time as a <b> String</b> , such as <b> 2007-12-03T10:15:30</b> .
             * <p>
             * The output will be one of the following ISO-8601 formats:
             * <ul>
             * <li><b> uuuu-MM-dd'T'HH:mm</b> </li>
             * <li><b> uuuu-MM-dd'T'HH:mm:ss</b> </li>
             * <li><b> uuuu-MM-dd'T'HH:mm:ss.SSS</b> </li>
             * <li><b> uuuu-MM-dd'T'HH:mm:ss.SSSSSS</b> </li>
             * <li><b> uuuu-MM-dd'T'HH:mm:ss.SSSSSSSSS</b> </li>
             * </ul>
             * The format used will be the shortest that outputs the full value of
             * the time where the omitted parts are implied to be zero.
             *
             * @return a string representation of this date-time
             */
            String toString() const override;
        };

    } // core
} // time

#endif //CORE23_LOCALDATETIME_H
