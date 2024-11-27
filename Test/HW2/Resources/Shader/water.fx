#ifndef _DEFAULT_FX_
#define _DEFAULT_FX_

#include "params.fx"

// Water parameters
cbuffer WATER_PARAMS : register(b3)
{
    float g_Time; // �ð� ��
    float g_ScrollSpeed; // �ؽ�ó ��ũ�� �ӵ�
    float g_Transparency; // �� ����
    float g_ReflectionStrength; // �ݻ� ����
};

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 worldPos : POSITION;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output;
    float4 worldPos = mul(float4(input.pos, 1.0f), g_matWorld);
    output.pos = mul(worldPos, g_matWVP);
    output.uv = input.uv;
    output.worldPos = worldPos.xyz;
    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    // UV ��ũ�Ѹ�
    float2 uv = input.uv + float2(g_Time * g_ScrollSpeed, g_Time * g_ScrollSpeed);

    // �ؽ�ó ���ø�
    float4 color = g_tex_0.Sample(g_sam_0, uv); // Diffuse Map
    float4 normal = g_tex_1.Sample(g_sam_0, uv); // Normal Map

    // �ݻ� ���� ���
    float3 viewDir = normalize(input.worldPos - g_matViewInv._41_42_43);
    float3 normalVector = normalize(normal.xyz * 2.0f - 1.0f); // ��� �ؽ�ó�� [-1, 1] ������ ��ȯ �ʿ�
    float3 reflection = reflect(viewDir, normalVector);

    // �ݻ� ȿ�� ����
    float2 reflectionUV = reflection.xy * 0.5 + 0.5; // [0, 1] ������ ����
    float4 reflectionColor = g_tex_2.Sample(g_sam_0, reflectionUV); // Reflection Map

    // ���� ����
    float4 finalColor = lerp(color, reflectionColor, g_ReflectionStrength);
    finalColor.a *= g_Transparency;

    return finalColor;
}

technique11 Render
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS_Main()));
        SetPixelShader(CompileShader(ps_5_0, PS_Main()));
    }
}

#endif
