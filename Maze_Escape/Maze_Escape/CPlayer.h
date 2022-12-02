#pragma once

#ifndef __CPLAYER_H__
#define __CPLAYER_H__

#include "CGameObject.h"

class CPlayer : public CGameObject
{
public:
	// CGameObject을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override; 
	virtual void Release() override;

};

#endif // !__CPLAYER_H__