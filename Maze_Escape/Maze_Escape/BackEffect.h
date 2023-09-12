#pragma once
#include "CGameObject.h"
class BackEffect : public CGameObject
{
public:
	BackEffect() {};
	~BackEffect() {};

public:
	virtual void Initialize();
	virtual void Update();
	virtual void Update(float _targetx, float _targety, float _targetz);
	virtual void Render();
	virtual void Release();

private:
	int lifetime = 100;
};

