#pragma once

#include "Component.h"
#include "Base.h"

BEGIN(Engine)


class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9	pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	_float			Get_ViewZ() { return m_fViewZ; }
	CComponent* Get_Component(COMPONENTID eID, wstring pComponentTag);
	void			Compute_ViewZ(const _vec3* pPos);


	D3DXVECTOR3		Get_Pos() { return m_vPos; }
	// 충돌할 물체는 Ready 시, lateUpdate 시에 Set_Pos하기  
	void			Set_Pos(D3DXVECTOR3 vPos) { m_vPos = vPos; }

	void			Set_ParticleIdx(unsigned int uParticleIdx) { m_uParticleIdx = uParticleIdx; }
	unsigned int	Get_ParticleIdx() { return m_uParticleIdx; }

	void			Push_CollisionMgr(COLLIDER_TYPE eType);

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		LateUpdate_GameObject();
	virtual void		Render_GameObject();



protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<const _tchar*, CComponent*>		m_mapComponent[ID_END];
	_float								m_fViewZ;
	D3DXVECTOR3							m_vPos;
	COLLIDER_TYPE						m_eCT_Type;
private:
	unsigned int						m_uParticleIdx;




private:
	CComponent* Find_Component(COMPONENTID eID, wstring pComponentTag);

public:
	virtual void		Free();
};

END