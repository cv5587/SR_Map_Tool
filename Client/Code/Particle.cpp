#include "stdafx.h"
#include "Particle.h"

#include"Export_Utility.h"
CParticle::CParticle(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vecOrigin, _float _fSize)
	:Engine::CGameObject(pGraphicDev),m_vecOrigin(_vecOrigin),m_fSize(_fSize)
{
}

CParticle::CParticle(const CParticle& rhs)
	:Engine::CGameObject(rhs)
{
}

CParticle::~CParticle()
{
}

HRESULT CParticle::Ready_GameObject()
{
	return S_OK;
}

_int CParticle::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_PARTICLE, this);

	CGameObject::Update_GameObject(fTimeDelta);	

	return 0;
}

void CParticle::LateUpdate_GameObject()
{
}

void CParticle::Render_GameObject()
{
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer(_ulong(m_fSize));
}

HRESULT CParticle::Add_Component()
{
	return S_OK;
}


CParticle* CParticle::Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vecOrigin, _float _fSize)
{
	CParticle* pInstance = new CParticle(pGraphicDev, _vecOrigin,_fSize);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);	
		MSG_BOX("Particle Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CParticle::Free()
{
}
