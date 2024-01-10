#version 430 core

layout(location=0) in vec3 inPos;
layout(location=1) in vec2 inUV;

layout(location=0) out vec2 outTexCoord;
layout(location=1) out vec3 outPos;

void main()
{
	outTexCoord = inUV;
	outPos = inPos;
	gl_Position = vec4(inPos , 1);
}