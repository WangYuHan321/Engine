#version 430 core

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 outPos;

in vec2 outTexCoord;

//变化视线的参数
uniform mat4 projView;
uniform mat4 invView;
uniform vec3 sphereCenter;
uniform float innerSphereRadius;
uniform float outerSphereRadius;
uniform float maxRenderDist;
uniform float cloudTopOffset;
uniform float weatherScale;
uniform float baseNoiseScale;
uniform float highFreqNoiseScale;
uniform float highFreqNoiseUVScale;
uniform float highFreqNoiseHScale;

uniform vec3 camPos;

uniform sampler2D currentPixelDepth;


uniform vec2 screenResolution;
uniform float FOV;

uniform sampler3D perlinworley;
uniform sampler3D worley;

uniform sampler2D weather; // 云覆盖率 降水概率 云类型 0.0 层云 0.5 层积云 1.0 积云

uniform vec3 lightDir;

uniform vec3 realLightColor;
uniform float lightFactor;

uniform vec3 zenitColor;
uniform vec3 horizonColor;

uniform vec3 cloudColor;

uniform int frame;

uniform float time;
uniform float cloudType;
uniform float cloudSpeed;
uniform vec3 windDirection;
uniform float coverageMultiplier;

// 锥体采样随机偏移
uniform vec3 noiseKernel[6u] = vec3[] 
(
	vec3( 0.38051305,  0.92453449, -0.02111345),
	vec3(-0.50625799, -0.03590792, -0.86163418),
	vec3(-0.32509218, -0.94557439,  0.01428793),
	vec3( 0.09026238, -0.27376545,  0.95755165),
	vec3( 0.28128598,  0.42443639, -0.86065785),
	vec3(-0.16852403,  0.14748697,  0.97460106)
);

//为起始射线深度添加随机偏移，以防止出现带状伪影 
#define BAYER_FACTOR 1.0 / 16.0

uniform float bayerFilter[16u] = float[]
(
	0.0*BAYER_FACTOR, 8.0*BAYER_FACTOR, 2.0*BAYER_FACTOR, 10.0*BAYER_FACTOR,
	12.0*BAYER_FACTOR, 4.0*BAYER_FACTOR, 14.0*BAYER_FACTOR, 6.0*BAYER_FACTOR,
	3.0*BAYER_FACTOR, 11.0*BAYER_FACTOR, 1.0*BAYER_FACTOR, 9.0*BAYER_FACTOR,
	15.0*BAYER_FACTOR, 7.0*BAYER_FACTOR, 13.0*BAYER_FACTOR, 5.0*BAYER_FACTOR
);

// 云类型高度密度梯度 
#define STRATUS_GRADIENT vec4(0.0, 0.1, 0.2, 0.3)
#define STRATOCUMULUS_GRADIENT vec4(0.02, 0.2, 0.48, 0.625)
#define CUMULUS_GRADIENT vec4(0.00, 0.1625, 0.88, 0.98)

// 光采样常数
#define CONE_STEP 0.1666666

//=============================================
// 光照函数
// 散射相位函数
float henyeyGreenstein(vec3 l, vec3 v, float g, float ca)
{
		float g2 = g * g;

	return ((1.0 - g2) / pow((1.0 + g2 - 2.0 * g * ca), 1.5 )) * (1.0 / (4.0 * 3.1415));
}

//===============================================
//强度随密度降低
float beer(float density)
{
	return exp(-density);
}

// 对于云的折痕处有更多的内散射 Beer's law inverted equation
float powder(float density, float ca)
{
	float f = 1.0 - exp(-density * 2.0);
	return mix(1.0, f, clamp(-ca * 0.5 + 0.5, 0.0, 1.0));
}

// 完整的光子计算公式
float lightEnergy(vec3 l, vec3 v, float ca, float coneDensity)
{
	return 50.0f * beer(coneDensity) * powder(coneDensity, ca) * henyeyGreenstein(l, v, ca, coneDensity);
}

//获取从高度环境光

vec3 ambientLight()
{
	vec3 lightColor = realLightColor;
	vec3 ambientColor = mix(horizonColor, zenitColor, 0.15);
	return mix(lightColor, ambientColor, 0.65) * lightFactor * 0.65;
}

//=========================================
//密度函数

//云层内的归一化高度 (0 - 1)
float getHeightFraction(vec3 p)
{
	return (length(p - sphereCenter) - innerSphereRadius) / (outerSphereRadius);
}

//将3为坐标转换为2维
vec2 sphericalUVProj(vec3 p)
{
	vec3 dirValue = normalize(p - sphereCenter);
	return (dirValue.xz + 1.0) / 2.0;
}

//获取雨天信息
vec3 getWeatherData(vec3 p)
{
	vec2 uv = sphericalUVProj(p) * weatherScale;
	return texture(weather, uv).rgb;
}

//重新映射函数
float remap(float original, float oMin, float oMax, float nMin, float nMax)
{
	return nMin + ((original - oMin) / (oMax - oMin)) * (nMax - nMin);
}

//获取云的密度函数 根据云类型和默认云模型之间的权重

float getDensityForCloud(float heightFraction, float cloudType)
{
	float stratusFactor = 1.0- clamp(cloudType * 2.0, 0.0 , 1.0);// stratus 层云因子
	float stratoCumulusFactor= 1.0 - abs(cloudType - 0.5) * 2.0; // 层积云因子
	float cumulusFactor = 1.0 - abs(cloudType - 0.5) * 2.0; // 积云因子

	vec4 baseGradient = stratusFactor * STRATUS_GRADIENT + stratoCumulusFactor * STRATOCUMULUS_GRADIENT + cumulusFactor * CUMULUS_GRADIENT;

	//梯度计算（参见 Siggraph 2017 Nubis-Decima 演讲）
	float result = remap(heightFraction, baseGradient.x, baseGradient.y, 0.0, 1.0) * remap(heightFraction, baseGradient.z, baseGradient.w, 1.0, 0.0);
	return result;
}

// 检索给定点的云密度
float sampleCloudDensity(vec3 p, float lod, bool expensive, float heightFraction)
{
	//让云随风而变
	p += heightFraction * windDirection * cloudTopOffset;
	p += windDirection * time * cloudSpeed;

	float deltaDist = clamp(length(p - camPos) / maxRenderDist, 0.0, 1.0);

	vec3 weatherData = getWeatherData(p);

	vec2 uv = sphericalUVProj(p);

	// 基础云形状噪声样本 (Perlin-Worley + 3 Worley)
	vec4 baseCloudNoise = textureLod(perlinworley, vec3(uv * baseNoiseScale, heightFraction), lod);

	// 构建低频 fbm 修改器
	float lowFreqFBM = ( baseCloudNoise.g * 0.625) + ( baseCloudNoise.b * 0.25 ) + ( baseCloudNoise.a * 0.125 );
	float baseCloudShape = remap(baseCloudNoise.r, -(1.0 - lowFreqFBM), 1.0, 0.0, 1.0);

	// 根据云类型应用密度梯度
	float densityGradient = getDensityForCloud(heightFraction, weatherData.g);
	baseCloudShape *= densityGradient;

	// 应用覆盖率
	float coverage = clamp(weatherData.r, 0.0, 1.0) * coverageMultiplier;
	// 确保密度小于实际覆盖范围的云消失
	float coveragedCloud = remap(baseCloudShape, coverage, 1.0, 0.0, 1.0);
	coveragedCloud *= coverage;
	coveragedCloud *= mix(1.0, 0.0, clamp(heightFraction / coverage, 0.0, 1.0));

	float finalCloud = coveragedCloud;

	// 仅在侵蚀可见时才侵蚀云（到目前为止采样的密度较低）
	if(expensive)
	{
		//构建高频沃利噪声 FBM。
		vec3 erodeCloudNoise = textureLod(worley, vec3(uv * highFreqNoiseUVScale, heightFraction * highFreqNoiseHScale) * highFreqNoiseScale, lod).rgb;
		float highFreqFBM = (erodeCloudNoise.r * 0.625) + (erodeCloudNoise.g * 0.25) + (erodeCloudNoise.b * 0.125);

		// 应用风和顶部偏移后重新计算高度分数
		heightFraction = getHeightFraction(p);

		float highFreqNoiseModifier = mix(highFreqFBM, 1.0 - highFreqFBM, clamp(heightFraction * 8.5, 0.0, 1.0));

		finalCloud = remap(coveragedCloud, highFreqNoiseModifier * 0.8, 1.0, 0.0, 1.0);
	}

	return clamp(finalCloud, 0.0, 1.0);
}

// =====================================================================
//Raymarchers

float raymarchToLight(vec3 pos , vec3 d, float stepSize)
{
	vec3 startPos = pos;
	//修改步长以在 6 个步骤中获取尽可能多的信息
	vec3 rayStep = lightDir * (stepSize) * 0.7;
	// 起始锥半径。随着移动半径增大
	float coneRadius = 1.0;

	//被采样的单位密度
	float invDepth = 1.0 / (stepSize * 6);
	float density = 0.0;

	//密度 - 迄今为止的透射率
	float coneDensity = 0.0;

	int i = 0;
	while(i<6)
	{
		//获取锥体位置
		vec3 posInCone = startPos + lightDir + coneRadius * noiseKernel[i] * float(i);
		float deltaDist = clamp(length(posInCone - camPos) / maxRenderDist, 0.0, 1.0);

		//向着光线前进，我们可能会走出大气层
		float heightFraction = getHeightFraction(posInCone);
		if(heightFraction  <= 1.0)
		{
			// 如果我们在边界附近（密度较低，如 0.3 或以下），则采用昂贵的方式采样
			float cloudDensity = sampleCloudDensity(posInCone, float(i + 1), coneDensity < 0.3, heightFraction);
			if(cloudDensity > 0.0)
			{
				density += cloudDensity;
				float transmittance = 1.0 - (density * invDepth);
				coneDensity += (cloudDensity * transmittance);
			}
		}

		startPos+= rayStep;
		coneRadius += CONE_STEP;
		i++;
	}

	//1 个用于阴影的远距离样本
	pos += (rayStep * 8.0);
	float heightFraction = getHeightFraction(pos);
	float cloudDensity = sampleCloudDensity(pos, 6.0, false, heightFraction);
	if(cloudDensity > 0.0)
	{
		density += cloudDensity;
		float transmittance = 1.0 - (density * invDepth);
		coneDensity += (cloudDensity * transmittance);
	}

	float ca = dot(lightDir, d);

	// 计算到达点的光能量
	return lightEnergy(lightDir, d, ca, coneDensity);
}

float frontToBackRaymarch(vec3 startPos, vec3 endPos, out vec3 color)
{
	//采样的路径长度计算
	vec3 path = endPos - startPos;

	int sampleCount = int(ceil(mix(48.0, 96.0, clamp((length(path) / (outerSphereRadius - innerSphereRadius)), 0.0, 1.0))));

	//ray march 数据
	vec3 stepVector = path / float(sampleCount - 1);
	float stepSize = length(stepVector);
	vec3 viewDir = normalize(path);

	vec3 pos = startPos;
	float density = 0.0;
	vec4 result = vec4(0.0);

	float lodAlpha = clamp(length(startPos - camPos) / maxRenderDist, 0.0, 1.0);
	float samplingLod = mix(0.0, 6.0, lodAlpha);

	//在起始光线位置上进行抖动以减少条带伪影
	int a = int(gl_FragCoord.x) % 4;
	int b = int(gl_FragCoord.x) % 4;
	pos += stepVector * bayerFilter[a * 4 + b];

	//环境光衰减系数
	float ambientFactor = max(min(lightFactor, 1.0), 0.1);
	
	//完整光照
	vec3 lc = realLightColor * lightFactor * cloudColor;
	vec3 ambientL = ambientLight();

	int i=0;

	while(i < sampleCount)
	{
		float cloudDensity = sampleCloudDensity(pos, samplingLod, true, getHeightFraction(pos));

		if(cloudDensity > 0.0) // 如果我们有密度，则启动光采样
		{
			density += cloudDensity;
			float lightEnergy = raymarchToLight(pos, viewDir, stepSize);

			float height =getHeightFraction(pos);
			vec4 src = vec4(lc * lightEnergy + ambientL, cloudDensity);
			src.rgb *= src.a;
			result = (1.0 - result.a) * src + result;

			if(result.a > 0.95)//提前退出完全不透明
				break;
		}

		pos +=stepVector;
		i++;
	}

	// 返回 alpha（密度）并存储最终颜色
	color = result.xyz;
	return result.a;
}

// collision detection

bool intersectSphere(vec3 o, vec3 d, out vec3 minT, out vec3 maxT)
{
	// 与内球体相交
	vec3 sphereToOrigin = (o - sphereCenter);
	float b = dot(d, sphereToOrigin);
	float c = dot(sphereToOrigin, sphereToOrigin);
	float sqrtOpInner = b*b - (c - innerSphereRadius * innerSphereRadius);

	//无解（我们在球体之外，远离它）
	float maxSInner;
	if(sqrtOpInner < 0.0)
	{
		return false;
	}
	
	float deInner = sqrt(sqrtOpInner);
	float solAInner = -b - deInner;
	float solBInner = -b + deInner;

	maxSInner = max(solAInner, solBInner);

	if(maxSInner < 0.0) return false;

	maxSInner = maxSInner < 0.0? 0.0 : maxSInner;
	
	//  与外球体相交
	float sqrtOpOuter = b*b - (c - outerSphereRadius * outerSphereRadius);

	//  无解 - 与内球相同
	if(sqrtOpOuter < 0.0)
	{
		return false;
	}
	
	float deOuter = sqrt(sqrtOpOuter);
	float solAOuter = -b - deOuter;
	float solBOuter = -b + deOuter;

	float maxSOuter = max(solAOuter, solBOuter);

	if(maxSOuter < 0.0) return false;

	maxSOuter = maxSOuter < 0.0? 0.0 : maxSOuter;

	//  计算进入和退出射线点
	float minSol = min(maxSInner, maxSOuter);

	if(minSol > maxRenderDist)
	{
		return false;
	}

	float maxSol = max(maxSInner, maxSOuter);

	minT = o + d * minSol;
	maxT = o + d * maxSol;

	return true;
}

void main()
{

	// 必须调整片元碎片坐标才能执行时间重投影
	// 我们正在渲染一个半宽框架，但是光线方向（基于屏幕坐标）
	// 应该考虑最终结果（全高清屏幕尺寸）
	int frameIter = frame % 2;

	float coordX = gl_FragCoord.x;
	float coordY = gl_FragCoord.y;

#define INTERLEAVED
#ifdef INTERLEAVED
	vec2 fragCoord = vec2(coordX * 2.0 + frameIter, coordY);
#else
	vec2 fragCoord = vec2(coordX + (screenResolution.x / 2.0) * frameIter, coordY);
#endif

	//如果片段未被遮挡，则对云层进行射线追踪
	if(texture(currentPixelDepth, vec2(fragCoord / screenResolution)).x < 1.0)
	{
		color = vec4(0);
		outPos = vec4(0,0,0,1);
	}
	else
	{
		//根据屏幕坐标和摄像机视角计算光线方向
		vec2 fulluv = fragCoord - screenResolution / 2.0;
		float z =  screenResolution.y / tan(radians(FOV));
		vec3 viewDir = normalize(vec3(fulluv, -z / 2.0));
		vec3 worldDir = normalize( (invView * vec4(viewDir, 0)).xyz);

		//体积物体的点
		vec3 startPos, endPos;

		bool interesect = intersectSphere(camPos, worldDir, startPos, endPos);
		vec4 ambientColor = vec4(mix(horizonColor, zenitColor, 0.15), 0.6);
		if(interesect)
		{
					vec3 outColor = vec3(0);
			// 如果相交 raymarch cloud
			float density = frontToBackRaymarch(startPos, endPos, outColor);
			density = clamp(density, 0.0, 1.0);

			vec4 finalColor = vec4(outColor, density);
			float alpha = length(startPos - camPos) / maxRenderDist;
			alpha *= alpha;
			alpha = clamp(alpha, 0, 1);
			finalColor = mix(finalColor, ambientColor * lightFactor, alpha);
			color = finalColor;
			outPos = vec4(startPos, 1.0);//vec4(camPos + normalize(startPos - camPos) * 100.0, 1.0);
		}
		else
		{
			color = ambientColor * lightFactor;
			outPos = vec4(0,0,0,1);
		}
	}
}