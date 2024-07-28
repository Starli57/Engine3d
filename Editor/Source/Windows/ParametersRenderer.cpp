
#include "ParametersRenderer.h"

void ParametersRenderer::RenderParameter(const char* label, int& parameter, int step)
{
	ImGui::InputInt(label, &parameter, step);
}

void ParametersRenderer::RenderParameter(const char* label, uint32_t& parameter, uint32_t step)
{
    int buf = static_cast<int>(parameter);
    if (ImGui::InputInt(label, &buf))
        parameter = static_cast<int32_t>(buf);
}

void ParametersRenderer::RenderParameter(const char* label, std::optional<int>& parameter, int step)
{
    ImGui::InputInt(label, &parameter.value(), step);
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

void ParametersRenderer::RenderParameter(const char* label, std::filesystem::path& parameter, 
    std::unordered_map<std::string, std::filesystem::path>& pathes)
{
    ImGui::Text(label);
    
    auto filename = parameter.filename();
    ImGui::SameLine();
    ImGui::Text(filename.string().c_str());

    if (ImGui::Button("Change.."))
        ImGui::OpenPopup("ChangePopup");

    if (ImGui::BeginPopup("ChangePopup"))
    {
        for (auto it : pathes)
        {
            if (ImGui::Selectable(it.first.c_str()))
            {
                ;
            }
        }

        ImGui::EndPopup();
    }
}