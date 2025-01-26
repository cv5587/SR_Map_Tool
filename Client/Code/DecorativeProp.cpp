#include "stdafx.h"
#include "..\Header\DecorativeProp.h"

#include "Export_System.h"
#include "Export_Utility.h"

CDecorativeProp::CDecorativeProp(LPDIRECT3DDEVICE9 pGraphicDev, DECOID eDecoID)
	: CProp(pGraphicDev), m_eDecoID(eDecoID)
{
}

CDecorativeProp::CDecorativeProp(const CDecorativeProp& rhs)
	: CProp(rhs), m_eDecoID(rhs.m_eDecoID)
{
	Ready_GameObject();
}

CDecorativeProp::~CDecorativeProp()
{
}

HRESULT CDecorativeProp::Ready_GameObject()
{
	//m_eDecoID = DECO_LAMP;
	TexNamer();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(-3.0f, 1.8f, 0.5f);

	return S_OK;
}

Engine::_int CDecorativeProp::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);

	return OBJ_NOEVENT;
}	

void CDecorativeProp::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

}

void CDecorativeProp::Render_GameObject()
{


	_matrix   matWorld, matView, matBill;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//if (m_eDecoID == DECO_LAMP)
	//{
	//	Engine::Enable_Light(m_pGraphicDev, LIGHT_LAMP, TRUE, m_pTransformCom->Get_Pos());
	//}
	//else if (m_eDecoID == DECO_SIREN)
	//{
	//	D3DLIGHT9 light;
	//	m_pGraphicDev->GetLight(LIGHT_SPOT, &light);
	//	m_fLightTime += Get_TimeDelta(L"Timer_FPS60") * 5.f;
	//	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction,
	//		&_vec3(cosf(m_fLightTime),
	//			-0.2f,
	//			sinf(m_fLightTime)));
	//	m_pGraphicDev->SetLight(LIGHT_SPOT, &light);
	//	Engine::Enable_Light(m_pGraphicDev, LIGHT_SPOT, TRUE, m_pTransformCom->Get_Pos());
	//	Engine::Enable_Material(m_pGraphicDev, MTRL_SIREN);
	//}

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	//Engine::Enable_Material(m_pGraphicDev);

	//if (m_eDecoID == DECO_LAMP)
	//{
	//	// 모든 전등 동시에 깜박임
	//	//if (m_fLightFrame >= 190.f)
	//	//{
	//	//   m_fLightFrame = 0.f;
	//	//}
	//	//else if (m_fLightFrame >= 180.f)
	//	//{
	//	//   Engine::Enable_Light(m_pGraphicDev, LIGHT_LAMP, FALSE);
	//	//}
	//}

}


void CDecorativeProp::TexNamer()
{
	switch (m_eDecoID)
	{
	case DECO_CORPSE:
		m_szTexName = L"Proto_CorpseTexture";
		break;
	case DECO_LAMP:
		m_szTexName = L"Proto_LampTexture";
		break;
	case DECO_LIGHTPOST:
		m_szTexName = L"Proto_LightpostTexture";
		break;
	case DECO_SIREN:
		m_szTexName = L"Proto_LampTexture";
		break;
	default:
		break;
	}
}

HRESULT CDecorativeProp::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_szTexName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ m_szTexName.c_str(), pComponent });

	pComponent  = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CDecorativeProp* CDecorativeProp::Create(LPDIRECT3DDEVICE9 pGraphicDev, DECOID eDecoID)
{
	CDecorativeProp* pInstance = new CDecorativeProp(pGraphicDev, eDecoID);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Prop Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CDecorativeProp::Free()
{
	__super::Free();
}
