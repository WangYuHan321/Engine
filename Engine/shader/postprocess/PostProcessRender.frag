#version 430 core

out vec4 outColor;

layout(location=0) in vec2 outTexCoord;

uniform sampler2D postProcessing_0;

void main()
{
	outColor = texture(postProcessing_0, outTexCoord);
}