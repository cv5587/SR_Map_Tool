#include "stdafx.h"
#include "Staff.h"

#include "Export_System.h"
#include "Export_Utility.h"

CStaff::CStaff(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
}

CStaff::CStaff(const CStaff& rhs)
	:CWeapon(rhs)
{
}

CStaff::~CStaff()
{
}

HRESULT CStaff::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iCurBullet = 50;
	m_iMaxBullet = 50;
	m_iDamage = 7;
	return S_OK;
}

_int CStaff::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bAttack)
	{
		if (20.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bAttack = false;
		}
		else
		{
			m_fFrame += 90.f * fTimeDelta;
		}
	}

	CGameObject::Update_GameObject(fTimeDelta);
	if (!m_bAttack && Engine::Get_DIMouseState(DIM_LB)) {
		if (m_iCurBullet > 0)
		{
			m_bAttack = true;
			--m_iCurBullet;
		}
	}
	return 0;
}

void CStaff::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CStaff::Render_GameObject()
{
	m_pTextureCom->Set_Texture((_int)m_fFrame / 5);
}

HRESULT CStaff::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Staff"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Staff", pComponent });

	return S_OK;
}

CStaff* CStaff::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStaff* pInstance = new CStaff(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Staff Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CStaff::Free()
{
	__super::Free();
}