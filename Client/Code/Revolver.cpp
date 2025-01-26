#include "stdafx.h"
#include "Revolver.h"

#include "Export_System.h"
#include "Export_Utility.h"

CRevolver::CRevolver(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
}

CRevolver::CRevolver(const CRevolver& rhs)
	:CWeapon(rhs)
{
}

CRevolver::~CRevolver()
{
}

HRESULT CRevolver::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iCurBullet = 30;
	m_iMaxBullet = 30;
	m_iDamage = 5;
	return S_OK;
}

_int CRevolver::Update_GameObject(const _float& fTimeDelta)
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

void CRevolver::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CRevolver::Render_GameObject()
{
	m_pTextureCom->Set_Texture((_int)m_fFrame / 5);
}

HRESULT CRevolver::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Revolver"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Revolver", pComponent });

	return S_OK;
}

CRevolver* CRevolver::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRevolver* pInstance = new CRevolver(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Revolver Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CRevolver::Free()
{
	__super::Free();
}