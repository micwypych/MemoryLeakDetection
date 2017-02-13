#include <iostream>

#include <memory>
#include <MemorySpy.h>

#include "MemoryLeakDetector.h"




int main() {
  std::cout << "Hello, World!" << std::endl;

  start_spying();

  int *p = new int {3};
  delete p;

  int *pt = new int[20];
  delete[] pt;

  stop_spying();

  bool result = verify();
  std::cout<<result<<std::endl;

  clear_state();

  return 0;
}