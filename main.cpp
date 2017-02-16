#include <iostream>

#include <memory>
#include <iterator>

#include "MemoryLeakDetector.h"




int main() {
  std::cout << "Hello, World!" << std::endl;

  memspy::StartSpying();

  int *p = new int {3};
  delete p;

  int *pt = new int[20];
//  delete[] pt;

  memspy::StopSpying();

  bool result = memspy::Verify();
  std::cout<<(result?"true":"false")<<std::endl;

  auto issues = memspy::Issues();
  std::copy(begin(issues), end(issues), std::ostream_iterator<std::string>(std::cout, "\n"));

  memspy::ClearState();

  return 0;
}