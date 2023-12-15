CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

PROJECT(Core23 VERSION 2023.6.5 LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Adding compatible lib folder
IF (WIN32)
#    The Operating System is Windows (Vista, MS-DOS, MS-Windows, ...)
    SET(SYSTEM_LIBDIR WinLib)
ELSE ()
#    The Operating System is Unix (Unix, Linux, Apple, Android, ...)
    SET(SYSTEM_LIBDIR UnixLib)
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
)

ADD_LIBRARY(Core23 STATIC ${Core23}
        Lib/core/util/SortedStruct.cpp
        Lib/core/util/SortedStruct.h
        Lib/core/util/StringTokenizer.cpp
        Lib/core/util/StringTokenizer.h
        Lib/core/native/StringArray.cpp
        Lib/core/native/StringArray.h)

INCLUDE_DIRECTORIES("Lib")


# This file is called to realize tests on Core23 library
ADD_EXECUTABLE(Main23 Main.cpp)

TARGET_LINK_LIBRARIES(Main23 PUBLIC Core23)

