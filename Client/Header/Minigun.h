#pragma once
#include "Weapon.h"

class CMinigun : public CWeapon
{
private:
	explicit CMinigun(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMinigun(const CMinigun& rhs);
	virtual ~CMinigun();

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		LateUpdate_GameObject();
	virtual void		Render_GameObject();
	virtual bool CanAttack() { return m_bCanAttack; }

private:
	HRESULT			Add_Component();
	_bool	m_bCanAttack;

public:
	static CMinigun* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;
};


