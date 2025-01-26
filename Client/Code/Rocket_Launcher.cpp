#include "stdafx.h"
#include "Rocket_Launcher.h"

#include "Export_System.h"
#include "Export_Utility.h"

CRocket_Launcher::CRocket_Launcher(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
}

CRocket_Launcher::CRocket_Launcher(const CRocket_Launcher& rhs)
	:CWeapon(rhs)
{
}

CRocket_Launcher::~CRocket_Launcher()
{
}

HRESULT CRocket_Launcher::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iCurBullet = 15;
	m_iMaxBullet = 15;
	m_iDamage = 20;
	return S_OK;
}

_int CRocket_Launcher::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bAttack)
	{
		if (30.f < m_fFrame)
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

void CRocket_Launcher::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CRocket_Launcher::Render_GameObject()
{
	m_pTextureCom->Set_Texture((_int)m_fFrame / 5);
}

HRESULT CRocket_Launcher::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Rocket_Launcher"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Rocket_Launcher", pComponent });

	return S_OK;
}

CRocket_Launcher* CRocket_Launcher::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRocket_Launcher* pInstance = new CRocket_Launcher(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Rocket_Launcher Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CRocket_Launcher::Free()
{
	__super::Free();
}