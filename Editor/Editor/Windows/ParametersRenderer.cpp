
#include "ParametersRenderer.h"

void ParametersRenderer::RenderParameter(const char* label, int& parameter, int step, int min, int max)
{
    int buf = static_cast<int>(parameter);
    if (ImGui::InputInt(label, &buf))
        parameter = std::clamp<int>(static_cast<int>(buf), min, max);
}

void ParametersRenderer::RenderParameter(const char* label, uint32_t& parameter, uint32_t step, uint32_t min, uint32_t max)
{
    int buf = static_cast<int>(parameter);
    if (ImGui::InputInt(label, &buf))
        parameter = std::clamp<int32_t>(static_cast<int32_t>(buf), min, max);
}

void ParametersRenderer::RenderParameter(const char* label, std::optional<int>& parameter, int step, int min, int max)
{
    int buf = parameter.has_value() ? static_cast<int>(parameter.value()) : 0;
    if (ImGui::InputInt(label, &buf))
        parameter = std::clamp<int>(static_cast<int>(buf), min, max);
}

void ParametersRenderer::RenderParameter(const char* label, float& parameter, float v_speed, float v_min, float v_max)
{
	ImGui::DragFloat(label, &parameter, v_speed, v_min, v_max, "%.1f");
}

void ParametersRenderer::RenderParameter(const char* label, glm::vec2& parameter, float v_speed, float v_min, float v_max)
{
	ImGui::DragFloat2(label, glm::value_ptr(parameter), v_speed, v_min, v_max, "%.1f");
}

void ParametersRenderer::RenderParameter(const char* label, glm::vec3& parameter, float v_speed, float v_min, float v_max)
{
	ImGui::DragFloat3(label, glm::value_ptr(parameter), v_speed, v_min, v_max, "%.1f");
}

void ParametersRenderer::RenderParameter(std::string& parameter)
{
    char buffer[64];
    strncpy_s(buffer, parameter.c_str(), sizeof(buffer));
    if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer)))
        parameter = std::string(buffer);
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