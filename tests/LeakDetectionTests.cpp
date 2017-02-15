//
// Created by mwypych on 15.02.17.
//

#include <gtest/gtest.h>
#include <memory>
#include <MemoryLeakDetector.h>
#include <MemorySpy.h>

class LeakDetectionTests : public ::testing::Test {

 protected:
  virtual void SetUp() override {
    start_spying();
  }

  virtual void TearDown() override {
    MemorySpy::stop_spying();
    clear_state();
  }
 public:
  std::string expected_message(void *pointer, size_t size) {
    std::stringstream ss;
    ss << "memory allocated at ";
    ss << pointer;
    ss << " of size ";
    ss << size;
    ss << " was not properly freed";
    return ss.str();
  }
};

TEST_F(LeakDetectionTests, CreateNewInstance) {
  std::cout << "hello world!" << std::endl;
}

TEST_F(LeakDetectionTests, MakeVerifictaionAfterEmptyCode) {
  ASSERT_TRUE(MemorySpy::verify());
}

TEST_F(LeakDetectionTests,
       VerifictaionAfterAllocationOfIntByNewOperatorWithoutDeallocationExpectedVerificationFailture) {
  int *pointer = new int {5};
  MemorySpy::stop_spying();
  ASSERT_FALSE(MemorySpy::verify());
  auto issues = MemorySpy::issues();
  ASSERT_EQ(1, issues.size());
  ASSERT_EQ(expected_message(pointer, sizeof(int)), issues[0]);
  delete pointer;
}

TEST_F(LeakDetectionTests,
       VerifictaionAfterAllocationOfIntTableByNewOperatorWithoutDeallocationExpectedVerificationFailture) {
  int *pointer = new int[20];
  MemorySpy::stop_spying();
  ASSERT_FALSE(MemorySpy::verify());
  auto issues = MemorySpy::issues();
  ASSERT_EQ(1, issues.size());
  ASSERT_EQ(expected_message(pointer, sizeof(int[20])), issues[0]);
  delete[] pointer;
}

TEST_F(LeakDetectionTests,
       VerifictaionAfterThreeAllocationByNewOperatorWithoutDeallocationExpectedVerificationFailture) {
  double *pointer = new double[17];
  std::string *str = new std::string();
  void *xyz = new char *;
  MemorySpy::stop_spying();
  ASSERT_FALSE(MemorySpy::verify());
  auto issues = MemorySpy::issues();
  ASSERT_EQ(3, issues.size());
  ASSERT_EQ(expected_message(pointer, sizeof(double[17])), issues[0]);
  ASSERT_EQ(expected_message(str, sizeof(std::string)), issues[1]);
  ASSERT_EQ(expected_message(xyz, sizeof(char *)), issues[2]);
  delete[] pointer;
  delete str;
  delete xyz;
}

TEST_F(LeakDetectionTests,
       VerifictaionAfterSingleAllocationBySmartPointerBeforeExitingScopeExpectedVerificationFailture) {
  std::unique_ptr<double> d = std::make_unique<double>(90.78);
  MemorySpy::stop_spying();
  ASSERT_FALSE(MemorySpy::verify());
  auto issues = MemorySpy::issues();
  ASSERT_EQ(1, issues.size());
  ASSERT_EQ(expected_message(&(*d), sizeof(double)), issues[0]);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
