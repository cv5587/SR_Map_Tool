#pragma once
#include "Thorn.h"

class CFire :
	public CThorn
{
#pragma once

private:
	explicit CFire(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFire(const CFire& rhs);
	virtual ~CFire();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;


public:
	static CFire* Create(LPDIRECT3DDEVICE9	pGraphicDev);

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
	//TODO : ��Ʈ ������ ����



};

