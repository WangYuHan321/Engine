#version 430 core

layout (location=0) out vec4 outColor;

layout(location=0) in vec2 outTexCoord;

uniform mat4 projMat;//ͶӰ����

uniform vec3 lightDirection;//���շ���

uniform sampler2D postProcessing_0;	// color

uniform sampler2D posBuffer;
uniform sampler2D depthBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D specularBuffer;//����Ⱦwater ���ò���vec4��1.0��

vec3 Raymarch(vec3 position, vec3 direction, float steep)
{
	vec3 prevRaySample, raySample;

	//��Ļ��׷
	for(int rayStepIdx = 0; rayStepIdx < 16; rayStepIdx++)
	{
		//�Ƚϵ�ǰ������Ⱥͳ������
		prevRaySample = raySample;//�����������̫������

		raySample = (rayStepIdx * steep) * direction + position;
		float zBufferVal = texture(depthBuffer, raySample.xy).x;

		// �������������λ����Ƚϸߡ�
		// ����ζ�����й���Ƚ�С�Ķ�����
		// ��ˣ������Ѿ��߹�������Ӧ�÷���ȥ

		if(raySample.z > zBufferVal)
		{
			vec3 minRaySample = prevRaySample;
			vec3 maxRaySample = raySample;
			vec3 midRaySample;

			//��������
			for(int i = 0;i < 6; i++)
			{
				midRaySample = mix(minRaySample, maxRaySample, 0.5);// ������Բ�ֵ
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
	//��ǰ��Ļ�����������
	vec3 ssPos = vec3(outTexCoord, depth); // xy��Ļ�ռ� z���

	//reflect ����
	vec3 camReflect = reflect(-pos, N);

	//�ӷ�����ͼ������Ļ�ռ�λ��
	vec3 pointAlongRefl =  camReflect * 10.0f + pos;//3Ϊ����λ��
	vec4 projPointAlong = projMat * vec4(pointAlongRefl, 1);
	projPointAlong /= projPointAlong.w;
	projPointAlong.xy = projPointAlong.xy * vec2(0.5, 0.5) + vec2(0.5, 0.5);//ת��Ϊ��Ļ�ϵ�����

	//��Ļ�ռ�ķ�������
	vec3 ssreflectdir = normalize(projPointAlong.xyz - ssPos);//ע�����ﶼ������Ļ�ռ� z�ڷǹ�һ���ռ�

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
