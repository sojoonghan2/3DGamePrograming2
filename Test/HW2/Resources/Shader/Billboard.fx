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
   
    output.color = float4(1, 0, 0, 1); // VS에서는 빨간색
   
    if (g_int_0 == 1)
    {
        output.posW = mul(float4(input.pos, 1.f), input.matWorld);
        output.color = float4(0, 1, 0, 1); // instancing이면 녹색
    }
    else
    {
        output.posW = mul(float4(input.pos, 1.f), g_matWorld);
        output.color = float4(0, 0, 1, 1); // 아니면 파란색
    }
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
    float3 up = float3(0.f, 1.f, 0.f);
    float3 look = g_vec4_0.xyz; // 카메라 위치에서 정점으로의 방향
    float3 right = normalize(cross(up, look)); // 빌보드의 오른쪽 방향
   
    float halfWidth = g_float_0; // 빌보드의 절반 너비
    float halfHeight = g_float_1; // 빌보드의 절반 높이
   
   // 입력값 디버그
    input[0].color = float4(halfWidth, halfHeight, 0, 1); // width와 height를 색상으로 표시
   
    float4 vertices[4];
    vertices[0] = float4(input[0].posW.xyz - right * halfWidth - up * halfHeight, 1.0f); // 좌하단
    vertices[1] = float4(input[0].posW.xyz - right * halfWidth + up * halfHeight, 1.0f); // 좌상단
    vertices[2] = float4(input[0].posW.xyz + right * halfWidth - up * halfHeight, 1.0f); // 우하단
    vertices[3] = float4(input[0].posW.xyz + right * halfWidth + up * halfHeight, 1.0f); // 우상단
   
    float2 uv[4] =
    {
        float2(0.0f, 1.0f), // 좌하단
       float2(0.0f, 0.0f), // 좌상단
       float2(1.0f, 1.0f), // 우하단
       float2(1.0f, 0.0f) // 우상단
    };
   
    GS_OUT output = (GS_OUT) 0;
   [unroll]
    for (int i = 0; i < 4; ++i)
    {
        output.pos = mul(vertices[i], g_matWVP);
        output.viewPos = mul(vertices[i], g_matWV).xyz;
        output.uv = uv[i];
        output.color = float4(1, 1, 0, 1); // 노란색으로 강제 출력
        outputStream.Append(output);
    }
}

struct PS_OUT
{
    float4 position : SV_Target0;
    float4 normal : SV_Target1;
    float4 color : SV_Target2;
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
        
    output.position = float4(input.viewPos.xyz, 1.f); // w값을 1로 설정
    output.normal = float4(0.f, 0.f, -1.f, 0.f); // 노말 방향 수정
    output.color = color;
    
    return output;
}

#endif