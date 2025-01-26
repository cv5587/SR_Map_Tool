#pragma once
#include "Weapon.h"
class CRevolver : public CWeapon
{
private:
	explicit CRevolver(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRevolver(const CRevolver& rhs);
	virtual ~CRevolver();

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		LateUpdate_GameObject();
	virtual void		Render_GameObject();

private:
	HRESULT			Add_Component();

public:
	static CRevolver* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;
};


