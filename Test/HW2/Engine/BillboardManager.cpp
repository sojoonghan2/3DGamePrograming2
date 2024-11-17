#include "pch.h"
#include "BillboardManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"

void BillboardManager::Render(std::vector<std::shared_ptr<GameObject>>& gameObjects)
{
    std::map<uint64, std::vector<std::shared_ptr<GameObject>>> cache;

    // GameObject¸¦ InstanceIDº°·Î ±×·ìÈ­
    for (const std::shared_ptr<GameObject>& gameObject : gameObjects)
    {
        const uint64 instanceId = gameObject->GetMeshRenderer()->GetInstanceID();
        cache[instanceId].push_back(gameObject);
    }

    // ±×·ìº° ·»´õ¸µ Ã³¸®
    for (auto& pair : cache)
    {
        const std::vector<std::shared_ptr<GameObject>>& vec = pair.second;

        // ´ÜÀÏ °´Ã¼´Â ÀÏ¹Ý ·»´õ¸µ
        if (vec.size() == 1)
        {
            vec[0]->GetMeshRenderer()->Render();
        }
        else
        {
            // ´ÙÁß °´Ã¼´Â ÀÎ½ºÅÏ½Ì ·»´õ¸µ
            const uint64 instanceId = pair.first;

            for (const std::shared_ptr<GameObject>& gameObject : vec)
            {
                BillboardParams params;
                params.position = gameObject->GetTransform()->GetWorldPosition();
                params.scale = gameObject->GetTransform()->GetLocalScale().x; // ±ÕÀÏ ½ºÄÉÀÏ °¡Á¤
                params.color = Vec4(1.0f, 1.0f, 1.0f, 1.0f); // ±âº» Èò»ö

                AddParam(instanceId, params);
            }

            std::shared_ptr<BillboardBuffer>& buffer = _buffers[instanceId];
            buffer->PushData(); // GPU·Î µ¥ÀÌÅÍ Àü¼Û

            vec[0]->GetMeshRenderer()->Render(buffer); // ÀÎ½ºÅÏ½Ì ·»´õ¸µ
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
        _buffers[instanceId]->Init(100); // ÃÊ±â Å©±â ¼³Á¤
    }

    _buffers[instanceId]->AddData(data);
}
