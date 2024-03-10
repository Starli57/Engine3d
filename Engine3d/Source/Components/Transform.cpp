#include "Pch.h"
#include "Transform.h"

//todo: reduce duplicatons in constructors
Transform::Transform()
{
	this->position = glm::vec3(0, 0, 0);
	this->rotation = glm::vec4(0, 0, 0, 0);
	this->scale = glm::vec3(1, 1, 1);

	uboModel.model = glm::mat4(1.0f);
	uboModel.model = glm::translate(uboModel.model, position);
	uboModel.model = glm::scale(uboModel.model, scale);
}

Transform::Transform(glm::vec3 position, glm::vec4 rotation, glm::vec3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	uboModel.model = glm::mat4(1.0f);
	uboModel.model = glm::translate(uboModel.model, position);
	uboModel.model = glm::scale(uboModel.model, scale);
}

UboModel Transform::GetUboModel()
{
	return uboModel;
}

void Transform::TransformUboModel(UboModel& model)
{
	uboModel.model = model.model;
}
