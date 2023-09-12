#include "stdafx.h"
#include "JumpingMonster.h"
#include <math.h>
#include <iostream>

void JumpingMonster::Initialize()
{
}

void JumpingMonster::Update()
{
}

void JumpingMonster::Update(float _targetx, float _targetz)
{
	if (HP <= 0.f) {
		_xPos = 99.f;
		_zPos = 99.f;
	}

	float dis = abs(_targetx - _xPos) + abs(_targetz - _zPos);

	if (dis < 20.f && dis > 2.f) {
		if (_targetx > _xPos)
			_xPos += objSpeed;
		else
			_xPos -= objSpeed;

		if (_targetz > _zPos)
			_zPos += objSpeed;
		else
			_zPos -= objSpeed;

		// jump
		if (_yPos < 2.f) {
			JumpCheck = false;
			_yPos = 2.f;
		}
		if (_yPos >= 5.5f)
			JumpCheck = true;
		if (!JumpCheck)
			_yPos += 0.02f;
		else
			_yPos -= 0.02f;

	}

	

}

void JumpingMonster::Render()
{
}

void JumpingMonster::Release()
{
}
