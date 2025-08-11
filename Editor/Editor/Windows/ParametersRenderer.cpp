
#include "ParametersRenderer.h"

bool ParametersRenderer::RenderParameter(const char* label, int& parameter, int step, const int min, const int max) const
{
    int buf = parameter;
    if (ImGui::InputInt(label, &buf))
    {
        parameter = std::clamp<int>(buf, min, max);
        return true;
    }
    return false;
}

bool ParametersRenderer::RenderParameter(const char* label, uint32_t& parameter, uint32_t step, const uint32_t min, const uint32_t max) const
{
    int buf = static_cast<int>(parameter);
    if (ImGui::InputInt(label, &buf))
    {
        parameter = std::clamp<int32_t>(buf, min, max);
        return true;
    }
    return false;
}

bool ParametersRenderer::RenderParameter(const char* label, std::optional<int>& parameter, int step, const int min, const int max) const
{
    int buf = parameter.has_value() ? parameter.value() : 0;
    if (ImGui::InputInt(label, &buf))
    {
        parameter = std::clamp<int>(buf, min, max);
        return true;
    }
    return false;
}

bool ParametersRenderer::RenderParameter(const char* label, float& parameter, float vSpeed, const float vMin, const float vMax) const
{
	return ImGui::DragFloat(label, &parameter, vSpeed, vMin, vMax, "%.1f");
}

bool ParametersRenderer::RenderParameter(const char* label, glm::vec2& parameter, float vSpeed, const float vMin, const float vMax) const
{
	return ImGui::DragFloat2(label, glm::value_ptr(parameter), vSpeed, vMin, vMax, "%.1f");
}

bool ParametersRenderer::RenderParameter(const char* label, glm::vec3& parameter, float vSpeed, const float vMin, const float vMax) const
{
	return ImGui::DragFloat3(label, glm::value_ptr(parameter), vSpeed, vMin, vMax, "%.1f");
}

bool ParametersRenderer::RenderParameter(std::string& parameter) const
{
    char buffer[64];
    strncpy_s(buffer, parameter.c_str(), sizeof(buffer));
    if (ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer)))
    {
        parameter = std::string(buffer);
        return true;
    }
    return false;
}

void ParametersRenderer::RenderParameter(const char* label, const std::filesystem::path& parameter,
                                         const std::unordered_map<std::string, std::filesystem::path>& paths) const
{
    ImGui::Text(label);
    
    auto filename = parameter.filename();
    ImGui::SameLine();
    ImGui::Text(filename.string().c_str());

    if (ImGui::Button("Change.."))
        ImGui::OpenPopup("ChangePopup");

    if (ImGui::BeginPopup("ChangePopup"))
    {
        for (auto it : paths)
        {
            if (ImGui::Selectable(it.first.c_str()))
            {
                ;
            }
        }

        ImGui::EndPopup();
    }
}

bool ParametersRenderer::RenderColorParameter(const char* label, glm::vec3& color) const
{
    return ImGui::ColorEdit3(label, glm::value_ptr(color));
}
