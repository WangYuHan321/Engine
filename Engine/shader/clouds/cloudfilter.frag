#version 430 core

layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outEmission;
layout (location=2) out vec4 outGodRays;

in vec2 outTexCoord;

uniform int frameIter;
uniform mat4 oldProjView;

uniform vec3 realLightColor;

uniform sampler2D repro1;
uniform sampler2D repro2;

uniform sampler2D vel1;
uniform sampler2D vel2;

uniform vec2 texelSize;


#define maskSize 8u
uniform vec2 bigaffectedTexels[maskSize] = vec2[](
	vec2(-1.0,1.0), vec2(0.0,1.0), vec2(1.0,1.0),
	vec2(-1.0,0.0), vec2(1.0,0.0),
	vec2(-1.0,-1.0), vec2(0.0,-1.0), vec2(1.0,-1.0));


vec4 getCloudInfo(vec2 modUV)
{
	int x = int(gl_FragCoord.x + modUV.x);
	float y = gl_FragCoord.y - 0.5 + modUV.y;
		
	float u = (float(x) * texelSize.x);
	float v = y * texelSize.y;

	vec2 realUV = vec2(u, v);

	int coordIter = x % 2;
	return coordIter == 0? texture(repro1, realUV) : texture(repro2, realUV);
}
void main()
{
	vec4 color = getCloudInfo(vec2(0));

	outColor = color;

	outEmission = vec4(0,0,0, color.a);

	vec3 gr = color.rgb * clamp(1.0 - color.a, 0, 1) * 1.0;
	outGodRays = vec4(gr, color.a);
	gl_FragDepth = 0.999;

}