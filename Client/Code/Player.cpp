#include "stdafx.h"
#include "..\Header\Player.h"
#include "Export_System.h"

#include "Claymore.h"
#include "Minigun.h"
#include "Revolver.h"
#include "Rocket_Launcher.h"
#include "Shotgun.h"
#include "Staff.h"
#include "Reactor.h"

//for monster test
#include "Yeti.h"
#include "Looker.h"
#include "Flamer.h"
#include "Bullet.h"


CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev), m_pCalculatorCom(nullptr), m_iCurHealth(100), m_iMaxHealth(100), m_iCurMana(50), m_iMaxMana(50), m_fSpeed(10.f)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject(const _vec3* pEye,
	const _vec3* pAngle,
	const _float& fFov, const _float& fAspect,
	const _float& fNear, const _float& fFar)
{

	//생성자(create) 바꾸기
	m_vInfo[INFO_POS] = *pEye;
	m_vAngle = *pAngle;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	m_eCurWeapon = REVOLVER;
	m_mWeapon[CLAYMORE] = CClaymore::Create(m_pGraphicDev);
	m_mWeapon[MINIGUN] = CMinigun::Create(m_pGraphicDev);
	m_mWeapon[REVOLVER] = CRevolver::Create(m_pGraphicDev);
	m_mWeapon[ROCKET_LAUNCHER] = CRocket_Launcher::Create(m_pGraphicDev);
	m_mWeapon[SHOTGUN] = CShotgun::Create(m_pGraphicDev);
	m_mWeapon[STAFF] = CStaff::Create(m_pGraphicDev);
	m_mWeapon[REACTOR] = CReactor::Create(m_pGraphicDev);

	Set_Pos(m_vEye);

	Push_CollisionMgr(CT_AABB);

	UpdateMatrix(0.f);

	FAILED_CHECK_RETURN(CCamera::Ready_GameObject(), E_FAIL);

	return S_OK;
}

Engine::_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	for (auto iter : m_lBullet)
	{
		iter->Update_GameObject(fTimeDelta);
	}
	_int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);

	if (false == m_bFix)
	{
		Mouse_Move();
		Mouse_Fix();
	}
	Change_Weapon();
	Key_Input(fTimeDelta);
	m_mWeapon[m_eCurWeapon]->Update_GameObject(fTimeDelta);

	UpdateMatrix(fTimeDelta);

	return iExit;
}

void CPlayer::LateUpdate_GameObject()
{
	Engine::CCamera::LateUpdate_GameObject();
	m_mWeapon[m_eCurWeapon]->LateUpdate_GameObject();
	for (auto iter : m_lBullet)
	{
		iter->LateUpdate_GameObject();
	}
	m_vPos = m_vEye;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	if (m_eCurWeapon == MINIGUN && Get_DIMouseState(DIM_LB))
	{
		if (dynamic_cast<CMinigun*>(m_mWeapon[m_eCurWeapon])->CanAttack())
		{
			m_lBullet.push_back(CBullet::Create(m_pGraphicDev));
			m_lBullet.back()->Ready_GameObject();
			_vec3 vDir = m_vAt - m_vEye;
			D3DXVec3Normalize(&vDir, &vDir);
			m_lBullet.back()->Set_Pos(m_vEye + vDir, vDir);
		}
	}
	_matrix         matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	if (Engine::Get_DIKeyState(DIK_R) & 0x80)
		m_bKey[0] = true;
	if (Engine::Get_DIKeyState(DIK_T) & 0x80)
		m_bKey[0] = false;
	if (Engine::Get_DIKeyState(DIK_F) & 0x80)
		m_bKey[1] = true;
	if (Engine::Get_DIKeyState(DIK_G) & 0x80)
		m_bKey[1] = false;
	if (Engine::Get_DIKeyState(DIK_V) & 0x80)
		m_bKey[2] = true;
	if (Engine::Get_DIKeyState(DIK_B) & 0x80)
		m_bKey[2] = false;

	if (m_iCurHealth < m_iMaxHealth && Engine::Get_DIKeyState(DIK_Q) & 0x80)
		m_iCurHealth++;
	else if (m_iCurHealth > 0 && Engine::Get_DIKeyState(DIK_E) & 0x80)
		m_iCurHealth--;

	if (m_iCurMana < m_iMaxMana && Engine::Get_DIKeyState(DIK_Z) & 0x80)
		m_iCurMana++;
	else if (m_iCurMana > 0 && Engine::Get_DIKeyState(DIK_C) & 0x80)
		m_iCurMana--;

	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3 vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		D3DXVec3Normalize(&vLook, &vLook);

		Move_Pos(&vLook, m_fSpeed, fTimeDelta);

	}

	if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3 vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		D3DXVec3Normalize(&vLook, &vLook);

		vLook = -vLook;

		Move_Pos(&vLook, m_fSpeed, fTimeDelta);
	}

	if (Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3 vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		D3DXVec3Normalize(&vRight, &vRight);

		Move_Pos(&vRight, m_fSpeed, fTimeDelta);
	}

	if (Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3 vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		D3DXVec3Normalize(&vRight, &vRight);

		vRight = -vRight;

		Move_Pos(&vRight, m_fSpeed, fTimeDelta);
	}

	//Monster Dmg Test
	if (Engine::Get_DIKeyState(DIK_Z) & 0x80)
	{
		CYeti* yet = dynamic_cast<CYeti*>(Engine::Get_GameObject(L"GameLogic", L"Yeti"));
		yet->Get_Dmg();

		CLooker* look = dynamic_cast<CLooker*>(Engine::Get_GameObject(L"GameLogic", L"Looker"));
		look->Get_Dmg();


		CFlamer* flame = dynamic_cast<CFlamer*>(Engine::Get_GameObject(L"GameLogic", L"Flamer"));
		flame->Get_Dmg();
	}

	if (Engine::Get_DIKeyState(DIK_TAB) & 0x80)
	{
		if (m_bCheck)
			return;

		m_bCheck = true;

		if (m_bFix)
			m_bFix = false;
		else
			m_bFix = true;
	}
	else
		m_bCheck = false;

	if (false == m_bFix)
		return;

}

void CPlayer::Change_Weapon()
{
	if (Engine::Get_DIKeyState(DIKEYBOARD_1)) {
		m_eCurWeapon = CLAYMORE;
	}
	else if (Engine::Get_DIKeyState(DIKEYBOARD_2)) {
		m_eCurWeapon = MINIGUN;
	}
	else if (Engine::Get_DIKeyState(DIKEYBOARD_3)) {
		m_eCurWeapon = STAFF;
	}
	else if (Engine::Get_DIKeyState(DIKEYBOARD_4)) {
		m_eCurWeapon = SHOTGUN;
	}
	else if (Engine::Get_DIKeyState(DIKEYBOARD_5)) {
		m_eCurWeapon = REVOLVER;
	}
	else if (Engine::Get_DIKeyState(DIKEYBOARD_6)) {
		m_eCurWeapon = ROCKET_LAUNCHER;
	}
	else if (Engine::Get_DIKeyState(DIKEYBOARD_7)) {
		m_eCurWeapon = REACTOR;
	}
}

void CPlayer::Mouse_Move()
{
	_matrix			matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_long	dwMouseMove(0);

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		m_vAngle.x += D3DXToRadian(dwMouseMove / 10.f);
		if (m_vAngle.x >= D3DXToRadian(90.f))
			m_vAngle.x = D3DXToRadian(90.f);
		if (m_vAngle.x <= -D3DXToRadian(90.f))
			m_vAngle.x = -D3DXToRadian(90.f);
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		m_vAngle.y += D3DXToRadian(dwMouseMove / 10.f);
		if (m_vAngle.y >= D3DXToRadian(360.f))
			m_vAngle.y -= D3DXToRadian(360.f);
		if (m_vAngle.y <= -D3DXToRadian(360.f))
			m_vAngle.y += D3DXToRadian(360.f);
	}
}

void CPlayer::UpdateMatrix(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_int i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	// 회전 변환
	_matrix			matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_int i = 0; i < INFO_POS; ++i)
	{
		for (_int j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	for (_int i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	m_vUp = m_vInfo[INFO_UP];
	m_vEye = m_vInfo[INFO_POS];
	m_vAt = m_vEye + m_vInfo[INFO_LOOK];


	return;
}

void CPlayer::Free()
{
	Engine::CCamera::Free();
	for_each(m_mWeapon.begin(), m_mWeapon.end(), CDeleteMap());
	for_each(m_lBullet.begin(), m_lBullet.end(), CDeleteObj());
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAngle, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAngle, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("dynamiccamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPlayer::Mouse_Fix()
{
	POINT	pt{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);


}