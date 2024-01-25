//
// Created by T.N.Brunshweeck on 26/12/2023.
//

#include <core/time/ZoneID.h>
#include <core/util/HashMap.h>
#include <Windows.h>
#include <core/native/CharArray.h>

#include "core/time/ZoneRegion.h"

namespace core {
    namespace time {
        namespace {
            CORE_FAST WCHAR tzRegPath[] = LR"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Time Zones)";
            CORE_FAST WCHAR currentTzRegPath[] = LR"(SYSTEM\CurrentControlSet\Control\TimeZoneInformation)";
            CORE_FAST WCHAR tzSubKey[] = L"TimeZoneKeyName";
            ZoneID *systemID = {};
        }

        const ZoneID &ZoneID::systemDefault() {
            if (systemID != null)
                return *systemID;
            HKEY key = null;
            LONG result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, currentTzRegPath, 0, KEY_READ, &key);
            if (result != ERROR_SUCCESS) {
                key = null;
                // Get the size and type of the value.
                DWORD type = REG_NONE;
                DWORD size = 0;
                result = RegQueryValueExW(key, tzSubKey, null, &type, nullptr, &size);
                if (result == ERROR_SUCCESS) {
                    // Workaround for rare cases where the trailing '\0' is missing.
                    if (type == REG_SZ)
                        size += 2;
                    CharArray array = CharArray((gint) size, 0);
                    result = RegQueryValueExW(key, tzSubKey, null, null, (LPBYTE) &array[0], &size);
                    if (result == ERROR_SUCCESS) {
                        switch (type) {
                            case REG_SZ:
                                if (size > 0) {
                                    String const ID = String(array, 0, (gint) size);
                                    systemID = &ZoneID::of(ID);
                                    return *systemID;
                                }
                            default:
                                break;
                        }
                    }
                }
            }
        SecondSolution:
            // check from utc
            TIME_ZONE_INFORMATION tzInfo = {};
            GetTimeZoneInformation(&tzInfo);
            if (tzInfo.StandardName[0] != 0)
                return ZoneID::of(tzInfo.StandardName+0);
            return ZoneID::of(tzInfo.DaylightName+0);
        }
    }
} // core::time
