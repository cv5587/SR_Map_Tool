#include "stdafx.h"
#include "..\Header\Looker.h"
#include "MonsterBullet.h"

CLooker::CLooker(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CLooker::CLooker(const CLooker& rhs)
	: CMonster(rhs)
{

}

CLooker::~CLooker()
{
}

HRESULT CLooker::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iMaxHealth = m_iCurHealth = 100;
	m_iDamage = 5;
	m_fSpeed = 1.f;
	m_fDtctRng = 30.f;
	m_fAtkRng = 15.f;
	m_bInit = false;
	m_bHit = false;
	m_fAtkTimer = 0.f;
	m_fStiffTimer = 0.f;
	m_pTransformCom->Set_Pos(5.f, 1.f, 5.f);

	m_eState = MONSTER_IDLE;
	m_ePreState = MONSTER_END;
	return S_OK;
}

Engine::_int CLooker::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);

	//m_fFrame += _float(m_fFrameSpeed) * fTimeDelta;

	////버그 있으면 수정요청 바람..
	//if (m_fFrame > _float(m_iMaxFrame) && m_eState != MONSTER_DEAD)
	//	m_fFrame = 0.f;

	//if (m_bHit)
	//{
	//	Looker_Get_Dmg(fTimeDelta);
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

	//			_vec3 vSrc;
	//			m_pTransformCom->Get_Info(INFO_POS, &vSrc);
	//			m_lBullets.push_back(CMonsterBullet::Create(m_pGraphicDev, BULLET_LOOKER, vSrc, m_pPlayer->Get_Player_Eye()));
	//			m_pPlayer->Add_Health(-10);
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

	if (!m_lBullets.empty())
	{
		for (auto iter : m_lBullets)
		{
			iter->Update_GameObject(fTimeDelta);
		}
	}
	//TODO ; 몬스터 불렛 리스트 업데이트

	return 0;
}

void CLooker::LateUpdate_GameObject()
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
				iter = m_lBullets.erase(iter);
				//iter++;
			}
			else
			{
				(*iter)->LateUpdate_GameObject();
				iter++;
			}
		}
	}
}

void CLooker::Render_GameObject()
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

HRESULT CLooker::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	//Textures
	pComponent = m_pTexCom_Idle = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LookerIdleTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LookerIdleTexture", pComponent });

	pComponent = m_pTexCom_Run = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LookerRunTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LookerRunTexture", pComponent });

	pComponent = m_pTexCom_Atk = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LookerAtkTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LookerAtkTexture", pComponent });

	pComponent = m_pTexCom_Hit = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LookerHitTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LookerHitTexture", pComponent });

	pComponent = m_pTexCom_Dead = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LookerDeadTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_LookerDeadTexture", pComponent });
	//Texture Ends

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CLooker* CLooker::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLooker* pInstance = DBG_NEW CLooker(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Monster Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLooker::Free()
{
	CMonster::Free();
	for_each(m_lBullets.begin(), m_lBullets.end(), CDeleteObj());
}

void CLooker::Motion_Change()
{
	if (m_eState != m_ePreState)
	{
		switch (m_eState)
		{
		case MONSTER_IDLE:
		{
			m_pTextureCom = m_pTexCom_Idle;
			m_iMaxFrame = 10;
			m_fFrameSpeed = 10.f;
		}
		break;
		case MONSTER_RUN:
		{
			m_pTextureCom = m_pTexCom_Run;
			m_iMaxFrame = 10;
			m_fFrameSpeed = 10.f;
		}
		break;
		case MONSTER_ATK:
		{
			m_pTextureCom = m_pTexCom_Atk;
			m_iMaxFrame = 17;
			m_fFrameSpeed = 10.f;
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
			m_iMaxFrame = 15;
			m_fFrameSpeed = 10.f;
		}
		break;
		}

		m_ePreState = m_eState;

	}

}

void CLooker::Looker_Get_Dmg(const _float& fTimeDelta)
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
