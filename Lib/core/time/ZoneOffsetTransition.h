//
// Created by T.N.Brunshweeck on 26/12/2023.
//

#ifndef CORE23_ZONEOFFSETTRANSITION_H
#define CORE23_ZONEOFFSETTRANSITION_H

#include <core/time/ZoneOffset.h>
#include <core/time/LocalDateTime.h>

namespace core {
    namespace time {

        class ZoneOffsetTransitionRule;
        class ZoneRules;

        /**
         * A transition between two offsets caused by a discontinuity in the local time-line.
         * <p>
         * A transition between two offsets is normally the result of a daylight savings cutover.
         * The discontinuity is normally a gap in spring and an overlap in autumn.
         * <b> ZoneOffsetTransition} models the transition between the two offsets.
         * <p>
         * Gaps occur where there are local date-times that simply do not exist.
         * An example would be when the offset changes from <b> +03:00} to <b> +04:00}.
         * This might be described as 'the clocks will move forward one hour tonight at 1am'.
         * <p>
         * Overlaps occur where there are local date-times that exist twice.
         * An example would be when the offset changes from <b> +04:00} to <b> +03:00}.
         * This might be described as 'the clocks will move back one hour tonight at 2am'.
         *
         * @implSpec
         * This class is immutable and thread-safe.
         *
         */
        class ZoneOffsetTransition CORE_FINAL : public Object, public Comparable<ZoneOffsetTransition> {
        private:
            /**
             * The transition epoch-second.
             */
            glong epochSecond;

            /**
             * The local transition date-time at the transition.
             */
            LocalDateTime transition;

            /**
             * The offset before transition.
             */
            ZoneOffset before;

            /**
             * The offset after transition.
             */
            ZoneOffset after;

        public:

            //-----------------------------------------------------------------------
            /**
             * Obtains an INSTANCE defining a transition between two offsets.
             * <p>
             * Applications should normally obtain an INSTANCE from <b style="color:orange;"> ZoneRules}.
             * This factory is only intended for use when creating <b style="color:orange;"> ZoneRules}.
             *
             * @param transition  the transition date-time at the transition, which never
             *  actually occurs, expressed local to the before offset
             * @param offsetBefore  the offset before the transition
             * @param offsetAfter  the offset at and after the transition
             * @return the transition
             * @throws IllegalArgumentException if <b> offsetBefore} and <b> offsetAfter}
             *         are equal, or <b> transition.getNano()} returns non-zero value
             */
            static ZoneOffsetTransition of(const LocalDateTime &transition,
                                           const ZoneOffset &offsetBefore, const ZoneOffset &offsetAfter);

        private:

            /**
             * Creates an INSTANCE defining a transition between two offsets.
             *
             * @param transition  the transition date-time with the offset before the transition
             * @param offsetBefore  the offset before the transition
             * @param offsetAfter  the offset at and after the transition
             */
             CORE_EXPLICIT ZoneOffsetTransition(const LocalDateTime &transition,
                                                const ZoneOffset &offsetBefore, const ZoneOffset &offsetAfter);

            /**
             * Creates an INSTANCE from epoch-second and offsets.
             *
             * @param epochSecond  the transition epoch-second
             * @param offsetBefore  the offset before the transition
             * @param offsetAfter  the offset at and after the transition
             */
            CORE_EXPLICIT ZoneOffsetTransition(glong epochSecond, const ZoneOffset &offsetBefore, const ZoneOffset &offsetAfter);

             friend ZoneOffsetTransitionRule;
             friend ZoneRules;

        public:
            //-----------------------------------------------------------------------
            /**
             * Gets the transition instant as an epoch second.
             *
             * @return the transition epoch second
             */
             glong toEpochSecond() const;

            //-------------------------------------------------------------------------
            /**
             * Gets the local transition date-time, as would be expressed with the 'before' offset.
             * <p>
             * This is the date-time where the discontinuity begins expressed with the 'before' offset.
             * At this instant, the 'after' offset is actually used, therefore the combination of this
             * date-time and the 'before' offset will never occur.
             * <p>
             * The combination of the 'before' date-time and offset represents the same instant
             * as the 'after' date-time and offset.
             *
             * @return the transition date-time expressed with the before offset
             */
             LocalDateTime dateTimeBefore() const;

            /**
             * Gets the local transition date-time, as would be expressed with the 'after' offset.
             * <p>
             * This is the first date-time after the discontinuity, when the new offset applies.
             * <p>
             * The combination of the 'before' date-time and offset represents the same instant
             * as the 'after' date-time and offset.
             *
             * @return the transition date-time expressed with the after offset
             */
             LocalDateTime dateTimeAfter() const;

            /**
             * Gets the offset before the transition.
             * <p>
             * This is the offset in use before the instant of the transition.
             *
             * @return the offset before the transition
             */
             ZoneOffset offsetBefore() const;

            /**
             * Gets the offset after the transition.
             * <p>
             * This is the offset in use on and after the instant of the transition.
             *
             * @return the offset after the transition
             */
             ZoneOffset offsetAfter() const;

            /**
             * Gets the duration of the transition.
             * <p>
             * In most cases, the transition duration is one hour, however this is not always the case.
             * The duration will be positive for a gap and negative for an overlap.
             * Time-zones are second-based, so the nanosecond part of the duration will be zero.
             *
             * @return the duration of the transition, positive for gaps, negative for overlaps
             */
             gint duration() const;

            /**
             * Does this transition represent a gap in the local time-line.
             * <p>
             * Gaps occur where there are local date-times that simply do not exist.
             * An example would be when the offset changes from <b> +01:00} to <b> +02:00}.
             * This might be described as 'the clocks will move forward one hour tonight at 1am'.
             *
             * @return true if this transition is a gap, false if it is an overlap
             */
             gbool isGap() const;

            /**
             * Does this transition represent an overlap in the local time-line.
             * <p>
             * Overlaps occur where there are local date-times that exist twice.
             * An example would be when the offset changes from <b> +02:00} to <b> +01:00}.
             * This might be described as 'the clocks will move back one hour tonight at 2am'.
             *
             * @return true if this transition is an overlap, false if it is a gap
             */
             gbool isOverlap() const;

            /**
             * Checks if the specified offset is valid during this transition.
             * <p>
             * This checks to see if the given offset will be valid at some point in the transition.
             * A gap will always return false.
             * An overlap will return true if the offset is either the before or after offset.
             *
             * @param offset  the offset to check, null returns false
             * @return true if the offset is valid during the transition
             */
             gbool isValidOffset(const ZoneOffset& offset) const;

            /**
             * Gets the valid offsets during this transition.
             * <p>
             * A gap will return an empty list, while an overlap will return both offsets.
             *
             * @return the list of valid offsets
             */
             Array<ZoneOffset> validOffsets() const;

            //-----------------------------------------------------------------------
            /**
             * Compares this transition to another based on the transition instant.
             * <p>
             * This compares the instants of each transition.
             * The offsets are ignored, making this order inconsistent with equals.
             *
             * @param transition  the transition to compare to
             * @return the comparator value, negative if less, positive if greater
             */
            gint compareTo(const ZoneOffsetTransition &transition) const override;

            //-----------------------------------------------------------------------
            /**
             * Checks if this object equals another.
             * <p>
             * The entire state of the object is compared.
             *
             * @param other  the other object to compare to, null returns false
             * @return true if equal
             */
            gbool equals(const Object &o) const override;

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

            /**
             * Return shadow copy of this transition
             */
            Object &clone() const override;

        };

    } // util
} // core

#endif //CORE23_ZONEOFFSETTRANSITION_H
