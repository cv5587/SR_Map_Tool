#include "stdafx.h"
#include "..\Header\PlayerUICamera.h"

#include "Export_Utility.h"


CPlayerUICamera::CPlayerUICamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUICamera(pGraphicDev), m_pBufferCom(nullptr),m_pTextureCom(nullptr),m_pTransformCom(nullptr)
{
}

CPlayerUICamera::~CPlayerUICamera()
{
}

HRESULT CPlayerUICamera::Ready_GameObject(const _float& fNear, const _float& fFar, const _uint& uWINCX, const _uint& uWINCY)
{
	Engine::CUICamera::Ready_UICamera(fNear, fFar, uWINCX, uWINCY);

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(0.f, -0.5f, 0.f);

	return S_OK;
}

_int CPlayerUICamera::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_UI, this);
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CPlayerUICamera::LateUpdate_GameObject()
{
	Engine::CUICamera::LateUpdate_GameObject();
}

void CPlayerUICamera::Render_GameObject()
{
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x80);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pTextureCom->Set_Texture(0);

	//m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT CPlayerUICamera::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlayerTexture"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Proto_PlayerTexture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

void CPlayerUICamera::Mouse_Move()
{
}

CPlayerUICamera* CPlayerUICamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fNear, const _float& fFar, const _uint& uWINCX, const _uint& uWINCY)
{
	CPlayerUICamera* pInstance = new CPlayerUICamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(fNear, fFar,uWINCX,uWINCY)))
	{
		Safe_Release(pInstance);
		MSG_BOX("GameUICamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPlayerUICamera::Free()
{
	Engine::CUICamera::Free();
}
