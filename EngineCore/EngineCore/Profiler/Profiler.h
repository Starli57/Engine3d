#pragma once
#include "ProfilerSample.h"

namespace Engine
{
    class Profiler
    {
    public:
        static Profiler& GetInstance();
    
        std::vector<ProfilerSample> samples;
        ProfilerSample* currentSample;
        
        Profiler();
    
        void BeginSample(std::string&& sampleName);
        void EndSample();

        void AddDrawCall();
        uint16_t GetDrawCalls() const;

        void AddTrianglesCount(uint32_t count);
        uint32_t GetTrianglesCount() const;
        
        void Reset();
    
        float GetDeltaTime(const ProfilerSample& sample) const;

    private:
        uint16_t drawCalls;
        uint32_t trianglesCount;
    };
}