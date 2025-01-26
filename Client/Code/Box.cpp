#include "stdafx.h"
#include "..\Header\Box.h"

#include "Export_System.h"
#include "Export_Utility.h"

CBox::CBox(LPDIRECT3DDEVICE9 pGraphicDev, BOXID eBoxID)
	: CBreakableProp(pGraphicDev), m_eBoxID(eBoxID)
{
}

CBox::CBox(const CBox& rhs)
	: CBreakableProp(rhs)
{

}

CBox::~CBox()
{
}

HRESULT CBox::Ready_GameObject()
{
	TexNamer();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(2.f, 1.f, 7.f);
	m_pTransformCom->m_vScale = _vec3(0.5f, 0.5f, 0.5f);
	return S_OK;
}

Engine::_int CBox::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);


	//죽는거 test
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));

	_vec3	vPlayerPos = pPlayer->Get_Player_Eye();

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (3 > D3DXVec3Length(&(vPlayerPos - vPos)))
	{
		m_bDead = true;
	}
	else
	{
		m_bDead = false;
	}
	//여까지 test

	if (m_bDead)
	{
		m_fFrame = m_iMaxHp - m_iHp;
		//return OBJ_DEAD;
	}
	else
		return OBJ_NOEVENT;
}

void CBox::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

}

void CBox::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(_uint(floorf(m_fFrame)));

	m_pBufferCom->Render_Buffer();

}

void CBox::TexNamer()
{
	switch (m_eBoxID)
	{
	case BOX_NORMAL:
		m_szTexName = L"Proto_BoxNormalTexture";
		m_iMaxHp = m_iHp = 2;
		break;
	case BOX_EXPL:
		m_szTexName = L"Proto_BoxExplTexture";
		m_iMaxHp = m_iHp = 2;
		break;
	case PILLAR:
		m_szTexName = L"Proto_PillarTexture";
		m_iMaxHp = m_iHp = 6;
	default:
		break;
	}
}

HRESULT CBox::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_szTexName));	
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ m_szTexName.c_str(), pComponent});

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CBox* CBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, BOXID eBoxID)
{
	CBox* pInstance = new CBox(pGraphicDev, eBoxID);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Prop Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBox::Free()
{
	__super::Free();
}
