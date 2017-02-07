cmake_minimum_required(VERSION 3.6)
project(memoryleakdetection)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp include/LibC.h src/LibCDelegator.cpp include/LibCDelegator.h src/MemorySpy.cpp include/MemorySpy.h src/MemoryEntry.cpp include/MemoryEntry.h include/MemoryLeakDetector.h src/MemoryLeakDetector.cpp)
add_executable(memoryleakdetection ${SOURCE_FILES})

include_directories(include)
target_link_libraries(memoryleakdetection dl)
