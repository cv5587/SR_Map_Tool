#include "stdafx.h"
#include "InteractiveProp.h"

CInteractiveProp::CInteractiveProp(LPDIRECT3DDEVICE9 pGraphicDev)
	: CProp(pGraphicDev)
{
}

CInteractiveProp::CInteractiveProp(const CInteractiveProp& rhs)
	: CProp(rhs)
{
}

CInteractiveProp::~CInteractiveProp()
{
}

HRESULT CInteractiveProp::Ready_GameObject()
{
	return E_NOTIMPL;
}

_int CInteractiveProp::Update_GameObject(const _float& fTimeDelta)
{
	return _int();
}

void CInteractiveProp::LateUpdate_GameObject()
{
}

void CInteractiveProp::Render_GameObject()
{
}

CInteractiveProp* CInteractiveProp::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CInteractiveProp::Free()
{
	CProp::Free();
}

HRESULT CInteractiveProp::Add_Component()
{
	return E_NOTIMPL;
}

void CInteractiveProp::TexNamer()
{
}
