#pragma once

using Microsoft::WRL::ComPtr;

struct BillboardParams
{
    Vec3 position;  // 빌보드 위치
    float scale;    // 빌보드 크기
    Vec4 color;     // 컬러 (선택적으로 사용 가능)
};

class BillboardBuffer
{
public:
    BillboardBuffer();
    ~BillboardBuffer();

    // GPU 버퍼 초기화
    void Init(uint32 maxCount);

    // 빌보드 데이터 추가
    void AddData(const BillboardParams& params);

    // 데이터를 GPU로 전송
    void PushData();

    // 이전 데이터 삭제
    void clear();

    // GPU 버퍼 정보 반환
    D3D12_VERTEX_BUFFER_VIEW GetBufferView() const;

    // 현재 데이터 개수 반환
    uint32 GetCount() const { return static_cast<uint32>(_data.size()); }

private:
    uint32 _maxCount = 0;                                // 최대 빌보드 개수
    ComPtr<ID3D12Resource> _buffer;                      // GPU 버퍼
    D3D12_VERTEX_BUFFER_VIEW _bufferView = {};           // 버퍼 뷰
    std::vector<BillboardParams> _data;                  // 빌보드 데이터
};
