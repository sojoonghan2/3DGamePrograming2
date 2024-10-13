#pragma once
#include "Component.h"

class Material;
class Mesh;
class StructuredBuffer;

struct ParticleInfo
{
	Vec3	worldPos;
	float	curTime;
	Vec3	worldDir;
	float	lifeTime;
	int32	alive;
	int32	padding[3];
};

struct ComputeSharedInfo
{
	int32 addCount;
	int32 padding[3];
};

class ParticleSystem : public Component
{
public:
    ParticleSystem();
    virtual ~ParticleSystem();

public:
    virtual void FinalUpdate();
    void Render();

    void ParticleStart() { _isRunning = true; }
    void ParticleStop() { _isRunning = false; }
    bool IsRunning() const { return _isRunning; }

public:
    virtual void Load(const wstring& path) override { }
    virtual void Save(const wstring& path) override { }

private:
    shared_ptr<StructuredBuffer> _particleBuffer;
    shared_ptr<StructuredBuffer> _computeSharedBuffer;
    uint32 _maxParticle = 1000;

    shared_ptr<Material> _computeMaterial;
    shared_ptr<Material> _material;
    shared_ptr<Mesh> _mesh;

    float _createInterval = 0.005f;
    float _accTime = 0.f;

    float _minLifeTime = 0.5f;
    float _maxLifeTime = 1.f;
    float _minSpeed = 100;
    float _maxSpeed = 50;
    float _startScale = 10.f;
    float _endScale = 5.f;

    bool _isRunning = true;  // 파티클 시스템 활성 상태
};
