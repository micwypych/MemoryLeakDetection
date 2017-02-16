//
// Created by mwypych on 07.02.17.
//

#include <cstddef>
#include <GnuLibCDelegator.h>
#include <MemorySpy.h>

using LibCDelegator_t = GnuLibCDelegator;

namespace memspy {
void StartSpying() {
  return MemorySpy<LibCDelegator_t>::StartSpying();
}

void StopSpying() {
  return MemorySpy<LibCDelegator_t>::StopSpying();
}

void ClearState() {
  return MemorySpy<LibCDelegator_t>::ClearState();
}

std::vector<std::string> Issues() {
  MemorySpy<LibCDelegator_t >::IssuesList issues = MemorySpy<LibCDelegator_t>::Issues();
  std::vector<std::string> to_return = issues;
  return to_return;
}

bool Verify() {
  return MemorySpy<LibCDelegator_t>::Verify();
}
}

void *malloc(size_t size) {
  return MemorySpy<LibCDelegator_t>::Malloc(size);
}

void free(void *aptr) {
  return MemorySpy<LibCDelegator_t>::Free(aptr);
}

