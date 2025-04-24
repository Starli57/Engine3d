#pragma once

class Profiler
{
public:
    static Profiler& GetInstance();
    
    void BeginSample(const std::string& sampleName);
    void EndSample(const std::string& sampleName);
    
    float GetDeltaTime(const std::string& sampleName);
    
private:
    std::unordered_map<std::string, std::chrono::steady_clock::time_point> begins;
    std::unordered_map<std::string, std::chrono::steady_clock::time_point> ends;
};
