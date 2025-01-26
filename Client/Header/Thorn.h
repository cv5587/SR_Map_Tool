#pragma once

#include "Base.h"
#include "BreakableProp.h"

class CThorn : public CBreakableProp
{
protected:
	explicit CThorn(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CThorn(const CThorn& rhs);
	virtual ~CThorn();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;


public:
	static CThorn* Create(LPDIRECT3DDEVICE9	pGraphicDev);

protected:
	virtual void Free() override;
	virtual HRESULT			Add_Component();
	virtual void TexNamer() override;

protected:
	void Detect_Player(const _float& fTimeDelta); //�÷��̾� �Ÿ� ����ؼ� Spike On/Off �Ǵ�, ���� �浹ó���� ��ü

protected:
	bool m_bSpike;
	float m_fAtkTimer;
	CPlayer* m_pPlayer;

	bool m_bInit;
	//TODO : ��Ʈ ������ ����
};

