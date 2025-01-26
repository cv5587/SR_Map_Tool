#include "stdafx.h"
#include "..\Header\Portal.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "Door.h"
CPortal::CPortal(LPDIRECT3DDEVICE9 pGraphicDev, PORTALID ePortalID)
	: CInteractiveProp(pGraphicDev), m_bWarp(false), m_ePortalID(ePortalID)
{
}

CPortal::CPortal(const CPortal& rhs)
	: CInteractiveProp(rhs)
{
}

CPortal::~CPortal()
{
}

HRESULT CPortal::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(7.f, 10.f, 7.f);

	m_ePortalID = PORTAL_A; //test
	return S_OK;
}

Engine::_int CPortal::Update_GameObject(const _float& fTimeDelta)
{

	m_fFrame += 40.f * fTimeDelta;

	if (40.f < m_fFrame)
		m_fFrame = 0.f;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);

	return OBJ_NOEVENT;


}

void CPortal::LateUpdate_GameObject()
{

	Detect_Player();

	if (m_bWarp)
	{
		Warp_Player();
	}
	__super::LateUpdate_GameObject();

}

void CPortal::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame / 5);

	m_pBufferCom->Render_Buffer();

}

HRESULT CPortal::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PortalTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_PortalTexture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CPortal::TexNamer()
{
}


void CPortal::Detect_Player()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));
	_vec3	vPlayerPos = pPlayer->Get_Player_Eye();

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (3 > D3DXVec3Length(&(vPlayerPos - vPos))) //test
	{
		m_bWarp = true;
	}
	else
	{
		m_bWarp = false;
	}
}

void CPortal::Warp_Player()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));

	_vec3	 vDstPos;

	switch (m_ePortalID)
	{
	case PORTAL_A:
		vDstPos = { 0.f, 2.5f, 0.f };
		break;
	case PORTAL_B:
		vDstPos = { 10.f, 20.f, 0.f };
		break;
	case PORTAL_C:
		break;
	default:
		break;
	}

	//eye, at ฟ๖วม
	pPlayer->Set_Player_Pos(vDstPos, vDstPos + pPlayer->Get_Player_At() - pPlayer->Get_Player_Eye());
}

CPortal* CPortal::Create(LPDIRECT3DDEVICE9 pGraphicDev, PORTALID ePortalID)
{
	CPortal* pInstance = new CPortal(pGraphicDev, ePortalID);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Portal Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPortal::Free()
{
	__super::Free();
}
