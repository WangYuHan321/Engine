#pragma once

#include "../../Util/common.h"
#include "../Mesh/Mesh.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace Engine
{
	typedef struct TreeGenerationData
	{
		// 种子用于生成随机数的各个部分。
		// 数字生成器。
		ulong seed;

		//最大生产的深度
		uint32 maxDepth;
		// 开始创建分支的深度。在此之前，只创建一个分支
		// 创建分支，以保持树的生长
		uint32 startBranchingDepth;
		// 每个深度级别的最大分支数
		uint32 maxBranchesSplit;
		// 是否在定义的旋转区间内进行随机旋转
		// 对第一个树块（主干）进行随机旋转
		bool rotateMainTrunk;
		
		// 最小和最大旋转间隔（旋转将从中随机选择）
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

		// 我们应该从哪个深度开始添加叶子（必须小于最大深度）
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


