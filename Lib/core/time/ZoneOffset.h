//
// Created by T.N.Brunshweeck on 25/12/2023.
//

#ifndef CORE23_ZONEOFFSET_H
#define CORE23_ZONEOFFSET_H

#include <core/time/ZoneID.h>
#include <core/time/ZoneRules.h>
#include <core/time/LocalTime.h>


namespace core {
    namespace time {
        /**
         * A time-zone offset from Greenwich/UTC, such as <b> +02:00</b> .
         * <p>
         * A time-zone offset is the amount of time that a time-zone differs from Greenwich/UTC.
         * This is usually a fixed number of hours and minutes.
         * <p>
         * Different parts of the world have different time-zone offsets.
         * The rules for how offsets vary by place and time of year are captured in the
         * <b style="color:orange;"> ZoneId</b>  class.
         * <p>
         * For example, Paris is one hour ahead of Greenwich/UTC in winter and two hours
         * ahead in summer. The <b> ZoneId</b>  instance for Paris will reference two
         * <b> ZoneOffset</b>  instances - a <b> +01:00</b>  instance for winter,
         * and a <b> +02:00</b>  instance for summer.
         * <p>
         * In 2008, time-zone offsets around the world extended from -12:00 to +14:00.
         * To prevent any problems with that range being extended, yet still provide
         * validation, the range of offsets is restricted to -18:00 to 18:00 inclusive.
         * <p>
         * This class is designed for use with the ISO calendar system.
         * The fields of hours, minutes and seconds make assumptions that are valid for the
         * standard ISO definitions of those fields. This class may be used with other
         * calendar systems providing the definition of the time fields matches those
         * of the ISO calendar system.
         * <p>
         * Instances of <b> ZoneOffset</b>  must be compared using <b style="color:orange;"> equals</b> .
         * Implementations may choose to cache certain common offsets, however
         * applications must not rely on such caching.
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
        class ZoneOffset CORE_FINAL : public ZoneID, public Comparable<ZoneOffset> {
        private:
            /** Cache of time-zone offset by offset in seconds. */
            static util::HashMap<Integer, ZoneOffset> SECONDS_CACHE;

            /** Cache of time-zone offset by ID. */
            static util::HashMap<String, ZoneOffset> ID_CACHE;

            /**
             * The abs maximum seconds.
             */
            static CORE_FAST gint MAX_SECONDS = 18 * LocalTime::SECONDS_PER_HOUR;

        public:
            /**
             * The time-zone offset for UTC, with an ID of 'Z'.
             */
            static const ZoneOffset UTC;

            /**
             * Constant for the minimum supported offset.
             */
            static const ZoneOffset MIN;

            /**
             * Constant for the maximum supported offset.
             */
            static const ZoneOffset MAX;

        private:
            static const ZoneOffset INVALID;

            /**
             * The total offset in seconds.
             */
            gint seconds = 0;

            /**
             * The string form of the time-zone offset.
             */
            String ID;

            ZoneRules zRules;

        public:
            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> ZoneOffset</b>  using the ID.
             * <p>
             * This method parses the string ID of a <b> ZoneOffset</b>  to
             * return an instance. The parsing accepts all the formats generated by
             * <b style="color:orange;"> id()</b> , plus some additional formats:
             * <ul>
             * <li><b> Z</b>  - for UTC
             * <li><b> +h</b> 
             * <li><b> +hh</b> 
             * <li><b> +hh:mm</b> 
             * <li><b> -hh:mm</b> 
             * <li><b> +hhmm</b> 
             * <li><b> -hhmm</b> 
             * <li><b> +hh:mm:ss</b> 
             * <li><b> -hh:mm:ss</b> 
             * <li><b> +hhmmss</b> 
             * <li><b> -hhmmss</b> 
             * </ul>
             * Note that &plusmn; means either the plus or minus symbol.
             * <p>
             * The ID of the returned offset will be normalized to one of the formats
             * described by <b style="color:orange;"> id()</b> .
             * <p>
             * The maximum supported range is from +18:00 to -18:00 inclusive.
             *
             * @param offsetId  the offset ID, not null
             * @return the zone-offset, not null
             * @throws DateTimeException if the offset ID is invalid
             */
            static ZoneOffset of(const String &offsetId);

            ZoneOffset(const ZoneOffset &offset);

            ZoneOffset(ZoneOffset &&offset) CORE_NOTHROW;

            ZoneOffset &operator=(const ZoneOffset &offset);

            ZoneOffset &operator=(ZoneOffset &&offset) CORE_NOTHROW;

        private:
            /**
             * Parse a two digit zero-prefixed number.
             *
             * @param offsetId  the offset ID, not null
             * @param pos  the position to parse, valid
             * @param precededByColon  should this number be prefixed by a precededByColon
             * @return the parsed number, from 0 to 99
             */
            static gint parseNumber(const String &offsetId, gint pos, gbool precededByColon);

        public:
            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> ZoneOffset</b>  using an offset in hours.
             *
             * @param hours  the time-zone offset in hours, from -18 to +18
             * @return the zone-offset, not null
             * @throws DateTimeException if the offset is not in the required range
             */
            static ZoneOffset of(gint hours);

            /**
             * Obtains an instance of <b> ZoneOffset</b>  using an offset in
             * hours and minutes.
             * <p>
             * The sign of the hours and minutes components must match.
             * Thus, if the hours is negative, the minutes must be negative or zero.
             * If the hours is zero, the minutes may be positive, negative or zero.
             *
             * @param hours  the time-zone offset in hours, from -18 to +18
             * @param minutes  the time-zone offset in minutes, from 0 to &plusmn;59, sign matches hours
             * @return the zone-offset, not null
             * @throws DateTimeException if the offset is not in the required range
             */
            static ZoneOffset of(gint hours, gint minutes);

            /**
             * Obtains an instance of <b> ZoneOffset</b>  using an offset in
             * hours, minutes and seconds.
             * <p>
             * The sign of the hours, minutes and seconds components must match.
             * Thus, if the hours is negative, the minutes and seconds must be negative or zero.
             *
             * @param hours  the time-zone offset in hours, from -18 to +18
             * @param minutes  the time-zone offset in minutes, from 0 to &plusmn;59, sign matches hours and seconds
             * @param seconds  the time-zone offset in seconds, from 0 to &plusmn;59, sign matches hours and minutes
             * @return the zone-offset, not null
             * @throws DateTimeException if the offset is not in the required range
             */
            static ZoneOffset of(gint hours, gint minutes, gint seconds);

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> ZoneOffset</b>  from a temporal object.
             * <p>
             * This obtains an offset based on the specified temporal.
             * A <b> TemporalAccessor</b>  represents an arbitrary set of date and time information,
             * which this factory converts to an instance of <b> ZoneOffset</b> .
             * <p>
             * A <b> TemporalAccessor</b>  represents some form of date and time information.
             * This factory converts the arbitrary temporal object to an instance of <b> ZoneOffset</b> .
             * <p>
             * The conversion uses the <b style="color:orange;"> Queries::offset()</b>  query, which relies
             * on extracting the <b style="color:orange;"> OFFSET_SECONDS</b>  field.
             * <p>
             * This method matches the signature of the functional interface <b style="color:orange;"> TemporalQuery</b> 
             * allowing it to be used as a query via method reference, <b> ZoneOffset::from</b> .
             *
             * @param temporal  the temporal object to convert, not null
             * @return the zone-offset, not null
             * @throws DateTimeException if unable to convert to an <b> ZoneOffset</b> 
             */
            static ZoneOffset from(const Temporal &temporal);

        private:
            //-----------------------------------------------------------------------
            /**
             * Validates the offset fields.
             *
             * @param hours  the time-zone offset in hours, from -18 to +18
             * @param minutes  the time-zone offset in minutes, from 0 to &plusmn;59
             * @param seconds  the time-zone offset in seconds, from 0 to &plusmn;59
             * @throws DateTimeException if the offset is not in the required range
             */
            static void validate(gint hours, gint minutes, gint seconds);

        public:
            /**
             * Calculates the total offset in seconds.
             *
             * @param hours  the time-zone offset in hours, from -18 to +18
             * @param minutes  the time-zone offset in minutes, from 0 to &plusmn;59, sign matches hours and seconds
             * @param seconds  the time-zone offset in seconds, from 0 to &plusmn;59, sign matches hours and minutes
             * @return the total in seconds
             */
            static gint totalSeconds(gint hours, gint minutes, gint seconds);

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> ZoneOffset</b>  specifying the total offset in seconds
             * <p>
             * The offset must be in the range <b> -18:00</b>  to <b> +18:00</b> , which corresponds to -64800 to +64800.
             *
             * @param totalSeconds  the total time-zone offset in seconds, from -64800 to +64800
             * @return the ZoneOffset, not null
             * @throws DateTimeException if the offset is not in the required range
             */
            static ZoneOffset ofSeconds(gint totalSeconds);

        private:
            //-----------------------------------------------------------------------
            /**
             * Constructor.
             *
             * @param totalSeconds  the total time-zone offset in seconds, from -64800 to +64800
             */
            CORE_EXPLICIT ZoneOffset(gint totalSeconds);

        public:
            //-----------------------------------------------------------------------
            /**
             * Gets the total zone offset in seconds.
             * <p>
             * This is the primary way to access the offset amount.
             * It returns the total of the hours, minutes and seconds fields as a
             * single offset that can be added to a time.
             *
             * @return the total zone offset amount in seconds
             */
            gint totalSeconds() const;

            /**
             * Gets the normalized zone offset ID.
             * <p>
             * The ID is minor variation to the standard ISO-8601 formatted string
             * for the offset. There are three formats:
             * <ul>
             * <li><b> Z</b>  - for UTC (ISO-8601)
             * <li><b> +hh:mm</b>  or <b> -hh:mm</b>  - if the seconds are zero (ISO-8601)
             * <li><b> +hh:mm:ss</b>  or <b> -hh:mm:ss</b>  - if the seconds are non-zero (not ISO-8601)
             * </ul>
             *
             * @return the zone offset ID, not null
             */
            String id() const override;

            /**
             * Gets the associated time-zone rules.
             * <p>
             * The rules will always return this offset when queried.
             * The implementation class is immutable, thread-safe and serializable.
             *
             * @return the rules, not null
             */
            ZoneRules rules() const override;

            ZoneOffset offset(glong epochSecond) const override;

            //-----------------------------------------------------------------------
            /**
             * Checks if the specified field is supported.
             * <p>
             * This checks if this offset can be queried for the specified field.
             * If false, then calling the <b style="color:orange;"> get</b>  methods will throw an exception.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoField</b>  then the query is implemented here.
             * The <b> OFFSET_SECONDS</b>  field returns true.
             * All other <b> ChronoField</b>  instances will return false.
             * <p>
             * If the field is not a <b> ChronoField</b> , then the result of this method
             * is obtained by invoking <b> TemporalField.isSupportedBy(TemporalAccessor)</b> 
             * passing <b> this</b>  as the argument.
             * Whether the field is supported is determined by the field.
             *
             * @param field  the field to check, null returns false
             * @return true if the field is supported on this offset, false if not
             */
            gbool supportField(Field field) const override;

            /**
             * Gets the value of the specified field from this offset as an <b> int</b> .
             * <p>
             * This queries this offset for the value of the specified field.
             * The returned value will always be within the valid range of values for the field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoField</b>  then the query is implemented here.
             * The <b> OFFSET_SECONDS</b>  field returns the value of the offset.
             * All other <b> ChronoField</b>  instances will throw an <b> UnsupportedTemporalTypeException</b> .
             * <p>
             * If the field is not a <b> ChronoField</b> , then the result of this method
             * is obtained by invoking <b> TemporalField.getFrom(TemporalAccessor)</b> 
             * passing <b> this</b>  as the argument. Whether the value can be obtained,
             * and what the value represents, is determined by the field.
             *
             * @param field  the field to get, not null
             * @return the value for the field
             * @throws DateTimeException if a value for the field cannot be obtained or
             *         the value is outside the range of valid values for the field
             * @throws UnsupportedTemporalTypeException if the field is not supported or
             *         the range of values exceeds an <b> int</b> 
             * @throws ArithmeticException if numeric overflow occurs
             */
            gint get(TemporalField field) const override;

            /**
             * Gets the value of the specified field from this offset as a <b> long</b> .
             * <p>
             * This queries this offset for the value of the specified field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoField</b>  then the query is implemented here.
             * The <b> OFFSET_SECONDS</b>  field returns the value of the offset.
             * All other <b> ChronoField</b>  instances will throw an <b> UnsupportedTemporalTypeException</b> .
             * <p>
             * If the field is not a <b> ChronoField</b> , then the result of this method
             * is obtained by invoking <b> TemporalField.getFrom(TemporalAccessor)</b> 
             * passing <b> this</b>  as the argument. Whether the value can be obtained,
             * and what the value represents, is determined by the field.
             *
             * @param field  the field to get, not null
             * @return the value for the field
             * @throws DateTimeException if a value for the field cannot be obtained
             * @throws UnsupportedTemporalTypeException if the field is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            glong getLong(TemporalField field) const override;

            //-----------------------------------------------------------------------
            /**
             * Queries this offset using the specified query.
             * <p>
             * This queries this offset using the specified query strategy object.
             * The <b> TemporalQuery</b>  object defines the logic to be used to
             * obtain the result. Read the documentation of the query to understand
             * what the result of this method will be.
             * <p>
             * The result of this method is obtained by invoking the
             * <b style="color:orange;"> Query::queryFrom(TemporalAccessor)</b>  method on the
             * specified query passing <b> this</b>  as the argument.
             *
             *
             * @param query  the query to invoke, not null
             * @return the query result, null may be returned (defined by the query)
             * @throws DateTimeException if unable to query (defined by the query)
             * @throws ArithmeticException if numeric overflow occurs (defined by the query)
             */
            const Object &query(const Query &query) const override;

            //-----------------------------------------------------------------------
            /**
             * Compares this offset to another offset in descending order.
             * <p>
             * The offsets are compared in the order that they occur for the same time
             * of day around the world. Thus, an offset of <b> +10:00</b>  comes before an
             * offset of <b> +09:00</b>  and so on down to <b> -18:00</b> .
             * <p>
             * The comparison is "consistent with equals", as defined by <b style="color:orange;"> Comparable</b> .
             *
             * @param other  the other date to compare to, not null
             * @return the comparator value, negative if less, positive if greater
             * @throws NullPointerException if <b> other</b>  is null
             */
            gint compareTo(const ZoneOffset &other) const override;

            //-----------------------------------------------------------------------
            /**
             * Checks if this offset is equal to another offset.
             * <p>
             * The comparison is based on the amount of the offset in seconds.
             * This is equivalent to a comparison by ID.
             *
             * @param obj  the object to check, null returns false
             * @return true if this is equal to the other offset
             */
            gbool equals(const Object &obj) const override;

            /**
             * A hash code for this offset.
             *
             * @return a suitable hash code
             */
            gint hash() const override;

            //-----------------------------------------------------------------------
            /**
             * Outputs this offset as a <b> String</b> , using the normalized ID.
             *
             * @return a string representation of this offset, not null
             */
            String toString() const override;

            Object &clone() const override;
        };
    } // time
} // core

#endif //CORE23_ZONEOFFSET_H