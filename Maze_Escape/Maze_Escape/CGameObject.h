#pragma once

#ifndef __CGAMEOBJECT_H__
#define __CGAMEOBJECT_H__

struct AABB {
	float maxX;
	float minX;
	float maxZ;
	float minZ;
	float lengthX;
	float lengthZ;
};

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
	AABB _AABB;
	float objSpeed = 0.f;
	float HP;

};

#endif // !__CGAMEOBJECT_H__