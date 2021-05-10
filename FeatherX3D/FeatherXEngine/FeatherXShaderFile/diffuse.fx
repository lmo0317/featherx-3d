// Constant registers 
// c0 - c3 world space transposed
// c8 - c11 world * view * proj
// c12 - Light Direction (In World Space)
// c33 - four constants with 0.5f
//
// Input registers
// dcl_position v0
// dcl_normal0 v3
// dcl_normal1 v4
// dcl_texcoord v7
// dcl_tangent0 v8
// dcl_tangent1 v9


float4x4 matWorldViewProj	:register(c8);	
float4x4 matWorld			:register(c0);	
float4 LightDir				:register(c12);
float4 IntPolFactor			:register(c37);


// -------------------------------------------------------------
// Output channels
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float3 Light : COLOR0;
    float2 Tex : TEXCOORD0;
    float2 Tex1 : TEXCOORD1;    
};



VS_OUTPUT VS(float4 Pos : POSITION : register(v0), 
			float3 Normal : NORMAL : register(v3),
			float3 Normal1 : NORMAL1 : register(v4),
			float3 Tex : TEXCOORD0 : register(v7),						
			float3 Tangent : TANGENT : register(v8),						
			float3 Tangent1 : TANGENT1 : register(v9))												
{
    VS_OUTPUT Out = (VS_OUTPUT)0;      
    Out.Pos = mul(Pos, matWorldViewProj);	// transform Position
    
    // decompress normals 
	// we use D3DCOLOR so we have to divide by 255.01
	// and to swizzle v3
	// the compression uses (value * 127.5) + 127.5
	// the decompresion uses (value / 127.5) - 1.0
	// /127.5 leads to a range from 0..2
	// -1 leads to a range from -1..1
	// the multiplication with 255.01f is done because of D3DCOLOR
	// which must be used on Geforce3/4TI cards, because they don't
	// support UBYTE4
	// (Normal / (127.5 * 250.01)) - 1.0f
	// Interpolation: normal1 +  fPol * (normal2 - normal1)
    float3 Norm = (Normal /127.5f * 250.01f) - 1.0f;  // normal decompression
    float3 Norm1 = (Normal1 /127.5f * 250.01f) - 1.0f;  // normal decompression    
    float3 NormInt = ((Norm1 - Norm) * IntPolFactor.x) + Norm;
    
    // decompress tangents and interpolate them
	// tangent1 +  fPol * (tangent2 - tangent1)
	float3 Tan = (Tangent / 127.5f * 250.01f) - 1.0f;  // tangent decompression
   	float3 Tan1 = (Tangent1 /127.5f * 250.01f) - 1.0f;  // tangent decompression
   	float3 TanInt = ((Tan1 - Tan) * IntPolFactor.x) + Tan;

   	// compute the 3x3 tranform matrix 
    // to transform from world space to tangent space
    float3x3 worldToTangentSpace;
    worldToTangentSpace[0] = mul(TanInt, matWorld);
    worldToTangentSpace[1] = mul(cross(TanInt, NormInt), matWorld);
    worldToTangentSpace[2] = mul(NormInt, matWorld);
	
	// transform the light vector with U, V, W
	float3 Light = Pos - LightDir;
    Out.Light.xyz = ((normalize(mul(worldToTangentSpace, Light))) * 0.5) + 0.5; 	// L
    
    Out.Tex = Tex.xy;
    Out.Tex1 = Tex.xy;    
    
   return Out;
}

