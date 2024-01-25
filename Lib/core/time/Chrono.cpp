//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "Chrono.h"

namespace core {
    namespace time {
        Chrono::Chrono() : seconds(-1), nanos(0) {}

        Chrono::Chrono(gbool autoStart): Chrono() {}

        glong Chrono::start() {
            if(!isStarted()) {
                return seconds;
            }
        }

        gbool Chrono::isStarted() const {
            return seconds != -1;
        }

        glong Chrono::elapsedTime() const {
            return elapsedTime(ChronoUnit::MILLIS);
        }

        glong Chrono::elapsedTime(Temporal::ChronoUnit unit) const {
            if(!isStarted())
                return -1;
        }

        gbool Chrono::hasExpired(glong time) const {
            return hasExpired(time, ChronoUnit::MILLIS);
        }

        gbool Chrono::hasExpired(glong time, Temporal::ChronoUnit unit) const {
            //
        }

        gbool Chrono::supportUnit(Temporal::Unit unit) const {
            //
        }

        glong Chrono::until(const Temporal &endExclusive, Temporal::ChronoUnit unit) const {
            //
        }

        gint Chrono::get(Temporal::ChronoField field) const {
            //
        }

        glong Chrono::getLong(Temporal::ChronoField field) const {
            //
        }

        gint Chrono::compareTo(const Chrono &other) const {
            //
        }

        gint Chrono::hash() const {
            //
        }

        gbool Chrono::equals(const Object &o) const {
            //
        }

        String Chrono::toString() const {
            //
        }

        Object &Chrono::clone() const {
            //
        }
    } // core
} // time
