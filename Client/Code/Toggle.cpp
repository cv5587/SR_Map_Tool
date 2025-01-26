#include "stdafx.h"
#include "..\Header\Toggle.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "Player.h"

CToggle::CToggle(LPDIRECT3DDEVICE9 pGraphicDev, TOGGLEID eToggleID)
	: CInteractiveProp(pGraphicDev), m_bToggleOn(false), m_eToggleID(eToggleID),m_RotY(0)
{
	for (int i = 0; i < DOOR_END; ++i)
	{
		m_bKeys[i] = false;
	}
}

CToggle::CToggle(const CToggle& rhs)
	: CInteractiveProp(rhs)
{
}

CToggle::~CToggle()
{
}

HRESULT CToggle::Ready_GameObject()
{
	TexNamer();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//test

	return S_OK;

}

Engine::_int CToggle::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);

	Detect_Player();

	if (m_bToggleOn)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));

		switch (m_eToggleID)
		{
		case TOG_NORMAL:
			//TODO : 구 충돌처리로 주변 문들 Set_Elevate()
			break;
		case TOG_RED:
			//DOOR_RED
		{
			if (!m_bKeys[DOOR_RED]) //플레이어 키 여부 확인
			{
				//플레이어 Set_TogRed
				m_fFrame = 1.f;
			}
		}
			break;
		case TOG_YELLOW:
		{
			if (!m_bKeys[DOOR_RED]) //플레이어 키 여부 확인
			{
				//플레이어 Set_TogYellow
				m_fFrame = 1.f;
			}
		}
			break;
		case TOG_BLUE:
		{
			if (!m_bKeys[DOOR_RED]) //플레이어 키 여부 확인
			{
				//플레이어 Set_TogYellow
				m_fFrame = 1.f;
			}
		}
			break;
		}
	}

	return 0;
}

void CToggle::LateUpdate_GameObject()
{
	
	__super::LateUpdate_GameObject();

}

void CToggle::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture((_uint)m_fFrame);

	m_pBufferCom->Render_Buffer();

}

HRESULT CToggle::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });
	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_szTexName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({m_szTexName.c_str(), pComponent});
	

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CToggle::TexNamer()
{	
	//ToggleID 값 따라 다른 텍스처 출력

	switch (m_eToggleID)
	{
	case TOG_NORMAL:
		m_szTexName = L"Proto_ToggleNormalTexture";
		break;
	case TOG_RED:
		m_szTexName = L"Proto_ToggleRedTexture";
		break;
	case TOG_YELLOW:
		m_szTexName = L"Proto_ToggleYellowTexture";
		break;
	case TOG_BLUE:
		m_szTexName = L"Proto_ToggleBlueTexture";
		break;
	default:
		break;
	}
}


void CToggle::Detect_Player()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));

	_vec3	vPlayerPos = pPlayer->Get_Player_Eye();

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (10.f > D3DXVec3Length(&(vPlayerPos - vPos))) //test
	{
		m_bToggleOn = true;
	}
	else
	{
		m_bToggleOn = false;
	}
}

CToggle* CToggle::Create(LPDIRECT3DDEVICE9 pGraphicDev, TOGGLEID eTogID)
{
	CToggle* pInstance = new CToggle(pGraphicDev, eTogID);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Door Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CToggle::Free()
{
	__super::Free();
}
