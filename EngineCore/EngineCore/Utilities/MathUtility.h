#pragma once

#include <glm/glm.hpp>

void CalculateDirection(glm::vec3* direction, const glm::vec3& rotation);
void Normalize(glm::vec3* vector);