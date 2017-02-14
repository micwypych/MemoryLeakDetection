//
// Created by mwypych on 14.02.17.
//

#ifndef MEMORYLEAKDETECTION_ALLOCATIONSTACK_H
#define MEMORYLEAKDETECTION_ALLOCATIONSTACK_H

#include <vector>
class AllocationStack {
 public:
  void push_start();
  void pop_stop();
  bool state();
 private:
  int states = 0;
};

#endif //MEMORYLEAKDETECTION_ALLOCATIONSTACK_H
