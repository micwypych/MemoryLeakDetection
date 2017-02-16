//
// Created by mwypych on 15.02.17.
//

#include <gtest/gtest.h>
#include <memory>
#include <MemoryLeakDetector.h>
#include "LeakDetectionTest.h"

class LeakDetectionWithLeakDetectionFixture : public virtual LeakDetectionTest {

 public:
  std::string *testing_string;
 protected:
  void SetUp() override {
    ::testing::Test::SetUp();
    testing_string = new std::string();
  }
  void TearDown() override {
    ::testing::Test::TearDown();
    delete testing_string;
  }
};

TEST_F(LeakDetectionWithLeakDetectionFixture, Create) {
  ASSERT_TRUE(true);
}
