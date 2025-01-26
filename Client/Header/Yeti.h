#pragma once
#include "Monster.h"


class CYeti : public CMonster
{
private:
	explicit CYeti(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CYeti(const CYeti& rhs);
	virtual ~CYeti();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();

public:
	static CYeti* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;

	virtual void	Motion_Change()							 override;
	void	Yeti_Get_Dmg(const _float& fTimeDelta);
private:

	//Textrue Components
	Engine::CTexture* m_pTexCom_Idle;
	Engine::CTexture* m_pTexCom_Run;
	Engine::CTexture* m_pTexCom_Atk;
	Engine::CTexture* m_pTexCom_Hit;
	Engine::CTexture* m_pTexCom_Dead;

	_float		m_fRunTimer;	//�ٱ� ������ �ð� Ÿ�̸�
	_float		m_fInitSpeed;	//���� �ӵ�
	const _float		m_fMaxSpeed;	//�ְ�ӵ�
	_float		m_fAccel;		//���ӵ�
};

