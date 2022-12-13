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
	float dis = abs(_targetx - _xPos) + abs(_targetz - _zPos);

	if (dis < 10.f) {
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
