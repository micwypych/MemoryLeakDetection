//
// Created by mwypych on 07.02.17.
//

#include <cstddef>
#include <dlfcn.h>
#include "GNULibCDelegator.h"

void *GNULibCDelegator::malloc(size_t size) {
  auto libc_malloc = reinterpret_cast<libc_malloc_t>(dlsym(RTLD_NEXT, "malloc"));
  return libc_malloc(size);
}

void GNULibCDelegator::free(void *aptr) {
  auto libc_free = reinterpret_cast<libc_free_t>(dlsym(RTLD_NEXT, "free"));
  return libc_free(aptr);
}

bool GNULibCDelegator::initialize() {
  return true;
}
