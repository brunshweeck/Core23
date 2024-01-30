//
// Created by T.N.Brunshweeck on 25/12/2023.
//

#ifndef CORE23_ZONEREGION_H
#define CORE23_ZONEREGION_H

#include <core/time/ZoneID.h>
#include <core/time/ZoneRules.h>

namespace core {
    namespace time {
        /**
         * A geographical region where the same time-zone rules apply.
         * <p>
         * Time-zone information is categorized as a setValue of rules defining when and
         * how the offset from UTC/Greenwich changes. These rules are accessed using
         * identifiers based on geographical regions, such as countries or states.
         * The most common region classification is the Time ZoneID Database (TzDB),
         * which defines regions such as 'Europe/Paris' and 'Asia/Tokyo'.
         * <p>
         * The region identifier, modeled by this class, is distinct from the
         * underlying rules, modeled by <b style="color:orange;"> ZoneRules}.
         * The rules are defined by governments and change frequently.
         * By contrast, the region identifier is well-defined and long-lived.
         * This separation also allows rules to be shared between regions if appropriate.
         *
         * @implSpec
         * This class is immutable and thread-safe.
         */
        class ZoneRegion CORE_FINAL : public ZoneID {
        private:
            /**
             * The time-zone ID.
             */
            String ID;

            ZoneRules zRules;

            /**
             * Obtains an INSTANCE of <b> ZoneId} from an identifier.
             *
             * @param ID  the time-zone ID
             * @param checkAvailable  whether to check if the zone ID is available
             * @return the zone ID
             * @throws DateTimeException if the ID format is invalid
             * @throws ZoneRulesException if checking availability and the ID cannot be found
             */
            static ZoneRegion of(const String &ID, gbool checkAvailable);

            /**
             * Checks that the given string is a legal Zone-ID name.
             *
             * @param ID  the time-zone ID
             * @throws DateTimeException if the ID format is invalid
             */
            static void checkName(const String &ID);

            //-------------------------------------------------------------------------
            /**
             * Constructor.
             *
             * @param ID  the time-zone ID
             * @param rules  the rules, null for lazy lookup
             */
            CORE_EXPLICIT ZoneRegion(const String &ID, const ZoneRules &rules);

            friend ZoneID;

        public:
            //-----------------------------------------------------------------------
            ZoneRegion of(const String &ID);

            ZoneRegion(const ZoneRegion &region);

            ZoneRegion(ZoneRegion &&region) CORE_NOTHROW;

            ZoneRegion &operator=(const ZoneRegion &region);

            ZoneRegion &operator=(ZoneRegion &&region) CORE_NOTHROW;

            //-----------------------------------------------------------------------
            /**
             * @override
             */
            String id() const override;

            /**
             * @override
             */
            ZoneRules rules() const override;

            ZoneOffset offset(glong epochSecond) const override;
        };
    } // time
} // core

#endif //CORE23_ZONEREGION_H
