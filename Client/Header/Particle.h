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
	Engine::CParticleBuffer* m_pBufferCom;//���ؽ� �İ�ü
	Engine::CTexture* m_pTextureCom;//�ؽ�ó �İ�ü

	_float m_fSize;//��ƼŬ ũ��
	_vec3 m_vecOrigin;//��ƼŬ ���� ��

protected:

public:
	static CParticle* Create(LPDIRECT3DDEVICE9	pGraphicDev, _vec3 _vecOrigin, _float _fSize);

protected:
	virtual void Free() override;

};

