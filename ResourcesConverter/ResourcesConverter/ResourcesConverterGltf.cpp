#include <tiny_gltf.h>

#include "EngineCore/Pch.h"
#include "ResourcesConverterGltf.h"
#include "EngineCore/Serialization/EntitySerializer.h"
#include "EngineCore/Utilities/YamlConverters.h"

#include "glm/gtc/type_ptr.hpp"

void ResourcesConverterGltf::ImportFolder(const std::string& inFolder, const std::string& outFolder,
	const std::string& directoryName)
{
	auto outParentPath = std::filesystem::path(outFolder).parent_path();
	CreateFolder(outParentPath);
    
	std::unordered_map<std::string, std::filesystem::path> meshesPaths;
	std::unordered_map<std::string, std::filesystem::path> texturesPaths;

	auto meshesExtensions = std::vector<std::string>();
	meshesExtensions.reserve(1);
	meshesExtensions.emplace_back(".gltf");
	meshesExtensions.emplace_back(".glb");

	auto texturesExtensions = std::vector<std::string>();
	texturesExtensions.reserve(4);
	texturesExtensions.emplace_back(".png");
	texturesExtensions.emplace_back(".jpg");
	texturesExtensions.emplace_back(".JPG");
	texturesExtensions.emplace_back(".jpeg");

	FillPaths(meshesPaths, meshesExtensions, inFolder);
	FillPaths(texturesPaths, texturesExtensions, inFolder);

	for (const auto& meshPath : meshesPaths)
	{
		ImportMesh(meshPath.first, meshPath.second, inFolder, outFolder, directoryName, texturesPaths);
	}

	spdlog::info("Imported folder: {}", inFolder);
}

void ResourcesConverterGltf::ImportMesh(const std::string& meshPathStr, const std::filesystem::path& meshPath,
                                        const std::string& inFolder, const std::string& outFolder, const std::string& directoryName,
                                        std::unordered_map<std::string, std::filesystem::path>& texturesPaths)
{
    tinygltf::Model model;
    tinygltf::TinyGLTF gltfLoader;
    std::string err;
    std::string warn;

	auto binary = meshPathStr.ends_with("glb");
	auto isLoaded = binary
		? gltfLoader.LoadBinaryFromFile(&model, &err, &warn, meshPath.string())
		: gltfLoader.LoadASCIIFromFile(&model, &err, &warn, meshPath.string());

    if (!warn.empty()) spdlog::warn(warn);
    if (!err.empty()) spdlog::error(err);

    if (!isLoaded)
    {
        spdlog::critical("Failed to load glTF by path: {}", meshPathStr);
        return;
    }
	
	auto fileName = meshPath.filename();
	auto meshName = fileName.string().substr(0, fileName.string().find_last_of("."));
	
	std::vector<ConvertingMeshData>* meshes = new std::vector<ConvertingMeshData>();
	meshes->resize(model.materials.size());
	
	std::vector<std::string> serializedMaterialNames;
	serializedMaterialNames.resize(model.materials.size());
	
	//materials
	uint16_t i = 0;
	for (auto& material : model.materials)
	{
		auto& pbr = material.pbrMetallicRoughness;
		
		YAML::Node materialNode;

		auto materialName = ToLowerCase(material.name) + std::to_string(i);
		materialNode["materialName"] = materialName;

		{
			if (pbr.baseColorTexture.index < 0) pbr.baseColorTexture.index = 0;
			auto baseColor = model.textures[pbr.baseColorTexture.index];
			auto baseImage = model.images[baseColor.source];
			BindTextureToMaterial(materialNode, "baseColorTextureName", outFolder, baseImage.uri);
		}
		
		if (pbr.metallicRoughnessTexture.index >= 0)
		{			
			auto image = model.textures[pbr.metallicRoughnessTexture.index];
			auto baseImage = model.images[image.source];
			BindTextureToMaterial(materialNode, "metallicRoughnessTextureName", outFolder, baseImage.uri);
		}

		if (material.normalTexture.index >= 0)
		{
			auto image = model.textures[material.normalTexture.index];
			auto baseImage = model.images[image.source];
			BindTextureToMaterial(materialNode, "normalsTextureName", outFolder, baseImage.uri);
		}

		if (material.occlusionTexture.index >= 0)
		{
			auto image = model.textures[material.occlusionTexture.index];
			auto baseImage = model.images[image.source];
			BindTextureToMaterial(materialNode, "occlusionTextureName", outFolder, baseImage.uri);
		}

		if (material.emissiveTexture.index >= 0)
		{
			auto image = model.textures[material.emissiveTexture.index];
			auto baseImage = model.images[image.source];
			BindTextureToMaterial(materialNode, "emissiveTextureName", outFolder, baseImage.uri);
		}
		
		bool isTransparent = material.alphaMode == "BLEND";
		materialNode["pipelineName"] = isTransparent ? "transparent" : "opaque";
		materialNode["isOpaque"] = !isTransparent;

		materialNode["roughness"] = pbr.roughnessFactor;
		materialNode["metallic"] = pbr.metallicFactor;
		materialNode["baseColor"] = pbr.baseColorFactor;
		
		auto materialPath = outFolder + materialName + ".material";
		std::ofstream fMaterialOut(materialPath);
		fMaterialOut << materialNode;
		fMaterialOut.close();
		serializedMaterialNames.at(i) = materialPath;

		meshes->at(i).materialIndex = i;
		meshes->at(i).meshIndex = i;
		meshes->at(i).materialName = materialName;
		meshes->at(i).materialPath = outFolder + materialName;
		
		i++;
	}

	//meshes
    for (const auto& mesh : model.meshes)
    {
	    for(const auto& primitive : mesh.primitives)
	    {
		    ConvertingMeshData& meshMeta = meshes->at(primitive.material);

	    	tinygltf::Accessor &indicesAccessor = model.accessors[primitive.indices > -1 ? primitive.indices : 0];
	    	tinygltf::Accessor& positionAccessor = model.accessors[primitive.attributes.at("POSITION")];
	    	tinygltf::Accessor& normalAccessor = model.accessors[primitive.attributes.at("NORMAL")];
	    	tinygltf::Accessor& uvAccessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];

	    	const tinygltf::BufferView &indicesView = model.bufferViews[indicesAccessor.bufferView];
	    	const tinygltf::BufferView &positionView = model.bufferViews[positionAccessor.bufferView];
	    	const tinygltf::BufferView &normalView = model.bufferViews[normalAccessor.bufferView];
	    	const tinygltf::BufferView &uvView = model.bufferViews[uvAccessor.bufferView];

	    	const tinygltf::Buffer &indicesBuffer = model.buffers[indicesView.buffer];
			const float* positionBuffer = reinterpret_cast<const float *>(&(model.buffers[positionView.buffer].data[positionAccessor.byteOffset + positionView.byteOffset]));
	    	const float* normalBuffer = reinterpret_cast<const float *>(&(model.buffers[normalView.buffer].data[normalAccessor.byteOffset + normalView.byteOffset]));
	    	const float* uvBuffer = reinterpret_cast<const float *>(&(model.buffers[uvView.buffer].data[uvAccessor.byteOffset + uvView.byteOffset]));

	    	uint64_t positionByteStride = positionAccessor.ByteStride(positionView)
	    		? (positionAccessor.ByteStride(positionView) / sizeof(float))
	    		: tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC3);

	    	uint64_t normalByteStride = normalAccessor.ByteStride(normalView)
	    		? (normalAccessor.ByteStride(normalView) / sizeof(float))
	    		: tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC3);
	    	
	    	uint64_t uvByteStride = uvAccessor.ByteStride(uvView)
				? (uvAccessor.ByteStride(uvView) / sizeof(float))
				: tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC2);

	    	const float* colorBuffer;
	    	uint64_t colorByteStride = 0;
	    	if (primitive.attributes.contains("COLOR_0"))
	    	{
	    		tinygltf::Accessor& colorAccessor = model.accessors[primitive.attributes.at("COLOR_0")];
	    		const tinygltf::BufferView &colorView = model.bufferViews[colorAccessor.bufferView];
	    		colorBuffer = reinterpret_cast<const float *>(&(model.buffers[colorView.buffer].data[colorAccessor.byteOffset + colorView.byteOffset]));

	    		colorByteStride = colorAccessor.ByteStride(colorView)
					? (colorAccessor.ByteStride(colorView) / sizeof(float))
					: tinygltf::GetNumComponentsInType(TINYGLTF_TYPE_VEC2);
	    	}
	    	
	    	uint32_t vertexStart = meshMeta.vertices.size();
	    	for (size_t v = 0; v < positionAccessor.count; v++)
	    	{
	    		Vertex vertex;
	    		vertex.position = glm::vec4(glm::make_vec3(&positionBuffer[v * positionByteStride]), 1.0f);
	    		vertex.normal = glm::normalize(glm::vec3(normalBuffer ? glm::make_vec3(&normalBuffer[v * normalByteStride]) : glm::vec3(0.0f)));
	    		vertex.uv = uvBuffer ? glm::make_vec2(&uvBuffer[v * uvByteStride]) : glm::vec3(0.0f);
	    		//vertex.uv1 = bufferTexCoordSet1 ? glm::make_vec2(&bufferTexCoordSet1[v * uv1ByteStride]) : glm::vec3(0.0f);
	    		vertex.color = colorBuffer != nullptr ? glm::make_vec4(&colorBuffer[v * colorByteStride]) : glm::vec4(1.0f);
	    		
	    		meshMeta.vertices.push_back(vertex);
	    	}

	    	auto indicesCount = static_cast<uint32_t>(indicesAccessor.count);
	    	const void *indicesData = &(indicesBuffer.data[indicesAccessor.byteOffset + indicesView.byteOffset]);

	    	switch (indicesAccessor.componentType)
	    	{
	    		case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
	    				const uint32_t *buf = static_cast<const uint32_t*>(indicesData);
	    				for (size_t index = 0; index < indicesAccessor.count; index++)
	    				{
	    					meshMeta.indices.push_back(buf[index] + vertexStart);
	    				}
	    				break;
	    		}
	    		case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
	    				const uint16_t *buf = static_cast<const uint16_t*>(indicesData);
	    				for (size_t index = 0; index < indicesAccessor.count; index++)
	    				{
	    					meshMeta.indices.push_back(buf[index] + vertexStart);
	    				}
	    				break;
	    		}
	    		case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
	    				const uint8_t *buf = static_cast<const uint8_t*>(indicesData);
	    				for (size_t index = 0; index < indicesAccessor.count; index++)
	    				{
	    					meshMeta.indices.push_back(buf[index] + vertexStart);
	    				}
	    				break;
	    		}
	    		default:
	    			spdlog::critical("Index component type {} is not supported", indicesAccessor.componentType);
	    			return;
	    	}
	    }
    }
	
    //CalculateNormalsTangents(meshes);
        
    //--format meshes
    std::vector<std::string> serializedMeshNames;
    serializedMeshNames.resize(meshes->size());
        
    std::for_each(std::execution::par, meshes->begin(), meshes->end(),
        [this, &meshPathStr,  &inFolder, &outFolder, &serializedMeshNames](ConvertingMeshData& meshIt)
    {
        auto fileNameWithoutExtension = meshPathStr.substr(0, meshPathStr.find_last_of("."));
        auto assetName = ToLowerCase(fileNameWithoutExtension + "_" + meshIt.materialName);

        if (meshIt.vertices.empty() || meshIt.indices.empty())
        {
            spdlog::warn("Mesh {} has {} vertices and {} indices -> it will not be serialized", assetName, meshIt.vertices.size(), meshIt.indices.size());
            return;
        }

        // Serialize mesh to binary
        auto serialized = SerializeMesh(outFolder + assetName + ".mesh", meshIt);
        if (serialized)
        {
            serializedMeshNames.at(meshIt.materialIndex) = outFolder + assetName + ".mesh";
            spdlog::info("Mesh {} saved to folder {}", assetName, outFolder);
        }
    });

    std::for_each(std::execution::par, texturesPaths.begin(), texturesPaths.end(), [this, &inFolder, &outFolder](const auto& pathIter)
    {
        auto textureName = ToLowerCase(FormatTextureName(pathIter.second.string(), inFolder));
        std::filesystem::copy(pathIter.second.string(), outFolder + textureName, std::filesystem::copy_options::overwrite_existing);
    });

    WriteSerializedMeshNames(outFolder + directoryName + "_" + meshName +  ".combinedmesh", serializedMeshNames, serializedMaterialNames);
        
    //clean
    delete meshes;
}

