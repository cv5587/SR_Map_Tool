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

	_ulong _vbSize;//버퍼가 보관할수 있는 파티클 수
	_ulong _vbOffset;//버퍼를 총 4단계 쓴다 그중 버퍼의 단계을 표시
	_ulong _vbBatchSize;//한 버퍼당 파티클수

	std::list<Attribute> m_listParticles;//파티클 속성리스트

	_vec3 m_vecOrigin;//파티클 생성 점
	_float m_fSize;//파티클 크기
	_float m_fEmitRate;//초당 파티클수


	_int m_iMaxParticles;//최대 파티클수

public:
	static CParticleBuffer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone( );

private:
	virtual void	Free();
};

END

