//
// Created by mwypych on 07.02.17.
//

#include <cstddef>
#include <MemorySpy.h>

void start_spying() {
  return MemorySpy::start_spying();
}

void stop_spying() {
  return MemorySpy::stop_spying();
}

void clear_state() {
  return MemorySpy::clear_state();
}

void *malloc(size_t size) {
  return MemorySpy::malloc(size);
}

void free(void *aptr) {
  return MemorySpy::free(aptr);
}

