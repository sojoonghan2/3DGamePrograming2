#ifndef _BILLBOARD_FX_
#define _BILLBOARD_FX_
#include "params.fx"

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    row_major matrix matWorld : W;
    row_major matrix matWV : WV;
    row_major matrix matWVP : WVP;
    uint instanceID : SV_InstanceID;
};

struct VS_OUT
{
    float4 posW : POSITION;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    // �ν��Ͻ� Ȱ��ȭ ���ο� ���� ���� ��ǥ ���
    if (g_int_0 == 1)
    {
        output.posW = mul(float4(input.pos, 1.0f), input.matWorld);
    }
    else
    {
        output.posW = mul(float4(input.pos, 1.0f), g_matWorld);
    }

    // UV ����
    output.uv = input.uv;

    return output;
}

struct GS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 viewPos : POSITION;
    float4 color : COLOR;
};

[maxvertexcount(4)]
void GS_Main(point VS_OUT input[1], inout TriangleStream<GS_OUT> outputStream)
{
    float3 up = normalize(float3(g_matViewInv._21, g_matViewInv._22, g_matViewInv._23));
    float3 right = normalize(float3(g_matViewInv._11, g_matViewInv._12, g_matViewInv._13));

    float halfWidth = g_float_0;
    float halfHeight = g_float_1;

    float4 vertices[4];
    vertices[0] = float4(input[0].posW.xyz - right * halfWidth - up * halfHeight, 1.0f); // ���ϴ�
    vertices[1] = float4(input[0].posW.xyz - right * halfWidth + up * halfHeight, 1.0f); // �»��
    vertices[2] = float4(input[0].posW.xyz + right * halfWidth - up * halfHeight, 1.0f); // ���ϴ�
    vertices[3] = float4(input[0].posW.xyz + right * halfWidth + up * halfHeight, 1.0f); // ����

    float2 uv[4] =
    {
        float2(0.0f, 1.0f), // ���ϴ�
        float2(0.0f, 0.0f), // �»��
        float2(1.0f, 1.0f), // ���ϴ�
        float2(1.0f, 0.0f) // ����
    };

    GS_OUT output = (GS_OUT) 0;
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        output.pos = mul(vertices[i], g_matWVP);
        output.viewPos = mul(vertices[i], g_matWV).xyz;
        output.uv = clamp(uv[i], 0.0f, 1.0f); // UV Ŭ����
        outputStream.Append(output);
    }
}

struct PS_OUT
{
    float4 color : SV_Target0;
};

PS_OUT PS_Main(GS_OUT input)
{
    PS_OUT output = (PS_OUT) 0;
    
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    
    if (g_tex_on_0 == 1)
    {
        color = g_tex_0.Sample(g_sam_0, input.uv);
        if (color.a < 0.1f)
            discard;
    }
    
    output.color = color;
    
    return output;
}

#endif