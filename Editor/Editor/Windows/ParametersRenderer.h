#pragma once

#include <imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <unordered_map>
#include <filesystem>
#include <optional>

class ParametersRenderer
{
public:
	bool RenderParameter(const char* label, int& parameter, int step = 1, int min = 0, int max = INT_MAX) const;
	bool RenderParameter(const char* label, uint32_t& parameter, uint32_t step = 1, uint32_t min = 0, uint32_t max = UINT32_MAX) const;
	bool RenderParameter(const char* label, std::optional<int>& parameter, int step = 1, int min = 0, int max = INT_MAX) const;
	bool RenderParameter(const char* label, float& parameter, float vSpeed = 0.1f, float vMin = 0, float vMax = 0) const;
	bool RenderParameter(const char* label, glm::vec2& parameter, float vSpeed = 0.1f, float vMin = 0, float vMax = 0) const;
	bool RenderParameter(const char* label, glm::vec3& parameter, float vSpeed = 0.1f, float vMin = 0, float vMax = 0) const;
	bool RenderParameter(std::string& parameter) const;
	void RenderParameter(const char* label, const std::filesystem::path& parameter,
	                     const std::unordered_map<std::string, std::filesystem::path>& paths) const;
	bool RenderColorParameter(const char* label, glm::vec3& color) const;
};