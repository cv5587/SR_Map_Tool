#pragma once

#include "Camera.h"
#include "Engine_Define.h"
#include "Export_Utility.h"
#include "Weapon.h"

class CBullet;

class CPlayer : public Engine::CCamera
{

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer();

public:
	const _vec3& Get_Player_Eye() { return m_vEye; }
	const _vec3& Get_Player_At() { return m_vAt; }

	void Set_Player_Pos(const _vec3& vSrc, const _vec3& vDst) { m_vEye = vSrc; m_vAt = vDst; }
	HRESULT Ready_GameObject(const _vec3* pEye,
		const _vec3* pAngle,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;

public:
	CWeapon* Get_Weapon() { return m_mWeapon[m_eCurWeapon]; }
	int Get_Health() { return m_iCurHealth; }
	int Get_MaxHealth() { return m_iMaxHealth; }
	int Get_Mana() { return m_iCurMana; }
	int Get_MaxMana() { return m_iMaxMana; }
	void		Add_Health(_int _iHealth) {
		m_iCurHealth += _iHealth;
		m_iCurHealth = max(0, m_iCurHealth);
		m_iCurHealth = min(m_iMaxHealth, m_iCurHealth);
	}
	void		Add_Mana(_int _iMana) {
		m_iCurMana += _iMana;
		m_iCurMana = max(0, m_iCurMana);
		m_iCurMana = min(m_iMaxMana, m_iCurMana);
	}
	_bool Get_Key(DOORID eColor) { return m_bKey[(_int)eColor]; }
public:
	void		Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta)
	{
		m_vInfo[INFO_POS] += *pDir * fSpeed * fTimeDelta;
	}

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Change_Weapon();
	void		Mouse_Move();
	void		UpdateMatrix(const _float& fTimeDelta);

private:
	_bool		m_bFix = false;
	_bool		m_bCheck = false;
	void		Mouse_Fix();
private:

	Engine::CCalculator* m_pCalculatorCom;
	_int		m_iMaxHealth;
	_int		m_iCurHealth;
	_int		m_iMaxMana;
	_int		m_iCurMana;
	WEAPON		m_eCurWeapon;
	map<WEAPON, CWeapon*> m_mWeapon;
	D3DXMATRIX		m_matWorld;
	D3DXVECTOR3		m_vInfo[INFO_END];
	D3DXVECTOR3		m_vAngle;
	_float		m_fSpeed;
	_bool      m_bKey[3];
	list<CBullet*> m_lBullet;
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAngle,
		const _float& fFov,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

private:
	virtual void Free();
};