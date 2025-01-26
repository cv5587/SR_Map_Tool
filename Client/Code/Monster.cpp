#include "stdafx.h"
#include "..\Header\Monster.h"

#include "Export_Utility.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pBufferCom(nullptr), m_pTransformCom(nullptr),
	m_pTextureCom(nullptr), m_pTexCom_Atk(nullptr), m_pTexCom_Dead(nullptr),
	m_pTexCom_Idle(nullptr), m_pTexCom_Run(nullptr), m_pTexCom_Hit(nullptr)
	, m_bHit(false), m_bDead(false), m_bInit(false), m_eState(MONSTER_IDLE), m_ePreState(MONSTER_END)
	, m_iCurHealth(0), m_iDamage(0), m_fSpeed(0.f), m_iMaxHealth(0),
	m_fDtctRng(0.f), m_fAtkRng(0.f),
	m_fFrame(0.f), m_fFrameSpeed(0.f), m_iMaxFrame(0),
	m_fAtkTimer(0.f), m_fStiffTimer(0.f), m_fStiffSecond(0.f), m_pPlayer(nullptr), m_bAtk(false)
{
}

CMonster::CMonster(const CMonster& rhs)
	: Engine::CGameObject(rhs), m_pBufferCom(rhs.m_pBufferCom), m_pTransformCom(rhs.m_pTransformCom),
	m_pTextureCom(rhs.m_pTextureCom), m_pTexCom_Atk(rhs.m_pTexCom_Atk), m_pTexCom_Dead(rhs.m_pTexCom_Dead),
	m_pTexCom_Idle(rhs.m_pTexCom_Idle), m_pTexCom_Run(rhs.m_pTexCom_Run), m_pTexCom_Hit(rhs.m_pTexCom_Hit)
	, m_bHit(rhs.m_bHit), m_bDead(rhs.m_bDead), m_bInit(rhs.m_bInit), m_eState(rhs.m_eState), m_ePreState(rhs.m_ePreState)
	, m_iCurHealth(rhs.m_iCurHealth), m_iDamage(rhs.m_iDamage), m_fSpeed(rhs.m_fSpeed), m_iMaxHealth(rhs.m_iMaxHealth),
	m_fDtctRng(rhs.m_fDtctRng), m_fAtkRng(rhs.m_fAtkRng),
	m_fFrame(rhs.m_fFrame), m_szTexName(rhs.m_szTexName), m_fFrameSpeed(rhs.m_fFrameSpeed), m_iMaxFrame(rhs.m_iMaxFrame),
	m_fAtkTimer(rhs.m_fAtkTimer), m_fStiffTimer(rhs.m_fStiffTimer), m_fStiffSecond(rhs.m_fStiffSecond), m_pPlayer(nullptr), m_bAtk(rhs.m_bAtk)
{


}

CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//test

	return S_OK;
}

Engine::_int CMonster::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CMonster::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMonster::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pBufferCom->Render_Buffer();
}



void CMonster::Motion_Change()
{
}

HRESULT CMonster::Add_Component()
{
	CComponent* pComponent = nullptr;


	return S_OK;
}

_float CMonster::DistanceToPlayer(CPlayer* pPlayer)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	return D3DXVec3Length(&(pPlayer->Get_Player_Eye() - vPos));
}


void CMonster::Chase_Player(CPlayer* pPlayer, const _float& fTimeDelta)
{
	//�÷��̾� ��ġ �޾ƿ�
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos = pPlayer->Get_Player_Eye() - vPos;
	vPos.y = 0.f;
	D3DXVec3Normalize(&vPos, &vPos);
	//���⼭ y�� ���� ��ų �� ����
}

void CMonster::Knock_Back(CPlayer* pPlayer, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos = vPos - pPlayer->Get_Player_Eye();
}




CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster* pInstance = DBG_NEW CMonster(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Monster Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMonster::Free()
{
	__super::Free();
}

