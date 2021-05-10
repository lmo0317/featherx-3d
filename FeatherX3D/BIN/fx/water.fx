#define NWAVES 2

float4x4 WorldViewProj	: WORLDVIEWPROJ;
float4 eyePosition;
float time;
float2 textureScale;
float waveFreq;
float waveAmp;
float BumpScale;
float2 bumpSpeed;
float4 deepColor;
float4 shallowColor;
float waterAmount;
float4 reflectionColor;
float reflectionAmount;
float reflectionBlur;
float fresnelPower;
float fresnelBias;
float hdrMultiplier;
texture texture0;
texture texture1;

sampler2D NormalMap: TEXUNIT0 = sampler_state
{
	Texture	  = (texture0);
    MIPFILTER = LINEAR;
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
};

samplerCUBE EnvironmentMap: TEXUNIT0 = sampler_state
{
	Texture	  = (texture1);
    MIPFILTER = LINEAR;
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
};

struct Wave {
  float freq;
  float amp;
  float phase;
  float2 dir;
};

struct a2v {
    float4 Position : POSITION;   // in object space
    float2 TexCoord : TEXCOORD0;
};

struct v2f {
    float4 Position  : POSITION;
    float3 rotMatrix1 : TEXCOORD0;
    float3 rotMatrix2 : TEXCOORD1;
    float3 rotMatrix3 : TEXCOORD2;
    float2 bumpCoord0 : TEXCOORD3;
    float2 bumpCoord1 : TEXCOORD4;
    float2 bumpCoord2 : TEXCOORD5;
    float3 eyeVector : TEXCOORD7;
};

struct p2f
{
    float4 color    : COLOR0;
};

Wave wave[NWAVES] = {
   { 1.0, 1.0, 0.5, float2(-1, 0) },
   { 2.0, 0.5, 1.7, float2(-0.7, 0.7) }
};

void main_vs(in a2v IN, out v2f OUT)
{
    wave[0].freq = waveFreq;
    wave[0].amp = waveAmp;
    wave[1].freq = waveFreq * 3.0;
    wave[1].amp = waveAmp * 0.33;
    float4 P = IN.Position;
    float ddx = 0.0, ddy = 0.0;
    float deriv;
    float angle;
    
    for(int i = 0; i<NWAVES; ++i)
    {
        angle = dot(wave[i].dir, P.xz) * wave[i].freq + time * wave[i].phase;
        P.y += wave[i].amp * sin( angle );
        deriv = wave[i].freq * wave[i].amp * cos(angle);
        ddx -= deriv * wave[i].dir.x;
        ddy -= deriv * wave[i].dir.y;
    }

    OUT.rotMatrix1.xyz = BumpScale * normalize(float3(1, ddy, 0));
    OUT.rotMatrix2.xyz = BumpScale * normalize(float3(0, ddx, 1));
    OUT.rotMatrix3.xyz = normalize(float3(ddx, 1, ddy));
    OUT.Position = mul(P, WorldViewProj);
    OUT.bumpCoord0.xy = IN.TexCoord * textureScale + time * bumpSpeed;
    OUT.bumpCoord1.xy = IN.TexCoord * textureScale * 2.0 + time * bumpSpeed * 4.0;
    OUT.bumpCoord2.xy = IN.TexCoord * textureScale * 4.0 + time * bumpSpeed * 8.0;
    OUT.eyeVector = P.xyz - eyePosition;
}

void main_fs(in v2f IN, out p2f OUT)
{
    float4 t0 = tex2D(NormalMap, IN.bumpCoord0) * 2.0 - 1.0;
    float4 t1 = tex2D(NormalMap, IN.bumpCoord1) * 2.0 - 1.0;
    float4 t2 = tex2D(NormalMap, IN.bumpCoord2) * 2.0 - 1.0;
    float3 N = t0.xyz + t1.xyz + t2.xyz;
    float3x3 m;
    m[0] = IN.rotMatrix1;
    m[1] = IN.rotMatrix2;
    m[2] = IN.rotMatrix3;
    N = normalize( mul( N, m ) );
    float3 E = normalize(IN.eyeVector);
    float4 R;
    R.xyz = reflect(E, N);
    R.z = -R.z;
    R.w = reflectionBlur;
    float4 reflection = texCUBEbias(EnvironmentMap, R);
    reflection.rgb *= (reflection.r + reflection.g + reflection.b) * hdrMultiplier;
    float facing = 1.0 - max(dot(-E, N), 0);
    float fresnel = saturate(fresnelBias + pow(facing, fresnelPower));
    float4 waterColor = lerp(shallowColor, deepColor, facing) * waterAmount;
    reflection = lerp(waterColor,  reflection * reflectionColor, fresnel) * reflectionAmount;
    OUT.color = waterColor + reflection;
}

technique water
{
    pass p0
    {
        vertexshader = compile vs_1_1 main_vs();
        pixelshader = compile ps_2_0 main_fs();
    }
}