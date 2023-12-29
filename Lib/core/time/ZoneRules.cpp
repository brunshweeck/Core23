//
// Created by T.N.Brunshweeck on 25/12/2023.
//

#include "ZoneRules.h"
#include <core/Long.h>
#include <core/Integer.h>
#include <core/util/ArrayList.h>
#include <core/time/ZoneOffset.h>
#include <core/time/ZoneOffsetTransition.h>
#include <core/time/ZoneOffsetTransitionRule.h>

namespace core {
    namespace time {

        using namespace util;
        using namespace native;

        namespace {

            template<class T>
            gint binarySearch(const ReferenceArray &a, const T &x) {
                gint start = 0;
                gint end = a.length();
                gint i = end >> 1;
                gint r = 0;
                do {
                    r = x.compareTo((const T &) a[i]);
                    if (r == 0)
                        return i;
                    else if (r > 0)
                        start = i + 1;
                    else
                        end = i - 1;
                    i = (start + end) / 2;
                } while (start < end);
                return -1;
            }

            gint binarySearch(const LongArray &a, glong x) {
                gint start = 0;
                gint end = a.length();
                gint i = end >> 1;
                glong r = 0;
                do {
                    r = x - a[i];
                    if (r == 0)
                        return i;
                    else if (r > 0)
                        start = i + 1;
                    else
                        end = i - 1;
                    i = (start + end) / 2;
                } while (start < end);
                return -1;
            }

        }

        ZoneRules ZoneRules::of(const ZoneOffset &baseStandardOffset, const ZoneOffset &baseWallOffset,
                                const util::List<ZoneOffsetTransition> &standardOffsetTransitionList,
                                const util::List<ZoneOffsetTransition> &transitionList,
                                const util::List<ZoneOffsetTransitionRule> &lastRules) {
            return ZoneRules(baseStandardOffset,
                             baseWallOffset,
                             standardOffsetTransitionList,
                             transitionList,
                             lastRules);
        }

        ZoneRules ZoneRules::of(const ZoneOffset &offset) {
            return ZoneRules(offset);
        }

        ZoneRules::ZoneRules(const ZoneOffset &baseStandardOffset, const ZoneOffset &baseWallOffset,
                             const List<ZoneOffsetTransition> &standardOffsetTransitionList,
                             const List<ZoneOffsetTransition> &transitionList,
                             const List<ZoneOffsetTransitionRule> &lastRules) {

            // convert standard transitions

            this->standardTransitions = LongArray(standardOffsetTransitionList.size());

            this->standardOffsets = ReferenceArray(standardOffsetTransitionList.size() + 1);
            this->standardOffsets.set(0, Unsafe::copyInstance(baseStandardOffset));
            for (gint i = 0; i < standardOffsetTransitionList.size(); i++) {
                this->standardTransitions[i] = standardOffsetTransitionList.get(i).toEpochSecond();
                this->standardOffsets.set(i + 1,
                                          Unsafe::copyInstance(standardOffsetTransitionList.get(i).offsetAfter()));
            }

            // convert savings transitions to locals
            ArrayList<LocalDateTime> localTransitionList = ArrayList<LocalDateTime>();
            ArrayList<ZoneOffset> localTransitionOffsetList = ArrayList<ZoneOffset>();
            localTransitionOffsetList.add(baseWallOffset);
            for (const ZoneOffsetTransition &trans: transitionList) {
                if (trans.isGap()) {
                    localTransitionList.add(trans.dateTimeBefore());
                    localTransitionList.add(trans.dateTimeAfter());
                } else {
                    localTransitionList.add(trans.dateTimeAfter());
                    localTransitionList.add(trans.dateTimeBefore());
                }
                localTransitionOffsetList.add(trans.offsetAfter());
            }
            this->savingsLocalTransitions = localTransitionList.toArray();
            this->wallOffsets = localTransitionOffsetList.toArray();

            // convert savings transitions to instants
            this->savingsInstantTransitions = LongArray(transitionList.size());
            for (gint i = 0; i < transitionList.size(); i++) {
                this->savingsInstantTransitions[i] = transitionList.get(i).toEpochSecond();
            }

            // last rules
            if (lastRules.size() > 0) {
                ReferenceArray const rulesArray = lastRules.toArray();
                if (rulesArray.length() > 16) {
                    ArgumentException("Too many transition rules").throws(__trace("core.time.ZoneRules"));
                }
                this->lastRules = rulesArray;
                this->lastRulesCache = HashMap<Integer, ReferenceArray>();
            } else {
                this->lastRules = ReferenceArray(0);
                this->lastRulesCache = HashMap<Integer, ReferenceArray>(0);
            }
        }

        ZoneRules::ZoneRules(const LongArray &standardTransitions, const ReferenceArray &standardOffsets,
                             const LongArray &savingsInstantTransitions, const ReferenceArray &wallOffsets,
                             const ReferenceArray &lastRules) :
                lastRules(lastRules), wallOffsets(wallOffsets),
                savingsInstantTransitions(savingsInstantTransitions), standardOffsets(standardOffsets),
                standardTransitions(standardTransitions) {

            this->lastRulesCache = HashMap<Integer, ReferenceArray>();

            if (savingsInstantTransitions.length() == 0) {
                this->savingsLocalTransitions = ReferenceArray(0);
            } else {
                // convert savings transitions to locals
                ArrayList<LocalDateTime> localTransitionList = ArrayList<LocalDateTime>();
                for (gint i = 0; i < savingsInstantTransitions.length(); i++) {
                    ZoneOffset const &before = (ZoneOffset &) wallOffsets[i];
                    ZoneOffset const &after = (ZoneOffset &) wallOffsets[i + 1];
                    ZoneOffsetTransition const trans = ZoneOffsetTransition(savingsInstantTransitions[i], before,
                                                                            after);
                    if (trans.isGap()) {
                        localTransitionList.add(trans.dateTimeBefore());
                        localTransitionList.add(trans.dateTimeAfter());
                    } else {
                        localTransitionList.add(trans.dateTimeAfter());
                        localTransitionList.add(trans.dateTimeBefore());
                    }
                }
                this->savingsLocalTransitions = localTransitionList.toArray();
            }
        }

        ZoneRules::ZoneRules(const ZoneOffset &offset) : standardOffsets(1), wallOffsets(1) {
            if (&offset == &ZoneOffset::UTC || &offset == &ZoneOffset::MIN || &offset == &ZoneOffset::MAX) {
                standardOffsets.set(0, (ZoneOffset &) offset);
            } else {
                standardOffsets.set(0, Unsafe::copyInstance(offset));
            }
            wallOffsets.set(0, standardOffsets[0]);
        }

        gbool ZoneRules::isFixedOffset() const {
            return standardOffsets[0].equals(wallOffsets[0]) &&
                   standardTransitions.length() == 0 &&
                   savingsInstantTransitions.length() == 0 &&
                   lastRules.length() == 0;
        }

        ZoneOffset ZoneRules::offset(glong epochSecond) const {
            if (savingsInstantTransitions.length() == 0) {
                return (ZoneOffset &) wallOffsets[0];
            }
            // check if using last rules
            if (lastRules.length() > 0 &&
                epochSecond > savingsInstantTransitions[savingsInstantTransitions.length() - 1]) {
                gint const year = findYear(epochSecond, (ZoneOffset &) wallOffsets[wallOffsets.length() - 1]);
                ReferenceArray transArray = transitionArray(year);
                gint i = 0;
                for (; i < transArray.length(); i++) {
                    ZoneOffsetTransition const trans = (ZoneOffsetTransition &) transArray[i];
                    if (epochSecond < trans.toEpochSecond()) {
                        return trans.offsetBefore();
                    }
                }
                return ((ZoneOffsetTransition &) transArray[i - 1]).offsetAfter();
            }

            // using historic rules
            gint index = binarySearch(savingsInstantTransitions, epochSecond);
            if (index < 0) {
                // switch negative insert position to start of matched range
                index = -index - 2;
            }
            return (ZoneOffset &) wallOffsets[index + 1];
        }

        ZoneOffset ZoneRules::offset(const LocalDateTime &dateTime) const {
            Object &obj = offsetInfo(dateTime);
            if (Class<ZoneOffsetTransition>::hasInstance(obj)) {
                ZoneOffsetTransition const &transition = (ZoneOffsetTransition &) obj;
                return transition.offsetBefore();
            }
            ZoneOffset const &offset = (ZoneOffset &) obj;
            return offset;
        }

        ArrayList<ZoneOffset> ZoneRules::validOffsets(const LocalDateTime &dateTime) const {
            Object &info = offsetInfo(dateTime);
            if (Class<ZoneOffsetTransition>::hasInstance(info)) {
                return ((ZoneOffsetTransition &) info).validOffsets();
            }
            ArrayList<ZoneOffset> offsets(1);
            offsets.add((ZoneOffset &) info);
            return offsets;
        }

        ZoneOffsetTransition ZoneRules::transition(const LocalDateTime &dateTime) const {
            Object &info = offsetInfo(dateTime);
            if (Class<ZoneOffsetTransition>::hasInstance(info)) {
                return (ZoneOffsetTransition &) info;
            } else {
                Error("Not such transaction").throws(__trace("core.time.dateTime"));
            }
        }

        ZoneOffset ZoneRules::standardOffset(glong epochSecond) const {
            if (standardTransitions.length() == 0) {
                return (ZoneOffset &) standardOffsets[0];
            }
            gint index = binarySearch(standardTransitions, epochSecond);
            if (index < 0) {
                // switch negative insert position to start of matched range
                index = -index - 2;
            }
            return (ZoneOffset &) standardOffsets[index + 1];
        }

        gint ZoneRules::daylightSavings(glong epochSecond) const {
            if (isFixedOffset()) {
                return 0;
            }
            ZoneOffset standardOffset = this->standardOffset(epochSecond);
            ZoneOffset const actualOffset = offset(epochSecond);
            return actualOffset.totalSeconds() - standardOffset.totalSeconds();
        }

        gbool ZoneRules::isDaylightSavings(glong epochSecond) const {
            return !standardOffset(epochSecond).equals(offset(epochSecond));
        }

        gbool ZoneRules::isValidOffset(const LocalDateTime &dateTime, const ZoneOffset &offset) const {
            return validOffsets(dateTime).contains(offset);
        }

        ZoneOffsetTransition ZoneRules::nextTransition(glong epochSecond) const {
            if (savingsInstantTransitions.length() == 0) {
                Error("Not such transition").throws(__trace("core.time.dateTime"));
            }
            // check if using last rules
            if (epochSecond >= savingsInstantTransitions[savingsInstantTransitions.length() - 1]) {
                if (lastRules.length() == 0) {
                    Error("Not such transition").throws(__trace("core.time.dateTime"));
                }
                // search year the instant is in
                gint const year = findYear(epochSecond, (ZoneOffset &) wallOffsets[wallOffsets.length() - 1]);
                ReferenceArray transArray = transitionArray(year);
                for (const Object &trans: transArray) {
                    if (epochSecond < ((ZoneOffsetTransition &) trans).toEpochSecond()) {
                        return (ZoneOffsetTransition &) trans;
                    }
                }
                // use first from following year
                if (year < 999999999) {
                    transArray = transitionArray(year + 1);
                    return (ZoneOffsetTransition &) transArray[0];
                }
                Error("Not such transition").throws(__trace("core.time.dateTime"));
            }
        }

        ZoneOffsetTransition ZoneRules::previousTransition(glong epochSecond) const {
            if (savingsInstantTransitions.length() == 0) {
                Error("Not such transition").throws(__trace("core.time.dateTime"));
            }

            // check if using last rules
            glong const lastHistoric = savingsInstantTransitions[savingsInstantTransitions.length() - 1];
            if (lastRules.length() > 0 && epochSecond > lastHistoric) {
                // search year the instant is in
                ZoneOffset const lastHistoricOffset = (ZoneOffset &) wallOffsets[wallOffsets.length() - 1];
                gint year = findYear(epochSecond, lastHistoricOffset);
                ReferenceArray transArray = transitionArray(year);
                for (gint i = transArray.length() - 1; i >= 0; i--) {
                    if (epochSecond > ((ZoneOffsetTransition &) transArray[i]).toEpochSecond()) {
                        return (ZoneOffsetTransition &) transArray[i];
                    }
                }
                // use last from preceding year
                gint const lastHistoricYear = findYear(lastHistoric, lastHistoricOffset);
                if (--year > lastHistoricYear) {
                    transArray = transitionArray(year);
                    return (ZoneOffsetTransition &) transArray[transArray.length() - 1];
                }
                // drop through
            }

            // using historic rules
            gint index = binarySearch(savingsInstantTransitions, epochSecond);
            if (index < 0) {
                index = -index - 1;
            }
            if (index <= 0) {
                Error("Not such transition").throws(__trace("core.time.dateTime"));
            }
            return ZoneOffsetTransition(savingsInstantTransitions[index - 1],
                                        (ZoneOffset &) wallOffsets[index - 1],
                                        (ZoneOffset &) wallOffsets[index]);
        }

        util::ArrayList<ZoneOffsetTransition> ZoneRules::transitions() const {
            ArrayList<ZoneOffsetTransition> list = {};
            for (gint i = 0; i < savingsInstantTransitions.length(); i++) {
                list.add(ZoneOffsetTransition(savingsInstantTransitions[i], (ZoneOffset &) wallOffsets[i],
                                              (ZoneOffset &) wallOffsets[i + 1]));
            }
            return list;
        }

        ArrayList<ZoneOffsetTransitionRule> ZoneRules::transitionRules() const {
            ArrayList<ZoneOffsetTransitionRule> list = {};
            for (gint i = 0; i < lastRules.length(); i++) {
                list.add((ZoneOffsetTransitionRule &) lastRules[i]);
            }
            return list;
        }

        gbool ZoneRules::equals(const Object &otherRules) const {
            if (this == &otherRules) {
                return true;
            }
            if (!Class<ZoneRules>::hasInstance(otherRules))
                return false;
            ZoneRules const &other = (ZoneRules &) otherRules;
            return Object::equals(standardTransitions, other.standardTransitions)
                   && Object::equals(standardOffsets, other.standardOffsets)
                   && Object::equals(savingsInstantTransitions, other.savingsInstantTransitions)
                   && Object::equals(wallOffsets, other.wallOffsets)
                   && Object::equals(lastRules, other.lastRules);
        }

        gint ZoneRules::hash() const {
            return (standardTransitions.hash()) ^
                   (standardOffsets.hash()) ^
                   (savingsInstantTransitions.hash()) ^
                   (wallOffsets.hash()) ^
                   (lastRules.hash());
        }

        String ZoneRules::toString() const {
            return "ZoneRules[currentStandardOffset=" + standardOffsets[standardOffsets.length() - 1].toString() + "]";
        }

        Object &ZoneRules::clone() const {
            return Unsafe::createInstance<ZoneRules>(*this);
        }

        gint ZoneRules::findYear(glong epochSecond, const ZoneOffset &offset) const {
            glong const localSecond = epochSecond + offset.totalSeconds();
            glong zeroDay = Math::floorDiv(localSecond, 86400) + DAYS_0000_TO_1970;

            // find the march-based year
            zeroDay -= 60;  // adjust to 0000-03-01 so leap day is at end of four year cycle
            glong adjust = 0;
            if (zeroDay < 0) {
                // adjust negative years to positive for calculation
                glong const adjustCycles = (zeroDay + 1) / DAYS_PER_CYCLE - 1;
                adjust = adjustCycles * 400;
                zeroDay += -adjustCycles * DAYS_PER_CYCLE;
            }
            glong yearEst = (400 * zeroDay + 591) / DAYS_PER_CYCLE;
            glong doyEst = zeroDay - (365 * yearEst + yearEst / 4 - yearEst / 100 + yearEst / 400);
            if (doyEst < 0) {
                // fix estimate
                yearEst--;
                doyEst = zeroDay - (365 * yearEst + yearEst / 4 - yearEst / 100 + yearEst / 400);
            }
            yearEst += adjust;  // reset any negative year

            // convert march-based values back to january-based, adjust year
            if (doyEst >= 306) {
                yearEst++;
            }

            // Cap to the max value
            return (gint) Math::min(yearEst, 999999999LL);
        }

        ReferenceArray ZoneRules::transitionArray(gint year) const {
            Integer const yearObj = year;  // should use Year class, but this saves a class load
            ReferenceArray transArray = lastRulesCache.get(yearObj);
            if (transArray != null) {
                return transArray;
            }
            ReferenceArray ruleArray = lastRules;
            transArray = ReferenceArray(ruleArray.length());
            for (gint i = 0; i < ruleArray.length(); i++) {
                transArray.set(i,
                               Unsafe::copyInstance(
                                       ((ZoneOffsetTransitionRule &) ruleArray[i]).createTransition(year)));
            }
            if (year < LAST_YEAR_CACHE) {
                ((HashMap<Integer, ReferenceArray> &) lastRulesCache).putIfAbsent(yearObj, transArray);
            }
            return transArray;
        }

        Object &ZoneRules::offsetInfo(const LocalDateTime &dt) const {
            if (savingsLocalTransitions.length() == 0) {
                return (Object &) wallOffsets[0];
            }
            // check if using last rules
            if (lastRules.length() > 0 &&
                dt.isAfter((LocalDateTime &) savingsLocalTransitions[savingsLocalTransitions.length() - 1])) {
                ReferenceArray const transArray = transitionArray(dt.year());
                for (const Object &trans: transArray) {
                    Object &info = offsetInfo(dt, (ZoneOffsetTransition &) trans);
                    if ((Class<ZoneOffsetTransition>::hasInstance(info)) ||
                        info.equals(((ZoneOffsetTransition &) trans).offsetBefore())) {
                        return info;
                    }
                }
                return null;
            }

            // using historic rules
            gint index = binarySearch(savingsLocalTransitions, dt);
            if (index == -1) {
                // before first transition
                return (ZoneOffset &) wallOffsets[0];
            }
            if (index < 0) {
                // switch negative insert position to start of matched range
                index = -index - 2;
            } else if (index < savingsLocalTransitions.length() - 1 &&
                       savingsLocalTransitions[index].equals(savingsLocalTransitions[index + 1])) {
                // handle overlap immediately following gap
                index++;
            }
            if ((index & 1) == 0) {
                // gap or overlap
                LocalDateTime const dtBefore = (LocalDateTime &) savingsLocalTransitions[index];
                LocalDateTime const dtAfter = (LocalDateTime &) savingsLocalTransitions[index + 1];
                ZoneOffset const offsetBefore = (ZoneOffset &) wallOffsets[index / 2];
                ZoneOffset const offsetAfter = (ZoneOffset &) wallOffsets[index / 2 + 1];
                if (offsetAfter.totalSeconds() > offsetBefore.totalSeconds()) {
                    // gap
                    ZoneOffsetTransition const trans = ZoneOffsetTransition(dtBefore, offsetBefore, offsetAfter);
                    return Unsafe::copyInstance(trans);
                } else {
                    // overlap
                    ZoneOffsetTransition const trans = ZoneOffsetTransition(dtAfter, offsetBefore, offsetAfter);
                    return Unsafe::copyInstance(trans);
                }
            } else {
                // normal (neither gap / overlap)
                return (ZoneOffset &) wallOffsets[index / 2 + 1];
            }
        }

        Object &ZoneRules::offsetInfo(const LocalDateTime &dt, const ZoneOffsetTransition &trans) const {
            LocalDateTime const localTransition = trans.dateTimeBefore();
            if (trans.isGap()) {
                if (dt.isBefore(localTransition)) {
                    return Unsafe::copyInstance(trans.offsetBefore());
                }
                if (dt.isBefore(trans.dateTimeAfter())) {
                    return Unsafe::copyInstance(trans);
                } else {
                    return Unsafe::copyInstance(trans.offsetAfter());
                }
            } else {
                if (dt.isBefore(localTransition) == false) {
                    return Unsafe::copyInstance(trans.offsetAfter());
                }
                if (dt.isBefore(trans.dateTimeAfter())) {
                    return Unsafe::copyInstance(trans.offsetBefore());
                } else {
                    return Unsafe::copyInstance(trans);
                }
            }
        }

        ZoneRules::ZoneRules(const ZoneRules &rules) :
                standardTransitions(rules.standardTransitions),
                standardOffsets(rules.standardOffsets),
                savingsInstantTransitions(rules.savingsInstantTransitions),
                savingsLocalTransitions(rules.savingsLocalTransitions),
                wallOffsets(rules.wallOffsets),
                lastRules(rules.lastRules),
                lastRulesCache(rules.lastRulesCache) {}

        ZoneRules::ZoneRules(ZoneRules &&rules) CORE_NOTHROW:
                standardTransitions(Unsafe::moveInstance(rules.standardTransitions)),
                standardOffsets(Unsafe::moveInstance(rules.standardOffsets)),
                savingsInstantTransitions(Unsafe::moveInstance(rules.savingsInstantTransitions)),
                savingsLocalTransitions(Unsafe::moveInstance(rules.savingsLocalTransitions)),
                wallOffsets(Unsafe::moveInstance(rules.wallOffsets)),
                lastRules(Unsafe::moveInstance(rules.lastRules)),
                lastRulesCache(Unsafe::moveInstance(rules.lastRulesCache)) {}

        ZoneRules &ZoneRules::operator=(const ZoneRules &rules) {
            if (this != &rules) {
                this->standardTransitions = rules.standardTransitions;
                this->standardOffsets = rules.standardOffsets;
                this->savingsInstantTransitions = rules.savingsInstantTransitions;
                this->savingsLocalTransitions = rules.savingsLocalTransitions;
                this->wallOffsets = rules.wallOffsets;
                this->lastRules = rules.lastRules;
            }
            return *this;
        }

        ZoneRules &ZoneRules::operator=(ZoneRules &&rules) CORE_NOTHROW {
            if (this != &rules) {
                this->standardTransitions = Unsafe::moveInstance(rules.standardTransitions);
                this->standardOffsets = Unsafe::moveInstance(rules.standardOffsets);
                this->savingsInstantTransitions = Unsafe::moveInstance(rules.savingsInstantTransitions);
                this->savingsLocalTransitions = Unsafe::moveInstance(rules.savingsLocalTransitions);
                this->wallOffsets = Unsafe::moveInstance(rules.wallOffsets);
                this->lastRules = Unsafe::moveInstance(rules.lastRules);
            }
            return *this;
        }

        ZoneRules::~ZoneRules() {
            lastRulesCache.clear();
        }
    } // time
} // core