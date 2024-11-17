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

    // GPU ���� ����
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
    _data.clear(); // ���� ������ ����
}

void BillboardBuffer::AddData(const BillboardParams& params)
{
    _data.push_back(params); // ������ ������ �߰�
}

void BillboardBuffer::PushData()
{
    const uint32 dataCount = GetCount();
    if (dataCount > _maxCount)
    {
        Init(dataCount); // ������ ������ �ʰ��ϸ� GPU ���� ���Ҵ�
    }

    const uint32 bufferSize = dataCount * sizeof(BillboardParams);

    void* dataBuffer = nullptr;
    D3D12_RANGE readRange{ 0, 0 }; // �б� ������ 0���� ���� (���⸸ ����)
    _buffer->Map(0, &readRange, &dataBuffer); // GPU �޸� ����
    memcpy(dataBuffer, &_data[0], bufferSize); // CPU �����͸� GPU�� ����
    _buffer->Unmap(0, nullptr); // ���� ����

    _bufferView.BufferLocation = _buffer->GetGPUVirtualAddress();
    _bufferView.StrideInBytes = sizeof(BillboardParams);
    _bufferView.SizeInBytes = bufferSize;
}

D3D12_VERTEX_BUFFER_VIEW BillboardBuffer::GetBufferView() const
{
    return _bufferView; // ���� �� ��ȯ
}