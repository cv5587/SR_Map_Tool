#pragma once
#include "Base.h"
#include "InteractiveProp.h"

class CToggle : public CInteractiveProp
{

private:
	explicit CToggle(LPDIRECT3DDEVICE9 pGraphicDev, TOGGLEID eToggleID);
	explicit CToggle(const CToggle& rhs);
	virtual ~CToggle();


public:
	static CToggle* Create(LPDIRECT3DDEVICE9	pGraphicDev, TOGGLEID eTogID);
	void	Set_ToggleOn() { m_bToggleOn = true; }
	void Get_Rot(float* p) { p= &m_RotY; }
private:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;
	virtual void Free()										 override;

	virtual  HRESULT	Add_Component()						override;
	virtual void		TexNamer()							override;
private:
	void Detect_Player();

private:
	bool	m_bToggleOn;
	TOGGLEID m_eToggleID;
	bool m_bKeys[DOOR_END];
	float m_RotY;

};

//������ ������ ���� : 
// 1. �÷��̾ ���� ���� ����� ������(m_bUnlock == true)
// 2. �÷��̾ Ű �Է� �ݰ� �ȿ� �ִ�
// 3. �÷��̾ ��ȣ�ۿ� Ű�� ������

// ��� ������ ���� :
// 1. �÷��̾ ���� ���� ���踦 ������ �ִ�
// 2. �÷��̾ Ű �Է� �ݰ� �ȿ� �ִ�
// 3. �÷��̾ ��ȣ�ۿ� Ű�� ������

// ����� ������ ��� �Ǵ°�
// 1. ����� �ؽ�ó�� �ٲ�
// 2. ��� ID�� ���� ���� ���� ������ m_bUnlock = true
// 
//���ٿͼ��Ұ� : ����ġ ��� . �浹ó���� ��ó�� �ִ� ���� �� �ø�����
//Ű �԰� ����ġ �Ѿ� �� �� �־���
//TODO : �ִϸ��̼� ������