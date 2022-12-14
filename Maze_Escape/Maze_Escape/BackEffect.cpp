#include "stdafx.h"
#include "BackEffect.h"

void BackEffect::Initialize()
{
}

void BackEffect::Update()
{
	if (_yPos > 10.f)
		_yPos = _startY;

	_xPos += (R_x / 5);
	_zPos += R_z / 5;
	_yPos += 0.1f;
}

void BackEffect::Update(float _targetx, float _targety, float _targetz)
{
	
}

void BackEffect::Render()
{
}

void BackEffect::Release()
{
}
