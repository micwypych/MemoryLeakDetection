
project(leakdetectiontests)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

set(GTEST_LIBS gtest gmock)

set(HEDEAR_FILES LeakDetectionTests.h LeakDetectionTest.h)
set(SOURCE_FILES LeakDetectionTests.cpp LeakDetectionTest.cpp LeakDetectionWithLeakDetectionFixture.cpp MemoryLeakInSetUpWithLeakDetectionFixture.cpp)

#add_executable(leakdetectiontests ${SOURCE_FILES} ${HEADER_FILES} ${DETECTOR_SOURCE_FILES})
add_executable(leakdetectiontests ${SOURCE_FILES})

target_link_libraries(leakdetectiontests ${GTEST_LIBS})
target_link_libraries(leakdetectiontests memoryleakdetection)




