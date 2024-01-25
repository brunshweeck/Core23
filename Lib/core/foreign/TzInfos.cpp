// 
// Created by T.N.Brunshweeck on 30/12/2023.
// 

#include <core/time/LocalDate.h>
#include <core/time/LocalTime.h>
#include <core/time/LocalDateTime.h>
#include <core/time/ZoneRules.h>
#include <core/time/ZoneOffset.h>
#include <core/time/ZoneRegion.h>
#include <core/time/ZoneOffsetTransition.h>
#include <core/time/ZoneOffsetTransitionRule.h>
#include <core/util/HashMap.h>
#include <core/native/LongArray.h>


using namespace core;
using namespace core::time;
using namespace core::util;
using namespace core::native;


namespace core {
    namespace time {
        const HashMap <String, ZoneRules> ZoneID::tzInfos = []() -> HashMap <String, ZoneRules> {

            HashMap<String, ZoneRules> tz{1024};
            return tz;
        }();
    }
}
