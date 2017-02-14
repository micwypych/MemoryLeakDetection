//
// Created by mwypych on 07.02.17.
//

#ifndef MEMORYLEAKDETECTION_MEMORYLEAKDETECTOR_H
#define MEMORYLEAKDETECTION_MEMORYLEAKDETECTOR_H

#include <cstddef>

void start_spying();
void stop_spying();
void clear_state();

void *malloc(size_t size);
void free(void *aptr);


#endif //MEMORYLEAKDETECTION_MEMORYLEAKDETECTOR_H
