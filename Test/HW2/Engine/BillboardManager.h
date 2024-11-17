#pragma once

#include "BillboardBuffer.h"

class GameObject;

class BillboardManager
{
    DECLARE_SINGLE(BillboardManager);

public:
    // ������ �Լ�
    void Render(std::vector<std::shared_ptr<GameObject>>& gameObjects);

    // ���� �ʱ�ȭ
    void ClearBuffer();
    void Clear() { _buffers.clear(); }


    void Init(uint32 maxCount);

public:
    // ������ �����͸� �߰�
    void AddParam(uint64 instanceId, BillboardParams& data);

private:
    std::map<uint64 /*instanceId*/, std::shared_ptr<BillboardBuffer>> _buffers; // �ν��Ͻ� ���� ��
};
