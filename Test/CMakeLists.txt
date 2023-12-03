
# This file is called to realize tests on Core23 library
ADD_EXECUTABLE(Main23 Main.cpp)
INCLUDE_DIRECTORIES(../Lib)
TARGET_LINK_LIBRARIES(Main23 PUBLIC Core23)
