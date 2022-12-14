#pragma once
#include "CGameObject.h"
class BossAttack : public CGameObject
{
public:
	BossAttack() {};
	BossAttack(int _idx) { MonsterNum = _idx; }
	~BossAttack() {};

public:
	virtual void Initialize();
	virtual void Update();
	virtual void Update(bool _isClear, bool _isBossDead);
	virtual void Render();
	virtual void Release();

public:
	int MonsterNum;
	int RemoveIDX = 0;
};

