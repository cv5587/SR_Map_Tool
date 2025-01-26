#include "stdafx.h"
#include "..\Header\Fire.h"

#include "Export_System.h"
#include "Export_Utility.h"
#include "Player.h"

CFire::CFire(LPDIRECT3DDEVICE9 pGraphicDev)
	: CThorn(pGraphicDev), m_fAtkTimer(0)
{
}

CFire::CFire(const CFire& rhs)
	: CThorn(rhs)
{

}

CFire::~CFire()
{
}

HRESULT CFire::Ready_GameObject()
{
	m_bInit = false;
	TexNamer();
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(7.f, 1.f, 2.f);
	return S_OK;
}

Engine::_int CFire::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bInit)
	{
		m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"));
		m_bInit = true;
	}

	m_fFrame += 35.f * fTimeDelta;

	if (35.f < m_fFrame)
		m_fFrame = 0.f;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);

	//Detect_Player(fTimeDelta);

	return OBJ_NOEVENT;
}

void CFire::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();

}

void CFire::Render_GameObject()
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

	m_pTextureCom->Set_Texture((_uint)m_fFrame / 5);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

void CFire::TexNamer()
{
	m_szTexName = L"Proto_FireTexture";
}

void CFire::Detect_Player(const _float& fTimeDelta)
{
	//_vec3	vPlayerPos = m_pPlayer->Get_Player_Eye();
	_vec3	vPlayerPos = { 0,0,0 };

	_vec3 vPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (5.f > D3DXVec3Length(&(vPlayerPos - vPos))) //test
	{
		m_fAtkTimer += 10 * fTimeDelta;
		if (30.f < m_fAtkTimer)
		{
			m_fAtkTimer = 0.f;
			//m_pPlayer->Add_Health(-1);
		}
		//일정시간마다 플레이어 체력 깎음
	}


}

HRESULT CFire::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_szTexName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({m_szTexName.c_str(), pComponent});

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	return S_OK;
}

CFire* CFire::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFire* pInstance = DBG_NEW CFire(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Prop Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CFire::Free()
{
	__super::Free();
}
