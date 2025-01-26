#pragma once
#include "Weapon.h"
class CStaff : public CWeapon
{
private:
	explicit CStaff(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaff(const CStaff& rhs);
	virtual ~CStaff();

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		LateUpdate_GameObject();
	virtual void		Render_GameObject();

private:
	HRESULT			Add_Component();

public:
	static CStaff* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;
};


