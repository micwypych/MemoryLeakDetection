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
#include "AllocationStack.h"

class MemorySpy {
 public:

  static void start_spying();
  static void stop_spying();
  static void clear_state();
  static void *malloc(const size_t size);
  static void free(void *aptr);
  static bool verify();
  static std::vector<std::string> issues();

//  static std::shared_ptr<MemorySpy> create();
 protected:
  static bool initialized;
  static MemorySpy &instance();
  MemorySpy() : entries({}), n_entries(0) {}
  static void *raw_malloc(const size_t size);
  static bool readyForSpying();
  static void raw_free(void *aptr);

  virtual void *spy_malloc(const size_t size);
  virtual void spy_free(void *aptr);
  virtual void spy_clear_state();
  virtual bool spy_verify();
  virtual std::vector<std::string> spy_issues();

  virtual void internal_allocation_start();
  virtual void internal_allocation_stop();

  template<typename T, typename Operation, typename DefualtOperation>
  static T safe_internall_call(Operation mainOperation,DefualtOperation defualtOperation);
  template<typename Operation, typename DefualtOperation>
  static void safe_internall_call(Operation op, DefualtOperation defaultResult);


 private:
  AllocationStack internal_allocation_state;
  constexpr static size_t MAX_ENTRIES_POSSIBLE = 1024;
  static MemorySpy _instance;
  int n_entries;
  LibCDelegator libc;
  static LibCDelegator raw_libc;
  std::vector<MemoryEntry> entries;
  void store_memory_malloc(const size_t size, const void *memory);
  void invalidate_entry(const void *aptr);
  bool try_invalidate_entry(const void *aptr);
  bool internal_allocation_on();
};

#endif //MEMORYLEAKDETECTION_MEMORYSPY_H
