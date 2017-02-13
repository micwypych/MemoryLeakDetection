//
// Created by mwypych on 07.02.17.
//

#include <iostream>
#include "LibCDelegator.h"
#include "MemorySpy.h"
#include <boost/format.hpp>




bool MemorySpy::initilized = false;
LibCDelegator MemorySpy::raw_libc {};

MemorySpy MemorySpy::_instance {};

void *MemorySpy::spy_malloc(const size_t size) {
  std::cout<<"hello"<<std::endl;
  void *memory = libc.malloc(size);
  store_memory_malloc(size, memory);
  return memory;
}

void MemorySpy::store_memory_malloc(const size_t size, const void *memory) {
  initilized = false;
  entries.emplace_back(memory, size);
  initilized = true;
}

void MemorySpy::spy_free(void *aptr) {
  std::cout<<"bye"<<std::endl;
  invalidate_entry(aptr);
  libc.free(aptr);
}

void MemorySpy::invalidate_entry(const void *aptr) {
  if (aptr != nullptr && initilized) {
    bool allocated = try_invalidate_entry(aptr);
    if (!allocated) {
      throw "memory was not allocated first";
    }
  }
}

bool MemorySpy::try_invalidate_entry(const void *aptr) {
  bool allocated{false};
  for (int i = 0; i < entries.size(); i++) {
    if (entries[i].is_valid() && entries[i].points_to(aptr)) {
      entries[i].invalidate();
      allocated = true;
    }
  }
  return allocated;
}

void MemorySpy::spy_clear_state() {
  for (int i = 0; i < n_entries; i++) {
    entries[i] = std::move(MemoryEntry{});
  }
  n_entries = 0;
}

bool MemorySpy::spy_verify() {
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

void *MemorySpy::malloc(const size_t size) {
  if (readyForSpying()) {
    return instance().spy_malloc(size);
  } else {
    return raw_malloc(size);
  }
}

void MemorySpy::free(void *aptr) {
  if (readyForSpying()) {
    instance().spy_free(aptr);
  } else {
    raw_free(aptr);
  }
}

bool MemorySpy::readyForSpying() {
  return initilized;
}
bool MemorySpy::verify() {
  if (readyForSpying()) {
    return instance().spy_verify();
  } else {
    return true;
  }
}

void MemorySpy::clear_state() {
  if (readyForSpying()) {
    instance();
  }
}

void *MemorySpy::raw_malloc(const size_t size) {
  return raw_libc.malloc(size);
}

void MemorySpy::raw_free(void *aptr) {
  return raw_libc.free(aptr);
}

void MemorySpy::start_spying() {
  initilized = true;
}

void MemorySpy::stop_spying() {
  initilized = false;
}

//std::shared_ptr<MemorySpy> MemorySpy::create() {
//  return std::shared_ptr<MemorySpy> {new MemorySpy(std::make_unique<LibCDelegator>())};
//}
