#pragma once
#include "Prop.h"

class CDecorativeProp : public CProp
{
public:
	explicit CDecorativeProp(const CDecorativeProp& rhs);

protected:
	explicit CDecorativeProp(LPDIRECT3DDEVICE9 pGraphicDev, DECOID eDecoID);
	virtual ~CDecorativeProp();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;


public:
	static CDecorativeProp* Create(LPDIRECT3DDEVICE9 pGraphicDev, DECOID eDecoID);

protected:
	virtual void				Free() override;
	virtual  HRESULT			Add_Component() override;
	virtual void				TexNamer() override;

protected:
	bool	m_bDead;
	DECOID	m_eDecoID;
	_float m_fLightFrame;
	_float m_fLightTime;

};

