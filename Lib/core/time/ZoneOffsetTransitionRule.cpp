//
// Created by T.N.Brunshweeck on 26/12/2023.
//

#include "ZoneOffsetTransitionRule.h"

namespace core {
    namespace time {

        namespace {


            gint numberOfDaysPerMonth(gint month, gbool leapYear) {
                switch ((LocalDate::Month) month) {
                    case LocalDate::JANUARY:
                    case LocalDate::MARCH:
                    case LocalDate::MAY:
                    case LocalDate::JULY:
                    case LocalDate::AUGUST:
                    case LocalDate::OCTOBER:
                    case LocalDate::DECEMBER:
                        return 31;
                    case LocalDate::FEBRUARY:
                        return leapYear ? 29 : 28;
                    case LocalDate::APRIL:
                    case LocalDate::JUNE:
                    case LocalDate::SEPTEMBER:
                    case LocalDate::NOVEMBER:
                    default:
                        return 30;
                }
            }


            gbool isISOLeapYear(gint prolepticYear) {
                return ((prolepticYear & 3) == 0) && ((prolepticYear % 100) != 0 || (prolepticYear % 400) == 0);
            }



            const String MonthName[13] = {
                    "",
                    "JANUARY",
                    "FEBRUARY",
                    "MARCH",
                    "APRIL",
                    "MAY",
                    "JUNE",
                    "JULY",
                    "AUGUST",
                    "SEPTEMBER",
                    "OCTOBER",
                    "NOVEMBER",
                    "DECEMBER",
            };

            const String DaysName[8] = {
                    "",
                    "MONDAY",
                    "TUESDAY",
                    "WEDNESDAY",
                    "THURSDAY",
                    "FRIDAY",
                    "SATURDAY",
                    "SUNDAY",
            };
        }

        LocalDateTime ZoneOffsetTransitionRule::createDateTime(
                ZoneOffsetTransitionRule::TimeDefinition td,
                const LocalDateTime &dateTime,
                const ZoneOffset &standardOffset,
                const ZoneOffset &wallOffset) {
            switch (td) {
                case UTC: {
                    int const difference = wallOffset.totalSeconds() - ZoneOffset::UTC.totalSeconds();
                    return dateTime.afterSeconds(difference);
                }
                case STANDARD: {
                    int const difference = wallOffset.totalSeconds() - standardOffset.totalSeconds();
                    return dateTime.afterSeconds(difference);
                }
                default:  // WALL
                    return dateTime;
            }
        }

        ZoneOffsetTransitionRule::ZoneOffsetTransitionRule(
                LocalDate::Month month, int dayOfMonthIndicator,
                LocalDate::DayOfWeek dayOfWeek, const LocalTime &time,
                gbool timeEndOfDay,
                ZoneOffsetTransitionRule::TimeDefinition timeDefinition,
                const ZoneOffset &standardOffset,
                const ZoneOffset &offsetBefore,
                const ZoneOffset &offsetAfter) :
                m(month), timeDef(timeDefinition), timeEndOfDay(timeEndOfDay), time(time),
                dow(dayOfWeek), dom(dayOfMonthIndicator), standard(standardOffset), after(offsetAfter),
                before(offsetBefore) {}

        ZoneOffsetTransitionRule
        ZoneOffsetTransitionRule::of(LocalDate::Month month, int dayOfMonthIndicator, LocalDate::DayOfWeek dayOfWeek,
                                     const LocalTime &time, gbool timeEndOfDay,
                                     ZoneOffsetTransitionRule::TimeDefinition timeDefinition,
                                     const ZoneOffset &standardOffset, const ZoneOffset &offsetBefore,
                                     const ZoneOffset &offsetAfter) {
            if (dayOfMonthIndicator < -28 || dayOfMonthIndicator > 31 || dayOfMonthIndicator == 0) {
                ArgumentException("Day of m indicator must be between -28 and 31 inclusive excluding zero")
                        .throws(__trace("core.time.ZoneOffsetTransitionRule"));
            }
            if (timeEndOfDay && !time.equals(LocalTime::MIDNIGHT)) {
                ArgumentException("Time must be midnight when end of day flag is true")
                        .throws(__trace("core.time.ZoneOffsetTransitionRule"));
            }
            if (time.nano() != 0) {
                ArgumentException("Time's nano-of-second must be zero")
                        .throws(__trace("core.time.ZoneOffsetTransitionRule"));
            }
            return ZoneOffsetTransitionRule(month, dayOfMonthIndicator, dayOfWeek, time, timeEndOfDay, timeDefinition,
                                            standardOffset, offsetBefore, offsetAfter);
        }

        LocalDate::Month ZoneOffsetTransitionRule::month() const {
            return m;
        }

        gint ZoneOffsetTransitionRule::dayOfMonthIndicator() const {
            return dom;
        }

        gint ZoneOffsetTransitionRule::dayOfWeek() const {
            return dow;
        }

        LocalTime ZoneOffsetTransitionRule::localTime() const {
            return time;
        }

        gbool ZoneOffsetTransitionRule::isMidnightEndOfDay() const {
            return timeEndOfDay;
        }

        ZoneOffsetTransitionRule::TimeDefinition ZoneOffsetTransitionRule::timeDefinition() const {
            return timeDef;
        }

        ZoneOffset ZoneOffsetTransitionRule::standardOffset() const {
            return standard;
        }

        ZoneOffset ZoneOffsetTransitionRule::offsetBefore() const {
            return before;
        }

        ZoneOffset ZoneOffsetTransitionRule::offsetAfter() const {
            return after;
        }

        ZoneOffsetTransition ZoneOffsetTransitionRule::createTransition(gint year) const {
            try {
                LocalDate date = LocalDate(year, 1, 1);
                if (dom < 0) {
                    date = LocalDate(year, m, numberOfDaysPerMonth(m, isISOLeapYear(year)) + 1 + dom);
                    if (date.dayOfWeek() != dow) {
                        gint const daysDiff = dow - date.dayOfWeek();
                        date = date.beforeDays(daysDiff >= 0 ? 7 - daysDiff : -daysDiff);
                    }
                } else {
                    date = LocalDate(year, m, dom);
                    if (date.dayOfWeek() != dow) {
                        gint const daysDiff = date.dayOfWeek() - dow;
                        date = date.beforeDays(daysDiff >= 0 ? 7 - daysDiff : -daysDiff);
                    }
                }
                if (timeEndOfDay) {
                    date = date.afterDays(1);
                }
                LocalDateTime const localDT = LocalDateTime(date, time);
                LocalDateTime const transition = createDateTime(timeDef, localDT, standard, before);
                return ZoneOffsetTransition(transition, before, after);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.ZoneOffsetTransitionRule")); }
        }

        gbool ZoneOffsetTransitionRule::equals(const Object &otherRule) const {
            if (&otherRule == this) {
                return true;
            }
            if (!Class<ZoneOffsetTransitionRule>::hasInstance(otherRule))
                return false;
            ZoneOffsetTransitionRule const &other = (ZoneOffsetTransitionRule &) otherRule;
            return m == other.m
                   && dom == other.dom
                   && dow == other.dow
                   && timeDef == other.timeDef
                   && timeEndOfDay == other.timeEndOfDay
                   && time.equals(other.time)
                   && standard.equals(other.standard)
                   && before.equals(other.before)
                   && after.equals(other.after);
        }

        gint ZoneOffsetTransitionRule::hash() const {
            int const hash = ((time.toSecondOfDay() + (timeEndOfDay ? 1 : 0)) << 15) +
                             (m << 11) + ((dom + 32) << 5) +
                             (dow << 2) + (timeDef);
            return hash ^ standard.hash() ^ before.hash() ^ after.hash();
        }

        String ZoneOffsetTransitionRule::toString() const {
            StringBuffer buf = {};
            buf.append("TransitionRule[")
                    .append(before.compareTo(after) > 0 ? "Gap " : "Overlap ")
                    .append(before).append(" to ").append(after).append(", ");
            if (dom == -1) {
                buf.append(DaysName[dow]).append(" on or before last day of ").append(MonthName[m]);
            } else if (dom < 0) {
                buf.append(DaysName[dow]).append(" on or before last day minus ").append(-dom - 1).append(" of ").append(MonthName[m]);
            } else {
                buf.append(DaysName[dow]).append(" on or after ").append(MonthName[m]).append(' ').append(dom);
            }
            buf.append(" at ").append(timeEndOfDay ? "24:00" : time.toString())
                    .append(" ").append(timeDef == STANDARD ? "STANDARD" : timeDef == UTC? "UTC": "WALL")
                    .append(", standard offset ").append(standard)
                    .append(']');
            return buf.toString();
        }

        Object &ZoneOffsetTransitionRule::clone() const {
            return  native::Unsafe::createInstance<ZoneOffsetTransitionRule>(*this);
        }
    } // time
} // core