#include "stdafx.h"
#include "Item.h"

#include "Export_Utility.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev, ITEMTYPE eItemType, _int iNum)
    : Engine::CGameObject(pGraphicDev), m_eItemtype(eItemType), m_iNum(iNum)
{
}

CItem::CItem(const CItem& rhs)
    : Engine::CGameObject(rhs), m_eItemtype(rhs.m_eItemtype), m_iNum(rhs.m_iNum)
{
}

CItem::~CItem()
{
}

HRESULT CItem::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return S_OK;
}

_int CItem::Update_GameObject(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
    CGameObject::Update_GameObject(fTimeDelta);
    return 0;
}

void CItem::LateUpdate_GameObject()
{
    __super::LateUpdate_GameObject();
}

void CItem::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 아이템 번호에 따라 texture번호 바꾸기
	m_pTextureCom->Set_Texture(m_iNum);

	m_pBufferCom->Render_Buffer();
}

void CItem::Get_Item()
{

}
HRESULT CItem::Add_Component()
{
	CComponent* pComponent = nullptr;

	switch (m_eItemtype)
	{
	case ITEM_INIT:
		return E_FAIL;
	case BULLET:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ItemTexture_Bullet"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_ItemTexture_Bullet", pComponent });
		break;
	case EXP:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ItemTexture_Exp"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_ItemTexture_Exp", pComponent });
		break;
	case HEALTH:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ItemTexture_Health"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_ItemTexture_Health", pComponent });
		break;
	case KEY:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ItemTexture_Key"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_ItemTexture_Key", pComponent });
		break;
	case MANA:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ItemTexture_Mana"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_ItemTexture_Mana", pComponent });
		break;
	}
	

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });
	
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Transform", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });

	return S_OK;
}

CItem* CItem::Create(LPDIRECT3DDEVICE9 pGraphicDev, ITEMTYPE eItemType, _int iNum)
{
	CItem* pInstance = new CItem(pGraphicDev, eItemType, iNum);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Item Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CItem::Free()
{
	__super::Free();
}