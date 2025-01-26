#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CBullet :	public Engine::CGameObject
{
public:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBullet(const CBullet& rhs);
	virtual ~CBullet();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;
	void Set_Pos(_vec3 vPos, _vec3 vDir);

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	_float m_fSpeed = 1.f;
	_vec3 m_vDir;
	_bool m_bFall = false;


public:
	static CBullet*		Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;
};