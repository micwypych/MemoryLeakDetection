//
// Created by mwypych on 07.02.17.
//

#include <cstddef>
#include <dlfcn.h>
#include "GnuLibCDelegator.h"

void *GnuLibCDelegator::Malloc(size_t size) {
  auto libc_malloc = reinterpret_cast<libc_malloc_t>(dlsym(RTLD_NEXT, "malloc"));
  return libc_malloc(size);
}

void GnuLibCDelegator::Free(void *aptr) {
  auto libc_free = reinterpret_cast<libc_free_t>(dlsym(RTLD_NEXT, "free"));
  return libc_free(aptr);
}

bool GnuLibCDelegator::Initialize() {
  return true;
}
