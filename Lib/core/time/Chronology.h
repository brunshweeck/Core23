//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_CHRONOLOGY_H
#define CORE23_CHRONOLOGY_H

#include <core/time/Temporal.h>
#include <core/time/DateTimeFormatter.h>
#include <core/Comparable.h>
#include <core/util/Locale.h>
#include <core/util/Set.h>
#include <core/util/Map.h>

namespace core {
    namespace time {

        class ChronoLocalDate;

        /**
         * A calendar system, used to organize and identify dates.
         * <p>
         * The main date and time API is built on the ISO calendar system.
         * The chronology operates behind the scenes to represent the general concept of a calendar system.
         * For example, the Japanese, Minguo, Thai Buddhist and others.
         * <p>
         * Most other calendar systems also operate on the shared concepts of year, m and day,
         * linked to the cycles of the Earth around the Sun, and the Moon around the Earth.
         * These shared concepts are defined by <b style="color:orange;"> ChronoField</b>  and are available
         * for use by any <b> Chronology</b>  implementation:
         * <pre> @code
         *   LocalDate isoDate = ...
         *   ThaiBuddhistDate thaiDate = ...
         *   int isoYear = isoDate.get(Temporal::YEAR);
         *   int thaiYear = thaiDate.get(Temporal::YEAR);
         * @endcode </pre>
         * As shown, although the date objects are in different calendar systems, represented by different
         * <b> Chronology</b>  instances, both can be queried using the same constant on <b> ChronoField</b> .
         * For a full discussion of the implications of this, see <b style="color:orange;"> ChronoLocalDate</b> .
         * In general, the advice is to use the known ISO-based <b> LocalDate</b> , rather than
         * <b> ChronoLocalDate</b> .
         * <p>
         * While a <b> Chronology</b>  object typically uses <b> ChronoField</b>  and is based on
         * an era, year-of-era, m-of-year, day-of-m model of a date, this is not required.
         * A <b> Chronology</b>  instance may represent a totally different kind of calendar system,
         * such as the Mayan.
         * <p>
         * In practical terms, the <b> Chronology</b>  instance also acts as a factory.
         * The <b style="color:orange;"> of(String)</b>  method allows an instance to be looked up by identifier,
         * while the <b style="color:orange;"> ofLocale(Locale)</b>  method allows lookup by locale.
         * <p>
         * The <b> Chronology</b>  instance provides a set of methods to create <b> ChronoLocalDate</b>  instances.
         * The date classes are used to manipulate specific dates.
         * <ul>
         * <li> <b style="color:orange;"> currentDate()</b>
         * <li> <b style="color:orange;"> currentDate(clock)</b>
         * <li> <b style="color:orange;"> currentDate(zone)</b>
         * <li> <b style="color:orange;"> date(yearProleptic, m, day)</b>
         * <li> <b style="color:orange;"> date(era, yearOfEra, m, day)</b>
         * <li> <b style="color:orange;"> dateYearDay(yearProleptic, dayOfYear)</b>
         * <li> <b style="color:orange;"> dateYearDay(era, yearOfEra, dayOfYear)</b>
         * <li> <b style="color:orange;"> date(Temporal)</b>
         * </ul>
         *
         * <h2 id="addcalendars">Adding New Calendars</h2>
         * The set of available chronologies can be extended by applications.
         * Adding a new calendar system requires the writing of an implementation of
         * <b> Chronology</b> , <b> ChronoLocalDate</b>  and <b> Era</b> .
         * The majority of the logic specific to the calendar system will be in the
         * <b> ChronoLocalDate</b>  implementation.
         * The <b> Chronology</b>  implementation acts as a factory.
         * <p>
         * To permit the discovery of additional chronologies, the <b style="color:orange;"> ServiceLoader</b>
         * is used.
         * See the ServiceLoader for more details on service loading.
         * For lookup by id or calendarType, the system provided calendars are found
         * first followed by application provided calendars.
         * <p>
         * Each chronology must define a chronology ID that is unique within the system.
         * If the chronology represents a calendar system defined by the
         * CLDR specification then the calendar type is the concatenation of the
         * CLDR type and, if applicable, the CLDR variant.
         *
         * @implSpec
         * This interface must be implemented with care to ensure other classes operate correctly.
         * All implementations that can be instantiated must be final, immutable and thread-safe.
         * Subclasses should be Serializable wherever possible.
         *
         */
        interface Chronology : public Object, public Comparable<Chronology> {

            CORE_ALIAS(Locale, util::Locale);

            /**
             * Obtains an instance of <b> Chronology</b>  from a temporal object.
             * <p>
             * This obtains a chronology based on the specified temporal.
             * A <b> TemporalAccessor</b>  represents an arbitrary set of date and time information,
             * which this factory converts to an instance of <b> Chronology</b> .
             * <p>
             * The conversion will obtain the chronology using <b style="color:orange;"> Query::CHRONOLOGY </b> .
             * If the specified temporal object does not have a chronology, <b style="color:orange;"> ISOChronology</b>  is returned.
             * <p>
             * This method matches the signature of the functional interface <b style="color:orange;"> Query</b>
             * allowing it to be used as a query via method reference, <b> Chronology::from</b> .
             *
             * @param temporal  the temporal to convert
             * @return the chronology
             * @throws DateTimeException if unable to convert to a <b> Chronology</b> 
             */
            static Chronology &from(const Temporal &temporal);

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> Chronology</b>  from a locale.
             * <p>
             * This returns a <b> Chronology</b>  based on the specified locale,
             * typically returning <b> IsoChronology</b> . Other calendar systems
             * are only returned if they are explicitly selected within the locale.
             * <p>
             * The <b style="color:orange;"> Locale</b>  class provide access to a range of information useful
             * for localizing an application. This includes the language and region,
             * such as "en-GB" for English as used in Great Britain.
             * <p>
             * The <b> Locale</b>  class also supports an extension mechanism that
             * can be used to identify a calendar system. The mechanism is a form
             * of key-value pairs, where the calendar system has the key "ca".
             * For example, the locale "en-JP-u-ca-japanese" represents the English
             * language as used in Japan with the Japanese calendar system.
             *
             * @param locale  the locale to use to obtain the calendar system
             * @return the calendar system associated with the locale
             * @throws DateTimeException if the locale-specified calendar cannot be found
             */
            static Chronology &ofLocale(const Locale &locale);

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> Chronology</b>  from a chronology ID or
             * calendar system type.
             * <p>
             * This returns a chronology based on either the ID or the type.
             * The <b style="color:orange;"> chronology ID</b>  uniquely identifies the chronology.
             * The <b style="color:orange;"> calendar system type</b>  is defined by the
             * CLDR specification.
             * <p>
             * The chronology may be a system chronology or a chronology
             * provided by the application via ServiceLoader configuration.
             * <p>
             * Since some calendars can be customized, the ID or type typically refers
             * to the default customization. For example, the Gregorian calendar can have multiple
             * cutover dates from the Julian, but the lookup only provides the default cutover date.
             *
             * @param id  the chronology ID or calendar system type
             * @return the chronology with the identifier requested
             * @throws DateTimeException if the chronology cannot be found
             */
            static Chronology &of(const String &id);

            /**
             * Returns the available chronologies.
             * <p>
             * Each returned <b> Chronology</b>  is available for use in the system.
             * The set of chronologies includes the system chronologies and
             * any chronologies provided by the application via ServiceLoader
             * configuration.
             *
             * @return the independent, modifiable set of the available chronology IDs
             */
            static const util::Set<Chronology> &availableChronologies();

            //-----------------------------------------------------------------------
            /**
             * Gets the ID of the chronology.
             * <p>
             * The ID uniquely identifies the <b> Chronology</b> .
             * It can be used to lookup the <b> Chronology</b>  using <b style="color:orange;"> of(String)</b> .
             *
             * @return the chronology ID
             */
            virtual String id() const = 0;

            /**
             * Gets the calendar type of the calendar system.
             * <p>
             * The calendar type is an identifier defined by the CLDR and
             * <em>Unicode Locale Data Markup Language (LDML)</em> specifications
             * to uniquely identify a calendar.
             * The <b> getCalendarType</b>  is the concatenation of the CLDR calendar type
             * and the variant, if applicable, is appended separated by "-".
             * The calendar type is used to lookup the <b> Chronology</b>  using <b style="color:orange;"> of(String)</b> .
             *
             * @return the calendar system type, null if the calendar is not defined by CLDR/LDML
             */
            virtual String calendarType() const = 0;

            //-----------------------------------------------------------------------
            /**
             * Obtains a local date in this chronology from the era, year-of-era,
             * m-of-year and day-of-m fields.
             *
             * @implSpec
             * The default implementation combines the era and year-of-era into a proleptic
             * year before calling <b style="color:orange;"> date(int, int, int)</b> .
             *
             * @param era  the era of the correct type for the chronology
             * @param yearOfEra  the chronology year-of-era
             * @param month  the chronology m-of-year
             * @param dayOfMonth  the chronology day-of-m
             * @return the local date in this chronology
             * @throws DateTimeException if unable to create the date
             * @throws ClassCastException if the <b> era</b>  is not of the correct type for the chronology
             */
            virtual const ChronoLocalDate &date(const Era &era, int yearOfEra, int month, int dayOfMonth) const = 0;

            /**
             * Obtains a local date in this chronology from the proleptic-year,
             * m-of-year and day-of-m fields.
             *
             * @param prolepticYear  the chronology proleptic-year
             * @param month  the chronology m-of-year
             * @param dayOfMonth  the chronology day-of-m
             * @return the local date in this chronology
             * @throws DateTimeException if unable to create the date
             */
            virtual const ChronoLocalDate &date(int prolepticYear, int month, int dayOfMonth) const = 0;

            /**
             * Obtains a local date in this chronology from the era, year-of-era and
             * day-of-year fields.
             *
             * @implSpec
             * The default implementation combines the era and year-of-era into a proleptic
             * year before calling <b style="color:orange;"> date(int, int)</b> .
             *
             * @param era  the era of the correct type for the chronology
             * @param yearOfEra  the chronology year-of-era
             * @param dayOfYear  the chronology day-of-year
             * @return the local date in this chronology
             * @throws DateTimeException if unable to create the date
             * @throws ClassCastException if the <b> era</b>  is not of the correct type for the chronology
             */
            virtual const ChronoLocalDate &date(const Era &era, int yearOfEra, int dayOfYear) const = 0;

            /**
             * Obtains a local date in this chronology from the proleptic-year and
             * day-of-year fields.
             *
             * @param prolepticYear  the chronology proleptic-year
             * @param dayOfYear  the chronology day-of-year
             * @return the local date in this chronology
             * @throws DateTimeException if unable to create the date
             */
            virtual const ChronoLocalDate &date(int prolepticYear, int dayOfYear) const = 0;

            /**
             * Obtains a local date in this chronology from the epoch-day.
             * <p>
             * The definition of <b style="color:orange;"> EPOCH_DAY</b>  is the same
             * for all calendar systems, thus it can be used for conversion.
             *
             * @param epochDay  the epoch day
             * @return the local date in this chronology
             * @throws DateTimeException if unable to create the date
             */
            virtual const ChronoLocalDate &date(long epochDay) const = 0;

            //-----------------------------------------------------------------------
            /**
             * Obtains the current local date in this chronology from the system clock in the default time-zone.
             *
             * <p>
             * Using this method will prevent the ability to use an alternate clock for testing
             * because the clock is hard-coded.
             *
             * @implSpec
             * The default implementation invokes <b style="color:orange;"> currentDate(Clock)</b> .
             *
             * @return the current local date using the system clock and default time-zone
             * @throws DateTimeException if unable to create the date
             */
            virtual const ChronoLocalDate &currentDate() const = 0;

            /**
             * Obtains the current local date in this chronology from the system clock in the specified time-zone.
             * <p>
             * This will query the <b style="color:orange;"> system clock</b>  to obtain the current date.
             * Specifying the time-zone avoids dependence on the default time-zone.
             * <p>
             * Using this method will prevent the ability to use an alternate clock for testing
             * because the clock is hard-coded.
             *
             * @implSpec
             * The default implementation invokes <b style="color:orange;"> currentDate(Clock)</b> .
             *
             * @param zone  the zone ID to use
             * @return the current local date using the system clock
             * @throws DateTimeException if unable to create the date
             */
            virtual const ChronoLocalDate &currentDate(const ZoneID &zone) const = 0;

            //-----------------------------------------------------------------------
            /**
             * Obtains a local date in this chronology from another temporal object.
             * <p>
             * This obtains a date in this chronology based on the specified temporal.
             * A <b> TemporalAccessor</b>  represents an arbitrary set of date and time information,
             * which this factory converts to an instance of <b> ChronoLocalDate</b> .
             * <p>
             * The conversion typically uses the <b style="color:orange;"> EPOCH_DAY</b>
             * field, which is standardized across calendar systems.
             * <p>
             * This method matches the signature of the functional interface <b style="color:orange;"> TemporalQuery</b> 
             * allowing it to be used as a query via method reference, <b> aChronology::date</b> .
             *
             * @param temporal  the temporal object to convert
             * @return the local date in this chronology
             * @throws DateTimeException if unable to create the date
             *
             */
            virtual const ChronoLocalDate &date(const Temporal &temporal) const = 0;

            //-----------------------------------------------------------------------
            /**
             * Checks if the specified year is a leap year.
             * <p>
             * A leap-year is a year of a longer length than normal.
             * The exact meaning is determined by the chronology according to the following constraints.
             * <ul>
             * <li>a leap-year must imply a year-length longer than a non leap-year.
             * <li>a chronology that does not support the concept of a year must return false.
             * <li>the correct result must be returned for all years within the
             *     valid range of years for the chronology.
             * </ul>
             * <p>
             * Outside the range of valid years an implementation is free to return
             * either a best guess or false.
             * An implementation must not throw an exception, even if the year is
             * outside the range of valid years.
             *
             * @param prolepticYear  the proleptic-year to check, not validated for range
             * @return true if the year is a leap year
             */
            virtual gbool isLeapYear() const = 0;

            /**
             * Calculates the proleptic-year given the era and year-of-era.
             * <p>
             * This combines the era and year-of-era into the single proleptic-year field.
             * <p>
             * If the chronology makes active use of eras, such as <b> JapaneseChronology</b> 
             * then the year-of-era will be validated against the era.
             * For other chronologies, validation is optional.
             *
             * @param era  the era of the correct type for the chronology
             * @param yearOfEra  the chronology year-of-era
             * @return the proleptic-year
             * @throws DateTimeException if unable to convert to a proleptic-year,
             *  such as if the year is invalid for the era
             * @throws ClassCastException if the <b> era</b>  is not of the correct type for the chronology
             */
            virtual gbool prolepticYear(const Era &era, gint yearOfEra) const = 0;

            /**
             * Creates the chronology era object from the numeric value.
             * <p>
             * The era is, conceptually, the largest division of the time-line.
             * Most calendar systems have a single epoch dividing the time-line into two eras.
             * However, some have multiple eras, such as one for the reign of each leader.
             * The exact meaning is determined by the chronology according to the following constraints.
             * <p>
             * The era in use at 1970-01-01 must have the value 1.
             * Later eras must have sequentially higher values.
             * Earlier eras must have sequentially lower values.
             * Each chronology must refer to an enum or similar singleton to provide the era values.
             * <p>
             * This method returns the singleton era of the correct type for the specified era value.
             *
             * @param eraValue  the era value
             * @return the calendar system era
             * @throws DateTimeException if unable to create the era
             */
            virtual const Era &eraOf(gint era) const = 0;

            /**
             * Gets the list of eras for the chronology.
             * <p>
             * Most calendar systems have an era, within which the year has meaning.
             * If the calendar system does not support the concept of eras, an empty
             * list must be returned.
             *
             * @return the list of eras for the chronology, may be immutable
             */
            virtual util::List<Era> &eras() const = 0;

            //-----------------------------------------------------------------------
            /**
             * Gets the textual representation of this chronology.
             * <p>
             * This returns the textual name used to identify the chronology,
             * suitable for presentation to the user.
             * The parameters control the style of the returned text and the locale.
             *
             * @implSpec
             * The default implementation behaves as though the formatter was used to
             * format the chronology textual name.
             *
             * @param style  the style of the text required
             * @param locale  the locale to use
             * @return the text value of the chronology
             */
            virtual String displayName(DateTimeFormatter::TextStyle style, const Locale &locale) const = 0;

            //-----------------------------------------------------------------------
            /**
             * Resolves parsed <b> ChronoField</b>  values into a date during parsing.
             * <p>
             * Most <b> TemporalField</b>  implementations are resolved using the
             * resolve method on the field. By contrast, the <b> ChronoField</b>  class
             * defines fields that only have meaning relative to the chronology.
             * As such, <b> ChronoField</b>  date fields are resolved here in the
             * context of a specific chronology.
             * <p>
             * The default implementation, which explains typical resolve behaviour,
             * is provided in <b style="color:orange;"> AbstractChronology</b> .
             *
             * @param fieldValues  the map of fields to values, which can be updated
             * @param resolverStyle  the requested type of resolve
             * @return the resolved date, null if insufficient information to create a date
             * @throws DateTimeException if the date cannot be resolved, typically
             *  because of a conflict in the input data
             */
            virtual const ChronoLocalDate &resolveDate(const util::Map<Enum<Temporal::Field>, Long> &fieldValues,
                                                       DateTimeFormatter::ResolverStyle resolverStyle) const = 0;

            //---------------------------------------------------------------------

            /**
             * Gets the number of seconds from the epoch of 1970-01-01T00:00:00Z.
             * <p>
             * The number of seconds is calculated using the proleptic-year,
             * m, day-of-m, hour, minute, second, and zoneOffset.
             *
             * @param prolepticYear the chronology proleptic-year
             * @param month the chronology m-of-year
             * @param dayOfMonth the chronology day-of-m
             * @param hour the hour-of-day, from 0 to 23
             * @param minute the minute-of-hour, from 0 to 59
             * @param second the second-of-minute, from 0 to 59
             * @param zoneOffset the zone offset
             * @return the number of seconds relative to 1970-01-01T00:00:00Z, may be negative
             * @throws DateTimeException if any of the values are out of range
             */
            virtual glong epochSecond(int prolepticYear, int month, int dayOfMonth,
                                      int hour, int minute, int second, const ZoneOffset &zoneOffset) const = 0;

            /**
             * Gets the number of seconds from the epoch of 1970-01-01T00:00:00Z.
             * <p>
             * The number of seconds is calculated using the era, year-of-era,
             * m, day-of-m, hour, minute, second, and zoneOffset.
             *
             * @param era  the era of the correct type for the chronology
             * @param yearOfEra the chronology year-of-era
             * @param month the chronology m-of-year
             * @param dayOfMonth the chronology day-of-m
             * @param hour the hour-of-day, from 0 to 23
             * @param minute the minute-of-hour, from 0 to 59
             * @param second the second-of-minute, from 0 to 59
             * @param zoneOffset the zone offset
             * @return the number of seconds relative to 1970-01-01T00:00:00Z, may be negative
             * @throws DateTimeException if any of the values are out of range
             *
             */
            virtual glong epochSecond(const Era &era, int yearOfEra, int month, int dayOfMonth,
                                      int hour, int minute, int second, const ZoneOffset &zoneOffset) const = 0;

            /**
             * Checks if this chronology is ISO based.
             * <p>
             * An ISO based chronology has the same basic structure as the <b style="color:orange;"> IsoChronology
             * ISO chronology</b> , i.e., the chronology has the same number of months, the number
             * of days in each m, and day-of-year and leap years are the same as ISO chronology.
             * It also supports the concept of week-based-year of ISO chronology.
             * For example, the <b style="color:orange;"> MinguoChronology Minguo</b> , <b style="color:orange;"> ThaiBuddhistChronology
             * ThaiThaiBuddhist</b>  and <b style="color:orange;"> JapaneseChronology Japanese</b>  chronologies are ISO based.
             *
             * @implSpec
             * The default implementation returns <b> false</b> .
             *
             * @return <b> true</b>  only if all the fields of <b style="color:orange;"> IsoFields</b>  are supported by
             *          this chronology. Otherwise, returns <b> false</b> .
             * @see IsoChronology
             * @see JapaneseChronology
             * @see MinguoChronology
             * @see ThaiBuddhistChronology
             * @see IsoFields
             * @since 19
             */
             virtual gbool isIsoBased() const = 0;

            //-----------------------------------------------------------------------
            /**
             * Compares this chronology to another chronology.
             * <p>
             * The comparison order first by the chronology ID string, then by any
             * additional information specific to the subclass.
             * It is "consistent with equals", as defined by <b style="color:orange;"> Comparable</b> .
             *
             * @param other  the other chronology to compare to
             * @return the comparator value, negative if less, positive if greater
             */
             gint compareTo(const Chronology& other) const override = 0;

            /**
             * Checks if this chronology is equal to another chronology.
             * <p>
             * The comparison is based on the entire state of the object.
             *
             * @param obj  the object to check, null returns false
             * @return true if this is equal to the other chronology
             */
             gbool equals(const Object& obj) const override = 0;

            /**
             * A hash code for this chronology.
             * <p>
             * The hash code should be based on the entire state of the object.
             *
             * @return a suitable hash code
             */
             gint hash() const override = 0;

            //-----------------------------------------------------------------------
            /**
             * Outputs this chronology as a <b> String</b> .
             * <p>
             * The format should include the entire state of the object.
             *
             * @return a string representation of this chronology
             */
             String toString() const override = 0;
        };

    } // time
}
#endif //CORE23_CHRONOLOGY_H
