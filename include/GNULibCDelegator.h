//
// Created by mwypych on 07.02.17.
//

#ifndef MEMORYLEAKDETECTION_LIBCDELEGATOR_H
#define MEMORYLEAKDETECTION_LIBCDELEGATOR_H

#include "LibC.h"
class GNULibCDelegator : public virtual LibC {
 public:
  typedef void *(*libc_malloc_t)(size_t);
  typedef void (*libc_free_t)(void *);

  bool initialize() override;
  void *malloc(size_t nbytes) override;
  void free(void *aptr) override;

};
#endif //MEMORYLEAKDETECTION_LIBCDELEGATOR_H
