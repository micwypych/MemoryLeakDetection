//
// Created by mwypych on 16.02.17.
//

#include <gtest/gtest.h>
#include <memory>
#include <MemoryLeakDetector.h>
#include <MemorySpy.h>
#include "LeakDetectionTest.h"

class MemoryLeakInSetUpWithLeakDetectionFixture : public virtual LeakDetectionTest {

 public:
  std::string *testing_string;
 protected:
  void SetUp() override {
    std::cout<<__PRETTY_FUNCTION__<<" running"<<std::endl;
    ::testing::Test::SetUp();
    testing_string = new std::string();
  }
  void TearDown() override {
    ::testing::Test::TearDown();
    //delete testing_string;
  }
};

TEST_F(MemoryLeakInSetUpWithLeakDetectionFixture,DISABLED_Create) {
  ASSERT_TRUE(true);
}