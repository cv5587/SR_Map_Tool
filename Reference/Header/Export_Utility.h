#pragma once

#include "Management.h"
#include "ProtoMgr.h"
#include "Renderer.h"
#include "LightMgr.h"

#include "TriCol.h"
#include "RcCol.h"
#include "RcTex.h"
#include "TerrainTex.h"
#include "CubeTex.h"

#include "Texture.h"
#include "Mesh.h"

#include "Transform.h"
#include "Camera.h"
#include "UICamera.h"
#include "Calculator.h"
#include "FBXLoader.h"
#include "CollisionMgr.h"

#include "ParticleBuffer.h"

BEGIN(Engine)

// Management

inline CComponent*		Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag);
inline CGameObject*		Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
inline HRESULT			Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagementInstance);
inline CLayer*				 Get_Layer(const _tchar* pLayerTag);

inline HRESULT			Set_Scene(CScene* pScene);
inline _int				Update_Scene(const _float& fTimeDelta);
inline void				LateUpdate_Scene();
inline void				Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

// ProtoMgr
inline HRESULT			Ready_Proto(const wstring pProtoTag, CComponent* pComponent);
inline CComponent*		Clone_Proto(const wstring pProtoTag);

// Renderer
inline void		Add_RenderGroup(RENDERID eID, CGameObject* pGameObject);
inline void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
inline void		Clear_RenderGroup();

inline void     Set_Camera(CCamera* pCamera);
inline void     Set_UI_Camera(CUICamera* pUICamera);


inline _int GetMeshCount();
inline const FbxMeshInfo& GetMesh(_int idx);
inline const std::vector<FbxMeshInfo>& GetMeshGroup();

inline _int GetDMeshCount();
inline const FbxMeshInfo& GetDMesh(_int idx);
inline const std::vector<FbxMeshInfo>& GetDMeshGroup();

inline HRESULT LoadFbx(std::wstring pPath, std::wstring pDPath);


// LightMgr
inline HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
								const D3DLIGHT9* pLightInfo,
								const _uint& iIndex);

//CollisionMgr
inline void LateUpte_CollisionMgr();
inline unsigned int Add_Collider(CGameObject* pGameObject);
inline HRESULT Remove_Collider(unsigned int particleIdx);

//string utils
inline wstring s2ws(const string& s);
inline string ws2s(const wstring& s);

// Release
inline void				Release_Utility();


#include "Export_Utility.inl"

END