//
// Created by mwypych on 07.02.17.
//

#include <cstddef>
#include <MemorySpy.h>

void *malloc(size_t size) {
  return MemorySpy::instance().malloc(size);
}

void free(void *aptr) {
  return MemorySpy::instance().free(aptr);
}
