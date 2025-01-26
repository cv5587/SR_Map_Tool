#include "stdafx.h"
#include "..\Header\Exit.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "Door.h"

CExit::CExit(LPDIRECT3DDEVICE9 pGraphicDev)
	: CInteractiveProp(pGraphicDev), m_bWarp(false)
{
}

CExit::CExit(const CExit& rhs)
	: CInteractiveProp(rhs)
{
}

CExit::~CExit()
{
}

HRESULT CExit::Ready_GameObject()
{
	TexNamer();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(3.f, 1.f, 3.f);

	//m_ePortalID = m_ePortalID;
	m_ePortalID = PORTAL_B; //test
	return S_OK;
}

Engine::_int CExit::Update_GameObject(const _float& fTimeDelta)
{

	m_fFrame += 20.f * fTimeDelta;

	if (20.f < m_fFrame)
		m_fFrame = 0.f;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);


	return 0;
}

void CExit::LateUpdate_GameObject()
{

	Detect_Player();

	if (m_bWarp)
	{
		//Warp_Player();
	}
	__super::LateUpdate_GameObject();

}

void CExit::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame / 5);

	m_pBufferCom->Render_Buffer();

}

HRESULT CExit::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ExitTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_ExitTexture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CExit::TexNamer()
{
}


void CExit::Detect_Player()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));

	//_vec3	vPlayerPos = pPlayer->Get_Player_Eye();
	_vec3	vPlayerPos = { 0,0,0 };

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (3.f > D3DXVec3Length(&(vPlayerPos - vPos))) //test
	{
		m_bWarp = true;
	}
	else
	{
		m_bWarp = false;
	}
}

void CExit::Scene_Change()
{

}

CExit* CExit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CExit* pInstance = new CExit(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Exit Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CExit::Free()
{
	__super::Free();
}
