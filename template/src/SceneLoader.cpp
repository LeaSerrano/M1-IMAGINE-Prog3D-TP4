#include "SceneLoader.h"
#include "Context.h"
#include "Material.h"
// ASSIMP includes
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/DefaultLogger.hpp>
// STD includes
#include <queue>
#include <vector>
#include <map>
#include <iostream>
#include <filesystem>
// GLM includes
#include <glm/gtx/string_cast.hpp>



// Implementation for your own log stream callback
void myCallback( const char *msg, char *userData) {
	std::cerr << msg << std::endl;
}
// Example stream
class myStream : public Assimp::LogStream {
public:
	// Write womethink using your own functionality
	void write(const char* message) {
		std::cout << message << std::endl;
	}
};

Mesh* createMesh(aiMesh* mesh) {
	Mesh* glMesh = new Mesh();
	glMesh->mNumVertices = mesh->mNumVertices;
	glMesh->mNumFaces = mesh->mNumFaces;

	glMesh->center = glm::vec3();
	for (int i = 0; i < mesh->mNumVertices; ++i) {
		glMesh->center += glm::vec3(mesh->mVertices[i][0], mesh->mVertices[i][1], mesh->mVertices[i][2]);
	}
	glMesh->center *= (1.0/(float)mesh->mNumVertices);
	std::cerr << "center: " << glm::to_string(glMesh->center) << std::endl;
	glGenBuffers(1, &glMesh->positionsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, glMesh->positionsBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), mesh->mVertices, GL_STATIC_DRAW);
	glGenBuffers(1, &glMesh->normalsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, glMesh->normalsBuffer);
	std::cerr << "normals:" << (mesh->mNormals!=nullptr) << std::endl;
	glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), mesh->mNormals, GL_STATIC_DRAW);
	glGenBuffers(1, &glMesh->tangentsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, glMesh->tangentsBuffer);
	std::cerr << "tangents:" << (mesh->mTangents!=nullptr) << std::endl;
	glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), mesh->mTangents, GL_STATIC_DRAW);
	if (mesh->HasTextureCoords(0)) {
		glGenBuffers(1, &glMesh->uv0Buffer);
		glBindBuffer(GL_ARRAY_BUFFER, glMesh->uv0Buffer);
		std::cerr << "texCoords:" << (mesh->mTextureCoords!=nullptr) << std::endl;
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), mesh->mTextureCoords[0], GL_STATIC_DRAW);
	}
	std::vector<unsigned int> indices;
	indices.reserve(mesh->mNumFaces * 3);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < 3; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	glGenBuffers(1, &glMesh->elementsBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMesh->elementsBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0] , GL_STATIC_DRAW);

	return glMesh;
}

Material* createMaterial(aiMaterial* material) {
	// TODO: implement recovery of your scene's material into your material schema
	return new Material();
}

void loadDataWithAssimp(const std::string& path) {
	// Select the kinds of messages you want to receive on this log stream
	const unsigned int severity = Assimp::Logger::Debugging|Assimp::Logger::Info|Assimp::Logger::Err|Assimp::Logger::Warn;
	// Attaching it to the default logger
	myStream* stream = new myStream;
	Assimp::DefaultLogger::create();
	Assimp::DefaultLogger::get()->attachStream(stream, severity);
	//Look into data to find other meshes
	Assimp::Importer importer;
	std::cerr << std::filesystem::current_path() / path << std::endl;
	std::cerr << "importer" << std::endl;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_GenSmoothNormals       |
		aiProcess_CalcTangentSpace
		| aiProcess_Triangulate
		| aiProcess_JoinIdenticalVertices);
		std::cerr << "importer done" << std::endl;
		if (nullptr == scene) {
			std::cerr << "Failed to load scene: "  << path << std::endl;
			exit (EXIT_FAILURE);
		}
		std::cerr << "Assimp has loaded scene: " << path << std::endl;
		if (!scene->HasMeshes()) {
			std::cerr << "no meshes in scene"<< std::endl;
			exit(EXIT_FAILURE);
		}

		std::cerr << "Loading meshes" << std::endl;
		for (int i = 0; i < scene->mNumMeshes; ++i) {
			aiMesh* mesh = scene->mMeshes[i];
			std::cerr << "Mesh " << i << " : " << mesh->mName.C_Str() << std::endl;
			Mesh* glMesh = createMesh(mesh);
			Context::meshes.push_back(glMesh);
		}
		std::cerr << Context::meshes.size() << " meshes loaded" << std::endl;

		std::cerr << "Loading materials" << std::endl;
		// materials.push_back(new UnlitMaterial());
		// materials[0]->init();
		for (int i = 0; i < scene->mNumMaterials; ++i) {
			aiMaterial* material = scene->mMaterials[i];
			aiString name;
			material->Get(AI_MATKEY_NAME, name);
			std::cerr << "Material " << i << " : " << name.C_Str() << std::endl;
			Material* glMaterial = createMaterial(material);
			Context::materials.push_back(glMaterial);
			glMaterial->init();
		}
		std::cerr << Context::materials.size() << " materials loaded" << std::endl;

		std::queue<std::pair<glm::mat4, aiNode*> > queue;
		queue.push({glm::mat4(1.0), scene->mRootNode});

		std::cerr << "Loading of scene" << std::endl;
		while (!queue.empty()) {
			std::pair<glm::mat4, aiNode*> pair = queue.front();
			queue.pop();

			aiNode* current = pair.second;
			glm::mat4 parentMatrix = pair.first;
			glm::mat4 model = parentMatrix * glm::make_mat4((float*)&current->mTransformation);
			std::cerr << "model:" << glm::to_string(model) << std::endl;
			for (int i = 0; i < current->mNumMeshes; ++i) {
				int meshIndex = current->mMeshes[i];
				std::cerr << "mesh-" << meshIndex << ";material-" << scene->mMeshes[meshIndex]->mMaterialIndex << std::endl;
				Instance inst;
				inst.matrix = model;
				inst.mesh = Context::meshes[meshIndex];
				inst.material = Context::materials[scene->mMeshes[meshIndex]->mMaterialIndex];
				Context::instances.push_back(inst);
			}

			for (int i = 0; i < current->mNumChildren; ++i) {
				aiNode* child = current->mChildren[i];
				queue.push({model, child});
			}
		}
		std::cerr << Context::instances.size() << " instances loaded" << std::endl;
		delete stream;
		importer.FreeScene();
	}
