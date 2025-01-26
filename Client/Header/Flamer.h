#pragma once
#include "Monster.h"


class CFlamer : public CMonster
{
private:
	explicit CFlamer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFlamer(const CFlamer& rhs);
	virtual ~CFlamer();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();

public:
	static CFlamer* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual void Free() override;

	virtual void	Motion_Change() override; //상태 변하면 텍스처 바꿔주는 함수
	void	Flamer_Get_Dmg(const _float& fTimeDelta);
private:

	list<CMonsterBullet*> m_lBullets;


};

