//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "Chronology.h"
#include <core/time/LocalDateTime.h>

namespace core {
    namespace time {

        Chronology &Chronology::from(const Temporal &temporal) {
            Object const& chronology = temporal.query(Temporal::CHRONOLOGY);
            if(chronology == null) {
                CORE_UNREACHABLE();
            }
            return (Chronology &) chronology;
        }

        Chronology &Chronology::ofLocale(const Locale &locale) {
            CORE_UNREACHABLE();
        }

        Chronology &Chronology::of(const String &id) {
            CORE_UNREACHABLE();
        }
    } // core
} // time