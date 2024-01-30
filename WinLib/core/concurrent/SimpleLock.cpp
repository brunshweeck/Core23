//
// Created by T.N.Brunshweeck on 26/01/2024.
//

#include <core/concurrent/SimpleLock.h>
#include <Windows.h>

namespace core {
    namespace concurrent {


        namespace {
            AtomicLong ID = 0;
        }

        void SimpleLock::initialLock0() {
            HANDLE handle = GetCurrentThread();
            owner.setOpaque((glong) handle);
        }

        namespace {
            HANDLE (*EventCreator)() =
            []() -> HANDLE {
                String const id = "SimpleLock#" + Long::toUnsignedString(ID.incrementAndGet());
                CharArray ca = id.chars();
                HANDLE handle = NULL;
#ifndef CORE_SYSTEM_WINRT
                handle = CreateEventW(NULL, FALSE, FALSE, (LPCWSTR) &ca[0]);
                if (handle == INVALID_HANDLE_VALUE) {
                    ID.decrementAndGet();
                    handle = CreateEventW(NULL, FALSE, FALSE, NULL);
                }
#else
                handle = CreateEventExW(NULL, (LPCWSTR) &ca[0], 0, EVENT_ALL_ACCESS);
            if (handle == INVALID_HANDLE_VALUE) {
                ID.decrementAndGet();
                handle = CreateEventExW(NULL, (LPCWSTR) &ca[0], 0, EVENT_ALL_ACCESS);
            }
#endif
                return handle;
            };

            gbool (*EventWaiter)(HANDLE event, const Duration &duration) =
            [](HANDLE event, const Duration &duration) -> gbool {
#ifndef CORE_SYSTEM_WINRT
                return WaitForSingleObject(event, duration.toMillis()) == WAIT_OBJECT_0;
#else
                return WaitForSingleObjectEx(event, duration.toMillis(), FALSE) == WAIT_OBJECT_0;
#endif
            };
        }

        gbool SimpleLock::tryLock0(const Duration &duration) {
            if (!duration.isPositive()) {
                return false;
            }
            while (!initialLock()) {
                SyncState const oldState = (SyncState) state.getAcquire();
                switch (oldState) {
                    case UNLOCKED:
                    case LOCKED:
                        break;
                    default:
                        return false;
                }
                HANDLE handle = (HANDLE) holder.getOpaque(0);
                // assert handle != NULL
                Duration copy = duration;
                CORE_FAST glong const MAX_SECONDS = INFINITE / LocalTime::MILLIS_PER_SECOND;
                if (copy.toSeconds() > MAX_SECONDS) {
                    copy = Duration::ofMillis(INFINITE);
                }
            }
        }

        //
    }
}
