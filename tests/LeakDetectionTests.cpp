//
// Created by mwypych on 15.02.17.
//

#include <gtest/gtest.h>
#include <memory>
#include <MemoryLeakDetector.h>

class LeakDetectionTests : public ::testing::Test {

 protected:
  virtual void SetUp() override {
    memspy::start_spying();
  }

  virtual void TearDown() override {
    memspy::stop_spying();
    memspy::clear_state();
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

  template<typename T>
  void alloc_unique_value(T &&t) {
    std::unique_ptr<T> p = std::make_unique<T>(std::forward<T>(t));
  }
};

TEST_F(LeakDetectionTests, CreateNewInstance) {

}

TEST_F(LeakDetectionTests, MakeVerifictaionAfterEmptyCode) {
  ASSERT_TRUE(memspy::verify());
}

TEST_F(LeakDetectionTests,
       VerifictaionAfterAllocationOfIntByNewOperatorWithoutDeallocationExpectedVerificationFailture) {
  int *pointer = new int {5};
  memspy::stop_spying();
  ASSERT_FALSE(memspy::verify());
  auto issues = memspy::issues();
  ASSERT_EQ(1, issues.size());
  ASSERT_EQ(expected_message(pointer, sizeof(int)), issues[0]);
  delete pointer;
}

TEST_F(LeakDetectionTests,
       VerifictaionAfterAllocationOfIntTableByNewOperatorWithoutDeallocationExpectedVerificationFailture) {
  int *pointer = new int[20];
  memspy::stop_spying();
  ASSERT_FALSE(memspy::verify());
  auto issues = memspy::issues();
  ASSERT_EQ(1, issues.size());
  ASSERT_EQ(expected_message(pointer, sizeof(int[20])), issues[0]);
  delete[] pointer;
}

TEST_F(LeakDetectionTests,
       VerifictaionAfterThreeAllocationByNewOperatorWithoutDeallocationExpectedVerificationFailture) {
  double *pointer = new double[17];
  std::string *str = new std::string();
  void *xyz = new char *;
  memspy::stop_spying();
  ASSERT_FALSE(memspy::verify());
  auto issues = memspy::issues();
  ASSERT_EQ(3, issues.size());
  ASSERT_EQ(expected_message(pointer, sizeof(double[17])), issues[0]);
  ASSERT_EQ(expected_message(str, sizeof(std::string)), issues[1]);
  ASSERT_EQ(expected_message(xyz, sizeof(char *)), issues[2]);
  delete[] pointer;
  delete str;
  delete reinterpret_cast<char *>(xyz);
}

TEST_F(LeakDetectionTests,
       VerifictaionAfterSingleAllocationBySmartPointerBeforeExitingScopeExpectedVerificationFailture) {
  std::unique_ptr<double> d = std::make_unique<double>(90.78);
  memspy::stop_spying();
  ASSERT_FALSE(memspy::verify());
  auto issues = memspy::issues();
  ASSERT_EQ(1, issues.size());
  ASSERT_EQ(expected_message(&(*d), sizeof(double)), issues[0]);
}

TEST_F(LeakDetectionTests,
       VerifictaionAfterSingleAllocationBySmartPointerAfterExitingScopeExpectedVerificationSuccess) {
  alloc_unique_value(89.9);
  memspy::stop_spying();
  ASSERT_TRUE(memspy::verify());
  auto issues = memspy::issues();
  ASSERT_EQ(0, issues.size());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
