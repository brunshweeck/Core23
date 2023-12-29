//
// Created by T.N.Brunshweeck on 23/12/2023.
//

#ifndef CORE23_ERA_H
#define CORE23_ERA_H

#include <core/time/Temporal.h>
#include <core/time/DateTimeFormatter.h>
#include "LocalDate.h"

namespace core {
    namespace time {

        /**
         * An era of the time-line.
         * <p>
         * Most calendar systems have a single epoch dividing the time-line into two eras.
         * However, some calendar systems, have multiple eras, such as one for the reign
         * of each leader.
         * In all cases, the era is conceptually the largest division of the time-line.
         * Each chronology defines the Era's that are known Eras and a
         * <b style="color:orange;"> Chronology.eras</b>  to get the valid eras.
         * <p>
         * For example, the Thai Buddhist calendar system divides time into two eras,
         * before and after a single date. By contrast, the Japanese calendar system
         * has one era for the reign of each Emperor.
         * <p>
         * Instances of <b> Era</b>  may be compared using the <b> ==</b>  operator.
         *
         * @implSpec
         * This interface must be implemented with care to ensure other classes operate correctly.
         * All implementations must be singletons - final, immutable and thread-safe.
         * It is recommended to use an enum whenever possible.
         */
        interface Era : public Temporal {

            /**
             * Gets the numeric value associated with the era as defined by the chronology.
             * Each chronology defines the predefined Eras and methods to list the Eras
             * of the chronology.
             * <p>
             * All fields, including eras, have an associated numeric value.
             * The meaning of the numeric value for era is determined by the chronology
             * according to these principles:
             * <ul>
             * <li>The era in use at the epoch 1970-01-01 (ISO) has the value 1.
             * <li>Later eras have sequentially higher values.
             * <li>Earlier eras have sequentially lower values, which may be negative.
             * </ul>
             *
             * @return the numeric era value
             */
            virtual gint value() const = 0;

            //-----------------------------------------------------------------------
            /**
             * Checks if the specified field is supported.
             * <p>
             * This checks if this era can be queried for the specified field.
             * If false, then calling the <b style="color:orange;"> get</b>  methods will throw an exception.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoField</b>  then the query is implemented here.
             * The <b> ERA</b>  field returns true.
             * All other <b> ChronoField</b>  instances will return false.
             *
             * @param field  the field to check, null returns false
             * @return true if the field is supported on this era, false if not
             */
            gbool supportField(Field field) const override;

            gbool supportUnit(Unit unit) const override;

            /**
             * Gets the value of the specified field from this era as an <b> int</b> .
             * <p>
             * This queries this era for the value of the specified field.
             * The returned value will always be within the valid range of values for the field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoField</b>  then the query is implemented here.
             * The <b> ERA</b>  field returns the value of the era.
             * All other <b> ChronoField</b>  instances will throw an <b> UnsupportedTemporalException</b> .
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
             * Gets the value of the specified field from this era as a <b> long</b> .
             * <p>
             * This queries this era for the value of the specified field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * <p>
             * If the field is a <b style="color:orange;"> ChronoField</b>  then the query is implemented here.
             * The <b> ERA</b>  field returns the value of the era.
             * All other <b> ChronoField</b>  instances will throw an <b> UnsupportedTemporalException</b> .
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
             * Queries this era using the specified query.
             * <p>
             * This queries this era using the specified query strategy object.
             * The <b> Query</b>  object defines the logic to be used to
             * obtain the result. Read the documentation of the query to understand
             * what the result of this method will be.
             *
             * @param <R> the type of the result
             * @param query  the query to invoke, not null
             * @return the query result, null may be returned (defined by the query)
             * @throws DateTimeException if unable to query (defined by the query)
             * @throws ArithmeticException if numeric overflow occurs (defined by the query)
             */
            const Object &query(const Query &query) const override;

            //-----------------------------------------------------------------------
            /**
             * Gets the textual representation of this era.
             * <p>
             * This returns the textual name used to identify the era,
             * suitable for presentation to the user.
             * The parameters control the style of the returned text and the locale.
             * <p>
             * If no textual mapping is found then the <b style="color:orange;"> numeric value</b>  is returned.
             *
             * @apiNote This default implementation is suitable for most implementations.
             *
             * @param style  the style of the text required, not null
             * @param locale  the locale to use, not null
             * @return the text value of the era, not null
             */
            virtual String displayName(DateTimeFormatter::TextStyle style, const LocalDate &localDate) const = 0;

            glong until(const Temporal &endExclusive, TemporalUnit unit) const override;

            //-----------------------------------------------------------------------
            /**
             * An era in the Hijrah calendar system.
             * <p>
             * The Hijrah calendar system has only one era covering the
             * proleptic years greater than zero.
             */
            class HijrahEra;

            /**
             * The singleton instance for the current era, 'Anno Hegirae',
             * which has the numeric value 1.
             */
            static const HijrahEra AH;

            //-----------------------------------------------------------------------

            /**
             * An era in the ISO calendar system.
             * <p>
             * The ISO-8601 standard does not define eras.
             * A definition has therefore been created with two eras - 'Current era' (CE) for
             * years on or after 0001-01-01 (ISO), and 'Before current era' (BCE) for years before that.
             *
             * <table class="striped" style="text-align:left">
             * <caption style="display:none">ISO years and eras</caption>
             * <thead>
             * <tr>
             * <th scope="col">year-of-era</th>
             * <th scope="col">era</th>
             * <th scope="col">proleptic-year</th>
             * </tr>
             * </thead>
             * <tbody>
             * <tr>
             * <td>2</td><td>CE</td><th scope="row">2</th>
             * </tr>
             * <tr>
             * <td>1</td><td>CE</td><th scope="row">1</th>
             * </tr>
             * <tr>
             * <td>1</td><td>BCE</td><th scope="row">0</th>
             * </tr>
             * <tr>
             * <td>2</td><td>BCE</td><th scope="row">-1</th>
             * </tr>
             * </tbody>
             * </table>
             */
            class ISOEra;

            /**
             * The singleton instance for the era before the current one, 'Before Current Era',
             * which has the numeric value 0.
             */
            static const ISOEra BCE;

            /**
             * The singleton instance for the current era, 'Current Era',
             * which has the numeric value 1.
             */
            static const ISOEra CE;

            //-----------------------------------------------------------------------

            /**
             * An era in the Japanese Imperial calendar system.
             * <p>
             * The Japanese government defines the official name and start date of
             * each era. Eras are consecutive and their date ranges do not overlap,
             * so the end date of one era is always the day before the start date
             * of the next era.
             * <p>
             * The Java SE Platform supports all eras defined by the Japanese government,
             * beginning with the Meiji era. Each era is identified in the Platform by an
             * integer value and a name. The <b style="color:orange;"> of(int)</b>  and <b style="color:orange;"> valueOf(String)</b>
             * methods may be used to obtain a singleton instance of <b> JapaneseEra</b>
             * for each era. The <b style="color:orange;"> values()</b>  method returns the singleton instances
             * of all supported eras.
             * <p>
             * For convenience, this class declares a number of public static final fields
             * that refer to singleton instances returned by the <b style="color:orange;"> values()</b>  method.
             *
             * @apiNote
             * The fields declared in this class may evolve over time, in line with the
             * results of the <b style="color:orange;"> values()</b>  method. However, there is not necessarily
             * a 1:1 correspondence between the fields and the singleton instances.
             *
             * @apiNote
             * The Japanese government may announce a new era and define its start
             * date but not its official name. In this scenario, the singleton instance
             * that represents the new era may return a name that is not stable until
             * the official name is defined. Developers should exercise caution when
             * relying on the name returned by any singleton instance that does not
             * correspond to a public static final field.
             */
            class JapaneseEra;

            /**
             * The singleton instance for the 'Meiji' era (1868-01-01 - 1912-07-29)
             * which has the value -1.
             */
            static const JapaneseEra MEIJI;

            /**
             * The singleton instance for the 'Taisho' era (1912-07-30 - 1926-12-24)
             * which has the value 0.
             */
            static const JapaneseEra TAISHO;

            /**
             * The singleton instance for the 'Showa' era (1926-12-25 - 1989-01-07)
             * which has the value 1.
             */
            static const JapaneseEra SHOWA;

            /**
             * The singleton instance for the 'Heisei' era (1989-01-08 - 2019-04-30)
             * which has the value 2.
             */
            static const JapaneseEra HEISEI;

            /**
             * The singleton instance for the 'Reiwa' era (2019-05-01 - )
             * which has the value 3. The end date of this era is not specified, unless
             * the Japanese Government defines it.
             */
            static const JapaneseEra REIWA;

            //-----------------------------------------------------------------------

            /**
             * An era in the Minguo calendar system.
             * <p>
             * The Minguo calendar system has two eras.
             * The current era, for years from 1 onwards, is known as the 'Republic of China' era.
             * All previous years, zero or earlier in the proleptic count or one and greater
             * in the year-of-era count, are part of the 'Before Republic of China' era.
             *
             * <table class="striped" style="text-align:left">
             * <caption style="display:none">Minguo years and eras</caption>
             * <thead>
             * <tr>
             * <th>year-of-era</th>
             * <th>era</th>
             * <th>proleptic-year</th>
             * <th>ISO proleptic-year</th>
             * </tr>
             * </thead>
             * <tbody>
             * <tr>
             * <td>2</td><td>ROC</td><th scope="row">2</th><td>1913</td>
             * </tr>
             * <tr>
             * <td>1</td><td>ROC</td><th scope="row">1</th><td>1912</td>
             * </tr>
             * <tr>
             * <td>1</td><td>BEFORE_ROC</td><th scope="row">0</th><td>1911</td>
             * </tr>
             * <tr>
             * <td>2</td><td>BEFORE_ROC</td><th scope="row">-1</th><td>1910</td>
             * </tr>
             * </tbody>
             * </table>
             */
            class MinguoEra;

            /**
             * The singleton instance for the era before the current one, 'Before Republic of China Era',
             * which has the numeric value 0.
             */
            static const MinguoEra BEFORE_ROC;

            /**
             * The singleton instance for the current era, 'Republic of China Era',
             * which has the numeric value 1.
             */
            static const MinguoEra ROC;

        };

        class Era::HijrahEra : public Era {
        private:
            friend Era;

            CORE_FAST HijrahEra() = default;

            CORE_FAST HijrahEra(const HijrahEra &) = default;

            CORE_FAST HijrahEra(HijrahEra &&) CORE_NOTHROW = default;

        public:

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> HijrahEra</b>  from an <b> int</b>  value.
             * <p>
             * The current era, which is the only accepted value, has the value 1
             *
             * @param era  the era to represent, only 1 supported
             * @return the HijrahEra.AH singleton, not null
             * @throws DateTimeException if the value is invalid
             */
            static const HijrahEra &of(gint era);

            //-----------------------------------------------------------------------
            /**
             * Gets the numeric era <b> int</b>  value.
             * <p>
             * The era AH has the value 1.
             *
             * @return the era value, 1 (AH)
             */
            gint value() const override;

            String displayName(DateTimeFormatter::TextStyle style, const LocalDate &localDate) const override;

            gbool equals(const Object &o) const override;

            Object &clone() const override;
        };

        class Era::ISOEra CORE_FINAL : public Era {
        private:
            friend Era;

            gint era;

            CORE_EXPLICIT ISOEra(gint era);

            CORE_FAST ISOEra(const ISOEra &era) = default;

            CORE_FAST ISOEra(ISOEra &&era) CORE_NOTHROW = default;

        public:

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> IsoEra</b>  from an <b> int</b>  value.
             * <p>
             * <b> IsoEra</b>  is an enum representing the ISO eras of BCE/CE.
             * This factory allows the enum to be obtained from the <b> int</b>  value.
             *
             * @param isoEra  the BCE/CE value to represent, from 0 (BCE) to 1 (CE)
             * @return the era singleton, not null
             * @throws DateTimeException if the value is invalid
             */
            static const ISOEra &of(gint era);

            //-----------------------------------------------------------------------
            /**
             * Gets the numeric era <b> int</b>  value.
             * <p>
             * The era BCE has the value 0, while the era CE has the value 1.
             *
             * @return the era value, from 0 (BCE) to 1 (CE)
             */
            gint value() const override;

            String displayName(DateTimeFormatter::TextStyle style, const LocalDate &localDate) const override;

            gbool equals(const Object &o) const override;

            Object &clone() const override;
        };

        class Era::JapaneseEra : public Era {
        private:
            friend Era;

            /**
             * The era value.
             */
            gint era;

            /**
             * the first day of the era
             */
            LocalDate since;

            /**
             * Creates an instance.
             *
             * @param eraValue  the era value, validated
             * @param since  the date representing the first date of the era, validated not null
             */
            CORE_EXPLICIT JapaneseEra(gint era, const LocalDate &since);

            CORE_FAST JapaneseEra(const JapaneseEra &era) = default;

            CORE_FAST JapaneseEra(JapaneseEra &&era) CORE_NOTHROW = default;

        public:

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> JapaneseEra</b>  from an <b> int</b>  value.
             * <ul>
             * <li>The value <b> 1</b>  is associated with the 'Showa' era, because
             * it contains 1970-01-01 (ISO calendar system).</li>
             * <li>The values <b> -1</b>  and <b> 0</b>  are associated with two earlier
             * eras, Meiji and Taisho, respectively.</li>
             * <li>A value greater than <b> 1</b>  is associated with a later era,
             * beginning with Heisei (<b> 2</b> ).</li>
             * </ul>
             *
             * @param era  the era to represent
             * @return the <b> JapaneseEra</b>  singleton, not null
             * @throws DateTimeException if the value is invalid
             */
            static const JapaneseEra &of(gint era);

            /**
             * Returns the <b> JapaneseEra</b>  with the name.
             * <p>
             * The string must match exactly the name of the era.
             * (Extraneous whitespace characters are not permitted.)
             * <p>
             * Valid era names are the names of eras returned from <b style="color:orange;"> #values()</b> .
             *
             * @param japaneseEra  the japaneseEra name; non-null
             * @return the <b> JapaneseEra</b>  singleton, never null
             * @throws ArgumentException if there is not JapaneseEra with the specified name
             */
            static const JapaneseEra &valueOf(const String &era);

            String displayName(DateTimeFormatter::TextStyle style, const LocalDate &localDate) const override;

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> JapaneseEra</b>  from a date.
             *
             * @param date  the date, not null
             * @return the Era singleton, never null
             */
            static const JapaneseEra &from(const LocalDate &date);

            //-----------------------------------------------------------------------
            /**
             * Gets the numeric era <b> int</b>  value.
             * <p>
             * The <b style="color:orange;"> SHOWA</b>  era that contains 1970-01-01 (ISO calendar system) has the value 1.
             * Later eras are numbered from 2 (<b style="color:orange;"> HEISEI</b> ).
             * Earlier eras are numbered 0 (<b style="color:orange;"> TAISHO</b> ), -1 (<b style="color:orange;"> MEIJI</b> )).
             *
             * @return the era value
             */
            gint value() const override;

            //-----------------------------------------------------------------------
            String abbreviation() const;

            String name() const;

            gbool equals(const Object &o) const override;

            Object &clone() const override;
        };

        class Era::MinguoEra CORE_FINAL : public Era {
        private:
            friend Era;

            gint era;

            CORE_EXPLICIT MinguoEra(gint era);

            CORE_FAST MinguoEra(const MinguoEra &era) = default;

            CORE_FAST MinguoEra(MinguoEra &&era) CORE_NOTHROW = default;

        public:

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of <b> MinguoEra</b>  from an <b> int</b>  value.
             * <p>
             * <b> MinguoEra</b>  is an enum representing the Minguo eras of BEFORE_ROC/ROC.
             * This factory allows the enum to be obtained from the <b> int</b>  value.
             *
             * @param era  the BEFORE_ROC/ROC value to represent, from 0 (BEFORE_ROC) to 1 (ROC)
             * @return the era singleton, not null
             * @throws DateTimeException if the value is invalid
             */
            static const MinguoEra &of(gint era);

            //-----------------------------------------------------------------------
            /**
             * Gets the numeric era <b> int</b>  value.
             * <p>
             * The era BEFORE_ROC has the value 0, while the era ROC has the value 1.
             *
             * @return the era value, from 0 (BEFORE_ROC) to 1 (ROC)
             */
            gint value() const override;

            String displayName(DateTimeFormatter::TextStyle style, const LocalDate &localDate) const override;

            Object &clone() const override;

            gbool equals(const Object &o) const override;
        };

    } // time
} // core

#endif //CORE23_ERA_H
