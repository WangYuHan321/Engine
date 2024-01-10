#version 430 core

layout (location=0) out vec4 outColor;

layout(location=0) in vec2 outTexCoord;

uniform vec2 lightScreenPos;
uniform float exposure;
uniform float decay;//衰减因子
uniform float density;//控制因子
uniform float weight;//权重
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
		//计算屏幕空间中从像素到光源的vector
		vec2 deltaTexCoord = (outTexCoord - lightScreenPos);
		float distFactor = clamp( 1.0 / length(deltaTexCoord), 0.0, 1.0);
		vec2 uv = outTexCoord;
		deltaTexCoord *= 1.0 /  (float(NUM_SAMPLES) * density);

		//存储初始化样本
		vec4 result = texture(postProcessing_2, outTexCoord);

		//设置照明衰减系数
		float illuminationDecay = 1.0f;
	
		for(int i = 0; i < NUM_SAMPLES; i++)
		{
			//沿射线步进样本的位置
			uv -= deltaTexCoord;
			vec4 sampl = texture(postProcessing_2, uv);

			//应用样本的衰减比例
			sampl *= illuminationDecay * weight;

			//叠加颜色
			result += sampl;

			//更新指数衰减因子
			illuminationDecay *= decay;
		}

		//混合最终颜色
		outColor = texture(postProcessing_0, outTexCoord) + result * exposure;
	}
	else
	{
		outColor = texture(postProcessing_0, outTexCoord);
	}

}