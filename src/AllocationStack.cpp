//
// Created by mwypych on 14.02.17.
//

#include "AllocationStack.h"
void AllocationStack::PushStart() {
  states_++;
}
void AllocationStack::PopStop() {
  if (states_ > 0) {
    states_--;
  }
}
bool AllocationStack::State() {
  return states_ > 1;
}
