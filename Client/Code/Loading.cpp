#include "stdafx.h"
#include "..\Header\Loading.h"
#include "Export_System.h"
#include "Export_Utility.h"


CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_szLoading, sizeof(m_szLoading));
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL,	// 디폴트 보안 속성
		0,		// 디폴트 스택 사이즈(1바이트)
		Thread_Main, // 동작 시킬 쓰레드 함수 이름(__stdcall 규약)
		this,		// 쓰레드 함수의 매개 변수로 전달할 값
		0,			// 쓰레드 생성 및 실행을 조절하기 위한 FLAG
		NULL);		// 쓰레드 ID 반환

	m_eID = eID;

	return S_OK;
}

_uint CLoading::Loading_ForStage()
{
	//Engine::LoadFbx(L"../Bin/Resource/Mesh/Combined Mesh_02a.fbx", L"../Bin/Resource/Mesh/Combined Mesh_02b.fbx");
	Engine::LoadFbx(L"../Bin/Resource/Mesh/ii.fbx", L"../Bin/Resource/Mesh/iid.fbx");
	//Engine::LoadFbx(L"../Bin/Resource/Mesh/boss.fbx", L"../Bin/Resource/Mesh/Dboss.fbx");

	Add_Environmental_Things();
	Add_Map_Things();

	Add_Player_Things();
	Add_PropTexture();
	Add_MonsterTexture();
	Add_MonsterBulletTexture();
	Add_Items();




	m_bFinish = true;

	lstrcpy(m_szLoading, L"Loading Complete");

	return 0;
}

unsigned int CLoading::Thread_Main(void* pArg)
{
	CLoading* pLoading = reinterpret_cast<CLoading*>(pArg);

	_uint iFlag(0);

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_BOSS:
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	//_endthreadex(0);

	return iFlag;
}

_uint CLoading::Add_PropTexture()
{

	//Prop 텍스처 추가

	//Door
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DoorNormalTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Doors/texture_antarctic_blastdoor_0000.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DoorGreyTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Doors/texture_antarctic_submarine_door_0003.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DoorRedTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Doors/texture_antarctic_submarine_door_0000.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DoorYellowTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Doors/texture_antarctic_submarine_door_0001.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DoorBlueTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Doors/texture_antarctic_submarine_door_0002.png")), E_FAIL);

	//Portal
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PortalTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/portal_entry_dark_blue000%d.png", 8)), E_FAIL);

	//Exit
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ExitTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/wormhole000%d.png", 4)), E_FAIL);

	//Toggle
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ToggleNormalTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Toggles/key_button_normal000%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ToggleRedTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Toggles/key_button_red000%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ToggleYellowTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Toggles/key_button_yellow000%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ToggleBlueTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/Toggles/key_button_blue000%d.png", 2)), E_FAIL);

	//Box
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BoxNormalTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/box000%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BoxExplTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/box_expl000%d.png", 2)), E_FAIL);

	//Pillar
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PillarTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/1broken_base_pillar000%d.png", 6)), E_FAIL);

	//Thorn
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ThornTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/ActiveObj/thorn_broken000%d.png", 4)), E_FAIL);

	//Lamp
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LampTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/InActiveObj/lamp.png")), E_FAIL);

	//Lightpost
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LightpostTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/InActiveObj/lightpost.png")), E_FAIL);

	//Corpse
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CorpseTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/InActiveObj/corpse.png")), E_FAIL);

	return S_OK;
}

_uint CLoading::Add_MonsterTexture()
{

	//Yeti
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_YetiIdleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Yeti/yeti_idle.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_YetiRunTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Yeti/yeti_run%d.png", 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_YetiAtkTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Yeti/yeti_slash%d.png", 17)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_YetiHitTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Yeti/yeti_hit.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_YetiDeadTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Yeti/yeti_death_run%d.png", 13)), E_FAIL);

	//Looker
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LookerIdleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Looker/looker_back%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LookerRunTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Looker/looker_float%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LookerAtkTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Looker/looker_attack%d.png", 17)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LookerHitTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Looker/looker_hit.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LookerDeadTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Looker/looker_death%d.png", 15)), E_FAIL);

	//Flamer
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlamerIdleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Flamerguy/flamerguy_back.png", 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlamerRunTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Flamerguy/flamerguy_walk%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlamerAtkTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Flamerguy/flamerguy_attack%d.png", 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlamerHitTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Flamerguy/flamerguy_hit.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlamerDeadTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Flamerguy/death_fire%d.png", 22)), E_FAIL);

	//Shambler
//Shambler_Normal
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShamblerNormal_IdleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL,
		L"../Bin/Resource/Object/MonsterObj/Newshambler/Newshambler_Normal/newshambler_back.png")), E_FAIL);


	//ExitMonster
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ExitMonsterIdleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Exit/exit_idle%d.png", 6)), E_FAIL);

	//Thingy
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_IdleTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_idle.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_HitTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_hit.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_MorphTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_morph%d.png", 18)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_WalkTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_walk%d.png", 6)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_DeathTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_death%d.png", 10)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_AttackATexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_attack_A%d.png", 18)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_AttackBTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_attack_B%d.png", 28)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_AttackTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_attack%d.png", 14)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Thingy_SpikeTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterObj/Thingy/thingy_spikes%d.png", 20)), E_FAIL);

	return S_OK;
}

_uint CLoading::Add_MonsterBulletTexture()
{
	//Looker Bullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LookerBulletTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterBulletObj/tentacle_monster_projectile%d.png", 5)), E_FAIL);

	//Flamer Bullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlamerBulletTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Object/MonsterBulletObj/fire_monster_%d.png", 8)), E_FAIL);

	return S_OK;
}

_uint CLoading::Add_Player_Things()
{

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Claymore", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Claymore/Claymore%d.png", 14)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Minigun", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Minigun/Minigun%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Revolver", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Revolver/Revolver%d.png", 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Rocket_Launcher", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Rocket_Launcher/Rocket_Launcher%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Shotgun", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Shotgun/Shotgun%d.png", 27)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Staff", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Staff/Staff%d.png", 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Reactor", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Reactor/Reactor%d.png", 37)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Bullet", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Weapon/Bullet/Bullet0.png")), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HUD", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/HUD/HUD%d.png", 14)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Face", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Face/Face%d.png", 25)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Key", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Key/Key%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIWeapon", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/UI/Weapon/UIWeapon%d.png", 6)), E_FAIL);

	return _uint();
}

_uint CLoading::Add_Map_Things()
{
	//0-바닥
//1-벽
//2=천장
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture0", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Lab_Terrain/texture_lab_floor_%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture1", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Lab_Terrain/texture_lab_wall_%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture2", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Lab_Terrain/texture_lab_ceiling_%d.png",2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture3", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Lab_Terrain/texture_glass.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture4", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Lab_Terrain/texture_base_doorframe_%d.png",9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture5", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Lab_Terrain/texture_antarctic_%d.png",2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture6", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Lab_Terrain/texture_black_goo000%d.png",4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture7", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Lab_Terrain/texture_boss_wall_%d.png",5)), E_FAIL);

	////아래는 클라꺼
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture0"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Map0/texture_base_floor_1a.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture1"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Map0/texture_antarctic_ceiling_1.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture2"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Map0/texture_antarctic_ceiling_1.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture4"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Map0/texture_rock_wall_1.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture5"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Map0/texture_lab_wall_1_edit.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture6"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Map0/texture_lab_floor_3.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture7"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Map0/texture_lab_wall_1_edit.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture8"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/texture_antarctic_alienwall_02.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture9"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/texture_antarctic_blastdoor_2.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture10"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/texture_antarctic_bloodsteps_01a.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture11"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/texture_antarctic_floor_14.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture12"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/texture_antarctic_door_blue.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture13"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/texture_antarctic_floor_snow.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture14"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/texture_antarctic_wall_base_1.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture15"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/TEXTURE_CELLDOOR_RED.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture16"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Map0/texture_lab_wall_1.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture17"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/texture_antarctic_enter.png")), E_FAIL); //enter
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture18"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/texture_city_metal_08.png")), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(L"Proto_TerrainTexture19"), CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/texture_lab_floor_3.png")), E_FAIL);

	const std::vector<Engine::FbxMeshInfo> tmpMeshGroup = Engine::GetMeshGroup();

	const _tchar* szProtoName = L"Proto_Mesh%d";

	for (_uint i = 0; i < tmpMeshGroup.size(); ++i)
	{

		TCHAR	szFileName[128] = L"";

		wsprintf(szFileName, szProtoName, i);

		FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(szFileName), CMesh::Create(m_pGraphicDev, Engine::GetMesh(i),1)), E_FAIL);	
	}

	const std::vector<Engine::FbxMeshInfo> tmpDMeshGroup = Engine::GetDMeshGroup();

	const _tchar* szProtoDName = L"Proto_DMesh%d";

	for (_uint i = 0; i < tmpDMeshGroup.size(); ++i)
	{

		TCHAR	szFileName[128] = L"";

		wsprintf(szFileName, szProtoDName, i);

		FAILED_CHECK_RETURN(Engine::Ready_Proto(wstring(szFileName), CMesh::Create(m_pGraphicDev, Engine::GetDMesh(i), 1)), E_FAIL);	
	}

	return S_OK;
}

_uint CLoading::Add_Environmental_Things()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcCol", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_EffectTexture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", 90)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkyBoxTexture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);

	return S_OK;
}

_uint CLoading::Add_Items()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ItemTexture_Bullet", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Item/Bullet/Bullet%d.png", 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ItemTexture_Exp", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Item/Exp/Exp%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ItemTexture_Health", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Item/Health/Health%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ItemTexture_Key", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Item/Key/Key%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ItemTexture_Mana", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Item/Mana/Mana%d.png", 2)), E_FAIL);


	return S_OK;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading* pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Loading Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);


	Safe_Release(m_pGraphicDev);
}
