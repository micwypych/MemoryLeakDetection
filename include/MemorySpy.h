//
// Created by mwypych on 07.02.17.
//

#ifndef MEMORYLEAKDETECTION_MEMORYSPY_H
#define MEMORYLEAKDETECTION_MEMORYSPY_H

#include <cstddef>
#include <memory>
#include <vector>
#include "LibCDelegator.h"
#include "MemoryEntry.h"

class MemorySpy {
 public:
  virtual void *malloc(const size_t size);
  virtual void free(void *aptr);
  virtual void clear_state();
  virtual bool verify();

  static MemorySpy &instance();
//  static std::shared_ptr<MemorySpy> create();
 protected:
  MemorySpy() : entries({}), n_entries(0) {}

 private:
  constexpr static size_t MAX_ENTRIES_POSSIBLE = 1024;
  static MemorySpy _instance;
  int n_entries;
  LibCDelegator libc;
  std::array<MemoryEntry, MAX_ENTRIES_POSSIBLE> entries;
  void store_memory_malloc(const size_t size, const void *memory);
  void invalidate_entry(const void *aptr);
  bool try_invalidate_entry(const void *aptr);
};

#endif //MEMORYLEAKDETECTION_MEMORYSPY_H
