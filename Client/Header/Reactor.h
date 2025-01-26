#pragma once
#include "Weapon.h"
class CReactor : public CWeapon
{
private:
	explicit CReactor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CReactor(const CReactor& rhs);
	virtual ~CReactor();

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		LateUpdate_GameObject();
	virtual void		Render_GameObject();

private:
	HRESULT			Add_Component();
	_bool		m_bShoot;

public:
	static CReactor* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;
};


