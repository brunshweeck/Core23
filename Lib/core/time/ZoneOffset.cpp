//
// Created by T.N.Brunshweeck on 25/12/2023.
//

#include "ZoneOffset.h"
#include <core/util/HashMap.h>

namespace core {
    namespace time {
        using util::HashMap;

        namespace {
            String const FieldName[30] = {
                    "NANO_OF_SECOND",
                    "NANO_OF_DAY",
                    "MICRO_OF_SECOND",
                    "MICRO_OF_DAY",
                    "MILLI_OF_SECOND",
                    "MILLI_OF_DAY",
                    "SECOND_OF_MINUTE",
                    "SECOND_DAY",
                    "MINUTE_OF_HOUR",
                    "MINUTE_OF_DAY",
                    "HOUR_OF_AMPM",
                    "CLOCK_HOUR_OF_AMPM",
                    "HOUR_OF_DAY",
                    "CLOCK_HOUR_OF_DAY",
                    "AMPM_OF_DAY",
                    "DAY_OF_WEEK",
                    "ALIGNED_DAY_OF_WEEK_IN_MONTH",
                    "ALIGNED_DAY_OF_WEEK_IN_YEAR",
                    "DAY_OF_MONTH",
                    "DAY_OF_YEAR",
                    "EPOCH_DAY",
                    "ALIGNED_WEEK_OF_MONTH",
                    "ALIGNED_WEEK_OF_YEAR",
                    "MONTH_OF_YEAR",
                    "PROLEPTIC_MONTH",
                    "YEAR_OF_ERA",
                    "YEAR",
                    "ERA",
                    "INSTANT_SECOND",
                    "OFFSET_SECOND",
            };
        }

        HashMap<Integer, ZoneOffset> ZoneOffset::SECONDS_CACHE = HashMap<Integer, ZoneOffset>(16, 0.75);
        HashMap<String, ZoneOffset> ZoneOffset::ID_CACHE = HashMap<String, ZoneOffset>(16, 0.75);

        const ZoneOffset ZoneOffset::UTC{0};
        const ZoneOffset ZoneOffset::MIN{-MAX_SECONDS};
        const ZoneOffset ZoneOffset::MAX{+MAX_SECONDS};
        const ZoneOffset ZoneOffset::INVALID{Integer::MAX_VALUE};


        ZoneOffset ZoneOffset::of(const String &offsetId) {
            const ZoneOffset &offset = ID_CACHE.getOrDefault(offsetId, INVALID);
            if (&offset != &INVALID) {
                return offset;
            }
            // parse - +h, +hh, +hhmm, +hh:mm, +hhmmss, +hh:mm:ss
            gint hours = 0;
            gint minutes = 0;
            gint seconds = 0;
            String id = offsetId;
            switch (id.length()) {
                case 2:
                    id = String::valueOf(id.charAt(0)) + "0"
                         + String::valueOf(id.charAt(1)); // fall-thr
                case 3:
                    try {
                        hours = parseNumber(id, 1, false);
                        minutes = 0;
                        seconds = 0;
                    } catch (const Exception &ex) { ex.throws(__trace("core.time.ZoneOffset")); }
                    break;
                case 5:
                    try {
                        hours = parseNumber(id, 1, false);
                        minutes = parseNumber(id, 3, false);
                        seconds = 0;
                    } catch (const Exception &ex) { ex.throws(__trace("core.time.ZoneOffset")); }
                    break;
                case 6:
                    try {
                        hours = parseNumber(id, 1, false);
                        minutes = parseNumber(id, 4, true);
                        seconds = 0;
                    } catch (const Exception &ex) { ex.throws(__trace("core.time.ZoneOffset")); }
                    break;
                case 7:
                    try {
                        hours = parseNumber(id, 1, false);
                        minutes = parseNumber(id, 3, false);
                        seconds = parseNumber(id, 5, false);
                    } catch (const Exception &ex) { ex.throws(__trace("core.time.ZoneOffset")); }
                    break;
                case 9:
                    try {
                        hours = parseNumber(id, 1, false);
                        minutes = parseNumber(id, 4, true);
                        seconds = parseNumber(id, 7, true);
                    } catch (const Exception &ex) { ex.throws(__trace("core.time.ZoneOffset")); }
                    break;
                default:
                    DateTimeException("Invalid ID for ZoneOffset, invalid format: " + id)
                            .throws(__trace("core.time.ZoneOffset"));
            }
            gchar const first = id.charAt(0);
            if (first != '+' && first != '-') {
                DateTimeException("Invalid ID for ZoneOffset, plus/minus not found when expected: " + id)
                        .throws(__trace("core.time.ZoneOffset"));
            }
            try {
                if (first == '-') {
                    return of(-hours, -minutes, -seconds);
                } else {
                    return of(hours, minutes, seconds);
                }
            } catch (const Exception &ex) { ex.throws(__trace("core.time.ZoneOffset")); }
        }

        gint ZoneOffset::parseNumber(const String &offsetId, gint pos, gbool precededByColon) {
            if (precededByColon && offsetId.charAt(pos - 1) != ':') {
                DateTimeException("Invalid ID for ZoneOffset, colon not found when expected: " + offsetId)
                        .throws(__trace("core.time.ZoneOffset"));
            }
            gchar const ch1 = offsetId.charAt(pos);
            gchar const ch2 = offsetId.charAt(pos + 1);
            if (ch1 < '0' || ch1 > '9' || ch2 < '0' || ch2 > '9') {
                DateTimeException("Invalid ID for ZoneOffset, non numeric characters found: " + offsetId)
                        .throws(__trace("core.time.ZoneOffset"));
            }
            return (ch1 - 48) * 10 + (ch2 - 48);
        }

        ZoneOffset ZoneOffset::of(gint hours) {
            try {
                return ZoneOffset::of(hours, 0, 0);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.ZoneOffset")); }
        }

        ZoneOffset ZoneOffset::of(gint hours, gint minutes) {
            try {
                return ZoneOffset::of(hours, minutes, 0);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.ZoneOffset")); }
        }

        ZoneOffset ZoneOffset::of(gint hours, gint minutes, gint seconds) {
            try {
                validate(hours, minutes, seconds);
                gint const totalSeconds = ZoneOffset::totalSeconds(hours, minutes, seconds);
                return ZoneOffset::ofSeconds(totalSeconds);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.ZoneOffset")); }
        }

        ZoneOffset ZoneOffset::from(const Temporal &temporal) {
            const Object &offset = temporal.query(OFFSET);
            if (offset == null) {
                DateTimeException("Unable to obtain ZoneOffset from TemporalAccessor: " + temporal.toString()
                                  + " of type " + temporal.classname()).throws(__trace("core.time.ZoneOffset"));
            }
            return (ZoneOffset &) offset;
        }

        void ZoneOffset::validate(gint hours, gint minutes, gint seconds) {
            if (hours < -18 || hours > 18) {
                DateTimeException("Zone offset hours not in valid range: value " + String::valueOf(hours)
                                  + " is not in the range -18 to 18").throws(__trace("core.time.ZoneOffset"));;
            }
            if (hours > 0) {
                if (minutes < 0 || seconds < 0) {
                    DateTimeException("Zone offset minutes and seconds must be positive because hours is positive")
                            .throws(__trace("core.time.ZoneOffset"));
                }
            } else if (hours < 0) {
                if (minutes > 0 || seconds > 0) {
                    DateTimeException("Zone offset minutes and seconds must be negative because hours is negative")
                            .throws(__trace("core.time.ZoneOffset"));
                }
            } else if ((minutes > 0 && seconds < 0) || (minutes < 0 && seconds > 0)) {
                DateTimeException("Zone offset minutes and seconds must have the same sign")
                        .throws(__trace("core.time.ZoneOffset"));
            }
            if (minutes < -59 || minutes > 59) {
                DateTimeException("Zone offset minutes not in valid range: value " +
                                  String::valueOf(minutes) + " is not in the range -59 to 59")
                        .throws(__trace("core.time.ZoneOffset"));
            }
            if (seconds < -59 || seconds > 59) {
                DateTimeException("Zone offset seconds not in valid range: value " +
                                  String::valueOf(seconds) + " is not in the range -59 to 59")
                        .throws(__trace("core.time.ZoneOffset"));
            }
            if (Math::abs(hours) == 18 && (minutes | seconds) != 0) {
                DateTimeException("Zone offset not in valid range: -18:00 to +18:00")
                        .throws(__trace("core.time.ZoneOffset"));
            }
        }

        gint ZoneOffset::totalSeconds(gint hours, gint minutes, gint seconds) {
            return hours * LocalTime::SECONDS_PER_HOUR + minutes * LocalTime::SECONDS_PER_MINUTE + seconds;
        }

        ZoneOffset ZoneOffset::ofSeconds(gint totalSeconds) {
            if (totalSeconds < -MAX_SECONDS || totalSeconds > MAX_SECONDS) {
                DateTimeException("Zone offset not in valid range: -18:00 to +18:00")
                        .throws(__trace("core.time.ZoneOffset"));
            }
            if (totalSeconds % (15 * LocalTime::SECONDS_PER_MINUTE) == 0) {
                ZoneOffset const result = ZoneOffset(totalSeconds);
                ID_CACHE.putIfAbsent(result.id(), result);
                return SECONDS_CACHE.putIfAbsent(totalSeconds, result);
            } else {
                return ZoneOffset(totalSeconds);
            }
        }

        ZoneOffset::ZoneOffset(gint totalSeconds) : seconds(totalSeconds), zRules(UTC) {
            if (totalSeconds == 0) {
                ID = "Z";
            } else {
                gint const absTotalSeconds = Math::abs(totalSeconds);
                StringBuffer buf = StringBuffer();
                gint const absHours = absTotalSeconds / LocalTime::SECONDS_PER_HOUR;
                gint const absMinutes = (absTotalSeconds / LocalTime::SECONDS_PER_MINUTE) % LocalTime::MINUTES_PER_HOUR;
                buf.append(totalSeconds < 0 ? "-" : "+").append(absHours < 10 ? "0" : "").append(absHours)
                        .append(absMinutes < 10 ? ":0" : ":").append(absMinutes);
                gint const absSeconds = absTotalSeconds % LocalTime::SECONDS_PER_MINUTE;
                if (absSeconds != 0) {
                    buf.append(absSeconds < 10 ? ":0" : ":").append(absSeconds);
                }
                ID = buf.toString();
                zRules = this != &MAX && this != &MIN ? ZoneRules::of(*this) :
                         ZoneRules(*this);
            }
        }

        gint ZoneOffset::totalSeconds() const {
            return seconds;
        }

        String ZoneOffset::id() const {
            return ID;
        }

        ZoneRules ZoneOffset::rules() const {
            return zRules;
        }

        ZoneOffset ZoneOffset::offset(glong  /*epochSecond*/) const {
            return *this;
        }

        gbool ZoneOffset::supportField(Temporal::Field field) const {
            return field == OFFSET_SECONDS;
        }

        gint ZoneOffset::get(Temporal::TemporalField field) const {
            if (field == OFFSET_SECONDS) {
                return seconds;
            } else {
                UnsupportedTemporalException("Unsupported field: " + FieldName[field])
                        .throws(__trace("core.time.ZoneOffset"));
            }
        }

        glong ZoneOffset::getLong(Temporal::TemporalField field) const {
            if (field == OFFSET_SECONDS) {
                return seconds;
            } else {
                UnsupportedTemporalException("Unsupported field: " + FieldName[field])
                        .throws(__trace("core.time.ZoneOffset"));
            }
        }

        const Object &ZoneOffset::query(const Temporal::Query &query) const {
            if (query == OFFSET || query == ZONE_ID) {
                return clone();
            }
            return null;
        }

        gint ZoneOffset::compareTo(const ZoneOffset &other) const {
            // abs(totalSeconds) <= MAX_SECONDS, so no overflow can happen here
            return other.seconds - seconds;
        }

        gbool ZoneOffset::equals(const Object &obj) const {
            if (this == &obj) {
                return true;
            }
            if (Class<ZoneOffset>::hasInstance(obj)) {
                return seconds == ((ZoneOffset &) obj).seconds;
            }
            return false;
        }

        gint ZoneOffset::hash() const {
            return seconds;
        }

        String ZoneOffset::toString() const {
            return ID;
        }

        Object &ZoneOffset::clone() const {
            if (this == &INVALID)
                return (ZoneOffset &) INVALID;
            return native::Unsafe::createInstance<ZoneOffset>(*this);
        }

        ZoneOffset::ZoneOffset(const ZoneOffset &offset) :
                seconds(offset.seconds), zRules(offset.zRules), ID(offset.ID) {}

        ZoneOffset::ZoneOffset(ZoneOffset &&offset) CORE_NOTHROW:
                seconds(offset.seconds), zRules(native::Unsafe::moveInstance(offset.zRules)),
                ID(native::Unsafe::moveInstance(offset.ID)) {}

        ZoneOffset &ZoneOffset::operator=(const ZoneOffset &offset) {
            if (this != &offset) {
                seconds = offset.seconds;
                ID = offset.ID;
                zRules = offset.zRules;
            }
            return *this;
        }

        ZoneOffset &ZoneOffset::operator=(ZoneOffset &&offset) CORE_NOTHROW {
            if (this != &offset) {
                seconds = offset.seconds;
                ID = native::Unsafe::moveInstance(offset.ID);
                zRules = native::Unsafe::moveInstance(offset.zRules);
            }
            return *this;
        }
    } // time
} // core
