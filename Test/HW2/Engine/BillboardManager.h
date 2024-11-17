#pragma once

#include "BillboardBuffer.h"

class GameObject;

class BillboardManager
{

public:
    // ������ �Լ�
    void Render(std::vector<std::shared_ptr<GameObject>>& gameObjects);

    // ���� �ʱ�ȭ
    void ClearBuffer();
    void Clear() { _buffers.clear(); }

public:
    // ������ �����͸� �߰�
    void AddParam(uint64 instanceId, BillboardParams& data);

    static BillboardManager* GetInstance()
    {
        static BillboardManager instance;
        return &instance;
    }

private:
    std::map<uint64 /*instanceId*/, std::shared_ptr<BillboardBuffer>> _buffers; // �ν��Ͻ� ���� ��
};
