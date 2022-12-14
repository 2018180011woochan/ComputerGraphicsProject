#include "stdafx.h"
#include "BossMonster.h"

void BossMonster::Initialize()
{
}

void BossMonster::Update()
{
}

void BossMonster::Update(float _targetx, float _targetz)
{
	if (HP <= 0.f) {
		_xPos = 999.f;
		_zPos = 999.f;
		isDead = true;
	}
	else
		isDead = false;
}

void BossMonster::Render()
{
}

void BossMonster::Release()
{
}
