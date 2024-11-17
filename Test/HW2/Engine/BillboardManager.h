#pragma once

#include "BillboardBuffer.h"

class GameObject;

class BillboardManager
{

public:
    // 렌더링 함수
    void Render(std::vector<std::shared_ptr<GameObject>>& gameObjects);

    // 버퍼 초기화
    void ClearBuffer();
    void Clear() { _buffers.clear(); }

public:
    // 빌보드 데이터를 추가
    void AddParam(uint64 instanceId, BillboardParams& data);

    static BillboardManager* GetInstance()
    {
        static BillboardManager instance;
        return &instance;
    }

private:
    std::map<uint64 /*instanceId*/, std::shared_ptr<BillboardBuffer>> _buffers; // 인스턴싱 버퍼 맵
};
