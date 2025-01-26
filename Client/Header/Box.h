#pragma once

#include "Base.h"
#include "BreakableProp.h"

class CBox : public CBreakableProp
{
private:
	explicit CBox(LPDIRECT3DDEVICE9 pGraphicDev, BOXID eBoxID);
	explicit CBox(const CBox& rhs);
	virtual ~CBox();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;


public:
	static CBox* Create(LPDIRECT3DDEVICE9	pGraphicDev, BOXID eBoxID);

protected:
	virtual void Free() override;
	virtual HRESULT			Add_Component();
	virtual void TexNamer() override;

private:
	BOXID m_eBoxID;
};

