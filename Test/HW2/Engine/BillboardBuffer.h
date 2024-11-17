#pragma once

using Microsoft::WRL::ComPtr;

struct BillboardParams
{
    Vec3 position;  // ������ ��ġ
    float scale;    // ������ ũ��
    Vec4 color;     // �÷� (���������� ��� ����)
};

class BillboardBuffer
{
public:
    BillboardBuffer();
    ~BillboardBuffer();

    // GPU ���� �ʱ�ȭ
    void Init(uint32 maxCount);

    // ������ ������ �߰�
    void AddData(const BillboardParams& params) { _data.push_back(params); }

    // �����͸� GPU�� ����
    void PushData();

    // ���� ������ ����
    void clear() { _data.clear(); }

    // GPU ���� ���� ��ȯ
    D3D12_VERTEX_BUFFER_VIEW GetBufferView() const { return _bufferView; }

    // ���� ������ ���� ��ȯ
    uint32 GetCount() const { return static_cast<uint32>(_data.size()); }

private:
    // �ִ� ������ ����
    uint32 _maxCount = 0;
    // GPU ����
    ComPtr<ID3D12Resource> _buffer;
    // GPU�� �� �����͸� �����ϱ� ���� ��Ÿ������(��)
    D3D12_VERTEX_BUFFER_VIEW _bufferView = {};
    // ������ ������
    std::vector<BillboardParams> _data;
};
