//
// Created by T.N.Brunshweeck on 24/12/2023.
//

#include "ZoneID.h"
#include <core/util/HashMap.h>
#include <core/time/ZoneOffset.h>
#include <core/time/ZoneRegion.h>

#include "core/util/HashSet.h"

namespace core {
    namespace time {
        using util::Map;
        using util::HashMap;

        CORE_ALIAS(U, native::Unsafe);

        namespace {
            struct entry CORE_FINAL {
                String key;
                String value;

                CORE_EXPLICIT entry(const String &key, const String &value) : key(key), value(value) {
                }
            };

            template<class... Entries>
            Map<String, String> &ofEntries(Entries &&... entries) {
                HashMap<String, String> &ids = U::createInstance<HashMap<String, String> >();
                entry const fbe = entry({}, {});
                gint eSize = sizeof...(entries);
                for (int i = 0; i < eSize; ++i) {
                    Class<int>::isConvertible<int>();
                    entry const e = Class<entry>::valueExactAt(i + 1, fbe, U::moveInstance(entries)...);
                    ids.put(e.key, e.value);
                }
                return ids;
            }
        }

        Map<String, String> const &ZoneID::ZIDs = ofEntries(
            entry("ACT", "Australia/Darwin"),
            entry("AET", "Australia/Sydney"),
            entry("AGT", "America/Argentina/Buenos_Aires"),
            entry("ART", "Africa/Cairo"),
            entry("AST", "America/Anchorage"),
            entry("BET", "America/Sao_Paulo"),
            entry("BST", "Asia/Dhaka"),
            entry("CAT", "Africa/Harare"),
            entry("CNT", "America/St_Johns"),
            entry("CST", "America/Chicago"),
            entry("CTT", "Asia/Shanghai"),
            entry("EAT", "Africa/Addis_Ababa"),
            entry("ECT", "Europe/Paris"),
            entry("IET", "America/Indiana/Indianapolis"),
            entry("IST", "Asia/Kolkata"),
            entry("JST", "Asia/Tokyo"),
            entry("MIT", "Pacific/Apia"),
            entry("NET", "Asia/Yerevan"),
            entry("NST", "Pacific/Auckland"),
            entry("PLT", "Asia/Karachi"),
            entry("PNT", "America/Phoenix"),
            entry("PRT", "America/Puerto_Rico"),
            entry("PST", "America/Los_Angeles"),
            entry("SST", "Pacific/Guadalcanal"),
            entry("VST", "Asia/Ho_Chi_Minh"),
            entry("EST", "-05:00"),
            entry("MST", "-07:00"),
            entry("HST", "-10:00")
        );

        HashMap<String, ZoneID> ZoneID::ID_CACHE = HashMap<String, ZoneID>(16, 0.75);

        gbool ZoneID::equals(const Object &obj) const {
            if (this == &obj) {
                return true;
            }
            return (Class<ZoneID>::hasInstance(obj))
                   && id().equals(((ZoneID &) obj).id());
        }

        gint ZoneID::hash() const {
            return id().hash();
        }

        String ZoneID::toString() const {
            return id();
        }

        String ZoneID::displayName(DateTimeFormatter::TextStyle style, const util::Locale &locale) const {
            return id();
        }

        glong ZoneID::until(const Temporal & /*endExclusive*/, Temporal::TemporalUnit /*unit*/) const {
            UnsupportedMethodException().throws(__trace("core.time.ZoneID"));
        }

        gint ZoneID::get(TemporalField /*field*/) const {
            UnsupportedMethodException().throws(__trace("core.time.ZoneID"));
        }

        glong ZoneID::getLong(TemporalField /*field*/) const {
            UnsupportedMethodException().throws(__trace("core.time.ZoneID"));
        }

        const util::Set<String> &ZoneID::availableZones() {
            return ZIDs.keySet();
        }

        const ZoneID &ZoneID::of(const String &id, const Map<String, String> &aliasMap) {
            try {
                String ID;
                if (aliasMap.containsKey(id)) {
                    ID = aliasMap.get(id);
                } else {
                    ID = id;
                }
                return ZoneID::of(id);
            } catch (const Exception &ex) { ex.throws(__trace("core.time.ZoneID")); }
        }

        const ZoneID &ZoneID::of(const String &zoneId) {
            try {
                if (zoneId.length() <= 1 || zoneId.startsWith("+") || zoneId.startsWith("-")) {
                    ZoneOffset const offset = ZoneOffset::of(zoneId);
                    return ID_CACHE.putIfAbsent(offset.id(), offset);
                } else {
                    gbool const checkAvailable = true;
                    gint prefixLength = 0;
                    gbool isOffset = false;
                    if (zoneId.startsWith("UTC") || zoneId.startsWith("GMT")) {
                        prefixLength = 3;
                        isOffset = true;
                    } else if (zoneId.startsWith("UT")) {
                        prefixLength = 2;
                        isOffset = true;
                    }
                    if (!isOffset)
                        return native::Unsafe::copyInstance(ZoneRegion::of(zoneId, true));

                    String const prefix = zoneId.subString(0, prefixLength);
                    if (zoneId.length() == prefixLength) {
                        return of(prefix, ZoneOffset::UTC);
                    }
                    if (zoneId.charAt(prefixLength) != '+' && zoneId.charAt(prefixLength) != '-') {
                        return native::Unsafe::copyInstance(ZoneRegion::of(zoneId, true));
                        // drop through to ZoneRulesProvider
                    }
                    ZoneOffset const offset = ZoneOffset::of(zoneId.subString(prefixLength));
                    if (offset == ZoneOffset::UTC) {
                        return of(prefix, offset);
                    }
                    return of(prefix, offset);
                }
            } catch (const DateTimeException &ex) {
                DateTimeException("Invalid ID for offset-based ZoneId: " + zoneId, ex)
                        .throws(__trace("core.time.ZoneID"));
            }
        }

        const ZoneID &ZoneID::of(const String &prefix, const ZoneOffset &offset) {
            if (prefix.isEmpty()) {
                return ID_CACHE.putIfAbsent(offset.id(), offset);
            }
            if (!prefix.equals("GMT") && !prefix.equals("UTC") && !prefix.equals("UT")) {
                ArgumentException("prefix should be GMT, UTC or UT, is: " + prefix)
                        .throws(__trace("core.time.ZoneID"));
            }

            ZoneRegion region = offset.totalSeconds() != 0
                                    ? ZoneRegion(prefix + offset.id(), offset.rules())
                                    : ZoneRegion(prefix, offset.rules());
            return ID_CACHE.putIfAbsent(region.id(), region);
        }

        const ZoneID &ZoneID::from(const Temporal &temporal) {
            Object const &obj = temporal.query(ZONE_ID);
            if (obj == null) {
                Object const &obj2 = temporal.query(OFFSET);
                if (obj2 == null) {
                    DateTimeException("Unable to obtain ZoneId from TemporalAccessor: " +
                                      temporal.toString() + " of type " + temporal.classname())
                            .throws(__trace("core.time.ZoneID"));
                }
                return (const ZoneID &) obj2;
            }
            return (const ZoneID &) obj;
        }
    } // time
} // core
