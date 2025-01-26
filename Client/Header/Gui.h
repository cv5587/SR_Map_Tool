#pragma once
#include "GameObject.h"
#include "MeshGroup.h"
#include "Engine_Define.h"
#include <stack>

BEGIN(Engine)

class CMesh;
class CTexture;
class CCalculator;
END
enum ToolType
{
	TILE, OBJ, ITEM, TOOL_END
};
//struct TileType
//{
//	TCHAR* TileName;
//	int	Tilenum;
//};
class CGui :
    public CGameObject
{
private:
	bool m_bPickTileUse;
	bool m_bPickObjUse;
	bool m_bPickItemUse;
private:
	explicit CGui(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGui(const CGui& rhs);
	virtual ~CGui();

public:
	virtual HRESULT Ready_GameObject()						 override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject()					 override;
	virtual void Render_GameObject()						 override;

private:
	virtual void Free() override;

public:
	void Obj_PickSinglemap(wstring _category);
	void Obj_PickMultimap(wstring vecref);

public:
	void OnSaveData(wstring _file);
	void OnLoadData(wstring _file);

private:
	HRESULT			Add_Component();
	_int			Picking_OnTerrain();
	_vec3			Picking_OnObj();
	void			Multi_map(wstring szProtoName2, int item_current_idx);
private:
	//추후 여러맵 제작을 위해
	vector<CMeshGroup*> m_Map;

public:
	static CGui* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	vector< wstring>* Get_vecTag(wstring _category);

	void Set_vecTile(TileType* _tile) { m_TileType.push_back(_tile); }
	void Set_vecDTile(TileType* _tile) { m_DTileType.push_back(_tile); }
	void Set_MapMesh(vector<Engine::CMesh*>* _pvecMeshGroupCom) { m_MeshGroupCom = _pvecMeshGroupCom; }
	void Set_MapTextrue(vector<Engine::CTexture*>* _pvecTextureGroupCom) { m_TextureGroupCom = _pvecTextureGroupCom; }
	void Set_MapDMesh(vector<Engine::CMesh*>* _pvecDMeshGroupCom) { m_DMeshGroupCom = _pvecDMeshGroupCom; }
	void Set_MapDTextrue(vector<Engine::CTexture*>* _pvecDTextureGroupCom) { m_DTextureGroupCom = _pvecDTextureGroupCom; }

	void Set_bMap(bool* _b) { m_bMap = _b; }
	void Set_bDecal(bool* _b) { m_bDecal = _b; }

private:
	vector<Engine::CMesh*>* m_MeshGroupCom;
	vector<Engine::CTexture*>* m_TextureGroupCom;
	vector<Engine::CMesh*>* m_DMeshGroupCom;
	vector<Engine::CTexture*>* m_DTextureGroupCom;
	Engine::CCalculator* m_pCalculatorCom;

	map< wstring, vector< wstring>* >		m_mapObj;//first=종류,second=종류에 맞는 오브젝트 프로토 벡터
	vector<wstring> m_mapTile;
	vector<TileType*> m_TileType;	
	vector<TileType*> m_DTileType;	
	std::stack<CGameObject*> stkGameObj;

	 bool* m_bMap ;
	 bool* m_bDecal ;
	 int item_set_idx;
	 int item_i_idx; // Here we store our selection data as an index.	
	 ToolType tooltype;
	 _float m_RotY;
	 _float m_PosY;

private:
	_int m_PickNum;
	_int m_PickCom;
	_int m_SinglePickTex;
	wstring	m_Pickname;
	wstring	m_Pick;
};

