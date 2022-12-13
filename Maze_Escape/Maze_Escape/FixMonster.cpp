#include "stdafx.h"
#include "FixMonster.h"
#include <math.h>
#include <iostream>

void FixMonster::Initialize()
{
}

void FixMonster::Update()
{
}

void FixMonster::Update(float _targetx, float _targetz)
{
	if (HP <= 0.f) {
		_xPos = 99.f;
		_zPos = 99.f;
	}

	float dis = abs(_targetx - _xPos) + abs(_targetz - _zPos);

	if (dis < 20.f && dis > 5.f) {
		if (_targetx > _xPos) 
			_xPos += objSpeed;
		else
			_xPos -= objSpeed;

		if (_targetz > _zPos)
			_zPos += objSpeed;
		else
			_zPos -= objSpeed;

	}
}

void FixMonster::Render()
{
}

void FixMonster::Release()
{
}
