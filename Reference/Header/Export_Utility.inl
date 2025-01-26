#include "Export_Utility.h"
#include <string>
#include <vector>

CComponent * Get_Component(COMPONENTID eID, const _tchar * pLayerTag, const _tchar * pObjTag, const _tchar * pComponentTag)
{
	return CManagement::GetInstance()->Get_Component(eID, pLayerTag, pObjTag, pComponentTag);
}

CGameObject* Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	return CManagement::GetInstance()->Get_GameObject(pLayerTag, pObjTag);
}
HRESULT Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement ** ppManagementInstance)
{
	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	*ppManagementInstance = pManagement;

	return S_OK;
}

CLayer* Get_Layer(const _tchar* pLayerTag)
{
	return CManagement::GetInstance()->Get_Layer(pLayerTag);
}
HRESULT Set_Scene(CScene * pScene)
{
	return CManagement::GetInstance()->Set_Scene(pScene);
}
_int Update_Scene(const _float & fTimeDelta)
{
	return CManagement::GetInstance()->Update_Scene(fTimeDelta);
}

void LateUpdate_Scene()
{
	CManagement::GetInstance()->LateUpdate_Scene();
}

void Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CManagement::GetInstance()->Render_Scene(pGraphicDev);
}

HRESULT			Ready_Proto(const wstring pProtoTag, CComponent* pComponent)
{
	return CProtoMgr::GetInstance()->Ready_Proto(pProtoTag, pComponent);
}
CComponent*		Clone_Proto(const wstring pProtoTag)
{
	return CProtoMgr::GetInstance()->Clone_Proto(pProtoTag);
}

// Renderer
void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(eID, pGameObject);
}
void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
}
void		Clear_RenderGroup()
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}

void     Set_Camera(CCamera* pCamera)
{
	CRenderer::GetInstance()->Set_Camera(pCamera);
}

void     Set_UI_Camera(CUICamera* pUICamera)
{
	CRenderer::GetInstance()->Set_UI_Camera(pUICamera);
}



wstring s2ws(const string& s)
{
	_int len;
	_int slength = static_cast<_int>(s.length()) + 1;
	len = ::MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	::MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	wstring ret(buf);
	delete[] buf;
	return ret;
}

string ws2s(const wstring& s)
{
	_int len;
	_int slength = static_cast<_int>(s.length()) + 1;
	len = ::WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	string r(len, '\0');
	::WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}

//CollisionMgr
void LateUpte_CollisionMgr()
{
	CCollisionMgr::GetInstance()->LateUpte_CollisionMgr();
}

unsigned int Add_Collider(CGameObject* pGameObject)
{
	return CCollisionMgr::GetInstance()->Add_Collider(pGameObject);
}

HRESULT Remove_Collider(unsigned int particleIdx)
{
	return CCollisionMgr::GetInstance()->Remove_Collider(particleIdx);
}

HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

HRESULT LoadFbx(std::wstring pPath, std::wstring pDPath)
{
	return CFBXLoader::GetInstance()->LoadFbx(pPath, pDPath);
}

_int GetMeshCount() { return static_cast<_int>(CFBXLoader::GetInstance()->GetMeshCount()); }
const FbxMeshInfo& GetMesh(_int idx) { return CFBXLoader::GetInstance()->GetMesh(idx); }

_int GetDMeshCount() { return static_cast<_int>(CFBXLoader::GetInstance()->GetDMeshCount()); }
const FbxMeshInfo& GetDMesh(_int idx) { return CFBXLoader::GetInstance()->GetDMesh(idx); }

const std::vector<FbxMeshInfo>& GetMeshGroup()
{
	return CFBXLoader::GetInstance()->GetMeshVector();
}
const std::vector<FbxMeshInfo>& GetDMeshGroup()
{
	return CFBXLoader::GetInstance()->GetDMeshVector();
}
void Release_Utility()
{
	CLightMgr::DestroyInstance();
	CRenderer::DestroyInstance();
	CProtoMgr::DestroyInstance();
	CManagement::DestroyInstance();
	CFBXLoader::DestroyInstance();
}
