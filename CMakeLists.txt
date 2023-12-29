CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

PROJECT(Core23 VERSION 2023.6.5 LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Adding compatible lib folder
IF (WIN32)
#    The Operating System is Windows (Vista, MS-DOS, MS-Windows, ...)
    SET(SYSTEM_LIBDIR WinLib)
    IF (MINGW)
        ADD_COMPILE_OPTIONS(--save-temps)
    ENDIF ()
ELSE ()
#    The Operating System is Unix (Unix, Linux, Apple, Android, ...)
    SET(SYSTEM_LIBDIR UnixLib)
    ADD_COMPILE_OPTIONS(--save-temps)
ENDIF ()

FILE(GLOB Core23
##=====[Common files]==============##
        Lib/core/*.h
        Lib/core/*.c??
        Lib/core/foreign/*.h
        Lib/core/foreign/*.c??
        Lib/core/io/*.h
        Lib/core/io/*.c??
        Lib/core/io/charset/*.h
        Lib/core/io/charset/*.c??
        Lib/core/io/native/*.h
        Lib/core/io/native/*.c??
        Lib/core/math/*.h
        Lib/core/math/*.c??
        Lib/core/net/*.h
        Lib/core/net/*.c??
        Lib/core/native/*.h
        Lib/core/native/*.c??
        Lib/core/private/*.h
        Lib/core/private/*.c??
        Lib/core/sql/*.h
        Lib/core/sql/*.c??
        Lib/core/text/*.h
        Lib/core/text/*.c??
        Lib/core/time/*.h
        Lib/core/time/*.c??
        Lib/core/util/*.h
        Lib/core/util/*.c??
        Lib/core/util/concurrent/*.h
        Lib/core/util/concurrent/*.c??
        Lib/core/util/function/*.h
        Lib/core/util/function/*.c??
        Lib/core/util/random/*.h
        Lib/core/util/random/*.c??
        Lib/core/util/regex/*.h
        Lib/core/util/regex/*.c??

##=============[System files]================##
        ${SYSTEM_LIBDIR}/core/native/*.h
        ${SYSTEM_LIBDIR}/core/native/*.c??
        ${SYSTEM_LIBDIR}/core/private/*.h
        ${SYSTEM_LIBDIR}/core/private/*.c??
        ${SYSTEM_LIBDIR}/core/time/*.h
        ${SYSTEM_LIBDIR}/core/time/*.c??
)

ADD_LIBRARY(Core23 SHARED ${Core23}
        Lib/core/io/native/NativeBuffer.cpp
        Lib/core/io/native/NativeBuffer.h
        Lib/core/AutoClosable.h
        Lib/core/io/FileDescriptor.cpp
        Lib/core/io/FileDescriptor.h
        Lib/core/io/Closeable.cpp
        Lib/core/io/Closeable.h
        Lib/core/io/FileArray.cpp
        Lib/core/io/FileArray.h)

INCLUDE_DIRECTORIES("Lib")


# This file is called to realize tests on Core23 library
ADD_EXECUTABLE(Main23 Main.cpp)

TARGET_LINK_LIBRARIES(Main23 PUBLIC Core23)

SET(CMAKE_GNUtoMS ON)
SET(CMAKE_AIX_EXPORT_ALL_SYMBOLS  ON)
