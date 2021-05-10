// Constant registers 
// c0 - c3 world * view * proj
// c4 color of sphere
//
// Input registers
// dcl_position v0


float4x4 matWorldViewProj	:register(c0);	
float4 SphereColor			:register(c4);



// -------------------------------------------------------------
// Output channels
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos  : POSITION;
	float4 Col	: COLOR0;
};



VS_OUTPUT VS(float4 Pos : POSITION : register(v0)) 
{
    VS_OUTPUT Out = (VS_OUTPUT)0;      
    Out.Pos = mul(Pos, matWorldViewProj);	// transform Position
	Out.Col = SphereColor;

   
   return Out;
}

