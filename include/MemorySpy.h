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
#include "GnuLibCDelegator.h"
#include "MemoryEntry.h"
#include "AllocationStack.h"

template<class LIBC>
class MemorySpy {
 public:

  using IssuesList = std::vector<std::string>;

  static void Initialize();
  static void StartSpying();
  static void StopSpying();
  static void ClearState();

  static void *Malloc(const size_t size);
  static void Free(void *aptr);

  static bool Verify();
  static IssuesList Issues();

//  static std::shared_ptr<MemorySpy> create();
 protected:
  static MemorySpy &Instance();
  static bool readyForSpying();
  MemorySpy() : entries_({}), n_entries_(0) {}

  static bool initialized_;
  static bool spying_;

  static void *RawMalloc(const size_t size);
  static void RawFree(void *aptr);

  virtual void *SpyMalloc(const size_t size);
  virtual void SpyFree(void *aptr);
  virtual void SpyClearState();
  virtual bool SpyVerify();
  virtual IssuesList SpyIssues();

  virtual void InternalAllocationStart();
  virtual void InternalAllocationStop();

  template<typename T, typename Operation, typename DefualtOperation>
  static T SafeInternallCall(Operation mainOperation, DefualtOperation defualtOperation);
  template<typename Operation, typename DefualtOperation>
  static void SafeInternallProc(Operation op, DefualtOperation defaultResult);

 private:
  AllocationStack internal_allocation_state_;
  static MemorySpy instance_;
  int n_entries_;
  LIBC libc_;
  static LIBC raw_libc_;
  std::vector<MemoryEntry> entries_;
  void StoreMemoryMalloc(const size_t size, const void *memory);
  void InvalidateEntry(const void *aptr);
  bool TryInvalidateEntry(const void *aptr);
  bool InternalAllocationOn();
};

template<class LIBC>
bool MemorySpy<LIBC>::initialized_ = false;

template<class LIBC>
bool MemorySpy<LIBC>::spying_ = false;

template<class LIBC>
LIBC MemorySpy<LIBC>::raw_libc_{};

template<class LIBC>
MemorySpy<LIBC> MemorySpy<LIBC>::instance_{};

template<class LIBC>
void *MemorySpy<LIBC>::SpyMalloc(const size_t size) {
  void *memory = libc_.Malloc(size);
  if (not InternalAllocationOn()) {
    std::cout << "hello" << std::endl;
    StoreMemoryMalloc(size, memory);
  }
  return memory;
}

template<class LIBC>
void MemorySpy<LIBC>::StoreMemoryMalloc(const size_t size, const void *memory) {
  entries_.emplace_back(memory, size);
}

template<class LIBC>
void MemorySpy<LIBC>::SpyFree(void *aptr) {
  if (not InternalAllocationOn()) {
    std::cout << "bye" << std::endl;
    InvalidateEntry(aptr);
  }
  libc_.Free(aptr);
}

template<class LIBC>
void MemorySpy<LIBC>::InvalidateEntry(const void *aptr) {
  if (aptr != nullptr) {
    bool allocated = TryInvalidateEntry(aptr);
    if (!allocated) {
      throw "memory was not allocated first";
    }
  }
}

template<class LIBC>
bool MemorySpy<LIBC>::TryInvalidateEntry(const void *aptr) {
  bool allocated{false};
  for (auto &entry : entries_) {
    if (entry.IsValid() && entry.PointsTo(aptr)) {
      entry.Invalidate();
      allocated = true;
    }
  }
  return allocated;
}

template<class LIBC>
void MemorySpy<LIBC>::SpyClearState() {
  entries_.clear();
}

template<class LIBC>
bool MemorySpy<LIBC>::SpyVerify() {
  int valid_elements = 0;
  std::for_each(entries_.begin(), entries_.end(),
                [&valid_elements]
                    (auto &entry) {
                  if (entry.IsValid()) {
                    valid_elements++;
                  }
                });
  return valid_elements == 0;
}

template<class LIBC>
MemorySpy<LIBC> &MemorySpy<LIBC>::Instance() {
  return instance_;
}

template<class LIBC>
void *MemorySpy<LIBC>::Malloc(const size_t size) {
  return SafeInternallCall<void *>(
      [size](auto &inst) {
        return inst.SpyMalloc(size);
      },
      [size]() {
        return RawMalloc(size);
      });
}

template<class LIBC>
void MemorySpy<LIBC>::Free(void *aptr) {
  SafeInternallProc(
      [aptr](auto &inst) {
        inst.SpyFree(aptr);
      },
      [aptr]() {
        RawFree(aptr);
      });
}

template<class LIBC>
bool MemorySpy<LIBC>::readyForSpying() {
  return initialized_;
}

template<class LIBC>
bool MemorySpy<LIBC>::Verify() {
  return SafeInternallCall<bool>(
      [](auto &inst) {
        return inst.SpyVerify();
      },
      [] {
        return true;
      });
}

template<class LIBC>
void MemorySpy<LIBC>::ClearState() {
  SafeInternallProc(
      [](auto &inst) {
        inst.SpyClearState();
      },
      [] {});
  initialized_ = false;
}

template<class LIBC>
void *MemorySpy<LIBC>::RawMalloc(const size_t size) {
  return raw_libc_.Malloc(size);
}

template<class LIBC>
void MemorySpy<LIBC>::RawFree(void *aptr) {
  return raw_libc_.Free(aptr);
}

template<class LIBC>
void MemorySpy<LIBC>::StartSpying() {
  Initialize();
  spying_ = true;
}

template<class LIBC>
void MemorySpy<LIBC>::StopSpying() {
  spying_ = false;
}

template<class LIBC>
std::vector<std::string> MemorySpy<LIBC>::Issues() {
  return SafeInternallCall<std::vector<std::string>>(
      [](auto &inst) {
        return inst.SpyIssues();
      },
      []() {
        return std::vector<std::string>();
      });
}

template<class LIBC>
typename MemorySpy<LIBC>::IssuesList MemorySpy<LIBC>::SpyIssues() {
  IssuesList result;
  std::vector<MemoryEntry> temp;
  std::remove_copy_if(
      begin(entries_),
      end(entries_),
      std::back_inserter(temp),
      [](const auto &entry) {
        return not entry.IsValid();
      });;
  std::transform(
      begin(temp),
      end(temp),
      std::back_inserter(result),
      [](const auto &entry) {
        return entry.Message("memory allocated at {PTR} of size {SIZE} was not properly freed");
      });
  return result;
}

template<class LIBC>
void MemorySpy<LIBC>::InternalAllocationStart() {
  internal_allocation_state_.PushStart();
}

template<class LIBC>
void MemorySpy<LIBC>::InternalAllocationStop() {
  internal_allocation_state_.PopStop();
}

template<class LIBC>
template<typename T, typename Operation, typename DefualtOperation>
T MemorySpy<LIBC>::SafeInternallCall(Operation op, DefualtOperation defaultResult) {
  //TODO does not work with void* static_assert(std::is_void<T>::value,"cannot use call with void return type instead use safe_internal_proc");
  if (readyForSpying()) {
    Instance().InternalAllocationStart();
    auto result = op(Instance());
    Instance().InternalAllocationStop();
    return result;
  } else {
    return defaultResult();
  }
}

//TODO and not recording suspended
template<class LIBC>
bool MemorySpy<LIBC>::InternalAllocationOn() {
  return internal_allocation_state_.State() or not spying_;
}

template<class LIBC>
template<typename Operation, typename DefualtOperation>
void MemorySpy<LIBC>::SafeInternallProc(Operation op, DefualtOperation defaultResult) {
  if (readyForSpying()) {
    Instance().InternalAllocationStart();
    op(Instance());
    Instance().InternalAllocationStop();
  } else {
    defaultResult();
  }
}

template<class LIBC>
void MemorySpy<LIBC>::Initialize() {
  if (not initialized_) {
    Instance().InternalAllocationOn();
    initialized_ = Instance().libc_.Initialize();
    Instance().InternalAllocationStop();
  }
}

#endif //MEMORYLEAKDETECTION_MEMORYSPY_H
