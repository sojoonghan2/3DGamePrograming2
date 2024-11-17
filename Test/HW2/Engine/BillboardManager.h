#pragma once

#include "BillboardBuffer.h"

class GameObject;

class BillboardManager
{
    DECLARE_SINGLE(BillboardManager);

public:
    // 렌더링 함수
    void Render(std::vector<std::shared_ptr<GameObject>>& gameObjects);

    // 버퍼 초기화
    void ClearBuffer();
    void Clear() { _buffers.clear(); }


    void Init(uint32 maxCount);

public:
    // 빌보드 데이터를 추가
    void AddParam(uint64 instanceId, BillboardParams& data);

private:
    std::map<uint64 /*instanceId*/, std::shared_ptr<BillboardBuffer>> _buffers; // 인스턴싱 버퍼 맵
};
