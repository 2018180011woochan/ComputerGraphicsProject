#include "stdafx.h"
#include "Effect.h"
#include <iostream>
#include <random>

using namespace std;



void Effect::Initialize()
{
}

void Effect::Update()
{
}

void Effect::Update(float _targetx, float _targety, float _targetz)
{
	if (lifetime < 0) {
		lifetime = 100;
		_xPos = 99.f;
		_yPos = 99.f;
		_zPos = 99.f;
		isDead = false;
	}

	_xPos += R_x;
	_yPos += R_y;
	_zPos += R_z;

	lifetime -= 1;
}

void Effect::Render()
{
}

void Effect::Release()
{
}
