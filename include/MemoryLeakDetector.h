//
// Created by mwypych on 07.02.17.
//

#ifndef MEMORYLEAKDETECTION_MEMORYLEAKDETECTOR_H
#define MEMORYLEAKDETECTION_MEMORYLEAKDETECTOR_H

#include <cstddef>

#include <vector>
#include <string>

namespace memspy {

void StartSpying();
void StopSpying();
void ClearState();

std::vector<std::string> Issues();
bool Verify();
}

void *malloc(size_t size);
void free(void *aptr);

#endif //MEMORYLEAKDETECTION_MEMORYLEAKDETECTOR_H
