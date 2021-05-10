// Constant registers 
// c8 - c11 world * view * proj
//
// Input registers
// dcl_position v0
// dcl_texcoord v7


float4x4 matWorldViewProj	:register(c8);	


// -------------------------------------------------------------
// Output channels
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float2 Tex : TEXCOORD0;
};



VS_OUTPUT VS(float4 Pos : POSITION : register(v0), 
			float3 Tex : TEXCOORD0 : register(v7))						
{
    VS_OUTPUT Out = (VS_OUTPUT)0;      
    Out.Pos = mul(Pos, matWorldViewProj);	// transform Position

    Out.Tex = Tex.xy;
    
   return Out;
}

