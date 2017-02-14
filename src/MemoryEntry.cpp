//
// Created by mwypych on 07.02.17.
//

#include <sstream>
#include "MemoryEntry.h"

MemoryEntry::MemoryEntry(const void *memory_ptr, const size_t memory_size)
    : memory_ptr{memory_ptr}, memory_size{memory_size}, validity{true} {}

MemoryEntry::MemoryEntry() : memory_ptr(nullptr), memory_size{0}, validity{false} {

}

bool MemoryEntry::is_valid() const {
  return validity;
}

bool MemoryEntry::points_to(const void *ptr) const {
  return memory_ptr == ptr;
}

void MemoryEntry::invalidate() {
  validity = false;
}

std::string MemoryEntry::message() const {
  if (is_valid()) {
    std::stringstream ss;
    ss << "memory allocated at ";
    ss << memory_ptr;
    ss << " of size ";
    ss << memory_size;
    return ss.str();
  } else {
    return "";
  }
}
MemoryEntry MemoryEntry::operator=(const MemoryEntry &entry) const {
  if (entry.is_valid()) {
    return MemoryEntry(entry.memory_ptr, entry.memory_size);
  } else {
    return {};
  }
}
