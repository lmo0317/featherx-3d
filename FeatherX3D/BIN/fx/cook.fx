
float4x4 mWVP;
float4 vColor;	
float4 vLightDir;
float3 vEyePos;
bool   gold;

texture DiffuseTexture : DiffuseMap;
sampler ColorMapSampler = sampler_state{};


// -------------------------------------------------------------
// �������̴����� �ȼ����̴��� �ѱ�� ������
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos			: POSITION;
    float4 Color		: COLOR0;
    float3 N			: TEXCOORD0;
    float3 X			: TEXCOORD1;
    float2 Tex			: TEXCOORD2;
};

// -------------------------------------------------------------
// ��鷻��
// -------------------------------------------------------------
VS_OUTPUT VS(
      float4 Pos    : POSITION,          // ������ġ��ǥ
      float4 Normal : NORMAL,            // ��������
      float2 Tex    : TEXCOORD0
){
	VS_OUTPUT Out = (VS_OUTPUT)0;        // ��µ�����
	
	// ��ǥ��ȯ
	Out.Pos = mul(Pos, mWVP);
	
	// Ȯ�걤+ȯ�汤
	float amb = -vLightDir.w;	// ȯ�汤�� ����
	float3 L = -vLightDir; // ������ǥ�迡���� ��������
	Out.Color = vColor * max(amb, dot(Normal, -vLightDir));
	
	// �ݿ��ݻ�� ����
	Out.N = Normal.xyz;
	Out.X = Pos.xyz;
	Out.Tex = Tex.xy;
	return Out;
}
// -------------------------------------------------------------
float4 PS(VS_OUTPUT In) : COLOR
{   
	float3 L = -vLightDir.xyz;				// ��������
	float3 N = normalize(In.N);				// ��������
	float3 V = normalize(vEyePos - In.X);	// �ü�����
	float3 H = normalize(L + V);			// ��������
	
	// ��꿡 �ʿ��� �������� ����
	float NV = dot(N,V);
	float NH = dot(N,H);
	float VH = dot(V,H);
	float NL = dot(N,L);
	float LH = dot(L,H);
	
	// Beckmann�����Լ�
	const float m = 0.5f;// ��ģ����
	float NH2 = NH*NH;
	float D = exp(-(1-NH2)/(NH2*m*m))/(4*m*m*NH2*NH2);
	
	// ���ϰ�����
	float G = min(1,min(2*NH*NV/VH, 2*NH*NL/VH));
	
	// ������
	float n = 100.0f;// ���ұ������� �Ǽ���
	float g = sqrt(n*n+LH*LH-1);
	float gpc = g+LH;
	float gnc = g-LH;
	float cgpc = LH*gpc-1;
	float cgnc = LH*gnc+1;
	float F = 0.5f*gnc*gnc*(1+cgpc*cgpc/(cgnc*cgnc))/(gpc*gpc);
	
	// �ݼ��� ��
	float4 ks = {2.0f*0.486f, 2.0f*0.433f, 2.0f*0.185f, 1.0f};
	float4 color = tex2D(ColorMapSampler, In.Tex);	
	
	if(In.Tex.x > 1)
		return (In.Color + ks * max(0,F*D*G/NV));
	else
		return (In.Color + ks * max(0,F*D*G/NV)) * color;
}

//-----------------------------------------------------------------------------------------------
technique vs_t
{
   pass P0
   {		
        VertexShader = compile vs_1_1 VS();
        PixelShader  = compile ps_2_0 PS();        
         Texture[0] = (DiffuseTexture);        
   }
}
