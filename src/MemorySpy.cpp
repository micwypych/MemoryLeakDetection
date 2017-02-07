//
// Created by mwypych on 07.02.17.
//

#include <iostream>
#include "LibCDelegator.h"
#include "MemorySpy.h"
#include <boost/format.hpp>

MemorySpy MemorySpy::_instance {};

void *MemorySpy::malloc(const size_t size) {
  std::cout<<"hello"<<std::endl;
  void *memory = libc.malloc(size);
  store_memory_malloc(size, memory);
  return memory;
}

void MemorySpy::store_memory_malloc(const size_t size, const void *memory) {
  if (n_entries < MAX_ENTRIES_POSSIBLE) {
    entries[n_entries++] = std::move(MemoryEntry {memory, size});
  }
}

void MemorySpy::free(void *aptr) {
  std::cout<<"bye"<<std::endl;
  invalidate_entry(aptr);
  libc.free(aptr);
}

void MemorySpy::invalidate_entry(const void *aptr) {
  if (aptr != nullptr) {
    bool allocated = try_invalidate_entry(aptr);
    if (!allocated) {
      throw "memory was not allocated first";
    }
  }
}

bool MemorySpy::try_invalidate_entry(const void *aptr) {
  bool allocated{false};
  for (int i = 0; i < n_entries; i++) {
    if (entries[i].is_valid() && entries[i].points_to(aptr)) {
      entries[i].invalidate();
      allocated = true;
    }
  }
  return allocated;
}

void MemorySpy::clear_state() {
  for (int i = 0; i < n_entries; i++) {
    entries[i] = std::move(MemoryEntry{});
  }
  n_entries = 0;
}

bool MemorySpy::verify() {
  int valid_elements = 0;
  for (int i = 0; i < n_entries; i++) {
    if (entries[i].is_valid()) {
      valid_elements++;
    }
  }
  return valid_elements == 0;
}

MemorySpy &MemorySpy::instance() {
  return _instance;
}

//std::shared_ptr<MemorySpy> MemorySpy::create() {
//  return std::shared_ptr<MemorySpy> {new MemorySpy(std::make_unique<LibCDelegator>())};
//}
