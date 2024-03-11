#include "Pch.h"
#include "Rotator.h"

//todo: make dependency transparent: Transform
Rotator::Rotator(Ref<Entity> entity) : entity(entity)
{
}

void Rotator::Update()
{
	//todo: replace
	static auto startTime = std::chrono::high_resolution_clock::now();

	//todo: replace
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	
	auto transform = entity.get()->GetComponent<Transform>();
	auto transformModel = transform.GetUboModel();
	transformModel.model = glm::rotate(transformModel.model, time * glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	transform.TransformUboModel(transformModel);
}
