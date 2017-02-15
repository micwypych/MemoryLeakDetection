#include <iostream>

#include <memory>
#include <MemorySpy.h>
#include <iterator>

#include "MemoryLeakDetector.h"




int main() {
  std::cout << "Hello, World!" << std::endl;

  start_spying();

  int *p = new int {3};
  delete p;

  int *pt = new int[20];
//  delete[] pt;

  stop_spying();

  bool result = MemorySpy::verify();
  std::cout<<(result?"true":"false")<<std::endl;

  auto issues = MemorySpy::issues();
  std::copy(begin(issues), end(issues), std::ostream_iterator<std::string>(std::cout, "\n"));
//  for (const auto & str : MemorySpy::issues()) {
//    std::cout << str << std::endl;
//  }

  clear_state();

  return 0;
}