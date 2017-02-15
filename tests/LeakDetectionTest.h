//
// Created by mwypych on 15.02.17.
//

#ifndef MEMORYLEAKDETECTION_LEAKDETECTIONTEST_H
#define MEMORYLEAKDETECTION_LEAKDETECTIONTEST_H

#include <iosfwd>
#include <string>
#include <MemorySpy.h>
#include <MemoryLeakDetector.h>
#include <sstream>
#include <gtest/gtest.h>

class LeakDetectionTest : public virtual ::testing::Test {
 public:
  LeakDetectionTest() {
    start_spying();
  }

  virtual ~LeakDetectionTest() {
    MemorySpy::stop_spying();
    auto accepted = MemorySpy::verify();
    std::string message;
    if (not accepted) {
      auto issues = MemorySpy::issues();
      std::stringstream ss;
      ss<<"There were discoverd "<<issues.size()<<" number of memory leakage: "<<std::endl;
      for (const auto &str : issues) {
        ss<<str<<std::endl;
      }
      message = ss.str();
    }
    clear_state();
    EXPECT_TRUE(accepted) << message;
  }
};

#endif //MEMORYLEAKDETECTION_LEAKDETECTIONTEST_H
