#include "stdafx.h"
#include "Gui.h"


#include "Export_Utility.h"
#include "Export_System.h"

#include "imgui.h"
#include"imgui_impl_dx9.h"
#include"imgui_impl_win32.h"

#include "MeshGroup.h"
#include "Yeti.h"
#include "Looker.h"
#include "Flamer.h"
#include "Item.h"
#include "Toggle.h"
#include "Door.h"
#include "Box.h"
#include "Thorn.h"
#include "Item.h"
#include "Shambler.h"
#include "Thingy.h"
#include"DecorativeProp.h"

CGui::CGui(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_PickNum(-1), m_bPickTileUse(FALSE), m_bPickObjUse(FALSE), m_bPickItemUse(FALSE),m_bMap(nullptr),m_bDecal(nullptr),
	m_MeshGroupCom(nullptr), m_TextureGroupCom(nullptr), m_DMeshGroupCom(nullptr), m_DTextureGroupCom(nullptr), item_set_idx(0), item_i_idx(0), tooltype(TOOL_END)
	,m_RotY(0),m_PosY(0)
{

}

CGui::CGui(const CGui& rhs)
	: Engine::CGameObject(rhs)
{
}

CGui::~CGui()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();	
	CGui::Free();
}

HRESULT CGui::Ready_GameObject()
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//single
	vector< wstring>* vecmoster = new vector< wstring>;
	vecmoster->push_back(L"Proto_YetiIdleTexture");	
	vecmoster->push_back(L"Proto_LookerIdleTexture");	
	vecmoster->push_back(L"Proto_FlamerIdleTexture");	
	vecmoster->push_back(L"Proto_ShamblerNormal_IdleTexture");		
	vecmoster->push_back(L"Proto_Thingy_IdleTexture");		
	m_mapObj.insert({L"monster", vecmoster });

	vector< wstring>* vectoggle = new vector< wstring>;
	vectoggle->push_back(L"Proto_ToggleNormalTexture");
	vectoggle->push_back(L"Proto_ToggleRedTexture");
	vectoggle->push_back(L"Proto_ToggleYellowTexture");
	vectoggle->push_back(L"Proto_ToggleBlueTexture");
	m_mapObj.insert({ L"toggle", vectoggle });
	
	vector< wstring>* vecdoor = new vector< wstring>;
	//vecdoor->push_back(L"Proto_DoorNormalTexture");
	vecdoor->push_back(L"Proto_DoorGreyTexture");
	vecdoor->push_back(L"Proto_DoorRedTexture");
	vecdoor->push_back(L"Proto_DoorYellowTexture");
	vecdoor->push_back(L"Proto_DoorBlueTexture");
	m_mapObj.insert({ L"door", vecdoor });

	vector< wstring>* vecbox = new vector< wstring>;
	vecbox->push_back(L"Proto_BoxNormalTexture");
	vecbox->push_back(L"Proto_BoxExplTexture");
	vecbox->push_back(L"Proto_PillarTexture");
	m_mapObj.insert({ L"box", vecbox });

	vector< wstring>* vecthorn = new vector< wstring>;
	vecthorn->push_back(L"Proto_ThornTexture");
	m_mapObj.insert({ L"thorn", vecthorn });

	vector< wstring>* vecdeco = new vector< wstring>;
	vecdeco->push_back(L"Proto_CorpseTexture");
	vecdeco->push_back(L"Proto_LampTexture");
	vecdeco->push_back(L"Proto_LightpostTexture");
	vecdeco->push_back(L"Proto_LampTexture");	
	m_mapObj.insert({ L"deco", vecdeco });

	//multi
	vector< wstring>* vecbullet = new vector< wstring>;
	vecbullet->push_back(L"Proto_ItemTexture_Bullet");
	m_mapObj.insert({ L"bullet", vecbullet });

	vector< wstring>* vecexp = new vector< wstring>;
	vecexp->push_back(L"Proto_ItemTexture_Exp");
	m_mapObj.insert({ L"exp", vecexp });

	vector< wstring>* veckey = new vector< wstring>;
	veckey->push_back(L"Proto_ItemTexture_Key");
	m_mapObj.insert({ L"key", veckey });

	vector< wstring>* vechealth = new vector< wstring>;
	vechealth->push_back(L"Proto_ItemTexture_Health");
	m_mapObj.insert({ L"health", vechealth });

	vector< wstring>* vecmana = new vector< wstring>;
	vecmana->push_back(L"Proto_ItemTexture_Mana");
	m_mapObj.insert({ L"mana", vecmana });

	return S_OK;
}

_int CGui::Update_GameObject(const _float& fTimeDelta)
{
	m_Pickname = wstring();
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();




	
	ImGui::Begin("MapTool");

	ImGui::Checkbox("Map", m_bMap);
	ImGui::Checkbox("Decal", m_bDecal);


	if (ImGui::TreeNode("Tile boxes"))
	{
		tooltype = TILE;
		const char* items[] = { "floor", "wall", "cell","glass","frame","enter","water","bosswall"};
		if (ImGui::BeginListBox("Tile"))
		{
			static int item_c_idx = 0; // Here we store our selection data as an index.	
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)	
			{
				const bool is_selected = (item_c_idx == n);
				if (ImGui::Selectable(items[n], is_selected))	
					item_c_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();

			TCHAR szProtoName2[128] = L"Proto_TerrainTexture";	

			Multi_map(wstring(szProtoName2) , item_c_idx);
		}
			
		ImGui::NewLine();
		if (m_bPickTileUse)
		{
			ImGui::Text("Picking Tile Data.");
			m_bPickObjUse = FALSE;
			m_bPickItemUse = FALSE;
		}

		ImGui::TreePop();
	}
	
	if (ImGui::TreeNode("Obj boxes"))
	{
		tooltype = OBJ;
		const char* items[] = { "monster", "toggle","door","box","thorn","deco"};
		if (ImGui::BeginListBox("Object"))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_set_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
					item_set_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
		if (item_set_idx ==0)
		{
			Obj_PickSinglemap(L"monster");		
		}
		else if (item_set_idx ==1)
		{
			Obj_PickSinglemap(L"toggle");
		}
		else if (item_set_idx == 2)
		{
			Obj_PickSinglemap(L"door");
		}
		else if (item_set_idx == 3)
		{
			Obj_PickSinglemap(L"box");
		}
		else if (item_set_idx == 4)
		{
			Obj_PickSinglemap(L"thorn");
		}
		else if (item_set_idx == 5)
		{
			Obj_PickSinglemap(L"deco");	
		}

		ImGui::NewLine();
		if (m_bPickObjUse)
		{
			ImGui::Text("Picking Obj Data.");
			m_bPickTileUse=FALSE;
			m_bPickItemUse=FALSE;
		}
		ImGui::InputFloat("input RotateY", &m_RotY);
		ImGui::InputFloat("input PositionY", &m_PosY);
		ImGui::TreePop();
	}
	
	if (ImGui::TreeNode("item boxes"))
	{
		tooltype = ITEM;
		const char* items[] = { "bullet", "exp","health","mana","key"};
		if (ImGui::BeginListBox("Item"))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_i_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
					item_i_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}

		if (item_i_idx == 0)
		{
			Obj_PickMultimap(L"Proto_ItemTexture_Bullet");
		}
		else if (item_i_idx == 1)
		{
			Obj_PickMultimap(L"Proto_ItemTexture_Exp");
		}
		else if (item_i_idx == 2)
		{
			Obj_PickMultimap(L"Proto_ItemTexture_Health");
		}
		else if (item_i_idx == 3)
		{
			Obj_PickMultimap(L"Proto_ItemTexture_Mana");
		}
		else if (item_i_idx == 4)
		{
			Obj_PickMultimap(L"Proto_ItemTexture_Key");
		}


		ImGui::NewLine();
		 if (m_bPickItemUse)
		{
			ImGui::Text("Picking Item Data.");
			m_bPickTileUse=FALSE;
			m_bPickObjUse=FALSE;
		}

		ImGui::TreePop();
	}

	if (ImGui::Button("Back"))
	{
		CGameObject* pGameObject = stkGameObj.top();
		stkGameObj.pop();
		auto lambda = [&pGameObject](std::pair<const std::wstring, CGameObject*> p1) {if (p1.second == pGameObject) return true; else return false; };
		
		multimap<wstring,CGameObject*>* multiM = dynamic_cast<CLayer*>(CManagement::GetInstance()->Get_Layer(L"GameData"))->Get_Map();
		auto& iter = find_if(multiM->begin(), multiM->end(), lambda);
		if (iter == multiM->end())
		{
			MSG_BOX("invalid iterator");
			return 0;
		}

		Safe_Release(pGameObject);
		multiM->erase(iter);

	}
	ImGui::NewLine();





	if (ImGui::Button("Save"))
		(OnSaveData(L"../Bin/Resource/Data/Obj.txt"));
	if (ImGui::Button("Load"))	
		(OnLoadData(L"../Bin/Resource/Data/Obj.txt"));

	ImGui::End();


	if (Engine::Get_DIMouseState(DIM_LB) & 0x80&& m_bPickTileUse)
	{
			if (m_Pickname.length() > 0)
			{
				int index = -1;
				index = Picking_OnTerrain();//47번 옵젝이 터레인
				//만약 피킹을 성공하면 메쉬에 텍스처 컴퍼넌트와 텍스처 넘버를 두개다 부여해야됨
				if (index > -1 && m_PickNum > -1)
				{
					if (*m_bMap)
					{
						Engine::Safe_Release((*m_TextureGroupCom)[index]);
							(*m_TextureGroupCom)[index] = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_Pick));
						*((*m_TextureGroupCom)[index]->Get_Texture()) = (_uint)m_PickNum;
						m_TileType[index]->TileName = wstring(m_Pick);
						m_TileType[index]->Tilenum = m_PickNum;
					}
					else if (*m_bDecal)
					{
						Engine::Safe_Release((*m_DTextureGroupCom)[index]);	
						(*m_DTextureGroupCom)[index] = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_Pick));
						*((*m_DTextureGroupCom)[index]->Get_Texture()) = (_uint)m_PickNum;
						m_DTileType[index]->TileName = wstring(m_Pick);
						m_DTileType[index]->Tilenum = m_PickNum;
					}
					m_bPickTileUse = FALSE;
				}
			}

	}
	else if ((Engine::Get_DIMouseState(DIM_LB) & 0x80) && m_bPickObjUse)
	{
		_vec3 position = { 0,0,0 };
		position = Picking_OnObj();//47번 옵젝이 터레인
		//만약 피킹을 성공하면 메쉬에 텍스처 컴퍼넌트와 텍스처 넘버를 두개다 부여해야됨
		if (position != _vec3(0,0,0))
		{
			if (*m_bMap)	
			{
				switch (item_set_idx)
				{
				case 0://몬스터
					switch (m_PickNum)
						{
							Engine::CGameObject* pGameObject;

							case 0:
								pGameObject = CYeti::Create(m_pGraphicDev);
								NULL_CHECK_RETURN(pGameObject, E_FAIL);
								CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"Yeti", pGameObject);
								dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
								stkGameObj.push(pGameObject);
								break;

							case 1:
								pGameObject = CLooker::Create(m_pGraphicDev);
								NULL_CHECK_RETURN(pGameObject, E_FAIL);
								CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"Looker", pGameObject);
								dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
								stkGameObj.push(pGameObject);
								break;

							case 2:
								pGameObject = CFlamer::Create(m_pGraphicDev);	
								NULL_CHECK_RETURN(pGameObject, E_FAIL);
								CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"Flamer", pGameObject);
								dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
								stkGameObj.push(pGameObject);
								break;

							case 3:
								pGameObject = CShambler::Create(m_pGraphicDev);
								NULL_CHECK_RETURN(pGameObject, E_FAIL);
								CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"Shambler", pGameObject);
								dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
								stkGameObj.push(pGameObject);
								break;

							case 4:
								pGameObject = CThingy::Create(m_pGraphicDev);
								NULL_CHECK_RETURN(pGameObject, E_FAIL);
								CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"Thingy", pGameObject);
								dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
								stkGameObj.push(pGameObject);
								break;
						}
					break;
				case 1://토글
					switch (m_PickNum)
					{
						Engine::CGameObject* pGameObject;
						if (m_PosY!=0.f)
						{
							position.y = m_PosY;
						}
					case 0:
						pGameObject = CToggle::Create(m_pGraphicDev, TOG_NORMAL);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"TOG_NORMAL", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						if(m_RotY!=0)
							dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Rotation(ROT_Y, D3DXToRadian(m_RotY));
						stkGameObj.push(pGameObject);
						break;
					case 1:
						pGameObject = CToggle::Create(m_pGraphicDev, TOG_RED);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"TOG_RED", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						if (m_RotY != 0)
							dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Rotation(ROT_Y, D3DXToRadian(m_RotY));
						stkGameObj.push(pGameObject);
						break;

					case 2:
						pGameObject = CToggle::Create(m_pGraphicDev, TOG_YELLOW);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"TOG_YELLOW", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						if (m_RotY != 0)
							dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Rotation(ROT_Y, D3DXToRadian(m_RotY));
						stkGameObj.push(pGameObject);
						break;
					case 3:
						pGameObject = CToggle::Create(m_pGraphicDev, TOG_BLUE);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"TOG_BLUE", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						if (m_RotY != 0)
							dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Rotation(ROT_Y, D3DXToRadian(m_RotY));
						stkGameObj.push(pGameObject);
						break;
					}
					m_RotY = 0.f;
					m_PosY = 0.f;
					break;
				case 2://문
					switch (m_PickNum)
					{
						Engine::CGameObject* pGameObject;

					case 0:
						pGameObject = CDoor::Create(m_pGraphicDev, DOOR_GREY);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"DOOR_GREY", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						if (m_RotY != 0)
							dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Rotation(ROT_Y, D3DXToRadian(m_RotY));
						stkGameObj.push(pGameObject);
						break;
						
					case 1:
						pGameObject = CDoor::Create(m_pGraphicDev, DOOR_RED);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"DOOR_RED", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						if (m_RotY != 0)
							dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Rotation(ROT_Y, D3DXToRadian(m_RotY));	
						stkGameObj.push(pGameObject);
						break;

					case 2:
						pGameObject = CDoor::Create(m_pGraphicDev, DOOR_YELLOW);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"DOOR_YELLOW", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						if (m_RotY != 0)
							dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Rotation(ROT_Y, D3DXToRadian(m_RotY));	
						stkGameObj.push(pGameObject);
						break;
					case 3:
						pGameObject = CDoor::Create(m_pGraphicDev, DOOR_BLUE);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"DOOR_BLUE", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						if (m_RotY != 0)
							dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Rotation(ROT_Y, D3DXToRadian(m_RotY));
						stkGameObj.push(pGameObject);
						break;
					}
					break;
				case 3://박스&기둥
					switch (m_PickNum)
					{
						Engine::CGameObject* pGameObject;

					case 0:
						pGameObject = CBox::Create(m_pGraphicDev, BOX_NORMAL);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"BOX_NORMAL", pGameObject);
						position.y = 1.5f;
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;

					case 1:
						pGameObject = CBox::Create(m_pGraphicDev, BOX_EXPL);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"BOX_EXPL", pGameObject);
						position.y = 1.5f;
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;
					case 2:
						pGameObject = CBox::Create(m_pGraphicDev, PILLAR);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"PILLAR", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;
					}
					break;
				case 4://가시
					switch (m_PickNum)
					{
						Engine::CGameObject* pGameObject;
					case 0:
						pGameObject = CThorn::Create(m_pGraphicDev);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"Thorn", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;
					}
					break;
				case 5://데코
					switch (m_PickNum)
					{
						Engine::CGameObject* pGameObject;

					case 0:
						pGameObject = CDecorativeProp::Create(m_pGraphicDev, (DECOID)0);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"Corpse", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;

					case 1:
						pGameObject = CDecorativeProp::Create(m_pGraphicDev, (DECOID)1);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"Lamp", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;

					case 2:
						pGameObject = CDecorativeProp::Create(m_pGraphicDev, (DECOID)2);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"Lightpost", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;
					case 3:
						pGameObject = CDecorativeProp::Create(m_pGraphicDev, (DECOID)3);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"Siren", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;
					}
					break;

				default:
					break;
				}
				
			}
			//Engine::Safe_Release(pGameObject);
			m_bPickObjUse = FALSE;	
		}

	}
	else if ((Engine::Get_DIMouseState(DIM_LB) & 0x80) && m_bPickItemUse)
	{

		_vec3 position = { 0,0,0 };
		position = Picking_OnObj();//47번 옵젝이 터레인
		//만약 피킹을 성공하면 메쉬에 텍스처 컴퍼넌트와 텍스처 넘버를 두개다 부여해야됨
		if (position != _vec3(0, 0, 0))
		{
			if (*m_bMap)
			{
				switch (item_i_idx)
				{	
				case 0://총알
					switch (m_PickNum)
					{
						Engine::CGameObject* pGameObject;

					case 0:
						pGameObject = CItem::Create(m_pGraphicDev, BULLET, 0);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"BULLET1", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;

					case 1:
						pGameObject = CItem::Create(m_pGraphicDev, BULLET, 1);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"BULLET2", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;

					case 2:
						pGameObject = CItem::Create(m_pGraphicDev, BULLET, 2);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"BULLET3", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;
					case 3:
						pGameObject = CItem::Create(m_pGraphicDev, BULLET, 3);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"BULLET4", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;
					}
					break;
				case 1://경험치
					switch (m_PickNum)
					{
						Engine::CGameObject* pGameObject;

					case 0:
						pGameObject = CItem::Create(m_pGraphicDev, EXP, 0);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"EXP1", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;
					case 1:
						pGameObject = CItem::Create(m_pGraphicDev, EXP, 1);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"EXP2", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;

					case 2:
						pGameObject = CItem::Create(m_pGraphicDev, EXP, 2);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"EXP3", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;
					}
					break;
				case 2://체력
					switch (m_PickNum)
					{
						Engine::CGameObject* pGameObject;

					case 0:
						pGameObject = CItem::Create(m_pGraphicDev, HEALTH, 0);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"HEALTH1", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;

					case 1:
						pGameObject = CItem::Create(m_pGraphicDev, HEALTH, 1);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"HEALTH2", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;
					}
					break;
				case 3://마나
					switch (m_PickNum)
					{
						Engine::CGameObject* pGameObject;

					case 0:
						pGameObject = CItem::Create(m_pGraphicDev, MANA, 0);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"MANA1", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;

					case 1:
						pGameObject = CItem::Create(m_pGraphicDev, MANA, 1);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"MANA2", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;

					}
					break;
				case 4://열쇠
					switch (m_PickNum)
					{
						Engine::CGameObject* pGameObject;

					case 0:
						pGameObject = CItem::Create(m_pGraphicDev, KEY, 0);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"KEY1", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;

					case 1:
						pGameObject = CItem::Create(m_pGraphicDev, KEY, 1);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"KEY2", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;

					case 2:
						pGameObject = CItem::Create(m_pGraphicDev, KEY, 2);
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						CManagement::GetInstance()->Get_Layer(L"GameData")->Add_GameObject(L"KEY3", pGameObject);
						dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(position);
						stkGameObj.push(pGameObject);
						break;
					}
					break;

				
				default:
					break;
				}

			}
			//Engine::Safe_Release(pGameObject);
			m_bPickItemUse = FALSE;
		}

	}
	else if ((Engine::Get_DIMouseState(DIM_LB) & 0x80) && !m_bPickItemUse && !m_bPickObjUse && !m_bPickTileUse)
	{

	}

				

			
		
	

	
	
	

	Engine::Add_RenderGroup(RENDER_UI, this);

	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CGui::LateUpdate_GameObject()
{
	__super::LateUpdate_GameObject();
}

void CGui::Render_GameObject()
{
	ImGui::Render();

	
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

}

HRESULT CGui::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_Calculator", pComponent });


	return S_OK;
}


CGui* CGui::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGui* pInstance = new CGui(pGraphicDev);
	
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Engine::Safe_Release(pInstance);
		MSG_BOX("Terrain Create Failed");
		return nullptr;
	}

	//ImGui
	IMGUI_CHECKVERSION();	
	ImGui::CreateContext();	
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();	
	ImGui_ImplWin32_Init(g_hWnd);	
	ImGui_ImplDX9_Init(pGraphicDev);	

	return pInstance;
}

vector< wstring>* CGui::Get_vecTag(wstring _category)
{
	auto& iter = find_if(m_mapObj.begin(), m_mapObj.end(), CTag_Finder2(_category));

	if (iter == m_mapObj.end())
		return nullptr;

	return (iter->second);
}

void CGui::Free()
{

	for (auto iter : m_mapObj)
	{
		iter.second->erase(iter.second->begin(), iter.second->end());
		iter.second = nullptr;
	}
	m_mapObj.clear();

	CGameObject::Free();
}


_int CGui::Picking_OnTerrain()
{

	CTransform* pMeshTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_STATIC, L"GameLogic", L"MeshGroup", L"Proto_Transform"));
	if (*m_bMap)
	{
		return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, (*m_MeshGroupCom), pMeshTransCom);
	}
	else if (*m_bDecal)
	{
		return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, (*m_DMeshGroupCom), pMeshTransCom);
	}
}

_vec3 CGui::Picking_OnObj()
{

	CTransform* pMeshTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(ID_STATIC, L"GameLogic", L"MeshGroup", L"Proto_Transform"));

	return m_pCalculatorCom->Picking_OnObj(g_hWnd, (*m_MeshGroupCom), pMeshTransCom);
	
}

//각 프로토 전체다 꺼내 쓸때 사용
void CGui::Obj_PickMultimap(wstring vecref)
{


	Engine::CTexture* m_TCom = nullptr;
	m_TCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(vecref));


	for (_uint j = 0; j < m_TCom->Get_TextureVec().size(); j++)
	{
		ImTextureID my_tex_id = m_TCom->Get_TextureVec()[j];

		float my_tex_w = 64.f;
		float my_tex_h = 64.f;

		//int d = m_TCom->Get_TextureVec().size();
		ImGui::PushID(j);

		ImVec2 size = ImVec2(32.0f, 32.0f);                         // Size of the image we want to make visible
		ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
		ImVec2 uv1 = ImVec2(my_tex_w / my_tex_w, my_tex_h / my_tex_h);    // UV coordinates for (32,32) in our texture
		ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
		if (ImGui::ImageButton("", my_tex_id, size, uv0, uv1, bg_col, tint_col))
		{
			if (tooltype == OBJ)
				m_bPickObjUse = TRUE;
			else if (tooltype == ITEM)
				m_bPickItemUse = TRUE;
			m_PickNum = j;
		}
		ImGui::PopID();
		ImGui::SameLine();
	}
	ImGui::NewLine();


}

//각 프로토 마다 하나씩 꺼내 쓸때 사용
void CGui::Obj_PickSinglemap(wstring _category)
{
	vector< wstring> vecproto;
	vecproto = *(Get_vecTag(_category));


	Engine::CTexture* m_TCom = nullptr;



	for (_uint i = 0; i < vecproto.size(); i++)
	{
		m_TCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(vecproto[i]));
		float my_tex_w = 64.f;
		float my_tex_h = 64.f;

		//int d = m_TCom->Get_TextureVec().size();
		ImTextureID my_tex_id = m_TCom->Get_TextureVec()[0];
		ImGui::PushID(i);

		ImVec2 size = ImVec2(32.0f, 32.0f);                         // Size of the image we want to make visible
		ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
		ImVec2 uv1 = ImVec2(my_tex_w / my_tex_w, my_tex_h / my_tex_h);    // UV coordinates for (32,32) in our texture
		ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
		if (ImGui::ImageButton("", my_tex_id, size, uv0, uv1, bg_col, tint_col))
		{
			if (tooltype == OBJ)
				m_bPickObjUse = TRUE;
			else if (tooltype == ITEM)
				m_bPickItemUse = TRUE;
			//_tcscpy_s(m_Pickname, _countof(m_Pickname), szProtoName2);
			m_PickNum = i;
		}

		ImGui::PopID();
		ImGui::SameLine();

	}





	ImGui::NewLine();
}

void CGui::Multi_map(wstring szProtoName2, int item_current_idx)
{
	Engine::CTexture* m_TCom = nullptr;
	wstring Pick = L"Proto_TerrainTexture%d";	
	m_Pick = L"Proto_TerrainTexture" + std::to_wstring(item_current_idx);	


	m_Pickname = szProtoName2+ std::to_wstring(item_current_idx);
	m_TCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_Pickname));
	float my_tex_w = 64.f;
	float my_tex_h = 64.f;
	for (_uint i = 0; i < m_TCom->Get_TextureVec().size(); i++)
	{
		ImTextureID my_tex_id = m_TCom->Get_TextureVec()[i];
		ImGui::PushID(i);

		ImVec2 size = ImVec2(32.0f, 32.0f);                         // Size of the image we want to make visible
		ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
		ImVec2 uv1 = ImVec2(my_tex_w / my_tex_w, my_tex_h / my_tex_h);    // UV coordinates for (32,32) in our texture
		ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint

		if (ImGui::ImageButton("", my_tex_id, size, uv0, uv1, bg_col, tint_col))
		{
			m_bPickTileUse = TRUE;
			m_PickNum = i;
		}

		ImGui::PopID();
		ImGui::SameLine();

	}
	Engine::Safe_Release(m_TCom);
}


void CGui::OnSaveData(wstring _file)
{
	wofstream		fout;
	fout.open(_file, ios::out);	

	if (!fout.fail())// 개방 성공
	{
		multimap<wstring, CGameObject*>*		Objmap = dynamic_cast<CLayer*>(CManagement::GetInstance()->Get_Layer(L"GameData"))->Get_Map();
		_vec3 pos(0, 0, 0);	
		_vec3 rot(0, 0, 0);	
		float rotY(0);
		fout << m_TileType.size() << endl;
		for (auto& iter : m_TileType)
		{
			fout << iter->TileName<< L"|" << iter->Tilenum << endl;
		}
		fout << m_DTileType.size() << endl;
		for (auto& iter : m_DTileType)
		{
			fout << iter->TileName << L"|" << iter->Tilenum << endl;	
		}
		fout << (*Objmap).size() << endl;
		for (auto iter : (*Objmap))
		{
			if ((iter.first == L"Yeti") || (iter.first == L"Looker") || (iter.first == L"Flamer") || (iter.first == L"Shambler") || (iter.first == L"Thingy"))
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << endl;
			}
			else if (iter.first == L"TOG_NORMAL")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				rot = dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Angle();
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << D3DXToDegree(rot.y) <<L"|"<<3 << endl;
			}
			else if (iter.first == L"TOG_RED")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				rot = dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Angle();
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << D3DXToDegree(rot.y) << L"|" << 0 << endl;
			}
			else if (iter.first == L"TOG_YELLOW")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				rot = dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Angle();
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << D3DXToDegree(rot.y) << L"|" << 1 << endl;
			}
			else if (iter.first == L"TOG_BLUE")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				rot=dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Angle();
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << D3DXToDegree(rot.y) << L"|" << 2 << endl;
			}
			else if (iter.first == L"DOOR_GREY")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				rot = dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Angle();
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << D3DXToDegree(rot.y) << L"|" << 3 << endl;
			}
			else if (iter.first == L"DOOR_RED")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				rot = dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Angle();
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << D3DXToDegree(rot.y) << L"|" << 0 << endl;
			}
			else if (iter.first == L"DOOR_YELLOW")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				rot = dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Angle();
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << D3DXToDegree(rot.y) << L"|" << 1 << endl;
			}
			else if (iter.first == L"DOOR_BLUE")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				rot = dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Angle();
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << D3DXToDegree(rot.y) << L"|" << 2 << endl;
			}
			else if (iter.first == L"BOX_NORMAL")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z  << L"|" << 0 << endl;
			}
			else if (iter.first == L"BOX_EXPL") 
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z  << L"|" << 1 << endl;
			}
			else if (iter.first == L"PILLAR")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 2 << endl;
			}
			else if (iter.first == L"Thorn")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 3 << endl;
			}
			else if (iter.first == L"BULLET1")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 1 << L"|" << 0 << endl;
			}
			else if (iter.first == L"BULLET2")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 1 << L"|" << 1 << endl;
			}
			else if (iter.first == L"BULLET3")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 1 << L"|" << 2 << endl;
			}
			else if (iter.first == L"BULLET4")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 1 << L"|" << 3 << endl;
			}
			else if (iter.first == L"EXP1")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 2 << L"|" << 0 << endl;
			}
			else if (iter.first == L"EXP2")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 2 << L"|" << 1 << endl;
			}
			else if (iter.first == L"EXP3")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 2 << L"|" << 2 << endl;
			}
			else if (iter.first == L"HEALTH1")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 3 << L"|" << 0 << endl;
			}
			else if (iter.first == L"HEALTH2")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 3 << L"|" << 1 << endl;
			}
			else if (iter.first == L"MANA1")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 5 << L"|" << 0 << endl;
			}
			else if (iter.first == L"MANA2")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 5 << L"|" << 1 << endl;
			}
			else if (iter.first == L"KEY1")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 4 << L"|" << 0 << endl;
			}
			else if (iter.first ==L"KEY2")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 4 << L"|" << 1 << endl;
			}
			else if (iter.first == L"KEY3")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 4 << L"|" << 2 << endl;
			}
			else if (iter.first == L"Corpse")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 0 << endl;
			}	
			else if (iter.first == L"Lamp")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 1 << endl;
			}
			else if (iter.first == L"Lightpost")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 2 << endl;
			}
			else if (iter.first == L"Siren")
			{
				dynamic_cast<CTransform*>(iter.second->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Get_Info(INFO_POS, &pos);
				fout << iter.first << L"|" << pos.x << L"|" << pos.y << L"|" << pos.z << L"|" << 3 << endl;
			}
		}
		fout.close();
	}
}

void CGui::OnLoadData(wstring _file)
{
	CLayer* GData = dynamic_cast<CLayer*>(CManagement::GetInstance()->Get_Layer(L"GameData"));
	multimap<wstring, CGameObject*>* Objmap = GData->Get_Map();
	for_each(Objmap->begin(), Objmap->end(), CDeleteMap());
	Objmap->clear();

	wifstream		fin;

	fin.open(_file);

	if (!fin.fail())
	{
		TCHAR	szName[256] = L"";
		TCHAR	szTilenum[256] = L"";
		TCHAR	szObjKey[256] = L"";
		_vec3	pos(0, 0, 0);
		TCHAR	vx[256] = L"";
		TCHAR	vy[256] = L"";
		TCHAR	vz[256] = L"";
		TCHAR	roty[256] = L"";
		float	Roty = 0;
		int		szTileD = 0;


		wstring	wstrCombined = L"";

		CGameObject* pGameObj = nullptr;

		for (auto iter : m_TileType)
		{
			delete iter;
			iter = nullptr;
		}
		for (auto iter : m_DTileType)
		{
			delete iter;
			iter = nullptr;
		}
		m_TileType.clear();
		m_DTileType.clear();

		for (int i = 0; i < (*m_TextureGroupCom).size(); ++i)
		{
			Safe_Release((*m_TextureGroupCom)[i]);
		}
		for (int i = 0; i < (*m_DTextureGroupCom).size(); ++i)
		{
			Safe_Release((*m_DTextureGroupCom)[i]);
		}
		int i(0), j(0);


		TCHAR sztexGroupSize[256] = L"";

		fin.getline(sztexGroupSize, MAX_PATH);
		int texGroupSize = stoi(sztexGroupSize);

		for (int i = 0; i < texGroupSize; ++i)
		{
			TCHAR szName[256] = L"";
			fin.getline(szName, 256, '|');

			if (wcsncmp(szName, L"Proto", 5) != 0)
			{
				MSG_BOX("GamemData Load Failed...");
				return;
			}

			TCHAR   szTileNum[256] = L"";
			fin.getline(szTileNum, 256);
			int tileNum = stoi(szTileNum);

			CTexture* pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(szName));
			*(pTextureCom->Get_Texture()) = (_uint)tileNum;
			(*m_TextureGroupCom)[i] = pTextureCom;
			TileType* ptile = new TileType;
			ptile->TileName = wstring(szName);
			ptile->Tilenum = tileNum;
			m_TileType.push_back(ptile);
		}

		TCHAR sztexDGroupSize[256] = L"";

		fin.getline(sztexDGroupSize, MAX_PATH);
		int texDGroupSize = stoi(sztexDGroupSize);

		for (int i = 0; i < texDGroupSize; ++i)
		{
			TCHAR szName[256] = L"";
			fin.getline(szName, 256, '|');
			CComponent* pComponent = nullptr;
			CTexture* pTextureCom = nullptr;

			if (wcsncmp(szName, L"Proto", 5) != 0)
			{
				MSG_BOX("GamedData Load Failed...");
				return;
			}

			TCHAR   szTileNum[256] = L"";
			fin.getline(szTileNum, 256);
			int tileNum = stoi(szTileNum);

			pComponent = pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(szName));
			*(pTextureCom->Get_Texture()) = (_uint)tileNum;
			(*m_DTextureGroupCom)[i] = pTextureCom;
			TileType* ptile = new TileType;
			ptile->TileName = wstring(szName);
			ptile->Tilenum = tileNum;
			m_DTileType.push_back(ptile);
		}

		TCHAR szobjGroupSize[256] = L"";

		fin.getline(szobjGroupSize, MAX_PATH);
		int objGroupSize = stoi(szobjGroupSize);

		while (true) 
		{
			TCHAR szName[256] = L"";
			fin.getline(szName, 256, '|');

			if (wcsncmp(szName, L"Yeti", 3) == 0 || wcsncmp(szName, L"Looker", 3) == 0 || wcsncmp(szName, L"Flamer", 3) == 0 || wcsncmp(szName, L"Shambler", 3) == 0|| wcsncmp(szName, L"Thingy", 3) == 0)
			{
				fin.getline(vx, 256, '|');
				fin.getline(vy, 256, '|');
				fin.getline(vz, MAX_PATH);
				pos.x = stof(vx);
				pos.y = stof(vy);
				pos.z = stof(vz);

				if (wcsncmp(szName, L"Yeti", 3) == 0)
					pGameObj = CYeti::Create(m_pGraphicDev);
				else if (wcsncmp(szName, L"Looker", 3) == 0)
					pGameObj = CLooker::Create(m_pGraphicDev);
				else if (wcsncmp(szName, L"Flamer", 3) == 0)
					pGameObj = CFlamer::Create(m_pGraphicDev);	
				else if (wcsncmp(szName, L"Shambler", 3) == 0)
					pGameObj = CFlamer::Create(m_pGraphicDev);
				else if (wcsncmp(szName, L"Thingy", 3) == 0)
					pGameObj = CFlamer::Create(m_pGraphicDev);

				dynamic_cast<CTransform*>(pGameObj->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(pos);
				GData->Add_GameObject(szName, pGameObj);
			}
			else if (wcsncmp(szName, L"TOG_NORMAL", 3) == 0 || wcsncmp(szName, L"TOG_RED", 3) == 0 || wcsncmp(szName, L"TOG_YELLOW", 3) == 0 || wcsncmp(szName, L"TOG_BLUE", 3) == 0
				|| wcsncmp(szName, L"DOOR_GREY", 3) == 0 || wcsncmp(szName, L"DOOR_RED", 3) == 0 || wcsncmp(szName, L"DOOR_YELLOW", 3) == 0 || wcsncmp(szName, L"DOOR_BLUE", 3) == 0)
			{
				fin.getline(vx, 256, '|');
				fin.getline(vy, 256, '|');
				fin.getline(vz, 256, '|');
				fin.getline(roty, 256, '|');
				fin.getline(szObjKey, MAX_PATH);

				pos.x = stof(vx);
				pos.y = stof(vy);
				pos.z = stof(vz);	
				Roty = stof(roty);
				szTileD = stoi(szObjKey);

				if (wcsncmp(szName, L"TOG", 3) == 0)
					pGameObj = CToggle::Create(m_pGraphicDev, (TOGGLEID)szTileD);
				else if (wcsncmp(szName, L"DOOR", 3) == 0)
					pGameObj = CDoor::Create(m_pGraphicDev, (DOORID)szTileD);


				dynamic_cast<CTransform*>(pGameObj->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(pos);
				dynamic_cast<CTransform*>(pGameObj->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Rotation(ROT_Y, D3DXToRadian(Roty));
				GData->Add_GameObject(szName, pGameObj);
			}
			else if (wcsncmp(szName, L"BOX", 3) == 0 || wcsncmp(szName, L"PILLAR", 3) == 0 || wcsncmp(szName, L"Thorn", 3) == 0)
			{
				fin.getline(vx, 256, '|');
				fin.getline(vy, 256, '|');
				fin.getline(vz, 256, '|');
				fin.getline(szObjKey, MAX_PATH);
				pos.x = stof(vx);
				pos.y = stof(vy);
				pos.z = stof(vz);
				szTileD = stoi(szObjKey);

				if (szTileD < 3)
				{
					pGameObj = CBox::Create(m_pGraphicDev, (BOXID)szTileD);
				}
				else if (szTileD == 3)
				{
					pGameObj = CThorn::Create(m_pGraphicDev);
				}


				dynamic_cast<CTransform*>(pGameObj->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(pos);
				GData->Add_GameObject(szName, pGameObj);
			}
			else if (wcsncmp(szName, L"BULLET", 3) == 0 || wcsncmp(szName, L"EXP", 3) == 0 || wcsncmp(szName, L"HEALTH", 3) == 0 || wcsncmp(szName, L"MANA", 3) == 0 || wcsncmp(szName, L"KEY", 3) == 0)
			{
				fin.getline(vx, 256, '|');
				fin.getline(vy, 256, '|');
				fin.getline(vz, 256, '|');
				fin.getline(szObjKey, 256, '|');
				fin.getline(szTilenum, MAX_PATH);
				pos.x = stof(vx);
				pos.y = stof(vy);
				pos.z = stof(vz);
				int ID = stoi(szObjKey);
				int num = stoi(szTilenum);
				pGameObj = CItem::Create(m_pGraphicDev, (ITEMTYPE)ID, num);
				dynamic_cast<CTransform*>(pGameObj->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(pos);
				GData->Add_GameObject(szName, pGameObj);
			}
			else if (wcsncmp(szName, L"Corpse", 3) == 0 || wcsncmp(szName, L"Lamp", 3) == 0 || wcsncmp(szName, L"Lightpost", 3) == 0 || wcsncmp(szName, L"Siren", 3) == 0)
			{
				fin.getline(vx, 256, '|');
				fin.getline(vy, 256, '|');
				fin.getline(vz, 256, '|');
				fin.getline(szObjKey, MAX_PATH);
				pos.x = stof(vx);
				pos.y = stof(vy);
				pos.z = stof(vz);
				szTileD = stoi(szObjKey);


				pGameObj = CDecorativeProp::Create(m_pGraphicDev, (DECOID)szTileD);



				dynamic_cast<CTransform*>(pGameObj->Get_Component(ID_DYNAMIC, L"Proto_Transform"))->Set_Pos(pos);
				GData->Add_GameObject(szName, pGameObj);
			}

			

			if (fin.eof())
				break;
		}

		fin.close();
	}
}




