//
// Created by mwypych on 07.02.17.
//

#ifndef MEMORYLEAKDETECTION_LIBC_H
#define MEMORYLEAKDETECTION_LIBC_H

#include <cstdlib>

class LibC {
 public:
  virtual bool initialize() =0;
  virtual void *malloc(size_t nbytes) =0;
//  virtual void *realloc(void *aptr, size_t nbytes ) =0;
//  virtual void *reallocf(void *aptr, size_t nbytes ) =0;
  virtual void free(void *aptr) =0;
//
//  virtual void *memalign(size_t align, size_t nbytes ) =0;rrc
//
//  virtual size_t malloc_usable_size(void *aptr) =0;
//
//  virtual void *_malloc_r(void *reent, size_t nbytes) =0;
//  virtual void *_realloc_r(void *reent, void *aptr, size_t nbytes) =0;
//  virtual void *_reallocf_r(void *reent, void *aptr, size_t nbytes) =0;
//  virtual void _free_r(void *reent, void *aptr) =0;
//  virtual void *_memalign_r(void *reent, size_t align, size_t nbytes) =0;
//  virtual size_t _malloc_usable_size_r(void *reent, void *aptr) =0;
};

#endif //MEMORYLEAKDETECTION_LIBC_H
