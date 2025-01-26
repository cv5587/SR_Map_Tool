#include "UICamera.h"

CUICamera::CUICamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev),m_fNear(0.f),m_fFar(0.f),m_uWINCX(0),m_uWINCY(0)
{
}

CUICamera::~CUICamera()
{
}

HRESULT CUICamera::Ready_UICamera(_float fNear, _float fFar, _uint uWINCX, _uint uWINCY)
{
	m_fNear = fNear;
	m_fFar = fFar;

	m_uWINCX = uWINCX;
	m_uWINCY = uWINCY;


	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixOrthoLH(&m_matProj, m_uWINCX, m_uWINCY, m_fNear, m_fFar);


	return S_OK;
}

_int CUICamera::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CUICamera::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CUICamera::Free()
{
	CGameObject::Free();
}
