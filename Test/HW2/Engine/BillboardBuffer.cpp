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
    // �� ���� ó�������� �ִ� ������ ����
    _maxCount = maxCount;

    const uint32 bufferSize = sizeof(BillboardParams) * maxCount;

    // ���ε� ��
    D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

    // ���� ���ҽ� ����
    D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

    // ���� ���ҽ� ����
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

    // ������ ������ �ʰ��ϸ� GPU ���� ���Ҵ�
    if (dataCount > _maxCount) Init(dataCount);

    const uint32 bufferSize = dataCount * sizeof(BillboardParams);

    void* dataBuffer = nullptr;
    // �б� ������ 0���� ���� (���⸸ ����)
    D3D12_RANGE readRange{ 0, 0 };
    // GPU �޸� ����
    _buffer->Map(0, &readRange, &dataBuffer);
    // CPU �����͸� GPU�� ����
    memcpy(dataBuffer, &_data[0], bufferSize); 
    // ���� ����
    _buffer->Unmap(0, nullptr); 

    _bufferView.BufferLocation = _buffer->GetGPUVirtualAddress();
    _bufferView.StrideInBytes = sizeof(BillboardParams);
    _bufferView.SizeInBytes = bufferSize;
}