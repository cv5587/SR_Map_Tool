#include "stdafx.h"
#include "..\Header\Bullet.h"
#include "Export_Utility.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CBullet::CBullet(const CBullet& rhs)
	: Engine::CGameObject(rhs)
{
}

CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

Engine::_int CBullet::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);
	m_fSpeed -= 0.1f;
	if (m_fSpeed < 0 && !m_bFall)
	{
		m_bFall = true;
		m_vDir.x = -m_vDir.x;
		m_vDir.z = -m_vDir.z;
	}
	m_pTransformCom->m_vScale = { 0.3f, 0.3f, 0.3f };
	m_pTransformCom->Move_Pos(&m_vDir, m_fSpeed, fTimeDelta);
	m_pTransformCom->Update_Component(fTimeDelta);
	return 0;
}

void CBullet::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CBullet::Render_GameObject()
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

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CBullet::Set_Pos(_vec3 vPos, _vec3 vDir)
{
	m_pTransformCom->Set_Pos(vPos);
	m_vDir = vDir;
	m_vDir.y = -m_vDir.y;
	/*_matrix	matRot;
	D3DXQUATERNION quatRot;
	float fRadian = D3DXToRadian(45.f);
	D3DXQuaternionRotationYawPitchRoll(&quatRot, fRadian, fRadian, fRadian);
	D3DXMatrixRotationQuaternion(&matRot, &quatRot);
	D3DXVec3TransformNormal(&m_vDir, &m_vDir, &matRot);
	D3DXVec3Normalize(&m_vDir, &m_vDir);*/
}

HRESULT CBullet::Add_Component()
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });
		
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Bullet"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Bullet", pComponent });


	return S_OK;
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBullet *	pInstance = DBG_NEW CBullet(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Bullet Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CBullet::Free()
{	
	__super::Free();
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}