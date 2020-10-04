#include "Model.h"

Model::Model(const std::string& path, bool gamma) :
	gammaCorrection(gamma)
{
	loadModel(path);
}

void Model::Draw(Shader& shader)
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;

	/*
	* Read the file. 
	* Assimp's interface defines each mesh as having an array of faces, where each face
	* is represented by a single primitive. The aiProcess_Triangulate flag defines the 
	* primitives as triangles.
	*/
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
	{
		std::cout << "ERROR::MODEL::LOAD_MODEL::ASSIMP" << std::endl;
		std::cout << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// First process all the node's meshes (if any)
	for (std::size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// Then process all of the node's children
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Mesh::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Mesh::Texture> textures;

	/*
	* Process the vertices.
	* Construct the position, normal and texture vectors.
	*/
	for (std::size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Mesh::Vertex vertex;

		// Construct the position vector for this vertex
		glm::vec3 positionVector;
		positionVector.x = mesh->mVertices[i].x;
		positionVector.y = mesh->mVertices[i].y;
		positionVector.z = mesh->mVertices[i].z;
		vertex.position = positionVector;

		// Construct the normal vector for this vertex
		glm::vec3 normalVector;
		normalVector.x = mesh->mNormals[i].x;
		normalVector.y = mesh->mNormals[i].y;
		normalVector.z = mesh->mNormals[i].z;
		vertex.normal = normalVector;

		// If the texture coordinates exist, construct the texture vector for this vertex
		// The index 0 is because Assimp allows for up to 8 differrent sets of texture coordinates
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 textureVector;
			textureVector.x = mesh->mTextureCoords[0][i].x;
			textureVector.y = mesh->mTextureCoords[0][i].y;
			vertex.textureCoordinates = textureVector;
		}
		else
		{
			vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);
		}
	}
	
	/*
	* Process the mesh faces i.e. the primitives.
	* In this case the primitives are triangles (due to the aiProcess_Triangulate flag).
	* For further explanation look at the Model::loadModel method.
	*/
	for (std::size_t i = 0; i < mesh->mNumFaces; i++)
	{
		// Get a face from the mesh
		aiFace face = mesh->mFaces[i];

		// Loop through the indices of this face
		for (std::size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// Load the diffuse textures
		std::vector<Mesh::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, 
			TEXTYPEenum::DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end()); // Insert the diffuse texture map to the end of textures struct

		// Load the specular textures
		std::vector<Mesh::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TEXTYPEenum::SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end()); // Insert the specular texture map to the end of textures struct
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Mesh::Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType aiType, TEXTYPEenum txType)
{
	std::vector<Mesh::Texture> textures;
	for (std::size_t i = 0; i < material->GetTextureCount(aiType); i++)
	{
		aiString path;
		material->GetTexture(aiType, i, &path);

		Mesh::Texture texture;
		texture.id = Mesh::LoadMaterialTextureFromFile(path.C_Str(), directory, gammaCorrection);
		texture.type = txType;
		texture.path = path.C_Str();
		textures.push_back(texture);
	}

	return textures;
}