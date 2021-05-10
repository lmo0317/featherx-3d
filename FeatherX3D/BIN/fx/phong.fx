float4x4 mWVP;
float4 vLightDir;	// 광원방향
float4 vColor;		// 광원*메시 색
float3 vEyePos;		// 카메라위치(로컬좌표계)

struct VS_OUTPUT
{
    float4 Pos			: POSITION;
    float4 Color		: COLOR0;
};

VS_OUTPUT VS(
      float4 Pos    : POSITION,          // 로컬위치좌표
      float4 Normal : NORMAL,            // 법선벡터
	  float2 Tex : TEXCOORD0

){
	VS_OUTPUT Out = (VS_OUTPUT)0;        // 출력데이터
	
	// 좌표변환
	Out.Pos = mul(Pos, mWVP);
	
	// 정점색
	float amb = -vLightDir.w;	// 환경광의 강도
	
	float3 eye = normalize(vEyePos - Pos.xyz);
	float3 L = -vLightDir; // 로컬좌표계에서의 광원벡터
	float3 N = Normal.xyz;
	float3 R = -eye + 2.0f*dot(N,eye)*N;	// 반사벡터
	
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
