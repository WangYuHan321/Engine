#version 430 core

layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outEmission;
layout (location=2) out vec4 outGodRaysInfo;

layout (location=0) in vec2 texCoord;

uniform sampler2D postProcessing_0; // color
uniform sampler2D postProcessing_1; // normal
uniform sampler2D postProcessing_2; // specular
uniform sampler2D postProcessing_3; // emissive
uniform sampler2D postProcessing_4; // pos
uniform sampler2D postProcessing_5; // info
uniform sampler2D postProcessing_6; // depth

// ===============================================
// back ground color, used for fog effect and ambient lighting
uniform vec3 zenitColor;
uniform vec3 horizonColor;

uniform float colorFactor;

// Different lights data

uniform int numSpotLights;
uniform int numPointLights;

layout(std140, binding = 0) uniform DLBuffer
{
	vec4 DLdirection [1];
	vec4 DLcolor [1];
	vec4 DLkFactors [1];
};

/*
layout(std140, binding = 1) uniform SLBuffer
{
	vec4 SLposition [];
	vec4 SLdirection [];
	vec4 SLcolor [];
	vec4 SLattenuation [];
	vec4 SLkFactors [];
};

layout(std140, binding = 2) uniform PLBuffer
{
	vec4 PLposition [];
	vec4 PLcolor [];
	vec4 PLattenuation [];
	vec4 PLkFactors [];
};
*/

// Objects properties to be used across shading fuctions
vec3 pos;
float distToCam;
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 Ke;
vec3 N;
float depth;
float alpha = 50.0;
vec3 ambientColor;

//��ɫ����
vec3 diffuseOrenNayar(vec3 ld, float roughness, vec3 albedo) 
{  
	vec3 v = normalize(-pos);
	float lv = dot(ld, v);
	float nl = dot(ld, N);
	float nv = dot(N, v);

	float s = lv - nl * nv;
	float t = mix(1.0, max(nl, nv), step(0.0, s));

	float a2 = roughness * roughness;
	vec3 A = 1.0 + a2 * (albedo / (a2 + 0.13) + 0.5 / (a2 + 0.33));
	vec3 B = vec3(0.45 * a2 / (a2 + 0.09));

	return albedo * max(0.0, nl) * (A + B * s / t) / 3.1415;
}

vec3 diffuseLambert(vec3 dl, vec3 albedo)
{
	return max(dot(N, dl), 0) * albedo;
}

vec3 processDirectionalLight(in float visibility)
{
	vec3 c = vec3(0,0,0);
	// Minimun visibility is 0.1 in shadow places to avoid unlit shading look in shadowed zones
	visibility = max(visibility, 0.1);

	vec3 L = DLdirection[0].xyz;
	vec3 lightColor = DLcolor[0].rgb;
	lightColor.y *= colorFactor * 1.5;
	lightColor.z *= colorFactor;

	vec3 Kfactors = DLkFactors[0].xyz;

	// Ambient
	c += ambientColor * Kfactors.x * Ka;

	// Diffuse
	c += diffuseLambert(L, lightColor * Kfactors.y * Kd) * visibility;
	//c += diffuseOrenNayar(L, 1.0, Kd * Kfactors.y * lightColor) * visibility; // too dark

	// Specular
	vec3 R = normalize(reflect(-L, N));
	vec3 V = normalize(-pos);
	float sFactor = max(dot(R, V), 0.01);
	c += lightColor * Kfactors.z * Ks * pow(sFactor, alpha) * visibility;

	return c;
}

vec3 processAtmosphericFog(in vec3 shadedColor)
{
	float d = length(pos);
	d -= 15.0;
	float lerpVal = d > 0.0? 1.0 / exp(0.002 * d * d) : 1.0;
	
	return mix(ambientColor * colorFactor, shadedColor, lerpVal);
}

// ================================================================================

void main()
{
	vec4 gbuffercolor =		texture(postProcessing_0, texCoord);
	vec4 gbuffernormal =	texture(postProcessing_1, texCoord);
	vec4 gbufferspec =		texture(postProcessing_2, texCoord);
	vec4 gbufferemissive =	texture(postProcessing_3, texCoord);
	vec4 gbufferpos =		texture(postProcessing_4, texCoord);
	vec4 gbufferinfo =		texture(postProcessing_5, texCoord);
	depth =					texture(postProcessing_6, texCoord).x;

	N = gbuffernormal.xyz;
	pos = gbufferpos.xyz;
	Ka = gbuffercolor.rgb;;
	Kd = Ka;
	Ks = gbufferspec.rgb;
	Ke = gbufferemissive.rgb;

	ambientColor = mix(horizonColor, zenitColor, 0.2);

	vec3 shaded = processDirectionalLight(gbufferinfo.y);
	shaded = processAtmosphericFog(shaded);

	outColor = vec4(shaded, 1.0);
	outEmission = gbufferemissive;
	outGodRaysInfo = vec4(0);
	gl_FragDepth = depth;
}