#pragma once
#include "GameObject.h"


BEGIN(Engine)

class CMesh;
class CTexture;
class CTransform;
END

class CMeshGroup :
    public CGameObject
{
private:
	explicit CMeshGroup(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMeshGroup(const CMeshGroup& rhs);
	virtual ~CMeshGroup();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	HRESULT			Add_Component();

private:
	vector<Engine::CMesh*> m_pvecMeshGroupCom;
	vector<Engine::CTexture*> m_pvecTextureGroupCom;

	vector<Engine::CMesh*> m_pvecDMeshGroupCom;
	vector<Engine::CTexture*> m_pvecDTextureGroupCom;

	Engine::CTransform* m_pTransformCom;
	bool SeeMap = false;
	bool SeeDecal = false;

public:
	static CMeshGroup* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	
private:
	virtual void Free() override;

public:
	vector<Engine::CMesh*>* Get_MeshCom() {return &m_pvecMeshGroupCom;	}
	vector<Engine::CTexture*>* Get_TextureCom() {return &m_pvecTextureGroupCom;	}
	vector<Engine::CMesh*>* Get_DMeshCom() { return &m_pvecDMeshGroupCom; }
	vector<Engine::CTexture*>* Get_DTextureCom() { return &m_pvecDTextureGroupCom; }
	bool* Get_SeeMap() { return &SeeMap; }
	bool* Get_SeeDecal() { return &SeeDecal; }
};

