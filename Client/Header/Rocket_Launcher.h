#pragma once
#include "Weapon.h"
class CRocket_Launcher : public CWeapon
{
private:
	explicit CRocket_Launcher(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRocket_Launcher(const CRocket_Launcher& rhs);
	virtual ~CRocket_Launcher();

public:
	virtual HRESULT		Ready_GameObject();
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		LateUpdate_GameObject();
	virtual void		Render_GameObject();

private:
	HRESULT			Add_Component();

public:
	static CRocket_Launcher* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;
};


