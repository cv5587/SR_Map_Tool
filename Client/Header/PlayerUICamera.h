#pragma once

#include "UICamera.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
END

class CPlayerUICamera : public Engine::CUICamera
{
private:
	explicit CPlayerUICamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerUICamera();

public:
	HRESULT Ready_GameObject(
		const _float& fNear,
		const _float& fFar,
		const _uint& uWINCX,
		const _uint& uWINCY);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject();

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

private:
	void		Mouse_Move();

private:
	_bool		m_bCheck = false;

public:
	static CPlayerUICamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _float& fNear,
		const _float& fFar,
		const _uint& uWINCX,
		const _uint& uWINCY
	);

private:
	virtual void Free();
};


