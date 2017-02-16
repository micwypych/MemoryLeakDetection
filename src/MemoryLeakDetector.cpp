//
// Created by mwypych on 07.02.17.
//

#include <cstddef>
#include <GNULibCDelegator.h>
#include <MemorySpy.h>

using LibCDelegator_t = GNULibCDelegator;

namespace memspy {
void start_spying() {
  return MemorySpy<LibCDelegator_t>::start_spying();
}

void stop_spying() {
  return MemorySpy<LibCDelegator_t>::stop_spying();
}

void clear_state() {
  return MemorySpy<LibCDelegator_t>::clear_state();
}

std::vector<std::string> issues() {
  return MemorySpy<LibCDelegator_t>::issues();
}

bool verify() {
  return MemorySpy<LibCDelegator_t>::verify();
}
}

void *malloc(size_t size) {
  return MemorySpy<LibCDelegator_t>::malloc(size);
}

void free(void *aptr) {
  return MemorySpy<LibCDelegator_t>::free(aptr);
}

