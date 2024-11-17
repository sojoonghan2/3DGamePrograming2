#include "pch.h"
#include "BillboardManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"

void BillboardManager::Init(uint32 maxCount)
{
    // 모든 InstanceID에 대해 BillboardBuffer 초기화
    for (auto& pair : _buffers)
    {
        pair.second = std::make_shared<BillboardBuffer>();
        pair.second->Init(maxCount);
    }
}

void BillboardManager::Render(std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
    std::map<uint64, std::vector<std::shared_ptr<GameObject>>> cache;

    // GameObject를 InstanceID별로 그룹화
    for (const std::shared_ptr<GameObject>& gameObject : gameObjects)
    {
        const uint64 instanceId = gameObject->GetMeshRenderer()->GetInstanceID();
        cache[instanceId].push_back(gameObject);
    }

    // 그룹별 렌더링 처리
    for (auto& pair : cache)
    {
        const std::vector<std::shared_ptr<GameObject>>& vec = pair.second;

        // 단일 객체는 일반 렌더링
        if (vec.size() == 1)
        {
            vec[0]->GetMeshRenderer()->Render();
        }
        else
        {
            // 다중 객체는 인스턴싱 렌더링
            const uint64 instanceId = pair.first;

            for (const std::shared_ptr<GameObject>& gameObject : vec)
            {
                BillboardParams params;
                params.position = gameObject->GetTransform()->GetWorldPosition();
                params.scale = gameObject->GetTransform()->GetLocalScale().x; // 균일 스케일 가정
                params.color = Vec4(1.0f, 1.0f, 1.0f, 1.0f); // 기본 흰색

                AddParam(instanceId, params);
            }

            std::shared_ptr<BillboardBuffer>& buffer = _buffers[instanceId];
            buffer->PushData(); // GPU로 데이터 전송

            vec[0]->GetMeshRenderer()->Render(buffer); // 인스턴싱 렌더링
        }
    }
}

void BillboardManager::ClearBuffer()
{
    for (auto& pair : _buffers)
    {
        std::shared_ptr<BillboardBuffer>& buffer = pair.second;
        buffer->clear();
    }
}

void BillboardManager::AddParam(uint64 instanceId, BillboardParams& data)
{
    if (_buffers.find(instanceId) == _buffers.end())
    {
        _buffers[instanceId] = std::make_shared<BillboardBuffer>();
        _buffers[instanceId]->Init(100); // 초기 크기 설정
    }

    _buffers[instanceId]->AddData(data);
}
