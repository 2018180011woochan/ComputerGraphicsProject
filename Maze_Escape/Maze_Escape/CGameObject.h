#pragma once

#ifndef __CGAMEOBJECT_H__
#define __CGAMEOBJECT_H__

class CGameObject
{
public:
	CGameObject() {}
	virtual ~CGameObject() {}

public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;

public:
	void SetPlayerXPos(int _x) { _xPos = _x; };
	void SetPlayerZPos(int _z) { _zPos = _z; };
	float GetPlayerXPos() { return _xPos; }
	float GetPlayerZPos() { return _zPos; }
public:
	float _xPos = 0.f;
	float _yPos = 0.f;
	float _zPos = 0.f;
};

#endif // !__CGAMEOBJECT_H__