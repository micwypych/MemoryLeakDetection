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

  static void start_spying();
  static void stop_spying();
  static void clear_state();
  static void *malloc(const size_t size);
  static void free(void *aptr);
  static bool verify();

//  static std::shared_ptr<MemorySpy> create();
 protected:
  static bool initilized;
  static MemorySpy &instance();
  MemorySpy() : entries({}), n_entries(0) {}
  static void *raw_malloc(const size_t size);
  static bool readyForSpying();
  static void raw_free(void *aptr);

  virtual void *spy_malloc(const size_t size);
  virtual void spy_free(void *aptr);
  virtual void spy_clear_state();
  virtual bool spy_verify();


 private:
  constexpr static size_t MAX_ENTRIES_POSSIBLE = 1024;
  static MemorySpy _instance;
  int n_entries;
  LibCDelegator libc;
  static LibCDelegator raw_libc;
  std::vector<MemoryEntry> entries;
  void store_memory_malloc(const size_t size, const void *memory);
  void invalidate_entry(const void *aptr);
  bool try_invalidate_entry(const void *aptr);
};

#endif //MEMORYLEAKDETECTION_MEMORYSPY_H
