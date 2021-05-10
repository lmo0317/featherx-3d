string description = "파티클 스프레이";
string shader = "ONLY_VS";

//tell max to transpose the matrix
bool RowMajor = true;
bool Particle = true;

//------------------------------------
float4x4 view			: VIEW;
float4x4 viewProj		: VIEWPROJ;

//------------------------------------
struct VS_OUTPUT
{
   float4 Pos:     	POSITION;
   float4 Diffuse:	COLOR0;
   float2 Tex:     	TEXCOORD0;
};

// texture
texture DiffuseTexture : DiffuseMap;
float FrameWidthNum;
float FrameHeightNum;
float Scale;

VS_OUTPUT V_Shader( float4 inPos			: POSITION,
					float2 inVertex			: TEXCOORD0,
					float2 inFactor			: TEXCOORD1,
					float4 inColor			: TEXCOORD2,
					float1 inRot			: TEXCOORD3,
					float2 inAxisAngle		: TEXCOORD4)

{
	//inAxisAngle = 100 * (3.14/180);
	int nFrame = (int)inPos.w;

	float2 Factor;
	Factor.x = sin( inRot ) * 1;
	Factor.y = cos( inRot ) * 1;

	Factor.x += sin(inAxisAngle.x) * (inFactor.x - 1);
	Factor.y += cos(inAxisAngle.x) * (inFactor.x - 1);

	Factor.x += sin(inAxisAngle.y) * (inFactor.y - 1);
	Factor.y += cos(inAxisAngle.y) * (inFactor.y - 1);


	float3 fTemp1 = view[0]*Factor.x * Scale;
	float3 fTemp2 = view[1]*Factor.y * Scale;
	float3 Pos = inPos.xyz + fTemp1 + fTemp2;

	//Pos.x += cos(inAxisAngle) * inFactor.x;
	//Pos.y += sin(inAxisAngle) * inFactor.y;

	VS_OUTPUT Output;
	Output.Pos		= mul( float4(Pos,1), viewProj );
	Output.Diffuse	= inColor;
	float1 fWidth	= 1.f / FrameWidthNum;
	float1 fHeight	= 1.f / FrameHeightNum;
	Output.Tex.x	= (fWidth *((int)(nFrame)%(int)(FrameWidthNum))) +(fWidth*inVertex.x);
	Output.Tex.y	= (fHeight*((int)(nFrame)/(int)(FrameHeightNum)))+(fHeight*inVertex.y);
	return Output;
}

technique vs_t
{
    pass p0
    {
        VertexShader = compile vs_1_1 V_Shader();

        // texture
        Texture[0] = (DiffuseTexture);

        ColorOp[0]   = MODULATE;
        ColorArg1[0] = TEXTURE;
        ColorArg2[0] = DIFFUSE;
        AlphaOp[0]   = MODULATE;
        AlphaArg1[0] = TEXTURE;
        AlphaArg2[0] = DIFFUSE;

        ColorOp[1]   = DISABLE;
        AlphaOp[1]   = DISABLE;
    }
}
