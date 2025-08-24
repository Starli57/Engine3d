#include "EngineCore/Pch.h"
#include "Profiler.h"

namespace Engine
{
    Profiler& Profiler::GetInstance()
    {
        static Profiler instance;
        return instance;
    }

    Profiler::Profiler()
    {
        samples = std::vector<ProfilerSample>();
        currentSample = nullptr;
    }

    void Profiler::BeginSample(std::string&& sampleName)
    {
        if (currentSample == nullptr)
        {
            auto newSample = ProfilerSample(std::move(sampleName), std::chrono::high_resolution_clock::now(), nullptr);
            samples.emplace_back(std::move(newSample));
            currentSample = &samples.back();
        }
        else
        {
            auto newSample = ProfilerSample(std::move(sampleName), std::chrono::high_resolution_clock::now(), currentSample);
            currentSample->childrenSamples.emplace_back(std::move(newSample));
            currentSample = &currentSample->childrenSamples.back();
        }
    }

    void Profiler::EndSample()
    {
        if (currentSample == nullptr) throw std::runtime_error("Profiler::EndSample: No sample found");
        currentSample->endTime = std::chrono::high_resolution_clock::now();
        currentSample = currentSample->parentSample;
    }

    void Profiler::AddDrawCall()
    {
        drawCalls++;
    }

    uint16_t Profiler::GetDrawCalls() const
    {
        return drawCalls;
    }

    void Profiler::AddTrianglesCount(uint32_t count)
    {
        trianglesCount += count;
    }

    uint32_t Profiler::GetTrianglesCount() const
    {
        return trianglesCount;
    }

    void Profiler::Reset()
    {
        samples.clear();
        currentSample = nullptr;
        drawCalls = 0;
        trianglesCount = 0;
    }

    float Profiler::GetDeltaTime(const ProfilerSample& sample) const
    {
        return std::chrono::duration<float>(sample.endTime - sample.beginTime).count();
    }
}