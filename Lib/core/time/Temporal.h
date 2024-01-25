//
// Created by T.N.Brunshweeck on 26/11/2023.
//

#ifndef CORE23_TEMPORAL_H
#define CORE23_TEMPORAL_H

#include <core/Object.h>

namespace core {
    namespace time {

        class LocalDate;

        class LocalDateTime;

        class LocalTime;

        class ZoneID;

        class ZoneOffset;

        class DateTimeFormatter;

        class Chronology;

        class Era;

        /**
         * Framework-level interface defining read-write access to a temporal object,
         * such as a date, time, offset or some combination of these.
         * <p>
         * This is the base interface type for date, time and offset objects that
         * are complete enough to be manipulated using plus and minus.
         * It is implemented by those classes that can provide and manipulate information
         * as <b style="color:green;"> fields</b>  or <b style="color:green;"> queries</b> .
         * <p>
         * Most date and time information can be represented as a number.
         * These are modeled using <b> Field</b>  with the number held using
         * a <b> glong</b>  to handle large values. Year, m and day-of-m are
         * simple examples of fields, but they also include instant and offsets.
         * <p>
         * Two pieces of date/time information cannot be represented by numbers,
         * the <b style="color:green;"> chronology</b>  and the
         * <b style="color:green;"> time-zone</b> .
         * <p>
         * This interface is a framework-level interface that should not be widely
         * used in application code. Instead, applications should create and pass
         * around instances of concrete types, such as <b> LocalDate</b> .
         * There are many reasons for this, part of which is that implementations
         * of this interface may be in calendar systems other than ISO.
         *
         * <h2>When to implement</h2>
         * <p>
         * A class should implement this interface if it meets three criteria:
         * <ul>
         * <li>it provides access to date/time/offset information.
         * <li>the set of fields are contiguous from the largest to the smallest
         * <li>the set of fields are complete, such that no other field is needed to define the
         *  valid range of values for the fields that are represented
         * </ul>
         * <p>
         * Four examples make this clear:
         * <ul>
         * <li><b> LocalDate</b>  implements this interface as it represents a set of fields
         *  that are contiguous from days to forever and require no external information to determine
         *  the validity of each date. It is therefore able to implement plus/minus correctly.
         * <li><b> LocalTime</b>  implements this interface as it represents a set of fields
         *  that are contiguous from nanos to within days and require no external information to determine
         *  validity. It is able to implement plus/minus correctly, by wrapping around the day.
         * </ul>
         *
         * @implSpec
         * This interface places no restrictions on the mutability of implementations,
         * however immutability is strongly recommended.
         * All implementations must be <b style="color:orange;"> Comparable</b> .
         *
         * @author Brunshweeck Tazeussong
         */
        interface Temporal : public Object {

            /**
             * A standard set of fields.
             * <p>
             * This set of fields provide field-based access to manipulate a date, time or date-time.
             * The standard set of fields have been extended by others temporal fields.
             * <p>
             * These fields are intended to be applicable in multiple calendar systems.
             * For example, most non-ISO calendar systems define dates as a year, m and day,
             * just with slightly different rules.
             * The documentation of each field explains how it operates.
             *
             * @implSpec
             * This is a final, immutable and thread-safe enum.
             *
             */
            enum Field : gbyte {

                /**
                 * The nano-of-second.
                 * <p>
                 * This counts the nanosecond within the second, from 0 to 999,999,999.
                 * This field has the same meaning for all calendar systems.
                 * <p>
                 * This field is used to represent the nano-of-second handling any fraction of the second.
                 * Implementations of <b> Temporal</b>  should provide a value for this field if
                 * they can return a value for <b style="color:orange;"> SECOND_OF_MINUTE</b> , <b style="color:orange;"> SECOND_OF_DAY</b>  or
                 * <b style="color:orange;"> INSTANT_SECONDS</b>  filling unknown precision with zero.
                 * <p>
                 * When this field is used for setting a value, it should set as much precision as the
                 * object stores, using integer division to remove excess precision.
                 * For example, if the <b> Temporal</b>  stores time to millisecond precision,
                 * then the nano-of-second must be divided by 1,000,000 before replacing the milli-of-second.
                 * <p>
                 * When parsing this field it behaves equivalent to the following:
                 * The value is validated in strict and smart mode but not in lenient mode.
                 * The field is resolved in combination with <b> MILLI_OF_SECOND</b>  and <b> MICRO_OF_SECOND</b> .
                 */
                NANO_OF_SECOND,

                /**
                 * The nano-of-day.
                 * <p>
                 * This counts the nanosecond within the day, from 0 to (24 * 60 * 60 * 1,000,000,000) - 1.
                 * This field has the same meaning for all calendar systems.
                 * <p>
                 * This field is used to represent the nano-of-day handling any fraction of the second.
                 * Implementations of <b> Temporal</b>  should provide a value for this field if
                 * they can return a value for <b style="color:orange;"> SECOND_OF_DAY</b>  filling unknown precision with zero.
                 * <p>
                 * When parsing this field it behaves equivalent to the following:
                 * The value is validated in strict and smart mode but not in lenient mode.
                 * The value is split to form <b> NANO_OF_SECOND</b> , <b> SECOND_OF_MINUTE</b> ,
                 * <b> MINUTE_OF_HOUR</b>  and <b> HOUR_OF_DAY</b>  fields.
                 */
                NANO_OF_DAY,

                /**
                 * The micro-of-second.
                 * <p>
                 * This counts the microsecond within the second, from 0 to 999,999.
                 * This field has the same meaning for all calendar systems.
                 * <p>
                 * This field is used to represent the micro-of-second handling any fraction of the second.
                 * Implementations of <b> Temporal</b>  should provide a value for this field if
                 * they can return a value for <b style="color:orange;"> SECOND_OF_MINUTE</b> , <b style="color:orange;"> SECOND_OF_DAY</b>  or
                 * <b style="color:orange;"> INSTANT_SECONDS</b>  filling unknown precision with zero.
                 * <p>
                 * When this field is used for setting a value, it should behave in the same way as
                 * setting <b style="color:orange;"> NANO_OF_SECOND</b>  with the value multiplied by 1,000.
                 * <p>
                 * When parsing this field it behaves equivalent to the following:
                 * The value is validated in strict and smart mode but not in lenient mode.
                 * The field is resolved in combination with <b> MILLI_OF_SECOND</b>  to produce
                 * <b> NANO_OF_SECOND</b> .
                 */
                MICRO_OF_SECOND,

                /**
                 * The micro-of-day.
                 * <p>
                 * This counts the microsecond within the day, from 0 to (24 * 60 * 60 * 1,000,000) - 1.
                 * This field has the same meaning for all calendar systems.
                 * <p>
                 * This field is used to represent the micro-of-day handling any fraction of the second.
                 * Implementations of <b> Temporal</b>  should provide a value for this field if
                 * they can return a value for <b style="color:orange;"> SECOND_OF_DAY</b>  filling unknown precision with zero.
                 * <p>
                 * When this field is used for setting a value, it should behave in the same way as
                 * setting <b style="color:orange;"> NANO_OF_DAY</b>  with the value multiplied by 1,000.
                 * <p>
                 * When parsing this field it behaves equivalent to the following:
                 * The value is validated in strict and smart mode but not in lenient mode.
                 * The value is split to form <b> MICRO_OF_SECOND</b> , <b> SECOND_OF_MINUTE</b> ,
                 * <b> MINUTE_OF_HOUR</b>  and <b> HOUR_OF_DAY</b>  fields.
                 */
                MICRO_OF_DAY,

                /**
                 * The milli-of-second.
                 * <p>
                 * This counts the millisecond within the second, from 0 to 999.
                 * This field has the same meaning for all calendar systems.
                 * <p>
                 * This field is used to represent the milli-of-second handling any fraction of the second.
                 * Implementations of <b> Temporal</b>  should provide a value for this field if
                 * they can return a value for <b style="color:orange;"> SECOND_OF_MINUTE</b> , <b style="color:orange;"> SECOND_OF_DAY</b>  or
                 * <b style="color:orange;"> INSTANT_SECONDS</b>  filling unknown precision with zero.
                 * <p>
                 * When this field is used for setting a value, it should behave in the same way as
                 * setting <b style="color:orange;"> NANO_OF_SECOND</b>  with the value multiplied by 1,000,000.
                 * <p>
                 * When parsing this field it behaves equivalent to the following:
                 * The value is validated in strict and smart mode but not in lenient mode.
                 * The field is resolved in combination with <b> MICRO_OF_SECOND</b>  to produce
                 * <b> NANO_OF_SECOND</b> .
                 */
                MILLI_OF_SECOND,

                /**
                 * The milli-of-day.
                 * <p>
                 * This counts the millisecond within the day, from 0 to (24 * 60 * 60 * 1,000) - 1.
                 * This field has the same meaning for all calendar systems.
                 * <p>
                 * This field is used to represent the milli-of-day handling any fraction of the second.
                 * Implementations of <b> Temporal</b>  should provide a value for this field if
                 * they can return a value for <b style="color:orange;"> SECOND_OF_DAY</b>  filling unknown precision with zero.
                 * <p>
                 * When this field is used for setting a value, it should behave in the same way as
                 * setting <b style="color:orange;"> NANO_OF_DAY</b>  with the value multiplied by 1,000,000.
                 * <p>
                 * When parsing this field it behaves equivalent to the following:
                 * The value is validated in strict and smart mode but not in lenient mode.
                 * The value is split to form <b> MILLI_OF_SECOND</b> , <b> SECOND_OF_MINUTE</b> ,
                 * <b> MINUTE_OF_HOUR</b>  and <b> HOUR_OF_DAY</b>  fields.
                 */
                MILLI_OF_DAY,

                /**
                 * The second-of-minute.
                 * <p>
                 * This counts the second within the minute, from 0 to 59.
                 * This field has the same meaning for all calendar systems.
                 * <p>
                 * When parsing this field it behaves equivalent to the following:
                 * The value is validated in strict and smart mode but not in lenient mode.
                 */
                SECOND_OF_MINUTE,

                /**
                 * The second-of-day.
                 * <p>
                 * This counts the second within the day, from 0 to (24 * 60 * 60) - 1.
                 * This field has the same meaning for all calendar systems.
                 * <p>
                 * When parsing this field it behaves equivalent to the following:
                 * The value is validated in strict and smart mode but not in lenient mode.
                 * The value is split to form <b> SECOND_OF_MINUTE</b> , <b> MINUTE_OF_HOUR</b> 
                 * and <b> HOUR_OF_DAY</b>  fields.
                 */
                SECOND_OF_DAY,

                /**
                 * The minute-of-hour.
                 * <p>
                 * This counts the minute within the hour, from 0 to 59.
                 * This field has the same meaning for all calendar systems.
                 * <p>
                 * When parsing this field it behaves equivalent to the following:
                 * The value is validated in strict and smart mode but not in lenient mode.
                 */
                MINUTE_OF_HOUR,

                /**
                 * The minute-of-day.
                 * <p>
                 * This counts the minute within the day, from 0 to (24 * 60) - 1.
                 * This field has the same meaning for all calendar systems.
                 * <p>
                 * When parsing this field it behaves equivalent to the following:
                 * The value is validated in strict and smart mode but not in lenient mode.
                 * The value is split to form <b> MINUTE_OF_HOUR</b>  and <b> HOUR_OF_DAY</b>  fields.
                 */
                MINUTE_OF_DAY,

                /**
                 * The hour-of-am-pm.
                 * <p>
                 * This counts the hour within the AM/PM, from 0 to 11.
                 * This is the hour that would be observed on a standard 12-hour digital clock.
                 * This field has the same meaning for all calendar systems.
                 * <p>
                 * When parsing this field it behaves equivalent to the following:
                 * The value is validated from 0 to 11 in strict and smart mode.
                 * In lenient mode the value is not validated. It is combined with
                 * <b> AMPM_OF_DAY</b>  to form <b> HOUR_OF_DAY</b>  by multiplying
                 * the <b> AMPM_OF_DAY</b>  value by 12.
                 * <p>
                 * See <b style="color:orange;"> CLOCK_HOUR_OF_AMPM</b>  for the related field that counts hours from 1 to 12.
                 */
                HOUR_OF_AMPM,

                /**
                 * The clock-hour-of-am-pm.
                 * <p>
                 * This counts the hour within the AM/PM, from 1 to 12.
                 * This is the hour that would be observed on a standard 12-hour analog wall clock.
                 * This field has the same meaning for all calendar systems.
                 * <p>
                 * When parsing this field it behaves equivalent to the following:
                 * The value is validated from 1 to 12 in strict mode and from
                 * 0 to 12 in smart mode. In lenient mode the value is not validated.
                 * The field is converted to an <b> HOUR_OF_AMPM</b>  with the same value,
                 * unless the value is 12, in which case it is converted to 0.
                 * <p>
                 * See <b style="color:orange;"> HOUR_OF_AMPM</b>  for the related field that counts hours from 0 to 11.
                 */
                CLOCK_HOUR_OF_AMPM,

                /**
                 * The hour-of-day.
                 * <p>
                 * This counts the hour within the day, from 0 to 23.
                 * This is the hour that would be observed on a standard 24-hour digital clock.
                 * This field has the same meaning for all calendar systems.
                 * <p>
                 * When parsing this field it behaves equivalent to the following:
                 * The value is validated in strict and smart mode but not in lenient mode.
                 * The field is combined with <b> MINUTE_OF_HOUR</b> , <b> SECOND_OF_MINUTE</b>  and
                 * <b> NANO_OF_SECOND</b>  to produce a <b> LocalTime</b> .
                 * <p>
                 * See <b style="color:orange;"> CLOCK_HOUR_OF_DAY</b>  for the related field that counts hours from 1 to 24.
                 */
                HOUR_OF_DAY,

                /**
                 * The clock-hour-of-day.
                 * <p>
                 * This counts the hour within the day, from 1 to 24.
                 * This is the hour that would be observed on a 24-hour analog wall clock.
                 * This field has the same meaning for all calendar systems.
                 * <p>
                 * When parsing this field it behaves equivalent to the following:
                 * The value is validated from 1 to 24 in strict mode and from
                 * 0 to 24 in smart mode. In lenient mode the value is not validated.
                 * The field is converted to an <b> HOUR_OF_DAY</b>  with the same value,
                 * unless the value is 24, in which case it is converted to 0.
                 * <p>
                 * See <b style="color:orange;"> HOUR_OF_DAY</b>  for the related field that counts hours from 0 to 23.
                 */
                CLOCK_HOUR_OF_DAY,

                /**
                 * The am-pm-of-day.
                 * <p>
                 * This counts the AM/PM within the day, from 0 (AM) to 1 (PM).
                 * This field has the same meaning for all calendar systems.
                 * <p>
                 * When parsing this field it behaves equivalent to the following:
                 * The value is validated from 0 to 1 in strict and smart mode.
                 * In lenient mode the value is not validated. It is combined with
                 * <b> HOUR_OF_AMPM</b>  (if not present, it defaults to '6') to form
                 * <b> HOUR_OF_DAY</b>  by multiplying the <b> AMPM_OF_DAY</b>  value
                 * by 12.
                 */
                AMPM_OF_DAY,

                /**
                 * The day-of-week, such as Tuesday.
                 * <p>
                 * This represents the standard concept of the day of the week.
                 * In the default ISO calendar system, this has values from Monday (1) to Sunday (7).
                 * The <b style="color:orange;"> DayOfWeek</b>  class can be used to interpret the result.
                 * <p>
                 * Most non-ISO calendar systems also define a seven day week that aligns with ISO.
                 * Those calendar systems must also use the same numbering system, from Monday (1) to
                 * Sunday (7), which allows <b> DayOfWeek</b>  to be used.
                 * <p>
                 * Chronology systems that do not have a standard seven day week should implement this field
                 * if they have a similar concept of named or numbered days within a period similar
                 * to a week. It is recommended that the numbering starts from 1.
                 */
                DAY_OF_WEEK,

                /**
                 * The aligned day-of-week within a m.
                 * <p>
                 * This represents concept of the count of days within the period of a week
                 * where the weeks are aligned to the start of the m.
                 * This field is typically used with <b style="color:orange;"> ALIGNED_WEEK_OF_MONTH</b> .
                 * <p>
                 * For example, in a calendar systems with a seven day week, the first aligned-week-of-m
                 * starts on day-of-m 1, the second aligned-week starts on day-of-m 8, and so on.
                 * Within each of these aligned-weeks, the days are numbered from 1 to 7 and returned
                 * as the value of this field.
                 * As such, day-of-m 1 to 7 will have aligned-day-of-week values from 1 to 7.
                 * And day-of-m 8 to 14 will repeat this with aligned-day-of-week values from 1 to 7.
                 * <p>
                 * Chronology systems that do not have a seven day week should typically implement this
                 * field in the same way, but using the alternate week length.
                 */
                ALIGNED_DAY_OF_WEEK_IN_MONTH,

                /**
                 * The aligned day-of-week within a year.
                 * <p>
                 * This represents concept of the count of days within the period of a week
                 * where the weeks are aligned to the start of the year.
                 * This field is typically used with <b style="color:orange;"> ALIGNED_WEEK_OF_YEAR</b> .
                 * <p>
                 * For example, in a calendar systems with a seven day week, the first aligned-week-of-year
                 * starts on day-of-year 1, the second aligned-week starts on day-of-year 8, and so on.
                 * Within each of these aligned-weeks, the days are numbered from 1 to 7 and returned
                 * as the value of this field.
                 * As such, day-of-year 1 to 7 will have aligned-day-of-week values from 1 to 7.
                 * And day-of-year 8 to 14 will repeat this with aligned-day-of-week values from 1 to 7.
                 * <p>
                 * Chronology systems that do not have a seven day week should typically implement this
                 * field in the same way, but using the alternate week length.
                 */
                ALIGNED_DAY_OF_WEEK_IN_YEAR,

                /**
                 * The day-of-m.
                 * <p>
                 * This represents the concept of the day within the m.
                 * In the default ISO calendar system, this has values from 1 to 31 in most months.
                 * April, June, September, November have days from 1 to 30, while February has days
                 * from 1 to 28, or 29 in a leap year.
                 * <p>
                 * Non-ISO calendar systems should implement this field using the most recognized
                 * day-of-m values for users of the calendar system.
                 * Normally, this is a count of days from 1 to the length of the m.
                 */
                DAY_OF_MONTH,

                /**
                 * The day-of-year.
                 * <p>
                 * This represents the concept of the day within the year.
                 * In the default ISO calendar system, this has values from 1 to 365 in standard
                 * years and 1 to 366 in leap years.
                 * <p>
                 * Non-ISO calendar systems should implement this field using the most recognized
                 * day-of-year values for users of the calendar system.
                 * Normally, this is a count of days from 1 to the length of the year.
                 * <p>
                 * Note that a non-ISO calendar system may have year numbering system that changes
                 * at a different point to the natural reset in the m numbering. An example
                 * of this is the Japanese calendar system where a change of era, which resets
                 * the year number to 1, can happen on any date. The era and year reset also cause
                 * the day-of-year to be reset to 1, but not the m-of-year or day-of-m.
                 */
                DAY_OF_YEAR,

                /**
                 * The epoch-day, based on the Java epoch of 1970-01-01 (ISO).
                 * <p>
                 * This field is the sequential count of days where 1970-01-01 (ISO) is zero.
                 * Note that this uses the <i>local</i> time-line, ignoring offset and time-zone.
                 * <p>
                 * This field is strictly defined to have the same meaning in all calendar systems.
                 * This is necessary to ensure interoperation between calendars.
                 * <p>
                 * Range of EpochDay is between (LocalDate.MIN.toEpochDay(), LocalDate.MAX.toEpochDay())
                 * both inclusive.
                 */
                EPOCH_DAY,

                /**
                 * The aligned week within a m.
                 * <p>
                 * This represents concept of the count of weeks within the period of a m
                 * where the weeks are aligned to the start of the m.
                 * This field is typically used with <b style="color:orange;"> ALIGNED_DAY_OF_WEEK_IN_MONTH</b> .
                 * <p>
                 * For example, in a calendar systems with a seven day week, the first aligned-week-of-m
                 * starts on day-of-m 1, the second aligned-week starts on day-of-m 8, and so on.
                 * Thus, day-of-m values 1 to 7 are in aligned-week 1, while day-of-m values
                 * 8 to 14 are in aligned-week 2, and so on.
                 * <p>
                 * Chronology systems that do not have a seven day week should typically implement this
                 * field in the same way, but using the alternate week length.
                 */
                ALIGNED_WEEK_OF_MONTH,

                /**
                 * The aligned week within a year.
                 * <p>
                 * This represents concept of the count of weeks within the period of a year
                 * where the weeks are aligned to the start of the year.
                 * This field is typically used with <b style="color:orange;"> ALIGNED_DAY_OF_WEEK_IN_YEAR</b> .
                 * <p>
                 * For example, in a calendar systems with a seven day week, the first aligned-week-of-year
                 * starts on day-of-year 1, the second aligned-week starts on day-of-year 8, and so on.
                 * Thus, day-of-year values 1 to 7 are in aligned-week 1, while day-of-year values
                 * 8 to 14 are in aligned-week 2, and so on.
                 * <p>
                 * Chronology systems that do not have a seven day week should typically implement this
                 * field in the same way, but using the alternate week length.
                 */
                ALIGNED_WEEK_OF_YEAR,

                /**
                 * The m-of-year, such as March.
                 * <p>
                 * This represents the concept of the m within the year.
                 * In the default ISO calendar system, this has values from January (1) to December (12).
                 * <p>
                 * Non-ISO calendar systems should implement this field using the most recognized
                 * m-of-year values for users of the calendar system.
                 * Normally, this is a count of months starting from 1.
                 */
                MONTH_OF_YEAR,

                /**
                 * The proleptic-m based, counting months sequentially from year 0.
                 * <p>
                 * This field is the sequential count of months where the first m
                 * in proleptic-year zero has the value zero.
                 * Later months have increasingly larger values.
                 * Earlier months have increasingly small values.
                 * There are no gaps or breaks in the sequence of months.
                 * Note that this uses the <i>local</i> time-line, ignoring offset and time-zone.
                 * <p>
                 * In the default ISO calendar system, June 2012 would have the value
                 * <b> (2012 * 12 + 6 - 1)</b> . This field is primarily for internal use.
                 * <p>
                 * Non-ISO calendar systems must implement this field as per the definition above.
                 * It is just a simple zero-based count of elapsed months from the start of proleptic-year 0.
                 * All calendar systems with a full proleptic-year definition will have a year zero.
                 * If the calendar system has a minimum year that excludes year zero, then one must
                 * be extrapolated in order for this method to be defined.
                 */
                PROLEPTIC_MONTH,

                /**
                 * The year within the era.
                 * <p>
                 * This represents the concept of the year within the era.
                 * This field is typically used with <b style="color:orange;"> ERA</b> .
                 * <p>
                 * The standard mental model for a date is based on three concepts - year, m and day.
                 * These map onto the <b> YEAR</b> , <b> MONTH_OF_YEAR</b>  and <b> DAY_OF_MONTH</b>  fields.
                 * Note that there is no reference to eras.
                 * The full model for a date requires four concepts - era, year, m and day. These map onto
                 * the <b> ERA</b> , <b> YEAR_OF_ERA</b> , <b> MONTH_OF_YEAR</b>  and <b> DAY_OF_MONTH</b>  fields.
                 * Whether this field or <b> YEAR</b>  is used depends on which mental model is being used.
                 * <p>
                 * In the default ISO calendar system, there are two eras defined, 'BCE' and 'CE'.
                 * The era 'CE' is the one currently in use and year-of-era runs from 1 to the maximum value.
                 * The era 'BCE' is the previous era, and the year-of-era runs backwards.
                 * <p>
                 * For example, subtracting a year each time yield the following:<br>
                 * - year-proleptic 2  = 'CE' year-of-era 2<br>
                 * - year-proleptic 1  = 'CE' year-of-era 1<br>
                 * - year-proleptic 0  = 'BCE' year-of-era 1<br>
                 * - year-proleptic -1 = 'BCE' year-of-era 2<br>
                 * <p>
                 * Note that the ISO-8601 standard does not actually define eras.
                 * Note also that the ISO eras do not align with the well-known AD/BC eras due to the
                 * change between the Julian and Gregorian calendar systems.
                 * <p>
                 * Non-ISO calendar systems should implement this field using the most recognized
                 * year-of-era value for users of the calendar system.
                 * Since most calendar systems have only two eras, the year-of-era numbering approach
                 * will typically be the same as that used by the ISO calendar system.
                 * The year-of-era value should typically always be positive, however this is not required.
                 */
                YEAR_OF_ERA,

                /**
                 * The proleptic year, such as 2012.
                 * <p>
                 * This represents the concept of the year, counting sequentially and using negative numbers.
                 * The proleptic year is not interpreted in terms of the era.
                 * See <b style="color:orange;"> YEAR_OF_ERA</b>  for an example showing the mapping from proleptic year to year-of-era.
                 * <p>
                 * The standard mental model for a date is based on three concepts - year, m and day.
                 * These map onto the <b> YEAR</b> , <b> MONTH_OF_YEAR</b>  and <b> DAY_OF_MONTH</b>  fields.
                 * Note that there is no reference to eras.
                 * The full model for a date requires four concepts - era, year, m and day. These map onto
                 * the <b> ERA</b> , <b> YEAR_OF_ERA</b> , <b> MONTH_OF_YEAR</b>  and <b> DAY_OF_MONTH</b>  fields.
                 * Whether this field or <b> YEAR_OF_ERA</b>  is used depends on which mental model is being used.
                 * <p>
                 * Non-ISO calendar systems should implement this field as follows.
                 * If the calendar system has only two eras, before and after a fixed date, then the
                 * proleptic-year value must be the same as the year-of-era value for the later era,
                 * and increasingly negative for the earlier era.
                 * If the calendar system has more than two eras, then the proleptic-year value may be
                 * defined with any appropriate value, although defining it to be the same as ISO may be
                 * the best option.
                 */
                YEAR,

                /**
                 * The era.
                 * <p>
                 * This represents the concept of the era, which is the largest division of the time-line.
                 * This field is typically used with <b style="color:orange;"> YEAR_OF_ERA</b> .
                 * <p>
                 * In the default ISO calendar system, there are two eras defined, 'BCE' and 'CE'.
                 * The era 'CE' is the one currently in use and year-of-era runs from 1 to the maximum value.
                 * The era 'BCE' is the previous era, and the year-of-era runs backwards.
                 * See <b style="color:orange;"> YEAR_OF_ERA</b>  for a full example.
                 * <p>
                 * Non-ISO calendar systems should implement this field to define eras.
                 * The value of the era that was active on 1970-01-01 (ISO) must be assigned the value 1.
                 * Earlier eras must have sequentially smaller values.
                 * Later eras must have sequentially larger values,
                 */
                ERA,

                /**
                 * The instant epoch-seconds.
                 * <p>
                 * This represents the concept of the sequential count of seconds where
                 * 1970-01-01T00:00Z (ISO) is zero.
                 * This field may be used with <b style="color:orange;"> NANO_OF_SECOND</b>  to represent the fraction of the second.
                 * <p>
                 * An <b style="color:orange;"> Instant</b>  represents an instantaneous point on the time-line.
                 * On their own, an instant has insufficient information to allow a local date-time to be obtained.
                 * Only when paired with an offset or time-zone can the local date or time be calculated.
                 * <p>
                 * This field is strictly defined to have the same meaning in all calendar systems.
                 * This is necessary to ensure interoperation between calendars.
                 */
                INSTANT_SECONDS,

                /**
                 * The offset from UTC/Greenwich.
                 * <p>
                 * This represents the concept of the offset in seconds of local time from UTC/Greenwich.
                 * <p>
                 * A <b style="color:orange;"> ZoneOffset</b>  represents the period of time that local time differs from UTC/Greenwich.
                 * This is usually a fixed number of hours and minutes.
                 * It is equivalent to the <b style="color:orange;"> total amount</b>  of the offset in seconds.
                 * For example, during the winter Paris has an offset of <b> +01:00</b> , which is 3600 seconds.
                 * <p>
                 * This field is strictly defined to have the same meaning in all calendar systems.
                 * This is necessary to ensure interoperation between calendars.
                 */
                OFFSET_SECONDS,

                // ----------------------------------------------------------------------
                //      Non Standard Fields
                // ----------------------------------------------------------------------

                /**
                 * Field number  indicating the daylight saving offset in milliseconds.
                 * <p>
                 * This field reflects the correct daylight saving offset value of
                 * the time zone of this Calendar if the <b> TimeZone} implementation
                 * supports historical Daylight Saving Time schedule changes.
                 */
                DAYLIGHT_SAVING_OFFSET,

            };

            CORE_ALIAS(ChronoField, Field);
            CORE_ALIAS(TemporalField, Field);

            /**
             * A standard set of date periods units.
             * <p>
             * This set of units provide unit-based access to manipulate a date, time or date-time.
             * The standard set of units have been extended by others temporal units.
             * <p>
             * These units are intended to be applicable in multiple calendar systems.
             * For example, most non-ISO calendar systems define units of years, months and days,
             * just with slightly different rules.
             * The documentation of each unit explains how it operates.
             *
             * @implSpec
             * This is a final, immutable and thread-safe enum.
             */
            enum Unit : gbyte {

                /**
                 * Unit that represents the concept of a nanosecond, the smallest supported unit of time.
                 * For the ISO calendar system, it is equal to the 1,000,000,000th part of the second unit.
                 */
                NANOS,

                /**
                 * Unit that represents the concept of a microsecond.
                 * For the ISO calendar system, it is equal to the 1,000,000th part of the second unit.
                 */
                MICROS,

                /**
                 * Unit that represents the concept of a millisecond.
                 * For the ISO calendar system, it is equal to the 1000th part of the second unit.
                 */
                MILLIS,

                /**
                 * Unit that represents the concept of a second.
                 * For the ISO calendar system, it is equal to the second in the SI system
                 * of units, except around a leap-second.
                 */
                SECONDS,

                /**
                 * Unit that represents the concept of a minute.
                 * For the ISO calendar system, it is equal to 60 seconds.
                 */
                MINUTES,

                /**
                 * Unit that represents the concept of an hour.
                 * For the ISO calendar system, it is equal to 60 minutes.
                 */
                HOURS,

                /**
                 * Unit that represents the concept of half a day, as used in AM/PM.
                 * For the ISO calendar system, it is equal to 12 hours.
                 */
                HALF_DAYS,

                /**
                 * Unit that represents the concept of a day.
                 * For the ISO calendar system, it is the standard day from midnight to midnight.
                 * The estimated duration of a day is <b> 24 Hours</b> .
                 * <p>
                 * When used with other calendar systems it must correspond to the day defined by
                 * the rising and setting of the Sun on Earth. It is not required that days begin
                 * at midnight - when converting between calendar systems, the date should be
                 * equivalent at midday.
                 */
                DAYS,

                /**
                 * Unit that represents the concept of a week.
                 * For the ISO calendar system, it is equal to 7 days.
                 * <p>
                 * When used with other calendar systems it must correspond to an integral number of days.
                 */
                WEEKS,

                /**
                 * Unit that represents the concept of a m.
                 * For the ISO calendar system, the length of the m varies by m-of-year.
                 * The estimated duration of a m is one twelfth of <b> 365.2425 Days</b> .
                 * <p>
                 * When used with other calendar systems it must correspond to an integral number of days.
                 */
                MONTHS,

                /**
                 * Unit that represents the concept of a year.
                 * For the ISO calendar system, it is equal to 12 months.
                 * The estimated duration of a year is <b> 365.2425 Days</b> .
                 * <p>
                 * When used with other calendar systems it must correspond to an integral number of days
                 * or months roughly equal to a year defined by the passage of the Earth around the Sun.
                 */
                YEARS,

                /**
                 * Unit that represents the concept of a decade.
                 * For the ISO calendar system, it is equal to 10 years.
                 * <p>
                 * When used with other calendar systems it must correspond to an integral number of days
                 * and is normally an integral number of years.
                 */
                DECADES,

                /**
                 * Unit that represents the concept of a century.
                 * For the ISO calendar system, it is equal to 100 years.
                 * <p>
                 * When used with other calendar systems it must correspond to an integral number of days
                 * and is normally an integral number of years.
                 */
                CENTURIES,

                /**
                 * Unit that represents the concept of a millennium.
                 * For the ISO calendar system, it is equal to 1000 years.
                 * <p>
                 * When used with other calendar systems it must correspond to an integral number of days
                 * and is normally an integral number of years.
                 */
                MILLENNIA,

                /**
                 * Unit that represents the concept of an era.
                 * The ISO calendar system doesn't have eras thus it is impossible to add
                 * an era to a date or date-time.
                 * The estimated duration of the era is artificially defined as <b> 1,000,000,000 Years</b> .
                 * <p>
                 * When used with other calendar systems there are no restrictions on the unit.
                 */
                ERAS,

                /**
                 * Artificial unit that represents the concept of forever.
                 * This is primarily used with <b style="color:orange;"> Field</b>  to represent unbounded fields
                 * such as the year or era.
                 * The estimated duration of this unit is artificially defined as the largest duration
                 * supported by <b style="color:orange;"> Duration</b>.
                 */
                FOREVER,

            };

            CORE_ALIAS(ChronoUnit, Unit);
            CORE_ALIAS(TemporalUnit, Unit);

            /**
             * Enumeration of the style of text formatting and parsing.
             * <p>
             * Text styles define three sizes for the formatted text - 'full', 'short' and 'narrow'.
             * Each of these three sizes is available in both 'standard' and 'stand-alone' variations.
             * <p>
             * The difference between the three sizes is obvious in most languages.
             * For example, in English the 'full' m is 'January', the 'short' m is 'Jan'
             * and the 'narrow' m is 'J'. Note that the narrow size is often not unique.
             * For example, 'January', 'June' and 'July' all have the 'narrow' text 'J'.
             * <p>
             * The difference between the 'standard' and 'stand-alone' forms is trickier to describe
             * as there is no difference in English. However, in other languages there is a difference
             * in the word used when the text is used alone, as opposed to in a complete date.
             * For example, the word used for a m when used alone in a date picker is different
             * to the word used for m in association with a day and year in a date.
             *
             * @implSpec
             * This is an immutable and thread-safe enum.
             *
             */
            enum FormatStyle : gbyte {
                // ordered from large to small
                // ordered so that bit 0 of the ordinal indicates stand-alone.

                /**
                 * Full text, typically the full description.
                 * For example, day-of-week Monday might output "Monday".
                 */
                FULL,

                /**
                 * Full text for stand-alone use, typically the full description.
                 * For example, day-of-week Monday might output "Monday".
                 */
                FULL_STANDALONE,

                /**
                 * Short text, typically an abbreviation.
                 * For example, day-of-week Monday might output "Mon".
                 */
                SHORT,

                /**
                 * Short text for stand-alone use, typically an abbreviation.
                 * For example, day-of-week Monday might output "Mon".
                 */
                SHORT_STANDALONE,

                /**
                 * Narrow text, typically a single letter.
                 * For example, day-of-week Monday might output "M".
                 */
                NARROW,

                /**
                 * Narrow text for stand-alone use, typically a single letter.
                 * For example, day-of-week Monday might output "M".
                 */
                NARROW_STANDALONE,
            };

            /**
             * Checks if the specified field is supported.
             * <p>
             * This checks if the date-time can be queried for the specified field.
             * If false, then calling the <b style="color:orange;"> get</b> methods will throw an exception.
             *
             * @implSpec
             * Implementations must check and handle all fields defined in <b style="color:orange;"> Field</b> .
             * If the field is supported, then true must be returned, otherwise false must be returned.
             *
             * <p>
             * Implementations must ensure that no observable state is altered when this
             * read-only method is invoked.
             *
             * @param field  the field to check, null returns false
             * @return true if this date-time can be queried for the field, false if not
             */
            virtual gbool supportField(Field field) const;

            /**
             * Checks if the specified unit is supported.
             * <p>
             * This checks if the specified unit can be added to, or subtracted from, this date-time.
             *
             * @implSpec
             * Implementations must check and handle all units defined in <b style="color:orange;"> Unit</b> .
             * If the unit is supported, then true must be returned, otherwise false must be returned.
             *
             * <p>
             * Implementations must ensure that no observable state is altered when this
             * read-only method is invoked.
             *
             * @param unit  the unit to check, null returns false
             * @return true if the unit can be added/subtracted, false if not
             */
            virtual gbool supportUnit(Unit unit) const;

            /**
             * Gets the value of the specified field as an <b> gint</b>.
             * <p>
             * This queries the date-time for the value of the specified field.
             * The returned value will always be within the valid range of values for the field.
             * If the date-time cannot return the value, because the field is unsupported or for
             * some other reason, an exception will be thrown.
             *
             * @implSpec
             * Implementations must check and handle all fields defined in <b style="color:orange;"> ChronoField</b>.
             * If the field is supported and has an <b> gint</b> range, then the value of
             * the field must be returned.
             * If unsupported, then an <b> UnsupportedTemporalException</b> must be thrown.
             *
             * <p>
             * Implementations must ensure that no observable state is altered when this
             * read-only method is invoked.
             *
             * @param field  the field to get
             * @return the value for the field, within the valid range of values
             * @throws DateTimeException if a value for the field cannot be obtained or
             *         the value is outside the range of valid values for the field
             * @throws UnsupportedTemporalException if the field is not supported or
             *         the range of values exceeds an <b> gint</b>
             * @throws ArithmeticException if numeric overflow occurs
             */
            virtual gint get(TemporalField field) const;

            /**
             * Gets the value of the specified field as a <b> glong</b> .
             * <p>
             * This queries the date-time for the value of the specified field.
             * The returned value may be outside the valid range of values for the field.
             * If the date-time cannot return the value, because the field is unsupported or for
             * some other reason, an exception will be thrown.
             *
             * @implSpec
             * Implementations must check and handle all fields defined in <b style="color:orange;"> Field</b> .
             * If the field is supported, then the value of the field must be returned.
             * If unsupported, then an <b> UnsupportedTemporalException</b>  must be thrown.
             *
             * <p>
             * Implementations must ensure that no observable state is altered when this
             * read-only method is invoked.
             *
             * @param field  the field to get
             * @return the value for the field
             * @throws DateTimeException if a value for the field cannot be obtained
             * @throws UnsupportedTemporalException if the field is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            virtual glong getLong(TemporalField field) const;

            /**
             * Calculates the amount of time until another temporal in terms of the specified unit.
             * <p>
             * This calculates the amount of time between two temporal objects
             * in terms of a single <b> TemporalUnit</b> .
             * The start and end points are <b> this</b>  and the specified temporal.
             * The end point is converted to be of the same type as the start point if different.
             * The result will be negative if the end is before the start.
             * For example, the amount in hours between two temporal objects can be
             * calculated using <b> startTime.until(endTime, HOURS)</b> .
             * <p>
             * The calculation returns a whole number, representing the number of
             * complete units between the two temporals.
             * For example, the amount in hours between the times 11:30 and 13:29
             * will only be one hour as it is one minute short of two hours.
             *
             * @implSpec
             * Implementations must begin by checking to ensure that the input temporal
             * object is of the same observable type as the implementation.
             * They must then perform the calculation for all instances of <b style="color:orange;"> Unit</b> .
             * An <b> UnsupportedTemporalException</b>  must be thrown for <b> Unit</b>
             * instances that are unsupported.
             *
             * <p>
             * Implementations must ensure that no observable state is altered when this
             * read-only method is invoked.
             *
             * @param endExclusive  the end temporal, exclusive, converted to be of the
             *  same type as this object
             * @param unit  the unit to measure the amount in
             * @return the amount of time between this temporal object and the specified one
             *  in terms of the unit; positive if the specified object is later than this one,
             *  negative if it is earlier than this one
             * @throws DateTimeException if the amount cannot be calculated, or the end
             *  temporal cannot be converted to the same type as this temporal
             * @throws UnsupportedTemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            virtual glong until(const Temporal &endExclusive, TemporalUnit unit) const = 0;

            /**
             * Check if the specified value is valid for given temporal field
             *
             * @throws IllegalArgumentException
             */
            static glong checkValue(TemporalField field, glong value);

        public:

            interface Query : public Object {

                /**
                 * Queries the specified temporal object.
                 * <p>
                 * This queries the specified temporal object to return an object using the logic
                 * encapsulated in the implementing class.
                 * Examples might be a query that checks if the date is the day before February 29th
                 * in a leap year, or calculates the number of days to your next birthday.
                 * <p>
                 * There are two equivalent ways of using this method.
                 * The first is to invoke this method directly.
                 * The second is to use <b style="color:orange;"> Temporal#query(TemporalQuery)</b>:
                 * <pre>
                 *   // these two lines are equivalent, but the second approach is recommended
                 *   temporal = thisQuery.queryFrom(temporal);
                 *   temporal = temporal.query(thisQuery);
                 * </pre>
                 * It is recommended to use the second approach, <b> query(TemporalQuery)</b>,
                 * as it is a lot clearer to read in code.
                 *
                 * @implSpec
                 * The implementation must take the input object and query it.
                 * The implementation defines the logic of the query and is responsible for
                 * documenting that logic.
                 * It may use any method on <b> Temporal</b> to determine the result.
                 * The input object must not be altered.
                 * <p>
                 * The input temporal object may be in a calendar system other than ISO.
                 * Implementations may choose to document compatibility with other calendar systems,
                 * or reject non-ISO temporal objects by <b style="color:orange;"> querying the chronology</b>.
                 * <p>
                 * This method may be called from multiple threads in parallel.
                 * It must be thread-safe when invoked.
                 *
                 * @param temporal  the temporal object to query
                 * @return the queried value, may return null to indicate not found
                 * @throws DateTimeException if unable to query
                 * @throws ArithmeticException if numeric overflow occurs
                 */
                virtual const Object &queryFrom(const Temporal &temporal) const = 0;
            };

            static const Query &LOCAL_TIME;
            static const Query &LOCAL_DATE;
            static const Query &CHRONOLOGY;
            static const Query &PRECISION;
            static const Query &ZONE_ID;
            static const Query &OFFSET;

            /**
             * Queries this date-time.
             * <p>
             * This queries this date-time using the specified query strategy object.
             * <p>
             * Queries are a key tool for extracting information from date-times.
             * They exists to externalize the process of querying, permitting different
             * approaches, as per the strategy design pattern.
             * Examples might be a query that checks if the date is the day before February 29th
             * in a leap year, or calculates the number of days to your next birthday.
             * <p>
             * The most common query implementations are method references, such as
             * <b> LocalDate::from</b> and <b> ZoneId::from</b>.
             * Additional implementations are provided as static methods on <b style="color:orange;"> TemporalQuery</b>.
             *
             * @implSpec
             * The default implementation must behave equivalent to this code:
             * <pre>
             * @code
             *  if (query == ZONE_ID || query == CHRONOLOGY || query == PRECISION) {
             *    return null;
             *  }
             *  return query.queryFrom(this);
             *  @endcode
             * </pre>
             * Future versions are permitted to add further queries to the if statement.
             * <p>
             * All classes implementing this interface and overriding this method must call
             * <b> Temporal::query(query)</b>. JDK classes may avoid calling
             * super if they provide behavior equivalent to the default behaviour, however
             * non-JDK classes may not utilize this optimization and must call <b> super</b>.
             * <p>
             * If the implementation can supply a value for one of the queries listed in the
             * if statement of the default implementation, then it must do so.
             * For example, an application-defined <b> HourMin</b> class storing the hour
             * and minute must override this method as follows:
             * <pre>
             *  if (query == PRECISION) {
             *    return MINUTES;
             *  }
             *  return Temporal::query(query);
             * </pre>
             * <p>
             * Implementations must ensure that no observable state is altered when this
             * read-only method is invoked.
             *
             * @param query  the query to invoke
             * @return the query result, null may be returned (defined by the query)
             * @throws DateTimeException if unable to query
             * @throws ArithmeticException if numeric overflow occurs
             */
            virtual const Object &query(const Query &query) const;
        };

    } // core
} // time

#endif //CORE23_TEMPORAL_H
