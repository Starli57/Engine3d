
#include "UniqueId.h"

std::atomic<uint32_t> UniqueIdGenerator::globalId{ 0 };

uint32_t UniqueIdGenerator::Generate()
{
    return globalId.fetch_add(1, std::memory_order_relaxed);
}