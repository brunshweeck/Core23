CMAKE_MINIMUM_REQUIRED(VERSION 3.25)

PROJECT(Core23 VERSION 2023.6.5 LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Adding compatible lib folder
IF (WIN32)
#    The Operating System is Windows (Vista, MS-DOS, MS-Windows, ...)
    ADD_SUBDIRECTORY(WinLib)
ELSE ()
#    The Operating System is Unix (Unix, Linux, Apple, Android, ...)
    ADD_SUBDIRECTORY(UnixLib)
ENDIF ()

# Adding test folder
ADD_SUBDIRECTORY(Test)

INCLUDE_DIRECTORIES("Lib/core")

ADD_COMPILE_DEFINITIONS(interface=struct)
