float4x4 mWVP;
float4 vLightDir;	// ????????
float4 vColor;		// ????*?޽? ??
float3 vEyePos;		// ī?޶???ġ(??????ǥ??)

struct VS_OUTPUT
{
    float4 Pos			: POSITION;
    float4 Color		: COLOR0;
};

VS_OUTPUT VS(
      float4 Pos    : POSITION,          // ??????ġ??ǥ
      float4 Normal : NORMAL,            // ????????
	  float2 Tex : TEXCOORD0

){
	VS_OUTPUT Out = (VS_OUTPUT)0;        // ???µ?????
	
	// ??ǥ??ȯ
	Out.Pos = mul(Pos, mWVP);
	
	// ??????
	float amb = -vLightDir.w;	// ȯ?汤?? ????
	
	float3 eye = normalize(vEyePos - Pos.xyz);
	float3 L = -vLightDir; // ??????ǥ?迡???? ????????
	float3 N = Normal.xyz;
	float3 R = -eye + 2.0f*dot(N,eye)*N;	// ?ݻ纤??
	
	Out.Color = vColor * max(amb, dot(Normal, -vLightDir))
				+ pow(max(0,dot(L, R)), 10);
	
	return Out;
}

float4 PS(VS_OUTPUT In) : COLOR
{   
    return In.Color;
}

technique TShader
{
    pass P0
    {
        VertexShader = compile vs_1_1 VS();
        PixelShader  = compile ps_2_0 PS();
    }
}
