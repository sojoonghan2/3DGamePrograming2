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
     // �ʱ�ȭ �Լ�: �ִ� ������ ���� ����
    void Init(uint32 maxCount);

    // ������ ������
    void Render(const std::vector<std::shared_ptr<GameObject>>& gameObjects);

    // ���� �ʱ�ȭ
    void ClearBuffer();
    void Clear();

private:
    // ������ ������ �߰�
    void AddParam(uint64 instanceId, const BillboardParams& params);

private:
    std::map<uint64, std::shared_ptr<BillboardBuffer>> _buffers; // �� �ν��Ͻ��� ������ ���� ����
};
