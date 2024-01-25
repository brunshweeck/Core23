//
// Created by T.N.Brunshweeck on 26/12/2023.
//

#ifndef CORE23_ZONEOFFSETTRANSITIONRULE_H
#define CORE23_ZONEOFFSETTRANSITIONRULE_H

#include <core/time/ZoneOffsetTransition.h>
#include <core/time/LocalDate.h>
#include <core/time/LocalTime.h>

namespace core {
    namespace time {

        class ZoneOffsetTransitionRule CORE_FINAL : public Object {
        public:

            //-----------------------------------------------------------------------
            /**
             * A definition of the way a local time can be converted to the actual
             * transition date-time.
             * <p>
             * Time zone rules are expressed in one of three ways:
             * <ul>
             * <li>Relative to UTC</li>
             * <li>Relative to the standard offset in force</li>
             * <li>Relative to the wall offset (what you would see on a clock on the wall)</li>
             * </ul>
             */
            enum TimeDefinition : gbyte {
                /** The local date-time is expressed in terms of the UTC offset. */
                UTC,

                /** The local date-time is expressed in terms of the wall offset. */
                WALL,

                /** The local date-time is expressed in terms of the standard offset. */
                STANDARD,

            };

        private:
            /**
             * The m of the m-day of the first day of the cut-over week.
             * The actual date will be adjusted by the dowChange field.
             */
            LocalDate::Month m;
            /**
             * The day-of-m of the m-day of the cut-over week.
             * If positive, it is the start of the week where the cut-over can occur.
             * If negative, it represents the end of the week where cut-over can occur.
             * The value is the number of days from the end of the m, such that
             * <b> -1} is the last day of the m, <b> -2} is the second
             * to last day, and so on.
             */
            gint dom: 8;

            /**
             * The cut-over day-of-week, null to retain the day-of-m.
             */
            LocalDate::DayOfWeek dow;
            /**
             * The cut-over time in the 'before' offset.
             */
            LocalTime time;
            /**
             * Whether the cut-over time is midnight at the end of day.
             */
            gbool timeEndOfDay;
            /**
             * The definition of how the local time should be interpreted.
             */
            TimeDefinition timeDef;

            /**
             * The standard offset at the cut-over.
             */
            ZoneOffset standard;

            /**
             * The offset before the cut-over.
             */
            ZoneOffset before;
            /**
             * The offset after the cut-over.
             */
            ZoneOffset after;

            /**
             * Converts the specified local date-time to the local date-time actually
             * seen on a wall clock.
             * <p>
             * This method converts using the type of this enum.
             * The output is defined relative to the 'before' offset of the transition.
             * <p>
             * The UTC type uses the UTC offset.
             * The STANDARD type uses the standard offset.
             * The WALL type returns the input date-time.
             * The result is intended for use with the wall-offset.
             *
             * @param dateTime  the local date-time
             * @param standardOffset  the standard offset
             * @param wallOffset  the wall offset
             * @return the date-time relative to the wall/before offset
             */
            static LocalDateTime createDateTime(TimeDefinition td, const LocalDateTime &dateTime,
                                                const ZoneOffset &standardOffset, const ZoneOffset &wallOffset);

        public:

            /**
             * Obtains an INSTANCE defining the yearly rule to create transitions between two offsets.
             * <p>
             * Applications should normally obtain an INSTANCE from <b style="color:orange;"> ZoneRules}.
             * This factory is only intended for use when creating <b style="color:orange;"> ZoneRules}.
             *
             * @param month  the m of the m-day of the first day of the cut-over week
             * @param dayOfMonthIndicator  the day of the m-day of the cut-over week, positive if the week is that
             *  day or later, negative if the week is that day or earlier, counting from the last day of the m,
             *  from -28 to 31 excluding 0
             * @param dayOfWeek  the required day-of-week, null if the m-day should not be changed
             * @param time  the cut-over time in the 'before' offset
             * @param timeEndOfDay  whether the time is midnight at the end of day
             * @param timeDefinition  how to interpret the cut-over
             * @param standardOffset  the standard offset in force at the cut-over
             * @param offsetBefore  the offset before the cut-over
             * @param offsetAfter  the offset after the cut-over
             * @return the rule
             * @throws IllegalArgumentException if the day of m indicator is invalid
             * @throws IllegalArgumentException if the end of day flag is true when the time is not midnight
             * @throws IllegalArgumentException if <b> time.getNano()} returns non-zero value
             */
            static ZoneOffsetTransitionRule of(
                    LocalDate::Month month,
                    gint dayOfMonthIndicator,
                    LocalDate::DayOfWeek dayOfWeek,
                    const LocalTime &time,
                    gbool timeEndOfDay,
                    TimeDefinition timeDefinition,
                    const ZoneOffset &standardOffset,
                    const ZoneOffset &offsetBefore,
                    const ZoneOffset &offsetAfter);

        private:

            /**
             * Creates an INSTANCE defining the yearly rule to create transitions between two offsets.
             *
             * @param month  the m of the m-day of the first day of the cut-over week
             * @param dayOfMonthIndicator  the day of the m-day of the cut-over week, positive if the week is that
             *  day or later, negative if the week is that day or earlier, counting from the last day of the m,
             *  from -28 to 31 excluding 0
             * @param dayOfWeek  the required day-of-week, null if the m-day should not be changed
             * @param time  the cut-over time in the 'before' offset
             * @param timeEndOfDay  whether the time is midnight at the end of day
             * @param timeDefinition  how to interpret the cut-over
             * @param standardOffset  the standard offset in force at the cut-over
             * @param offsetBefore  the offset before the cut-over
             * @param offsetAfter  the offset after the cut-over
             * @throws IllegalArgumentException if the day of m indicator is invalid
             * @throws IllegalArgumentException if the end of day flag is true when the time is not midnight
             */
            CORE_EXPLICIT ZoneOffsetTransitionRule(
                    LocalDate::Month month,
                    gint dayOfMonthIndicator,
                    LocalDate::DayOfWeek dayOfWeek,
                    const LocalTime &time,
                    gbool timeEndOfDay,
                    TimeDefinition timeDefinition,
                    const ZoneOffset &standardOffset,
                    const ZoneOffset &offsetBefore,
                    const ZoneOffset &offsetAfter);

        public:

            //-----------------------------------------------------------------------
            /**
             * Gets the m of the transition.
             * <p>
             * If the rule defines an exact date then the m is the m of that date.
             * <p>
             * If the rule defines a week where the transition might occur, then the m
             * if the m of either the earliest or latest possible date of the cut-over.
             *
             * @return the m of the transition
             */
             LocalDate::Month month() const;

            /**
             * Gets the indicator of the day-of-month of the transition.
             * <p>
             * If the rule defines an exact date then the day is the month of that date.
             * <p>
             * If the rule defines a week where the transition might occur, then the day
             * defines either the start of the end of the transition week.
             * <p>
             * If the value is positive, then it represents a normal day-of-month, and is the
             * earliest possible date that the transition can be.
             * The date may refer to 29th February which should be treated as 1st March in non-leap years.
             * <p>
             * If the value is negative, then it represents the number of days back from the
             * end of the month where <b> -1} is the last day of the month.
             * In this case, the day identified is the latest possible date that the transition can be.
             *
             * @return the day-of-month indicator, from -28 to 31 excluding 0
             */
             gint dayOfMonthIndicator() const;

            /**
             * Gets the day-of-week of the transition.
             * <p>
             * If the rule defines an exact date then this returns null.
             * <p>
             * If the rule defines a week where the cut-over might occur, then this method
             * returns the day-of-week that the month-day will be adjusted to.
             * If the day is positive then the adjustment is later.
             * If the day is negative then the adjustment is earlier.
             *
             * @return the day-of-week that the transition occurs, null if the rule defines an exact date
             */
             gint dayOfWeek() const;

            /**
             * Gets the local time of day of the transition which must be checked with
             * <b style="color:orange;"> #isMidnightEndOfDay()}.
             * <p>
             * The time is converted into an instant using the time definition.
             *
             * @return the local time of day of the transition
             */
             LocalTime localTime() const;

            /**
             * Is the transition local time midnight at the end of day.
             * <p>
             * The transition may be represented as occurring at 24:00.
             *
             * @return whether a local time of midnight is at the start or end of the day
             */
             gbool isMidnightEndOfDay() const;

            /**
             * Gets the time definition, specifying how to convert the time to an instant.
             * <p>
             * The local time can be converted to an instant using the standard offset,
             * the wall offset or UTC.
             *
             * @return the time definition
             */
             TimeDefinition timeDefinition() const;

            /**
             * Gets the standard offset in force at the transition.
             *
             * @return the standard offset
             */
             ZoneOffset standardOffset() const;

            /**
             * Gets the offset before the transition.
             *
             * @return the offset before
             */
             ZoneOffset offsetBefore() const;

            /**
             * Gets the offset after the transition.
             *
             * @return the offset after
             */
             ZoneOffset offsetAfter() const;

            //-----------------------------------------------------------------------
            /**
             * Creates a transition INSTANCE for the specified year.
             * <p>
             * Calculations are performed using the ISO-8601 chronology.
             *
             * @param year  the year to create a transition for
             * @return the transition INSTANCE
             */
             ZoneOffsetTransition createTransition(gint year) const;

            //-----------------------------------------------------------------------
            /**
             * Checks if this object equals another.
             * <p>
             * The entire state of the object is compared.
             *
             * @param otherRule  the other object to compare to, null returns false
             * @return true if equal
             */
             gbool equals(const Object& otherRule) const override;

            /**
             * Returns a suitable hash code.
             *
             * @return the hash code
             */
             gint hash() const override;

            //-----------------------------------------------------------------------
            /**
             * Returns a string describing this object.
             *
             * @return a string for debugging
             */
             String toString() const override;

            Object &clone() const override;
        };

    } // time
} // core

#endif //CORE23_ZONEOFFSETTRANSITIONRULE_H
