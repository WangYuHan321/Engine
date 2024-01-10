#version 430 core

layout (location=0) out vec4 outColor;

layout(location=0) in vec2 outTexCoord;

uniform vec2 lightScreenPos;
uniform float exposure;
uniform float decay;//˥������
uniform float density;//��������
uniform float weight;//Ȩ��
uniform bool onlyPass;
uniform float alpha;

#define NUM_SAMPLES 100

uniform sampler2D postProcessing_0; // color
uniform sampler2D postProcessing_1; // emission
uniform sampler2D postProcessing_2; // god rays

void main()
{
	if(!onlyPass)
	{
		//������Ļ�ռ��д����ص���Դ��vector
		vec2 deltaTexCoord = (outTexCoord - lightScreenPos);
		float distFactor = clamp( 1.0 / length(deltaTexCoord), 0.0, 1.0);
		vec2 uv = outTexCoord;
		deltaTexCoord *= 1.0 /  (float(NUM_SAMPLES) * density);

		//�洢��ʼ������
		vec4 result = texture(postProcessing_2, outTexCoord);

		//��������˥��ϵ��
		float illuminationDecay = 1.0f;
	
		for(int i = 0; i < NUM_SAMPLES; i++)
		{
			//�����߲���������λ��
			uv -= deltaTexCoord;
			vec4 sampl = texture(postProcessing_2, uv);

			//Ӧ��������˥������
			sampl *= illuminationDecay * weight;

			//������ɫ
			result += sampl;

			//����ָ��˥������
			illuminationDecay *= decay;
		}

		//���������ɫ
		outColor = texture(postProcessing_0, outTexCoord) + result * exposure;
	}
	else
	{
		outColor = texture(postProcessing_0, outTexCoord);
	}

}