//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_LOCALDATE_H
#define CORE23_LOCALDATE_H

#include <core/time/Temporal.h>
#include <core/Comparable.h>

namespace core::time {
 class ZoneID;
}

namespace core {
    namespace time {

        /**
         * A date without a time-zone in the ISO-8601 calendar system,
         * such as <b> 2007-12-03</b>.
         * <p>
         * <b> LocalDate</b> is an immutable date-time object that represents a date,
         * often viewed as year-m-day. Other date fields, such as day-of-year,
         * day-of-week and week-of-year, can also be accessed.
         * For example, the value "2nd October 2007" can be stored in a <b> LocalDate</b>.
         * <p>
         * This class does not store or represent a time or time-zone.
         * Instead, it is a description of the date, as used for birthdays.
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
         * <b style="color:green;"> equal</b> as interchangeable and should not
         * use instances for synchronization, or unpredictable behavior may
         * occur. For example, in a future release, synchronization may fail.
         * The <b> equals</b> method should be used for comparisons.
         *
         * @implSpec
         * This class is immutable and thread-safe.
         *
         */
        class LocalDate CORE_FINAL : public Temporal, public Comparable<LocalDate> {
        public:

            /**
             * The minimum supported <b> LocalDate</b>, '-999999999-01-01'.
             * This could be used by an application as a "far past" date.
             */
            static const LocalDate MIN;

            /**
             * The maximum supported <b> LocalDate</b>, '+999999999-12-31'.
             * This could be used by an application as a "far future" date.
             */
            static const LocalDate MAX;

            /**
             * The epoch year <b> LocalDate</b>, '1970-01-01'.
             *
             */
            static const LocalDate EPOCH;

        private:

            /**
             * The number of days in a 400 year cycle.
             */
            static CORE_FAST gint DAYS_PER_CYCLE = 146097;

            /**
             * The number of days from year zero to year 1970.
             * There are five 400 year cycles from year zero to 2000.
             * There are 7 leap years from 1970 to 2000.
             */
            static CORE_FAST glong DAYS_0000_TO_1970 = (DAYS_PER_CYCLE * 5L) - (30L * 365L + 7L);

            /**
             * The LocalDate fields (40 bits)
             * <p>
             * mask = (signum(Y) & 1) &lt;&lt; 39 | 0x3FFFFFFF &lt;&lt; 9 | F &lt;&lt; 5 | 1F
             * <p>
             * yyyyMMdd = (Math::signum(year) & 1) &lt;&lt; 39 | Math::abs(year) &lt;&lt; 9 | m &lt;&lt; 5 | day
             * <p>
             * year = 30 bits + 1 bit (sign)
             * m = 4 bits
             * day = 5 bits
             */
            glong yyyyMMdd = {};

            //-----------------------------------------------------------------------

        public:
            /**
             * Obtains the current date from the system clock in the default time-zone.
             * <p>
             * This will query the <b style="color:orange;"> system clock</b> in the default
             * time-zone to obtain the current date.
             * <p>
             * Using this method will prevent the ability to use an alternate clock for testing
             * because the clock is hard-coded.
             *
             * @return the current date using the system clock and default time-zone
             */
            static LocalDate now();

            /**
             * Obtains the current date from the system clock in the specified time-zone.
             * <p>
             * This will query the <b style="color:orange;"> system clock</b> to obtain the current date.
             * Specifying the time-zone avoids dependence on the default time-zone.
             * <p>
             * Using this method will prevent the ability to use an alternate clock for testing
             * because the clock is hard-coded.
             *
             * @param zone  the zone ID to use
             * @return the current date using the system clock
             */
            static LocalDate now(const ZoneID& zone);

            //-----------------------------------------------------------------------

            /**
             * A m-of-year, such as 'July'.
             * <p>
             * <b> Month</b> is an enum representing the 12 months of the year -
             * January, February, March, April, May, June, July, August, September, October,
             * November and December.
             * <p>
             * In addition to the textual enum name, each m-of-year has an <b> gint</b> value.
             * The <b> gint</b> value follows normal usage and the ISO-8601 standard,
             * from 1 (January) to 12 (December). It is recommended that applications use the enum
             * rather than the <b> gint</b> value to ensure code clarity.
             * <p>
             * <b>Do not use <b> ordinal()</b> to obtain the numeric representation of <b> Month</b>.
             * Use <b> getValue()</b> instead.</b>
             * <p>
             * This enum represents a common concept that is found in many calendar systems.
             * As such, this enum may be used by any calendar system that has the m-of-year
             * concept defined exactly equivalent to the ISO-8601 calendar system.
             *
             * @implSpec
             * This is an immutable and thread-safe enum.
             *
             */
            enum Month : gbyte {

                /**
                 * The singleton instance for the m of January with 31 days.
                 * This has the numeric value of <b> 1</b>.
                 */
                JANUARY = 1,

                /**
                 * The singleton instance for the m of February with 28 days, or 29 in a leap year.
                 * This has the numeric value of <b> 2</b>.
                 */
                FEBRUARY,

                /**
                 * The singleton instance for the m of March with 31 days.
                 * This has the numeric value of <b> 3</b>.
                 */
                MARCH,

                /**
                 * The singleton instance for the m of April with 30 days.
                 * This has the numeric value of <b> 4</b>.
                 */
                APRIL,

                /**
                 * The singleton instance for the m of May with 31 days.
                 * This has the numeric value of <b> 5</b>.
                 */
                MAY,

                /**
                 * The singleton instance for the m of June with 30 days.
                 * This has the numeric value of <b> 6</b>.
                 */
                JUNE,

                /**
                 * The singleton instance for the m of July with 31 days.
                 * This has the numeric value of <b> 7</b>.
                 */
                JULY,

                /**
                 * The singleton instance for the m of August with 31 days.
                 * This has the numeric value of <b> 8</b>.
                 */
                AUGUST,

                /**
                 * The singleton instance for the m of September with 30 days.
                 * This has the numeric value of <b> 9</b>.
                 */
                SEPTEMBER,

                /**
                 * The singleton instance for the m of October with 31 days.
                 * This has the numeric value of <b> 10</b>.
                 */
                OCTOBER,

                /**
                 * The singleton instance for the m of November with 30 days.
                 * This has the numeric value of <b> 11</b>.
                 */
                NOVEMBER,

                /**
                 * The singleton instance for the m of December with 31 days.
                 * This has the numeric value of <b> 12</b>.
                 */
                DECEMBER,
            };

            /**
             * A day-of-week, such as 'Tuesday'.
             * <p>
             * <b> DayOfWeek</b> is an enum representing the 7 days of the week -
             * Monday, Tuesday, Wednesday, Thursday, Friday, Saturday and Sunday.
             * <p>
             * In addition to the textual enum name, each day-of-week has an <b> gint</b> value.
             * The <b> gint</b> value follows the ISO-8601 standard, from 1 (Monday) to 7 (Sunday).
             * It is recommended that applications use the enum rather than the <b> gint</b> value
             * to ensure code clarity.
             * <p>
             * This enum provides access to the localized textual form of the day-of-week.
             * Some locales also assign different numeric values to the days, declaring
             * Sunday to have the value 1, however this class provides no support for this.
             * See <b style="color:orange;"> WeekFields</b> for localized week-numbering.
             * <p>
             * <b>Do not use <b> ordinal()</b> to obtain the numeric representation of <b> DayOfWeek</b>.
             * Use <b> getValue()</b> instead.</b>
             * <p>
             * This enum represents a common concept that is found in many calendar systems.
             * As such, this enum may be used by any calendar system that has the day-of-week
             * concept defined exactly equivalent to the ISO calendar system.
             *
             * @implSpec
             * This is an immutable and thread-safe enum.
             *
             */
            enum DayOfWeek : gbyte {

                /**
                 * The singleton instance for the day-of-week of Monday.
                 * This has the numeric value of <b> 1</b>.
                 */
                MONDAY = 1,

                /**
                 * The singleton instance for the day-of-week of Tuesday.
                 * This has the numeric value of <b> 2</b>.
                 */
                TUESDAY,

                /**
                 * The singleton instance for the day-of-week of Wednesday.
                 * This has the numeric value of <b> 3</b>.
                 */
                WEDNESDAY,

                /**
                 * The singleton instance for the day-of-week of Thursday.
                 * This has the numeric value of <b> 4</b>.
                 */
                THURSDAY,

                /**
                 * The singleton instance for the day-of-week of Friday.
                 * This has the numeric value of <b> 5</b>.
                 */
                FRIDAY,

                /**
                 * The singleton instance for the day-of-week of Saturday.
                 * This has the numeric value of <b> 6</b>.
                 */
                SATURDAY,

                /**
                 * The singleton instance for the day-of-week of Sunday.
                 * This has the numeric value of <b> 7</b>.
                 */
                SUNDAY,
            };

            /**
             * Obtains an instance of <b> LocalDate</b> from a year, m and day.
             * <p>
             * This returns a <b> LocalDate</b> with the specified year, m and day-of-m.
             * The day must be valid for the year and m, otherwise an exception will be thrown.
             *
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the m-of-year to represent
             * @param dayOfMonth  the day-of-m to represent, from 1 to 31
             * @return the local date
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-m is invalid for the m-year
             */
            CORE_EXPLICIT LocalDate(gint year, Month month, gint dayOfMonth);

            /**
             * Obtains an instance of <b> LocalDate</b> from a year, m and day.
             * <p>
             * This returns a <b> LocalDate</b> with the specified year, m and day-of-m.
             * The day must be valid for the year and m, otherwise an exception will be thrown.
             *
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the m-of-year to represent, from 1 (January) to 12 (December)
             * @param dayOfMonth  the day-of-m to represent, from 1 to 31
             * @return the local date
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-m is invalid for the m-year
             */
            CORE_EXPLICIT LocalDate(gint year, gint month, gint dayOfMonth);

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> LocalDate</b> from a year and day-of-year.
             * <p>
             * This returns a <b> LocalDate</b> with the specified year and day-of-year.
             * The day-of-year must be valid for the year, otherwise an exception will be thrown.
             *
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param dayOfYear  the day-of-year to represent, from 1 to 366
             * @return the local date
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-year is invalid for the year
             */
            CORE_EXPLICIT LocalDate(gint year, gint dayOfYear);

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> LocalDate</b> from the epoch day count.
             * <p>
             * This returns a <b> LocalDate</b> with the specified epoch-day.
             * The <b style="color:orange;"> EPOCH_DAY</b> is a simple incrementing count
             * of days where day 0 is 1970-01-01. Negative numbers represent earlier days.
             *
             * @param epochDay  the Epoch Day to convert, based on the epoch 1970-01-01
             * @return the local date
             * @throws DateTimeException if the epoch day exceeds the supported date range
             */
            static LocalDate ofEpochDay(glong epochDay);

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> LocalDate</b> from a temporal object.
             * <p>
             * This obtains a local date based on the specified temporal.
             * A <b> Temporal</b> represents an arbitrary set of date and time information,
             * which this factory converts to an instance of <b> LocalDate</b>.
             * <p>
             * The conversion uses the <b style="color:orange;"> Temporal::LOCAL_DATE</b> query, which relies
             * on extracting the <b style="color:orange;"> ChronoField#EPOCH_DAY EPOCH_DAY</b> field.
             * <p>
             * This method matches the signature of the functional interface <b style="color:orange;"> TemporalQuery</b>
             * allowing it to be used as a query via method reference, <b> LocalDate::from</b>.
             *
             * @param temporal  the temporal object to convert
             * @return the local date
             * @throws DateTimeException if unable to convert to a <b> LocalDate</b>
             */
            static LocalDate from(const Temporal &temporal);

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> LocalDate</b> from a text string such as <b> 2007-12-03</b>.
             * <p>
             * The string must represent a valid date and is parsed using
             * <b style="color:orange;"> DateTimeFormatter::ISO_LOCAL_DATE</b>.
             *
             * @param text  the text to parse such as "2007-12-03"
             * @return the parsed local date
             * @throws DateTimeParseException if the text cannot be parsed
             */
            static LocalDate parse(const String &text);

            /**
             * Obtains an instance of <b> LocalDate</b> from a text string using a specific formatter.
             * <p>
             * The text is parsed using the formatter, returning a date.
             *
             * @param text  the text to parse
             * @param formatter  the formatter to use
             * @return the parsed local date
             * @throws DateTimeParseException if the text cannot be parsed
             */
            static LocalDate parse(const String &text, const DateTimeFormatter &formatter);

            //-----------------------------------------------------------------------
            /**
             * Checks if the specified field is supported.
             * <p>
             * This checks if this date can be queried for the specified field.
             * If false, then calling the <b style="color:orange;"> range</b>,
             * <b style="color:orange;"> get</b> and <b style="color:orange;"> with </b>
             * methods will throw an exception.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoField</b> then the query is implemented here.
             * The supported fields are:
             * <ul>
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
             * All other <b> ChronoField</b> instances will return false.
             * <p>
             * If the field is not a <b> ChronoField</b>, then the result of this method
             * is obtained by invoking <b> TemporalField.isSupportedBy(Temporal)</b>
             * passing <b> this</b> as the argument.
             * Whether the field is supported is determined by the field.
             *
             * @param field  the field to check, null returns false
             * @return true if the field is supported on this date, false if not
             */
            gbool supportField(TemporalField field) const override;

            /**
             * Checks if the specified unit is supported.
             * <p>
             * This checks if the specified unit can be added to, or subtracted from, this date.
             * If false, then calling the <b style="color:orange;"> after</b> and
             * <b style="color:orange;"> before</b> methods will throw an exception.
             * <p>
             * If the unit is a <b style="color:orange;"> ChronoUnit</b> then the query is implemented here.
             * The supported units are:
             * <ul>
             * <li><b> DAYS</b>
             * <li><b> WEEKS</b>
             * <li><b> MONTHS</b>
             * <li><b> YEARS</b>
             * <li><b> DECADES</b>
             * <li><b> CENTURIES</b>
             * <li><b> MILLENNIA</b>
             * <li><b> ERAS</b>
             * </ul>
             * All other <b> ChronoUnit</b> instances will return false.
             * <p>
             * If the unit is not a <b> ChronoUnit</b>, then the result of this method
             * is obtained by invoking <b> TemporalUnit.isSupportedBy(Temporal)</b>
             * passing <b> this</b> as the argument.
             * Whether the unit is supported is determined by the unit.
             *
             * @param unit  the unit to check, null returns false
             * @return true if the unit can be added/subtracted, false if not
             */
            gbool supportUnit(TemporalUnit unit) const override;

            //-----------------------------------------------------------------------
            /**
             * Gets the value of the specified field from this date as an <b> gint</b>.
             * <p>
             * This queries this date for the value of the specified field.
             * The returned value will always be within the valid range of values for the field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoField</b> then the query is implemented here.
             * The <b style="color:orange;"> supported fields</b> will return valid
             * values based on this date, except <b> EPOCH_DAY</b> and <b> PROLEPTIC_MONTH</b>
             * which are too large to fit in an <b> gint</b> and throw an <b> UnsupportedTemporalTypeException</b>.
             * All other <b> ChronoField</b> instances will throw an <b> UnsupportedTemporalTypeException</b>.
             * <p>
             * If the field is not a <b> ChronoField</b>, then the result of this method
             * is obtained by invoking <b> TemporalField.getFrom(Temporal)</b>
             * passing <b> this</b> as the argument. Whether the value can be obtained,
             * and what the value represents, is determined by the field.
             *
             * @param field  the field to get
             * @return the value for the field
             * @throws DateTimeException if a value for the field cannot be obtained or
             *         the value is outside the range of valid values for the field
             * @throws UnsupportedTemporalTypeException if the field is not supported or
             *         the range of values exceeds an <b> gint</b>
             * @throws ArithmeticException if numeric overflow occurs
             */
            gint get(TemporalField field) const override;

            /**
             * Gets the value of the specified field from this date as a <b> glong</b>.
             * <p>
             * This queries this date for the value of the specified field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoField</b> then the query is implemented here.
             * The <b style="color:orange;"> supported fields</b> will return valid
             * values based on this date.
             * All other <b> ChronoField</b> instances will throw an <b> UnsupportedTemporalTypeException</b>.
             * <p>
             * If the field is not a <b> ChronoField</b>, then the result of this method
             * is obtained by invoking <b> TemporalField.getFrom(Temporal)</b>
             * passing <b> this</b> as the argument. Whether the value can be obtained,
             * and what the value represents, is determined by the field.
             *
             * @param field  the field to get
             * @return the value for the field
             * @throws DateTimeException if a value for the field cannot be obtained
             * @throws UnsupportedTemporalTypeException if the field is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            glong getLong(TemporalField field) const override;

            //-----------------------------------------------------------------------
            /**
             * Gets the chronology of this date, which is the ISO calendar system.
             * <p>
             * The <b> Chronology</b> represents the calendar system in use.
             * The ISO-8601 calendar system is the modern civil calendar system used today
             * in most of the world. It is equivalent to the proleptic Gregorian calendar
             * system, in which today's rules for leap years are applied for all time.
             *
             * @return the ISO chronology
             */
//            Chronology &chronology() const;

            /**
             * Gets the era applicable at this date.
             * <p>
             * The official ISO-8601 standard does not define eras, however <b> IsoChronology</b> does.
             * It defines two eras, 'CE' from year one onwards and 'BCE' from year zero backwards.
             * Since dates before the Julian-Gregorian cutover are not in line with history,
             * the cutover between 'BCE' and 'CE' is also not aligned with the commonly used
             * eras, often referred to using 'BC' and 'AD'.
             * <p>
             * Users of this class should typically ignore this method as it exists primarily
             * to fulfill the <b style="color:orange;"> ChronoLocalDate</b> contract where it is necessary to support
             * the Japanese calendar system.
             *
             * @return the IsoEra applicable at this date
             */
//            Era &era() const;

            /**
             * Gets the year field.
             * <p>
             * This method returns the primitive <b> gint</b> value for the year.
             * <p>
             * The year returned by this method is proleptic as per <b> get(YEAR)</b>.
             * To obtain the year-of-era, use <b> get(YEAR_OF_ERA)</b>.
             *
             * @return the year, from MIN_YEAR to MAX_YEAR
             */
            gint year() const;

            /**
             * Gets the m-of-year field using the <b> Month</b> enum.
             * <p>
             * This method returns the enum <b style="color:orange;"> Month</b> for the m.
             * This avoids confusion as to what <b> gint</b> values mean.
             *
             * @return the m-of-year
             */
            Month month() const;

            /**
             * Gets the day-of-m field.
             * <p>
             * This method returns the primitive <b> gint</b> value for the day-of-m.
             *
             * @return the day-of-m, from 1 to 31
             */
            gint dayOfMonth() const;

            /**
             * Gets the day-of-year field.
             * <p>
             * This method returns the primitive <b> gint</b> value for the day-of-year.
             *
             * @return the day-of-year, from 1 to 365, or 366 in a leap year
             */
            gint dayOfYear() const;

            /**
             * Gets the day-of-week field, which is an enum <b> DayOfWeek</b>.
             * <p>
             * This method returns the enum <b style="color:orange;"> DayOfWeek</b> for the day-of-week.
             * This avoids confusion as to what <b> gint</b> values mean.
             * <p>
             * Additional information can be obtained from the <b> DayOfWeek</b>.
             * This includes textual names of the values.
             *
             * @return the day-of-week
             */
            DayOfWeek dayOfWeek() const;

            //-----------------------------------------------------------------------
            /**
             * Checks if the year is a leap year, according to the ISO proleptic
             * calendar system rules.
             * <p>
             * This method applies the current rules for leap years across the whole time-line.
             * In general, a year is a leap year if it is divisible by four without
             * remainder. However, years divisible by 100, are not leap years, with
             * the exception of years divisible by 400 which are.
             * <p>
             * For example, 1904 is a leap year it is divisible by 4.
             * 1900 was not a leap year as it is divisible by 100, however 2000 was a
             * leap year as it is divisible by 400.
             * <p>
             * The calculation is proleptic - applying the same rules into the far future and far past.
             * This is historically inaccurate, but is correct for the ISO-8601 standard.
             *
             * @return true if the year is leap, false otherwise
             */
            gbool isLeapYear() const;

            /**
             * Returns the length of the m represented by this date.
             * <p>
             * This returns the length of the m in days.
             * For example, a date in January would return 31.
             *
             * @return the length of the m in days
             */
            gint daysPerMonth() const;

            /**
             * Returns the length of the year represented by this date.
             * <p>
             * This returns the length of the year in days, either 365 or 366.
             *
             * @return 366 if the year is leap, 365 otherwise
             */
            gint daysPerYear() const;

            //-----------------------------------------------------------------------

            /**
             * Returns a copy of this date with the specified field set to a new value.
             * <p>
             * This returns a <b> LocalDate</b>, based on this one, with the value
             * for the specified field changed.
             * This can be used to change any supported field, such as the year, m or day-of-m.
             * If it is not possible to set the value, because the field is not supported or for
             * some other reason, an exception is thrown.
             * <p>
             * In some cases, changing the specified field can cause the resulting date to become invalid,
             * such as changing the m from 31st January to February would make the day-of-m invalid.
             * In cases like this, the field is responsible for resolving the date. Typically it will choose
             * the previous valid date, which would be the last valid day of February in this example.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoField</b> then the adjustment is implemented here.
             * The supported fields behave as follows:
             * <ul>
             * <li><b> DAY_OF_WEEK</b> -
             *  Returns a <b> LocalDate</b> with the specified day-of-week.
             *  The date is adjusted up to 6 days forward or backward within the boundary
             *  of a Monday to Sunday week.
             * <li><b> ALIGNED_DAY_OF_WEEK_IN_MONTH</b> -
             *  Returns a <b> LocalDate</b> with the specified aligned-day-of-week.
             *  The date is adjusted to the specified m-based aligned-day-of-week.
             *  Aligned weeks are counted such that the first week of a given m starts
             *  on the first day of that m.
             *  This may cause the date to be moved up to 6 days into the following m.
             * <li><b> ALIGNED_DAY_OF_WEEK_IN_YEAR</b> -
             *  Returns a <b> LocalDate</b> with the specified aligned-day-of-week.
             *  The date is adjusted to the specified year-based aligned-day-of-week.
             *  Aligned weeks are counted such that the first week of a given year starts
             *  on the first day of that year.
             *  This may cause the date to be moved up to 6 days into the following year.
             * <li><b> DAY_OF_MONTH</b> -
             *  Returns a <b> LocalDate</b> with the specified day-of-m.
             *  The m and year will be unchanged. If the day-of-m is invalid for the
             *  year and m, then a <b> DateTimeException</b> is thrown.
             * <li><b> DAY_OF_YEAR</b> -
             *  Returns a <b> LocalDate</b> with the specified day-of-year.
             *  The year will be unchanged. If the day-of-year is invalid for the
             *  year, then a <b> DateTimeException</b> is thrown.
             * <li><b> EPOCH_DAY</b> -
             *  Returns a <b> LocalDate</b> with the specified epoch-day.
             *  This completely replaces the date and is equivalent to <b style="color:orange;"> ofEpochDay(glong)</b>.
             * <li><b> ALIGNED_WEEK_OF_MONTH</b> -
             *  Returns a <b> LocalDate</b> with the specified aligned-week-of-m.
             *  Aligned weeks are counted such that the first week of a given m starts
             *  on the first day of that m.
             *  This adjustment moves the date in whole week chunks to match the specified week.
             *  The result will have the same day-of-week as this date.
             *  This may cause the date to be moved into the following m.
             * <li><b> ALIGNED_WEEK_OF_YEAR</b> -
             *  Returns a <b> LocalDate</b> with the specified aligned-week-of-year.
             *  Aligned weeks are counted such that the first week of a given year starts
             *  on the first day of that year.
             *  This adjustment moves the date in whole week chunks to match the specified week.
             *  The result will have the same day-of-week as this date.
             *  This may cause the date to be moved into the following year.
             * <li><b> MONTH_OF_YEAR</b> -
             *  Returns a <b> LocalDate</b> with the specified m-of-year.
             *  The year will be unchanged. The day-of-m will also be unchanged,
             *  unless it would be invalid for the new m and year. In that case, the
             *  day-of-m is adjusted to the maximum valid value for the new m and year.
             * <li><b> PROLEPTIC_MONTH</b> -
             *  Returns a <b> LocalDate</b> with the specified proleptic-m.
             *  The day-of-m will be unchanged, unless it would be invalid for the new m
             *  and year. In that case, the day-of-m is adjusted to the maximum valid value
             *  for the new m and year.
             * <li><b> YEAR_OF_ERA</b> -
             *  Returns a <b> LocalDate</b> with the specified year-of-era.
             *  The era and m will be unchanged. The day-of-m will also be unchanged,
             *  unless it would be invalid for the new m and year. In that case, the
             *  day-of-m is adjusted to the maximum valid value for the new m and year.
             * <li><b> YEAR</b> -
             *  Returns a <b> LocalDate</b> with the specified year.
             *  The m will be unchanged. The day-of-m will also be unchanged,
             *  unless it would be invalid for the new m and year. In that case, the
             *  day-of-m is adjusted to the maximum valid value for the new m and year.
             * <li><b> ERA</b> -
             *  Returns a <b> LocalDate</b> with the specified era.
             *  The year-of-era and m will be unchanged. The day-of-m will also be unchanged,
             *  unless it would be invalid for the new m and year. In that case, the
             *  day-of-m is adjusted to the maximum valid value for the new m and year.
             * </ul>
             * <p>
             * In all cases, if the new value is outside the valid range of values for the field
             * then a <b> DateTimeException</b> will be thrown.
             * <p>
             * All other <b> ChronoField</b> instances will throw an <b> UnsupportedTemporalTypeException</b>.
             * <p>
             * If the field is not a <b> ChronoField</b>, then the result of this method
             * is obtained by invoking <b> TemporalField.adjustInto(Temporal, glong)</b>
             * passing <b> this</b> as the argument. In this case, the field determines
             * whether and how to adjust the instant.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param field  the field to set in the result
             * @param newValue  the new value of the field in the result
             * @return a <b> LocalDate</b> based on <b> this</b> with the specified field set
             * @throws DateTimeException if the field cannot be set
             * @throws UnsupportedTemporalTypeException if the field is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            LocalDate with(TemporalField field, glong newValue) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this <b> LocalDate</b> with the year altered.
             * <p>
             * If the day-of-m is invalid for the year, it will be changed to the last valid day of the m.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param year  the year to set in the result, from MIN_YEAR to MAX_YEAR
             * @return a <b> LocalDate</b> based on this date with the requested year
             * @throws DateTimeException if the year value is invalid
             */
            LocalDate withYear(gint year) const;

            /**
             * Returns a copy of this <b> LocalDate</b> with the m-of-year altered.
             * <p>
             * If the day-of-m is invalid for the year, it will be changed to the last valid day of the m.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param month  the m-of-year to set in the result, from 1 (January) to 12 (December)
             * @return a <b> LocalDate</b> based on this date with the requested m
             * @throws DateTimeException if the m-of-year value is invalid
             */
            LocalDate withMonth(gint month) const;

            /**
             * Returns a copy of this <b> LocalDate</b> with the day-of-m altered.
             * <p>
             * If the resulting date is invalid, an exception is thrown.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param dayOfMonth  the day-of-m to set in the result, from 1 to 28-31
             * @return a <b> LocalDate</b> based on this date with the requested day
             * @throws DateTimeException if the day-of-m value is invalid,
             *  or if the day-of-m is invalid for the m-year
             */
            LocalDate withDayOfMonth(gint dayOfMonth) const;

            /**
             * Returns a copy of this <b> LocalDate</b> with the day-of-year altered.
             * <p>
             * If the resulting date is invalid, an exception is thrown.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param dayOfYear  the day-of-year to set in the result, from 1 to 365-366
             * @return a <b> LocalDate</b> based on this date with the requested day
             * @throws DateTimeException if the day-of-year value is invalid,
             *  or if the day-of-year is invalid for the year
             */
            LocalDate withDayOfYear(gint dayOfYear) const;

            /**
             * Returns a copy of this date with the specified amount added.
             * <p>
             * This returns a <b> LocalDate</b>, based on this one, with the amount
             * in terms of the unit added. If it is not possible to add the amount, because the
             * unit is not supported or for some other reason, an exception is thrown.
             * <p>
             * In some cases, adding the amount can cause the resulting date to become invalid.
             * For example, adding one m to 31st January would result in 31st February.
             * In cases like this, the unit is responsible for resolving the date.
             * Typically it will choose the previous valid date, which would be the last valid
             * day of February in this example.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoUnit</b> then the addition is implemented here.
             * The supported fields behave as follows:
             * <ul>
             * <li><b> DAYS</b> -
             *  Returns a <b> LocalDate</b> with the specified number of days added.
             *  This is equivalent to <b style="color:orange;"> afterDays(glong)</b>.
             * <li><b> WEEKS</b> -
             *  Returns a <b> LocalDate</b> with the specified number of weeks added.
             *  This is equivalent to <b style="color:orange;"> afterWeeks(glong)</b> and uses a 7 day week.
             * <li><b> MONTHS</b> -
             *  Returns a <b> LocalDate</b> with the specified number of months added.
             *  This is equivalent to <b style="color:orange;"> afterMonths(glong)</b>.
             *  The day-of-m will be unchanged unless it would be invalid for the new
             *  m and year. In that case, the day-of-m is adjusted to the maximum
             *  valid value for the new m and year.
             * <li><b> YEARS</b> -
             *  Returns a <b> LocalDate</b> with the specified number of years added.
             *  This is equivalent to <b style="color:orange;"> afterYears(glong)</b>.
             *  The day-of-m will be unchanged unless it would be invalid for the new
             *  m and year. In that case, the day-of-m is adjusted to the maximum
             *  valid value for the new m and year.
             * <li><b> DECADES</b> -
             *  Returns a <b> LocalDate</b> with the specified number of decades added.
             *  This is equivalent to calling <b style="color:orange;"> afterYears(glong)</b> with the amount
             *  multiplied by 10.
             *  The day-of-m will be unchanged unless it would be invalid for the new
             *  m and year. In that case, the day-of-m is adjusted to the maximum
             *  valid value for the new m and year.
             * <li><b> CENTURIES</b> -
             *  Returns a <b> LocalDate</b> with the specified number of centuries added.
             *  This is equivalent to calling <b style="color:orange;"> afterYears(glong)</b> with the amount
             *  multiplied by 100.
             *  The day-of-m will be unchanged unless it would be invalid for the new
             *  m and year. In that case, the day-of-m is adjusted to the maximum
             *  valid value for the new m and year.
             * <li><b> MILLENNIA</b> -
             *  Returns a <b> LocalDate</b> with the specified number of millennia added.
             *  This is equivalent to calling <b style="color:orange;"> afterYears(glong)</b> with the amount
             *  multiplied by 1,000.
             *  The day-of-m will be unchanged unless it would be invalid for the new
             *  m and year. In that case, the day-of-m is adjusted to the maximum
             *  valid value for the new m and year.
             * <li><b> ERAS</b> -
             *  Returns a <b> LocalDate</b> with the specified number of eras added.
             *  Only two eras are supported so the amount must be one, zero or before one.
             *  If the amount is non-zero then the year is changed such that the year-of-era
             *  is unchanged.
             *  The day-of-m will be unchanged unless it would be invalid for the new
             *  m and year. In that case, the day-of-m is adjusted to the maximum
             *  valid value for the new m and year.
             * </ul>
             * <p>
             * All other <b> ChronoUnit</b> instances will throw an <b> UnsupportedTemporalTypeException</b>.
             * <p>
             * If the field is not a <b> ChronoUnit</b>, then the result of this method
             * is obtained by invoking <b> TemporalUnit.addTo(Temporal, glong)</b>
             * passing <b> this</b> as the argument. In this case, the unit determines
             * whether and how to perform the addition.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param amountToAdd  the amount of the unit to add to the result, may be negative
             * @param unit  the unit of the amount to add
             * @return a <b> LocalDate</b> based on this date with the specified amount added
             * @throws DateTimeException if the addition cannot be made
             * @throws UnsupportedTemporalTypeException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            LocalDate after(glong amountToAdd, TemporalUnit unit) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this <b> LocalDate</b> with the specified number of years added.
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
             * @param yearsToAdd  the years to add, may be negative
             * @return a <b> LocalDate</b> based on this date with the years added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDate afterYears(glong yearsToAdd) const;

            /**
             * Returns a copy of this <b> LocalDate</b> with the specified number of months added.
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
             * @param monthsToAdd  the months to add, may be negative
             * @return a <b> LocalDate</b> based on this date with the months added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDate afterMonths(glong monthsToAdd) const;

            /**
             * Returns a copy of this <b> LocalDate</b> with the specified number of weeks added.
             * <p>
             * This method adds the specified amount in weeks to the days field incrementing
             * the m and year fields as necessary to ensure the result remains valid.
             * The result is only invalid if the maximum/minimum year is exceeded.
             * <p>
             * For example, 2008-12-31 after one week would result in 2009-01-07.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param weeksToAdd  the weeks to add, may be negative
             * @return a <b> LocalDate</b> based on this date with the weeks added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDate afterWeeks(glong weeksToAdd) const;

            /**
             * Returns a copy of this <b> LocalDate</b> with the specified number of days added.
             * <p>
             * This method adds the specified amount to the days field incrementing the
             * m and year fields as necessary to ensure the result remains valid.
             * The result is only invalid if the maximum/minimum year is exceeded.
             * <p>
             * For example, 2008-12-31 after one day would result in 2009-01-01.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param daysToAdd  the days to add, may be negative
             * @return a <b> LocalDate</b> based on this date with the days added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDate afterDays(glong daysToAdd) const;

            //-----------------------------------------------------------------------

            /**
             * Returns a copy of this date with the specified amount subtracted.
             * <p>
             * This returns a <b> LocalDate</b>, based on this one, with the amount
             * in terms of the unit subtracted. If it is not possible to subtract the amount,
             * because the unit is not supported or for some other reason, an exception is thrown.
             * <p>
             * This method is equivalent to <b style="color:orange;"> after(glong, TemporalUnit)</b> with the amount negated.
             * See that method for a full description of how addition, and thus subtraction, works.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param amountToSubtract  the amount of the unit to subtract from the result, may be negative
             * @param unit  the unit of the amount to subtract
             * @return a <b> LocalDate</b> based on this date with the specified amount subtracted
             * @throws DateTimeException if the subtraction cannot be made
             * @throws UnsupportedTemporalTypeException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            LocalDate before(glong amountToSubtract, TemporalUnit unit) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this <b> LocalDate</b> with the specified number of years subtracted.
             * <p>
             * This method subtracts the specified amount from the years field in three steps:
             * <ol>
             * <li>Subtract the input years from the year field</li>
             * <li>Check if the resulting date would be invalid</li>
             * <li>Adjust the day-of-m to the last valid day if necessary</li>
             * </ol>
             * <p>
             * For example, 2008-02-29 (leap year) before one year would result in the
             * invalid date 2007-02-29 (standard year). Instead of returning an invalid
             * result, the last valid day of the m, 2007-02-28, is selected instead.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param yearsToSubtract  the years to subtract, may be negative
             * @return a <b> LocalDate</b> based on this date with the years subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDate beforeYears(glong yearsToSubtract) const;

            /**
             * Returns a copy of this <b> LocalDate</b> with the specified number of months subtracted.
             * <p>
             * This method subtracts the specified amount from the months field in three steps:
             * <ol>
             * <li>Subtract the input months from the m-of-year field</li>
             * <li>Check if the resulting date would be invalid</li>
             * <li>Adjust the day-of-m to the last valid day if necessary</li>
             * </ol>
             * <p>
             * For example, 2007-03-31 before one m would result in the invalid date
             * 2007-02-31. Instead of returning an invalid result, the last valid day
             * of the m, 2007-02-28, is selected instead.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param monthsToSubtract  the months to subtract, may be negative
             * @return a <b> LocalDate</b> based on this date with the months subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDate beforeMonths(glong monthsToSubtract) const;

            /**
             * Returns a copy of this <b> LocalDate</b> with the specified number of weeks subtracted.
             * <p>
             * This method subtracts the specified amount in weeks from the days field decrementing
             * the m and year fields as necessary to ensure the result remains valid.
             * The result is only invalid if the maximum/minimum year is exceeded.
             * <p>
             * For example, 2009-01-07 before one week would result in 2008-12-31.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param weeksToSubtract  the weeks to subtract, may be negative
             * @return a <b> LocalDate</b> based on this date with the weeks subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDate beforeWeeks(glong weeksToSubtract) const;

            /**
             * Returns a copy of this <b> LocalDate</b> with the specified number of days subtracted.
             * <p>
             * This method subtracts the specified amount from the days field decrementing the
             * m and year fields as necessary to ensure the result remains valid.
             * The result is only invalid if the maximum/minimum year is exceeded.
             * <p>
             * For example, 2009-01-01 before one day would result in 2008-12-31.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param daysToSubtract  the days to subtract, may be negative
             * @return a <b> LocalDate</b> based on this date with the days subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDate beforeDays(glong daysToSubtract) const;

            //-----------------------------------------------------------------------
            /**
             * Queries this date using the specified query.
             * <p>
             * This queries this date using the specified query strategy object.
             * The <b> TemporalQuery</b> object defines the logic to be used to
             * obtain the result. Read the documentation of the query to understand
             * what the result of this method will be.
             * <p>
             * The result of this method is obtained by invoking the
             * <b style="color:orange;"> Query::queryFrom(Temporal)</b> method on the
             * specified query passing <b> this</b> as the argument.
             *
             * @param <R> the type of the result
             * @param query  the query to invoke
             * @return the query result, null may be returned (defined by the query)
             * @throws DateTimeException if unable to query (defined by the query)
             * @throws ArithmeticException if numeric overflow occurs (defined by the query)
             */
            const Object &query(const Query &query) const override;

            /**
             * Calculates the amount of time until another date in terms of the specified unit.
             * <p>
             * This calculates the amount of time between two <b> LocalDate</b>
             * objects in terms of a single <b> TemporalUnit</b>.
             * The start and end points are <b> this</b> and the specified date.
             * The result will be negative if the end is before the start.
             * The <b> Temporal</b> passed to this method is converted to a
             * <b> LocalDate</b> using <b style="color:orange;"> from(Temporal)</b>.
             * For example, the amount in days between two dates can be calculated
             * using <b> startDate.until(endDate, DAYS)</b>.
             * <p>
             * The calculation returns a whole number, representing the number of
             * complete units between the two dates.
             * For example, the amount in months between 2012-06-15 and 2012-08-14
             * will only be one m as it is one day short of two months.
             * <p>
             * The calculation is implemented in this method for <b style="color:orange;"> ChronoUnit</b>.
             * The units <b> DAYS</b>, <b> WEEKS</b>, <b> MONTHS</b>, <b> YEARS</b>,
             * <b> DECADES</b>, <b> CENTURIES</b>, <b> MILLENNIA</b> and <b> ERAS</b>
             * are supported. Other <b> ChronoUnit</b> values will throw an exception.
             * <p>
             * If the unit is not a <b> ChronoUnit</b>, then the result of this method
             * is obtained by invoking <b> TemporalUnit.between(Temporal, Temporal)</b>
             * passing <b> this</b> as the first argument and the converted input temporal
             * as the second argument.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param endExclusive  the end date, exclusive, which is converted to a <b> LocalDate</b>
             * @param unit  the unit to measure the amount in
             * @return the amount of time between this date and the end date
             * @throws DateTimeException if the amount cannot be calculated, or the end
             *  temporal cannot be converted to a <b> LocalDate</b>
             * @throws UnsupportedTemporalTypeException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            glong until(const Temporal &endExclusive, TemporalUnit unit) const override;

            /**
             * Formats this date using the specified formatter.
             * <p>
             * This date will be passed to the formatter to produce a string.
             *
             * @param formatter  the formatter to use
             * @return the formatted date string
             * @throws DateTimeException if an error occurs during printing
             */
            String format(const DateTimeFormatter& formatter);

            //-----------------------------------------------------------------------
            /**
             * Combines this date with a time to create a <b> LocalDateTime</b>.
             * <p>
             * This returns a <b> LocalDateTime</b> formed from this date at the specified time.
             * All possible combinations of date and time are valid.
             *
             * @param time  the time to combine with
             * @return the local date-time formed from this date and the specified time
             */
            LocalDateTime atTime(LocalTime time) const;

            /**
             * Combines this date with a time to create a <b> LocalDateTime</b>.
             * <p>
             * This returns a <b> LocalDateTime</b> formed from this date at the
             * specified hour and minute.
             * The seconds and nanosecond fields will be set to zero.
             * The individual time fields must be within their valid range.
             * All possible combinations of date and time are valid.
             *
             * @param hour  the hour-of-day to use, from 0 to 23
             * @param minute  the minute-of-hour to use, from 0 to 59
             * @return the local date-time formed from this date and the specified time
             * @throws DateTimeException if the value of any field is out of range
             */
            LocalDateTime atTime(gint hour, gint minute) const;

            /**
             * Combines this date with a time to create a <b> LocalDateTime</b>.
             * <p>
             * This returns a <b> LocalDateTime</b> formed from this date at the
             * specified hour, minute and second.
             * The nanosecond field will be set to zero.
             * The individual time fields must be within their valid range.
             * All possible combinations of date and time are valid.
             *
             * @param hour  the hour-of-day to use, from 0 to 23
             * @param minute  the minute-of-hour to use, from 0 to 59
             * @param second  the second-of-minute to represent, from 0 to 59
             * @return the local date-time formed from this date and the specified time
             * @throws DateTimeException if the value of any field is out of range
             */
            LocalDateTime atTime(gint hour, gint minute, gint second) const;

            /**
             * Combines this date with a time to create a <b> LocalDateTime</b>.
             * <p>
             * This returns a <b> LocalDateTime</b> formed from this date at the
             * specified hour, minute, second and nanosecond.
             * The individual time fields must be within their valid range.
             * All possible combinations of date and time are valid.
             *
             * @param hour  the hour-of-day to use, from 0 to 23
             * @param minute  the minute-of-hour to use, from 0 to 59
             * @param second  the second-of-minute to represent, from 0 to 59
             * @param nanoOfSecond  the nano-of-second to represent, from 0 to 999,999,999
             * @return the local date-time formed from this date and the specified time
             * @throws DateTimeException if the value of any field is out of range
             */
            LocalDateTime atTime(gint hour, gint minute, gint second, gint nanoOfSecond) const;

            /**
             * Combines this date with the time of midnight to create a <b> LocalDateTime</b>
             * at the start of this date.
             * <p>
             * This returns a <b> LocalDateTime</b> formed from this date at the time of
             * midnight, 00:00, at the start of this date.
             *
             * @return the local date-time of midnight at the start of this date
             */
            LocalDateTime atStartOfDay() const;

            //-----------------------------------------------------------------------

            glong toEpochDay() const;

            /**
             * Converts this <b> LocalDate</b> to the number of seconds since the epoch
             * of 1970-01-01T00:00:00Z.
             * <p>
             * This combines this local date with the specified time and
             * offset to calculate the epoch-second value, which is the
             * number of elapsed seconds from 1970-01-01T00:00:00Z.
             * Instants on the time-line after the epoch are positive, earlier
             * are negative.
             *
             * @param time the local time
             * @param offset the zone offset
             * @return the number of seconds since the epoch of 1970-01-01T00:00:00Z, may be negative
             *
             */
            glong toEpochSecond(LocalTime time, ZoneOffset offset) const;

            //-----------------------------------------------------------------------
            /**
             * Compares this date to another date.
             * <p>
             * The comparison is primarily based on the date, from earliest to latest.
             * It is "consistent with equals", as defined by <b style="color:orange;"> Comparable</b>.
             *
             * @param other  the other date to compare to
             * @return the comparator value, negative if less, positive if greater
             */
            gint compareTo(const LocalDate &other) const override;

            /**
             * Checks if this date is after the specified date.
             * <p>
             * This checks to see if this date represents a point on the
             * local time-line after the other date.
             * <pre>
             *   LocalDate a = LocalDate.of(2012, 6, 30);
             *   LocalDate b = LocalDate.of(2012, 7, 1);
             *   a.isAfter(b) == false
             *   a.isAfter(a) == false
             *   b.isAfter(a) == true
             * </pre>
             * <p>
             * This method only considers the position of the two dates on the local time-line.
             * It does not take into account the chronology, or calendar system.
             *
             * @param other  the other date to compare to
             * @return true if this date is after the specified date
             */
            gbool isAfter(const LocalDate& other) const;

            /**
             * Checks if this date is before the specified date.
             * <p>
             * This checks to see if this date represents a point on the
             * local time-line before the other date.
             * <pre>
             *   LocalDate a = LocalDate.of(2012, 6, 30);
             *   LocalDate b = LocalDate.of(2012, 7, 1);
             *   a.isBefore(b) == true
             *   a.isBefore(a) == false
             *   b.isBefore(a) == false
             * </pre>
             * <p>
             * This method only considers the position of the two dates on the local time-line.
             * It does not take into account the chronology, or calendar system.
             *
             * @param other  the other date to compare to
             * @return true if this date is before the specified date
             */
            gbool isBefore(const LocalDate& other) const;

            //-----------------------------------------------------------------------
            /**
             * Checks if this date is equal to another date.
             * <p>
             * Compares this <b> LocalDate</b> with another ensuring that the date is the same.
             * <p>
             * Only objects of type <b> LocalDate</b> are compared, other types return false.
             * To compare the dates of two <b> Temporal</b> instances, including dates
             * in two different chronologies, use <b style="color:orange;"> ChronoField.EPOCH_DAY</b> as a comparator.
             *
             * @param obj  the object to check, null returns false
             * @return true if this is equal to the other date
             */
            gbool equals(const Object &obj) const override;

            /**
             * A hash code for this date.
             *
             * @return a suitable hash code
             */
            gint hash() const override;

            //-----------------------------------------------------------------------
            /**
             * Outputs this date as a <b> String</b>, such as <b> 2007-12-03</b>.
             * <p>
             * The output will be in the ISO-8601 format <b> uuuu-MM-dd</b>.
             *
             * @return a string representation of this date
             */
            String toString() const override;

        };

    } // core
} // time

#endif //CORE23_LOCALDATE_H
