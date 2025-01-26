#pragma once

#include "Base.h"
#include "InteractiveProp.h"

class CExit : public CInteractiveProp
{
private:
	explicit CExit(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CExit(const CExit& rhs);
	virtual ~CExit();


public:
	static CExit* Create(LPDIRECT3DDEVICE9	pGraphicDev);

private:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;
	virtual void Free()										 override;

	virtual  HRESULT	Add_Component();
	virtual void TexNamer() override;

private:
	void Detect_Player(); //플레이어 거리 계산해서 Warp On/Off 판단, 추후 충돌처리로 대체
	void Scene_Change(); // 플레이어 위치 이동
private:
	PORTALID m_ePortalID;
	bool m_bWarp; //true되면 위로 올라감
};

