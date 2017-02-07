#include <iostream>

#include <memory>
#include <MemorySpy.h>

#include "MemoryLeakDetector.h"




int main() {
  std::cout << "Hello, World!" << std::endl;

  int *p = new int {3};
  delete p;

  int *pt = new int[20];
  delete[] pt;

  bool result = MemorySpy::instance().verify();
  std::cout<<result<<std::endl;
  return 0;
}