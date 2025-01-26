#include "stdafx.h"
#include "Weapon.h"

CWeapon::CWeapon(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iCurBullet(0), m_iMaxBullet(0), m_pBufferCom(nullptr),m_pTextureCom(nullptr), m_pTransformCom(nullptr)
	, m_iDamage(0), m_bAttack(false), m_fFrame(0.f)
{
}

CWeapon::CWeapon(const CWeapon& rhs)
	: Engine::CGameObject(rhs), m_iCurBullet(0), m_iMaxBullet(0),m_pBufferCom(rhs.m_pBufferCom), m_pTextureCom(rhs.m_pTextureCom), m_pTransformCom(rhs.m_pTransformCom)
	, m_iDamage(0), m_bAttack(false), m_fFrame(0.f)
{
}

CWeapon::~CWeapon()
{
}

void CWeapon::Free()
{
	__super::Free();
}