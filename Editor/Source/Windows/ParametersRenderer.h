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
	void RenderParameter(const char* label, int& parameter, int step = 1);
	void RenderParameter(const char* label, uint32_t& parameter, uint32_t step = 1);
	void RenderParameter(const char* label, std::optional<int>& parameter, int step = 1);
	void RenderParameter(const char* label, float& parameter, float v_speed = 0.1f, float v_min = 0, float v_max = 0);
	void RenderParameter(const char* label, glm::vec2& parameter, float v_speed = 0.1f, float v_min = 0, float v_max = 0);
	void RenderParameter(const char* label, glm::vec3& parameter, float v_speed = 0.1f, float v_min = 0, float v_max = 0);
	void RenderParameter(std::string& parameter);
	void RenderParameter(const char* label, std::filesystem::path& parameter,
		std::unordered_map<std::string, std::filesystem::path>& pathes);
};