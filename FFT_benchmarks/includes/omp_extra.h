#pragma once

#include "../includes/common.h"

void manage_thread_affinity();

template<class BinType>
void reduce( BinType** arrs, uint64_t bins, uint64_t begin, uint64_t end);