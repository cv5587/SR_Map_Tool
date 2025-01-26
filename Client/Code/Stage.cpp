#include "stdafx.h"
#include "..\Header\Stage.h"

#include "Export_System.h"
#include "Export_Utility.h"

#include "Player.h"
#include "Monster.h"
#include "BackGround.h"
#include "Terrain.h"
#include "PlayerUICamera.h"
#include "SkyBox.h"
#include "Effect.h"
#include "TmpMesh.h"
#include "MeshGroup.h"

#include "Gui.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Layer_GameObj(L"GameData"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);


	return S_OK;
}

Engine::_int CStage::Update_Scene(const _float& fTimeDelta)
{	
	return __super::Update_Scene(fTimeDelta);
}

void CStage::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CStage::Render_Scene()
{
	 const _tchar* szProtoName = L"Player Pos: (X : %d, Y : %d, Z: %d ";
	const _vec3 vPlayerPos = m_pPlayer->Get_Player_Eye();

	TCHAR   szPlayerPos[128] = L"";

	wsprintf(szPlayerPos, szProtoName, (int)vPlayerPos.x, (int)vPlayerPos.y, (int)vPlayerPos.z);

	Render_Font(L"Font_Default", szPlayerPos, &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	//Engine::DrawMesh();
	// DEBUG
}

HRESULT CStage::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	
	Engine::CGameObject*		pGameObject = nullptr;


	
		
	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.insert({ pLayerTag, pLayer });

	Engine::CGameObject*		pGameObject = nullptr;


	pGameObject = m_pPlayer = CPlayer::Create(m_pGraphicDev,
			&_vec3(-2.f, 2.1f, 1.f),
			&_vec3(0.f, D3DXToRadian(180.f), 0.f),
			D3DXToRadian(60.f),
			(_float)WINCX / WINCY,
			0.1f,
			1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	Engine::Set_Camera(static_cast<Engine::CCamera*>(pGameObject));

	pGameObject = CMeshGroup::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MeshGroup", pGameObject), E_FAIL);
	

	pGameObject = CGui::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Gui", pGameObject), E_FAIL);



	dynamic_cast<CGui*>(Get_GameObject(L"GameLogic", L"Gui"))->Set_MapTextrue(
		dynamic_cast<CMeshGroup*>(Get_GameObject(L"GameLogic", L"MeshGroup"))->Get_TextureCom());

	dynamic_cast<CGui*>(Get_GameObject(L"GameLogic", L"Gui"))->Set_MapMesh(
		dynamic_cast<CMeshGroup*>(Get_GameObject(L"GameLogic", L"MeshGroup"))->Get_MeshCom());

	dynamic_cast<CGui*>(Get_GameObject(L"GameLogic", L"Gui"))->Set_MapDTextrue(
		dynamic_cast<CMeshGroup*>(Get_GameObject(L"GameLogic", L"MeshGroup"))->Get_DTextureCom());

	dynamic_cast<CGui*>(Get_GameObject(L"GameLogic", L"Gui"))->Set_MapDMesh(
		dynamic_cast<CMeshGroup*>(Get_GameObject(L"GameLogic", L"MeshGroup"))->Get_DMeshCom());

	dynamic_cast<CGui*>(Get_GameObject(L"GameLogic", L"Gui"))->Set_bMap(
		dynamic_cast<CMeshGroup*>(Get_GameObject(L"GameLogic", L"MeshGroup"))->Get_SeeMap());

	dynamic_cast<CGui*>(Get_GameObject(L"GameLogic", L"Gui"))->Set_bDecal(
		dynamic_cast<CMeshGroup*>(Get_GameObject(L"GameLogic", L"MeshGroup"))->Get_SeeDecal());

	for (int i = 0; i < (dynamic_cast<CMeshGroup*>(Get_GameObject(L"GameLogic", L"MeshGroup"))->Get_TextureCom())->size(); i++)
	{
		TileType* ptile = new TileType;

		ptile->TileName = L"Proto_TerrainTexture0";
		ptile->Tilenum = 0;
		dynamic_cast<CGui*>(Get_GameObject(L"GameLogic", L"Gui"))->Set_vecTile(ptile);

	}
	for (int i = 0; i < (dynamic_cast<CMeshGroup*>(Get_GameObject(L"GameLogic", L"MeshGroup"))->Get_DTextureCom())->size(); i++)
	{
		TileType* ptile = new TileType;

		ptile->TileName = L"Proto_TerrainTexture0";
		ptile->Tilenum = 0;
		dynamic_cast<CGui*>(Get_GameObject(L"GameLogic", L"Gui"))->Set_vecDTile(ptile);
	}




	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	//pGameObject = CPlayerUICamera::Create(m_pGraphicDev, 0.f, 50.f, 2.f, 3.56f);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GameUICamera", pGameObject), E_FAIL);

	//Engine::Set_UI_Camera(static_cast<Engine::CUICamera*>(pGameObject));

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameObj(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();	
	NULL_CHECK_RETURN(pLayer, E_FAIL);	

	//클릭하면 여기 레이어로 모든 오브젝트 배치 한덩이가 좋을듯
	Engine::CGameObject* pGameObject = nullptr;


	m_mapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

HRESULT CStage::Ready_LightInfo()
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.f);
	tLightInfo.Specular  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.f);
	tLightInfo.Ambient	 = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage *	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Stage Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CStage::Free()
{
	__super::Free();
}
