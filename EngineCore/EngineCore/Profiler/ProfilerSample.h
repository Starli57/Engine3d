#pragma once

namespace EngineCore
{
    struct ProfilerSample
    {
        std::string sampleName;
        std::chrono::steady_clock::time_point beginTime;
        std::chrono::steady_clock::time_point endTime;
        std::vector<ProfilerSample> childrenSamples;
        ProfilerSample* parentSample;

        ProfilerSample();
        ProfilerSample(std::string&& sampleName, std::chrono::steady_clock::time_point beginTime, ProfilerSample* parentSample);
    };
}