//
// Created by T.N.Brunshweeck on 26/12/2023.
//

#include "ZoneOffsetTransition.h"
#include <core/time/ZoneOffset.h>
#include <core/util/ArrayList.h>
#include <core/Long.h>

namespace core {
    namespace time {
        ZoneOffsetTransition ZoneOffsetTransition::of(const LocalDateTime &transition, const ZoneOffset &offsetBefore,
                                                      const ZoneOffset &offsetAfter) {
            if (offsetBefore.equals(offsetAfter)) {
                IllegalArgumentException("Offsets must not be equal").throws(__trace("core.time.ZoneOffsetTransition"));
            }
            if (transition.nano() != 0) {
                IllegalArgumentException("Nano-of-second must be zero").throws(__trace("core.time.ZoneOffsetTransition"));
            }
            return ZoneOffsetTransition(transition, offsetBefore, offsetAfter);
        }

        ZoneOffsetTransition::ZoneOffsetTransition(const LocalDateTime &transition, const ZoneOffset &offsetBefore,
                                                   const ZoneOffset &offsetAfter) :
                epochSecond(transition.toEpochSecond(offsetBefore)), after(offsetAfter), before(offsetBefore),
                transition(transition) {}

        glong ZoneOffsetTransition::toEpochSecond() const {
            return epochSecond;
        }

        LocalDateTime ZoneOffsetTransition::dateTimeBefore() const {
            return transition;
        }

        LocalDateTime ZoneOffsetTransition::dateTimeAfter() const {
            try {
                return transition.afterSeconds(duration());
            } catch (const Exception &ex) { ex.throws(__trace("core.time.ZoneOffsetTransition")); }
        }

        ZoneOffset ZoneOffsetTransition::offsetBefore() const {
            return before;
        }

        ZoneOffset ZoneOffsetTransition::offsetAfter() const {
            return after;
        }

        gint ZoneOffsetTransition::duration() const {
            return after.totalSeconds() - before.totalSeconds();
        }

        gbool ZoneOffsetTransition::isGap() const {
            return after.totalSeconds() > before.totalSeconds();
        }

        gbool ZoneOffsetTransition::isOverlap() const {
            return after.totalSeconds() < before.totalSeconds();
        }

        gbool ZoneOffsetTransition::isValidOffset(const ZoneOffset &offset) const {
            return isGap() ? false :
                   (before.equals(offset) || after.equals(offset));
        }

        Array<ZoneOffset> ZoneOffsetTransition::validOffsets() const {
            if (isGap())
                return {};
            return Array<ZoneOffset>::of(before, after);
        }

        gint ZoneOffsetTransition::compareTo(const ZoneOffsetTransition &trans) const {
            return Long::compare(epochSecond, trans.epochSecond);
        }

        gbool ZoneOffsetTransition::equals(const Object &other) const {
            if (&other == this) {
                return true;
            }
            if (!Class<ZoneOffsetTransition>::hasInstance(other))
                return false;
            ZoneOffsetTransition const &d = (ZoneOffsetTransition &) other;
            return epochSecond == d.epochSecond
                   && before.equals(d.before)
                   && after.equals(d.after);
        }

        gint ZoneOffsetTransition::hash() const {
            return transition.hash() ^ before.hash() ^ Integer::rotateLeft(after.hash(), 16);
        }

        String ZoneOffsetTransition::toString() const {
            StringBuffer buf = {};
            buf.append("Transition[")
                    .append(isGap() ? "Gap" : "Overlap")
                    .append(" at ")
                    .append(transition)
                    .append(before)
                    .append(" to ")
                    .append(after)
                    .append(']');
            return buf.toString();
        }

        Object &ZoneOffsetTransition::clone() const {
            return native::Unsafe::allocateInstance<ZoneOffsetTransition>(*this);
        }

        ZoneOffsetTransition::ZoneOffsetTransition(glong epochSecond, const ZoneOffset &offsetBefore,
                                                   const ZoneOffset &offsetAfter) :
                epochSecond(epochSecond), after(offsetAfter), before(offsetBefore),
                transition(LocalDateTime::ofEpochSecond(epochSecond, 0, offsetBefore)) {}

    } // util
} // core