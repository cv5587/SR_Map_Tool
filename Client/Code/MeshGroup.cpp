#include "stdafx.h"
#include "MeshGroup.h"

#include "Export_Utility.h"

CMeshGroup::CMeshGroup(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMeshGroup::CMeshGroup(const CMeshGroup& rhs)
	: Engine::CGameObject(rhs)
{
}

CMeshGroup::~CMeshGroup()
{
	CMeshGroup::Free();
}

HRESULT CMeshGroup::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CMeshGroup::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CMeshGroup::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CMeshGroup::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	if (SeeMap)
	{
		for (int i = 0; i < m_pvecMeshGroupCom.size(); i++)
		{
			m_pvecTextureGroupCom[i]->Set_Texture(*(m_pvecTextureGroupCom[i]->Get_Texture()));
			for (int j = 0; j < m_pvecMeshGroupCom[i]->GetNumFaces(); j++)
			{
				m_pvecMeshGroupCom[i]->Render_Buffer(j);
			}
		}

	}

	
	if (SeeDecal)
	{
		for (int i = 0; i < m_pvecDMeshGroupCom.size(); i++)
		{
			m_pvecDTextureGroupCom[i]->Set_Texture(*(m_pvecDTextureGroupCom[i]->Get_Texture()));
			for (int j = 0; j < m_pvecDMeshGroupCom[i]->GetNumFaces(); j++)
			{
				m_pvecDMeshGroupCom[i]->Render_Buffer(j);
			}
		}

	}

	

}

HRESULT CMeshGroup::Add_Component()
{
	
	const std::vector<Engine::FbxMeshInfo> tmpMeshGroup = Engine::GetMeshGroup();

	//tmpMeshGroup.front().vertices[0].vPosition;
	
	TCHAR szProtoName[128] = L"Proto_Mesh%d";

	for (int i = 0; i < tmpMeshGroup.size(); ++i)
	{
		TCHAR	szName[128] = L"";
		
		wsprintf(szName, szProtoName, i);

		CComponent* pComponent = nullptr;
		CMesh* pMesh = nullptr;

		pComponent = pMesh = dynamic_cast<CMesh*>(Engine::Clone_Proto(wstring(szName)));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ wstring(szName).c_str(), pComponent});
		m_pvecMeshGroupCom.push_back(pMesh);
	}


	for (int i = 0; i < tmpMeshGroup.size(); ++i)
	{

		CComponent* pComponent = nullptr;
		CTexture* pTextureCom = nullptr;

		pComponent = pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_TerrainTexture0"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({L"Proto_TerrainTexture0", pComponent});
		m_pvecTextureGroupCom.push_back(pTextureCom);
	}


	const std::vector<Engine::FbxMeshInfo> tmpDMeshGroup = Engine::GetDMeshGroup();

	TCHAR szProtoDName[128] = L"Proto_DMesh%d";

	for (int i = 0; i < tmpDMeshGroup.size(); ++i)
	{
		TCHAR	szName[128] = L"";

		wsprintf(szName, szProtoDName, i);

		CComponent* pComponent = nullptr;
		CMesh* pMesh = nullptr;

		pComponent = pMesh = dynamic_cast<CMesh*>(Engine::Clone_Proto(wstring(szName)));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ wstring(szName).c_str(), pComponent });
		m_pvecDMeshGroupCom.push_back(pMesh);
	}


	for (int i = 0; i < tmpDMeshGroup.size(); ++i)
	{

		CComponent* pComponent = nullptr;
		CTexture* pTextureCom = nullptr;

		pComponent = pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_TerrainTexture0"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[ID_STATIC].insert({ L"Proto_TerrainTexture0", pComponent });
		m_pvecDTextureGroupCom.push_back(pTextureCom);
	}

	CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Transform", pComponent });


	return S_OK;
}


CMeshGroup* CMeshGroup::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMeshGroup* pInstance = new CMeshGroup(pGraphicDev);
	
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Terrain Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMeshGroup::Free()
{
	for (int i = 0; i < m_pvecMeshGroupCom.size(); ++i)
	{
		Safe_Release(m_pvecMeshGroupCom[i]);
	}
	for (int i = 0; i < m_pvecTextureGroupCom.size(); ++i)
	{
		Safe_Release(m_pvecTextureGroupCom[i]);
	}

	for (int i = 0; i < m_pvecDMeshGroupCom.size(); ++i)
	{
		Safe_Release(m_pvecDMeshGroupCom[i]);
	}
	for (int i = 0; i < m_pvecDTextureGroupCom.size(); ++i)
	{
		Safe_Release(m_pvecDTextureGroupCom[i]);
	}

	m_pvecMeshGroupCom.clear();
	m_pvecDMeshGroupCom.clear();

	CGameObject::Free();
}
