#version 410 core

#ifndef SHADOW_MAP
layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outNormal;
layout (location=2) out vec4 outSpecular;
layout (location=3) out vec4 outEmissive;
layout (location=4) out vec4 outPos1;
layout (location=5) out vec4 outInfo;

layout (location=0) in vec2 outUV;
layout (location=1) in vec3 outPos;
layout (location=2) in vec4 inShadowMapPos;
layout (location=3) in vec4 inShadowMapPos1;

uniform mat4 normal;

uniform sampler2D depthTexture;
uniform sampler2D depthTexture1;

uniform sampler2D inInfo;
uniform vec2 screenSize;

uniform vec3 lightDir;
vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

uniform ivec2 gridPos;

uniform float time;
uniform vec3 watercolor;
uniform float waterspeed;

// ================================================================================

float Random2D(in vec2 st)
{
	//fract() 计算参数的小数部分。
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

// ================================================================================

uniform float scale = 200.0;
uniform float amplitude = 0.5;
uniform float frecuency = 1.0;
uniform int octaves = 4;

float NoiseInterpolation(in vec2 i_coord, in float i_size)
{
	vec2 grid = i_coord *i_size;

	vec2 randomInput = floor(grid);
	vec2 weights = fract(grid);


	float p0 = Random2D(randomInput);
	float p1 = Random2D(randomInput + vec2(1.0, 0.0));
	float p2 = Random2D(randomInput + vec2(0.0, 1.0));
	float p3 = Random2D(randomInput + vec2(1.0, 1.0));

	weights = smoothstep(vec2(0.0, 0.0), vec2(1.0, 1.0), weights);

	return p0 +
		(p1 - p0) * (weights.x) +
		(p2 - p0) * (weights.y) * (1.0 - weights.x) +
		(p3 - p1) * (weights.y * weights.x);
}

float noiseHeight(in vec2 pos)
{

	float noiseValue = 0.0;

	float localAplitude = amplitude;
	float localFrecuency = frecuency;

	for (int index = 0; index < octaves; index++)
	{

		noiseValue += NoiseInterpolation(pos + time * waterspeed, scale * localFrecuency) * localAplitude;
		noiseValue += NoiseInterpolation(pos.yx - time * waterspeed, scale * localFrecuency) * localAplitude;

		localAplitude /= 2.0;
		localFrecuency *= 2.0;
	}

	return noiseValue * 0.000001;
}

// =====================================================================
// Shadow map look up
bool whithinRange(vec2 texCoord)
{
	return texCoord.x >= 0.0 && texCoord.x <= 1.0 && texCoord.y >= 0.0 && texCoord.y <= 1.0;
}

float getShadowVisibility(vec3 rawNormal)
{
	float bias = clamp(0.005 * tan(acos(dot(rawNormal, lightDir))), 0.0, 0.01);
	float visibility = 1.0;

	if(whithinRange(inShadowMapPos.xy))
	{
		float curDepth = inShadowMapPos.z - bias;
		//visibility = texture(depthTexture, inShadowMapPos.xy).x  <  curDepth? 0.0 : 1.0;
		
		for (int i = 0; i < 4; i++)
		{
			visibility -= 0.25 * ( texture(depthTexture, inShadowMapPos.xy + poissonDisk[i] / 700.0).x  <  curDepth? 1.0 : 0.0 );
		}
		
	}
	else if(whithinRange(inShadowMapPos1.xy))
	{
		float curDepth = inShadowMapPos1.z - bias;
		//visibility = texture(depthTexture1, inShadowMapPos1.xy).x  <  curDepth? 0.0 : 1.0;
	
		for (int i = 0; i < 4; i++)
		{
			visibility -= 0.25 * ( texture( depthTexture1, inShadowMapPos1.xy + poissonDisk[i] / 700.0 ).x  <  curDepth? 1.0 : 0.0 );
		}
		
	}

	return visibility;
}

#else
layout (location=0) out vec4 lightdepth;
#endif

// ================================================================================

void main()
{
#ifdef SHADOW_MAP
	lightdepth = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1);
#else

#if defined WIRE_MODE || defined POINT_MODE
	vec3 rawNormal = vec3(0,1,0);
#else
	float u = outUV.x;
	float v = outUV.y;

	// COMPUTE NORMAL
	// ------------------------------------------------------------------------------
	float step = 0.0015;
	float tH = noiseHeight(vec2(u, v + step)); 
	float bH = noiseHeight(vec2(u, v - step));
	float rH = noiseHeight(vec2(u + step, v)); 
	float lH = noiseHeight(vec2(u - step, v));

	vec3 rawNormal = normalize(vec3(lH - rH, step * step, bH - tH));

	// Correct normal if we have pass from +X to -X, from +Z to -Z, viceversa, or both
	int xSign = sign(gridPos.x);
	int ySign = sign(gridPos.y);
	rawNormal.x = xSign != 0 ? rawNormal.x * xSign : rawNormal.x;
	rawNormal.z = ySign != 0 ? rawNormal.z * ySign : rawNormal.z;
#endif

	vec3 n = normalize((normal * vec4(rawNormal, 0.0)).xyz);	
	// COMPUTE COLOR
	// ------------------------------------------------------------------------------
	float alpha = 1.0;
#if defined WIRE_MODE || defined POINT_MODE
	vec3 color = vec3(0);
#else
	vec3 color = watercolor;
	alpha = 1.0 - texture(inInfo, gl_FragCoord.xy / screenSize).z;
#endif

	// APPLY SHADOW MAP
	// ------------------------------------------------------------------------------
	float visibility = getShadowVisibility(rawNormal);

	// OUTPUT TO G-BUFFERS
	// ------------------------------------------------------------------------------
	outColor = vec4(color, alpha);
	outNormal = vec4(n, 1.0);
	outPos1 = vec4(outPos, 1.0);
	outInfo = vec4(0, visibility, 0, alpha);
#if defined WIRE_MODE || defined POINT_MODE
	outSpecular = vec4(0);
	outEmissive = vec4(0);
#else
	outSpecular = vec4(1.0,1.0,1.0,1);
	outEmissive = vec4(0);
#endif
#endif
}