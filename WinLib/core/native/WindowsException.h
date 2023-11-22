//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#ifndef CORE23_WINDOWSEXCEPTION_H
#define CORE23_WINDOWSEXCEPTION_H

#include <core/String.h>
#include <core/io/IOException.h>

namespace core {
    namespace native {

        class WindowsException: public Exception {

            gint error = 0;
            String msg;

        public:
            CORE_EXPLICIT WindowsException(gint lastError);

            CORE_EXPLICIT WindowsException(String msg);

            gint lastError() const;

            String errorString() const;

            String message() const override;

            IOException& asIOException(String file) const;

            IOException& asIOException(String file, String other) const;

            /**
             * Enumeration of basic error codes
             */
            enum BasicError {
                FILE_NOT_FOUND        = 2,
                PATH_NOT_FOUND        = 3,
                ACCESS_DENIED         = 5,
                INVALID_HANDLE        = 6,
                INVALID_DATA          = 13,
                NOT_SAME_DEVICE       = 17,
                NOT_READY             = 21,
                SHARING_VIOLATION     = 32,
                FILE_EXISTS           = 80,
                INVALID_PARAMETER     = 87,
                DISK_FULL             = 112,
                INSUFFICIENT_BUFFER   = 122,
                INVALID_LEVEL         = 124,
                DIR_NOT_ROOT          = 144,
                DIR_NOT_EMPTY         = 145,
                ALREADY_EXISTS        = 183,
                MORE_DATA             = 234,
                DIRECTORY             = 267,
                NOTIFY_ENUM_DIR       = 1022,
                PRIVILEGE_NOT_HELD    = 1314,
                NONE_MAPPED           = 1332,
                CANT_ACCESS_FILE      = 1920,
                NOT_A_REPARSE_POINT   = 4390,
                INVALID_REPARSE_DATA  = 4392,
            };
        };

    } // core
} // native

#endif //CORE23_WINDOWSEXCEPTION_H
