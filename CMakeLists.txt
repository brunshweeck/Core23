CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

PROJECT(Core23 VERSION 2023.6.5 LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
SET(PROJECT_LIBDIR Lib)

# Adding compatible lib folder

IF (MINGW)
    #    The operating System is Windows Witch Unix library
    SET(SYSTEM_LIBDIR WinLib)
    #    SET(SYSTEM_LIBDIR UnixLib)
#    ADD_COMPILE_OPTIONS(--save-temps)
ELSEIF (WIN32)
    #    The Operating System is Windows (Vista, MS-DOS, MS-Windows, ...)
    SET(SYSTEM_LIBDIR WinLib)
    IF (MSVC)
        ADD_COMPILE_OPTIONS(/constexpr:depth64)
    ENDIF (MSVC)
ELSE ()
    #    The Operating System is Unix (Unix, Linux, Apple, Android, ...)
    SET(SYSTEM_LIBDIR UnixLib)
#    ADD_COMPILE_OPTIONS(--save-temps)
    IF (LINUX)
        MESSAGE(NOTICE "LINUX System")
    ELSEIF (APPLE)
        MESSAGE(NOTICE "APPLE System")
    ELSEIF (BSD)
        MESSAGE(NOTICE "BSD System")
    ELSEIF (Unix)
        MESSAGE(NOTICE "UNIX System")
    ELSE ()
        MESSAGE(NOTICE "WSL/Other Unix System")
    ENDIF ()
ENDIF ()

FILE(GLOB Core23
        ## ========= [project files] ============== ##
        ${PROJECT_LIBDIR}/core/*.h
        ${PROJECT_LIBDIR}/core/*.c??
        ${PROJECT_LIBDIR}/core/private/*.h
        ${PROJECT_LIBDIR}/core/private/*.c??
        ${PROJECT_LIBDIR}/core/foreign/*.h
        ${PROJECT_LIBDIR}/core/foreign/*.c??
        ${PROJECT_LIBDIR}/core/foreign/private/*.h
        ${PROJECT_LIBDIR}/core/foreign/private/*.c??
        ${PROJECT_LIBDIR}/core/io/*.h
        ${PROJECT_LIBDIR}/core/io/*.c??
        ${PROJECT_LIBDIR}/core/io/private/*.h
        ${PROJECT_LIBDIR}/core/io/private/*.c??
        ${PROJECT_LIBDIR}/core/charset/*.h
        ${PROJECT_LIBDIR}/core/charset/*.c??
        ${PROJECT_LIBDIR}/core/charset/private/*.h
        ${PROJECT_LIBDIR}/core/charset/private/*.c??
        ${PROJECT_LIBDIR}/core/math/*.h
        ${PROJECT_LIBDIR}/core/math/*.c??
        ${PROJECT_LIBDIR}/core/math/private/*.h
        ${PROJECT_LIBDIR}/core/math/private/*.c??
        ${PROJECT_LIBDIR}/core/net/*.h
        ${PROJECT_LIBDIR}/core/net/*.c??
        ${PROJECT_LIBDIR}/core/net/private/*.h
        ${PROJECT_LIBDIR}/core/net/private/*.c??
        ${PROJECT_LIBDIR}/core/native/*.h
        ${PROJECT_LIBDIR}/core/native/*.c??
        ${PROJECT_LIBDIR}/core/native/private/*.h
        ${PROJECT_LIBDIR}/core/native/private/*.c??
        ${PROJECT_LIBDIR}/core/sql/*.h
        ${PROJECT_LIBDIR}/core/sql/*.c??
        ${PROJECT_LIBDIR}/core/sql/private/*.h
        ${PROJECT_LIBDIR}/core/sql/private/*.c??
        ${PROJECT_LIBDIR}/core/text/*.h
        ${PROJECT_LIBDIR}/core/text/*.c??
        ${PROJECT_LIBDIR}/core/text/private/*.h
        ${PROJECT_LIBDIR}/core/text/private/*.c??
        ${PROJECT_LIBDIR}/core/time/*.h
        ${PROJECT_LIBDIR}/core/time/*.c??
        ${PROJECT_LIBDIR}/core/time/private/*.h
        ${PROJECT_LIBDIR}/core/time/private/*.c??
        ${PROJECT_LIBDIR}/core/util/*.h
        ${PROJECT_LIBDIR}/core/util/*.c??
        ${PROJECT_LIBDIR}/core/util/private/*.h
        ${PROJECT_LIBDIR}/core/util/private/*.c??
        ${PROJECT_LIBDIR}/core/concurrent/*.h
        ${PROJECT_LIBDIR}/core/concurrent/*.c??
        ${PROJECT_LIBDIR}/core/concurrent/private/*.h
        ${PROJECT_LIBDIR}/core/concurrent/private/*.c??
        ${PROJECT_LIBDIR}/core/function/*.h
        ${PROJECT_LIBDIR}/core/function/*.c??
        ${PROJECT_LIBDIR}/core/function/private/*.h
        ${PROJECT_LIBDIR}/core/function/private/*.c??
        ${PROJECT_LIBDIR}/core/random/*.h
        ${PROJECT_LIBDIR}/core/random/*.c??
        ${PROJECT_LIBDIR}/core/random/private/*.h
        ${PROJECT_LIBDIR}/core/random/private/*.c??
        ${PROJECT_LIBDIR}/core/regex/*.h
        ${PROJECT_LIBDIR}/core/regex/*.c??
        ${PROJECT_LIBDIR}/core/regex/private/*.h
        ${PROJECT_LIBDIR}/core/regex/private/*.c??

        ## ============= [System files] ================ ##
        ${SYSTEM_LIBDIR}/core/*.h
        ${SYSTEM_LIBDIR}/core/*.c??
        ${SYSTEM_LIBDIR}/core/private/*.h
        ${SYSTEM_LIBDIR}/core/private/*.c??
        ${SYSTEM_LIBDIR}/core/foreign/*.h
        ${SYSTEM_LIBDIR}/core/foreign/*.c??
        ${SYSTEM_LIBDIR}/core/foreign/private/*.h
        ${SYSTEM_LIBDIR}/core/foreign/private/*.c??
        ${SYSTEM_LIBDIR}/core/io/*.h
        ${SYSTEM_LIBDIR}/core/io/*.c??
        ${SYSTEM_LIBDIR}/core/io/private/*.h
        ${SYSTEM_LIBDIR}/core/io/private/*.c??
        ${SYSTEM_LIBDIR}/core/charset/*.h
        ${SYSTEM_LIBDIR}/core/charset/*.c??
        ${SYSTEM_LIBDIR}/core/charset/private/*.h
        ${SYSTEM_LIBDIR}/core/charset/private/*.c??
        ${SYSTEM_LIBDIR}/core/math/*.h
        ${SYSTEM_LIBDIR}/core/math/*.c??
        ${SYSTEM_LIBDIR}/core/math/private/*.h
        ${SYSTEM_LIBDIR}/core/math/private/*.c??
        ${SYSTEM_LIBDIR}/core/net/*.h
        ${SYSTEM_LIBDIR}/core/net/*.c??
        ${SYSTEM_LIBDIR}/core/net/private/*.h
        ${SYSTEM_LIBDIR}/core/net/private/*.c??
        ${SYSTEM_LIBDIR}/core/native/*.h
        ${SYSTEM_LIBDIR}/core/native/*.c??
        ${SYSTEM_LIBDIR}/core/native/private/*.h
        ${SYSTEM_LIBDIR}/core/native/private/*.c??
        ${SYSTEM_LIBDIR}/core/sql/*.h
        ${SYSTEM_LIBDIR}/core/sql/*.c??
        ${SYSTEM_LIBDIR}/core/sql/private/*.h
        ${SYSTEM_LIBDIR}/core/sql/private/*.c??
        ${SYSTEM_LIBDIR}/core/text/*.h
        ${SYSTEM_LIBDIR}/core/text/*.c??
        ${SYSTEM_LIBDIR}/core/text/private/*.h
        ${SYSTEM_LIBDIR}/core/text/private/*.c??
        ${SYSTEM_LIBDIR}/core/time/*.h
        ${SYSTEM_LIBDIR}/core/time/*.c??
        ${SYSTEM_LIBDIR}/core/time/private/*.h
        ${SYSTEM_LIBDIR}/core/time/private/*.c??
        ${SYSTEM_LIBDIR}/core/util/*.h
        ${SYSTEM_LIBDIR}/core/util/*.c??
        ${SYSTEM_LIBDIR}/core/util/private/*.h
        ${SYSTEM_LIBDIR}/core/util/private/*.c??
        ${SYSTEM_LIBDIR}/core/concurrent/*.h
        ${SYSTEM_LIBDIR}/core/concurrent/*.c??
        ${SYSTEM_LIBDIR}/core/concurrent/private/*.h
        ${SYSTEM_LIBDIR}/core/concurrent/private/*.c??
        ${SYSTEM_LIBDIR}/core/function/*.h
        ${SYSTEM_LIBDIR}/core/function/*.c??
        ${SYSTEM_LIBDIR}/core/function/private/*.h
        ${SYSTEM_LIBDIR}/core/function/private/*.c??
        ${SYSTEM_LIBDIR}/core/random/*.h
        ${SYSTEM_LIBDIR}/core/random/*.c??
        ${SYSTEM_LIBDIR}/core/random/private/*.h
        ${SYSTEM_LIBDIR}/core/random/private/*.c??
        ${SYSTEM_LIBDIR}/core/regex/*.h
        ${SYSTEM_LIBDIR}/core/regex/*.c??
        ${SYSTEM_LIBDIR}/core/regex/private/*.h
        ${SYSTEM_LIBDIR}/core/regex/private/*.c??
)

ADD_LIBRARY(Core23 STATIC ${Core23}
        Lib/core/stream/BaseStream.h)

INCLUDE_DIRECTORIES("${PROJECT_LIBDIR}")
INCLUDE_DIRECTORIES("${SYSTEM_LIBDIR}")


# This file is called to realize tests on Core23 library
ADD_EXECUTABLE(Main23 Main.cpp)

TARGET_LINK_LIBRARIES(Main23 PUBLIC Core23)

SET(CMAKE_GNUtoMS ON)
SET(CMAKE_AIX_EXPORT_ALL_SYMBOLS ON)
