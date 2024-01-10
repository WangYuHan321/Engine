#pragma once

#include "../../Util/common.h"
#include "../Mesh/Mesh.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace Engine
{
	typedef struct TreeGenerationData
	{
		// ������������������ĸ������֡�
		// ������������
		ulong seed;

		//������������
		uint32 maxDepth;
		// ��ʼ������֧����ȡ��ڴ�֮ǰ��ֻ����һ����֧
		// ������֧���Ա�����������
		uint32 startBranchingDepth;
		// ÿ����ȼ��������֧��
		uint32 maxBranchesSplit;
		// �Ƿ��ڶ������ת�����ڽ��������ת
		// �Ե�һ�����飨���ɣ����������ת
		bool rotateMainTrunk;
		
		// ��С�������ת�������ת���������ѡ��
		glm::vec3 minBranchRotation;
		glm::vec3 maxBranchRotation;

		// Scaling factor applied on every depth level
		glm::vec3 scalingFactor;

		// Trunk color
		glm::vec3 startTrunkColor;
		glm::vec3 endTrunkColor;

		// Leaf color
		bool emissiveLeaf;
		glm::vec3 leafStartColor;
		glm::vec3 leafEndColor;

		// ����Ӧ�ô��ĸ���ȿ�ʼ���Ҷ�ӣ�����С�������ȣ�
		uint32 depthStartingLeaf;
	}TreeGenerationData;


	class ProceduralVegetation
	{
		TreeGenerationData treeData;

	public:

		ProceduralVegetation(const TreeGenerationData& data);

		virtual Mesh* Generate() = 0;

	};
}


