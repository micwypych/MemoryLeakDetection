//
// Created by mwypych on 07.02.17.
//

#ifndef MEMORYLEAKDETECTION_LIBCDELEGATOR_H
#define MEMORYLEAKDETECTION_LIBCDELEGATOR_H

#include "LibC.h"
class GnuLibCDelegator : public virtual LibC {
 public:
  typedef void *(*libc_malloc_t)(size_t);
  typedef void (*libc_free_t)(void *);

  bool Initialize() override;
  void *Malloc(size_t nbytes) override;
  void Free(void *aptr) override;

};
#endif //MEMORYLEAKDETECTION_LIBCDELEGATOR_H
