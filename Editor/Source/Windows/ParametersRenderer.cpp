
#include "ParametersRenderer.h"

void ParametersRenderer::RenderParameter(const char* label, int& parameter, int step)
{
	ImGui::InputInt(label, &parameter, step);
}

void ParametersRenderer::RenderParameter(const char* label, float& parameter, float v_speed, float v_min, float v_max)
{
	ImGui::DragFloat(label, &parameter, v_speed, v_min, v_max);
}

void ParametersRenderer::RenderParameter(const char* label, glm::vec2& parameter, float v_speed, float v_min, float v_max)
{
	ImGui::DragFloat2(label, glm::value_ptr(parameter), v_speed, v_min, v_max);
}

void ParametersRenderer::RenderParameter(const char* label, glm::vec3& parameter, float v_speed, float v_min, float v_max)
{
	ImGui::DragFloat3(label, glm::value_ptr(parameter), v_speed, v_min, v_max);
}

void ParametersRenderer::RenderParameter(std::string& parameter)
{
	ImGui::Text(parameter.c_str());
}