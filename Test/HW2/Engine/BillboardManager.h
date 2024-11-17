#pragma once

#include "BillboardBuffer.h"
#include <map>
#include <memory>
#include <vector>

class GameObject;

class BillboardManager
{
    DECLARE_SINGLE(BillboardManager);

public:
     // 초기화 함수: 최대 빌보드 개수 설정
    void Init(uint32 maxCount);

    // 빌보드 렌더링
    void Render(const std::vector<std::shared_ptr<GameObject>>& gameObjects);

    // 버퍼 초기화
    void ClearBuffer();
    void Clear();

private:
    // 빌보드 데이터 추가
    void AddParam(uint64 instanceId, const BillboardParams& params);

private:
    std::map<uint64, std::shared_ptr<BillboardBuffer>> _buffers; // 각 인스턴스별 빌보드 버퍼 관리
};
