#pragma once

#include "Base.h"
#include "InteractiveProp.h"
//TODO : 문 타입마다 텍스처 바꾸는 거 + 키 보유 정보 받아와서 언락시키기
class CDoor : public CInteractiveProp
{
private:
	explicit CDoor(LPDIRECT3DDEVICE9 pGraphicDev, DOORID eDoorID);
	explicit CDoor(const CDoor& rhs);
	virtual ~CDoor();


public:
	static CDoor* Create(LPDIRECT3DDEVICE9	pGraphicDev, DOORID eDoorID);
	void	Set_Elevate() { m_bElevate = true; }
	void	Set_Unlock() { m_bUnlock = true; }

private:
		virtual HRESULT Ready_GameObject()						 override;
		virtual _int Update_GameObject(const _float& fTimeDelta) override;
		virtual void LateUpdate_GameObject()					 override;
		virtual void Render_GameObject()						 override;
		virtual void Free()										 override;

		virtual  HRESULT	Add_Component()						override;
		virtual void		TexNamer()							override;

private:
	bool Elevate(const _float& fTimeDelta);		// 문 상승시키는 함수
	void Detect_Player(); //플레이어 거리 계산해서 Elevate On/Off 판단, 추후 충돌처리로 대체

private:
	DOORID m_eDoorID;
	float m_fSpeed; // 올라가는 속도값
	bool m_bElevate; //true되면 위로 올라감
	bool m_bUnlock;
};

