#pragma once

#include <assimp/scene.h>
#include "../../Util/common.h"


namespace Engine
{
	class Mesh
	{
	private:
		uint32 numFaces;
		uint32 numVertices;
		uint32 verticesPerFace;

		uint32* faces;
		float* vertices;
		float* normals;
		float* colors;
		float* emission;
		float* uvs;
		float* tangents;

	public:
		uint32 vao;
		uint32 vboFaces;
		uint32 vboVertices;
		uint32 vboNormals;
		uint32 vboColors;
		uint32 vboEmission;
		uint32 vboUVs;
		uint32 vboTangents;

	public:
		Mesh();
		Mesh(aiMesh* mesh);
		Mesh(const unsigned int numF, const unsigned int numV, const unsigned int* f, const float* v, const float* c, const float* n, const float* uv, const float* t, const float* e = 0);
		Mesh(const Mesh& other);
		~Mesh();

		void loadFromMesh(aiMesh* mesh);

		const unsigned int getNumFaces() const;
		const unsigned int getNumVertices() const;
		const unsigned int getNumVerticesPerFace() const;

		const unsigned int* getFaces() const;
		const float* getVertices() const;
		const float* getNormals() const;
		const float* getColor() const;
		const float* getUVs() const;
		const float* getTangetns() const;
		const float* getEmissive() const;

		void computeNormals();
		void computeTangents();

		void syncGPU();

		void releaseGPU();
		void releaseCPU();

		void use() const;
	private:
		void extractTopology(aiMesh* mesh);
		void extractGeometry(aiMesh* mesh);

	};
}



