

#include "EngineCore/Pch.h"
#include "EngineCore/Assets/Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_USE_CPP14
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>


Mesh::Mesh(const std::filesystem::path& path)
{
	auto fileExtension = path.extension();

	if (fileExtension == ".obj") LoadAndInitObj(path);
	else if (fileExtension == ".gltf") LoadAndInitGltf(path);
	else spdlog::critical("Mesh can't be loaded, as extension {} is not supported", fileExtension.string());
}

Mesh::Mesh(Ref<std::vector<Vertex>> vertices, Ref<std::vector<uint32_t>> indices)
{
	this->vertices = vertices;
	this->indices = indices;
}

Mesh::~Mesh()
{
}

void Mesh::LoadAndInitObj(const std::filesystem::path& path)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	auto isLoaded = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.string().c_str());
	if (!warn.empty()) spdlog::warn(warn);
	if (!err.empty()) spdlog::error(err);
	if (!isLoaded) spdlog::error("Failed to load obj");

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

			if (index.normal_index != -1)
			{
				vertex.normal =
				{
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
				};
			}

			if (index.texcoord_index != -1)
			{
				vertex.uv =
				{
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};
			}

			vertex.color = { 1.0f, 1.0f, 1.0f };

			vertices->push_back(vertex);
			indices->push_back(static_cast<uint32_t>(indices->size()));
		}
	}
}

void Mesh::LoadAndInitGltf(const std::filesystem::path& path)
{
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool isLoaded = loader.LoadASCIIFromFile(&model, &err, &warn, path.string());

    if (!warn.empty()) spdlog::warn(warn);
    if (!err.empty()) spdlog::error(err);
    if (!isLoaded) {
        spdlog::error("Failed to load glTF");
        return;
    }

    // Initialize vertices and indices
    vertices = CreateRef<std::vector<Vertex>>();
    indices = CreateRef<std::vector<uint32_t>>();

    // Iterate over meshes
    for (const auto& mesh : model.meshes)
    {
        for (const auto& primitive : mesh.primitives)
        {
            // Positions
            const tinygltf::Accessor& posAccessor = model.accessors[primitive.attributes.find("POSITION")->second];
            const tinygltf::BufferView& posView = model.bufferViews[posAccessor.bufferView];
            const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
            const float* positions = reinterpret_cast<const float*>(&posBuffer.data[posView.byteOffset + posAccessor.byteOffset]);
            size_t posStride = posView.byteStride ? posView.byteStride / sizeof(float) : 3;

            // Normals
            const float* normals = nullptr;
            size_t normStride = 0;
            if (primitive.attributes.find("NORMAL") != primitive.attributes.end())
            {
                const tinygltf::Accessor& normAccessor = model.accessors[primitive.attributes.find("NORMAL")->second];
                const tinygltf::BufferView& normView = model.bufferViews[normAccessor.bufferView];
                const tinygltf::Buffer& normBuffer = model.buffers[normView.buffer];
                normals = reinterpret_cast<const float*>(&normBuffer.data[normView.byteOffset + normAccessor.byteOffset]);
                normStride = normView.byteStride ? normView.byteStride / sizeof(float) : 3;
            }

            // UVs
            const float* uvs = nullptr;
            size_t uvStride = 0;
            if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end())
            {
                const tinygltf::Accessor& uvAccessor = model.accessors[primitive.attributes.find("TEXCOORD_0")->second];
                const tinygltf::BufferView& uvView = model.bufferViews[uvAccessor.bufferView];
                const tinygltf::Buffer& uvBuffer = model.buffers[uvView.buffer];
                uvs = reinterpret_cast<const float*>(&uvBuffer.data[uvView.byteOffset + uvAccessor.byteOffset]);
                uvStride = uvView.byteStride ? uvView.byteStride / sizeof(float) : 2;
            }

            // Colors
            const float* colors = nullptr;
            size_t colorStride = 0;
            if (primitive.attributes.find("COLOR_0") != primitive.attributes.end())
            {
                const tinygltf::Accessor& colorAccessor = model.accessors[primitive.attributes.find("COLOR_0")->second];
                const tinygltf::BufferView& colorView = model.bufferViews[colorAccessor.bufferView];
                const tinygltf::Buffer& colorBuffer = model.buffers[colorView.buffer];
                colors = reinterpret_cast<const float*>(&colorBuffer.data[colorView.byteOffset + colorAccessor.byteOffset]);
                colorStride = colorView.byteStride ? colorView.byteStride / sizeof(float) : 3;
            }

            // Add vertices
            for (size_t i = 0; i < posAccessor.count; ++i)
            {
                Vertex vertex = {};

                // Position
                vertex.position = glm::vec3(
                    positions[i * posStride + 0],
                    positions[i * posStride + 1],
                    positions[i * posStride + 2]
                );

                // Normal
                if (normals) {
                    vertex.normal = glm::vec3(
                        normals[i * normStride + 0],
                        normals[i * normStride + 1],
                        normals[i * normStride + 2]
                    );
                }

                // UV
                if (uvs) {
                    vertex.uv = glm::vec2(
                        uvs[i * uvStride + 0],
                        uvs[i * uvStride + 1]
                    );
                }

                // Color
                if (colors) {
                    vertex.color = glm::vec3(
                        colors[i * colorStride + 0],
                        colors[i * colorStride + 1],
                        colors[i * colorStride + 2]
                    );
                }
                else {
                    vertex.color = glm::vec3(1.0f, 1.0f, 1.0f); // Default white color
                }

                vertices->push_back(vertex);
            }

            // Extract indices
            if (primitive.indices >= 0)
            {
                const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
                const tinygltf::BufferView& indexView = model.bufferViews[indexAccessor.bufferView];
                const tinygltf::Buffer& indexBuffer = model.buffers[indexView.buffer];

                if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
                {
                    const uint16_t* indicesData = reinterpret_cast<const uint16_t*>(&indexBuffer.data[indexView.byteOffset + indexAccessor.byteOffset]);
                    for (size_t i = 0; i < indexAccessor.count; ++i) {
                        indices->push_back(indicesData[i]);
                    }
                }
                else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
                {
                    const uint32_t* indicesData = reinterpret_cast<const uint32_t*>(&indexBuffer.data[indexView.byteOffset + indexAccessor.byteOffset]);
                    for (size_t i = 0; i < indexAccessor.count; ++i) {
                        indices->push_back(indicesData[i]);
                    }
                }
            }
        }
    }
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
