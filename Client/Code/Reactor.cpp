#include "stdafx.h"
#include "Reactor.h"

#include "Export_System.h"
#include "Export_Utility.h"

CReactor::CReactor(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev), m_bShoot(false)
{
}

CReactor::CReactor(const CReactor& rhs)
	:CWeapon(rhs)
{
}

CReactor::~CReactor()
{
}

HRESULT CReactor::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iCurBullet = 20;
	m_iMaxBullet = 20;
	m_iDamage = 10;
	return S_OK;
}

_int CReactor::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bAttack)
	{
		if (180.f < m_fFrame)
		{
			m_bAttack = false;
			m_fFrame = 0.f;
			//m_bShoot = true;
		}
		else
		{
			m_fFrame += 90.f * fTimeDelta;
		}
	}
	/*else if (m_bShoot)
	{
		if (180.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bShoot = false;
		}
		else
		{
			m_fFrame += 90.f * fTimeDelta;
		}
	}*/
	CGameObject::Update_GameObject(fTimeDelta);
	if (!m_bAttack && !m_bShoot && Engine::Get_DIMouseState(DIM_LB)) {
		if (m_iCurBullet > 0)
		{
			m_bAttack = true;
			--m_iCurBullet;
		}
	}
	return 0;
}

void CReactor::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CReactor::Render_GameObject()
{
	m_pTextureCom->Set_Texture((_int)m_fFrame / 5);
}

HRESULT CReactor::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Reactor"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Reactor", pComponent });

	return S_OK;
}

CReactor* CReactor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CReactor* pInstance = DBG_NEW CReactor(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Reactor Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CReactor::Free()
{
	__super::Free();
}