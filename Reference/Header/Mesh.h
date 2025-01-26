#pragma once
#include "Component.h"

BEGIN(Engine)



class ENGINE_DLL CMesh : public CComponent
{
private:
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphicDev);

public:

	CMesh(const CMesh& rhs);
	virtual ~CMesh();

public:
	virtual HRESULT		Ready_Buffer(const FbxMeshInfo& vMesh, _uint faceCount);
	void		Render_Buffer(_ulong AttribId);

public:
	static CMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const FbxMeshInfo& vMesh, _uint faceCount);
	virtual CComponent* Clone();

public:
	_ulong GetNumFaces() { return m_dwTriCnt; }

public:
	 vector<_vec3> Get_VtxPos() { return m_pPos; }
	 vector<INDEX32> Get_VtxIdc() { return m_pIdc; }
	 ID3DXMesh* Get_Mesh() { return m_pMesh; }
private:
	HRESULT Set_SubSet(_uint faceCount);


private:
	virtual void	Free();

private:
	LPD3DXMESH m_pMesh;
	_ulong		m_dwVtxCnt;
	_ulong		m_dwVtxSize;
	_ulong		m_dwTriCnt;
	_ulong		m_dwFVF;

	
	vector<_vec3> m_pPos;
	vector<INDEX32> m_pIdc;
};

END

