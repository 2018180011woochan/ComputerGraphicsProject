#include "stdafx.h"
#include "BossAttack.h"
#include <iostream>
#include <random>
using namespace std;

random_device rdidx;
default_random_engine drr(rdidx());
uniform_real_distribution<> uidd(0, 20);

void BossAttack::Initialize()
{
}

void BossAttack::Update()
{
}

void BossAttack::Update(bool _isClear, bool _isBossDead)
{
	if (!_isClear)
		return;

	if (_isBossDead)
		return;

	if (MonsterNum == RemoveIDX) {
		_zPos = -99.f;
	}
	if (_xPos < -30) {
		RemoveIDX = uidd(drr);
		_xPos = 97;
	}
	_xPos -= 0.5f;
}

void BossAttack::Render()
{
}

void BossAttack::Release()
{
}
