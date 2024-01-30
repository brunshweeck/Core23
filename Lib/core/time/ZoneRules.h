//
// Created by T.N.Brunshweeck on 25/12/2023.
//

#ifndef CORE23_ZONERULES_H
#define CORE23_ZONERULES_H


#include <core/native/LongArray.h>
#include <core/time/LocalDateTime.h>
#include <core/util/HashMap.h>
#include <core/native/Array.h>

namespace core {
    namespace time {

        /**
         * The rules defining how the zone offset varies for a single time-zone.
         * <p>
         * The rules model all the historic and future transitions for a time-zone.
         * <b style="color:orange;"> ZoneOffsetTransition</b>  is used for known transitions, typically historic.
         * <b style="color:orange;"> ZoneOffsetTransitionRule</b>  is used for future transitions that are based
         * on the result of an algorithm.
         * <p>
         * Serializing an INSTANCE of <b> ZoneRules</b>  will store the entire setValue of rules.
         * It does not store the zone ID as it is not part of the state of this object.
         * <p>
         * A rule implementation may or may not store full information about historic
         * and future transitions, and the information stored is only as accurate as
         * that supplied to the implementation by the rules provider.
         * Applications should treat the data provided as representing the best information
         * available to the implementation of this rule.
         *
         * @implSpec
         * This class is immutable and thread-safe.
         *
         */
        class ZoneRules CORE_FINAL : public Object {
        private:
            /**
             * The last year to have its transitions cached.
             */
            static CORE_FAST gint LAST_YEAR_CACHE = 2100;

            /**
             * The transitions between standard offsets (epoch seconds), sorted.
             */
            LongArray standardTransitions;

            /**
             * The standard offsets.
             */
            native::ObjectArray standardOffsets;
            /**
             * The transitions between instants (epoch seconds), sorted.
             */
            LongArray savingsInstantTransitions;

            /**
             * The transitions between local date-times, sorted.
             * This is a paired root, where the first entry is the start of the transition
             * and the second entry is the end of the transition.
             */
            Array<LocalDateTime> savingsLocalTransitions;
            /**
             * The wall offsets.
             */
            native::ObjectArray wallOffsets;
            /**
             * The last rule.
             */
            native::ObjectArray lastRules;
            /**
             * The map of recent transitions.
             */
            util::HashMap<Integer, native::ObjectArray> lastRulesCache;
            /**
             * The number of days in a 400 year cycle.
             */
            static CORE_FAST gint DAYS_PER_CYCLE = 146097;
            /**
             * The number of days from year zero to year 1970.
             * There are five 400 year cycles from year zero to 2000.
             * There are 7 leap years from 1970 to 2000.
             */
            static CORE_FAST gint DAYS_0000_TO_1970 = (DAYS_PER_CYCLE * 5L) - (30L * 365L + 7L);

        public:
            /**
             * Obtains an INSTANCE of a ZoneRules.
             *
             * @param baseStandardOffset  the standard offset to use before legal rules were setValue
             * @param baseWallOffset  the wall offset to use before legal rules were setValue
             * @param standardOffsetTransitionList  the list of changes to the standard offset
             * @param transitionList  the list of transitions
             * @param lastRules  the recurring last rules, size 16 or less
             * @return the zone rules
             */
            static ZoneRules of(const ZoneOffset &baseStandardOffset,
                                const ZoneOffset &baseWallOffset,
                                const util::List<ZoneOffsetTransition> &standardOffsetTransitionList,
                                const util::List<ZoneOffsetTransition> &transitionList,
                                const util::List<ZoneOffsetTransitionRule> &lastRules);

            /**
             * Obtains an INSTANCE of ZoneRules that has fixed zone rules.
             *
             * @param offset  the offset this fixed zone rules is based on
             * @return the zone rules
             * @see isFixedOffset()
             */
            static ZoneRules of(const ZoneOffset &offset);

            ZoneRules(const ZoneRules &rules);

            ZoneRules(ZoneRules &&rules) CORE_NOTHROW;

            ZoneRules &operator=(const ZoneRules &rules);

            ZoneRules &operator=(ZoneRules &&rules) CORE_NOTHROW;

            ~ZoneRules() override;

        private:
            /**
             * Creates an INSTANCE.
             *
             * @param baseStandardOffset  the standard offset to use before legal rules were setValue
             * @param baseWallOffset  the wall offset to use before legal rules were setValue
             * @param standardOffsetTransitionList  the list of changes to the standard offset
             * @param transitionList  the list of transitions
             * @param lastRules  the recurring last rules, size 16 or less
             */
            CORE_EXPLICIT ZoneRules(const ZoneOffset &baseStandardOffset,
                                    const ZoneOffset &baseWallOffset,
                                    const util::List<ZoneOffsetTransition> &standardOffsetTransitionList,
                                    const util::List<ZoneOffsetTransition> &transitionList,
                                    const util::List<ZoneOffsetTransitionRule> &lastRules);

            /**
             * Constructor.
             *
             * @param standardTransitions  the standard transitions
             * @param standardOffsets  the standard offsets
             * @param savingsInstantTransitions  the standard transitions
             * @param wallOffsets  the wall offsets
             * @param lastRules  the recurring last rules, size 15 or less
             */
            CORE_EXPLICIT ZoneRules(const LongArray &standardTransitions,
                                    const Array<ZoneOffset> &standardOffsets,
                                    const LongArray &savingsInstantTransitions,
                                    const Array<ZoneOffset> &wallOffsets,
                                    const Array<ZoneOffsetTransitionRule> &lastRules);

            /**
             * Creates an INSTANCE of ZoneRules that has fixed zone rules.
             *
             * @param offset  the offset this fixed zone rules is based on
             * @see #isFixedOffset()
             */
            CORE_EXPLICIT ZoneRules(const ZoneOffset &offset);

            CORE_FRATERNITY(ZoneID);
            CORE_FRATERNITY(ZoneRegion);
            CORE_FRATERNITY(ZoneOffset);

        public:
            /**
             * Checks of the zone rules are fixed, such that the offset never varies.
             *
             * @return true if the time-zone is fixed and the offset never changes
             */
            gbool isFixedOffset() const;

            /**
             * Gets the offset applicable at the specified instant in these rules.
             * <p>
             * The mapping from an instant to an offset is simple, there is only
             * one valid offset for each instant.
             * This method returns that offset.
             *
             * @param epochSecond  the instant to find the offset fo, but null
             *  may be ignored if the rules have a single offset for all instants
             * @return the offset
             */
            ZoneOffset offset(glong epochSecond) const;

            /**
             * Gets a suitable offset for the specified local date-time in these rules.
             * <p>
             * The mapping from a local date-time to an offset is not straightforward.
             * There are three cases:
             * <ul>
             * <li>Normal, with one valid offset. For the vast majority of the year, the normal
             *  case applies, where there is a single valid offset for the local date-time.</li>
             * <li>Gap, with zero valid offsets. This is when clocks jump forward typically
             *  due to the spring daylight savings change from "winter" to "summer".
             *  In a gap there are local date-time values with no valid offset.</li>
             * <li>Overlap, with two valid offsets. This is when clocks are setValue back typically
             *  due to the autumn daylight savings change from "summer" to "winter".
             *  In an overlap there are local date-time values with two valid offsets.</li>
             * </ul>
             * Thus, for any given local date-time there can be zero, one or two valid offsets.
             * This method returns the single offset in the Normal case, and in the Gap or Overlap
             * case it returns the offset before the transition.
             * <p>
             * Since, in the case of Gap and Overlap, the offset returned is a "best" value, rather
             * than the "correct" value, it should be treated with care. Applications that care
             * about the correct offset should use a combination of this method,
             * <b style="color:orange;"> validOffsets(LocalDateTime)</b>  and <b style="color:orange;"> transition(LocalDateTime)</b> .
             *
             * @param dateTime  the local date-time to query, but null
             *  may be ignored if the rules have a single offset for all instants
             * @return the best available offset for the local date-time
             */
            ZoneOffset offset(const LocalDateTime &dateTime) const;

            /**
             * Gets the offset applicable at the specified local date-time in these rules.
             * <p>
             * The mapping from a local date-time to an offset is not straightforward.
             * There are three cases:
             * <ul>
             * <li>Normal, with one valid offset. For the vast majority of the year, the normal
             *  case applies, where there is a single valid offset for the local date-time.</li>
             * <li>Gap, with zero valid offsets. This is when clocks jump forward typically
             *  due to the spring daylight savings change from "winter" to "summer".
             *  In a gap there are local date-time values with no valid offset.</li>
             * <li>Overlap, with two valid offsets. This is when clocks are setValue back typically
             *  due to the autumn daylight savings change from "summer" to "winter".
             *  In an overlap there are local date-time values with two valid offsets.</li>
             * </ul>
             * Thus, for any given local date-time there can be zero, one or two valid offsets.
             * This method returns that list of valid offsets, which is a list of size 0, 1 or 2.
             * In the case where there are two offsets, the earlier offset is returned at index 0
             * and the later offset at index 1.
             * <p>
             * There are various ways to handle the conversion from a <b> LocalDateTime</b> .
             * One technique, using this method, would be:
             * <pre> @code
             *  ArrayList&lt;ZoneOffset&gt; validOffsets = rules.validOffsets(localDT);
             *  if (validOffsets.size() == 1) {
             *    // Normal case: only one valid offset
             *    zoneOffset = validOffsets.get(0);
             *  }  else {
             *    // Gap or Overlap: determine what to do from transition (which will be non-null)
             *    ZoneOffsetTransition trans = rules.transition(localDT);
             *  }
             * @endcode </pre>
             * <p>
             * In theory, it is possible for there to be more than two valid offsets.
             * This would happen if clocks to be put back more than once in quick succession.
             * This has never happened in the history of time-zones and thus has no special handling.
             * However, if it were to happen, then the list would return more than 2 entries.
             *
             * @param dateTime  the local date-time to query for valid offsets, but null
             *  may be ignored if the rules have a single offset for all instants
             * @return the list of valid offsets, may be immutable
             */
            Array<ZoneOffset> validOffsets(const LocalDateTime &dateTime) const;

            /**
             * Gets the offset transition applicable at the specified local date-time in these rules.
             * <p>
             * The mapping from a local date-time to an offset is not straightforward.
             * There are three cases:
             * <ul>
             * <li>Normal, with one valid offset. For the vast majority of the year, the normal
             *  case applies, where there is a single valid offset for the local date-time.</li>
             * <li>Gap, with zero valid offsets. This is when clocks jump forward typically
             *  due to the spring daylight savings change from "winter" to "summer".
             *  In a gap there are local date-time values with no valid offset.</li>
             * <li>Overlap, with two valid offsets. This is when clocks are setValue back typically
             *  due to the autumn daylight savings change from "summer" to "winter".
             *  In an overlap there are local date-time values with two valid offsets.</li>
             * </ul>
             * A transition is used to model the cases of a Gap or Overlap.
             * The Normal case will return null.
             * <p>
             * There are various ways to handle the conversion from a <b> LocalDateTime</b> .
             * One technique, using this method, would be:
             * <pre> @code
             *  ZoneOffsetTransition trans = rules.transition(localDT);
             *  if (trans != null) {
             *    // Gap or Overlap: determine what to do from transition
             *  }   else {
             *    // Normal case: only one valid offset
             *    zoneOffset = rule.offset(localDT);
             *  }
             * @endcode </pre>
             *
             * @param dateTime  the local date-time to query for offset transition, but null
             *  may be ignored if the rules have a single offset for all instants
             * @return the offset transition, null if the local date-time is not in transition
             */
            ZoneOffsetTransition transition(const LocalDateTime &dateTime) const;

            /**
             * Gets the standard offset for the specified instant in this zone.
             * <p>
             * This provides access to historic information on how the standard offset
             * has changed over time.
             * The standard offset is the offset before any daylight saving time is applied.
             * This is typically the offset applicable during winter.
             *
             * @param epochSecond  the instant to find the offset information for, but null
             *  may be ignored if the rules have a single offset for all instants
             * @return the standard offset
             */
            ZoneOffset standardOffset(glong epochSecond) const;

            /**
             * Gets the amount of daylight savings in use for the specified instant in this zone.
             * <p>
             * This provides access to historic information on how the amount of daylight
             * savings has changed over time.
             * This is the difference between the standard offset and the actual offset.
             * Typically the amount is zero during winter and one hour during summer.
             * Time-zones are second-based, so the nanosecond part of the duration will be zero.
             * <p>
             * This default implementation calculates the duration from the
             * <b style="color:orange;"> actual</b>  and
             * <b style="color:orange;"> standard</b>  offsets.
             *
             * @param epochSecond  the instant to find the daylight savings for, but null
             *  may be ignored if the rules have a single offset for all instants
             * @return the difference between the standard and actual offset
             */
            gint daylightSavings(glong epochSecond) const;

            /**
             * Checks if the specified instant is in daylight savings.
             * <p>
             * This checks if the standard offset and the actual offset are the same
             * for the specified instant.
             * If they are not, it is assumed that daylight savings is in operation.
             * <p>
             * This default implementation compares the <b style="color:orange;"> actual</b>
             * and <b style="color:orange;"> standard</b>  offsets.
             *
             * @param epochSecond  the instant to find the offset information for, but null
             *  may be ignored if the rules have a single offset for all instants
             * @return the standard offset
             */
            gbool isDaylightSavings(glong epochSecond) const;

            /**
             * Checks if the offset date-time is valid for these rules.
             * <p>
             * To be valid, the local date-time must not be in a gap and the offset
             * must match one of the valid offsets.
             * <p>
             * This default implementation checks if <b style="color:orange;"> validOffsets(LocalDateTime)</b>
             * contains the specified offset.
             *
             * @param dateTime  the date-time to check, but null
             *  may be ignored if the rules have a single offset for all instants
             * @param offset  the offset to check, null returns false
             * @return true if the offset date-time is valid for these rules
             */
            gbool isValidOffset(const LocalDateTime &dateTime, const ZoneOffset &offset) const;

            /**
             * Gets the next transition after the specified instant.
             * <p>
             * This returns details of the next transition after the specified instant.
             * For example, if the instant represents a point where "Summer" daylight savings time
             * applies, then the method will return the transition to the next "Winter" time.
             *
             * @param epochSecond  the instant to get the next transition after, but null
             *  may be ignored if the rules have a single offset for all instants
             * @return the next transition after the specified instant, null if this is after the last transition
             */
            ZoneOffsetTransition nextTransition(glong epochSecond) const;

            /**
             * Gets the previous transition before the specified instant.
             * <p>
             * This returns details of the previous transition before the specified instant.
             * For example, if the instant represents a point where "summer" daylight saving time
             * applies, then the method will return the transition from the previous "winter" time.
             *
             * @param epochSecond  the instant to get the previous transition after, but null
             *  may be ignored if the rules have a single offset for all instants
             * @return the previous transition before the specified instant, null if this is before the first transition
             */
            ZoneOffsetTransition previousTransition(glong epochSecond) const;

            /**
             * Gets the complete list of fully defined transitions.
             * <p>
             * The complete setValue of transitions for this rules INSTANCE is defined by this method
             * and <b style="color:orange;"> transitionRules()</b> . This method returns those transitions that have
             * been fully defined. These are typically historical, but may be in the future.
             * <p>
             * The list will be empty for fixed offset rules and for any time-zone where there has
             * only ever been a single offset. The list will also be empty if the transition rules are unknown.
             *
             * @return an immutable list of fully defined transitions
             */
            Array<ZoneOffsetTransition> transitions() const;

            /**
             * Gets the list of transition rules for years beyond those defined in the transition list.
             * <p>
             * The complete setValue of transitions for this rules INSTANCE is defined by this method
             * and <b style="color:orange;"> transitions()</b> . This method returns instances of <b style="color:orange;"> ZoneOffsetTransitionRule</b>
             * that define an algorithm for when transitions will occur.
             * <p>
             * For any given <b> ZoneRules</b> , this list contains the transition rules for years
             * beyond those years that have been fully defined. These rules typically refer to future
             * daylight saving time rule changes.
             * <p>
             * If the zone defines daylight savings into the future, then the list will normally
             * be of size two and hold information about entering and exiting daylight savings.
             * If the zone does not have daylight savings, or information about future changes
             * is uncertain, then the list will be empty.
             * <p>
             * The list will be empty for fixed offset rules and for any time-zone where there is no
             * daylight saving time. The list will also be empty if the transition rules are unknown.
             *
             * @return an immutable list of transition rules
             */
            Array<ZoneOffsetTransitionRule> transitionRules() const;

            /**
             * Checks if this setValue of rules equals another.
             * <p>
             * Two rule sets are equal if they will always result in the same output
             * for any given input instant or local date-time.
             * Rules from two different groups may return false even if they are in fact the same.
             * <p>
             * This definition should result in implementations comparing their entire state.
             *
             * @param otherRules  the other rules, null returns false
             * @return true if this rules is the same as that specified
             */
            gbool equals(const Object &otherRules) const override;

            /**
             * Returns a suitable hash code given the definition of <b> #equals</b> .
             *
             * @return the hash code
             */
            gint hash() const override;

            /**
             * Returns a string describing this object.
             *
             * @return a string for debugging
             */
            String toString() const override;

            Object &clone() const override;

        private:
            static gint findYear(glong epochSecond, const ZoneOffset &offset);

            /**
             * Finds the appropriate transition root for the given year.
             *
             * @param year  the year
             * @return the transition root
             */
            Array<ZoneOffsetTransition> transitionArray(gint year) const;

            Object &offsetInfo(const LocalDateTime &dateTime) const;

            static Object &offsetInfo(const LocalDateTime &dateTime, const ZoneOffsetTransition &transition);
        };
    } // time
} // core

#endif //CORE23_ZONERULES_H
