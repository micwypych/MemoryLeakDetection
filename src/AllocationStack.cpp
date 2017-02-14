//
// Created by mwypych on 14.02.17.
//

#include "AllocationStack.h"
void AllocationStack::push_start() {
  states++;
}
void AllocationStack::pop_stop() {
  if (states > 0) {
    states--;
  }
}
bool AllocationStack::state() {
  return states > 1;
}
