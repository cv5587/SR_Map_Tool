#include "stdafx.h"
#include "BreakableProp.h"

CBreakableProp::CBreakableProp(LPDIRECT3DDEVICE9 pGraphicDev)
	: CProp(pGraphicDev), m_bDead(false)
{
}

CBreakableProp::CBreakableProp(const CBreakableProp& rhs)
	:	CProp(rhs)
{
}

CBreakableProp::~CBreakableProp()
{
	__super::Free();
}

HRESULT CBreakableProp::Ready_GameObject()
{
	return E_NOTIMPL;
}

_int CBreakableProp::Update_GameObject(const _float& fTimeDelta)
{
	return _int();
}

void CBreakableProp::LateUpdate_GameObject()
{
}

void CBreakableProp::Render_GameObject()
{
}

CBreakableProp* CBreakableProp::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CBreakableProp::Free()
{
}

HRESULT CBreakableProp::Add_Component()
{
	return E_NOTIMPL;
}

void CBreakableProp::TexNamer()
{
}
