#pragma once

#include "StorageTable.h"
#include "../Render/Mesh/Mesh.h"

namespace Engine
{
	class MeshTable : public CSingleton<MeshTable>,
		public StorageTable
	{
	private:
		std::map<string, Mesh*> meshCache;

	public:

		void AddMeshToCache(string name, const Mesh& mesh);
		Mesh* GetMesh(string name);

		void Initialize();

		void Clean();

		~MeshTable();
	};

}

