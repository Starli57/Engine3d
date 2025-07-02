#pragma once

#include <atomic>

namespace EngineCore
{
    class UniqueIdGenerator
    {
    public:

        static uint32_t Generate();

    private:

        static std::atomic<uint32_t> globalId;

    };

    struct UniqueId 
    {
    public:
        uint32_t Get() const { return id; }

        UniqueId() { id = UniqueIdGenerator::Generate(); }

        bool operator==(const UniqueId& other)  const { return id == other.id; }
        bool equals(const UniqueId& other)      const { return id == other.id; }

    private:
        uint32_t id;
    };
}