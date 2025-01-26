#pragma once

#include "Base.h"
#include "InteractiveProp.h"
//TODO : �� Ÿ�Ը��� �ؽ�ó �ٲٴ� �� + Ű ���� ���� �޾ƿͼ� �����Ű��
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
	bool Elevate(const _float& fTimeDelta);		// �� ��½�Ű�� �Լ�
	void Detect_Player(); //�÷��̾� �Ÿ� ����ؼ� Elevate On/Off �Ǵ�, ���� �浹ó���� ��ü

private:
	DOORID m_eDoorID;
	float m_fSpeed; // �ö󰡴� �ӵ���
	bool m_bElevate; //true�Ǹ� ���� �ö�
	bool m_bUnlock;
};

