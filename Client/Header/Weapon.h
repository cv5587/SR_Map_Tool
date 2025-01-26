#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CWeapon : public Engine::CGameObject
{
protected:
	explicit CWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWeapon(const CWeapon& rhs);
	virtual ~CWeapon();

public:
	int Get_CurBullet() { return m_iCurBullet; }
	int Get_MaxBullet() { return m_iMaxBullet; }

public:
	_int m_iMaxBullet;
	_int m_iCurBullet;
	_int m_iDamage;
	_bool m_bAttack;
	_float m_fFrame;
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTexture* m_pUITextureCom;

protected:
	virtual void Free() override;
};