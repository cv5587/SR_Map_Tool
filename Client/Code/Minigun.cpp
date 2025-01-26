#include "stdafx.h"
#include "Minigun.h"

#include "Export_System.h"
#include "Export_Utility.h"

CMinigun::CMinigun(LPDIRECT3DDEVICE9 pGraphicDev)
	:CWeapon(pGraphicDev), m_bCanAttack(true)
{
}

CMinigun::CMinigun(const CMinigun& rhs)
	:CWeapon(rhs)
{
}

CMinigun::~CMinigun()
{
}

HRESULT CMinigun::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iCurBullet = 670;
	m_iMaxBullet = 670;
	m_iDamage = 1;
	return S_OK;
}

_int CMinigun::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bAttack)
	{
		if (36.f < m_fFrame)
		{
			m_fFrame = 0.f;
			m_bAttack = false;
		}
		else
		{
			m_fFrame += 90.f * fTimeDelta;
			if (!((_int)m_fFrame % 3))
			{
				--m_iCurBullet;
				m_bCanAttack = true;
			}
			else m_bCanAttack = false;
			m_iCurBullet = max(m_iCurBullet, 0);
		}
	}
	CGameObject::Update_GameObject(fTimeDelta);
	if (!m_bAttack && Engine::Get_DIMouseState(DIM_LB)) {
		m_bAttack = true;
	}
	return 0;
}

void CMinigun::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMinigun::Render_GameObject()
{
	m_pTextureCom->Set_Texture((_int)(m_fFrame / 4.5f));
	m_pBufferCom->Render_Buffer();
}

HRESULT CMinigun::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Minigun"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Player_Minigun", pComponent });

	return S_OK;
}

CMinigun* CMinigun::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMinigun* pInstance = new CMinigun(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Minigun Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMinigun::Free()
{
	__super::Free();
}