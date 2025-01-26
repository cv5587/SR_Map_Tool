#pragma once
#include "Prop.h"
#include	"Player.h"
class CBreakableProp : public CProp
{
protected:
	explicit CBreakableProp(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBreakableProp(const CBreakableProp& rhs);
	virtual ~CBreakableProp();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

public:
	static CBreakableProp* Create(LPDIRECT3DDEVICE9	pGraphicDev);
protected:
	virtual void				Free() override;
	virtual  HRESULT			Add_Component() override;
	virtual void				TexNamer() override;

protected:
	_int	m_iHp;
	_int	m_iMaxHp;
	bool	m_bDead;


};

