//
// Created by T.N.Brunshweeck on 23/12/2023.
//

#include "Era.h"
#include <core/time/UnsupportedTemporalException.h>
#include <core/Enum.h>
#include <core/private/Unsafe.h>

namespace core {
    namespace time {

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

        gbool Era::supportField(Temporal::Field field) const {
            return field == ERA;
        }

        gbool Era::supportUnit(Temporal::Unit unit) const {
            return false;
        }

        gint Era::get(Temporal::TemporalField field) const {
            if (field == ERA)
                return value();
            UnsupportedTemporalException("Unsupported field: " + FieldName[field]).throws(__trace("core.time.Era"));
        }

        glong Era::getLong(Temporal::TemporalField field) const {
            if (field == ERA)
                return value();
            UnsupportedTemporalException("Unsupported field: " + FieldName[field]).throws(__trace("core.time.Era"));
        }

        const Object &Era::query(const Temporal::Query &query) const {
            if (query == PRECISION)
                return native::Unsafe::createInstance<Enum<Unit>>(ERAS);
            return query.queryFrom(*this);
        }

        glong Era::until(const Temporal &endExclusive, Temporal::TemporalUnit unit) const {
            UnsupportedMethodException("until").throws(__trace("core.time.Era"));
        }

        const Era::MinguoEra &Era::MinguoEra::of(gint era) {
            if (era == 0)
                return BEFORE_ROC;
            if (era == 1)
                return ROC;
            DateTimeException("Invalid era: " + String::valueOf(era)).throws(__trace("core.time.Era.MinguoEra"));
        }

        gint Era::MinguoEra::value() const {
            return this == &BEFORE_ROC ? 0 : 1;
        }

        String Era::MinguoEra::displayName(DateTimeFormatter::TextStyle style, const LocalDate &localDate) const {
            return {};
        }

        Era::MinguoEra::MinguoEra(gint era) : era(era) {}

        Object &Era::MinguoEra::clone() const {
            return (Object &) of(era);
        }

        gbool Era::MinguoEra::equals(const Object &o) const {
            if (this == &o)
                return true;
            if (!Class<MinguoEra>::hasInstance(o))
                return false;
            const MinguoEra &other = (MinguoEra &) o;
            return era == other.era;
        }

        Era::JapaneseEra::JapaneseEra(gint era, const LocalDate &since) : era(era), since(since) {}

        const Era::JapaneseEra &Era::JapaneseEra::of(gint era) {
            switch (era) {
                case -1:
                    return MEIJI;
                case 0:
                    return TAISHO;
                case 1:
                    return SHOWA;
                case 2:
                    return HEISEI;
                case 3:
                    return REIWA;
            }
            DateTimeException("Invalid era: " + String::valueOf(era)).throws(__trace("core.time.Era.JapaneseEra"));
        }

        const Era::JapaneseEra &Era::JapaneseEra::valueOf(const String &era) {
            //
        }

        String Era::JapaneseEra::displayName(DateTimeFormatter::TextStyle style, const LocalDate &localDate) const {
            return String();
        }

        gint Era::JapaneseEra::value() const {
            return era;
        }

        String Era::JapaneseEra::abbreviation() const {
            return String();
        }

        String Era::JapaneseEra::name() const {
            return String();
        }

        gbool Era::JapaneseEra::equals(const Object &o) const {
            if (this == &o)
                return true;
            if (!Class<JapaneseEra>::hasInstance(o))
                return false;
            const JapaneseEra &other = (JapaneseEra &) o;
            return era == other.era && since == other.since;
        }

        Object &Era::JapaneseEra::clone() const {
            return (Object &) of(era);
        }

        const Era::JapaneseEra &Era::JapaneseEra::from(const LocalDate &date) {
            if (date.isBefore(MEIJI.since))
                DateTimeException("JapaneseDate before Meiji 6 are not supported")
                        .throws(__trace("core.time.Era.JapaneseEra"));
            if (date.compareTo(REIWA.since) >= 0) return REIWA;
            else if (date.compareTo(HEISEI.since) >= 0) return HEISEI;
            else if (date.compareTo(SHOWA.since) >= 0) return SHOWA;
            else if (date.compareTo(TAISHO.since) >= 0) return TAISHO;
            else return MEIJI;
        }

        Era::ISOEra::ISOEra(gint era) : era(era) {}

        const Era::ISOEra &Era::ISOEra::of(gint era) {
            if (era == 0)
                return BCE;
            if (era == 1)
                return CE;
            DateTimeException("Invalid era: " + String::valueOf(era)).throws(__trace("core.time.Era.ISOEra"));
        }

        gint Era::ISOEra::value() const {
            return era;
        }

        String Era::ISOEra::displayName(DateTimeFormatter::TextStyle style, const LocalDate &localDate) const {
            return "ISO";
        }

        gbool Era::ISOEra::equals(const Object &o) const {
            if (this == &o)
                return true;
            if (!Class<ISOEra>::hasInstance(o))
                return false;
            const ISOEra &other = (ISOEra &) o;
            return era == other.era;
        }

        Object &Era::ISOEra::clone() const {
            return (Object &) of(era);
        }

        const Era::HijrahEra &Era::HijrahEra::of(gint era) {
            if (era == 1)
                return AH;
            DateTimeException("Invalid era: " + String::valueOf(era)).throws(__trace("core.time.Era.HijrahEra"));
        }

        gint Era::HijrahEra::value() const {
            return 1;
        }

        String Era::HijrahEra::displayName(DateTimeFormatter::TextStyle style, const LocalDate &localDate) const {
            return {};
        }

        gbool Era::HijrahEra::equals(const Object &o) const {
            return this == &AH;
        }

        Object &Era::HijrahEra::clone() const {
            return (Object &) AH;
        }

        const Era::HijrahEra Era::AH = {};
        const Era::ISOEra Era::BCE = ISOEra(0);
        const Era::ISOEra Era::CE = ISOEra(1);
        const Era::JapaneseEra Era::MEIJI = JapaneseEra(-1, LocalDate(1868, 1, 1));
        const Era::JapaneseEra Era::TAISHO = JapaneseEra(0, LocalDate(1912, 7, 30));
        const Era::JapaneseEra Era::SHOWA = JapaneseEra(1, LocalDate(1926, 12, 25));
        const Era::JapaneseEra Era::HEISEI = JapaneseEra(2, LocalDate(1989, 1, 8));
        const Era::JapaneseEra Era::REIWA = JapaneseEra(3, LocalDate(2019, 5, 1));
        const Era::MinguoEra Era::BEFORE_ROC = MinguoEra(0);
        const Era::MinguoEra Era::ROC = MinguoEra(0);

    } // time
} // core