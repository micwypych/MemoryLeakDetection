//
// Created by mwypych on 07.02.17.
//

#ifndef MEMORYLEAKDETECTION_MEMORYENTRY_H
#define MEMORYLEAKDETECTION_MEMORYENTRY_H

#include <cstddef>
#include <string>
class MemoryEntry {
 public:
  MemoryEntry(const void *memory_ptr, const size_t memory_size);
  MemoryEntry();

  virtual bool IsValid() const;
  virtual bool PointsTo(const void *ptr) const;
  virtual void Invalidate();

  ///
  /// \param pattern  {PTR} is replaced with address of the internal pointer and {SIZE} is replaced with memory size
  /// \return pattern with value substitution if entry is valid or empty string otherwise
  virtual std::string Message(std::string pattern) const;

  MemoryEntry operator=(const MemoryEntry &entry) const;

 private:
  const void *memory_ptr;
  size_t memory_size;
  bool validity;
  static std::string ToString(const void *address);
  static std::string ToString(size_t size);
};

#endif //MEMORYLEAKDETECTION_MEMORYENTRY_H
