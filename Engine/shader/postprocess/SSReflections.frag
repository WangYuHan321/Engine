#version 430 core

layout (location=0) out vec4 outColor;

layout(location=0) in vec2 outTexCoord;

uniform mat4 projMat;//投影矩阵

uniform vec3 lightDirection;//光照方向

uniform sampler2D postProcessing_0;	// color

uniform sampler2D posBuffer;
uniform sampler2D depthBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D specularBuffer;//在渲染water 设置参数vec4（1.0）

vec3 Raymarch(vec3 position, vec3 direction, float steep)
{
	vec3 prevRaySample, raySample;

	//屏幕光追
	for(int rayStepIdx = 0; rayStepIdx < 16; rayStepIdx++)
	{
		//比较当前射线深度和场景深度
		prevRaySample = raySample;//这个是留步长太大作用

		raySample = (rayStepIdx * steep) * direction + position;
		float zBufferVal = texture(depthBuffer, raySample.xy).x;

		// 如果我们所处的位置深度较高、
		// 这意味着曾有过深度较小的东西、
		// 因此，我们已经走过了它，应该反回去

		if(raySample.z > zBufferVal)
		{
			vec3 minRaySample = prevRaySample;
			vec3 maxRaySample = raySample;
			vec3 midRaySample;

			//二分搜索
			for(int i = 0;i < 6; i++)
			{
				midRaySample = mix(minRaySample, maxRaySample, 0.5);// 混合线性插值
				zBufferVal = texture(depthBuffer, midRaySample.xy).x;

				if(midRaySample.z > zBufferVal)
					maxRaySample = midRaySample;
				else
					minRaySample = midRaySample;
			}
			return texture(postProcessing_0, midRaySample.xy).rgb;
		}

	}

	return texture(postProcessing_0, outTexCoord).rgb;
}

vec3 ComputeReflectionColor(float depth, vec3 pos, vec3 N)
{
	//当前屏幕坐标和深度组合
	vec3 ssPos = vec3(outTexCoord, depth); // xy屏幕空间 z深度

	//reflect 向量
	vec3 camReflect = reflect(-pos, N);

	//从反射视图方向看屏幕空间位置
	vec3 pointAlongRefl =  camReflect * 10.0f + pos;//3为坐标位置
	vec4 projPointAlong = projMat * vec4(pointAlongRefl, 1);
	projPointAlong /= projPointAlong.w;
	projPointAlong.xy = projPointAlong.xy * vec2(0.5, 0.5) + vec2(0.5, 0.5);//转换为屏幕上的向量

	//屏幕空间的反射向量
	vec3 ssreflectdir = normalize(projPointAlong.xyz - ssPos);//注意这里都是在屏幕空间 z在非归一化空间

	float viewFactor = clamp(dot(normalize(-pos), N),0,1);

	return Raymarch(ssPos, ssreflectdir, 0.02);
}

void main()
{
	vec3 pos = texture(posBuffer, outTexCoord).xyz;
	vec3 N = texture(normalBuffer, outTexCoord).xyz;
	float depth = texture(depthBuffer, outTexCoord).x;
	float reflection = texture(specularBuffer, outTexCoord).w;

	// Compute relfection only for specular surfaces
	if(reflection > 0.0)
	{
		outColor = vec4(ComputeReflectionColor(depth, pos, N), 1.0) * 0.5 + texture(postProcessing_0, outTexCoord) * 0.5;
	}
	else
	{
		outColor = texture(postProcessing_0, outTexCoord);
	}
}
