#pragma once
#include "GameObject.h"
#include "Engine_Define.h"
#include "Mesh.h"
#include "fbxsdk.h"

BEGIN(Engine)

class ENGINE_DLL CFBXLoader : public CBase
{
	DECLARE_SINGLETON(CFBXLoader)
private:
	explicit CFBXLoader();
	virtual ~CFBXLoader();

public:

	HRESULT LoadFbx(std::wstring pPath, std::wstring pDPath);
	void DrawMesh();

public:
	_int GetMeshCount() { return static_cast<_int>(m_vMesh.size()); }
	const FbxMeshInfo& GetMesh(_int idx) { return m_vMesh[idx]; }
	const std::vector<FbxMeshInfo>& GetMeshVector() { return m_vMesh; }

	_int GetDMeshCount() { return static_cast<_int>(m_vDMesh.size()); }
	const FbxMeshInfo& GetDMesh(_int idx) { return m_vDMesh[idx]; }
	const std::vector<FbxMeshInfo>& GetDMeshVector() { return m_vDMesh; }

private:
	void Import(const wstring& path);

	void ParseNode(FbxNode* node);
	void LoadMesh(FbxMesh* mesh);

	void		GetNormal(FbxMesh* mesh, FbxMeshInfo* container, _int idx, _int vertexCounter);
	void		GetUV(FbxMesh* mesh, FbxMeshInfo* container, _int idx, _int vertexCounter);


private:
	virtual void		Free();

private:
	std::vector<FbxMeshInfo> m_vMesh;
	std::vector<FbxMeshInfo> m_vDMesh;
	_uint m_iFileIndex;

	FbxManager* m_pManager = nullptr;
	FbxManager* m_pDManager = nullptr;
	FbxScene* m_pScene = nullptr;
	FbxScene* m_pDScene = nullptr;
	fbxsdk::FbxImporter* m_pImporter = nullptr;


};

END