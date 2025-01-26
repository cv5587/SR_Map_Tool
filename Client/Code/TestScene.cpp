#include "stdafx.h"
#include "..\Header\TestScene.h"

#include "Export_System.h"
#include "Export_Utility.h"


#include "BackGround.h"
#include "Stage.h"

CTestScene::CTestScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CTestScene::~CTestScene()
{
}

HRESULT CTestScene::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Environment"), E_FAIL);

	return S_OK;
}

Engine::_int CTestScene::Update_Scene(const _float& fTimeDelta)
{
	_int	iExit = __super::Update_Scene(fTimeDelta);


		if (GetAsyncKeyState(VK_RETURN))
		{
			Engine::CScene* pScene = nullptr;

			pScene = CStage::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, -1);

			FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);

			return 0;
		}

	return iExit;
}

void CTestScene::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void CTestScene::Render_Scene()
{
}

HRESULT CTestScene::Ready_Prototype()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LogoTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Logo/IU.jpg")), E_FAIL);


	return S_OK;
}

HRESULT CTestScene::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);


	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

CTestScene* CTestScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestScene* pInstance = new CTestScene(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTestScene::Free()
{
	__super::Free();
}
