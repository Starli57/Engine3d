#include "EngineCore/Pch.h"
#include "ProfilerSample.h"

namespace EngineCore
{
    ProfilerSample::ProfilerSample()
    {
        this->parentSample = nullptr;
    }

    ProfilerSample::ProfilerSample(std::string&& sampleName, const std::chrono::steady_clock::time_point beginTime, ProfilerSample* parentSample)
    {
        this->sampleName = std::move(sampleName);
        this->beginTime = beginTime;
        this->parentSample = parentSample;
    }
}