#pragma once
#include "Component.h"

BEGIN(Engine)



class ENGINE_DLL CParticle : public CComponent
{
private:
	explicit CParticle(LPDIRECT3DDEVICE9 pGraphicDev);

public:

	CParticle(const CParticle& rhs);
	virtual ~CParticle();

public:
	virtual HRESULT		Ready_Buffer();
	void		Render_Buffer(_ulong AttribId);

public:
	static CParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual void	Free();

private:
	ID3DXMesh* m_pMesh;
	_ulong		m_dwVtxCnt;
	_ulong		m_dwVtxSize;
	_ulong		m_dwTriCnt;
	_ulong		m_dwFVF;

	
	vector<_vec3> m_pPos;
	vector<INDEX32> m_pIdc;
};

END

