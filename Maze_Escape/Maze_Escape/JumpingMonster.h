#pragma once
#include "CGameObject.h"
class JumpingMonster : public CGameObject
{
public:
	JumpingMonster() {};
	JumpingMonster(int _idx) { MonsterNum = _idx; }
	~JumpingMonster() {};

public:
	virtual void Initialize();
	virtual void Update();
	virtual void Update(float _targetx, float _targetz);
	virtual void Render();
	virtual void Release();

private:
	int MonsterNum;
};

