cmake_minimum_required(VERSION 3.6)
project(memoryleakdetection)

set(CMAKE_CXX_STANDARD 14)

set(DETECTOR_SOURCE_FILES include/LibC.h src/GnuLibCDelegator.cpp include/GnuLibCDelegator.h src/MemorySpy.cpp include/MemorySpy.h src/MemoryEntry.cpp include/MemoryEntry.h include/MemoryLeakDetector.h src/MemoryLeakDetector.cpp src/AllocationStack.cpp include/AllocationStack.h)
add_library(memoryleakdetection ${DETECTOR_SOURCE_FILES})
target_link_libraries(memoryleakdetection dl)
target_include_directories(memoryleakdetection PUBLIC include)

add_executable(memoryleakdetection_main main.cpp)
target_link_libraries(memoryleakdetection_main memoryleakdetection)

include_directories(include)


add_subdirectory(lib/gtest-1.8.0)
add_subdirectory(tests)
