#include "stdafx.h"
#include "Shotgun.h"

#include "Export_System.h"
#include "Export_Utility.h"

CShotgun::CShotgun(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev), m_bReload(false)
{
}

CShotgun::CShotgun(const CShotgun& rhs)
	:CWeapon(rhs)
{
}

CShotgun::~CShotgun()
{
}

HRESULT CShotgun::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iCurBullet = 20;
	m_iMaxBullet = 20;
	m_iDamage = 10;
	return S_OK;
}

_int CShotgun::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bAttack)
	{
		if (12.f < m_fFrame)
		{
			m_bAttack = false;
			m_bReload = true;
		}
		else
		{
			m_fFrame += 90.f * fTimeDelta;
		}
	}
	else if (m_bReload)
	{
		if (130.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bReload = false;
		}
		else
		{
			m_fFrame += 90.f * fTimeDelta;
		}
	}
	CGameObject::Update_GameObject(fTimeDelta);
	if (!m_bAttack && !m_bReload && Engine::Get_DIMouseState(DIM_LB)) {
		if (m_iCurBullet > 0)
		{
			m_bAttack = true;
			--m_iCurBullet;
		}
	}
	return 0;
}

void CShotgun::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CShotgun::Render_GameObject()
{
	m_pTextureCom->Set_Texture((_int)m_fFrame / 5);
}

HRESULT CShotgun::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Shotgun"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Shotgun", pComponent });

	return S_OK;
}

CShotgun* CShotgun::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShotgun* pInstance = new CShotgun(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Shotgun Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CShotgun::Free()
{
	__super::Free();
}