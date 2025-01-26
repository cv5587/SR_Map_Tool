#pragma once
#include "Weapon.h"
class CShotgun : public CWeapon
{
private:
	explicit CShotgun(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShotgun(const CShotgun& rhs);
	virtual ~CShotgun();

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		LateUpdate_GameObject();
	virtual void		Render_GameObject();

private:
	HRESULT			Add_Component();
	_bool		m_bReload;

public:
	static CShotgun* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;
};


