//
// Created by mwypych on 07.02.17.
//

#ifndef MEMORYLEAKDETECTION_MEMORYSPY_H
#define MEMORYLEAKDETECTION_MEMORYSPY_H

#include <cstddef>
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#include "GNULibCDelegator.h"
#include "MemoryEntry.h"
#include "AllocationStack.h"

template<class LIBC>
class MemorySpy {
 public:

  static void initialize();
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
  static bool spying;
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
  static T safe_internall_call(Operation mainOperation, DefualtOperation defualtOperation);
  template<typename Operation, typename DefualtOperation>
  static void safe_internall_proc(Operation op, DefualtOperation defaultResult);

 private:
  AllocationStack internal_allocation_state;
  constexpr static size_t MAX_ENTRIES_POSSIBLE = 1024;
  static MemorySpy _instance;
  int n_entries;
  LIBC libc;
  static LIBC raw_libc;
  std::vector<MemoryEntry> entries;
  void store_memory_malloc(const size_t size, const void *memory);
  void invalidate_entry(const void *aptr);
  bool try_invalidate_entry(const void *aptr);
  bool internal_allocation_on();
};

template<class LIBC>
bool MemorySpy<LIBC>::initialized = false;

template<class LIBC>
bool MemorySpy<LIBC>::spying = false;

template<class LIBC>
LIBC MemorySpy<LIBC>::raw_libc{};

template<class LIBC>
MemorySpy<LIBC> MemorySpy<LIBC>::_instance{};

template<class LIBC>
void *MemorySpy<LIBC>::spy_malloc(const size_t size) {
  void *memory = libc.malloc(size);
  if (not internal_allocation_on()) {
    std::cout << "hello" << std::endl;
    store_memory_malloc(size, memory);
  }
  return memory;
}

template<class LIBC>
void MemorySpy<LIBC>::store_memory_malloc(const size_t size, const void *memory) {
  entries.emplace_back(memory, size);
}

template<class LIBC>
void MemorySpy<LIBC>::spy_free(void *aptr) {
  if (not internal_allocation_on()) {
    std::cout << "bye" << std::endl;
    invalidate_entry(aptr);
  }
  libc.free(aptr);
}

template<class LIBC>
void MemorySpy<LIBC>::invalidate_entry(const void *aptr) {
  if (aptr != nullptr) {
    bool allocated = try_invalidate_entry(aptr);
    if (!allocated) {
      throw "memory was not allocated first";
    }
  }
}

template<class LIBC>
bool MemorySpy<LIBC>::try_invalidate_entry(const void *aptr) {
  bool allocated{false};
  for (auto &entry : entries) {
    if (entry.is_valid() && entry.points_to(aptr)) {
      entry.invalidate();
      allocated = true;
    }
  }
  return allocated;
}

template<class LIBC>
void MemorySpy<LIBC>::spy_clear_state() {
  entries.clear();
}

template<class LIBC>
bool MemorySpy<LIBC>::spy_verify() {
  int valid_elements = 0;
  std::for_each(entries.begin(), entries.end(),
                [&valid_elements]
                    (auto &entry) {
                  if (entry.is_valid()) {
                    valid_elements++;
                  }
                });
  return valid_elements == 0;
}

template<class LIBC>
MemorySpy<LIBC> &MemorySpy<LIBC>::instance() {
  return _instance;
}

template<class LIBC>
void *MemorySpy<LIBC>::malloc(const size_t size) {
  return safe_internall_call<void *>(
      [size](auto &inst) {
        return inst.spy_malloc(size);
      },
      [size]() {
        return raw_malloc(size);
      });
}

template<class LIBC>
void MemorySpy<LIBC>::free(void *aptr) {
  safe_internall_proc(
      [aptr](auto &inst) {
        inst.spy_free(aptr);
      },
      [aptr]() {
        raw_free(aptr);
      });
}

template<class LIBC>
bool MemorySpy<LIBC>::readyForSpying() {
  return initialized;
}

template<class LIBC>
bool MemorySpy<LIBC>::verify() {
  return safe_internall_call<bool>(
      [](auto &inst) {
        return inst.spy_verify();
      },
      [] {
        return true;
      });
}

template<class LIBC>
void MemorySpy<LIBC>::clear_state() {
  safe_internall_proc(
      [](auto &inst) {
        inst.spy_clear_state();
      },
      [] {});
  initialized = false;
}

template<class LIBC>
void *MemorySpy<LIBC>::raw_malloc(const size_t size) {
  return raw_libc.malloc(size);
}

template<class LIBC>
void MemorySpy<LIBC>::raw_free(void *aptr) {
  return raw_libc.free(aptr);
}

template<class LIBC>
void MemorySpy<LIBC>::start_spying() {
  initialize();
  spying = true;
}

template<class LIBC>
void MemorySpy<LIBC>::stop_spying() {
  spying = false;
}

template<class LIBC>
std::vector<std::string> MemorySpy<LIBC>::issues() {
  return safe_internall_call<std::vector<std::string>>(
      [](auto &inst) {
        return inst.spy_issues();
      },
      []() {
        return std::vector<std::string>();
      });
}

template<class LIBC>
std::vector<std::string> MemorySpy<LIBC>::spy_issues() {
  std::vector<std::string> result;
  std::vector<MemoryEntry> temp;
  std::remove_copy_if(
      begin(entries),
      end(entries),
      std::back_inserter(temp),
      [](const auto &entry) {
        return not entry.is_valid();
      });;
  std::transform(
      begin(temp),
      end(temp),
      std::back_inserter(result),
      [](const auto &entry) {
        return entry.message("memory allocated at {PTR} of size {SIZE} was not properly freed");
      });
  return result;
}

template<class LIBC>
void MemorySpy<LIBC>::internal_allocation_start() {
  internal_allocation_state.push_start();
}

template<class LIBC>
void MemorySpy<LIBC>::internal_allocation_stop() {
  internal_allocation_state.pop_stop();
}

template<class LIBC>
template<typename T, typename Operation, typename DefualtOperation>
T MemorySpy<LIBC>::safe_internall_call(Operation op, DefualtOperation defaultResult) {
  //TODO does not work with void* static_assert(std::is_void<T>::value,"cannot use call with void return type instead use safe_internal_proc");
  if (readyForSpying()) {
    instance().internal_allocation_start();
    auto result = op(instance());
    instance().internal_allocation_stop();
    return result;
  } else {
    return defaultResult();
  }
}

//TODO and not recording suspended
template<class LIBC>
bool MemorySpy<LIBC>::internal_allocation_on() {
  return internal_allocation_state.state() or not spying;
}

template<class LIBC>
template<typename Operation, typename DefualtOperation>
void MemorySpy<LIBC>::safe_internall_proc(Operation op, DefualtOperation defaultResult) {
  if (readyForSpying()) {
    instance().internal_allocation_start();
    op(instance());
    instance().internal_allocation_stop();
  } else {
    defaultResult();
  }
}

template<class LIBC>
void MemorySpy<LIBC>::initialize() {
  if (not initialized) {
    instance().internal_allocation_on();
    initialized = instance().libc.initialize();
    instance().internal_allocation_stop();
  }
}

#endif //MEMORYLEAKDETECTION_MEMORYSPY_H
