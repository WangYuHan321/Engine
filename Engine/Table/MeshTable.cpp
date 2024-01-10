#include "MeshTable.h"
#include <assimp\cimport.h>
#include <assimp\postprocess.h>
#include <glm/glm.hpp>

void Engine::MeshTable::AddMeshToCache(Engine::string name, const Engine::Mesh& mesh)
{
	std::map<std::string, Engine::Mesh* >::iterator it = meshCache.find(name);
	if (it == meshCache.end())
	{
		meshCache[name] = new Engine::Mesh(mesh);
	}
}

void Engine::MeshTable::Initialize()
{
	float vertices[12];
	vertices[0] = 0.0f; vertices[1] = 0.0f; vertices[2] = 0.0f;
	vertices[3] = 1.0f; vertices[4] = 0.0f; vertices[5] = 0.0f;
	vertices[6] = 0.0f; vertices[7] = 0.0f; vertices[8] = 1.0f;
	vertices[9] = 1.0f; vertices[10] = 0.0f; vertices[11] = 1.0f;

	unsigned int faces[6];
	faces[0] = 0; faces[1] = 2; faces[2] = 1;
	faces[3] = 1; faces[4] = 2; faces[5] = 3;

	float normals[12];
	normals[0] = 0.0f; normals[1] = 1.0f; normals[2] = 0.0f;
	normals[3] = 0.0f; normals[4] = 1.0f; normals[5] = 0.0f;
	normals[6] = 0.0f; normals[7] = 1.0f; normals[8] = 0.0f;
	normals[9] = 0.0f; normals[10] = 1.0f; normals[11] = 0.0f;

	float uv[8];
	uv[0] = 0.0f; uv[1] = 0.0f;
	uv[2] = 1.0f; uv[3] = 0.0f;
	uv[4] = 0.0f; uv[5] = 1.0f;
	uv[6] = 1.0f; uv[7] = 1.0f;

	Engine::Mesh plane(2, 4, faces, vertices, 0, normals, uv, 0);
	Engine::MeshTable::getInstance()->AddMeshToCache("terrain_tile", plane);

	const unsigned int planeNVertex = 4;

	// Post process - or any purpose - plane
	const float planeVertexPos[] = {
		-1.0f,	-1.0f,	 0.0f,
		1.0f,	-1.0f,	 0.0f,
		-1.0f,	 1.0f,	 0.0f,
		1.0f,	 1.0f,	 0.0f,
	};

	const float planeUVs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};

	Engine::Mesh planeTest(0, planeNVertex, 0, planeVertexPos, 0, 0, planeUVs, 0);
	Engine::MeshTable::getInstance()->AddMeshToCache("plane", planeTest);


	//const unsigned int X_SEGMENTS = 64;
	//const unsigned int Y_SEGMENTS = 64;

	//std::vector<float> positions_;
	//std::vector<float> uv_;
	//std::vector<float> normals_;
	//std::vector<unsigned int> indices_;

	//const float PI = 3.14159265359f;
	//for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
	//{
	//	for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
	//	{
	//		float xSegment = (float)x / (float)X_SEGMENTS;
	//		float ySegment = (float)y / (float)Y_SEGMENTS;
	//		float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
	//		float yPos = std::cos(ySegment * PI);
	//		float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

	//		positions_.push_back(xPos);
	//		positions_.push_back(yPos);
	//		positions_.push_back(zPos);

	//		uv_.push_back(xSegment);
	//		uv_.push_back(ySegment);
	//		
	//		normals_.push_back(xPos);
	//		normals_.push_back(yPos);
	//		normals_.push_back(zPos);
	//	}
	//}

	//bool oddRow = false;
	//for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
	//{
	//	if (!oddRow) // even rows: y == 0, y == 2; and so on
	//	{
	//		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
	//		{
	//			indices_.push_back(y * (X_SEGMENTS + 1) + x);
	//			indices_.push_back((y + 1) * (X_SEGMENTS + 1) + x);
	//		}
	//	}
	//	else
	//	{
	//		for (int x = X_SEGMENTS; x >= 0; --x)
	//		{
	//			indices_.push_back((y + 1) * (X_SEGMENTS + 1) + x);
	//			indices_.push_back(y * (X_SEGMENTS + 1) + x);
	//		}
	//	}
	//	oddRow = !oddRow;
	//}

	//Engine::Mesh sphereTest(0, 64 * 64 * 3, indices_.data(), positions_.data(), 0, 0, uv_.data(), 0);
	//Engine::MeshTable::getInstance()->AddMeshToCache("sphere", sphereTest);

	const int cubeNVertex = 24;
	const int cubeNTriangleIndex = 12;

	const unsigned int cubeTriangleIndex[] =
	{
		//Face z = 1
		0,1,2,			1,3,2,
		//Face z = -1
		4,6,5,			5,6,7,
		//Face x = 1
		8,10,9,			9,10,11,
		//Face x = -1
		12,13,14,		13,15,14,
		//Face y = 1
		16,17,18,		17,19,18,
		//Face y = -1
		20,22,21,		21,22,23,
	};

	const float cubeVertexPos[] =
	{
		//Face z = 1
		-1.0f,	-1.0f,	 1.0f, //0
		 1.0f,	-1.0f,	 1.0f, //1
		-1.0f,	 1.0f,	 1.0f, //2
		 1.0f,	 1.0f,	 1.0f, //3

		//Face z = -1		   
		-1.0f,	-1.0f,	-1.0f, //4
		 1.0f,	-1.0f,	-1.0f, //5
		-1.0f,	 1.0f,	-1.0f, //6
		 1.0f,	 1.0f,	-1.0f, //7

		//Face x = 1		   
		1.0f,	-1.0f,	-1.0f, //8
		1.0f,	-1.0f,	 1.0f, //9
		1.0f,	 1.0f,	-1.0f, //10
		1.0f,	 1.0f,	 1.0f, //11

		//Face x = -1		   
		-1.0f,	-1.0f,	-1.0f, //12
		-1.0f,	-1.0f,	 1.0f, //13
		-1.0f,	 1.0f,	-1.0f, //14
		-1.0f,	 1.0f,	 1.0f, //15

		//Face y = 1		   
		-1.0f,	 1.0f,	-1.0f, //16
		-1.0f,	 1.0f,	 1.0f, //17
		 1.0f,	 1.0f,	-1.0f, //18
		 1.0f,	 1.0f,	 1.0f, //19

		//Face y = -1		   
		-1.0f,	-1.0f,	-1.0f, //20
		-1.0f,	-1.0f,	 1.0f, //21
		 1.0f,	-1.0f,	-1.0f, //22
		 1.0f,	-1.0f,	 1.0f  //23
	};

	const float cubeVertexNormal[] =
	{
		//Face z = 1
		0.0f,	0.0f,	 1.0f,
		0.0f,	0.0f,	 1.0f,
		0.0f,	0.0f,	 1.0f,
		0.0f,	0.0f,	 1.0f,

		//Face z = -1		   
		0.0f,	0.0f,	-1.0f,
		0.0f,	0.0f,	-1.0f,
		0.0f,	0.0f,	-1.0f,
		0.0f,	0.0f,	-1.0f,

		//Face x = 1		   
		1.0f,	0.0f,	 0.0f,
		1.0f,	0.0f,	 0.0f,
		1.0f,	0.0f,	 0.0f,
		1.0f,	0.0f,	 0.0f,

		//Face x = -1		   
		-1.0f,	0.0f,	 0.0f,
		-1.0f,	0.0f,	 0.0f,
		-1.0f,	0.0f,	 0.0f,
		-1.0f,	0.0f,	 0.0f,

		//Face y = 1		   
		0.0f,	1.0f,	0.0f,
		0.0f,	1.0f,	0.0f,
		0.0f,	1.0f,	0.0f,
		0.0f,	1.0f,	0.0f,

		//Face y = -1		   
		0.0f,	-1.0f,	0.0f,
		0.0f,	-1.0f,	0.0f,
		0.0f,	-1.0f,	0.0f,
		0.0f,	-1.0f,	0.0f
	};

	const float cubeVertexColor[] =
	{
		//Face z = 1
		0.0f,	0.0f,	 1.0f,
		0.0f,	0.0f,	 1.0f,
		0.0f,	0.0f,	 1.0f,
		0.0f,	0.0f,	 1.0f,

		//Face z = -1		   
		0.6f,	0.6f,	 1.0f,
		0.6f,	0.6f,	 1.0f,
		0.6f,	0.6f,	 1.0f,
		0.6f,	0.6f,	 1.0f,

		//Face x = 1		   
		1.0f,	0.0f,	 0.0f,
		1.0f,	0.0f,	 0.0f,
		1.0f,	0.0f,	 0.0f,
		1.0f,	0.0f,	 0.0f,

		//Face x = -1		   
		1.0f,	0.6f,	 0.6f,
		1.0f,	0.6f,	 0.6f,
		1.0f,	0.6f,	 0.6f,
		1.0f,	0.6f,	 0.6f,

		//Face y = 1		   
		0.0f,	1.0f,	0.0f,
		0.0f,	1.0f,	0.0f,
		0.0f,	1.0f,	0.0f,
		0.0f,	1.0f,	0.0f,

		//Face y = -1		   
		0.6f,	1.0f,	0.6f,
		0.6f,	1.0f,	0.6f,
		0.6f,	1.0f,	0.6f,
		0.6f,	1.0f,	0.6f
	};

	const float cubeVertexTexCoord[] =
	{
		//Face z = 1
		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 0.0f, 1.0f,
		 1.0f, 1.0f,

		 //Face z = -1
		 0.0f, 1.0f,
		 1.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,

		 //Face x = 1	
		 0.0f,	1.0f,
		 1.0f,	1.0f,
		 0.0f,	0.0f,
		 1.0f,	0.0f,

		 //Face x = -1
		 0.0f,	0.0f,
		 1.0f,	0.0f,
		 0.0f,	1.0f,
		 1.0f,	1.0f,

		 //Face y = 1	
		 0.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 1.0f,
		 1.0f, 0.0f,

		 //Face y = -1
		 0.0f, 0.0f,
		 0.0f, 1.0f,
		 1.0f, 0.0f,
		 1.0f, 1.0f,
	};

	const float cubeVertexTangent[] =
	{
		//Face z = 1
		1.0f,	0.0f,	0.0f,
		1.0f,	0.0f,	0.0f,
		1.0f,	0.0f,	0.0f,
		1.0f,	0.0f,	0.0f,

		//Face z = -1				
		1.0f,	0.0f,	0.0f,
		1.0f,	0.0f,	0.0f,
		1.0f,	0.0f,	0.0f,
		1.0f,	0.0f,	0.0f,

		//Face x = 1				
		0.0f,	0.0f,	-1.0f,
		0.0f,	0.0f,	-1.0f,
		0.0f,	0.0f,	-1.0f,
		0.0f,	0.0f,	-1.0f,

		//Face x = -1				
		0.0f,	0.0f,	1.0f,
		0.0f,	0.0f,	1.0f,
		0.0f,	0.0f,	1.0f,
		0.0f,	0.0f,	1.0f,

		//Face y = 1	   
		1.0f,	0.0f,	0.0f,
		1.0f,	0.0f,	0.0f,
		1.0f,	0.0f,	0.0f,
		1.0f,	0.0f,	0.0f,

		//Face y = -1					   
		1.0f,	0.0f,	0.0f,
		1.0f,	0.0f,	0.0f,
		1.0f,	0.0f,	0.0f,
		1.0f,	0.0f,	0.0f,
	};

	Engine::Mesh cubeTest(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, cubeVertexPos, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);
	Engine::MeshTable::getInstance()->AddMeshToCache("cube", cubeTest);

}

Engine::Mesh* Engine::MeshTable::GetMesh(Engine::string name)
{
	std::map<std::string, Engine::Mesh*>::iterator it = meshCache.find(name);
	if (it != meshCache.end())
	{
		return meshCache[name];
	}
	else
	{
		unsigned int flags = aiPostProcessSteps::aiProcess_GenUVCoords | aiPostProcessSteps::aiProcess_JoinIdenticalVertices;
		const aiScene* scene = aiImportFile(name.c_str(), flags);

		if (!scene)
		{
			return NULL;
		}

		if (scene->HasMeshes())
		{
			aiMesh* rawMesh = scene->mMeshes[0];
			Mesh* m = new Mesh(rawMesh);

			meshCache[name] = m;
			aiReleaseImport(scene);
			return meshCache[name];
		}

		aiReleaseImport(scene);
	}

	return NULL;
}

void Engine::MeshTable::Clean()
{
	std::map<std::string, Engine::Mesh* >::iterator it = meshCache.begin();
	while (it != meshCache.end())
	{
		it->second->releaseGPU();
		delete it->second;
		it++;
	}
}

Engine::MeshTable::~MeshTable()
{
	Clean();
}