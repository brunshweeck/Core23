//
// Created by T.N.Brunshweeck on 25/01/2024.
//

#include <core/time/Chrono.h>
#include <core/time/LocalDateTime.h>
#include <core/time/ZoneOffset.h>
#include <core/InternalError.h>
#include <Windows.h>

namespace core {
    namespace time {

        void Chrono::start0() {
            CORE_ALIAS(MethodHandle, void(*)());
            CORE_ALIAS(MethodGetter, glong(*)());
            CORE_ALIAS(MethodGetter2, glong(*)(glong tickCount));
            static gbool isResolved = false;
            static glong FREQ = {};
            static gbool useWin32API = true;

            static MethodHandle ResolveFreq = []() -> void {
                gbool const resolved = isResolved;
                if (resolved)
                    return;
                LARGE_INTEGER freq;
                BOOL const result = QueryPerformanceFrequency(&freq);
                isResolved = true;
                if (result != FALSE)
                    FREQ = freq.QuadPart;
                else
                    FREQ = 0;
            };

            static MethodGetter2 GetSecondsFromTick = [](glong tick) -> glong {
                if (FREQ != 0) {
                    if (tick > 0) {
                        return Math::floorDiv(tick, FREQ);
                    } else {
                        ULONGLONG const uTick = (ULONGLONG) tick;
                        return (glong) (uTick / FREQ);
                    }
                } else if (tick > 0) {
                    return Math::floorDiv(tick, LocalTime::MILLIS_PER_SECOND);
                } else {
                    ULONGLONG const uTick = (ULONGLONG) tick;
                    return (glong) (uTick / LocalTime::MILLIS_PER_SECOND);
                }
            };

            static MethodGetter2 GetNanosFromTick = [](glong tick) -> glong {
                if (FREQ != 0) {
                    if (tick > 0) {
                        return Math::floorMod(tick, FREQ) * LocalTime::NANOS_PER_MICRO;
                    } else {
                        ULONGLONG const uTick = (ULONGLONG) tick;
                        return (glong) (uTick % FREQ) * LocalTime::NANOS_PER_MICRO;
                    }
                } else if (tick > 0) {
                    return Math::floorMod(tick, LocalTime::MILLIS_PER_SECOND) * LocalTime::NANOS_PER_MILLI;
                } else {
                    ULONGLONG const uTick = (ULONGLONG) tick;
                    return (glong) (uTick % LocalTime::MILLIS_PER_SECOND) * LocalTime::NANOS_PER_MILLI;
                }
            };

            static MethodGetter GetTick = []() -> glong {
                ResolveFreq();
                if (FREQ != 0) {
                    LARGE_INTEGER counter = {};
                    BOOL const result = QueryPerformanceCounter(&counter);
                    if (result == FALSE)
                        InternalError("Could not obtain current Tick count")
                                .throws(__ftrace("core.time.Chrono"));
                    return counter.QuadPart;
                }
                return (glong) GetTickCount64();
            };


            CORE_TRY_RETHROW
            (if (useWin32API) {
                glong const currentTick = GetTick();
                glong const newSeconds = GetSecondsFromTick(currentTick);
                gint const newNanos = (gint) GetNanosFromTick(currentTick);
                seconds = newSeconds;
                nanos = newNanos;
            } else {
                LocalDateTime const ldt = LocalDateTime::now();
                seconds = ldt.toEpochSecond(ZoneOffset::UTC);
                nanos = ldt.nano();
            }, {
                 if (ResolveFreq && FREQ == 0) {
                     useWin32API = false;
                     LocalDateTime const ldt = LocalDateTime::now();
                     seconds = ldt.toEpochSecond(ZoneOffset::UTC);
                     nanos = ldt.nano();
                     return;
                 }
             }, __trace("core.time.Chrono"))
        }
    }
}
