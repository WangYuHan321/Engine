#version 430 core

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 outPos;

in vec2 outTexCoord;

//�仯���ߵĲ���
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

uniform sampler2D weather; // �Ƹ����� ��ˮ���� ������ 0.0 ���� 0.5 ����� 1.0 ����

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

// ׶��������ƫ��
uniform vec3 noiseKernel[6u] = vec3[] 
(
	vec3( 0.38051305,  0.92453449, -0.02111345),
	vec3(-0.50625799, -0.03590792, -0.86163418),
	vec3(-0.32509218, -0.94557439,  0.01428793),
	vec3( 0.09026238, -0.27376545,  0.95755165),
	vec3( 0.28128598,  0.42443639, -0.86065785),
	vec3(-0.16852403,  0.14748697,  0.97460106)
);

//Ϊ��ʼ�������������ƫ�ƣ��Է�ֹ���ִ�״αӰ 
#define BAYER_FACTOR 1.0 / 16.0

uniform float bayerFilter[16u] = float[]
(
	0.0*BAYER_FACTOR, 8.0*BAYER_FACTOR, 2.0*BAYER_FACTOR, 10.0*BAYER_FACTOR,
	12.0*BAYER_FACTOR, 4.0*BAYER_FACTOR, 14.0*BAYER_FACTOR, 6.0*BAYER_FACTOR,
	3.0*BAYER_FACTOR, 11.0*BAYER_FACTOR, 1.0*BAYER_FACTOR, 9.0*BAYER_FACTOR,
	15.0*BAYER_FACTOR, 7.0*BAYER_FACTOR, 13.0*BAYER_FACTOR, 5.0*BAYER_FACTOR
);

// �����͸߶��ܶ��ݶ� 
#define STRATUS_GRADIENT vec4(0.0, 0.1, 0.2, 0.3)
#define STRATOCUMULUS_GRADIENT vec4(0.02, 0.2, 0.48, 0.625)
#define CUMULUS_GRADIENT vec4(0.00, 0.1625, 0.88, 0.98)

// ���������
#define CONE_STEP 0.1666666

//=============================================
// ���պ���
// ɢ����λ����
float henyeyGreenstein(vec3 l, vec3 v, float g, float ca)
{
		float g2 = g * g;

	return ((1.0 - g2) / pow((1.0 + g2 - 2.0 * g * ca), 1.5 )) * (1.0 / (4.0 * 3.1415));
}

//===============================================
//ǿ�����ܶȽ���
float beer(float density)
{
	return exp(-density);
}

// �����Ƶ��ۺ۴��и������ɢ�� Beer's law inverted equation
float powder(float density, float ca)
{
	float f = 1.0 - exp(-density * 2.0);
	return mix(1.0, f, clamp(-ca * 0.5 + 0.5, 0.0, 1.0));
}

// �����Ĺ��Ӽ��㹫ʽ
float lightEnergy(vec3 l, vec3 v, float ca, float coneDensity)
{
	return 50.0f * beer(coneDensity) * powder(coneDensity, ca) * henyeyGreenstein(l, v, ca, coneDensity);
}

//��ȡ�Ӹ߶Ȼ�����

vec3 ambientLight()
{
	vec3 lightColor = realLightColor;
	vec3 ambientColor = mix(horizonColor, zenitColor, 0.15);
	return mix(lightColor, ambientColor, 0.65) * lightFactor * 0.65;
}

//=========================================
//�ܶȺ���

//�Ʋ��ڵĹ�һ���߶� (0 - 1)
float getHeightFraction(vec3 p)
{
	return (length(p - sphereCenter) - innerSphereRadius) / (outerSphereRadius);
}

//��3Ϊ����ת��Ϊ2ά
vec2 sphericalUVProj(vec3 p)
{
	vec3 dirValue = normalize(p - sphereCenter);
	return (dirValue.xz + 1.0) / 2.0;
}

//��ȡ������Ϣ
vec3 getWeatherData(vec3 p)
{
	vec2 uv = sphericalUVProj(p) * weatherScale;
	return texture(weather, uv).rgb;
}

//����ӳ�亯��
float remap(float original, float oMin, float oMax, float nMin, float nMax)
{
	return nMin + ((original - oMin) / (oMax - oMin)) * (nMax - nMin);
}

//��ȡ�Ƶ��ܶȺ��� ���������ͺ�Ĭ����ģ��֮���Ȩ��

float getDensityForCloud(float heightFraction, float cloudType)
{
	float stratusFactor = 1.0- clamp(cloudType * 2.0, 0.0 , 1.0);// stratus ��������
	float stratoCumulusFactor= 1.0 - abs(cloudType - 0.5) * 2.0; // ���������
	float cumulusFactor = 1.0 - abs(cloudType - 0.5) * 2.0; // ��������

	vec4 baseGradient = stratusFactor * STRATUS_GRADIENT + stratoCumulusFactor * STRATOCUMULUS_GRADIENT + cumulusFactor * CUMULUS_GRADIENT;

	//�ݶȼ��㣨�μ� Siggraph 2017 Nubis-Decima �ݽ���
	float result = remap(heightFraction, baseGradient.x, baseGradient.y, 0.0, 1.0) * remap(heightFraction, baseGradient.z, baseGradient.w, 1.0, 0.0);
	return result;
}

// ��������������ܶ�
float sampleCloudDensity(vec3 p, float lod, bool expensive, float heightFraction)
{
	//����������
	p += heightFraction * windDirection * cloudTopOffset;
	p += windDirection * time * cloudSpeed;

	float deltaDist = clamp(length(p - camPos) / maxRenderDist, 0.0, 1.0);

	vec3 weatherData = getWeatherData(p);

	vec2 uv = sphericalUVProj(p);

	// ��������״�������� (Perlin-Worley + 3 Worley)
	vec4 baseCloudNoise = textureLod(perlinworley, vec3(uv * baseNoiseScale, heightFraction), lod);

	// ������Ƶ fbm �޸���
	float lowFreqFBM = ( baseCloudNoise.g * 0.625) + ( baseCloudNoise.b * 0.25 ) + ( baseCloudNoise.a * 0.125 );
	float baseCloudShape = remap(baseCloudNoise.r, -(1.0 - lowFreqFBM), 1.0, 0.0, 1.0);

	// ����������Ӧ���ܶ��ݶ�
	float densityGradient = getDensityForCloud(heightFraction, weatherData.g);
	baseCloudShape *= densityGradient;

	// Ӧ�ø�����
	float coverage = clamp(weatherData.r, 0.0, 1.0) * coverageMultiplier;
	// ȷ���ܶ�С��ʵ�ʸ��Ƿ�Χ������ʧ
	float coveragedCloud = remap(baseCloudShape, coverage, 1.0, 0.0, 1.0);
	coveragedCloud *= coverage;
	coveragedCloud *= mix(1.0, 0.0, clamp(heightFraction / coverage, 0.0, 1.0));

	float finalCloud = coveragedCloud;

	// ������ʴ�ɼ�ʱ����ʴ�ƣ���ĿǰΪֹ�������ܶȽϵͣ�
	if(expensive)
	{
		//������Ƶ�������� FBM��
		vec3 erodeCloudNoise = textureLod(worley, vec3(uv * highFreqNoiseUVScale, heightFraction * highFreqNoiseHScale) * highFreqNoiseScale, lod).rgb;
		float highFreqFBM = (erodeCloudNoise.r * 0.625) + (erodeCloudNoise.g * 0.25) + (erodeCloudNoise.b * 0.125);

		// Ӧ�÷�Ͷ���ƫ�ƺ����¼���߶ȷ���
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
	//�޸Ĳ������� 6 �������л�ȡ�����ܶ����Ϣ
	vec3 rayStep = lightDir * (stepSize) * 0.7;
	// ��ʼ׶�뾶�������ƶ��뾶����
	float coneRadius = 1.0;

	//�������ĵ�λ�ܶ�
	float invDepth = 1.0 / (stepSize * 6);
	float density = 0.0;

	//�ܶ� - ����Ϊֹ��͸����
	float coneDensity = 0.0;

	int i = 0;
	while(i<6)
	{
		//��ȡ׶��λ��
		vec3 posInCone = startPos + lightDir + coneRadius * noiseKernel[i] * float(i);
		float deltaDist = clamp(length(posInCone - camPos) / maxRenderDist, 0.0, 1.0);

		//���Ź���ǰ�������ǿ��ܻ��߳�������
		float heightFraction = getHeightFraction(posInCone);
		if(heightFraction  <= 1.0)
		{
			// ��������ڱ߽總�����ܶȽϵͣ��� 0.3 �����£�������ð���ķ�ʽ����
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

	//1 ��������Ӱ��Զ��������
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

	// ���㵽���Ĺ�����
	return lightEnergy(lightDir, d, ca, coneDensity);
}

float frontToBackRaymarch(vec3 startPos, vec3 endPos, out vec3 color)
{
	//������·�����ȼ���
	vec3 path = endPos - startPos;

	int sampleCount = int(ceil(mix(48.0, 96.0, clamp((length(path) / (outerSphereRadius - innerSphereRadius)), 0.0, 1.0))));

	//ray march ����
	vec3 stepVector = path / float(sampleCount - 1);
	float stepSize = length(stepVector);
	vec3 viewDir = normalize(path);

	vec3 pos = startPos;
	float density = 0.0;
	vec4 result = vec4(0.0);

	float lodAlpha = clamp(length(startPos - camPos) / maxRenderDist, 0.0, 1.0);
	float samplingLod = mix(0.0, 6.0, lodAlpha);

	//����ʼ����λ���Ͻ��ж����Լ�������αӰ
	int a = int(gl_FragCoord.x) % 4;
	int b = int(gl_FragCoord.x) % 4;
	pos += stepVector * bayerFilter[a * 4 + b];

	//������˥��ϵ��
	float ambientFactor = max(min(lightFactor, 1.0), 0.1);
	
	//��������
	vec3 lc = realLightColor * lightFactor * cloudColor;
	vec3 ambientL = ambientLight();

	int i=0;

	while(i < sampleCount)
	{
		float cloudDensity = sampleCloudDensity(pos, samplingLod, true, getHeightFraction(pos));

		if(cloudDensity > 0.0) // ����������ܶȣ������������
		{
			density += cloudDensity;
			float lightEnergy = raymarchToLight(pos, viewDir, stepSize);

			float height =getHeightFraction(pos);
			vec4 src = vec4(lc * lightEnergy + ambientL, cloudDensity);
			src.rgb *= src.a;
			result = (1.0 - result.a) * src + result;

			if(result.a > 0.95)//��ǰ�˳���ȫ��͸��
				break;
		}

		pos +=stepVector;
		i++;
	}

	// ���� alpha���ܶȣ����洢������ɫ
	color = result.xyz;
	return result.a;
}

// collision detection

bool intersectSphere(vec3 o, vec3 d, out vec3 minT, out vec3 maxT)
{
	// ���������ཻ
	vec3 sphereToOrigin = (o - sphereCenter);
	float b = dot(d, sphereToOrigin);
	float c = dot(sphereToOrigin, sphereToOrigin);
	float sqrtOpInner = b*b - (c - innerSphereRadius * innerSphereRadius);

	//�޽⣨����������֮�⣬Զ������
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
	
	//  ���������ཻ
	float sqrtOpOuter = b*b - (c - outerSphereRadius * outerSphereRadius);

	//  �޽� - ��������ͬ
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

	//  ���������˳����ߵ�
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

	// �������ƬԪ��Ƭ�������ִ��ʱ����ͶӰ
	// ����������Ⱦһ������ܣ����ǹ��߷��򣨻�����Ļ���꣩
	// Ӧ�ÿ������ս����ȫ������Ļ�ߴ磩
	int frameIter = frame % 2;

	float coordX = gl_FragCoord.x;
	float coordY = gl_FragCoord.y;

#define INTERLEAVED
#ifdef INTERLEAVED
	vec2 fragCoord = vec2(coordX * 2.0 + frameIter, coordY);
#else
	vec2 fragCoord = vec2(coordX + (screenResolution.x / 2.0) * frameIter, coordY);
#endif

	//���Ƭ��δ���ڵ�������Ʋ��������׷��
	if(texture(currentPixelDepth, vec2(fragCoord / screenResolution)).x < 1.0)
	{
		color = vec4(0);
		outPos = vec4(0,0,0,1);
	}
	else
	{
		//������Ļ�����������ӽǼ�����߷���
		vec2 fulluv = fragCoord - screenResolution / 2.0;
		float z =  screenResolution.y / tan(radians(FOV));
		vec3 viewDir = normalize(vec3(fulluv, -z / 2.0));
		vec3 worldDir = normalize( (invView * vec4(viewDir, 0)).xyz);

		//�������ĵ�
		vec3 startPos, endPos;

		bool interesect = intersectSphere(camPos, worldDir, startPos, endPos);
		vec4 ambientColor = vec4(mix(horizonColor, zenitColor, 0.15), 0.6);
		if(interesect)
		{
					vec3 outColor = vec3(0);
			// ����ཻ raymarch cloud
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