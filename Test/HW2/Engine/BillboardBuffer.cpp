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
    _maxCount = maxCount;

    const uint32 bufferSize = sizeof(BillboardParams) * maxCount;

    D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

    // GPU 버퍼 생성
    DEVICE->CreateCommittedResource(
        &heapProperty,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&_buffer));
}

void BillboardBuffer::clear()
{
    _data.clear(); // 이전 데이터 삭제
}

void BillboardBuffer::AddData(const BillboardParams& params)
{
    _data.push_back(params); // 빌보드 데이터 추가
}

void BillboardBuffer::PushData()
{
    const uint32 dataCount = GetCount();
    if (dataCount > _maxCount)
    {
        Init(dataCount); // 데이터 개수가 초과하면 GPU 버퍼 재할당
    }

    const uint32 bufferSize = dataCount * sizeof(BillboardParams);

    void* dataBuffer = nullptr;
    D3D12_RANGE readRange{ 0, 0 }; // 읽기 범위를 0으로 설정 (쓰기만 수행)
    _buffer->Map(0, &readRange, &dataBuffer); // GPU 메모리 매핑
    memcpy(dataBuffer, &_data[0], bufferSize); // CPU 데이터를 GPU로 복사
    _buffer->Unmap(0, nullptr); // 매핑 해제

    _bufferView.BufferLocation = _buffer->GetGPUVirtualAddress();
    _bufferView.StrideInBytes = sizeof(BillboardParams);
    _bufferView.SizeInBytes = bufferSize;
}

D3D12_VERTEX_BUFFER_VIEW BillboardBuffer::GetBufferView() const
{
    return _bufferView; // 버퍼 뷰 반환
}