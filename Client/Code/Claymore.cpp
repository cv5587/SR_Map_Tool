#include "stdafx.h"
#include "Claymore.h"

#include "Export_System.h"
#include "Export_Utility.h"

CClaymore::CClaymore(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev)
{
}

CClaymore::CClaymore(const CClaymore& rhs)
	:CWeapon(rhs)
{
}

CClaymore::~CClaymore()
{
}

HRESULT CClaymore::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iCurBullet = 0;
	m_iMaxBullet = 0;
	m_iDamage = 10;
	return S_OK;
}

_int CClaymore::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bAttack)
	{
		if (65.f < m_fFrame)
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
		m_bAttack = true;
	}
	return 0;
}

void CClaymore::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CClaymore::Render_GameObject()
{
	//m_pUITextureCom->Set_Texture(0);
	//m_pBufferCom->Render_Buffer();

	m_pTextureCom->Set_Texture((_int)m_fFrame / 5);
}

HRESULT CClaymore::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Claymore"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Claymore", pComponent });

	pComponent = m_pUITextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIWeapon"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_UIWeapon", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CClaymore* CClaymore::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CClaymore* pInstance = new CClaymore(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Claymore Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CClaymore::Free()
{
	__super::Free();
}