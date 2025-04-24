#include "EngineCore/Pch.h"
#include "Profiler.h"

Profiler& Profiler::GetInstance()
{
    static Profiler instance;
    return instance;
}

void Profiler::BeginSample(const std::string& sampleName)
{
    begins.insert_or_assign(sampleName, std::chrono::high_resolution_clock::now());
}

void Profiler::EndSample(const std::string& sampleName)
{
    ends.insert_or_assign(sampleName, std::chrono::high_resolution_clock::now());
}

float Profiler::GetDeltaTime(const std::string& sampleName)
{
    const auto begin = begins.find(sampleName);
    const auto end = ends.find(sampleName);
    if (begin == begins.end() || end == ends.end()) return -1;

    return std::chrono::duration<float>(end->second - begin->second).count();
}
