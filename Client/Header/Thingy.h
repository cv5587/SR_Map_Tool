#pragma once
#include "Monster.h"

class CThingy :
	public CMonster
{
public:
	explicit CThingy(const CThingy& rhs);

private:
	explicit CThingy(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CThingy();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()	override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();

public:
	static CThingy* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;

	virtual void	Motion_Change() override; //���� ���ϸ� �ؽ�ó �ٲ��ִ� �Լ�
	void	Thingy_Take_Dmg(const _float& fTimeDelta);
private:
	enum THINGYSTATE {
		THINGY_IDLE,
		THINGY_MORPH,
		THINGY_RUN,
		THINGY_ATK1,
		THINGY_ATK2,
		THINGY_MAGIC,
		THINGY_HIT,
		THINGY_DEAD,
		THINGY_END
	};

	THINGYSTATE		m_eThingyState;
	THINGYSTATE		m_ePreThingyState;

	_int				m_iBulletCount;

	//�߰� �ؽ�ó ������Ʈ

	Engine::CTexture* m_pTexCom_Morph;
	Engine::CTexture* m_pTexCom_Atk2;
	Engine::CTexture* m_pTexCom_Magic;

};

