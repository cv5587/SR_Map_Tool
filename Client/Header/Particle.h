#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CParticleBuffer;
END



class CParticle :
	public Engine::CGameObject
{
protected:
	explicit CParticle(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vecOrigin, _float _fSize);
	explicit CParticle(const CParticle& rhs);
	virtual ~CParticle();
		
public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

protected:
	HRESULT			Add_Component();


protected:	
	Engine::CParticleBuffer* m_pBufferCom;//버텍스 컴객체
	Engine::CTexture* m_pTextureCom;//텍스처 컴객체

	_float m_fSize;//파티클 크기
	_vec3 m_vecOrigin;//파티클 생성 점

protected:

public:
	static CParticle* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vecOrigin, _float _fSize);

protected:
	virtual void Free() override;

};

