//
// Created by T.N.Brunshweeck on 27/12/2023.
//

#include <core/time/LocalDateTime.h>
#include <core/time/ZoneOffset.h>
#include <time.h>
#include <sys/time.h>


namespace core {
    namespace time {
        LocalDateTime LocalDateTime::now() {
            time_t t;
            localtime(&t);
            return ofEpochSecond(t);
        }

        LocalDateTime LocalDateTime::now(const ZoneID &zone) {
            time_t t;
            localtime(&t);
            t += zone.getRules().getOffset(t).totalSeconds();
            return ofEpochSecond(t);
        }
        
    }
}
