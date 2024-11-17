#include "pch.h"
#include "BillboardBuffer.h"
#include "Engine.h"

BillboardBuffer::BillboardBuffer()
{
}

BillboardBuffer::~BillboardBuffer()
{
}

void BillboardBuffer::Init(uint32 maxCount)
{
    // 한 번에 처리가능한 최대 빌보드 개수
    _maxCount = maxCount;

    const uint32 bufferSize = sizeof(BillboardParams) * maxCount;

    // 업로드 힙
    D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

    // 버퍼 리소스 생성
    D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

    // 실제 리소스 생성
    DEVICE->CreateCommittedResource(
        &heapProperty,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&_buffer));
}

void BillboardBuffer::PushData()
{
    const uint32 dataCount = GetCount();

    // 데이터 개수가 초과하면 GPU 버퍼 재할당
    if (dataCount > _maxCount) Init(dataCount);

    const uint32 bufferSize = dataCount * sizeof(BillboardParams);

    void* dataBuffer = nullptr;
    // 읽기 범위를 0으로 설정 (쓰기만 수행)
    D3D12_RANGE readRange{ 0, 0 };
    // GPU 메모리 매핑
    _buffer->Map(0, &readRange, &dataBuffer);
    // CPU 데이터를 GPU로 복사
    memcpy(dataBuffer, &_data[0], bufferSize); 
    // 매핑 해제
    _buffer->Unmap(0, nullptr); 

    _bufferView.BufferLocation = _buffer->GetGPUVirtualAddress();
    _bufferView.StrideInBytes = sizeof(BillboardParams);
    _bufferView.SizeInBytes = bufferSize;
}