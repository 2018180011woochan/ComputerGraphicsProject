#pragma once
#include "CGameObject.h"

class FixMonster : public CGameObject
{
public:
	FixMonster() {};
	FixMonster(int _idx) { MonsterNum = _idx; }
	~FixMonster() {};

public:
	virtual void Initialize();
	virtual void Update();
	virtual void Update(float _targetx, float _targetz);
	virtual void Render();
	virtual void Release();

private:
	int MonsterNum;
	
};

