#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CUICamera : public CGameObject
{
protected:
	explicit CUICamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUICamera();

public:
	virtual HRESULT Ready_UICamera(_float fNear, _float fFar,_uint uWINCX, _uint uWINCY);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;

public:

	void Set_World_Space() { m_pGraphicDev->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&m_matWorld)); }
	//Layer의 Renderer에서 호출
	void Set_View_Space() { m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView); }
	void Set_Projection_Space() { m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj); }

protected:
	_float			m_fNear, m_fFar;
	_uint			m_uWINCX, m_uWINCY;
	_matrix			m_matWorld,m_matView, m_matProj;

protected:
	virtual void	Free();
};

END