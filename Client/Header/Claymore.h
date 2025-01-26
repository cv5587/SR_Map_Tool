#pragma once
#include "Weapon.h"
class CClaymore : public CWeapon
{
private:
	explicit CClaymore(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CClaymore(const CClaymore& rhs);
	virtual ~CClaymore();

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		LateUpdate_GameObject();
	virtual void		Render_GameObject();

private:
	HRESULT			Add_Component();

public:
	static CClaymore* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;
};


