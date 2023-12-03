
# This file is called on windows system only

FILE(GLOB Core23
        ../Lib/core/*.h
        ../Lib/core/*.c??
        ../Lib/core/foreign/*.h
        ../Lib/core/foreign/*.c??
        ../Lib/core/io/*.h
        ../Lib/core/io/*.c??
        ../Lib/core/io/charset/*.h
        ../Lib/core/io/charset/*.c??
        ../Lib/core/io/native/*.h
        ../Lib/core/io/native/*.c??
        ../Lib/core/math/*.h
        ../Lib/core/math/*.c??
        ../Lib/core/net/*.h
        ../Lib/core/net/*.c??
        ../Lib/core/primitive/*.h
        ../Lib/core/primitive/*.c??
        ../Lib/core/private/*.h
        ../Lib/core/private/*.c??
        ../Lib/core/sql/*.h
        ../Lib/core/sql/*.c??
        ../Lib/core/text/*.h
        ../Lib/core/text/*.c??
        ../Lib/core/time/*.h
        ../Lib/core/time/*.c??
        ../Lib/core/util/*.h
        ../Lib/core/util/*.c??
        ../Lib/core/util/concurrent/*.h
        ../Lib/core/util/concurrent/*.c??
        ../Lib/core/util/function/*.h
        ../Lib/core/util/function/*.c??
        ../Lib/core/util/random/*.h
        ../Lib/core/util/random/*.c??
        ../Lib/core/util/regex/*.h
        ../Lib/core/util/regex/*.c??
## =============================================================== ##
        ../WinLib/core/native/*.h
        ../WinLib/core/native/*.c??
        ../WinLib/core/private/*.h
        ../WinLib/core/private/*.c??
)

ADD_LIBRARY(Core23 STATIC EXCLUDE_FROM_ALL ${Core23})

INCLUDE_DIRECTORIES("../Lib")
