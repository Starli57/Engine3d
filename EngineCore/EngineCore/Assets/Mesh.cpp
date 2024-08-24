
#include "EngineCore/Pch.h"
#include "EngineCore/Assets/Mesh.h"

#include <tiny_obj_loader.h>

Mesh::Mesh(const std::filesystem::path& path)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	auto isLoaded = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.string().c_str());
	CAssert::Check(isLoaded, warn + err);

	vertices = CreateRef<std::vector<Vertex>>();
	indices = CreateRef<std::vector<uint32_t>>();
	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vertex;

			vertex.position =
			{
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.normal =
			{
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};

			vertex.uv =
			{
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			vertices->push_back(vertex);
			indices->push_back(static_cast<uint32_t>(indices->size()));
		}
	}

}

Mesh::Mesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices)
{
	this->vertices = vertices;
	this->indices = indices;
}

Mesh::~Mesh()
{
}

Ref<std::vector<Vertex>> Mesh::GetVertices()
{
	return vertices;
}

Ref<std::vector<uint32_t>> Mesh::GetIndices()
{
	return indices;
}

size_t Mesh::GetVerticesCount()
{
	return vertices->size();
}

size_t Mesh::GetIndicesCount()
{
	return indices->size();
}
