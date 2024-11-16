#ifndef _BILLBOARD_FX_
#define _BILLBOARD_FX_
#include "params.fx"

// VS_IN 구조체는 동일하게 사용
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

// 정점 셰이더에서 기하 셰이더로 전달할 데이터
struct VS_OUT
{
    float4 posW : POSITION; // 월드 공간의 위치
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    
    if (g_int_0 == 1)
    {
        output.posW = mul(float4(input.pos, 1.f), input.matWorld); // 월드 변환만 수행
    }
    else
    {
        output.posW = mul(float4(input.pos, 1.f), g_matWorld);
    }
    output.uv = input.uv;
    
    return output;
}

// 기하 셰이더에서 픽셀 셰이더로 전달할 데이터
struct GS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 viewPos : POSITION;
};

// 기하 셰이더
[maxvertexcount(4)]
void GS_Main(point VS_OUT input[1], inout TriangleStream<GS_OUT> outputStream)
{
    // 카메라를 향하는 빌보드 방향 계산
    float3 up = float3(0.f, 1.f, 0.f);
    float3 look = normalize(g_vec4_0.xyz); // 카메라 방향
    float3 right = normalize(cross(up, look));
    
    float halfWidth = g_float_0;
    float halfHeight = g_float_1;
    
    // 4개의 정점 생성
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
        // WVP 변환을 한번에 적용
        output.pos = mul(vertices[i], g_matWVP);
        // 뷰 공간의 위치도 계산
        output.viewPos = mul(vertices[i], g_matWV).xyz;
        output.uv = uv[i];
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
        
    output.position = float4(input.viewPos.xyz, 0.f);
    output.normal = float4(0.f, 0.f, 1.f, 0.f);
    output.color = color;
    
    return output;
}

#endif