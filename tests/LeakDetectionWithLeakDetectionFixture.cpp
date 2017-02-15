//
// Created by mwypych on 15.02.17.
//

#include <gtest/gtest.h>
#include <memory>
#include <MemoryLeakDetector.h>
#include <MemorySpy.h>
#include "LeakDetectionTest.h"

class LeakDetectionWithLeakDetectionFixture : public virtual LeakDetectionTest {

};

TEST_F(LeakDetectionWithLeakDetectionFixture,Create) {
  ASSERT_TRUE(true);
}