//
// Created by mwypych on 14.02.17.
//

#ifndef MEMORYLEAKDETECTION_ALLOCATIONSTACK_H
#define MEMORYLEAKDETECTION_ALLOCATIONSTACK_H

class AllocationStack {
 public:
  void PushStart();
  void PopStop();
  bool State();
 private:
  int states_ = 0;
};

#endif //MEMORYLEAKDETECTION_ALLOCATIONSTACK_H
