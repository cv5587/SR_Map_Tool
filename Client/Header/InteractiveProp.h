#pragma once
#include "Prop.h"
#include "Player.h"

class CInteractiveProp : public CProp
{
protected:
	explicit CInteractiveProp(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CInteractiveProp(const CInteractiveProp& rhs);
	virtual ~CInteractiveProp();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

public:
	static CInteractiveProp* Create(LPDIRECT3DDEVICE9	pGraphicDev);
protected:
	virtual void				Free() override;
	virtual  HRESULT			Add_Component() override;
	virtual void				TexNamer() override;

protected:
	float m_fHp;

};

