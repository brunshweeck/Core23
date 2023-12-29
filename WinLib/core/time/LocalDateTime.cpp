//
// Created by T.N.Brunshweeck on 27/12/2023.
//

#include <core/time/LocalDateTime.h>
#include <core/time/ZoneOffset.h>
#include <Windows.h>


namespace core {
    namespace time {
        LocalDateTime LocalDateTime::now() {
            SYSTEMTIME sys = {};
            GetLocalTime(&sys);
            return LocalDateTime(sys.wYear, sys.wMonth, sys.wDay,
                                 sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds * 1000000);
        }

        LocalDateTime LocalDateTime::now(const ZoneID &zone) {
            LocalDateTime const utc = now();
            glong const epochSecond = utc.toLocalDate().toEpochDay() * LocalTime::SECONDS_PER_DAY
                                + utc.toLocalTime().getLong(SECOND_OF_DAY);
            gint const nano = utc.nano();
            return ofEpochSecond(epochSecond, nano, zone.rules().offset(epochSecond));
        }

    }
}
