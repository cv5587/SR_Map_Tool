#include "stdafx.h"
#include "..\Header\Flamer.h"


CFlamer::CFlamer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CFlamer::CFlamer(const CFlamer& rhs)
	: CMonster(rhs)
{

}

CFlamer::~CFlamer()
{
}

HRESULT CFlamer::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iMaxHealth = m_iCurHealth = 100;
	m_iDamage = 5;
	m_fSpeed = 1.f;
	m_fDtctRng = 20.f;
	m_fAtkRng = 7.f;
	m_bInit = false;
	m_bHit = false;
	m_fAtkTimer = 0.f;
	m_fStiffTimer = 0.f;
	m_pTransformCom->Set_Pos(5.f, 1.f, 5.f);

	m_eState = MONSTER_IDLE;
	m_ePreState = MONSTER_END;
	return S_OK;
}

Engine::_int CFlamer::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);

	//m_fFrame += _float(m_fFrameSpeed) * fTimeDelta;

	////버그 있으면 수정요청 바람..
	//if (m_fFrame > m_iMaxFrame && m_eState != MONSTER_DEAD)
	//	m_fFrame = 0.f;

	//if (m_bHit)
	//{
	//	Flamer_Get_Dmg(fTimeDelta);
	//}

	//switch (m_eState)
	//{
	//case MONSTER_IDLE:
	//{
	//	if (!m_bInit)
	//	{
	//		m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));
	//		m_bInit = true;
	//	}

	//	if (m_fDtctRng > DistanceToPlayer(m_pPlayer))
	//	{
	//		m_eState = MONSTER_RUN;
	//	}
	//}
	//break;
	//case MONSTER_RUN:
	//{
	//	if (m_fAtkRng > DistanceToPlayer(m_pPlayer))
	//	{
	//		m_eState = MONSTER_ATK;

	//	}
	//	else
	//	{
	//		Chase_Player(m_pPlayer, fTimeDelta);
	//	}
	//}
	//break;
	//case MONSTER_ATK:
	//{
	//	if (m_fAtkRng < DistanceToPlayer(m_pPlayer))
	//	{
	//		m_eState = MONSTER_RUN;
	//	}
	//	else
	//	{
	//		m_fAtkTimer += 10 * fTimeDelta;
	//		if (30.f < m_fAtkTimer)
	//		{
	//			m_fAtkTimer = 0.f;
	//			m_pPlayer->Add_Health(-3);

	//			_vec3 vSrc;
	//			m_pTransformCom->Get_Info(INFO_POS, &vSrc);
	//			m_lBullets.push_back(CMonsterBullet::Create(m_pGraphicDev, BULLET_FLAMER, vSrc, m_pPlayer->Get_Player_Eye()));
	//		}
	//	}
	//}
	//break;
	//case MONSTER_HIT:
	//{
	//	m_fStiffTimer += 10 * fTimeDelta;
	//	if (30.f < m_fStiffTimer)
	//	{
	//		m_fStiffTimer = 0.f;
	//		m_eState = MONSTER_RUN;
	//	}
	//}
	//break;
	//case MONSTER_DEAD:
	//{
	//	if (m_iMaxFrame <= (_uint)(m_fFrame))
	//	{
	//		//TODO : 매끄러운 사망 구현
	//		return OBJ_DEAD;
	//	}

	//}
	//break;
	//}

	//if (!m_lBullets.empty())
	//{
	//	for (auto iter : m_lBullets)
	//	{
	//		iter->Update_GameObject(fTimeDelta);
	//	}
	//}

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

	return 0;
}

void CFlamer::LateUpdate_GameObject()
{
	Motion_Change();
	__super::LateUpdate_GameObject();

	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	__super::Compute_ViewZ(&vPos);

	if (!m_lBullets.empty())
	{
		for (auto iter = m_lBullets.begin(); iter != m_lBullets.end();)
		{
			if ((*iter)->Get_Dead())
			{
				//*m_lBullets.erase(iter);
				iter++;
			}
			else
			{
				(*iter)->LateUpdate_GameObject();
				iter++;
			}
		}
	}
}

void CFlamer::Render_GameObject()
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

	m_pTextureCom->Set_Texture((_uint)(m_fFrame) % m_iMaxFrame);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

HRESULT CFlamer::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	//Textures
	pComponent = m_pTexCom_Idle = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FlamerIdleTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_FlamerIdleTexture", pComponent });

	pComponent = m_pTexCom_Run = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FlamerRunTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_FlamerIdleTexture", pComponent });

	pComponent = m_pTexCom_Atk = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FlamerAtkTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_FlamerAtkTexture", pComponent });

	pComponent = m_pTexCom_Hit = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FlamerHitTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_FlamerHitTexture", pComponent });

	pComponent = m_pTexCom_Dead = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FlamerDeadTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_FlamerDeadTexture", pComponent });
	//Texture Ends

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CFlamer* CFlamer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFlamer* pInstance = DBG_NEW CFlamer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Monster Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CFlamer::Free()
{
	CMonster::Free();
}

void CFlamer::Motion_Change()
{
	if (m_eState != m_ePreState)
	{
		switch (m_eState)
		{
		case MONSTER_IDLE:
		{
			m_pTextureCom = m_pTexCom_Idle;
			m_iMaxFrame = 1;
			m_fFrameSpeed = 1.f;
		}
		break;
		case MONSTER_RUN:
		{
			m_pTextureCom = m_pTexCom_Run;
			m_iMaxFrame = 6;
			m_fFrameSpeed = 5.f;
		}
		break;
		case MONSTER_ATK:
		{
			m_pTextureCom = m_pTexCom_Atk;
			m_iMaxFrame = 16;
			m_fFrameSpeed = 15.f;
		}
		break;
		case MONSTER_HIT:
		{
			m_pTextureCom = m_pTexCom_Hit;
			m_iMaxFrame = 1;
			m_fFrameSpeed = 1.f;
		}
		break;
		case MONSTER_DEAD:
		{
			m_pTextureCom = m_pTexCom_Dead;
			m_iMaxFrame = 22;
			m_fFrameSpeed = 10.f;
		}
		break;
		}

		m_ePreState = m_eState;

	}

}

void CFlamer::Flamer_Get_Dmg(const _float& fTimeDelta)
{
	if (m_eState != MONSTER_DEAD)
	{
		if (0 < m_iCurHealth)
		{
			m_eState = MONSTER_HIT;
			Knock_Back(m_pPlayer, m_fSpeed * 0.8f, fTimeDelta);
		}
		else if (0 >= m_iCurHealth)
		{
			m_eState = MONSTER_DEAD;
			m_fFrame = 0.f;
		}
		m_bHit = false;
	}
}
