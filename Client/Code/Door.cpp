#include "stdafx.h"
#include "..\Header\Door.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"

CDoor::CDoor(LPDIRECT3DDEVICE9 pGraphicDev, DOORID eDoorID)
	: CInteractiveProp(pGraphicDev), m_eDoorID(eDoorID), m_bElevate(false), m_bUnlock(false)
{

}

CDoor::CDoor(const CDoor& rhs)
	: CInteractiveProp(rhs)
{
}

CDoor::~CDoor()
{
}

HRESULT CDoor::Ready_GameObject()
{
	TexNamer();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 1.f;

	switch (m_eDoorID)
	{
	case DOOR_GREY:
		m_bUnlock = true;
		m_pTransformCom->Set_Pos(2.f, 2.1f, 1.f); //test
		Set_Pos(D3DXVECTOR3(12.f, 2.1f, 1.f));
		break;
	case DOOR_RED:
		m_pTransformCom->Set_Pos(2.f, 2.1f, 1.f); //test
		Set_Pos(D3DXVECTOR3(2.f, 2.1f, 1.f));
		break;
	case DOOR_YELLOW:
	case DOOR_BLUE:
	default:
		break;
	}

	Push_CollisionMgr(CT_AABB);

	return S_OK;
}

Engine::_int CDoor::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);

	//Detect_Player();

	if (m_bUnlock && m_bElevate)
	{
		return Elevate(fTimeDelta);
	}

	return 0;
}

void CDoor::LateUpdate_GameObject()
{
	
	__super::LateUpdate_GameObject();

}

void CDoor::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

}

HRESULT CDoor::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_szTexName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ m_szTexName.c_str(), pComponent});

	return S_OK;
}

void CDoor::TexNamer()
{	switch (m_eDoorID)
	{
	case DOOR_RED:
		m_szTexName = L"Proto_DoorRedTexture";
		break;
	case DOOR_YELLOW:
		m_szTexName = L"Proto_DoorYellowTexture";
		break;
	case DOOR_BLUE:
		m_szTexName = L"Proto_DoorBlueTexture";
		break;
	case DOOR_GREY:
		m_szTexName = L"Proto_DoorGreyTexture";
		break;
	default:
		break;
	}
}

bool CDoor::Elevate(const _float& fTimeDelta)
{
	_vec3 yUp = { 0.f, 1.f, 0.f };
	m_pTransformCom->Move_Pos(&yUp, m_fSpeed, fTimeDelta);

	_vec3 pos;
	m_pTransformCom->Get_Info(INFO_POS, &pos);
	
	if (100 > pos.y)
	{
		return OBJ_NOEVENT;
	}
	else
	{
		return OBJ_DEAD;
	}
}

void CDoor::Detect_Player()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));

	//_vec3	vPlayerPos = pPlayer->Get_Player_Eye();
	_vec3	vPlayerPos = { 0,0,0 };

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (5.f > D3DXVec3Length(&(vPlayerPos - vPos))) //test
	{
		m_bElevate = true;
	}
	else
	{
		m_bElevate = false;
	}
}

CDoor* CDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev, DOORID eDoorID)
{
	CDoor* pInstance = new CDoor(pGraphicDev, eDoorID);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Door Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CDoor::Free()
{
	__super::Free();
}
