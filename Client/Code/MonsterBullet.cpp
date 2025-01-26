#include "stdafx.h"
#include "..\Header\MonsterBullet.h"

#include "Export_Utility.h"

CMonsterBullet::CMonsterBullet(LPDIRECT3DDEVICE9 pGraphicDev, BULLETID eBulletID, const _vec3& vFrom, const _vec3& vTo)
	: Engine::CGameObject(pGraphicDev), m_bDead(false), m_bInit(true), m_eBulletID(eBulletID), m_vFrom(vFrom), m_vTo(vTo),
	m_fSpeed(1.f)
{
}

CMonsterBullet::CMonsterBullet(const CMonsterBullet& rhs)
	: Engine::CGameObject(rhs)
{

}

CMonsterBullet::~CMonsterBullet()
{
	Free();
}

HRESULT CMonsterBullet::Ready_GameObject()
{
	switch (m_eBulletID)
	{
	case BULLET_LOOKER:
		*m_szTexName = L"Proto_LookerBulletTexture";
		m_iMaxFrame = 5;
		m_fFrameSpeed = 3.f;
		break;
	case BULLET_FLAMER:
		*m_szTexName = L"Proto_FlamerBulletTexture";
		m_iMaxFrame = 8;
		m_fFrameSpeed = 10.f;
		break;
	default:
		break;
	}

	m_pPlayer = dynamic_cast<CPlayer*>(Get_GameObject(L"GameLogic", L"Player"));
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(m_vFrom);
	return S_OK;
}

Engine::_int CMonsterBullet::Update_GameObject(const _float& fTimeDelta)
{

	m_fFrame += _float(m_fFrameSpeed) * fTimeDelta;

	//버그 있으면 수정요청 바람..
	if (m_fFrame > m_iMaxFrame)
		m_fFrame = 0.f;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	_vec3 vAngle = { 0.f, 0.f, 0.f };

	//왜 빌보드 적용하면 회전이 맛가는가



	Engine::Add_RenderGroup(RENDER_ALPHA, this);


	CGameObject::Update_GameObject(fTimeDelta);

	_vec3 vGo;
	D3DXVec3Normalize(&vGo, &(m_vTo - m_vFrom));
	m_pTransformCom->Move_Pos(&vGo, m_fSpeed, fTimeDelta);

	m_pTransformCom->Update_Component(fTimeDelta);

	if (!m_bDead)
	{
		if (3.f > DistanceToPlayer(m_pPlayer))
		{
			m_bDead = true;
		}
	}
	

	return 0;
}

void CMonsterBullet::LateUpdate_GameObject()
{

	//if(m_bDead)
	//{
	//	Free();
	//}

	__super::LateUpdate_GameObject();	

	//_vec3	vPos;
	//m_pTransformCom->Get_Info(INFO_POS, &vPos);
	//__super::Compute_ViewZ(&vPos);
}

void CMonsterBullet::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xcc);

	_matrix	matWorld, matView, matBill;

m_pTransformCom->Get_WorldMatrix(&matWorld);
m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
D3DXMatrixIdentity(&matBill);

matBill._11 = matView._11;
matBill._13 = matView._13;
matBill._31 = matView._31;
matBill._33 = matView._33;

D3DXMatrixInverse(&matBill, NULL, &matBill);

m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom->Set_Texture((_uint)(m_fFrame) % m_iMaxFrame);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

void CMonsterBullet::Motion_Change()
{
}

HRESULT CMonsterBullet::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(*m_szTexName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ *m_szTexName, pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });
	//왜 다이나믹으로 바꾸면 출력이 안되는가

	return S_OK;
}

_float CMonsterBullet::DistanceToPlayer(CPlayer* pPlayer)
{
	_vec3 vPos;
	_matrix* vWorld;

	vWorld = m_pTransformCom->Get_WorldMatrix();

	vPos.x = vWorld->m[3][0];
	vPos.y = vWorld->m[3][1];
	vPos.z = vWorld->m[3][2];

	return D3DXVec3Length(&(pPlayer->Get_Player_Eye() - vPos));
}

void CMonsterBullet::Chase_Player(CPlayer* pPlayer, const _float& fTimeDelta)
{

	//플레이어 위치 받아옴
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos = pPlayer->Get_Player_Eye() - vPos;
	//여기서 y값 고정 시킬 수 있음
	m_pTransformCom->Move_Pos(&vPos, m_fSpeed, fTimeDelta);
}

void CMonsterBullet::Knock_Back(CPlayer* pPlayer, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos = vPos - pPlayer->Get_Player_Eye();
	m_pTransformCom->Move_Pos(&vPos, fSpeed, fTimeDelta);
}




CMonsterBullet* CMonsterBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, BULLETID eBulletID, const _vec3& vFrom, const _vec3& vTo)
{
	CMonsterBullet* pInstance = DBG_NEW CMonsterBullet(pGraphicDev, eBulletID, vFrom, vTo);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("MonsterBullet Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CMonsterBullet::Free()
{
	__super::Free();
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}
