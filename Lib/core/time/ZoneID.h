//
// Created by T.N.Brunshweeck on 24/12/2023.
//

#ifndef CORE23_ZONEID_H
#define CORE23_ZONEID_H

#include <core/String.h>
#include <core/util/Map.h>
#include <core/util/Locale.h>
#include <core/time/Temporal.h>
#include <core/time/DateTimeFormatter.h>


namespace core {
    namespace time {

        /**
         * A time-zone ID, such as <b> Europe/Paris</b> .
         * <p>
         * A <b> ZoneId</b>  is used to identify the rules used to convert between
         * an <b style="color:orange;"> Instant</b>  and a <b style="color:orange;"> LocalDateTime</b> .
         * There are two distinct types of ID:
         * <ul>
         * <li>Fixed offsets - a fully resolved offset from UTC/Greenwich, that uses
         *  the same offset for all local date-times
         * <li>Geographical regions - an area where a specific setValue of rules for finding
         *  the offset from UTC/Greenwich apply
         * </ul>
         * Most fixed offsets are represented by <b style="color:orange;"> ZoneOffset</b> .
         * Calling <b style="color:orange;"> #normalized()</b>  on any <b> ZoneId</b>  will ensure that a
         * fixed offset ID will be represented as a <b> ZoneOffset</b> .
         * <p>
         * The actual rules, describing when and how the offset changes, are defined by <b style="color:orange;"> ZoneRules</b> .
         * This class is simply an ID used to obtain the underlying rules.
         * This approach is taken because rules are defined by governments and change
         * frequently, whereas the ID is stable.
         * <p>
         * The distinction has other effects. Serializing the <b> ZoneId</b>  will only send
         * the ID, whereas serializing the rules sends the entire data setValue.
         * Similarly, a comparison of two IDs only examines the ID, whereas
         * a comparison of two rules examines the entire data setValue.
         *
         * <h2>Time-zone IDs</h2>
         * The ID is unique within the system.
         * There are three types of ID.
         * <p>
         * The simplest type of ID is that from <b> ZoneOffset</b> .
         * This consists of 'Z' and IDs starting with '+' or '-'.
         * <p>
         * The next type of ID are offset-style IDs with some form of prefix,
         * such as 'GMT+2' or 'UTC+01:00'.
         * The recognised prefixes are 'UTC', 'GMT' and 'UT'.
         * The offset is the suffix and will be normalized during creation.
         * These IDs can be normalized to a <b> ZoneOffset</b>  using <b> normalized()</b> .
         * <p>
         * The third type of ID are region-based IDs. A region-based ID must be of
         * two or more characters, and not start with 'UTC', 'GMT', 'UT' '+' or '-'.
         * Region-based IDs are defined by configuration, see <b style="color:orange;"> ZoneRulesProvider</b> .
         * The configuration focuses on providing the lookup from the ID to the
         * underlying <b> ZoneRules</b> .
         * <p>
         * Time-zone rules are defined by governments and change frequently.
         * There are a number of organizations, known here as groups, that monitor
         * time-zone changes and collate them.
         * The default group is the IANA Time ZoneID Database (TZDB).
         * Other organizations include IATA (the airline industry body) and Microsoft.
         * <p>
         * Each group defines its own format for the region ID it provides.
         * The TZDB group defines IDs such as 'Europe/London' or 'America/New_York'.
         * TZDB IDs take precedence over other groups.
         * <p>
         * It is strongly recommended that the group name is included in all IDs supplied by
         * groups other than TZDB to avoid conflicts. For example, IATA airline time-zone
         * region IDs are typically the same as the three letter airport code.
         * However, the airport of Utrecht has the code 'UTC', which is obviously a conflict.
         * The recommended format for region IDs from groups other than TZDB is 'group~region'.
         * Thus if IATA data were defined, Utrecht airport would be 'IATA~UTC'.
         *
         * <h2>Serialization</h2>
         * This class can be serialized and stores the string zone ID in the external form.
         * The <b> ZoneOffset</b>  subclass uses a dedicated format that only stores the
         * offset from UTC/Greenwich.
         * <p>
         * A <b> ZoneId</b>  can be deserialized in a Java Runtime where the ID is unknown.
         * For example, if a server-side Java Runtime has been updated with a new zone ID, but
         * the client-side Java Runtime has not been updated. In this case, the <b> ZoneId</b> 
         * object will exist, and can be queried using <b> getId</b> , <b> equals</b> ,
         * <b> hashCode</b> , <b> toString</b> , <b> getDisplayName</b>  and <b> normalized</b> .
         * However, any call to <b> getRules</b>  will fail with <b> ZoneRulesException</b> .
         * This approach is designed to allow a <b style="color:orange;"> ZonedDateTime</b>  to be loaded and
         * queried, but not modified, on a Java Runtime with incomplete time-zone information.
         * <p>
         * This is a <a href="">value-based</a> class; programmers should treat instances that are
         * <b style="color:green;"> #equals(Object) equal</b>  as interchangeable and should not
         * use instances for synchronization, or unpredictable behavior may
         * occur. For example, in a future release, synchronization may fail.
         * The <b> equals</b>  method should be used for comparisons.
         *
         * @implSpec
         * This abstract sealed class permits two implementations, both of which are immutable and
         * thread-safe. One implementation models region-based IDs, the other is <b> ZoneOffset</b> 
         * modelling offset-based IDs. This difference is visible in serialization.
         *
         */
        class ZoneID : public Temporal {
        private:
            /**
             * A map of zone overrides to enable the short time-zone names to be used.
             * <p>
             * Use of short zone IDs has been deprecated in <b> java.util.TimeZone</b> .
             * This map allows the IDs to continue to be used via the
             * <b style="color:orange;"> of(String, Map)</b>  factory method.
             * <p>
             * This map contains a mapping of the IDs that is in line with TZDB 2005r and
             * later, where 'EST', 'MST' and 'HST' map to IDs which do not include daylight
             * savings.
             * <p>
             * This maps as follows:
             * <ul>
             * <li>EST - -05:00</li>
             * <li>HST - -10:00</li>
             * <li>MST - -07:00</li>
             * <li>ACT - Australia/Darwin</li>
             * <li>AET - Australia/Sydney</li>
             * <li>AGT - America/Argentina/Buenos_Aires</li>
             * <li>ART - Africa/Cairo</li>
             * <li>AST - America/Anchorage</li>
             * <li>BET - America/Sao_Paulo</li>
             * <li>BST - Asia/Dhaka</li>
             * <li>CAT - Africa/Harare</li>
             * <li>CNT - America/St_Johns</li>
             * <li>CST - America/Chicago</li>
             * <li>CTT - Asia/Shanghai</li>
             * <li>EAT - Africa/Addis_Ababa</li>
             * <li>ECT - Europe/Paris</li>
             * <li>IET - America/Indiana/Indianapolis</li>
             * <li>IST - Asia/Kolkata</li>
             * <li>JST - Asia/Tokyo</li>
             * <li>MIT - Pacific/Apia</li>
             * <li>NET - Asia/Yerevan</li>
             * <li>NST - Pacific/Auckland</li>
             * <li>PLT - Asia/Karachi</li>
             * <li>PNT - America/Phoenix</li>
             * <li>PRT - America/Puerto_Rico</li>
             * <li>PST - America/Los_Angeles</li>
             * <li>SST - Pacific/Guadalcanal</li>
             * <li>VST - Asia/Ho_Chi_Minh</li>
             * </ul>
             * The map is unmodifiable.
             */
            static const util::Map<String, String> &ZIDs;

            /**
             * The map containing informations of all available time-zone
             */
            static const util::HashMap<String, ZoneRules> tzInfos;

            CORE_FRATERNITY(ZoneRegion);
            CORE_FRATERNITY(ZoneOffset);

        public:
            //-----------------------------------------------------------------------
            /**
             * Gets the system default time-zone.
             * <p>
             * This queries <b style="color:orange;"> TimeZone#systemDefault()</b>  to find the default time-zone
             * and converts it to a <b> ZoneId</b> . If the system default time-zone is changed,
             * then the result of this method will also change.
             *
             * @return the zone ID
             * @throws DateTimeException if the converted zone ID has an invalid format
             * @throws ZoneRulesException if the converted zone region ID cannot be found
             */
            static const ZoneID &systemDefault();

            /**
             * Gets the setValue of available zone IDs.
             * <p>
             * This setValue includes the string form of all available region-based IDs.
             * Offset-based zone IDs are not included in the returned setValue.
             * The ID can be passed to <b style="color:orange;"> of(String)</b>  to create a <b> ZoneId</b> .
             * <p>
             * The setValue of zone IDs can increase over time, although in a typical application
             * the setValue of IDs is fixed. Each call to this method is thread-safe.
             *
             * @return a modifiable copy of the setValue of zone IDs
             */
            static const util::Set<String> &availableZones();

            //-----------------------------------------------------------------------
            /**
             * Obtains an INSTANCE of <b> ZoneID</b>  using its ID using a map
             * of aliases to supplement the standard zone IDs.
             * <p>
             * Many users of time-zones use short abbreviations, such as PST for
             * 'Pacific Standard Time' and PDT for 'Pacific Daylight Time'.
             * These abbreviations are not unique, and so cannot be used as IDs.
             * This method allows a map of string to time-zone to be setup and reused
             * within an application.
             *
             * @param id  the time-zone ID
             * @param aliasMap  a map of alias zone IDs (typically abbreviations) to real zone IDs
             * @return the zone ID
             * @throws DateTimeException if the zone ID has an invalid format
             * @throws ZoneRulesException if the zone ID is a region ID that cannot be found
             */
            static const ZoneID &of(const String &id, const util::Map<String, String> &aliasMap);

            /**
             * Obtains an INSTANCE of <b> ZoneId</b>  from an ID ensuring that the
             * ID is valid and available for use.
             * <p>
             * This method parses the ID producing a <b> ZoneId</b>  or <b> ZoneOffset</b> .
             * A <b> ZoneOffset</b>  is returned if the ID is 'Z', or starts with '+' or '-'.
             * The result will always be a valid ID for which <b style="color:orange;"> ZoneRules</b>  can be obtained.
             * <p>
             * Parsing matches the zone ID step by step as follows.
             * <ul>
             * <li>If the zone ID equals 'Z', the result is <b> ZoneOffset.UTC</b> .
             * <li>If the zone ID consists of a single letter, the zone ID is invalid
             *  and <b> DateTimeException</b>  is thrown.
             * <li>If the zone ID starts with '+' or '-', the ID is parsed as a
             *  <b> ZoneOffset</b>  using <b style="color:orange;"> ZoneOffset#of(String)</b> .
             * <li>If the zone ID equals 'GMT', 'UTC' or 'UT' then the result is a <b> ZoneId</b> 
             *  with the same ID and rules equivalent to <b> ZoneOffset.UTC</b> .
             * <li>If the zone ID starts with 'UTC+', 'UTC-', 'GMT+', 'GMT-', 'UT+' or 'UT-'
             *  then the ID is a prefixed offset-based ID. The ID is split in two, with
             *  a two or three letter prefix and a suffix starting with the sign.
             *  The suffix is parsed as a <b style="color:orange;"> ZoneOffset#of(String) ZoneOffset</b> .
             *  The result will be a <b> ZoneId</b>  with the specified UTC/GMT/UT prefix
             *  and the normalized offset ID as per <b style="color:orange;"> ZoneOffset#getId()</b> .
             *  The rules of the returned <b> ZoneId</b>  will be equivalent to the
             *  parsed <b> ZoneOffset</b> .
             * <li>All other IDs are parsed as region-based zone IDs. Region IDs must
             *  match the regular expression <b> [A-Za-z][A-Za-z0-9~/._+-]+</b> 
             *  otherwise a <b> DateTimeException</b>  is thrown. If the zone ID is not
             *  in the configured setValue of IDs, <b> ZoneRulesException</b>  is thrown.
             *  The detailed format of the region ID depends on the group supplying the data.
             *  The default setValue of data is supplied by the IANA Time ZoneID Database (TZDB).
             *  This has region IDs of the form '{area</b> /{city</b> ', such as 'Europe/Paris' or 'America/New_York'.
             *  This is compatible with most IDs from <b style="color:orange;"> java.util.TimeZone</b> .
             * </ul>
             *
             * @param ID  the time-zone ID
             * @return the zone ID
             * @throws DateTimeException if the zone ID has an invalid format
             * @throws ZoneRulesException if the zone ID is a region ID that cannot be found
             */
            static ZoneID &of(const String &ID);

            /**
             * Obtains an INSTANCE of <b> ZoneId</b>  wrapping an offset.
             * <p>
             * If the prefix is "GMT", "UTC", or "UT" a <b> ZoneId</b> 
             * with the prefix and the non-zero offset is returned.
             * If the prefix is empty <b> ""</b>  the <b> ZoneOffset</b>  is returned.
             *
             * @param prefix  the time-zone ID
             * @param offset  the offset
             * @return the zone ID
             * @throws IllegalArgumentException if the prefix is not one of
             *     "GMT", "UTC", or "UT", or ""
             */
            static ZoneID &of(const String &prefix, const ZoneOffset &offset);

            //-----------------------------------------------------------------------
            /**
             * Obtains an INSTANCE of <b> ZoneId</b>  from a temporal object.
             * <p>
             * This obtains a zone based on the specified temporal.
             * A <b> TemporalAccessor</b>  represents an arbitrary setValue of date and time information,
             * which this factory converts to an INSTANCE of <b> ZoneId</b> .
             * <p>
             * A <b> TemporalAccessor</b>  represents some form of date and time information.
             * This factory converts the arbitrary temporal object to an INSTANCE of <b> ZoneId</b> .
             * <p>
             * The conversion will try to obtain the zone in a way that favours region-based
             * zones over offset-based zones using <b style="color:orange;"> TemporalQueries#zone()</b> .
             * <p>
             * This method matches the signature of the functional interface <b style="color:orange;"> TemporalQuery</b> 
             * allowing it to be used as a query via method reference, <b> ZoneId::from</b> .
             *
             * @param temporal  the temporal object to convert
             * @return the zone ID
             * @throws DateTimeException if unable to convert to a <b> ZoneId</b> 
             */
            static const ZoneID &from(const Temporal &temporal);

        protected:
            //-----------------------------------------------------------------------
            /**
             * Constructor only accessible within the package.
             */
            CORE_FAST ZoneID() = default;

        public:
            //-----------------------------------------------------------------------
            /**
             * Gets the unique time-zone ID.
             * <p>
             * This ID uniquely defines this object.
             * The format of an offset based ID is defined by <b style="color:orange;"> ZoneOffset#getId()</b> .
             *
             * @return the time-zone unique ID
             */
            virtual String id() const = 0;

            //-----------------------------------------------------------------------
            /**
             * Gets the time-zone rules for this ID allowing calculations to be performed.
             * <p>
             * The rules provide the functionality associated with a time-zone,
             * such as finding the offset for a given instant or local date-time.
             * <p>
             * A time-zone can be invalid if it is deserialized in a Java Runtime which
             * does not have the same rules loaded as the Java Runtime that stored it.
             * In this case, calling this method will throw a <b> ZoneRulesException}.
             * <p>
             * The rules are supplied by <b style="color:orange;"> ZoneRulesProvider}. An advanced provider may
             * support dynamic updates to the rules without restarting the Java Runtime.
             * If so, then the result of this method may change over time.
             * Each individual call will be still remain thread-safe.
             * <p>
             * <b style="color:orange;"> ZoneOffset} will always return a setValue of rules where the offset never changes.
             *
             * @return the rules
             * @throws ZoneRulesException if no rules are available for this ID
             */
            virtual ZoneRules rules() const = 0;

            /**
             * Get the effective offset for an instant at the given epochSecond.
             */
            virtual ZoneOffset offset(glong epochSecond) const = 0;

            //-----------------------------------------------------------------------
            /**
             * Checks if this time-zone ID is equal to another time-zone ID.
             * <p>
             * The comparison is based on the ID.
             *
             * @param obj  the object to check, null returns false
             * @return true if this is equal to the other time-zone ID
             */
            gbool equals(const Object &obj) const override;

            /**
             * A hash code for this time-zone ID.
             *
             * @return a suitable hash code
             */
            gint hash() const override;

            /**
             * Outputs this zone as a <b> String}, using the ID.
             *
             * @return a string representation of this time-zone ID
             */
            String toString() const override;

            //-----------------------------------------------------------------------
            /**
             * Gets the textual representation of the zone, such as 'British Time' or
             * '+02:00'.
             * <p>
             * This returns the textual name used to identify the time-zone ID,
             * suitable for presentation to the user.
             * The parameters control the style of the returned text and the locale.
             * <p>
             * If no textual mapping is found then the <b style="color:orange;"> #getId() full ID</b>  is returned.
             *
             * @param style  the length of the text required
             * @param locale  the locale to use
             * @return the text value of the zone
             */
            virtual String displayName(DateTimeFormatter::TextStyle style, const util::Locale &locale) const;

        private:
            glong until(const Temporal &endExclusive, TemporalUnit unit) const override;

            gint get(TemporalField field) const override;

            glong getLong(TemporalField field) const override;
        };
    } // time
} // core

#endif //CORE23_ZONEID_H
