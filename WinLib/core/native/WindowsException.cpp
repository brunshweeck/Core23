//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#include "WindowsException.h"
#include <core/private/Unsafe.h>
#include <core/io/private/NativeBuffer.h>
#include <Windows.h>

namespace core {
    namespace native {

        using namespace io;

        WindowsException::WindowsException(gint lastError) : error(lastError) {}

        WindowsException::WindowsException(String msg) : errorMsg(Unsafe::moveInstance(msg)) {}

        gint WindowsException::lastError() const {
            return error;
        }

        String WindowsException::errorString() const {
            if (!errorMsg.isEmpty())
                return errorMsg;
            if(lastError() == 0)
                return "";
            NativeBuffer const buffer{512 * 2};
            DWORD size = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                        NULL, lastError(), MAKELANGID(LANG_USER_DEFAULT, SUBLANG_DEFAULT),
                                        (LPWSTR) buffer.address(), 512, NULL);
            if (size != 0){
                gint n = (gint) size;
                while(n > 0 && ((LPCWSTR) buffer.address())[n-1] == L'\n')
                    n -= 1;
                while(n > 0 && ((LPCWSTR) buffer.address())[n-1] == L'\r')
                    n -= 1;
                while(n > 0 && ((LPCWSTR) buffer.address())[n-1] == L'\t')
                    n -= 1;
                (String& ) errorMsg = String((LPCWSTR) buffer.address(), 0, n);
                return errorMsg;
            }
            size = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                  NULL, lastError(), MAKELANGID(LANG_SYSTEM_DEFAULT, SUBLANG_DEFAULT),
                                  (LPWSTR) buffer.address(), 512, NULL);
            if (size != 0){
                gint n = (gint) size;
                while(n > 0 && ((LPCWSTR) buffer.address())[n-1] == L'\n')
                    n -= 1;
                while(n > 0 && ((LPCWSTR) buffer.address())[n-1] == L'\r')
                    n -= 1;
                while(n > 0 && ((LPCWSTR) buffer.address())[n-1] == L'\t')
                    n -= 1;
                (String& ) errorMsg = String((LPCWSTR) buffer.address(), 0, n);
                return errorMsg;
            }
            return "code: " + String::valueOf(lastError());
        }

        String WindowsException::message() const {
            return Exception::message();
        }

        void WindowsException::throwAsIOException(const String &file) const {
            NativeBuffer const buffer = NativeBuffer(1024);
            throw IOException(errorString());
        }

        void WindowsException::throwAsIOException(const String &file, const String &other) const {
            throw IOException(errorString());
        }

        void WindowsException::throwAsIOException(const String &file, const Trace &trace) const {
            IOException(errorString()).throws(trace);
        }

        void WindowsException::throwAsIOException(const String &file, const String &other, const Trace &trace) const {
            IOException(errorString()).throws(trace);
        }
    } // core
} // private