#pragma once
#include "CGameObject.h"
class BossMonster : public CGameObject
{
public:
	BossMonster() {};
	~BossMonster() {};

public:
	virtual void Initialize();
	virtual void Update();
	virtual void Update(float _targetx, float _targetz);
	virtual void Render();
	virtual void Release();
};

