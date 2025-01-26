#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CParticleBuffer : public CComponent
{
private:
	explicit CParticleBuffer(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 _vecOrigin);

public:
	CParticleBuffer(const CParticleBuffer& rhs);
	virtual ~CParticleBuffer();
	virtual _int		Update_Component(const _float& fTimeDelta);

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer(_ulong AttribId);
	virtual void		Reset();
	virtual void		RemoveDeadParticles();
	virtual void		AddParticle();
	virtual void		ResetParticle(Attribute* att);
	_float	GetRandomFloat(_float lowBound, _float highBound);
	void		GetRandomVector(_vec3* out, _vec3* min, _vec3* max);
private:
	LPDIRECT3DVERTEXBUFFER9 m_vb;

	_ulong _vbSize;//���۰� �����Ҽ� �ִ� ��ƼŬ ��
	_ulong _vbOffset;//���۸� �� 4�ܰ� ���� ���� ������ �ܰ��� ǥ��
	_ulong _vbBatchSize;//�� ���۴� ��ƼŬ��

	std::list<Attribute> m_listParticles;//��ƼŬ �Ӽ�����Ʈ

	_vec3 m_vecOrigin;//��ƼŬ ���� ��
	_float m_fSize;//��ƼŬ ũ��
	_float m_fEmitRate;//�ʴ� ��ƼŬ��


	_int m_iMaxParticles;//�ִ� ��ƼŬ��

public:
	static CParticleBuffer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone( );

private:
	virtual void	Free();
};

END

