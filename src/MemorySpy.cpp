//
// Created by mwypych on 07.02.17.
//

#include <iostream>
#include "LibCDelegator.h"
#include "MemorySpy.h"
#include <boost/format.hpp>

bool MemorySpy::initialized = false;
LibCDelegator MemorySpy::raw_libc{};

MemorySpy MemorySpy::_instance{};

void *MemorySpy::spy_malloc(const size_t size) {
  void *memory = libc.malloc(size);
  if (not internal_allocation_on()) {
    std::cout << "hello" << std::endl;
    store_memory_malloc(size, memory);
  }
  return memory;
}

void MemorySpy::store_memory_malloc(const size_t size, const void *memory) {
  entries.emplace_back(memory, size);
}

void MemorySpy::spy_free(void *aptr) {
  if (not internal_allocation_on()) {
    std::cout << "bye" << std::endl;
    invalidate_entry(aptr);
  }
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
  for (auto &entry : entries) {
    if (entry.is_valid() && entry.points_to(aptr)) {
      entry.invalidate();
      allocated = true;
    }
  }
  return allocated;
}

void MemorySpy::spy_clear_state() {
  entries.clear();
}

bool MemorySpy::spy_verify() {
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

MemorySpy &MemorySpy::instance() {
  return _instance;
}

void *MemorySpy::malloc(const size_t size) {
  return safe_internall_call<void *>([size](auto &inst) {
                                       return inst.spy_malloc(size);
                                     },
                                     [size]() {
                                       return raw_malloc(size);
                                     });
}

void MemorySpy::free(void *aptr) {
  safe_internall_call([aptr](auto &inst) { inst.spy_free(aptr); }, [aptr]() { raw_free(aptr); });
}

bool MemorySpy::readyForSpying() {
  return initialized;
}

bool MemorySpy::verify() {
  return safe_internall_call<bool>([](auto &inst) {
                                     return inst.spy_verify();
                                   },
                                   []() {
                                     return true;
                                   });
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
  initialized = true;
}

void MemorySpy::stop_spying() {
  initialized = false;
}

std::vector<std::string> MemorySpy::issues() {
  return safe_internall_call<std::vector<std::string>>([](auto &inst) {
                                                         return inst.spy_issues();
                                                       },
                                                       []() {
                                                         return std::vector<std::string>();
                                                       });
}
std::vector<std::string> MemorySpy::spy_issues() {
  std::vector<std::string> result;
  std::vector<MemoryEntry> temp;
  std::remove_copy_if(entries.begin(),
                      entries.end(),
                      std::back_inserter(temp),
                      [](const auto &entry) {
                        return not entry.is_valid();
                      });
  std::transform(temp.begin(),
                 temp.end(),
                 std::back_inserter(result),
                 [](const auto &entry) {
                   return entry.message();
                 });
  return result;
}

void MemorySpy::internal_allocation_start() {
  internal_allocation_state.push_start();
}

void MemorySpy::internal_allocation_stop() {
  internal_allocation_state.pop_stop();
}

template<typename T, typename Operation, typename DefualtOperation>
T MemorySpy::safe_internall_call(Operation op, DefualtOperation defaultResult) {

  if (readyForSpying()) {
    instance().internal_allocation_start();
    auto result = op(instance());
    instance().internal_allocation_stop();
    return result;
  } else {
    return defaultResult();
  }
}

bool MemorySpy::internal_allocation_on() {
  return internal_allocation_state.state();
}

template<typename Operation, typename DefualtOperation>
void MemorySpy::safe_internall_call(Operation op, DefualtOperation defaultResult) {

  if (readyForSpying()) {
    instance().internal_allocation_start();
    op(instance());
    instance().internal_allocation_stop();
  } else {
    defaultResult();
  }
}
