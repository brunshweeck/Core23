CMAKE_MINIMUM_REQUIRED(VERSION 3.26)

PROJECT(Core23 VERSION 2023.6.5 LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 23)

# Adding compatible lib folder
IF (WIN32)
#    The Operating System is Windows (Vista, Windows X, ...)
    ADD_SUBDIRECTORY(WinLib)
    IF (MSVC)
        ADD_COMPILE_OPTIONS("/utf8")
    ENDIF ()
ELSE ()
#    The Operating System is Unix (Linux, Apple, Android, ...)
    ADD_SUBDIRECTORY(UnixLib)
ENDIF ()

# Adding test folder
ADD_SUBDIRECTORY(Test)

INCLUDE_DIRECTORIES("Lib/core")

ADD_COMPILE_DEFINITIONS(interface=struct)
